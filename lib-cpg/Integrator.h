//
//  Integrator.h
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
//  Created by Lancelot Caron.
//

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <functional>
#include <vector>

class Integrator {
public:
    Integrator();
    Integrator(int count, std::vector<std::function<double (double, std::vector<double>)> > functions, std::vector<double> variables);
    std::vector<double> rk4(double t, double dt);
    std::vector<double> rk4(double t, double dt, std::vector<double> variables2);
    ~Integrator();

private:
    std::vector<double> k;
    std::vector<double> l;
    std::vector<double> m;
    std::vector<double> lvariables;
    std::vector<double> mvariables;
    std::vector<double> nvariables;
    std::vector<std::function<double (double, std::vector<double>)> > functions;
    std::vector<double> variables;
    int count;
};

#endif
