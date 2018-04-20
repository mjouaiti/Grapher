//
//  Robot.h
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
//  Created by Melanie Jouaiti on 09/11/2017.
//

#ifndef Robot_h
#define Robot_h

#include <iostream>
#include <vector>
#include <map>

class Robot
{
    public:
    
    Robot();
    Robot(std::vector<std::string> jointNames);
    
    ~Robot();
    
    virtual double getAngularPosition(const unsigned int jointID) = 0;
    virtual double getAngularVelocity(const unsigned int jointID) = 0;
    virtual double getAngularForce(const unsigned int jointID) = 0;
    
    virtual double getAngularPosition(const std::string jointName) = 0;
    virtual double getAngularVelocity(const std::string jointName) = 0;
    virtual double getAngularForce(const std::string jointName) = 0;
    
    virtual bool setPosition(const unsigned int jointID, const double pos) = 0;
    virtual bool setVelocity(const unsigned int jointID, const double speed) = 0;
    virtual bool setForce(const unsigned int jointID, const double force) = 0;
    
    virtual bool setPosition(const std::string jointName, const double pos) = 0;
    virtual bool setVelocity(const std::string jointName, const double speed) = 0;
    virtual bool setForce(const std::string jointName, const double force) = 0;
    
    virtual bool updateAngularPosition(const unsigned int index = -1) = 0;
    virtual bool updateAngularVelocity(const unsigned int index = -1) = 0;
    virtual bool updateAngularForce(const unsigned int index = -1) = 0;
    
    virtual bool updateAngularPosition(const std::string jointName = "") = 0;
    virtual bool updateAngularVelocity(const std::string jointName = "") = 0;
    virtual bool updateAngularForce(const std::string jointName = "") = 0;

    protected:
    
    std::vector<std::string> m_jointNames;          /**< list of the robot joint names */
    std::vector<double> m_positions;                /**< vector of joint positions */
    std::vector<double> m_velocities;               /**< vector of joint velocities */
    std::vector<double> m_forces;                   /**< vector of joint forces */
};

#endif /* Robot_h */
