#include "Header.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //srand(QDateTime::currentDateTime().toTime_t());
    ui->setupUi(this);

    double start_time =  clock(); // начальное время

    arg_beg = 0.0 * M_PI/180;

    GenGeom.init_rotor   ();
    GenGeom.init_stator  ();
    GenGrid.Gen_Grid_Vector_init(&GenGeom);
    GenGrid.Gen_Grid_Calc(&GenGeom);
    GenGrid.Gen_Grid_Pos_rot(&GenGeom, arg_beg);
    GenGrid.Gen_Grid_Pos_stat(&GenGeom);
    Mat.Materials_array(&GenGrid, true);
    Mat.Materials_array(&GenGrid, false);

    //Установить в окошках параметры по умолчанию
    ParGeomSet      ();
    ParGenGridSet   ();

    GenGraph.GenGraphCurveSize (&GenGeom);

    GenGraph.rotor_steel_curve_Create       (&GenGeom, arg_beg);
    GenGraph.GridPoint_rotor                (&GenGrid);
    GenGraph.stator_coil_curve_Create       (&GenGeom);
    GenGraph.stator_steel_curve_Create      (&GenGeom);
    GenGraph.GridPoint_stator               (&GenGrid);
    GenGraph.rotor_coil_curve_Create        (&GenGeom,  &GenGrid, arg_beg);
    IW.add_Curve(ui, &GenGraph, &GenGeom, ui->CP_Construct, true, true);
    IW.add_Point(&GenGraph, &GenGrid, ui->CP_Construct);

    IW.add_Curve(ui, &GenGraph, &GenGeom, ui->CP_EM_Field, false, false);

    CurS.SourceSinSize();
    CurS.SourceSinCreate ();
    ParCurSourceSet ();
    ChangeFunc();
    customPlot(ui->CP_Construct, "Generator geometry");
    customPlot_ConstructSet();

    ui->CP_Construct->replot();

    double end_time = clock(); // конечное время
    double search_time = end_time - start_time; // искомое время
    ui->time_prepare->setText(QString::fromStdString(std::to_string(search_time) + " мс"));
}

void MainWindow::customPlot(QCustomPlot *CP, QString NameCP)
{
    CP->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);// Флаги выставляются по или, Drag - таскать, Zoom - зумить, Axes - оси, Legend - легенда, SelectPlottables - выделять

    CP->axisRect()->setupFullAxesBox();// функция axisRect выдает указатель на объект Rect; setupFullAxesBox - отрисовываем две оси
      //тут мы создали все сразу с осями
    CP->plotLayout()->insertRow(0); //insertRow(0) - функция, которая вставляет данные, здесь он пустой

    //Подписываем оси
    CP->xAxis->setLabel("x Axis");// Подписываем оси
    CP->yAxis->setLabel("y Axis");


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
    CP->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(CP, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
// */
    //Добавляем заголовок
    QCPTextElement *title = new QCPTextElement(CP, NameCP, QFont("sans", 17, QFont::Bold));
    // Добавляем область для рисования LayOut
    CP->plotLayout()->addElement(0, 0, title);// добавляем в layOut (область рисования вне рамки) объект title
    connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));

    CP->rescaleAxes();
}
void MainWindow::customPlot_ConstructSet()
{

    //Создаем легенду
    ui->CP_Construct->legend->setVisible(true);//создаем легенду
    QFont legendFont = font();// конструктор для объекта QFont
    legendFont.setPointSize(10);
    ui->CP_Construct->legend->setFont(legendFont);//устанавливаем объекту legend тип legendFont
    ui->CP_Construct->legend->setSelectedFont(legendFont);//Установим два разных шрифта - на обычный режим и на выделенный
    ui->CP_Construct->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

    ui->PB_stator_color->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 255);"));
    ui->PB_stator_color->update();

    ui->PB_rotor_color->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 0);"));
    ui->PB_rotor_color->update();

    ui->PB_air_gap_color->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);"));
    ui->PB_air_gap_color->update();

    //----------------------------------------------
    ui->only_borders_checkBox   ->setChecked(false);
    ui->show_field_coil_checkBox->setChecked(true);
    ui->show_A_coil_checkBox    ->setChecked(true);
    ui->show_B_coil_checkBox    ->setChecked(true);
    ui->show_C_coil_checkBox    ->setChecked(true);
    ui->show_L1_coil_checkBox   ->setChecked(true);
    ui->show_L2_coil_checkBox   ->setChecked(true);
    //----------------------------------------------
}

void MainWindow::ChangeFunc()
{

    ui->sign_gr_w->clearGraphs();
    sign_i_x.clear();
    sign_i_y.clear();

    for(int i = 0; i < CurS.Nsh; i++)
    {
        if (i == 0)
        {ui->sign_gr_w->addGraph();}

        ui->sign_gr_w->xAxis->setRange(0,CurS.time [CurS.Nsh-1]);
        ui->sign_gr_w->yAxis->setRange(-(CurS.Imax_sin + 1.0), CurS.Imax_sin + 1.0);
        sign_i_x.insert(i, CurS.time [i]);
        sign_i_y.insert(i, CurS.I_sin[i]);
    }
    ui->sign_gr_w->graph(0)->addData(sign_i_x,sign_i_y);
    ui->sign_gr_w->replot();
}


void MainWindow::ParCurSourceSet ()
{
    ui->SB_Nsh      ->setValue( CurS.Nsh);
    ui->SB_t_sh     ->setValue( CurS.dT);
    ui->SB_Imax_sin ->setValue( CurS.Imax_sin);
    ui->SB_Psi_0    ->setValue( CurS.Psi_0);
    ui->SB_f        ->setValue( CurS.f);
}

void MainWindow::ParCurSourceGet ()
{

    CurS.Nsh            = ui->SB_Nsh        ->value();
    CurS.dT             = ui->SB_t_sh       ->value();
    CurS.Imax_sin       = ui->SB_Imax_sin   ->value();
    CurS.Psi_0          = ui->SB_Psi_0      ->value();
    CurS.f              = ui->SB_f          ->value();
}

void MainWindow::ParGeomSet ()
{
    ui->SB_N_poles             ->setValue( GenGeom.rot_par.n_pole);
    ui->SB_rotor_num_slot      ->setValue( GenGeom.rot_par.n_coil_slot);
    ui->SB_rotor_total_num_slot->setValue( GenGeom.rot_par.n_slot_pin);
    ui->SB_rotor_coil_layer    ->setValue( GenGeom.rot_par.n_layer);

    ui->SB_rotor_d             ->setValue(2* GenGeom.rot_par.R);
    ui->SB_rotor_slot_w        ->setValue( GenGeom.rot_par.h_wedge);
    ui->SB_rotor_slot_b        ->setValue( GenGeom.rot_par.b_slot);
    ui->SB_rotor_slot_h        ->setValue( GenGeom.rot_par.h_slot);
    //********************************************************************************
    ui->rotor_slot_angle       ->setText(QString::fromStdString(std::to_string(GenGeom.rot_par.slot_arg*180/3.1415926)));
    ui->rotor_pin_angle        ->setText(QString::fromStdString(std::to_string(GenGeom.rot_par.pin_arg*180/3.1415926)));
    //********************************************************************************

    ui->SB_air_gap             ->setValue( GenGeom.air_gap);
    ui->SB_stator_l            ->setValue( GenGeom.L);

    ui->SB_stator_num_slot     ->setValue( GenGeom.stat_par.n_slot_pin);
    ui->SB_stator_coil_layer   ->setValue( GenGeom.stat_par.n_layer);
    ui->SB_stator_slot_shift   ->setValue( GenGeom.stat_par.shift_coil);
    ui->SB_stator_d            ->setValue(2* GenGeom.stat_par.R_out);
    ui->SB_stator_slot_w       ->setValue( GenGeom.stat_par.h_wedge);
    ui->SB_stator_slot_b       ->setValue( GenGeom.stat_par.b_slot);
    ui->SB_stator_slot_h       ->setValue( GenGeom.stat_par.h_slot);

    ui->SB_a0                  ->setValue( arg_beg*180/M_PI);
    //********************************************************************************
    ui->stator_slot_angle       ->setText(QString::fromStdString(std::to_string(GenGeom.stat_par.slot_arg*180/3.1415926)));
    ui->stator_pin_angle        ->setText(QString::fromStdString(std::to_string(GenGeom.stat_par.pin_arg *180/3.1415926)));
    //********************************************************************************
}

void MainWindow::ParGenGridSet ()
{
    ui->SB_rotor_wedge_level   ->setValue(GenGrid.rotor_grid_par.n_level_wedge);
    ui->SB_rotor_arm_level     ->setValue(GenGrid.rotor_grid_par.n_level_arm);
    ui->SB_rotor_body_level    ->setValue(GenGrid.rotor_grid_par.n_level_body);

    ui->SB_stator_wedge_level  ->setValue(GenGrid.stator_grid_par.n_level_wedge);
    ui->SB_stator_arm_level    ->setValue(GenGrid.stator_grid_par.n_level_arm);
    ui->SB_stator_body_level   ->setValue(GenGrid.stator_grid_par.n_level_body);
    ui->SB_stator_air_level    ->setValue(GenGrid.stator_grid_par.n_level_air);

    ui->SB_grid_mark_size     ->setValue(GenGrid.grid_mark_size );

}

void MainWindow::ParGenGeomGet ()

{     
        GenGeom.rot_par.n_pole      = ui->SB_N_poles->value();
        GenGeom.rot_par.n_coil_slot = ui->SB_rotor_num_slot->value();
        GenGeom.rot_par.n_slot_pin  = ui->SB_rotor_total_num_slot->value();
        GenGeom.rot_par.n_layer     = ui->SB_rotor_coil_layer->value();
        GenGeom.rot_par.R           = ui->SB_rotor_d->value()/2;
        GenGeom.rot_par.h_wedge     = ui->SB_rotor_slot_w->value();
        GenGeom.rot_par.b_slot      = ui->SB_rotor_slot_b->value();
        GenGeom.rot_par.h_slot      = ui->SB_rotor_slot_h->value();

        GenGeom.air_gap             =  ui->SB_air_gap->value();
        GenGeom.L                   =  ui->SB_stator_l->value();

        GenGeom.stat_par.n_slot_pin = ui->SB_stator_num_slot->value();
        GenGeom.stat_par.n_layer    = ui->SB_stator_coil_layer->value();
        GenGeom.stat_par.shift_coil = ui->SB_stator_slot_shift->value();
        GenGeom.stat_par.R_out      = ui->SB_stator_d->value()/2;
        GenGeom.stat_par.h_wedge    = ui->SB_stator_slot_w->value();
        GenGeom.stat_par.b_slot     = ui->SB_stator_slot_b->value();
        GenGeom.stat_par.h_slot     = ui->SB_stator_slot_h->value();
        arg_beg                     = ui->SB_a0->value()*M_PI/180;

}

void MainWindow::ParGenGridGet ()
{
    GenGrid.rotor_grid_par.n_level_wedge    = ui->SB_rotor_wedge_level  ->value();
    GenGrid.rotor_grid_par.n_level_arm      = ui->SB_rotor_arm_level    ->value();
    GenGrid.rotor_grid_par.n_level_body     = ui->SB_rotor_body_level   ->value();

    GenGrid.stator_grid_par.n_level_wedge   = ui->SB_stator_wedge_level ->value();
    GenGrid.stator_grid_par.n_level_arm     = ui->SB_stator_arm_level   ->value();
    GenGrid.stator_grid_par.n_level_body    = ui->SB_stator_body_level  ->value();
    GenGrid.stator_grid_par.n_level_air     = ui->SB_stator_air_level   ->value();

    GenGrid.grid_mark_size                  = ui->SB_grid_mark_size   ->value();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    GenGraph.QV_rotor_grid_Ez.clear();
    GenGraph.QV_rotor_grid_Hx.clear();
    GenGraph.QV_rotor_grid_Hy.clear();

    GenGraph.QV_stator_grid_Ez.clear();
    GenGraph.QV_stator_grid_Hx.clear();
    GenGraph.QV_stator_grid_Hy.clear();

    GenGrid.rot_grid_pos.clear();
    GenGrid.stat_grid_pos.clear();

    ui->CP_Construct->clearPlottables ();

    //Установить в объектах параметры из окошек
    ParGenGeomGet ();
    ParGenGridGet ();

    GenGeom.init_rotor();
    GenGeom.init_stator();
    GenGrid.Gen_Grid_Vector_init(&GenGeom);
    GenGrid.Gen_Grid_Calc(&GenGeom);
    GenGrid.Gen_Grid_Pos_rot(&GenGeom, arg_beg);
    GenGrid.Gen_Grid_Pos_stat(&GenGeom);

    GenGraph.GenGraphCurveSize (&GenGeom);

    GenGraph.rotor_steel_curve_Create       (&GenGeom, arg_beg);
    GenGraph.GridPoint_rotor                (&GenGrid);
    GenGraph.stator_coil_curve_Create       (&GenGeom);
    GenGraph.stator_steel_curve_Create      (&GenGeom);
    GenGraph.GridPoint_stator               (&GenGrid);
    GenGraph.rotor_coil_curve_Create        (&GenGeom,  &GenGrid, arg_beg);
    IW.add_Curve(ui, &GenGraph, &GenGeom, ui->CP_Construct, true, true);
    IW.add_Point(&GenGraph, &GenGrid, ui->CP_Construct);

}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->CP_Construct->replot();
    }
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->CP_Construct->replot();
    }
  }
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->CP_Construct->replot();
    }
  }
}

/*
void MainWindow::selectionChanged()
{

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot1->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot1->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot1->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot1->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot1->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot1->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot1->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot1->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot1->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot1->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot1->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot1->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot1->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot1->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot1->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (ui->customPlot1->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot1->axisRect()->setRangeDrag(ui->customPlot1->xAxis->orientation());
  else if (ui->customPlot1->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot1->axisRect()->setRangeDrag(ui->customPlot1->yAxis->orientation());
  else
    ui->customPlot1->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->customPlot1->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot1->axisRect()->setRangeZoom(ui->customPlot1->xAxis->orientation());
  else if (ui->customPlot1->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot1->axisRect()->setRangeZoom(ui->customPlot1->yAxis->orientation());
  else
    ui->customPlot1->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}
*/

void MainWindow::removeSelectedGraph()
{
  if (ui->CP_Construct->selectedGraphs().size() > 0)
  {
    ui->CP_Construct->removeGraph(ui->CP_Construct->selectedGraphs().first());
    ui->CP_Construct->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->CP_Construct->clearGraphs();
  ui->CP_Construct->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->CP_Construct->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    //Как выполнить эти условия?
    if (ui->CP_Construct->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->CP_Construct->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }
  menu->popup(ui->CP_Construct->mapToGlobal(pos));
}

void MainWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->CP_Construct->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->CP_Construct->replot();
    }
  }
}

void MainWindow::on_Set_air_gap_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::red, this);
    if (color.isValid())
    {
        QString qss = QString("background-color: %1").arg(color.name());
        ui->PB_air_gap_color->setStyleSheet(qss);
        ui->PB_air_gap_color->update();

        GenGraph.stator_steel_curve->setBrush(QBrush(color,Qt::SolidPattern));
        //stator_steel_curve->setPen(color);
        GenGraph.stator_steel_curve->setPen(QPen(color,3,Qt::SolidLine));

        ui->CP_Construct->replot();
    }
}

void MainWindow::on_Set_rotor_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid())
    {
        QString qss = QString("background-color: %1").arg(color.name());
        ui->PB_rotor_color->setStyleSheet(qss);
        ui->PB_rotor_color->update();

        GenGraph.rotor_steel_curve->setBrush(QBrush(color,Qt::SolidPattern));
        //rotor_steel_curve->setPen(color);
        GenGraph.rotor_steel_curve->setPen(QPen(color,3,Qt::SolidLine));

        ui->CP_Construct->replot();
    }
}

void MainWindow::on_Set_stator_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue, this);
    if (color.isValid())
    {
        QString qss = QString("background-color: %1").arg(color.name());
        ui->PB_stator_color->setStyleSheet(qss);
        ui->PB_stator_color->update();

        GenGraph.gener_total_curve->setBrush(QBrush(color,Qt::SolidPattern));
       // gener_total_curve->setPen(color);
        GenGraph.gener_total_curve->setPen(QPen(color,3,Qt::SolidLine));

        ui->CP_Construct->replot();
    }
}

void MainWindow::on_show_A_coil_checkBox_toggled(bool checked)
{
    if(ui->show_L1_coil_checkBox->isChecked())    GenGraph.stator_coil_A1_curve->setVisible(checked);
    if(ui->show_L2_coil_checkBox->isChecked())    GenGraph.stator_coil_A2_curve->setVisible(checked);
    ui->CP_Construct->replot();
}

void MainWindow::on_show_B_coil_checkBox_toggled(bool checked)
{
    if(ui->show_L1_coil_checkBox->isChecked())    GenGraph.stator_coil_B1_curve->setVisible(checked);
    if(ui->show_L2_coil_checkBox->isChecked())    GenGraph.stator_coil_B2_curve->setVisible(checked);
    ui->CP_Construct->replot();
}

void MainWindow::on_show_C_coil_checkBox_toggled(bool checked)
{
    if(ui->show_L1_coil_checkBox->isChecked())    GenGraph.stator_coil_C1_curve->setVisible(checked);
    if(ui->show_L2_coil_checkBox->isChecked())    GenGraph.stator_coil_C2_curve->setVisible(checked);
    ui->CP_Construct->replot();
}

void MainWindow::on_show_L1_coil_checkBox_toggled(bool checked)
{
    if(ui->show_A_coil_checkBox->isChecked())    GenGraph.stator_coil_A1_curve->setVisible(checked);
    if(ui->show_B_coil_checkBox->isChecked())    GenGraph.stator_coil_B1_curve->setVisible(checked);
    if(ui->show_C_coil_checkBox->isChecked())    GenGraph.stator_coil_C1_curve->setVisible(checked);
    ui->CP_Construct->replot();
}

void MainWindow::on_show_L2_coil_checkBox_toggled(bool checked)
{
    if(ui->show_A_coil_checkBox->isChecked())    GenGraph.stator_coil_A2_curve->setVisible(checked);
    if(ui->show_B_coil_checkBox->isChecked())    GenGraph.stator_coil_B2_curve->setVisible(checked);
    if(ui->show_C_coil_checkBox->isChecked())    GenGraph.stator_coil_C2_curve->setVisible(checked);
    ui->CP_Construct->replot();
}

void MainWindow::on_only_borders_checkBox_toggled(bool checked)
{

}

void MainWindow::on_show_field_coil_checkBox_toggled(bool checked)
{

    if(ui->show_field_coil_checkBox->isChecked())
        GenGraph.rotor_coil_curve->setVisible(checked);
    else
        GenGraph.rotor_coil_curve->setVisible(false);

    ui->CP_Construct->replot();
}

void MainWindow::on_PB_source_new_clicked()
{
    ParCurSourceGet ();
    CurS.SourceSinSize();
    CurS.SourceSinCreate ();
    ChangeFunc();
}

void MainWindow::time_em_Label(double time)
{
    ui->time_em->setText(QString::fromStdString(std::to_string(time) + " мс"));
}
