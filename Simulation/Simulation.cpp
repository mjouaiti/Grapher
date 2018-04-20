//
//  Simulation.cpp
//
//  Code_Frontiers
//  Copyright (C) 2018  Université de Lorraine - CNRS
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  Created by Melanie Jouaiti on 30/09/2017.
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
 * @param jointNames joint names of the robot (std::vector<std::string>)
 * @see Simulation()
 */
Simulation::Simulation(std::vector<std::string> jointNames): m_robot(), m_t(0),
                                                            m_dt(0.01), m_record(),
                                                            m_startForce()
{
    start();
}

/**
 * Simulation Destructor
 */
Simulation::~Simulation()
{
    // Stop V-REP Simulation and close the connection
    simxStopSimulation(m_clientID, simx_opmode_blocking);
    int pt;
    simxGetPingTime(m_clientID, &pt);
    simxFinish(m_clientID);
    std::cout << "Connection closed"  << std::endl;
}

/**
 * Simulation start: Connects to remote API server and retrieves sphere informations
 */
void Simulation::start()
{
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
