#ifndef IMAGEFIELD_HPP
#define IMAGEFIELD_HPP
//#include "qPlot/qcustomplot.h"
#include "Header.hpp"


class ImageField: public QObject
{

public:
    ImageField(QCustomPlot *CP, QString NameCP);
    ~ImageField();

    QCPColorMap *colorMapIF;
    QCPColorScale *colorScaleIF;
    QCPMarginGroup *marginGroupIF;
    void addSnapshot(QCustomPlot *Cp, int Nt, GenGrid2D *GenGr, GenGeom2D *GenGeom);
    void OpenFiles(std::string nameFile, int Nt, int &N, int &M, bool part);

    double* ez_IF_r;
    double* ez_IF_s;

};

#endif // IMAGEFIELD_HPP
