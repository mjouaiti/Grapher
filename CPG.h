//
//  CPG.h
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
//  Created by Melanie Jouaiti on 24/12/2017.
//

#ifndef CPG_h
#define CPG_h

#include <iostream>
#include "lib-cpg/Integrator.h"

enum var {
    V_E, y_E, sigma_sE, V_F, y_F, sigma_sF,
    Af_E, Af_F, eps_E, eps_F
};

class CPG
{
public:
    CPG();
    CPG(const double* params, std::vector<double> var);
    ~CPG();
    
    void setInput(const double f);
    
    double step(const double vmes, const double t, const double dt);
    void init();
    void finalise();
    
    /**
     * CPG equal operator
     */
    CPG& operator=(const CPG& cpg)
    {
        return *this;
    }
    
    friend void coupleCPG(CPG* cpg1, CPG* cpg2);
    std::vector<double> getData();
    
private:
    const double m_tauM, m_tauS;                /**< CPG time constants */
    double m_lambda;                            /**< CPG Af learning step */
    double m_sigmaF;                            /**< CPG sigma_f */
    double m_mu;                                /**< CPG epsilon learning step */
    double m_W;                                 /**< CPG inhibitory factor */
    std::vector<double> m_variables;            /**< CPG variables: V_e, y_e, ss_e, V_f, y_f, ss_f for each joint */
    std::vector<double> m_phi;                  /**< coupled CPGs variables: phi_e, phi_f */
    std::vector<std::function<double (double, std::vector<double>)>> m_functions;           /**< CPG equations */
    Integrator m_runge;                         /**< Runge-Kutta Integrator */
    Integrator m_phiRunge;                      /**< Runge-Kutta Integrator for phi */
    std::vector<CPG*> m_coupledCPGs;            /**< vector of coupled CPGs */
    std::vector<double> m_record;               /**< vector containing the data at each iteration */
};

#endif /* CPG_h */
