#ifndef MATERIALS_HPP
#define MATERIALS_HPP
#include "Header.hpp"

struct material_par
    {
        double nu;
        double sigma;
        double epsilon;
    };

class Materials
{
public:
    Materials(int in_XSize_rot, int in_YSize_rot, int in_XSize_stat, int in_YSize_stat);
    void Materials_array(double dT, GenGrid2D *Gr);
    //Размер счетного пространства
    int XSize_rot;
    int YSize_rot;
    int XSize_stat;
    int YSize_stat;

    material_par **mat_par;
    //массивы предварительно вычисленных коэффициентов
    double **AE_rot;//C1
    double **BE_rot;//C2
    double **AH_rot;//C

    double **AE_stat;//C1
    double **BE_stat;//C2
    double **AH_stat;//C

    material_par rot_wedge;//rot - 1
    material_par rot_arm;//rot - 2
    material_par rot_steel;//rot - 3

    material_par stat_air;//stat - 1
    material_par stat_wedge;//stat - 2
    material_par stat_arm;//stat - 3
    material_par stat_steel_across;//stat - 4
    material_par stat_steel_along;//stat - 5
};

#endif // MATERIALS_HPP
