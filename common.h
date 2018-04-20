//
//  common.h
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
//  Created by Melanie Jouaiti and Lacelot Caron on 07/10/2017.
//

#ifndef common_h
#define common_h

#include <cmath>

#define PID_P 0.05
#define PID_I 0
#define PID_D 0

#define TMAX 130

#define JOINT_NB1 1
#define JOINT_NB2 2

//------ Rowat-Selverston equations
#define _VEQ(y, yother, input, eps, W) (y - W*y/(1 + exp(-4*yother)) + eps*input(t))
#define _YEQ(V, y, ss, tauM, tauS, sigmaF, Af) (1 / tauM * (sigmaF - tauM/tauS - 1.0 - sigmaF * pow(tanh(sigmaF * V / Af), 2))*y - (1.0 + ss)/(tauS*tauM)*V + Af * tanh(sigmaF * V / Af) / (tauS * tauM))
#define _SSEQ(V, y, ss, input, tauM, tauS, sigmaF, eps) (2 * eps * input(t) * sqrt(tauM * tauS * (1 + ss - sigmaF))*y/r(V, y))
#define _PHIEQ(V0, y0, V, y, phi) sin(theta(V0, y0) - theta(V, y) - phi)
#define _AFEQ(V, y, input, Af, eps, sigmaF, mu) -mu * (400 * pow(V / Af * sigmaF, 2) - pow(input(t), 2))
#define _EPSEQ(input, eps, lambda) lambda * tanh(std::abs(100.0 * input(t))) * (1.0 - pow(input(t)*eps, 2))

double r(double x, double y);
double PF(double Vi);
double S(double vmes);
double MN(double PFi, double Si);
double inputSignal(double t);
double theta(double x, double y);

#endif /* common_h */
