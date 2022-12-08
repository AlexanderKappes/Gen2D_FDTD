﻿#include "Header.hpp"

Fdtd_calc::Fdtd_calc()
{
    iH = 0;
    iE = 0;
}


void Fdtd_calc::updateH2d(grid_fdtd *g, grid_fdtd *gj, GenGrid2D *GenGr, double dT, bool part, int im_out)
{
    int i, j;
    int M, N;

    double Ez_next_y, Ez_next_x, Ez;
    double Chxh, Chxe, Chyh, Chye;
    double Hy, Hx;
    std::string strPath;

    if (part)
        strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\EH\\Rotor\\";
    else
        strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\EH\\Stator\\";

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
        /*
            for (i = 0; i < (N - 1); i++)
            {
                Chyh        = g->chyh[i];
                Hy          = g->hy[i];
                Chye        = g->chye[i];
                Ez_next_x   = g->ez[i + 1];
                Ez          = g->ez[i]; 
                g->hy[i]   = Chyh * Hy + Chye * (Ez_next_x - Ez);
            }
            */
        }
    else {
        for (i = 0; i < N; i+=2)
            for (j = 0; j < M; j+=2)
            {
                    Chxh        = g->chxh [j  + i * M];
                    Hx          = g->hx   [j  + (i+1) * M];
                    Chxe        = g->chxe [j  + i * M];
                    Ez_next_x   = g->ez   [j  + (i+2) * M];
                    Ez          = g->ez   [j  + i * M];
                    g->hx   [j  + i * M] = Chxh * Hx - Chxe * (Ez_next_x - Ez);
                }

        for (i = 0; i < N; i+=2)
        {
            for (j = 0; j < M; j+=2)
            {
                    Chyh        = g->chyh [j + i * M];
                    Hy          = g->hy   [j + 1  + i * M];
                    Chye        = g->chye [j + i * M];
                    Ez          = g->ez   [j + i * M];

                    if (j  < (M-2))
                    {
                        Ez_next_y   = g->ez   [j + 2  + i * M];
                    }
                    else if (j == (M-2) && part)
                        //Cвязь с сеткой статора через Ez
                    {
                        Ez_next_y      = GenGr->join_Ez_grid_pos[i/2].val;
                    }

                    g->hy   [j  + i * M] = Chyh * Hy + Chye * (Ez_next_y - Ez);
                }

            int check =0;
        }

    }
    iH++;

    int nj = 0;
    if (part)
        for (i = 0; i < N && nj < GenGr->stator_grid_par.Col / 2; i+=2)
        {
            if (i == GenGr->join_Hy_grid_pos[nj].i1)//если это сетка статора, сохраняем для расчета сетки ротора
                //Cвязь с сеткой статора через Ez
            {
                while (i == GenGr->join_Hy_grid_pos[nj].i1)
                {
                    GenGr->join_Hy_grid_pos[nj].val =
                            g->hy   [GenGr->join_Hy_grid_pos[nj].i2 * M] * GenGr->join_Hy_grid_pos[nj].arg1/GenGr->join_Hy_grid_pos[nj].arg +
                            g->hy   [GenGr->join_Hy_grid_pos[nj].i1 * M] * GenGr->join_Hy_grid_pos[nj].arg2/GenGr->join_Hy_grid_pos[nj].arg;
                    //GenGr->join_Hy_grid_pos[nj].val = i+1;
                    nj++;
                    if ((nj * 2) == GenGr->stator_grid_par.Col)
                        break;
                }
            }
        }

    if (part && iH == im_out)
    {
        ArrOutText (strPath, "Hy_Rotor" + std::to_string(iH), N, M, GenGr->rotor_grid_par.Np_s,  GenGr->rotor_grid_par.Np_p, g->hy);
        ArrOutText (strPath, "Hx_Rotor" + std::to_string(iH), N, M, GenGr->rotor_grid_par.Np_s,  GenGr->rotor_grid_par.Np_p, g->hx);
    }
    else if (!part && iH == im_out)
    {
        ArrOutText (strPath, "Hy_Stator" + std::to_string(iH), N, M, GenGr->stator_grid_par.Np_s,  GenGr->stator_grid_par.Np_p, g->hy);
        ArrOutText (strPath, "Hx_Stator" + std::to_string(iH), N, M, GenGr->stator_grid_par.Np_s,  GenGr->stator_grid_par.Np_p, g->hx);
    }


    return;
}

/* update electric field */
void Fdtd_calc::updateE2d(grid_fdtd *g, grid_fdtd *gj, GenGrid2D *GenGr, double dT, bool part, int im_out)  {

    int i, j;
    int M, N;

    double Ez, Ceze, Cezhx, Cezhy;
    double Hy, Hy_pr, Hx, Hx_pr;
    std::string strPath;

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
    if (part)
        strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\EH\\Rotor\\";
    else
        strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\EH\\Stator\\";

    if (g->type == oneDGrid)
        {
        /*
        for (i = 1; i < (N - 1); i++)
            {
                Ez          = g->ez[i];
                Ceze        = g->ceze[i];
                Cezhx       = g->cezhx[i];
                Cezhy       = g->cezhy[i];
                Hy          = g->hy[i];
                Hy_pr       = g->hy[i-1];
                g->ez[i]    = Ceze * Ez + Cezhx * (Hy - Hy_pr);
            }
            */
        }
    else
        {
        for (i = 0; i < N; i+=2)
            for (j = 0; j < M; j+=2)
            {
                    if (j == 0)
                    {
                        Ez      = 0;
                    }
                    else
                    {
                        Ez      = g->ez     [j  + i * M];
                    }
                    Ceze    = g->ceze   [j  + i * M];
                    Cezhx   = g->cezhx  [j  + i * M];
                    Cezhy   = g->cezhy  [j  + i * M];
                    Hy      = g->hy     [j  + i * M];

                    if (j == 0 && part)
                    {
                        Hy_pr   = 0;
                    }
                    else if (j == 0 && !part)
                    {
                        //Cвязь с сеткой ротора через Hy
                        Hy_pr   = GenGr->join_Hy_grid_pos[i/2].val;
                    }
                        else
                    {
                        Hy_pr   = g->hy     [j +1  + i * M];
                    }

                    if (i == 0)
                    {
                        Hx      = g->hx     [j  + (i + 1) * M];
                    }
                        else
                    {
                        Hx_pr   = g->hx     [j   + N*M - 2*M];
                    }

                    g->ez [j  + i * M] = Ceze * Ez + Cezhy * ( Hy - Hy_pr) - Cezhx * ( Hx - Hx_pr);
            }
        }

    int nj = 0;
    if (!part)
        for (i = 0; i < N && nj < GenGr->rotor_grid_par.Col / 2; i+=2)
        {
            if (i == GenGr->join_Ez_grid_pos[nj].i1)//если это сетка статора, сохраняем для расчета сетки ротора
                //Cвязь с сеткой статора через Ez
            {
                while (i == GenGr->join_Ez_grid_pos[nj].i1)
                {
                    double Ez1 = g->ez   [M - 2 + GenGr->join_Ez_grid_pos[nj].i1 * M];
                    double Ez2 = g->ez   [M - 2 + GenGr->join_Ez_grid_pos[nj].i2 * M];
                    double arg1 = GenGr->join_Ez_grid_pos[nj].arg1;
                    double arg2 = GenGr->join_Ez_grid_pos[nj].arg2;
                    double arg = GenGr->join_Ez_grid_pos[nj].arg;

                    //GenGr->join_Ez_grid_pos[nj].val =
                    //        g->ez   [M - 2 + GenGr->join_Ez_grid_pos[nj].i2 * M] * GenGr->join_Ez_grid_pos[nj].arg1/GenGr->join_Ez_grid_pos[nj].arg +
                    //        g->ez   [M - 2 + GenGr->join_Ez_grid_pos[nj].i1 * M] * GenGr->join_Ez_grid_pos[nj].arg2/GenGr->join_Ez_grid_pos[nj].arg;

                    GenGr->join_Ez_grid_pos[nj].val = Ez2*arg1/arg + Ez1*arg2/arg;

                    nj++;
                    if ((nj * 2) == GenGr->rotor_grid_par.Col)
                        break;
                }
            }
        }
    iE++;

    if (part && iE == im_out)
    {
        ArrOutText (strPath, "Ez_Rotor" + std::to_string(iE), N, M, GenGr->rotor_grid_par.Np_s,  GenGr->rotor_grid_par.Np_p, g->ez);
    }
    else if (!part && iE == im_out)
    {
        ArrOutText (strPath, "Ez_Stator" + std::to_string(iE), N, M, GenGr->stator_grid_par.Np_s,  GenGr->stator_grid_par.Np_p, g->ez);
    }
return;
}



