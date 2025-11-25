#ifndef CURRENTSOURCE_HPP
#define CURRENTSOURCE_HPP
#include "Header.hpp"
class Source
{
public:
    Source(int Nsh_in, double dT_in, double Imax_sin_in, double f_in, double Psi_0_in);
    ~Source();

    void SourceSinCreate ();
    void SourceSinSize      ();
    void SourceE (grid_fdtd *g, GenGrid2D *GenGr, bool part);

    QVector<double> I_sin;
    QVector<double> time;

    int      Nsh;
    double   dT;
    double Imax_sin;
    double Psi_0;
    double f;
};

#endif // CURRENTSOURCE_HPP
