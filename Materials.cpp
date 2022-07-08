#include "Header.hpp"
material_par set_material_data(double nu_in,    double sigma_in,    double epsilon_in)
{
    material_par val;
    val.nu           = nu_in;
    val.sigma        = sigma_in;
    val.epsilon      = epsilon_in;

    return val;
}

Materials::Materials(int in_XSize_rot, int in_YSize_rot, int in_XSize_stat, int in_YSize_stat)
{
    //Размер счетного пространства
    XSize_rot   = in_XSize_rot;
    YSize_rot   = in_YSize_rot;
    XSize_stat  = in_XSize_stat;
    YSize_stat  = in_YSize_stat;
    //массивы предварительно вычисленных коэффициентов

    rot_wedge               = set_material_data(1.0,    1.0,    1.0);//rot - 1
    rot_arm                 = set_material_data(1.0,    1.0,    1.0);//rot - 2
    rot_steel               = set_material_data(1.0,    1.0,    1.0);//rot - 3

    stat_air                = set_material_data(1.0,    1.0,    1.0);//stat - 1
    stat_wedge              = set_material_data(1.0,    1.0,    1.0);//stat - 2
    stat_arm                = set_material_data(1.0,    1.0,    1.0);//stat - 3
    stat_steel_across       = set_material_data(1.0,    1.0,    1.0);//stat - 4
    stat_steel_along        = set_material_data(1.0,    1.0,    1.0);//stat - 5
}



void Materials::Materials_array(double dT, GenGrid2D *Gr)
{
    for (int i = 0; i < YSize_rot; i++)
    {
        for (int j = 0; j < XSize_rot; j++)
            AE_rot[i][j] = 1.0;
    }
}

void gridInit(grid_fdtd *g) {
    double imp0 = 377.0;
    int mm, nn;

    g->type = tmZGrid;
    g->sizeX = 101; // x size of domain
    g->sizeY = 81; // y size of domain
    g->maxTime = 300; // duration of simulation
    g->cdtds = 1.0 / sqrt(2.0); // Courant number

    ALLOC_2D(g->hx,     g->sizeX,      g->sizeY - 1,  double);
    ALLOC_2D(g->chxh,   g->sizeX,      g->sizeY - 1,  double);
    ALLOC_2D(g->chxe,   g->sizeX,      g->sizeY - 1,  double);
    ALLOC_2D(g->hy,     g->sizeX - 1,  g->sizeY,      double);
    ALLOC_2D(g->chyh,   g->sizeX - 1,  g->sizeY,      double);
    ALLOC_2D(g->chye,   g->sizeX - 1,  g->sizeY,      double);
    ALLOC_2D(g->ez,     g->sizeX,      g->sizeY,      double);
    ALLOC_2D(g->ceze,   g->sizeX,      g->sizeY,      double);
    ALLOC_2D(g->cezh,   g->sizeX,      g->sizeY,      double);

    /* set electric-field update coefficients */
    for (mm = 0; mm < g->sizeX; mm++)
        for (nn = 0; nn < g->sizeY; nn++)
        {
            g->ceze [(mm) * g->sizeY + (nn)] = 1.0;
            g->cezh [(mm) * g->sizeY + (nn)] = g->cdtds * imp0;
        }

    /* set magnetic-field update coefficients */
    for (mm = 0; mm < g->sizeX; mm++)
        for (nn = 0; nn < g->sizeY - 1; nn++)
        {
            g->chxh [(mm) * (g->sizeY-1) + (nn)] = 1.0;
            g->chxe [(mm) * (g->sizeY-1) + (nn)] = g->cdtds / imp0;
        }

    for (mm = 0; mm < g->sizeX - 1; mm++)
        for (nn = 0; nn < g->sizeY; nn++)
        {
            g->chyh [(mm) * g->sizeY + (nn)] = 1.0;
            g->chye [(mm) * g->sizeY + (nn)] = g->cdtds / imp0;
        }

    int check1 =0;
    return;
}
