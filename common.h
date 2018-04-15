//
//  common.h
//  Handshaking
//
//  Created by Melanie Jouaiti on 07/10/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#ifndef common_h
#define common_h

#include <cmath>

#define PID_P 0.1
#define PID_I 0
#define PID_D 0

#define TMAX 50

#define JOINT_NB1 1
#define JOINT_NB2 2

//------ Rowat-Selverston equations
#define _VEQ(y, yother, input, eps, W) (y - W*y/(1 + exp(-4*yother)) + eps*input(t))
#define _YEQ(V, y, ss, tauM, tauS, sigmaF, Af) (1 / tauM * (sigmaF - tauM/tauS - 1.0 - sigmaF * pow(tanh(sigmaF * V / Af), 2))*y - (1.0 + ss)/(tauS*tauM)*V + Af * tanh(sigmaF * V / Af) / (tauS * tauM))
#define _SSEQ(V, y, ss, input, tauM, tauS, sigmaF, eps) (2 * eps * input(t) * sqrt(tauM * tauS * (1 + ss - sigmaF))*y/r(V, y))
#define _PHIEQ(V0, y0, V, y, phi) sin(theta(V0, y0) - theta(V, y) - phi)
#define _AFEQ(V, y, input, Af, eps, sigmaF, mu) -mu * (600 * pow(V / Af * sigmaF, 2) - pow(input(t), 2))
#define _EPSEQ(input, eps, lambda) lambda * tanh(std::abs(100.0 * input(t))) * (1.0 - pow(input(t)*eps, 2))

double r(double x, double y);
double PF(double Vi);
double S(double vmes);
double MN(double PFi, double Si);
double inputSignal(double t);
double theta(double x, double y);

#endif /* common_h */
