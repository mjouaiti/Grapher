//
//  Robot_VREP.cpp
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

#include "Robot_VREP.h"
#include <cmath>

/**
 * Robot_VREP Constructor
 * @see Robot_VREP(const int clientId, std::vector<std::string> jointNames)
 */
Robot_VREP::Robot_VREP(): m_clientID(-1)
{
    
}

/**
 * Robot_VREP Constructor
 * @param clientId Id of the VREP client (int)
 * @param jointNames list of the joint names of the robot (std::vector<std::string>)
 * @see Robot_VREP()
 */
Robot_VREP::Robot_VREP(const int clientId, std::vector<std::string> jointNames): Robot(jointNames), m_clientID(clientId)
{
    std::vector<int> tmp(m_jointNames.size());
    int i = 0;
    
    //Get all joint handles
    for(auto jointName: m_jointNames)
    {
        int err = simxGetObjectHandle(m_clientID, jointName.c_str(), &tmp[i], simx_opmode_blocking);
        m_jointHandles[jointName] = tmp[i++];
        if(err != simx_return_ok)
        {
            std::cerr << "Couldn't get object: " << jointName << std::endl;
        }
    }
    m_positions.resize(m_jointNames.size());
    m_velocities.resize(m_jointNames.size());
    m_forces.resize(m_jointNames.size());
}

/**
 * Robot_VREP Destructor
 * @see Robot_VREP()
 * @see Robot_VREP(const int clientId, std::vector<std::string> jointNames)
 */
Robot_VREP::~Robot_VREP()
{
}

/**
 * Gets the angular position of a given joint
 * @param jointID id of the joint we want to get the position of (unsigned int)
 * @return the angular position of the joint in radians (double)
 * @see updateAngularPosition(const unsigned int index)
 * @see setPosition(const unsigned int jointID, const double pos)
 * @see getAngularVelocity(const unsigned int jointID)
 * @see getAngularForce(const unsigned int jointID)
 */
double Robot_VREP::getAngularPosition(const unsigned int jointID)
{
    return m_positions[jointID];
}

/**
 * Gets the angular velocity of a given joint
 * @param jointID id of the joint we want to get the velocity of (unsigned int)
 * @return the angular velocity of the joint in radians (double)
 * @see updateAngularVelocity(const unsigned int index)
 * @see setVelocity(const unsigned int jointID, const double vel)
 * @see getAngularPosition(const unsigned int jointID)
 * @see getAngularForce(const unsigned int jointID)
 */
double Robot_VREP::getAngularVelocity(const unsigned int jointID)
{
    return m_velocities[jointID];
}

/**
 * Gets the angular force of a given joint
 * @param jointID id of the joint we want to get the force of (unsigned int)
 * @return the angular force of the joint in radians (double)
 * @see updateAngularForce(const unsigned int index)
 * @see setForce(const unsigned int jointID, const double force)
 * @see getAngularPosition(const unsigned int jointID)
 * @see getAngularVelocity(const unsigned int jointID)
 */
double Robot_VREP::getAngularForce(const unsigned int jointID)
{
    return m_forces[jointID];
}

/**
 * Gets the angular position of a given joint
 * @param jointName name of the joint we want to get the position of (std::string)
 * @return the angular position of the joint in radians (double)
 * @see getAngularPosition(const unsigned int jointID)
 * @see updateAngularPosition(const unsigned int index)
 * @see setPosition(const unsigned int jointID, const double pos)
 * @see getAngularVelocity(const unsigned int jointID)
 * @see getAngularForce(const unsigned int jointID)
 */
double Robot_VREP::getAngularPosition(const std::string jointName)
{
    return m_positions[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()];
}

/**
 * Gets the angular velocity of a given joint
 * @param jointName name of the joint we want to get the velocity of (std::string)
 * @return the angular velocity of the joint in radians (double)
 * @see getAngularForce(const unsigned int jointID)
 * @see updateAngularVelocity(const unsigned int index)
 * @see setVelocity(const unsigned int jointID, const double vel)
 * @see getAngularPosition(const unsigned int jointID)
 * @see getAngularForce(const unsigned int jointID)
 */
double Robot_VREP::getAngularVelocity(const std::string jointName)
{
    return m_velocities[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()];
}

/**
 * Gets the angular force of a given joint
 * @param jointName name of the joint we want to get the force of (std::string)
 * @return the angular force of the joint in radians (double)
 * @see getAngularForce(const unsigned int jointID)
 * @see updateAngularForce(const unsigned int index)
 * @see setForce(const unsigned int jointID, const double force)
 * @see getAngularPosition(const unsigned int jointID)
 * @see getAngularVelocity(const unsigned int jointID)
 */
double Robot_VREP::getAngularForce(const std::string jointName)
{
    return m_forces[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()];
}

/**
 * Sets the angular position of a given joint
 * @param jointID id of the joint we want to set the position of (unsigned int)
 * @param pos angular position to be applied (double)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularPosition(const unsigned int index)
 * @see getAngularPosition(const unsigned int jointID)
 * @see setVelocity(const unsigned int jointID, const double speed)
 * @see setForce(const unsigned int jointID, const double force)
 */
bool Robot_VREP::setPosition(const unsigned int jointID, const double pos)
{
    return setPosition(m_jointNames[jointID], pos);
}

/**
 * Sets the angular velocity of a given joint
 * @param jointID id of the joint we want to set the velocity of (unsigned int)
 * @param speed angular velocity to be applied (double)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularVelocity(const unsigned int index)
 * @see getAngularVelocity(const unsigned int jointID)
 * @see setPosition(const unsigned int jointID, const double pos)
 * @see setForce(const unsigned int jointID, const double force)
 */
bool Robot_VREP::setVelocity(const unsigned int jointID, const double speed)
{
    return setVelocity(m_jointNames[jointID], speed);
}

/**
 * Sets the angular force of a given joint
 * @param jointID id of the joint we want to set the force of (unsigned int)
 * @param force force to be applied (double)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularForce(const unsigned int index)
 * @see getAngularForce(const unsigned int jointID)
 * @see setPosition(const unsigned int jointID, const double pos)
 * @see setVelocity(const unsigned int jointID, const double speed)
 */
bool Robot_VREP::setForce(const unsigned int jointID, const double force)
{
    return setForce(m_jointNames[jointID], force);
}

/**
 * Sets the angular velocity of a given joint
 * @param jointName name of the joint we want to set the position of (std::string)
 * @param pos angular position to be applied (double)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularPosition(const unsigned int index)
 * @see getAngularPosition(const unsigned int jointID)
 * @see setPosition(const std::string jointName, const double pos)
 * @see setVelocity(const std::string jointName, const double speed)
 * @see setForce(const std::string jointName, const double force)
 * @see setVelocity(const std::string jointName, const double speed)
 * @see setForce(const unsigned int jointID, const double force)
 */
bool Robot_VREP::setPosition(const std::string jointName, const double pos)
{
    int err = simxSetJointTargetPosition(m_clientID, m_jointHandles[jointName], (float)pos, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for joint: " << jointName << std::endl;
    return err == simx_return_ok;
}

/**
 * Sets the angular velocity of a given joint
 * @param jointName name of the joint we want to set the velocity of (std::string)
 * @param speed angular velocity to be applied (double)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularVelocity(const unsigned int index)
 * @see getAngularVelocity(const unsigned int jointID)
 * @see setVelocity(const std::string jointName, const double speed)
 * @see setForce(const std::string jointName, const double force)
 * @see setPosition(const std::string jointName, const double pos)
 * @see setPosition(const unsigned int jointID, const double pos)
 * @see setForce(const unsigned int jointID, const double force)
 */
bool Robot_VREP::setVelocity(const std::string jointName, const double speed)
{
    int err = simxSetJointTargetVelocity(m_clientID, m_jointHandles[jointName], (float)speed, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Velocity for joint: " << jointName << std::endl;
    return err == simx_return_ok;
}

/**
 * Sets the angular force of a given joint
 * @param jointName name of the joint we want to set the force of (std::string)
 * @param force force to be applied (double)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularForce(const unsigned int index)
 * @see setForce(const unsigned int jointID, const double force)
 * @see getAngularForce(const unsigned int jointID)
 * @see setVelocity(const std::string jointName, const double speed)
 * @see setPosition(const std::string jointName, const double pos)
 * @see setPosition(const unsigned int jointID, const double pos)
 * @see setVelocity(const unsigned int jointID, const double speed)
 */
bool Robot_VREP::setForce(const std::string jointName, const double force)
{
    int err = simxSetJointForce(m_clientID, m_jointHandles[jointName], (float)force, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Force for joint: " << jointName << std::endl;
    return err == simx_return_ok;
}

/**
 * Updates the angular position of a given joint
 * @param index (optional) id of the joint we want to update. If not set, will update every joint (unsigned int)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularVelocity(const unsigned int index)
 * @see updateAngularForce(const unsigned int index)
 * @see getAngularPosition(const unsigned int jointID)
 * @see setPosition(const unsigned int jointID, const double pos)
 */
bool Robot_VREP::updateAngularPosition(const unsigned int index)
{
    return updateAngularPosition(index == -1? "": m_jointNames[index]);
}

/**
 * Updates the angular velocity of a given joint
 * @param index (optional) id of the joint we want to update. If not set, will update every joint (unsigned int)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularPosition(const unsigned int index)
 * @see updateAngularForce(const unsigned int index)
 * @see getAngularVelocity(const unsigned int jointID)
 * @see setVelocity(const unsigned int jointID, const double speed)
 */
bool Robot_VREP::updateAngularVelocity(const unsigned int index)
{
    return updateAngularVelocity(index == -1? "": m_jointNames[index]);
}

/**
 * Updates the angular force of a given joint (Be careful not to call this function more than necessary since it's terribly slow)
 * @param index (optional) id of the joint we want to update. If not set, will update every joint (unsigned int)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularPosition(const unsigned int index)
 * @see updateAngularVelocity(const unsigned int index)
 * @see getAngularForce(const unsigned int jointID)
 * @see setForce(const unsigned int jointID, const double force)
 */
bool Robot_VREP::updateAngularForce(const unsigned int index)
{
    return updateAngularForce(index == -1? "": m_jointNames[index]);
}

/**
 * Updates the angular position of a given joint
 * @param jointName (optional) name of the joint we want to update. If not set, will update every joint (std::string)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularPosition(const unsigned int index)
 * @see updateAngularVelocity(const unsigned int index)
 * @see updateAngularForce(const unsigned int index)
 * @see getAngularPosition(const unsigned int jointID)
 * @see setPosition(const unsigned int jointID, const double pos)
 */
bool Robot_VREP::updateAngularPosition(const std::string jointName)
{
    int err = simx_return_ok;
    int i = 0;
    if(jointName == "")
    {
        for(auto jointName: m_jointNames)
        {
            float pos;
            err = simxGetJointPosition(m_clientID, m_jointHandles[jointName], &pos, simx_opmode_blocking);
            if(err != simx_return_ok)
                std::cerr << "Could not get Position for joint: " << jointName << std::endl;
            m_positions[i++] = pos;
        }
    }
    else
    {
        float pos;
        err = simxGetJointPosition(m_clientID, m_jointHandles[jointName], &pos, simx_opmode_blocking);
        if(err != simx_return_ok)
            std::cerr << "Could not get Position for joint: " << jointName << std::endl;
        m_positions[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()] = pos;
    }
    return err == simx_return_ok;
}

/**
 * Updates the angular velocity of a given joint
 * @param jointName (optional) name of the joint we want to update. If not set, will update every joint (std::string)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularVelocity(const unsigned int index)
 * @see updateAngularPosition(const unsigned int index)
 * @see updateAngularForce(const unsigned int index)
 * @see getAngularVelocity(const unsigned int jointID)
 * @see setVelocity(const unsigned int jointID, const double speed)
 */
bool Robot_VREP::updateAngularVelocity(const std::string jointName)
{
    int err = simx_return_ok;
    int i = 0;
    if(jointName == "")
    {
        for(auto jointName: m_jointNames)
        {
            float vmes;
            err = simxGetObjectFloatParameter(m_clientID, m_jointHandles[jointName], 2012, &vmes, simx_opmode_blocking);
            if(err != simx_return_ok)
                std::cerr << "Could not get Velocity for joint: " << jointName << std::endl;
            m_velocities[i++] = vmes;
        }
    }
    else
    {
        float vmes;
        err = simxGetObjectFloatParameter(m_clientID, m_jointHandles[jointName], 2012, &vmes, simx_opmode_blocking);
        if(err != simx_return_ok)
            std::cerr << "Could not get Velocity for joint: " << jointName << std::endl;
        m_velocities[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()] = vmes;
    }
    
    return err == simx_return_ok;
}

/**
 * Updates the angular force of a given joint (Be careful not to call this function more than necessary since it's terribly slow)
 * @param jointName (optional) name of the joint we want to update. If not set, will update every joint (std::string)
 * @return true if the operation succeeded, false otherwise (bool)
 * @see updateAngularForce(const unsigned int index)
 * @see updateAngularPosition(const unsigned int index)
 * @see updateAngularVelocity(const unsigned int index)
 * @see getAngularForce(const unsigned int jointID)
 * @see setForce(const unsigned int jointID, const double force)
 */
bool Robot_VREP::updateAngularForce(const std::string jointName)
{
    int err = simx_return_ok;
    int i = 0;
    if(jointName == "")
    {
        for(auto jointName: m_jointNames)
        {
            float force;
            err = simxGetJointForce(m_clientID, m_jointHandles[jointName], &force, simx_opmode_blocking);
            if(err != simx_return_ok)
                std::cerr << "Could not get Force for joint: " << jointName << std::endl;
            m_forces[i++] = force;
        }
    }
    else
    {
        float force;
        err = simxGetJointForce(m_clientID, m_jointHandles[jointName], &force, simx_opmode_blocking);
        if(err != simx_return_ok)
            std::cerr << "Could not get Force for joint: " << jointName << std::endl;
        m_forces[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()] = force;
    }
    return err == simx_return_ok;
}

/*
 * Close the robot gripper. (Warning: This fonction is gripper specific, if you use a different gripper, you'll have to adapt it)
 * @param motorHandles motors the closing velocity will be applied to (int*)
 * @return whether the operation succedeed (bool)
 */
bool Robot_VREP::closeGripper(const int* motorHandles)
{
    float speed = -0.1;
    int err = simx_return_ok;
    for(unsigned int i = 4; i < 4; i++)
    {
        err = simxSetJointTargetVelocity(m_clientID, motorHandles[i], speed, simx_opmode_blocking);
        if(err != simx_return_ok)
            std::cerr << "Could not set Velocity for motor: " << i << std::endl;
    }
    
    int finger;
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint1_finger1", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 10.0 / 180.0 * M_PI, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint1_finger1" << std::endl;
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger1", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 75.0 / 180.0 * M_PI, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger1" << std::endl;
    
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger2", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 70.0 / 180.0 * M_PI, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger2" << std::endl;
    
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger3", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 70.0 / 180.0 * M_PI, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger3" << std::endl;
    
    
    return err == simx_return_ok;
}

/*
 * Open the robot gripper. (Warning: This fonction is gripper specific, if you use a different gripper, you'll have to adapt it)
 * @param motorHandles motors the opening velocity will be applied to (int*)
 * @return whether the operation succedeed (bool)
 */
bool Robot_VREP::openGripper(const int* motorHandles)
{
    float speed = 0;
    int err = simx_return_ok;
    for(unsigned int i = 0; i < 4; i++)
    {
        err = simxSetJointTargetVelocity(m_clientID, motorHandles[i], speed, simx_opmode_blocking);
        if(err != simx_return_ok)
            std::cerr << "Could not set Velocity for motor: " << i << std::endl;
    }
    
    int finger;
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint1_finger1", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 0, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint1_finger1" << std::endl;
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger1", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 0, simx_opmode_blocking);
    if(err != simx_return_ok)
    std::cerr << "Could not set Position for finger: JacoHand_joint2_finger1" << std::endl;
    
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger1", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 0, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger1" << std::endl;
    
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger2", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 0, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger2" << std::endl;

    err = simxGetObjectHandle(m_clientID, "JacoHand_joint1_finger3", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 0, simx_opmode_blocking);
    if(err != simx_return_ok)
    std::cerr << "Could not set Position for finger: JacoHand_joint1_finger3" << std::endl;
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger3", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 0, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger3" << std::endl;
    
    
    return err == simx_return_ok;
}
