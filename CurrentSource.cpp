#include "Header.hpp"

CurrentSource::CurrentSource(int Nsh_in, double dT_in, double Imax_sin_in, double f_in, double Psi_0_in)
{
    Imax_sin    =   Imax_sin_in;
    Psi_0       =   Psi_0_in;
    f           =   f_in;

    Nsh         = Nsh_in;
    dT          = dT_in;

    double ppw = 30.0;
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

static double cdtds, ppw = 0;

/* initialize source-function variables */
void ezIncInit(grid_fdtd *g){

        printf("Enter the points per wavelength for Ricker source: ");
        //scanf(" %lf", &ppw);
        ppw = 30.0;
        //ppw = ppw_from_SB;
        cdtds = g->cdtds;
        return;
}

/* calculate source function at given time and location */
double ezInc(double time, double location) {
    double arg;

    if (ppw <= 0) {
        fprintf(stderr,
        "ezInc: ezIncInit() must be called before ezInc.\n"
        " Points per wavelength must be positive.\n");
        exit(-1);
    }

    arg = M_PI * ((cdtds * time - location) / ppw - 1.0);
    arg = arg * arg;

return (1.0 - 2.0 * arg) * exp(-arg);
}


CurrentSource::~CurrentSource()
{

}
