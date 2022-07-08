#include "Header.hpp"
#include "FdtdAlloc.hpp"

Fdtd_calc::Fdtd_calc(int in_XSize, int in_YSize, int in_ZSize, double in_TX, double in_TY, double in_TZ, int    in_NT, double in_To, double in_dT)
{
    //Размер счетного пространства
    XSize = in_XSize;
    YSize = in_YSize;
    ZSize = in_ZSize;
    //Коэффициенты
    TX = in_TX; //1/dX
    TY = in_TY; //1/dY
    TZ = in_TZ; //1/dZ

    NT = in_NT;
    To = in_To;//Исходное значение времени
    dT = in_dT;

}

/* update magnetic field */
void Fdtd_calc::updateH2d(grid_fdtd *g) {
int mm, nn;

double Ez_next_y, Ez_next_x, Ez;
double Chxh, Chxe, Chyh, Chye;
double Hy, Hy_pr, Hx, Hx_pr;

if (g->type == oneDGrid)
    {
        for (mm = 0; mm < g->sizeX - 1; mm++)
        {
            Chyh        = g->chyh[mm];
            Hy          = g->hy[mm];
            Chye        = g->chye[mm];
            Ez_next_x   = g->ez[mm + 1];
            Ez          = g->ez[mm];
            g->hy[mm]   = Chyh * Hy + Chye * (Ez_next_x - Ez);
        }
    }
else {
        for (mm = 0; mm < g->sizeX; mm++)
            for (nn = 0; nn < g->sizeY - 1; nn++)
            {
                Chxh        = g->chxh [(mm) * (g->sizeY-1) + (nn)];
                Hx          = g->hx   [(mm) * (g->sizeY-1) + (nn)];
                Chxe        = g->chxe [(mm) * (g->sizeY-1) + (nn)];
                Ez_next_x   = g->ez   [(mm) * g->sizeY + (nn +1 )];
                Ez          = g->ez   [(mm) * g->sizeY + (nn)];
                g->hx   [(mm) * (g->sizeY-1) + (nn)] = Chxh * Hx - Chxe * (Ez_next_x - Ez);
            }

        for (mm = 0; mm < g->sizeX - 1; mm++)
            for (nn = 0; nn < g->sizeY; nn++)
            {
                Chyh        = g->chyh [(mm) * g->sizeY + (nn)];
                Hy          = g->hy   [(mm) * g->sizeY + (nn)];
                Chye        = g->chye [(mm) * g->sizeY + (nn)];
                Ez_next_y   = g->ez   [(mm + 1) * g->sizeY + (nn)];
                Ez          = g->ez   [(mm) * g->sizeY + (nn)];
                g->hy   [(mm) * g->sizeY + (nn)] = Chyh * Hy + Chye * (Ez_next_y - Ez);
            }
     }

    int check = 0;
    return;
}

/* update electric field */
void Fdtd_calc::updateE2d(grid_fdtd *g) {
int mm, nn;
//double EZcheck;

double Ez, Ceze, Cezh;
double Hy, Hy_pr, Hx, Hx_pr;

if (g->type == oneDGrid)
    {
        for (mm = 1; mm < g->sizeX - 1; mm++)
        {
            Ez          = g->ez[mm];
            Ceze        = g->ceze[mm];
            Cezh        = g->cezh[mm];
            Hy          = g->hy[mm];
            Hy_pr       = g->hy[mm-1];
            g->ez[mm]   = Ceze * Ez + Cezh * (Hy - Hy_pr);
        }
    }
else
    {
        for (mm = 1; mm < g->sizeX - 1; mm++)
            for (nn = 1; nn < g->sizeY - 1; nn++)
            {
                Ez      = g->ez [(mm) * g->sizeY + (nn)];
                Ceze    = g->ceze [(mm) * g->sizeY + (nn)];
                Cezh    = g->cezh [(mm) * g->sizeY + (nn)];
                Hy      = g->hy [(mm) * g->sizeY + (nn)];
                Hy_pr   = g->hy [(mm - 1) * g->sizeY + (nn)];
                Hx      = g->hx [(mm) * g->sizeY + (nn)];
                Hx_pr   = g->hx [(mm) * g->sizeY + (nn - 1)];
                g->ez [(mm) * g->sizeY + (nn)] = Ceze * Ez + Cezh * (( Hy - Hy_pr) - ( Hx - Hx_pr));
            }
    }

int check = 0;
return;
}



