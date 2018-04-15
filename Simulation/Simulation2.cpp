//
//  Simulation2.cpp
//  Handshaking
//
//  Created by Melanie Jouaiti on 07/10/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#include "Simulation2.h"
#include "../common.h"
#include <algorithm>

/**
 * Simulation2 Constructor
 * @see Simulation2(std::vector<std::string> jointNames)
 */
Simulation2::Simulation2(): Simulation()
{
}

/**
 * Simulation2 Constructor
 * @param jointNames joint names of the robot (std::vector<std::string>)
 * @see Simulation2()
 */
Simulation2::Simulation2(std::vector<std::string> jointNames): Simulation(jointNames)
{
    m_startForce = new float[2];
#ifdef VREP
    m_robot = new Robot_VREP(m_clientID, jointNames);
    m_PIDControllers.push_back(PIDController(PID_P, PID_I, PID_D));
    m_PIDControllers.push_back(PIDController(PID_P, PID_I, PID_D));
#endif
    
    std::vector<double> variables{0.0, -0.216, 10.0, 0.0, -0.216, 10.0,
        0.05, 0.05, 0.01, 0.01};
    std::vector<double> variables2{0.0, -0.216, 10.0, 0.0, -0.216, 10.0,
        0.05, 0.05, 0.02, 0.02};
    
    double param[] = {0.35, 3.5, 1.0, 0.000, 0.00000};
    double param2[] = {0.35, 3.5, 1.0, 0.000, 0.00000};
    
    m_CPGs["Mico_joint2"] = new CPG(param, variables);
    m_CPGs["Mico_joint3"] = new CPG(param2, variables2);
    
    coupleCPG(m_CPGs["Mico_joint2"], m_CPGs["Mico_joint3"]);
    for(auto p: m_CPGs)
        p.second->finalise();
    
    int err = simxGetObjectHandle(m_clientID, "Sphere", &m_sphereHandle, simx_opmode_blocking);
    err = simxGetObjectPosition(m_clientID, m_sphereHandle, -1, m_initPos, simx_opmode_blocking);
    if(err != simx_return_ok)
    std::cerr << "Couldn't get sphere position" << std::endl;
    dynamic_cast<Robot_VREP*>(m_robot)->closeGripper(m_motorHandles);
}

/**
 * Simulation2 Destructor
 */
Simulation2::~Simulation2()
{
    simxSetObjectIntParameter(m_clientID, m_sphereHandle, sim_shapeintparam_static, 1, simx_opmode_blocking);
#if defined(__linux__)
    saveToFile("/home/melanie/Documents/log/");
#else
    saveToFile("/Users/Melanie/Downloads/log/TwoJoints/new/");
#endif
}

/**
 * initializes Simulation: initializes functions, Integrator and updates forces.
 */
void Simulation2::init()
{
    m_startForce[0] = 1.24;
    m_startForce[1] = 2.62;
}

/**
 * Simulation step
 * @return the new variables values for this step (std::vector<double>)
 */
std::vector<double> Simulation2::step()
{
    if(m_t > TMAX - 10 - 0.02 && m_t < TMAX - 10)
        dynamic_cast<Robot_VREP*>(m_robot)->openGripper(m_motorHandles);
    if(m_t > TMAX - 10 - 0.01 && m_t < TMAX - 10 + 0.01)
        m_initPos[0] += 0.1;
    float newPos[3] = {m_initPos[0], m_initPos[1], m_initPos[2]};
    newPos[2] += inputSignal(m_t);
    simxSetObjectPosition(m_clientID, m_sphereHandle, -1, newPos, simx_opmode_blocking);

    m_robot->updateAngularPosition(JOINT_NB1);
    m_robot->updateAngularPosition(JOINT_NB2);
    m_robot->updateAngularVelocity(JOINT_NB1);
    m_robot->updateAngularForce(JOINT_NB1);
    m_robot->updateAngularVelocity(JOINT_NB2);
    m_robot->updateAngularForce(JOINT_NB2);
    
    //------------------ CPG stuff
    std::vector<double> temp;
    temp.push_back(m_t);
    temp.push_back(inputSignal(m_t));
    
    float speed[2];
    int i = 0;
    for(auto p: m_CPGs)
    {
        p.second->setInput((m_t < TMAX - 10)?(m_robot->getAngularForce(p.first) - m_startForce[i]): 0);
//        p.second->setInput((m_t < TMAX - 10)?(m_robot->getAngularVelocity(p.first)): 0);
//        p.second->setInput((m_t < TMAX - 10)?(m_robot->getAngularPosition(p.first) - m_startForce[i]): 0);
        float vmes = m_robot->getAngularVelocity(p.first);
        speed[i] =  10.0 * p.second->step(vmes, m_t, m_dt);
        
        m_robot->setVelocity(p.first, m_PIDControllers[i].update(m_robot->getAngularPosition(p.first), speed[i]));
        
        temp.push_back(m_robot->getAngularPosition(p.first));
        temp.push_back(vmes);
        std::vector<double> cpgTmp = p.second->getData();
        temp.insert(temp.end(), cpgTmp.begin(), cpgTmp.end());
        temp.push_back(speed[i]);
        i++;
    }
    
    simxSynchronousTrigger(m_clientID);
    //------------------ Recording stuff
    std::cout << m_t << ", " << std::endl;
    m_record.insert(m_record.end(), temp.begin(), temp.end());
    m_t += m_dt;
    
    return temp;
}

/**
 * saves the recorded values into a csv file
 * @param path path of the file where the values are saved (std::string)
 */
void Simulation2::saveToFile(const std::string& path)
{
    std::string fileName = ".csv";
    std::ofstream out(path + fileName, std::ios::out);
    if(!out.is_open())
        std::cerr << "Could not open file: " << path + fileName << std::endl;
    else
    {
        std::string header = "t,I";
        int i = 2;
        for(auto p: m_CPGs)
        {
            std::string nb = std::to_string(i);
            header += ",pos" + nb + "," + "vel" + nb + "," + "F" + nb + ",";
            header += "v" + nb + "e," + "v" + nb + "f," + "y" + nb + "e," + "y" + nb + "f,";
            header += "ss" + nb + "e," + "ss" + nb + "f," + "Af" + nb + "e," + "Af" + nb + "f,";
            header += "eps" + nb + "e," + "eps" + nb + "f," + "phi" + nb + "e," + "phi" + nb + "f,";
            header += "PF" + nb + "E," + "PF" + nb + "F," + "SN" + nb + ",MN" + nb + "E," + "MN" + nb + "F,";
            header += "s" + nb;
            i++;
        }
        
        int nb = (int)std::count(header.begin(), header.end(), ',') + 1;
        out << header << std::endl;
        for(unsigned int i = 0; i < m_record.size(); i++)
            out << std::to_string(m_record[i]) + (((i+1) % nb == 0)? "\n": ",");
    }
    out.close();
}


