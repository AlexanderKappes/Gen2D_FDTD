#include "ImageField.hpp"

ImageField::ImageField()
{

}
void ImageField::addSnapshot(QCustomPlot *Cp, int Nt, grid_fdtd *g, GenGrid2D *GenGr, bool part)
{
    // set up the QCPColorMap:
   int M = 0, N = 0;
   double* ez_IF ;

   std::string strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\EH\\";
   std::string tmp, res;

   char numberChar[50]= {0};
   int numCount = 0;
   int n1 = 0;
   char res_prev;

   if (part)
        {
            std::ifstream file1( strPath+"Ez_Rotor"+std::to_string(Nt)+".txt");
            while(getline(file1, tmp))
            {
                res += tmp;
                res += '\n';
            }
        }
   else
        {
            std::ifstream file1( strPath+"Ez_Stator"+std::to_string(Nt)+".txt");
            while(getline(file1, tmp))
            {
                res += tmp;
                res += '\n';
            }
        }

   for (unsigned int i = 0; i < res.length(); i++)
   {

       if (res [i] != ' ')
       {
           numberChar[n1] = res [i];
           n1++;
       }
       if (res [i] == '\n' &&  numCount == 1)
       {
           N = std::stof(numberChar);
           numCount++;
           n1 = n1 + 3;//два пробела и перенос
           break;
       }

       if (res [i] == ' ' &&  numCount == 0)
       {
           M = std::stof(numberChar);
           numCount++;
           n1 = 0;
       }
   }

    ALLOC_2D(ez_IF,      N,     M,  double);
    unsigned int n2 = 0;

    for (int i = 0; i < N; i++)
    {
       for (int j = 0; j < M; j++)
           {
                while (res [n1] != '\n' && res [n1] != '\t' )
                        {
                            numberChar[n2] = res [n1];
                            n1++;
                            n2++;
                        }
               res_prev = res [n1];
               n1++;
               if (res_prev == '\n' && res [n1] == '\n') n1++;

                ez_IF[j + i*M]  = std::stof(numberChar);
                n2 = 0;
           }
    }

    colorMap1 = new QCPColorMap(Cp->xAxis, Cp->yAxis);
    int dim1 = 2;
    int dim2 = 2;
    colorMap1->data()->setSize(dim1*1000, dim2*1000); // we want the color map to have nx * ny data points

    QCPRange vis_field_x(-dim1*1000, dim1*1000);
    QCPRange vis_field_y(-dim2*1000, dim2*1000);
    colorMap1->data()->setRange(vis_field_x, vis_field_y);
    colorMap1->setName("Color map");
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    double x_i, y_i;
    double val_alpha = 100;//0...255 0 full transperent

/*
    for (int yIndex= - dim1*1000; yIndex<dim1*1000; ++yIndex)
    {
     for (int xIndex= - dim2*1000; xIndex<dim2*1000; ++xIndex)
     {
       //colorMap1->data()->cellToCoord(xIndex, yIndex, &x, &y);
       //colorMap1->data()->setCell(xIndex, yIndex, ez_IF [yIndex + xIndex*M]);
       colorMap1->data()->setAlpha(xIndex, yIndex, val_alpha);
     }
    }
*/
    colorMap1->data()->fill (0);
    colorMap1->data()->fillAlpha (100);
    colorMap1->setInterpolate ( true);
    double x, y;
    for (int i = 0; i < N; i++)
    {
       for (int j = 0; j < M; j++)
            {
                if (part)
                {
                    x = GenGr->rot_grid_pos[j + i*M].x;
                    y = GenGr->rot_grid_pos[j + i*M].y;
                }
                else
                {
                    x = GenGr->stat_grid_pos[j + i*M].x;
                    y = GenGr->stat_grid_pos[j + i*M].y;
                }
                double z = ez_IF [j + i*M];
                int xInd = 0, yInd =0;

                colorMap1->data()->coordToCell (x*1000, y*1000, &xInd, &yInd);
                colorMap1->data()->cellToCoord(xInd, yInd, &x_i, &y_i);
                //colorMap1->data()->setAlpha(xInd, yInd, val_alpha);
                //colorMap1->data()->setData(GenGr->rot_grid_pos[j + i*M].x, GenGr->rot_grid_pos[j + i*M].y, ez_IF [j + i*M]);
                colorMap1->data()->setCell(xInd, yInd, ez_IF [j + i*M]);

                int check =0;
            }
    }

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(Cp);
    Cp->plotLayout()->addElement(1, 1,colorScale); // add it to the right of the main axis rect
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
