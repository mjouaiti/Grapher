//
//  Robot_VREP.h
//  Handshaking
//
//  Created by Melanie Jouaiti on 24/09/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#ifndef Robot_VREP_h
#define Robot_VREP_h

#include <iostream>
#include <chrono>

#include "Robot.h"

extern "C" {
#include <extApi.h>
}

class Robot_VREP: public Robot
{
    public:
    
    Robot_VREP();
    Robot_VREP(const int clientId, std::vector<std::string> jointNames);
    
    ~Robot_VREP();
    
    virtual double getAngularPosition(const unsigned int jointID);
    virtual double getAngularVelocity(const unsigned int jointID);
    virtual double getAngularForce(const unsigned int jointID);
    
    virtual double getAngularPosition(const std::string jointName);
    virtual double getAngularVelocity(const std::string jointName);
    virtual double getAngularForce(const std::string jointName);
    
    virtual bool setPosition(const unsigned int jointID, const double pos);
    virtual bool setVelocity(const unsigned int jointID, const double speed);
    virtual bool setForce(const unsigned int jointID, const double force);
    
    virtual bool setPosition(const std::string jointName, const double pos);
    virtual bool setVelocity(const std::string jointName, const double speed);
    virtual bool setForce(const std::string jointName, const double force);
    
    virtual bool updateAngularPosition(const unsigned int index = -1);
    virtual bool updateAngularVelocity(const unsigned int index = -1);
    virtual bool updateAngularForce(const unsigned int index = -1);
    
    virtual bool updateAngularPosition(const std::string jointName = "");
    virtual bool updateAngularVelocity(const std::string jointName = "");
    virtual bool updateAngularForce(const std::string jointName = "");
    
    bool lockJoint(const int jointHandle);
    bool closeGripper(const int* motorHandles);
    bool openGripper(const int* motorHandles);
    
    private:
    
    int m_clientID;                                 /**< id of the VREP client */
    std::map<std::string, int> m_jointHandles;      /**< map of the joint handles stored by joint name */
};


#endif /* Robot_VREP_h */
