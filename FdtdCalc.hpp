#ifndef FDTD_CALC_H
#define FDTD_CALC_H
#include "Header.hpp"

class Fdtd_calc
{
public:
    Fdtd_calc();

    void updateH2d(grid_fdtd *g, GenGrid2D *GenGr, bool part) ;
    void updateE2d(grid_fdtd *g, GenGrid2D *GenGr, bool part) ;

    int iH;
    int iE;
};

#endif // FDTD_CALC_H
