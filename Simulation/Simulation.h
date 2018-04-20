//
//  Simulation.h
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

#ifndef Simulation_h
#define Simulation_h

#define VREP

#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include "../lib-cpg/Integrator.h"
#include "../Robot/Robot_VREP.h"
#include "../PIDController.h"
#include "../CPG.h"

class Simulation
{
public:
    Simulation();
    Simulation(std::vector<std::string> jointNames);
    ~Simulation();
    void start();
    virtual void init() = 0;
    void run();
    virtual std::vector<double> step() = 0;
    virtual void saveToFile(const std::string& path) = 0;
    
protected:
    Robot* m_robot;                             /**< robot */
    float m_initPos[3];                         /**< initial pose of the sphere */
    int m_sphereHandle;                         /**< VREP handle of the sphere */
    int m_motorHandles[4];                      /**< VREP handles of the finger motore */
    int m_clientID;                             /**< id of the VREP client */
    std::vector<PIDController> m_PIDControllers; /**< vector of PIDControllers, one for each joint */
    double m_t, m_dt;                           /**< current time in seconds, timestep */
    std::vector<double> m_record;               /**< recorded values */
    float* m_startForce;                        /**< force applied to the joint at the beginning of the simulation */
    
    std::map<std::string, CPG*> m_CPGs;
};

#endif /* Simulation_h */
