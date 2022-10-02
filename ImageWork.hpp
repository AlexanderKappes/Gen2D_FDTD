#ifndef IMAGE_HPP
#define IMAGE_HPP
#include "Header.hpp"

class ImageWork
{


public:
    ImageWork();
    ~ImageWork();
    //ImageWork(QWidget *parent = 0);

    void add_Curve(Ui::MainWindow* ui, GenGraph2D *GenGraph, GenGeom2D *GenGeom, QCustomPlot *CP, bool brush, bool coil);
    void add_Point(GenGraph2D *GenGraph, GenGrid2D *GenGrid, QCustomPlot *CP);
    void ColorPlace(Ui::MainWindow *ui, GenGraph2D *GenGraph);


};

#endif // IMAGE_HPP
