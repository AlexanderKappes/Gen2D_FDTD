#ifndef FDTD_CALC_H
#define FDTD_CALC_H
#include "Header.hpp"

class Fdtd_calc
{
public:
    Fdtd_calc();

    void updateH2d(grid_fdtd *g, grid_fdtd *gj, GenGrid2D *GenGr, double dT, bool part, int im_out) ;
    void updateE2d(grid_fdtd *g, grid_fdtd *gj, GenGrid2D *GenGr, double dT, bool part, int im_out) ;

    int iH;
    int iE;
};

#endif // FDTD_CALC_H
