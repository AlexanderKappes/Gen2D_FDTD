#include "Header.hpp"
material_par set_material_data(double mu_in,    double epsilon_in,  double sigma_in)
{
    material_par val;
    val.mu           = mu_in;
    val.epsilon      = epsilon_in;
    val.sigma        = sigma_in;

    return val;
}

Materials::Materials()
{
}



void Materials::Materials_array(GenGrid2D *GenGr, bool part)
{
    rot_wedge               = set_material_data(1.0             ,    1.0        ,    0.0            );//rot - 1
    rot_arm                 = set_material_data(0.999994        ,    1.0        ,    59590000.000   );//rot - 2
    rot_steel               = set_material_data(477.57171205428 ,    1.0        ,    1.0            );//rot - 3

    stat_air                = set_material_data(1.00000037      ,    1.0001959  ,    1.0            );//stat - 1
    stat_wedge              = set_material_data(1.0             ,    1.0        ,    1.0            );//stat - 2
    stat_arm                = set_material_data(0.999994        ,    1.0        ,    1.0            );//stat - 3
    stat_steel_across       = set_material_data(3958.85448744299,    1.0        ,    59590000.000   );//stat - 4
    stat_steel_along        = set_material_data(4918.34363619326,    1.0        ,    1.0            );//stat - 5


    int M;
    int N;
    //Ротор
    //1 - клин, 2 - обмотка, 3 - сталь
    //Статор
    //1 - воздух, 2 - клин, 3 - обмотка, 4 - сталь поперек
    //5 - сталь вдоль
    if (part)
    {
        M = GenGr->rotor_grid_par.Row;
        N = GenGr->rotor_grid_par.Col;
        rot_mat.resize(GenGr->rotor_grid_par.Num);
    }
    else
    {
        M = GenGr->stator_grid_par.Row;
        N = GenGr->stator_grid_par.Col;
        stat_mat.resize(GenGr->stator_grid_par.Num);
    }

    if (part)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
                {
                    switch(GenGr->rot_grid_pos[j + i*M].material)
                        {
                            case 1:
                                rot_mat[j + i*M].mu     = rot_wedge.mu;
                                rot_mat[j + i*M].epsilon= rot_wedge.epsilon;
                                rot_mat[j + i*M].sigma  = rot_wedge.sigma;
                                break;
                            case 2:
                                rot_mat[j + i*M].mu     = rot_arm.mu;
                                rot_mat[j + i*M].epsilon= rot_arm.epsilon;
                                rot_mat[j + i*M].sigma  = rot_arm.sigma;
                                break;
                            case 3:
                                rot_mat[j + i*M].mu     = rot_steel.mu;
                                rot_mat[j + i*M].epsilon= rot_steel.epsilon;
                                rot_mat[j + i*M].sigma  = rot_steel.sigma;
                                break;
                        }
                }
        }
    }
    else
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
                {
                    switch(GenGr->stat_grid_pos[j + i*M].material)
                        {
                            case 1:
                                stat_mat[j + i*M].mu     = stat_air.mu;
                                stat_mat[j + i*M].epsilon= stat_air.epsilon;
                                stat_mat[j + i*M].sigma  = stat_air.sigma;
                                break;
                            case 2:
                                stat_mat[j + i*M].mu     = stat_wedge.mu;
                                stat_mat[j + i*M].epsilon= stat_wedge.epsilon;
                                stat_mat[j + i*M].sigma  = stat_wedge.sigma;
                                break;
                            case 3:
                                stat_mat[j + i*M].mu     = stat_arm.mu;
                                stat_mat[j + i*M].epsilon= stat_arm.epsilon;
                                stat_mat[j + i*M].sigma  = stat_arm.sigma;
                                break;
                            case 4:
                                stat_mat[j + i*M].mu     = stat_steel_across.mu;
                                stat_mat[j + i*M].epsilon= stat_steel_across.epsilon;
                                stat_mat[j + i*M].sigma  = stat_steel_across.sigma;
                                break;
                            case 5:
                                stat_mat[j + i*M].mu     = stat_steel_along.mu;
                                stat_mat[j + i*M].epsilon= stat_steel_along.epsilon;
                                stat_mat[j + i*M].sigma  = stat_steel_along.sigma;
                                break;
                        }
                }
        }
    }
}

void Materials::gridInit(grid_fdtd *g, GenGrid2D *GenGr, double dT, bool part)
{

    int i, j;

    int M;
    int N;
    //QVector<material_par> *arr_mat;
    //Ротор
    //1 - клин, 2 - обмотка, 3 - сталь
    //Статор
    //1 - воздух, 2 - клин, 3 - обмотка, 4 - сталь поперек
    //5 - сталь вдоль
    if (part)
    {
        M = GenGr->rotor_grid_par.Row;
        N = GenGr->rotor_grid_par.Col;
        //arr_mat = &rot_mat;
    }
    else
    {
        M = GenGr->stator_grid_par.Row;
        N = GenGr->stator_grid_par.Col;
        //arr_mat = &stat_mat;
    }

    g->type = tmZGrid;
    g->cdtds = 1.0 / sqrt(2.0); // Courant number

    ALLOC_2D(g->hx,      N,      M - 1,  double);
    ALLOC_2D(g->chxh,    N,      M - 1,  double);
    ALLOC_2D(g->chxe,    N,      M - 1,  double);
    ALLOC_2D(g->hy,      N - 1,  M,      double);
    ALLOC_2D(g->chyh,    N - 1,  M,      double);
    ALLOC_2D(g->chye,    N - 1,  M,      double);
    ALLOC_2D(g->ez,      N,      M,      double);
    ALLOC_2D(g->ceze,    N,      M,      double);
    ALLOC_2D(g->cezhx,   N,      M,      double);
    ALLOC_2D(g->cezhy,   N,      M,      double);
    double sigma, epsilon, mu;
    double dx, dy;

    if (part)
    {
        for (i = 0; i < N; i++)
            for (j = 0; j < M; j++)
            {
                sigma   = rot_mat[j  + i * M].sigma;
                epsilon = rot_mat[j  + i * M].epsilon;
                dx      = GenGr->rot_dx[j + i*M];
                dy      = GenGr->rot_dy[j + i*M];

                g->ceze  [j  + i * M] = (1.0 - sigma * dT / (2 * epsilon)) / (1.0 + sigma * dT / (2 * epsilon));
                g->cezhx [j  + i * M] = (1/(1 + sigma * dT/(2 * epsilon))) * dT/ (epsilon * dx);
                g->cezhy [j  + i * M] = (1/(1 + sigma * dT/(2 * epsilon))) * dT/ (epsilon * dy);
            }

        /* set magnetic-field update coefficients */
        for (i = 0; i < N; i++)
            for (j = 0; j < M - 1; j++)
            {
                mu      = rot_mat[j  + i * M].mu;
                dy      = GenGr->rot_dy[j + i*M];

                g->chxh [j  + i * (M - 1)] = 1.0;
                g->chxe [j  + i * (M - 1)] = dT / (mu * dy);
            }

        for (i = 0; i < N - 1; i++)
            for (j = 0; j < M; j++)
            {
                mu      = rot_mat[j  + i * M].mu;
                dx      = GenGr->rot_dx[j + i*M];

                g->chyh [j  + i * M] = 1.0;
                g->chye [j  + i * M] = dT / (mu * dx);
            }
    }
    else
    {
        for (i = 0; i < N; i++)
            for (j = 0; j < M; j++)
            {
                sigma   = stat_mat[j  + i * M].sigma;
                epsilon = stat_mat[j  + i * M].epsilon;
                dx      = GenGr->stat_dx[j + i*M];
                dy      = GenGr->stat_dy[j + i*M];

                g->ceze  [j  + i * M] = (1.0 - sigma * dT / (2 * epsilon)) / (1.0 + sigma * dT / (2 * epsilon));
                g->cezhx [j  + i * M] = (1/(1 + sigma * dT/(2 * epsilon))) * dT/ (epsilon * dx);
                g->cezhy [j  + i * M] = (1/(1 + sigma * dT/(2 * epsilon))) * dT/ (epsilon * dy);
            }

        /* set magnetic-field update coefficients */
        for (i = 0; i < N; i++)
            for (j = 0; j < M - 1; j++)
            {
                mu      = stat_mat[j  + i * M].mu;
                dy      = GenGr->stat_dy[j + i*M];

                g->chxh [j  + i * (M - 1)] = 1.0;
                g->chxe [j  + i * (M - 1)] = dT / (mu * dy);
            }

        for (i = 0; i < N - 1; i++)
            for (j = 0; j < M; j++)
            {
                mu      = stat_mat[j  + i * M].mu;
                dx      = GenGr->stat_dx[j + i*M];

                g->chyh [j  + i * M] = 1.0;
                g->chye [j  + i * M] = dT / (mu * dx);
            }
    }

    return;
}
