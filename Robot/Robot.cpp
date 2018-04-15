//
//  Robot.cpp
//  Graphical
//
//  Created by Melanie Jouaiti on 09/11/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#include "Robot.h"

/**
 * Robot constructor
 * @see Robot(std::vector<std::string> jointNames)
 */
Robot::Robot()
{
    
}

/**
 * Robot constructor
 * @param jointNames vector of robot joint names
 * @see Robot()
 */
Robot::Robot(std::vector<std::string> jointNames): m_jointNames(jointNames)
{
    
}

/**
 * Robot destructor
 */
Robot::~Robot()
{
    
}
