//
//  Simulation.h
//  Handshaking
//
//  Created by Melanie Jouaiti on 30/09/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#ifndef Simulation_h
#define Simulation_h

#define VREP

#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include "../lib-cpg/Integrator.h"
#ifdef VREP
#include "../Robot/Robot_VREP.h"
#else
#include "../lib-robot/Robot.h"
#endif
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
#ifdef VREP
    int m_sphereHandle;                         /**< VREP handle of the sphere */
    int m_motorHandles[4];                      /**< VREP handles of the finger motore */
    int m_clientID;                             /**< id of the VREP client */
    std::vector<PIDController> m_PIDControllers; /**< vector of PIDControllers, one for each joint */
#endif
    double m_t, m_dt;                           /**< current time in seconds, timestep */
    std::vector<double> m_record;               /**< recorded values */
    float* m_startForce;                        /**< force applied to the joint at the beginning of the simulation */
    
    std::map<std::string, CPG*> m_CPGs;
};

#endif /* Simulation_h */
