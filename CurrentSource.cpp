#include "Header.hpp"

CurrentSource::CurrentSource(int Nsh_in, double dT_in, double Imax_sin_in, double f_in, double Psi_0_in)
{
    Imax_sin    =   Imax_sin_in;
    Psi_0       =   Psi_0_in;
    f           =   f_in;

    Nsh         = Nsh_in;
    dT          = dT_in;
}

void CurrentSource::CurrentSourceSinSize()
{
    I_sin.resize(Nsh);
    time.resize(Nsh);
}

void CurrentSource::CurrentSourceSinCreate ()
{
    for (int i = 0; i < Nsh; i++)
    {
            if (i == 0) time [i] = 0;
                else
                    time [i] = time [i - 1] + dT;
    }

    double w = f * 2 * M_PI;

    for (int t = 0; t < Nsh; t++)
    {
            I_sin[t] =  Imax_sin * qSin(w * time [t] + Psi_0 * M_PI / 180);
    }
}

CurrentSource::~CurrentSource()
{

}
