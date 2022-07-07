#ifndef IMAGEFIELD_HPP
#define IMAGEFIELD_HPP
#include "qPlot/qcustomplot.h"

class ImageField
{
public:
    ImageField();
    ~ImageField();

    QCPColorMap *colorMap1;
    QCPColorScale *colorScale1;
    QCPMarginGroup *marginGroup1;
    void addSnapshot(QCustomPlot *Cp, int N);
};

#endif // IMAGEFIELD_HPP
