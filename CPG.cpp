//
//  CPG.cpp
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
//  Created by Melanie Jouaiti and Lancelot Caron on 24/12/2017.
//

#include "CPG.h"
#include "common.h"

double* input;

/**
 * CPG constructor
 * @see CPG(const double* params, std::vector<double> var)
 */
CPG::CPG(): m_tauM(0.35), m_tauS(3.5),
            m_lambda(0.002),
            m_sigmaF(1.0), m_mu(0.0005),
            m_W(0.005), m_variables(), m_runge()
{
    m_variables = { 0.0, -0.216, 10.0, 0.0, -0.216, 10.0, 0.05, 0.05, 0.01, 0.01};
    m_phi = {0.0, 0.0};
    init();
}

/**
 * CPG constructor
 * @see CPG()
 */
CPG::CPG(const double* params, std::vector<double> var): m_tauM(params[0]), m_tauS(params[1]),
                                                        m_lambda(params[3]), m_sigmaF(params[2]),
                                                        m_mu(params[4]), m_W(0.005),
                                                        m_variables(var), m_runge()
{
    init();
    m_phi = {0.0, 0.0};
}

/**
 * CPG destructor
 */
CPG::~CPG()
{
    
}

/**
 * CPG initialisation function
 * @see finalize()
 */
void CPG::init()
{
    input = new double[1];
    
    m_functions = std::vector<std::function<double (double, std::vector<double>)> >(0, NULL);
    
    std::vector<std::function<double (double)> > inputs {
        [this](double t)->double { return -input[0]; }, //RGE input
        [this](double t)->double { return input[0]; }//RGF input
    };
    m_functions.push_back([inputs, this](double t, std::vector<double> args)->double {
        return _VEQ(args[y_E], args[y_F], inputs[0], args[eps_E], m_W);
    });
    m_functions.push_back([this](double t, std::vector<double> args)->double {
        return _YEQ(args[V_E], args[y_E], args[sigma_sE], m_tauM, m_tauS, m_sigmaF, args[Af_E]);
    });
    m_functions.push_back([inputs, this](double t, std::vector<double> args)->double {
        return _SSEQ(args[V_E], args[y_E], args[sigma_sE], inputs[0], m_tauM, m_tauS, m_sigmaF, args[eps_E]);
    });
    m_functions.push_back([inputs, this](double t, std::vector<double> args)->double {
        return _VEQ(args[y_F], args[y_E], inputs[1], args[eps_F], m_W);
    });
    m_functions.push_back([this](double t, std::vector<double> args)->double {
        return _YEQ(args[V_F], args[y_F], args[sigma_sF], m_tauM, m_tauS, m_sigmaF, args[Af_F]);
    });
    m_functions.push_back([inputs, this](double t, std::vector<double> args)->double {
        return _SSEQ(args[V_F], args[y_F], args[sigma_sF], inputs[1], m_tauM, m_tauS, m_sigmaF, args[eps_F]);
    });
    m_functions.push_back([inputs, this](double t, std::vector<double> args)->double {
        return _AFEQ(args[V_E], args[y_E], inputs[0], args[Af_E], args[eps_E], m_sigmaF, m_mu);
    });
    m_functions.push_back([inputs, this](double t, std::vector<double> args)->double {
        return _AFEQ(args[V_F], args[y_F], inputs[1], args[Af_F], args[eps_F], m_sigmaF, m_mu);
    });
    m_functions.push_back([inputs, this](double t, std::vector<double> args)->double {
        return _EPSEQ(inputs[0], args[eps_E], m_lambda);
    });
    m_functions.push_back([inputs, this](double t, std::vector<double> args)->double {
        return _EPSEQ(inputs[1], args[eps_F], m_lambda);
    });
}

/**
 * CPG coupling function
 * @param cpg1 first CPG
 * @param cpg2 secong CPG
 */
void coupleCPG(CPG* cpg1, CPG* cpg2)
{
    std::vector<std::function<double (double, std::vector<double>)> > functions(0, NULL);
    functions.push_back( [cpg1, cpg2](double t, std::vector<double> args)->double {
        return _PHIEQ(cpg2->m_variables[V_E], cpg2->m_variables[y_E],
                      cpg1->m_variables[V_E], cpg1->m_variables[y_E], args[0]);
    });
    functions.push_back( [cpg1, cpg2](double t, std::vector<double> args)->double {
        return _PHIEQ(cpg2->m_variables[V_F], cpg2->m_variables[y_F],
                      cpg1->m_variables[V_F], cpg1->m_variables[y_F], args[1]);
    });
    cpg2->m_phiRunge = Integrator(2, functions, cpg2->m_phi);
    cpg1->m_coupledCPGs.push_back(cpg2);
    
    std::vector<std::function<double (double, std::vector<double>)> > functions2(0, NULL);
    functions2.push_back( [cpg1, cpg2](double t, std::vector<double> args)->double {
        return _PHIEQ(cpg1->m_variables[V_E], cpg1->m_variables[y_E],
                      cpg2->m_variables[V_E], cpg2->m_variables[y_E], args[0]);
    });
    functions2.push_back( [cpg1, cpg2](double t, std::vector<double> args)->double {
        return _PHIEQ(cpg1->m_variables[V_F], cpg1->m_variables[y_F],
                      cpg2->m_variables[V_F], cpg2->m_variables[y_F], args[1]);
    });
    cpg1->m_phiRunge = Integrator(2, functions2, cpg1->m_phi);
    cpg2->m_coupledCPGs.push_back(cpg1);
}

/**
 * finalisation function, to be called after init
 * @see CPG(const double* params, std::vector<double> var)
 */
void CPG::finalise()
{
    m_runge = Integrator((int)m_functions.size(), m_functions, m_variables);
}

/**
 * sets the GPS input
 * @param f input force */
void CPG::setInput(const double f)
{
    input[0] = f;
}

/**
 * step function
 * @param vmes current joint velocity
 * @param t current time
 * @param dt timestep
 */
double CPG::step(const double vmes, const double t, const double dt)
{
    m_variables = m_runge.rk4(t, dt);
    if(m_coupledCPGs.size() > 0)
        m_phi = m_phiRunge.rk4(t, dt);
    double PFE = PF(m_variables[V_E]);
    double PFF = PF(m_variables[V_F]);
    double SN = S(vmes);
    double MNE = MN(PFE, SN);
    double MNF = MN(PFF, SN);
    
    //-------------------
    m_record.clear();
    m_record.push_back(input[0]);
    m_record.push_back(m_variables[V_E]);
    m_record.push_back(m_variables[V_F]);
    m_record.push_back(m_variables[y_E]);
    m_record.push_back(m_variables[y_F]);
    m_record.push_back(m_variables[sigma_sE]);
    m_record.push_back(m_variables[sigma_sF]);
    m_record.push_back(m_variables[Af_E]);
    m_record.push_back(m_variables[Af_F]);
    m_record.push_back(m_variables[eps_E]);
    m_record.push_back(m_variables[eps_F]);
    m_record.push_back(m_phi[0]);
    m_record.push_back(m_phi[1]);
    m_record.push_back(PFE);
    m_record.push_back(PFF);
    m_record.push_back(SN);
    m_record.push_back(MNE);
    m_record.push_back(MNF);
    
    return MNE - MNF;
}

/**
 * data getter function
 * @return a vector
 * @see step(const double vmes, const double t, const double dt)
 */
std::vector<double> CPG::getData()
{
    return m_record;
}
