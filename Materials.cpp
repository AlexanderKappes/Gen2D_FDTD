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
