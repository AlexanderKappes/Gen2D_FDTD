#ifndef CURRENTSOURCE_HPP
#define CURRENTSOURCE_HPP
#include "Header.hpp"
class CurrentSource
{
public:
    CurrentSource(int Nsh_in, double dT_in, double Imax_sin_in, double f_in, double Psi_0_in);
    ~CurrentSource();

    void CurrentSourceSinCreate ();
    void CurrentSourceSinSize      ();

    QVector<double> I_sin;
    QVector<double> time;

    int      Nsh;
    double   dT;
    double Imax_sin;
    double Psi_0;
    double f;
};

#endif // CURRENTSOURCE_HPP
