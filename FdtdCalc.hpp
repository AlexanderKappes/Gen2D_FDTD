#ifndef FDTD_CALC_H
#define FDTD_CALC_H
#include "Header.hpp"

class Fdtd_calc
{
public:
    Fdtd_calc(grid_fdtd *g, int in_XSize, int in_YSize, int in_ZSize, double in_TX, double in_TY, double in_TZ, int   in_NT, double in_To, double in_dT);

    //Размер счетного пространства
    int XSize;
    int YSize;
    int ZSize;
    //Коэффициенты
    double TX; //1/dX
    double TY; //1/dY
    double TZ; //1/dZ

    int    NT;
    double To;//Исходное значение времени
    double dT;

    void updateH2d(grid_fdtd *g);
    void updateE2d(grid_fdtd *g);


};

#endif // FDTD_CALC_H
