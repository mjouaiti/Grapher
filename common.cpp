//
//  common.cpp
//  Handshaking
//
//  Created by Melanie Jouaiti on 19/10/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
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
    return 0.08 * sin(2 * 2 * M_PI * t) * (t < TMAX - 10);
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


