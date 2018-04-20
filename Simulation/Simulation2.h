//
//  Simulation2.h
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
//  Created by Melanie Jouaiti on 07/10/2017.
//

#ifndef Simulation2_h
#define Simulation2_h

#define NB_VARIABLES 44

#include "Simulation.h"

// enum to be used for display or recording purposes
enum values {
    _t, _I,
    _pos2, _vel2, _F2,
    _V_2E, _V_2F, _y_2E, _y_2F, _sigma_s2E, _sigma_s2F,
    _Af_2E, _Af_2F, _Eps_2E, _Eps_2F, _phi_2E, _phi_2F,
    _pf2E, _pf2F, _sn2, _mn2E, mn2F, _s2,
    
    _pos3, _vel3, _F3,
    _V_3E, _V_3F, _y_3E, _y_3F, _sigma_s3E, _sigma_s3F,
    _Af_3E, _Af_3F, _Eps_3E, _Eps_3F, _phi_3E, _phi_3F,
    _pf3E, _pf3F, _sn3, _mn3E, _mn3F, _s3
};

class Simulation2: public Simulation
{
public:
    Simulation2();
    Simulation2(std::vector<std::string> jointNames);
    ~Simulation2();
    
    virtual void init();
    virtual std::vector<double> step();
    virtual void saveToFile(const std::string& path);
    
private:
};

#endif /* Simulation2_h */
