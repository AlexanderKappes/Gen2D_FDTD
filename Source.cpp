#include "Header.hpp"

Source::Source(int Nsh_in, double dT_in, double Imax_sin_in, double f_in, double Psi_0_in)
{
    Imax_sin    = Imax_sin_in;
    Psi_0       = Psi_0_in;
    f           = f_in;

    Nsh         = Nsh_in;
    dT          = dT_in;

}

void Source::SourceSinSize()
{
    I_sin.resize(Nsh);
    time.resize(Nsh);
}

void Source::SourceSinCreate ()
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

void Source::SourceE (grid_fdtd *g, GenGrid2D *GenGr, bool part)
{
    unsigned int i, j;
    unsigned int M, N;

    if (part)
    {
        M = GenGr->rotor_grid_par.Row;
        N = GenGr->rotor_grid_par.Col;

        for (i = 0; i < N; i+=2)
            for (j = 0; j < M; j+=2)
            {
                if( GenGr->rot_grid_pos[j + i*M].source)
                    g->ez   [j  + i * M] = 0.0;
            }
    }
    else
    {
        M = GenGr->stator_grid_par.Row;
        N = GenGr->stator_grid_par.Col;

        for (i = 0; i < N; i+=2)
            for (j = 0; j < M; j+=2)
            {
                if( GenGr->stat_grid_pos[j + i*M].source)
                    g->ez   [j  + i * M] = 50.0;
            }
    }
}

/*
static double cdtds, ppw = 0;

void ezIncInit(grid_fdtd *g){

        printf("Enter the points per wavelength for Ricker source: ");
        //scanf(" %lf", &ppw);
        ppw = 30.0;
        //ppw = ppw_from_SB;
        cdtds = g->cdtds;
        return;
}

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
*/

Source::~Source()
{

}
