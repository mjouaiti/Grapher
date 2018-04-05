//
//  Robot_VREP.cpp
//  Handshaking
//
//  Created by Melanie Jouaiti on 24/09/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
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
 * @param clientId Id of the VREP client
 * @param jointNames list of the joint names of the robot
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
 */
Robot_VREP::~Robot_VREP()
{
}

/**
 * Gets the angular position of a given joint
 * @param jointID id of the joint we want to get the position of
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
 * @param jointID id of the joint we want to get the velocity of
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
 * @param jointID id of the joint we want to get the force of
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

double Robot_VREP::getAngularPosition(const std::string jointName)
{
    return m_positions[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()];
}

double Robot_VREP::getAngularVelocity(const std::string jointName)
{
    return m_velocities[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()];
}

double Robot_VREP::getAngularForce(const std::string jointName)
{
    return m_forces[find(m_jointNames.begin(), m_jointNames.end(), jointName) - m_jointNames.begin()];
}

/**
 * Sets the angular position of a given joint
 * @param jointID id of the joint we want to set the position of
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
 * @param jointID id of the joint we want to set the velocity of
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
 * @param jointID id of the joint we want to set the force of
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

bool Robot_VREP::setPosition(const std::string jointName, const double pos)
{
    int err = simxSetJointTargetPosition(m_clientID, m_jointHandles[jointName], (float)pos, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for joint: " << jointName << std::endl;
    return err == simx_return_ok;
}

bool Robot_VREP::setVelocity(const std::string jointName, const double speed)
{
    int err = simxSetJointTargetVelocity(m_clientID, m_jointHandles[jointName], (float)speed, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Velocity for joint: " << jointName << std::endl;
    return err == simx_return_ok;
}

bool Robot_VREP::setForce(const std::string jointName, const double force)
{
    int err = simxSetJointForce(m_clientID, m_jointHandles[jointName], (float)force, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Force for joint: " << jointName << std::endl;
    return err == simx_return_ok;
}

/**
 * Updates the angular position of a given joint
 * @param index (optional) id of the joint we want to update. If not set, will update every joint
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
 * @param index (optional) id of the joint we want to update. If not set, will update every joint
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
 * @param index (optional) id of the joint we want to update. If not set, will update every joint
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

/**
 * Is supposed to lock the joint so that it can't move (doesn't seem to be working...)
 * @param jointHandle handle of the joint to lock
 * @return true if the operation succeeded, false otherwise (bool)
 */
bool Robot_VREP::lockJoint(const int jointHandle)
{
    int err;
    err = simxSetObjectIntParameter(m_clientID, jointHandle, 2001, 1, simx_opmode_blocking);
    float pos[1];
    err |= simxGetJointPosition(m_clientID, jointHandle, pos, simx_opmode_blocking);
    err |= simxSetJointTargetPosition(m_clientID, jointHandle, pos[0], simx_opmode_blocking);
    return err == simx_return_ok;
}

/*
 * Close the robot gripper. (Warning: This fonction is gripper specific, if you use a different gripper, you'll have to adapt it)
 * @param motor Handles (pretty useless, the position control part does it all)
 * @return whether the operation succedeed (bool)
 */
bool Robot_VREP::closeGripper(const int* motorHandles)
{
    float speed = -0.004;
    int err = simx_return_ok;
    for(unsigned int i = 4; i < 4; i++)
    {
        err = simxSetJointTargetVelocity(m_clientID, motorHandles[i], speed, simx_opmode_blocking);
        if(err != simx_return_ok)
            std::cerr << "Could not set Velocity for motor: " << i << std::endl;
    }
    
    int finger;
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint1_finger1", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 5.0 / 180.0 * M_PI, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint1_finger1" << std::endl;
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger1", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 70.0 / 180.0 * M_PI, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger1" << std::endl;
    
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger2", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 65.0 / 180.0 * M_PI, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger2" << std::endl;
    
    err = simxGetObjectHandle(m_clientID, "JacoHand_joint2_finger3", &finger, simx_opmode_blocking);
    err = simxSetJointTargetPosition(m_clientID, finger, 65.0 / 180.0 * M_PI, simx_opmode_blocking);
    if(err != simx_return_ok)
        std::cerr << "Could not set Position for finger: JacoHand_joint2_finger3" << std::endl;
    
    
    return err == simx_return_ok;
}

/*
 * Open the robot gripper. (Warning: This fonction is gripper specific, if you use a different gripper, you'll have to adapt it)
 * @param motor Handles (pretty useless, the position control part does it all)
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

////--------------------- Time
///**
// * Starts the clock
// * @see updateClock()
// * @see getMicroseconds()
// * @see getSeconds()
// */
//void Robot_VREP::startClock() {
//    m_startTime = std::chrono::high_resolution_clock::now();
//    m_currentTime = m_startTime;
//}
//
///**
// * Updates the clock
// * @see startClock()
// * @see getMicroseconds()
// * @see getSeconds()
// */
//void Robot_VREP::updateClock() {
//    m_currentTime = std::chrono::high_resolution_clock::now();
//}
//
///**
// * Updates the clock
// * @return elapsed time in microseconds (double)
// * @see startClock()
// * @see updateClock()
// * @see getSeconds()
// */
//double Robot_VREP::getMicroseconds() {
//    return std::chrono::duration_cast<std::chrono::microseconds>(m_currentTime-m_startTime).count();
//}
//
///**
// * Updates the clock
// * @return elapsed time in seconds (double)
// * @see startClock()
// * @see updateClock()
// * @see getMicroseconds()
// */
//double Robot_VREP::getSeconds() {
//    return std::chrono::duration_cast<std::chrono::microseconds>(m_currentTime-m_startTime).count() / 1000000.0;
//}

