#include "ImageField.hpp"

ImageField::ImageField(QCustomPlot *CP, QString NameCP)
{
    CP->setInteractions(QCP::iRangeDrag     | QCP::iRangeZoom | QCP::iSelectAxes |
                        QCP::iSelectLegend  | QCP::iSelectPlottables);// Флаги выставляются по или, Drag - таскать, Zoom - зумить, Axes - оси, Legend - легенда, SelectPlottables - выделять

    CP->axisRect()->setupFullAxesBox();// функция axisRect выдает указатель на объект Rect; setupFullAxesBox - отрисовываем две оси
      //тут мы создали все сразу с осями
    CP->plotLayout()->insertRow(0); //insertRow(0) - функция, которая вставляет данные, здесь он пустой
    CP->setContextMenuPolicy(Qt::CustomContextMenu);

// /* //Без этоq части не работают MouseEvent
    // connect slot that ties some axis selections together (especially opposite axes):
    connect(CP, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(CP, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(CP, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

    // make bottom and left axes transfer their ranges to top and right axes:
    connect(CP->xAxis, SIGNAL(rangeChanged(QCPRange)), CP->xAxis2, SLOT(setRange(QCPRange)));
    connect(CP->yAxis, SIGNAL(rangeChanged(QCPRange)), CP->yAxis2, SLOT(setRange(QCPRange)));

    // connect some interaction slots:
    connect(CP, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(CP, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(CP, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));

    // setup policy and connect slot for context menu popup:

    connect(CP, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
// */

    //Добавляем заголовок
    QCPTextElement *title = new QCPTextElement(CP, NameCP, QFont("sans", 17, QFont::Bold));
    // Добавляем область для рисования LayOut
    CP->plotLayout()->addElement(0, 0, title);// добавляем в layOut (область рисования вне рамки) объект title
    connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));

    //ОСИ
    CP->axisRect()->setupFullAxesBox(true);
    CP->xAxis->setLabel("X");// Подписываем оси
    CP->yAxis->setLabel("Y");
    colorMapIF = new QCPColorMap(CP->xAxis, CP->yAxis);

    // add a color scale:
    QCPColorScale *colorScale = new QCPColorScale(CP);
    CP->plotLayout()->addElement(1, 1,colorScale); // add it to the right of the main axis rect
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMapIF->setColorScale(colorScale); // associate the color map with the color scale
    colorScale->axis()->setLabel("Electric Field Strength");
    // set the color gradient of the color map to one of the presets:
    colorMapIF->setGradient(QCPColorGradient::gpJet);
    // we could have also created a QCPColorGradient instance and added own colors to
    // the gradient, see the documentation of QCPColorGradient for what's possible.
    // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:

    // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
    QCPMarginGroup *marginGroup = new QCPMarginGroup(CP);
    CP->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

}

void ImageField::OpenFiles(std::string nameFile, int Nt, int &N, int &M, bool part)
{
    std::string strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\EH\\";
    std::string tmp, res;

    char numberChar[50]= {0};
    int numCount = 0;
    int n1 = 0;
    char res_prev;

    std::ifstream file_r( strPath+nameFile+std::to_string(Nt)+".txt");
    while(getline(file_r, tmp))
    {
        res += tmp;
        res += '\n';
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

    unsigned int n2 = 0;

    if (part)
    {
         ALLOC_2D(ez_IF_r,      N,     M,  double);

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

                     ez_IF_r[j + i*M]  = std::stof(numberChar);
                     n2 = 0;
                }
         }
    }
    else
    {
        ALLOC_2D(ez_IF_s,      N,     M,  double);

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

                    ez_IF_s[j + i*M]  = std::stof(numberChar);
                    n2 = 0;
               }
        }
    }
}

void ImageField::addSnapshot(QCustomPlot *Cp, int Nt, grid_fdtd *g_r, grid_fdtd *g_s, GenGrid2D *GenGr, GenGeom2D *GenGeom)
{
    // set up the QCPColorMap:
   int M_r = 0, M_s = 0, N_r = 0, N_s = 0;

   OpenFiles("Ez_Rotor", Nt, N_r, M_r, true);
   OpenFiles("Ez_Stator", Nt, N_s, M_s, false);

    int nx = 2000;
    int ny = 2000;

    colorMapIF->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
    colorMapIF->data()->setRange(QCPRange(-GenGeom->stat_par.R_out - 1.0, GenGeom->stat_par.R_out + 1.0), QCPRange(-GenGeom->stat_par.R_out - 1.0, GenGeom->stat_par.R_out + 1.0)); // and span the coordinate range -4..4 in both key (x) and value (y) dimensions
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:

    colorMapIF->setName("Color map");
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    double x_i, y_i;
    double val_alpha = 100;//0...255 0 full transperent

    colorMapIF->data()->fill (0);
    colorMapIF->data()->fillAlpha (100);
    colorMapIF->setInterpolate ( true);

    double x, y;
    for (int i = 0; i < N_r; i++)
    {
       for (int j = 0; j < M_r; j++)
            {
                x = GenGr->rot_grid_pos[j + i*M_r].x;
                y = GenGr->rot_grid_pos[j + i*M_r].y;
                int xInd = 0, yInd =0;

                colorMapIF->data()->coordToCell (x, y, &xInd, &yInd);
                colorMapIF->data()->cellToCoord(xInd, yInd, &x_i, &y_i);
                colorMapIF->data()->setCell(xInd, yInd, ez_IF_r [j + i*M_r]);
            }
    }

    for (int i = 0; i < N_s; i++)
    {
       for (int j = 0; j < M_s; j++)
            {
                x = GenGr->stat_grid_pos[j + i*M_s].x;
                y = GenGr->stat_grid_pos[j + i*M_s].y;
                int xInd = 0, yInd =0;

                colorMapIF->data()->coordToCell (x, y, &xInd, &yInd);
                colorMapIF->data()->cellToCoord(xInd, yInd, &x_i, &y_i);
                colorMapIF->data()->setCell(xInd, yInd, ez_IF_s [j + i*M_s]);
            }
    }
/*
     //ПРИМЕР (nx и ny  - число точек в выбранном диапазоне, при выгрузке массива нужно пробежаться по каждой из них по оси Х и Y)
    // now we assign some data, by accessing the QCPColorMapData instance of the color map:
    double x, y, z;
    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
      for (int yIndex=0; yIndex<ny; ++yIndex)
      {
        colorMapIF->data()->cellToCoord(xIndex, yIndex, &x, &y);
        double r = 3*qSqrt(x*x+y*y)+1e-2;
        z = 2*x*(qCos(r+2)/r-qSin(r+2)/r); // the B field strength of dipole radiation (modulo physical constants)
        colorMapIF->data()->setCell(xIndex, yIndex, z);
      }
    }
*/

    colorMapIF->rescaleDataRange();
    // rescale the key (x) and value (y) axes so the whole color map is visible:
    Cp->rescaleAxes();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Cp->replot();  
}

ImageField::~ImageField()
{

}
