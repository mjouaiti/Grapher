//
//  Reach.h
//  Graphical
//
//  Created by Melanie Jouaiti on 25/10/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#ifndef Reach_h
#define Reach_h

#include <stdio.h>

#include "Simulation.h"

class Reach: public Simulation
{
public:
    Reach();
    Reach(std::vector<std::string> jointNames);
    ~Reach();
    
    void run();
    virtual void init();
    virtual std::vector<double> step();
    virtual void saveToFile(const std::string& path);
    
private:
    bool m_gotBall;                             /**< true if the ball is in the gripper, false otherwise */
    int m_handHandle, m_sensor_handle;          /**< VREP handle of the gripper ans sensor */
    float m_handPos[3];                         /**< initial pose of the sphere */
};

#endif /* Reach_h */
