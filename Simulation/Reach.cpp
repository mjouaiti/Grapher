//
//  Reach.cpp
//  Graphical
//
//  Created by Melanie Jouaiti on 25/10/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#include "Reach.h"
#include "../common.h"
#include <algorithm>
#include <unistd.h>
double* distance;

#define HMIN 72
#define HMAX 145

/**
 * Reach Constructor
 * @see Reach(std::vector<std::string> jointNames)
 */
Reach::Reach(): Simulation()
{
}

/**
 * Reach Constructor
 * @param jointNames joint names of the robot
 * @see Reach()
 */
Reach::Reach(std::vector<std::string> jointNames): Simulation(jointNames), m_gotBall(false)
{
#ifdef VREP
    m_robot = new Robot_VREP(m_clientID, jointNames);
#else
    m_dt = 0.05;
    m_robot = new Robot();
    m_initPos[2] = .9;
#endif
    distance = new double[2];
    m_startForce = new float[2];
#ifdef VREP
    m_PIDControllers.push_back(PIDController(PID_P, PID_I, PID_D));
    m_PIDControllers.push_back(PIDController(PID_P, PID_I, PID_D));
    m_PIDControllers.push_back(PIDController(PID_P, PID_I, PID_D));
#endif
    
    std::vector<double> var = {0.0, -0.216, 10.0, 0.0, -0.216, 10.0, 0.05, 0.05, 1.2, 1.2};
    std::vector<double> var1 = {0.0, -0.216, 10.0, 0.0, -0.216, 10.0, 0.05, 0.05, 0.8, 0.8};
    std::vector<double> var2 = {0.0, -0.216, 10.0, 0.0, -0.216, 10.0, 0.05, 0.05, 1.8, 1.8};
    double p[5] = {0.35, 3.5, 0.0, 0.002, 0.0};
    m_CPGs[jointNames[0]] = new CPG(p, var);
    m_CPGs[jointNames[1]] = new CPG(p, var1);
    m_CPGs[jointNames[2]] = new CPG(p, var2);
    
    for(auto p: m_CPGs)
        p.second->finalise();
#ifdef  VREP
    int err = simxGetObjectHandle(m_clientID, "JacoHand_attachPoint", &m_handHandle, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Couldn't get object: JacoHand_attachPoint" << std::endl;
    err = simxGetObjectPosition(m_clientID, m_handHandle, -1, m_handPos, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Couldn't get JacoHand_attachPoint position" << std::endl;
    
    err = simxGetObjectHandle(m_clientID, "JacoHand_forceSens1_finger3", &m_sensor_handle, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Couldn't get object: Force_sensor" << std::endl;
#endif
    
    //init();
}

/**
 * Reach Destructor
 */
Reach::~Reach()
{
    simxSetObjectIntParameter(m_clientID, m_sphereHandle, sim_shapeintparam_static, 1, simx_opmode_blocking);
#if defined(__linux__)
    saveToFile("/home/melanie/Documents/log/");
#else
    //saveToFile("/Users/Melanie/Downloads/log/TwoJoints/Reach/");
#endif
}

/**
 * initializes Reach: initializes functions, Integrator and updates forces.
 */
void Reach::init()
{
    //-------------------------- Initialisation
}

bool contact = false;
//static int count = 0;
/**
 * Reach step
 * @return the new variables values for this step (std::vector<double>)
 */
std::vector<double> Reach::step()
{
#ifdef VREP
    m_robot->updateAngularPosition(0);
    m_robot->updateAngularPosition(JOINT_NB1);
    m_robot->updateAngularPosition(JOINT_NB2);
#else
    m_robot->updateAngularPosition();
#endif

    simxGetObjectPosition(m_clientID, m_handHandle, -1, m_handPos, simx_opmode_blocking);
    
    /*if(!m_gotBall && std::abs(m_initPos[2] - m_handPos[2]) < 0.05 &&
       std::abs(m_initPos[1] - m_handPos[1]) < 0.001 && count == 0)
    {
        m_initPos[1] -= 0.3;
        float newPos[3] = {m_initPos[0], m_initPos[1], m_initPos[2]};
        simxSetObjectPosition(m_clientID, m_sphereHandle, -1, newPos, simx_opmode_blocking);
        count++;
    }
    else if(!m_gotBall && std::abs(m_initPos[2] - m_handPos[2]) < 0.05 &&
            std::abs(m_initPos[1] - m_handPos[1]) < 0.001 && count == 1)
    {
        m_initPos[2] -= 0.25;
        float newPos[3] = {m_initPos[0], m_initPos[1], m_initPos[2]};
        simxSetObjectPosition(m_clientID, m_sphereHandle, -1, newPos, simx_opmode_blocking);
        count++;
    }
    else if(!m_gotBall && std::abs(m_initPos[2] - m_handPos[2]) < 0.05 &&
            std::abs(m_initPos[1] - m_handPos[1]) < 0.001 && count == 2)
    {
        m_initPos[1] += 0.3;
        float newPos[3] = {m_initPos[0], m_initPos[1], m_initPos[2]};
        simxSetObjectPosition(m_clientID, m_sphereHandle, -1, newPos, simx_opmode_blocking);
        count++;
    }
    else if(!m_gotBall && std::abs(m_initPos[2] - m_handPos[2]) < 0.05 &&
            std::abs(m_initPos[1] - m_handPos[1]) < 0.01 && count == 3)
    {
        m_initPos[2] += 0.25;
        m_initPos[1] -= 0.15;
        float newPos[3] = {m_initPos[0], m_initPos[1], m_initPos[2]};
        simxSetObjectPosition(m_clientID, m_sphereHandle, -1, newPos, simx_opmode_blocking);
        count++;
    }
    else */if(!m_gotBall && std::abs(m_initPos[2] - m_handPos[2]) < 0.02 &&
            std::abs(m_initPos[1] - m_handPos[1]) < 0.02)// && count == 4)
    {
        m_initPos[0] -= std::abs(m_initPos[0] - m_handPos[0])/4.0;
        float newPos[3] = {m_initPos[0], m_initPos[1], m_initPos[2]};
        simxSetObjectPosition(m_clientID, m_sphereHandle, -1, newPos, simx_opmode_blocking);
    }

    if(!m_gotBall && std::abs(m_initPos[0] - m_handPos[0]) < 0.05)
    {
        dynamic_cast<Robot_VREP*>(m_robot)->closeGripper(m_motorHandles);
        m_gotBall = true;
        //simxSetObjectIntParameter(m_clientID, m_sphereHandle, sim_shapeintparam_static, 0, simx_opmode_blocking);
//        m_CPGs["Mico_joint1"]->reset(20.0, .1);
//        m_CPGs["Mico_joint2"]->reset(20.0, .1);
//        m_CPGs["Mico_joint3"]->reset(20.0, .1);
    }
  
#ifdef VREP
    m_robot->updateAngularVelocity(0);
    m_robot->updateAngularForce(0);
    m_robot->updateAngularVelocity(JOINT_NB1);
    m_robot->updateAngularForce(JOINT_NB1);
    m_robot->updateAngularVelocity(JOINT_NB2);
    m_robot->updateAngularForce(JOINT_NB2);
#else
    m_robot->updateAngularVelocity();
#endif
    
    distance[0] = m_gotBall? 0: (m_initPos[2] - m_handPos[2]);
    distance[1] = m_gotBall? 0: (m_initPos[1] - m_handPos[1]);
    
    //------------------ CPG stuff
    std::vector<double> temp;
    temp.push_back(m_t);
    temp.push_back(0);
    double speed[3];
    int i = 0;
    for(auto p: m_CPGs)
    {
        if(p.first == "Mico_joint2")
            p.second->setInput(distance[0]);
        else if(p.first == "Mico_joint3")
            p.second->setInput(-distance[0]);
        else
            p.second->setInput(-distance[1]);
        float vmes = m_robot->getAngularVelocity(p.first);
        speed[i] = 20 * p.second->step(vmes, m_t, m_dt);
        m_robot->setVelocity(p.first, m_PIDControllers[i].update(m_robot->getAngularPosition(p.first), speed[i]));

        temp.push_back(m_robot->getAngularPosition(p.first));
        temp.push_back(vmes);
        std::vector<double> cpgTmp = p.second->getData();
        temp.insert(temp.end(), cpgTmp.begin(), cpgTmp.end());
        temp.push_back(speed[i]);
        i++;
    }
    
#ifdef VREP
    simxSynchronousTrigger(m_clientID);
#endif
    //------------------ Recording stuff
    std::cout << m_t << ", " << distance[0] << ", " << distance[1] << std::endl;
    
    m_record.insert(m_record.end(), temp.begin(), temp.end());
    m_t += m_dt;
    
    return temp;
}

/**
 * saves the recorded values into a csv file
 * @param path path of the file where the values are saved
 */
void Reach::saveToFile(const std::string& path)
{
    std::string fileName = "Mico_d_r2.csv";
    std::ofstream out(path + fileName, std::ios::out);
    if(!out.is_open())
        std::cerr << "Could not open file: " << path + fileName << std::endl;
    else
    {
        std::string header = "t,I";
        int i = 1;
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

/**
 * runs the whole Reach
 */
void Reach::run()
{
    init();
    while (m_t < 2 && !m_gotBall)
    {
        step();
    }
}

