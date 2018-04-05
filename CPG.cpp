//
//  CPG.cpp
//  Handshaking
//
//  Created by Melanie Jouaiti on 24/12/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#include "CPG.h"
#include "common.h"

double* input;

CPG::CPG(): m_tauM(0.35), m_tauS(3.5),
            m_lambda(0.002),
            m_sigmaF(1.0), m_mu(0.0005),
            m_W(0.005), m_variables(), m_runge()
{
    m_variables = { 0.0, -0.216, 10.0, 0.0, -0.216, 10.0, 0.05, 0.05, 0.01, 0.01};
    m_phi = {0.0, 0.0};
    init();
}

CPG::CPG(const double tauM, const double tauS,
         const double sigmaF,
         const double epsilon, const double lambda,
         const double Af, const double mu): m_tauM(tauM), m_tauS(tauS),
                                            m_lambda(lambda),
                                            m_sigmaF(sigmaF), m_mu(mu),
                                            m_W(0.005), m_variables(), m_runge()
{
    m_variables = { 0.0, -0.216, 10.0, 0.0, -0.216, 10.0, Af, Af, epsilon, epsilon};
    m_phi = {0.0, 0.0};
    init();
}

CPG::CPG(const double* params, std::vector<double> var): m_tauM(params[0]), m_tauS(params[1]),
                                                        m_lambda(params[3]), m_sigmaF(params[2]),
                                                        m_mu(params[4]), m_W(0),
                                                        m_variables(var), m_runge()
{
    init();
    m_phi = {0.0, 0.0};
}

CPG::~CPG()
{
    
}

void CPG::init()
{
    input = new double[1];
    
    m_functions = std::vector<std::function<double (double, std::vector<double>)> >(0, NULL);
    
    std::vector<std::function<double (double)> > inputs {
        [this](double t)->double { return input[0]; }, //RGE input
        [this](double t)->double { return -input[0]; }//RGF input
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

void CPG::finalise()
{
    m_runge = Integrator((int)m_functions.size(), m_functions, m_variables);
}

void CPG::reset(const double ss, const double af)
{
    m_variables[sigma_sE] = ss;
    m_variables[sigma_sF] = ss;
    m_variables[Af_E] = af;
    m_variables[Af_F] = af;
    m_sigmaF = 2.0;
    finalise();
}

void CPG::setSigmaF(const double sF)
{
    m_sigmaF = sF;
}

void CPG::setW(const double W)
{
    m_W = W;
}

void CPG::setInput(const double f)
{
    input[0] = f;
}

void CPG::setEpsilon(const double eps)
{
    m_variables[eps_E] = m_variables[eps_F] = eps;
}

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

std::vector<double> CPG::getVariables()
{
    return m_variables;
}

std::vector<double> CPG::getParameters()
{
    std::vector<double> p = {m_tauM, m_tauS, m_sigmaF, m_lambda, m_mu};
    return p;
}

std::vector<double> CPG::getData()
{
    return m_record;
}


double CPG::getOutput()
{
    return m_variables[V_E] - m_variables[V_F];
}
