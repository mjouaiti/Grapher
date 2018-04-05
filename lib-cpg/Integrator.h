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
    //std::vector<std::function<double (double, std::vector<double>)> > functions;
    std::vector<std::function<double (double, std::vector<double>)> > functions;
    std::vector<double> variables;
    int count;
};

class Integrator2 {
public:
    Integrator2();
    Integrator2(int count, std::vector<std::function<double (double, std::vector<double>, std::vector<double>)> > functions, std::vector<double> variables);
    std::vector<double> rk4(double t, double dt, std::vector<double> variables2);
    ~Integrator2();
    
private:
    std::vector<double> k;
    std::vector<double> l;
    std::vector<double> m;
    std::vector<double> lvariables;
    std::vector<double> mvariables;
    std::vector<double> nvariables;
    //std::vector<std::function<double (double, std::vector<double>)> > functions;
    std::vector<std::function<double (double, std::vector<double>, std::vector<double>)> > functions;
    std::vector<double> variables;
    int count;
};

#endif
