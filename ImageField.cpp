#include "ImageField.hpp"

ImageField::ImageField()
{

}
void ImageField::addSnapshot(QCustomPlot *Cp, int N)
{
    // set up the QCPColorMap:
   colorMap1 = new QCPColorMap(Cp->xAxis, Cp->yAxis);
   int dim1 = 2;
   int dim2 = 2;

   colorMap1->data()->setSize(dim1, dim2); // we want the color map to have nx * ny data points

   QCPRange vis_field_x(-dim1, dim1);
   QCPRange vis_field_y(-dim2, dim2);
   colorMap1->data()->setRange(vis_field_x, vis_field_y);
   float ez [dim1][dim2];

   for (int i = 0; i < dim1; i++)
   {
       for (int j = 0; j < dim2; j++)
       {
        ez [i][j] = rand();
       }
   }
   colorMap1->setName("Color map");
   // now we assign some data, by accessing the QCPColorMapData instance of the color map:
   double x, y;
   double val_alpha = 100;//0...255 0 full transperent
   for (int yIndex= - dim1; yIndex<dim1; ++yIndex)
   {
     for (int xIndex= - dim2; xIndex<dim2; ++xIndex)
     {
       colorMap1->data()->cellToCoord(xIndex, yIndex, &x, &y);
       colorMap1->data()->setCell(xIndex, yIndex, ez [yIndex][xIndex]); 
       colorMap1->data()->setAlpha(xIndex, yIndex, val_alpha);
     }
   }
   // add a color scale:
   QCPColorScale *colorScale = new QCPColorScale(Cp);
   Cp->plotLayout()->addElement(0, 1,colorScale); // add it to the right of the main axis rect
   colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
   colorMap1->setColorScale(colorScale); // associate the color map with the color scale
   colorScale->axis()->setLabel("Electric Field Strength");
   // set the color gradient of the color map to one of the presets:
   colorMap1->setGradient(QCPColorGradient::gpJet);
   // we could have also created a QCPColorGradient instance and added own colors to
   // the gradient, see the documentation of QCPColorGradient for what's possible.
   // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
   colorMap1->rescaleDataRange();

   // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
   QCPMarginGroup *marginGroup = new QCPMarginGroup(Cp);
   Cp->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
   colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
   // rescale the key (x) and value (y) axes so the whole color map is visible:
   Cp->rescaleAxes();
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Cp->replot();
}
ImageField::~ImageField()
{

}
