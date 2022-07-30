 #include "Header.hpp"

ImageWork::ImageWork()
{

}
void ImageWork::add_Point(GenGraph2D *GenGraph, GenGrid2D *GenGrid, QCustomPlot *CP)
{
    GenGraph->stator_grid_curve_Ez      = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->stator_grid_curve_Hx      = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->stator_grid_curve_Hy      = new QCPCurve(CP->xAxis, CP->yAxis);

    GenGraph->rotor_grid_curve_Ez       = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->rotor_grid_curve_Hx       = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->rotor_grid_curve_Hy       = new QCPCurve(CP->xAxis, CP->yAxis);

    GenGraph->stator_grid_curve_Ez       ->data()->set(GenGraph->QV_stator_grid_Ez, true);
    GenGraph->stator_grid_curve_Ez       ->setName("stator grid Ez");
    GenGraph->stator_grid_curve_Ez       ->setPen(QPen(Qt::black,1,Qt::SolidLine));
    GenGraph->stator_grid_curve_Ez       ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, GenGrid->grid_mark_size));
    GenGraph->stator_grid_curve_Ez       ->setLineStyle((QCPCurve::LineStyle)0);

    GenGraph->stator_grid_curve_Hx       ->data()->set(GenGraph->QV_stator_grid_Hx, true);
    GenGraph->stator_grid_curve_Hx       ->setName("stator grid Hx");
    GenGraph->stator_grid_curve_Hx       ->setPen(QPen(Qt::black,1,Qt::SolidLine));
    GenGraph->stator_grid_curve_Hx       ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, GenGrid->grid_mark_size));
    GenGraph->stator_grid_curve_Hx       ->setLineStyle((QCPCurve::LineStyle)0);

    GenGraph->stator_grid_curve_Hy       ->data()->set(GenGraph->QV_stator_grid_Hy, true);
    GenGraph->stator_grid_curve_Hy       ->setName("stator grid Hy");
    GenGraph->stator_grid_curve_Hy       ->setPen(QPen(Qt::black,1,Qt::SolidLine));
    GenGraph->stator_grid_curve_Hy       ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, GenGrid->grid_mark_size));
    GenGraph->stator_grid_curve_Hy       ->setLineStyle((QCPCurve::LineStyle)0);

    //В Классе QCP Curve нет массива данных, мы его подпихиваем через пустой QV_rotor_grid
    GenGraph->rotor_grid_curve_Ez        ->data()->set(GenGraph->QV_rotor_grid_Ez, true);//QV_rotor_grid - Массив, которые подпихивается в массив rotor_grid_curve
    GenGraph->rotor_grid_curve_Ez        ->setName("rotor grid Ez");
    GenGraph->rotor_grid_curve_Ez        ->setPen(QPen(Qt::black,1,Qt::SolidLine));//Qt::NoPen
    GenGraph->rotor_grid_curve_Ez        ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, GenGrid->grid_mark_size));
    GenGraph->rotor_grid_curve_Ez        ->setLineStyle((QCPCurve::LineStyle)0);

    GenGraph->rotor_grid_curve_Hx        ->data()->set(GenGraph->QV_rotor_grid_Hx, true);//QV_rotor_grid - Массив, которые подпихивается в массив rotor_grid_curve
    GenGraph->rotor_grid_curve_Hx        ->setName("rotor grid Hx");
    GenGraph->rotor_grid_curve_Hx        ->setPen(QPen(Qt::black,1,Qt::SolidLine));//Qt::NoPen
    GenGraph->rotor_grid_curve_Hx        ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, GenGrid->grid_mark_size));
    GenGraph->rotor_grid_curve_Hx        ->setLineStyle((QCPCurve::LineStyle)0);

    GenGraph->rotor_grid_curve_Hy        ->data()->set(GenGraph->QV_rotor_grid_Hy, true);//QV_rotor_grid - Массив, которые подпихивается в массив rotor_grid_curve
    GenGraph->rotor_grid_curve_Hy        ->setName("rotor grid Hy");
    GenGraph->rotor_grid_curve_Hy        ->setPen(QPen(Qt::black,1,Qt::SolidLine));//Qt::NoPen
    GenGraph->rotor_grid_curve_Hy        ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, GenGrid->grid_mark_size));
    GenGraph->rotor_grid_curve_Hy        ->setLineStyle((QCPCurve::LineStyle)0);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    CP->replot();
}

void ImageWork::add_Curve(Ui::MainWindow *ui, GenGraph2D *GenGraph, GenGeom2D *GenGeom, QCustomPlot *CP, bool brush, bool coil)
{
    GenGraph->gener_total_curve         = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->stator_steel_curve        = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->rotor_steel_curve         = new QCPCurve(CP->xAxis, CP->yAxis);

    GenGraph->rotor_R_curve             = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->rotor_w_curve             = new QCPCurve(CP->xAxis, CP->yAxis);

    GenGraph->stator_R_curve            = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->stator_w_curve            = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->stator_L1_curve           = new QCPCurve(CP->xAxis, CP->yAxis);
    GenGraph->stator_L2_curve           = new QCPCurve(CP->xAxis, CP->yAxis);

    if (coil)
    {
        GenGraph->stator_coil_A1_curve      = new QCPCurve(CP->xAxis, CP->yAxis);
        GenGraph->stator_coil_B1_curve      = new QCPCurve(CP->xAxis, CP->yAxis);
        GenGraph->stator_coil_C1_curve      = new QCPCurve(CP->xAxis, CP->yAxis);

        GenGraph->stator_coil_A2_curve      = new QCPCurve(CP->xAxis, CP->yAxis);
        GenGraph->stator_coil_B2_curve      = new QCPCurve(CP->xAxis, CP->yAxis);
        GenGraph->stator_coil_C2_curve      = new QCPCurve(CP->xAxis, CP->yAxis);

        GenGraph->rotor_coil_curve          = new QCPCurve(CP->xAxis, CP->yAxis);
    }

    if (GenGeom->rot_par.n_layer >= 1)
    {
        GenGraph->rotor_L1_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }
    if (GenGeom->rot_par.n_layer >= 2)
    {
        GenGraph->rotor_L2_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }
    if (GenGeom->rot_par.n_layer >= 3)
    {
        GenGraph->rotor_L3_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }
    if (GenGeom->rot_par.n_layer >= 4)
    {
        GenGraph->rotor_L4_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }
    if (GenGeom->rot_par.n_layer >= 5)
    {
        GenGraph->rotor_L5_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }
    if (GenGeom->rot_par.n_layer >= 6)
    {
        GenGraph->rotor_L6_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }
    if (GenGeom->rot_par.n_layer >= 7)
    {
        GenGraph->rotor_L7_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }
    if (GenGeom->rot_par.n_layer >= 8)
    {
        GenGraph->rotor_L8_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }
    if (GenGeom->rot_par.n_layer >= 9)
    {
        GenGraph->rotor_L9_curve = new QCPCurve(CP->xAxis, CP->yAxis);
    }

    GenGraph->gener_total_curve       ->data()->set(GenGraph->gener_total, true);
    GenGraph->gener_total_curve       ->setName("stator");
    if (brush)
    {
        QBrush stator_color_Brush         = ui->PB_stator_color->palette().button().color();
        GenGraph->gener_total_curve       ->setBrush(stator_color_Brush);
        GenGraph->gener_total_curve       ->setPen(QPen(stator_color_Brush.color(),3,Qt::SolidLine));
    }

    //GGr.gener_total_curve     ->setPen(QPen(Qt::black,1,Qt::SolidLine));// Тип линии
    //GGr.gener_total_curve     ->setLineStyle((QCPCurve::LineStyle)1);//Еще один стиль линии

    GenGraph->stator_steel_curve      ->data()->set(GenGraph->stator_steel, true);
    GenGraph->stator_steel_curve      ->setName("air gap");
    if (brush)
    {
        QBrush air_gap_Brush             = ui->PB_air_gap_color->palette().button().color();
        GenGraph->stator_steel_curve      ->setBrush(air_gap_Brush);
        GenGraph->stator_steel_curve      ->setPen(QPen(air_gap_Brush.color(),3,Qt::SolidLine));
    }
    //GGr.stator_steel_curve    ->setPen(QPen(Qt::black,1,Qt::SolidLine));// Тип линии
    //GGr.stator_steel_curve    ->setLineStyle((QCPCurve::LineStyle)1);//Еще один стиль линии

    GenGraph->rotor_steel_curve       ->data()->set(GenGraph->rot_steel, true);
    GenGraph->rotor_steel_curve       ->setName("rotor");
    if (brush)
    {
        QBrush rot_color_Brush           = ui->PB_rotor_color->palette().button().color();
        GenGraph->rotor_steel_curve       ->setBrush(rot_color_Brush);
        GenGraph->rotor_steel_curve       ->setPen(QPen(rot_color_Brush.color(),3,Qt::SolidLine));
    }

    //GGr.rotor_steel_curve     ->setPen(QPen(Qt::black,1,Qt::SolidLine));// Тип линии
    //GGr.rotor_steel_curve     ->setLineStyle((QCPCurve::LineStyle)1);//Еще один стиль линии

    if (coil)
    {
        GenGraph->stator_coil_A1_curve    ->data()->set(GenGraph->stator_coil_A1, true);//это пунктирная линия соединяющая точки обмотки
        GenGraph->stator_coil_A1_curve    ->setName("A1 coil");
        GenGraph->stator_coil_A1_curve    ->setPen(QPen(Qt::yellow,1,Qt::SolidLine));// Тип линии
        GenGraph->stator_coil_A1_curve    ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));// тип точек внутри линии
        GenGraph->stator_coil_A1_curve    ->setLineStyle((QCPCurve::LineStyle)1);//Еще один стиль линии

        GenGraph->stator_coil_B1_curve    ->data()->set(GenGraph->stator_coil_B1, true);
        GenGraph->stator_coil_B1_curve    ->setName("B1 coil");
        GenGraph->stator_coil_B1_curve    ->setPen(QPen(Qt::green,1,Qt::SolidLine));
        GenGraph->stator_coil_B1_curve    ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
        GenGraph->stator_coil_B1_curve    ->setLineStyle((QCPCurve::LineStyle)1);

        GenGraph->stator_coil_C1_curve    ->data()->set(GenGraph->stator_coil_C1, true);
        GenGraph->stator_coil_C1_curve    ->setName("C1 coil");
        GenGraph->stator_coil_C1_curve    ->setPen(QPen(Qt::red,1,Qt::SolidLine));
        GenGraph->stator_coil_C1_curve    ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
        GenGraph->stator_coil_C1_curve    ->setLineStyle((QCPCurve::LineStyle)1);

        GenGraph->stator_coil_A2_curve    ->data()->set(GenGraph->stator_coil_A2, true);
        GenGraph->stator_coil_A2_curve    ->setName("A2 coil");
        GenGraph->stator_coil_A2_curve    ->setPen(QPen(Qt::yellow,1,Qt::SolidLine));
        GenGraph->stator_coil_A2_curve    ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
        GenGraph->stator_coil_A2_curve    ->setLineStyle((QCPCurve::LineStyle)1);

        GenGraph->stator_coil_B2_curve    ->data()->set(GenGraph->stator_coil_B2, true);
        GenGraph->stator_coil_B2_curve    ->setName("B2 coil");
        GenGraph->stator_coil_B2_curve    ->setPen(QPen(Qt::green,1,Qt::SolidLine));
        GenGraph->stator_coil_B2_curve    ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
        GenGraph->stator_coil_B2_curve    ->setLineStyle((QCPCurve::LineStyle)1);

        GenGraph->stator_coil_C2_curve    ->data()->set(GenGraph->stator_coil_C2, true);
        GenGraph->stator_coil_C2_curve    ->setName("C2 coil");
        GenGraph->stator_coil_C2_curve    ->setPen(QPen(Qt::red,1,Qt::SolidLine));
        GenGraph->stator_coil_C2_curve    ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
        GenGraph->stator_coil_C2_curve    ->setLineStyle((QCPCurve::LineStyle)1);

        GenGraph->rotor_coil_curve        ->data()->set(GenGraph->rotor_coil, true);
        GenGraph->rotor_coil_curve        ->setName("rotor coil");
        GenGraph->rotor_coil_curve        ->setPen(QPen(Qt::blue,1,Qt::DashLine));
        GenGraph->rotor_coil_curve        ->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
        GenGraph->rotor_coil_curve        ->setLineStyle((QCPCurve::LineStyle)1);
    }

    GenGraph->stator_R_curve          ->data()->set(GenGraph->stator_R, true);
    GenGraph->stator_R_curve          ->setName("stator internal border");
    GenGraph->stator_R_curve          ->setPen(QPen(Qt::black,1,Qt::DotLine));

    GenGraph->stator_w_curve          ->data()->set(GenGraph->stator_w, true);
    GenGraph->stator_w_curve          ->setName("stator wedge");
    GenGraph->stator_w_curve          ->setPen(QPen(Qt::black,1,Qt::DashLine));

    GenGraph->stator_L1_curve         ->data()->set(GenGraph->stator_L1, true);
    GenGraph->stator_L1_curve         ->setName("stator slot layer1");
    GenGraph->stator_L1_curve         ->setPen(QPen(Qt::black,1,Qt::DashLine));

    GenGraph->stator_L2_curve         ->data()->set(GenGraph->stator_L2, true);
    GenGraph->stator_L2_curve         ->setName("stator slot layer2");
    GenGraph->stator_L2_curve         ->setPen(QPen(Qt::black,1,Qt::DotLine));

    GenGraph->rotor_R_curve           ->data()->set(GenGraph->rotor_R, true);
    GenGraph->rotor_R_curve           ->setName("rotor border");
    GenGraph->rotor_R_curve           ->setPen(QPen(Qt::black,1,Qt::DotLine));

    GenGraph->rotor_w_curve           ->data()->set(GenGraph->rotor_w, true);
    GenGraph->rotor_w_curve           ->setName("rotor wedge");
    GenGraph->rotor_w_curve           ->setPen(QPen(Qt::black,1,Qt::DashLine));

    if (GenGeom->rot_par.n_layer >= 1)
    {
        GenGraph->rotor_L1_curve->data()->set(GenGraph->rotor_L1, true);
        GenGraph->rotor_L1_curve->setName("rotor slot layer1");
        GenGraph->rotor_L1_curve->setPen(QPen(Qt::black,1,Qt::DashLine));
    }
    if (GenGeom->rot_par.n_layer >= 2)
    {
        GenGraph->rotor_L2_curve->data()->set(GenGraph->rotor_L2, true);
        GenGraph->rotor_L2_curve->setName("rotor slot layer2");
        GenGraph->rotor_L2_curve->setPen(QPen(Qt::black,1,Qt::DashLine));
    }
    if (GenGeom->rot_par.n_layer >= 3)
    {
        GenGraph->rotor_L3_curve->data()->set(GenGraph->rotor_L3, true);
        GenGraph->rotor_L3_curve->setName("rotor slot layer3");
        GenGraph->rotor_L3_curve->setPen(QPen(Qt::black,1,Qt::DotLine));
    }
    if (GenGeom->rot_par.n_layer >= 4)
    {
        GenGraph->rotor_L4_curve->data()->set(GenGraph->rotor_L4, true);
        GenGraph->rotor_L4_curve->setName("rotor slot layer4");
        GenGraph->rotor_L4_curve->setPen(QPen(Qt::black,1,Qt::DashLine));
    }
    if (GenGeom->rot_par.n_layer >= 5)
    {
        GenGraph->rotor_L5_curve->data()->set(GenGraph->rotor_L5, true);
        GenGraph->rotor_L5_curve->setName("rotor slot layer5");
        GenGraph->rotor_L5_curve->setPen(QPen(Qt::black,1,Qt::DashLine));
    }
    if (GenGeom->rot_par.n_layer >= 6)
    {
        GenGraph->rotor_L6_curve->data()->set(GenGraph->rotor_L6, true);
        GenGraph->rotor_L6_curve->setName("rotor slot layer6");
        GenGraph->rotor_L6_curve->setPen(QPen(Qt::black,1,Qt::DotLine));
    }
    if (GenGeom->rot_par.n_layer >= 7)
    {
        GenGraph->rotor_L7_curve->data()->set(GenGraph->rotor_L7, true);
        GenGraph->rotor_L7_curve->setName("rotor slot layer7");
        GenGraph->rotor_L7_curve->setPen(QPen(Qt::black,1,Qt::DashLine));
    }
    if (GenGeom->rot_par.n_layer >= 8)
    {
        GenGraph->rotor_L8_curve->data()->set(GenGraph->rotor_L8, true);
        GenGraph->rotor_L8_curve->setName("rotor slot layer8");
        GenGraph->rotor_L8_curve->setPen(QPen(Qt::black,1,Qt::DashLine));
    }
    if (GenGeom->rot_par.n_layer >= 9)
    {
        GenGraph->rotor_L9_curve->data()->set(GenGraph->rotor_L9, true);
        GenGraph->rotor_L9_curve->setName("rotor slot layer9");
        GenGraph->rotor_L9_curve->setPen(QPen(Qt::black,1,Qt::DotLine));
    }

    CP->replot();
}

/*
void ImageWork::ChangeFunc(Ui::MainWindow *ui, CurrentSource *CurS)
{

    ui->sign_gr_w->clearGraphs();
    sign_i_x.clear();
    sign_i_y.clear();

    for(int i = 0; i < CurS->Nsh; i++)
    {
        if (i == 0)
        {ui->sign_gr_w->addGraph();}

        ui->sign_gr_w->xAxis->setRange(0,CurS->time [CurS->Nsh-1]);
        ui->sign_gr_w->yAxis->setRange(-(CurS->Imax_sin + 1.0), CurS->Imax_sin + 1.0);
        sign_i_x.insert(i, CurS->time [i]);
        sign_i_y.insert(i, CurS->I_sin[i]);
    }
    ui->sign_gr_w->graph(0)->addData(sign_i_x,sign_i_y);
    ui->sign_gr_w->replot();
}
*/
ImageWork::~ImageWork()
{

}
