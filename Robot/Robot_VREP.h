//
//  Robot_VREP.h
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
//  Created by Melanie Jouaiti on 24/09/2017.
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
    
    bool closeGripper(const int* motorHandles);
    bool openGripper(const int* motorHandles);
    
    private:
    
    int m_clientID;                                 /**< id of the VREP client */
    std::map<std::string, int> m_jointHandles;      /**< map of the joint handles stored by joint name */
};


#endif /* Robot_VREP_h */
