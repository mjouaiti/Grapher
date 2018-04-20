//
//  PIDController.h
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
//  Created by Melanie Jouaiti on 08/10/2017.
//

#ifndef PIDController_h
#define PIDController_h

#include <stdio.h>

class PIDController
{
public:
    PIDController();
    PIDController(const double kp, const double ki, const double kd, const double dt = 0.01);
    ~PIDController();
    double update(const double currentPos, const double targetVel);
    
private:
    double m_kp, m_ki, m_kd;                            /**< P, I , D values */
    double m_dt;                                        /**< timestep */
    bool m_init;                                        /**< indicates whether the controller has already been initialised */
    double m_pidCumulativeErrorForIntegralParam;        /**< sum of the error values */
    double m_pidLastErrorForDerivativeParam;            /**< last error value */
    double m_targetPos;                                 /**< target position */
};

#endif /* PIDController_h */
