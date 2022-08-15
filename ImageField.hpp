#ifndef IMAGEFIELD_HPP
#define IMAGEFIELD_HPP
//#include "qPlot/qcustomplot.h"
#include "Header.hpp"


class ImageField
{
public:
    ImageField();
    ~ImageField();

    QCPColorMap *colorMap1;
    QCPColorScale *colorScale1;
    QCPMarginGroup *marginGroup1;
    void addSnapshot(QCustomPlot *Cp, int Nt, grid_fdtd *g, GenGrid2D *GenGr, bool part);
};

#endif // IMAGEFIELD_HPP
