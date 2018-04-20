//
//  common.cpp
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
//  Created by Melanie Jouaiti and Lancelot Caron on 19/10/2017.
//

#include "common.h"

/**
 * sqrt(x^2 + y^2)
 * @param x first variable
 * @param y second variable
 */
double r(double x, double y)
{
    return sqrt(x*x + y*y);
}

/**
 * Pattern Formation Neuron
 * @param Vi output of the rhytmic neuron
 */
double PF(double Vi)
{
    return 1.0 / (1.0 + exp(-0.5 * Vi));
}

/**
 * Sensory Neuron
 * @param vmes joint velocity
 */
double S(double vmes)
{
    return 1.0 / (1.0 + exp(-0.061342 * vmes));
}

/**
 * Motor Neuron
 * @param PFi output of the pattern formation neuron
 * @param Si output of the sensory neuron
 */
double MN(double PFi, double Si)
{
    return 1.0 / (1.0 + exp(3.0 * (PFi - Si)));
}

/**
 * Input Signal (sinusoidal at 2 Hz, amplitude 0.16)
 * @param t time t
 */
double inputSignal(double t)
{
//    return 0.08 * sin(2 * 2 * M_PI * t) * (t < TMAX - 10);
    if(t < 35)
        return 0.08 * sin(2.0 * 2 * M_PI * (t - 0));
    else if(t < 70)
        return 0.08 * sin(1.0 * 2 * M_PI * (t - 35));
    else if(t < 100)
        return 0.08 * sin(2.0 * 2 * M_PI * (t - 70));
    else if(t < 120)
        return 0.08 * sin(2.5 * 2 * M_PI * (t - 100));
    else
        return 0;
}

/**
 * theta function
 * @param V output of the rhytmic neuron
 * @param y derivative of V
 */
double theta(double V, double y) {
    return (2*(V >= 0) - 1) * acos(-y / sqrt(V*V + y*y));
}


