//
//  CPG.h
//  Handshaking
//
//  Created by Melanie Jouaiti on 24/12/2017.
//  Copyright © 2017 Melanie Jouaiti. All rights reserved.
//

#ifndef CPG_h
#define CPG_h

#include <iostream>
#include "lib-cpg/Integrator.h"


#define H_MYU 0.5
//---------------
#define M_W 4.0
#define M_BETA 10.0
#define M_TAU_R 0.5
#define M_TAU_A 0.25
#define M_EPS 0.1
#define M_S_0 0.1 //0.20

enum var {
    V_E, y_E, sigma_sE, V_F, y_F, sigma_sF,
    Af_E, Af_F, eps_E, eps_F
};

class CPG
{
public:
    CPG();
    CPG(const double tauM, const double tauS,
        const double sigmaF,
        const double epsilon, const double lambda,
        const double Af, const double mu);
    CPG(const double* params, std::vector<double> var);
    CPG(const std::string& type, std::vector<double> var);
    ~CPG();
    
    void setSigmaF(const double sF);
    void setW(const double W);
    void setInput(const double f);
    void setEpsilon(const double eps);
    
    void reset(const double ss, const double af);
    
    double step(const double vmes, const double t, const double dt);
    void init();
    void finalise();
    void coupleCPG(CPG* cpg, const double tauR = 0.005, const double W = 0.005);
    std::vector<double> getVariables();
    std::vector<double> getParameters();
    double getOutput();
    
    CPG& operator=(const CPG& cpg)
    {
        return *this;
    }
    
    friend void coupleCPG(CPG* cpg1, CPG* cpg2);
    std::vector<double> getData();
    
private:
    const double m_tauM, m_tauS;
    double m_lambda;
    double m_sigmaF;
    double m_mu;
    double m_W;
    std::vector<double> m_variables;            /**< CPG variables: V_e, y_e, ss_e, phi_e, V_f, y_f, ss_f, phi_f for each joint */
    std::vector<double> m_phi;
    std::vector<std::function<double (double, std::vector<double>)>> m_functions;
    Integrator m_runge;                         /**< Runge-Kutta Integrator */
    Integrator m_phiRunge;
    std::vector<CPG*> m_coupledCPGs;
    std::vector<double> m_record;
    bool m_isCoupled;
    
    
    //--------
    double y_e, y_f;
};

#endif /* CPG_h */
