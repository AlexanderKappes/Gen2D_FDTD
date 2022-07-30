#ifndef MATERIALS_HPP
#define MATERIALS_HPP
#include "Header.hpp"

struct material_par
    {
        double mu;
        double sigma;
        double epsilon;
    };

class Materials
{
public:
    Materials();
    void Materials_array(GenGrid2D *Gr, bool part);
    void gridInit(grid_fdtd *g, GenGrid2D *GenGr, double dT, bool part);
    //Размер счетного пространства
    int XSize_rot;
    int YSize_rot;
    int XSize_stat;
    int YSize_stat;
    QVector<material_par> rot_mat;
    QVector<material_par> stat_mat;

    material_par rot_wedge;//Дюралюминий: rot - 1, mu = 1, sigma = 0, epsilon = 1
    material_par rot_arm;//Медь: rot - 2, mu = 1, sigma = 0, epsilon = 1
    material_par rot_steel;//Сталь 1030: rot - 3, mu = 477.6, sigma = 0, epsilon = 1

    material_par stat_air;//Воздух: stat - 1, mu = 1, sigma = 0, epsilon = 1
    material_par stat_wedge;//???: stat - 2, mu = 1, sigma = 0, epsilon = 1
    material_par stat_arm;//Медь: stat - 3, mu = 1, sigma = 0, epsilon = 1
    material_par stat_steel_across;//Сталь 3413 поперек (лин. участок): stat - 4, mu = 3959, sigma = 0, epsilon = 1
    material_par stat_steel_along;//Сталь 3413 вдоль (лин. участок): stat - 5, mu = 4918, sigma = 0, epsilon = 1
};

#endif // MATERIALS_HPP
