//
//  PIDController.h
//  Handshaking
//
//  Created by Melanie Jouaiti on 08/10/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
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
