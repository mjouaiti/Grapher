#include "Integrator.h"

Integrator::Integrator(): count(0), functions(), variables(), k(), l(), m(), lvariables(), mvariables(), nvariables()
{
    
}

Integrator::Integrator(int count, std::vector<std::function<double (double, std::vector<double>)> > functions, std::vector<double> variables) {
    this->count = count;
    this->functions = functions;
    this->variables = variables;

    k = std::vector<double>(count, 0.0);
    l = std::vector<double>(count, 0.0);
    m = std::vector<double>(count, 0.0);
    lvariables = std::vector<double>(count, 0.0);
    mvariables = std::vector<double>(count, 0.0);
    nvariables = std::vector<double>(count, 0.0);
}

Integrator::~Integrator() {
}

std::vector<double> Integrator::rk4(double t, double dt) {
    for (int i = 0; i < count; i++) {
        k[i] = dt * functions[i](t, variables);
        lvariables[i] = variables[i] + k[i] / 2.0;
    }
    for (int i = 0; i < count; i++) {
        l[i] = dt * functions[i](t + dt /2.0, lvariables);
        mvariables[i] = variables[i] + l[i] / 2.0;
    }
    for (int i = 0; i < count; i++) {
        m[i] = dt * functions[i](t + dt / 2.0, mvariables);
        nvariables[i] = variables[i] + m[i];
    }
    for (int i = 0; i < count; i++) {
        variables[i] = variables[i] + (k[i] + 2.0*l[i] + 2.0*m[i] + dt*functions[i](t, nvariables)) / 6.0;
    }

    return variables;
}



Integrator2::Integrator2(): count(0), functions(), variables(), k(), l(), m(), lvariables(), mvariables(), nvariables()
{
    
}

Integrator2::Integrator2(int count, std::vector<std::function<double (double, std::vector<double>, std::vector<double>)> > functions, std::vector<double> variables) {
    this->count = count;
    this->functions = functions;
    this->variables = variables;
    
    k = std::vector<double>(count, 0.0);
    l = std::vector<double>(count, 0.0);
    m = std::vector<double>(count, 0.0);
    lvariables = std::vector<double>(count, 0.0);
    mvariables = std::vector<double>(count, 0.0);
    nvariables = std::vector<double>(count, 0.0);
}

Integrator2::~Integrator2() {
}


std::vector<double> Integrator2::rk4(double t, double dt,
                                    std::vector<double> variables2) {
    for (int i = 0; i < count; i++) {
        k[i] = dt * functions[i](t, variables, variables2);
        lvariables[i] = variables[i] + k[i] / 2.0;
    }
    for (int i = 0; i < count; i++) {
        l[i] = dt * functions[i](t + dt /2.0, lvariables, variables2);
        mvariables[i] = variables[i] + l[i] / 2.0;
    }
    for (int i = 0; i < count; i++) {
        m[i] = dt * functions[i](t + dt / 2.0, mvariables, variables2);
        nvariables[i] = variables[i] + m[i];
    }
    for (int i = 0; i < count; i++) {
        variables[i] = variables[i] + (k[i] + 2.0*l[i] + 2.0*m[i] + dt*functions[i](t, nvariables, variables2)) / 6.0;
    }
    
    return variables;
}
