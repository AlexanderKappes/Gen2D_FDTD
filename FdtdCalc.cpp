#include "Header.hpp"

Fdtd_calc::Fdtd_calc()
{

}


void Fdtd_calc::updateH2d(grid_fdtd *g, GenGrid2D *GenGr, double dT, bool part)
{
    int i, j;

    double Ez_next_y, Ez_next_x, Ez;
    double Chxh, Chxe, Chyh, Chye;
    double Hy, Hx;
    int M;
    int N;

    if (part)
    {
        M = GenGr->rotor_grid_par.Row;
        N = GenGr->rotor_grid_par.Col;
    }
    else
    {
        M = GenGr->stator_grid_par.Row;
        N = GenGr->stator_grid_par.Col;
    }
    if (g->type == oneDGrid)
        {
            for (i = 0; i < N; i++)
            {
                Chyh        = g->chyh[i];
                Hy          = g->hy[i];
                Chye        = g->chye[i];
                Ez_next_x   = g->ez[i + 1];
                Ez          = g->ez[i];
                g->hy[i]   = Chyh * Hy + Chye * (Ez_next_x - Ez);
            }
        }
    else {
        for (i = 0; i < N; i++)
            for (j = 0; j < M - 2; j++)
            {
                    Chxh        = g->chxh [j  + i * M];
                    Hx          = g->hx   [j  + i * M];
                    Chxe        = g->chxe [j  + i * M];
                    Ez_next_x   = g->ez   [j  + i * M];
                    Ez          = g->ez   [j  + i * M];
                    g->hx   [j  + i * M] = Chxh * Hx - Chxe * (Ez_next_x - Ez);
                }

        for (i = 0; i < N; i++)
            for (j = 0; j < M - 2; j++)
            {
                    Chyh        = g->chyh [j  + i * M];
                    Hy          = g->hy   [j  + i * M];
                    Chye        = g->chye [j  + i * M];
                    Ez_next_y   = g->ez   [j  + i * M];
                    Ez          = g->ez   [j  + i * M];
                    g->hy   [j  + i * M] = Chyh * Hy + Chye * (Ez_next_y - Ez);
                }
         }

    return;
}

/* update electric field */
void Fdtd_calc::updateE2d(grid_fdtd *g, GenGrid2D *GenGr, double dT, bool part)  {
    /*
int mm, nn;
//double EZcheck;

double Ez, Ceze, Cezhx, Cezhy;
double Hy, Hy_pr, Hx, Hx_pr;

if (g->type == oneDGrid)
    {
        for (mm = 1; mm < g->sizeX - 1; mm++)
        {
            Ez          = g->ez[mm];
            Ceze        = g->ceze[mm];
            Cezhx        = g->cezhx[mm];
            Cezhy        = g->cezhy[mm];
            Hy          = g->hy[mm];
            Hy_pr       = g->hy[mm-1];
            g->ez[mm]   = Ceze * Ez + Cezhx * (Hy - Hy_pr);
        }
    }
else
    {
        for (mm = 1; mm < g->sizeX - 1; mm++)
            for (nn = 1; nn < g->sizeY - 1; nn++)
            {
                Ez      = g->ez [(mm) * g->sizeY + (nn)];
                Ceze    = g->ceze [(mm) * g->sizeY + (nn)];
                Cezhx    = g->cezhx [(mm) * g->sizeY + (nn)];
                Cezhy    = g->cezhy [(mm) * g->sizeY + (nn)];
                Hy      = g->hy [(mm) * g->sizeY + (nn)];
                Hy_pr   = g->hy [(mm - 1) * g->sizeY + (nn)];
                Hx      = g->hx [(mm) * g->sizeY + (nn)];
                Hx_pr   = g->hx [(mm) * g->sizeY + (nn - 1)];
                g->ez [(mm) * g->sizeY + (nn)] = Ceze * Ez + Cezhx * (( Hy - Hy_pr) - ( Hx - Hx_pr));
            }
    }
*/
return;
}



