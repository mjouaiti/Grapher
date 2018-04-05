//
//  Simulation.cpp
//  Handshaking
//
//  Created by Melanie Jouaiti on 30/09/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#include "Simulation.h"
#include "../common.h"

/**
 * Simulation Constructor
 * @see Simulation(std::vector<double> variables, std::vector<std::string> jointNames)
 */
Simulation::Simulation(): m_robot(), m_t(0),
                        m_dt(0.01), m_record(), m_startForce()
                        
{
    
}

/**
 * Simulation Constructor
 * @param jointNames joint names of the robot
 * @see Simulation()
 */
Simulation::Simulation(std::vector<std::string> jointNames): m_robot(), m_t(0),
                                                            m_dt(0.01), m_record(),
                                                            m_startForce()
{
    start();
#ifdef VREP
    
#else
    std::vector<double> newPID {PID_P, PID_I, PID_D};
    //m_robot->setPID(3, newPID);
#endif
}

/**
 * Simulation Destructor
 */
Simulation::~Simulation()
{
    // Stop V-REP Simulation and close the connection
#ifdef VREP
    simxStopSimulation(m_clientID, simx_opmode_blocking);
    int pt;
    simxGetPingTime(m_clientID, &pt);
    simxFinish(m_clientID);
    std::cout << "Connection closed"  << std::endl;
#endif
}

/**
 * Simulation start: Connects to remote API server and retrieves sphere informations
 */
void Simulation::start()
{
#ifdef VREP
    simxFinish(-1);
    m_clientID = simxStart((simxChar*)"127.0.0.1",19997,true,true,2000,5);
    if (m_clientID!=-1)
    {
        std::cout << "Connected to remote API server" << std::endl;
        simxSetFloatingParameter(m_clientID, sim_floatparam_simulation_time_step, m_dt, simx_opmode_oneshot);
        simxStartSimulation(m_clientID, simx_opmode_blocking);

        int err = simxGetObjectHandle(m_clientID, "Sphere", &m_sphereHandle, simx_opmode_blocking);
        if(err != simx_return_ok)
            std::cerr << "Couldn't get object: Sphere" << std::endl;
        //Get initial position of the sphere
        err = simxGetObjectPosition(m_clientID, m_sphereHandle, -1, m_initPos, simx_opmode_blocking);
        if(err != simx_return_ok)
            std::cerr << "Couldn't get sphere position" << std::endl;
        
        err = simxGetObjectHandle(m_clientID, "JacoHand_finger3_motor1", &m_motorHandles[0], simx_opmode_blocking);
        if(err != simx_return_ok)
        std::cerr << "Couldn't get object: JacoHand_finger3_motor1" << std::endl;
        err = simxGetObjectHandle(m_clientID, "JacoHand_finger3_motor2", &m_motorHandles[1], simx_opmode_blocking);
        if(err != simx_return_ok)
        std::cerr << "Couldn't get object: JacoHand_finger3_motor2" << std::endl;
        err = simxGetObjectHandle(m_clientID, "JacoHand_fingers12_motor1", &m_motorHandles[2], simx_opmode_blocking);
        if(err != simx_return_ok)
        std::cerr << "Couldn't get object: JacoHand_fingers12_motor1" << std::endl;
        err = simxGetObjectHandle(m_clientID, "JacoHand_fingers12_motor2", &m_motorHandles[3], simx_opmode_blocking);
        if(err != simx_return_ok)
        std::cerr << "Couldn't get object: JacoHand_fingers12_motor2" << std::endl;
    }
    else
    {
        std::cerr << "Cannot Connect to remote API server";
        exit(0);
    }
#endif
}

/**
 * runs whole Simulation (Do NOT use this function with the Grapher! Use step only)
 */
void Simulation::run()
{
    init();
    while (m_t < TMAX)
    {
        step();
    }
}

/**
 * getter for the current simulation time
 * @return current simulation time (double)
 */
double Simulation::getTime() const
{
    return  m_t;
}

/**
 * getter for the simulation timestep
 * @return the timestep (double)
 */
double Simulation::getTimeStep() const
{
    return  m_dt;
}

/**
 * getter for the VREP Robot
 * @return the VREP Robot (Robot_VREP)
 */
Robot* Simulation::getRobot() const
{
    return m_robot;
}

/**
 * getter for the sphere handle
 * @return the sphere handle (int)
 */
int Simulation::getSphereHandle() const
{
#ifdef VREP
    return m_sphereHandle;
#else
    return -1;
#endif
}

/**
 * getter for the VREP client ID
 * @return the VREP client ID (int)
 */
#ifdef VREP
int Simulation::getClientId() const
{
    return m_clientID;
}
#endif

/**
 * getter for the recorded values
 * @return the recorded values (std::vector<double>)
 */
std::vector<double> Simulation::getRecord() const
{
    return m_record;
}

std::map<std::string, CPG*> Simulation::getCPGs() const
{
    return m_CPGs;
}

