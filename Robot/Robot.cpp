//
//  Robot.cpp
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
