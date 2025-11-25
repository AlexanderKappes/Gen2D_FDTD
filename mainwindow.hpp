 #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ImageWork.hpp"
#include "ImageField.hpp"
#include "Header.hpp"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const double Nsh_o = 500.0;
    const double dT_o = 0.001;
    const double T_o = 0.0;

    const double Imax_sin_o = 1.0;
    const double f_o = 50.0;
    const double psi_o = 0.0;

    const double TX_o = 1.0;    const double TY_o = 1.0;    const double TZ_o = 1.0;
    const int    Xsize_o = 10;      const int Ysize_o = 10;      const int Zsize_o = 10;

    double dT_em = 0.00000001;//10^(-8)

    //double c = c_light;
    //double eo = epsilon_o;
    //double no = nu_o;
    //material_air ();

    GenGeom2D   GenGeom;
    GenGrid2D   GenGrid;
    GenGraph2D  GenGraph;
    ImageWork   IW;

    BoundCond   BC;
    Source CurS = Source(Nsh_o, dT_o, Imax_sin_o, f_o, psi_o);
    Source S_Er = Source(Nsh_o, dT_o, Imax_sin_o, f_o, psi_o);
    Source S_Es = Source(Nsh_o, dT_o, Imax_sin_o, f_o, psi_o);
    Materials   Mat;

    double      arg_beg;
    QVector <double> sign_i_x, sign_i_y;

    void time_em_Label(double time);
    Ui::MainWindow *ui;

    int SP_next;
    int im_Out;
    int MaxStep;
    int Snapshot_step;

private slots:
    void titleDoubleClick(QMouseEvent* event);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    //void selectionChanged();
    //void mousePress();
    //void mouseWheel();
    void removeSelectedGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void on_pushButton_clicked();

    void on_Set_rotor_color_clicked();

    void on_Set_air_gap_color_clicked();

    void on_Set_stator_color_clicked();

    void on_show_A_coil_checkBox_toggled(bool checked);

    void on_show_B_coil_checkBox_toggled(bool checked);

    void on_show_C_coil_checkBox_toggled(bool checked);

    void on_show_L1_coil_checkBox_toggled(bool checked);

    void on_show_L2_coil_checkBox_toggled(bool checked);

    void on_only_borders_checkBox_toggled(bool checked);

    void on_show_field_coil_checkBox_toggled(bool checked);

    void on_PB_source_new_clicked();

    void ChangeFunc();

    void on_PB_nextSP_clicked();

private:
   void ParGeomSet ();
   void ParGenGridSet ();
   void ParCurSourceSet ();

   void ParGenGeomGet ();
   void ParGenGridGet ();
   void ParCurSourceGet ();

   void add_image ();
   void ColorPlace();
   void customPlot(QCustomPlot *CP, QString NameCP);
   void customPlot_ConstructSet();

};

#endif // MAINWINDOW_H
