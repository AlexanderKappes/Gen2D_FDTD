﻿#include "Header.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ImageField  FI(w.ui->CP_EM_Field, "Electromagnetic field");
    double start_time =  clock(); // начальное время

    int im_Out = 1;
    // при dT_em = 0.00000001;//10^(-8) // im_Out = 10000000 // maxTime = 0.1 сек
    double maxTime = w.dT_em*im_Out;

    grid_fdtd   g_r;
    grid_fdtd   g_s;
    Fdtd_calc   FDTD_R;
    Fdtd_calc   FDTD_S;

    std::string strPath_j = "D:\\work\\Gen2D_FDTD\\TextFiles\\";
    int N_Ej = w.GenGrid.rotor_grid_par.Col/2;
    int N_Hj = w.GenGrid.stator_grid_par.Col/2;

    w.Mat.gridInit(&g_r, &w.GenGrid, w.dT_em, true);
    w.Mat.gridInit(&g_s, &w.GenGrid, w.dT_em, false);

    w.S_Er.SourceE (&g_r, &w.GenGrid, true);
    w.S_Es.SourceE (&g_s, &w.GenGrid, false);
    //snapshotInit2d(go); // initialize snapshots

    ArrOutText ( strPath_j + "JointGrid\\", "Ez_rotor_joint" , N_Ej, w.GenGrid.join_Ez_grid_pos);
    ArrOutText ( strPath_j + "JointGrid\\", "Hy_stator_joint", N_Hj, w.GenGrid.join_Hy_grid_pos);
    ///*
    for ( double time = 0.0; time <= maxTime; time+=w.dT_em) {

        //w.S_Er.SourceE (&g_r, &w.GenGrid, w.dT_em, true);
        //w.S_Es.SourceE (&g_s, &w.GenGrid, w.dT_em, false);

        FDTD_R.updateH2d(&g_r, &w.GenGrid, true, im_Out); // update magnetic fields
        FDTD_R.updateE2d(&g_r, &w.GenGrid, true, im_Out); // update electric fields

        FDTD_S.updateH2d(&g_s, &w.GenGrid, false, im_Out); // update magnetic fields
        FDTD_S.updateE2d(&g_s, &w.GenGrid, false, im_Out); // update electric fields


        //w.BC.abc(&w.g_s); // apply ABC
        //snapshot2d(go, w.strPath); // take a snapshot (if appropriate)
    } // end of time-stepping
    //*/

     FI.addSnapshot(w.ui->CP_EM_Field, im_Out, &g_r, &g_s, &w.GenGrid, &w.GenGeom);


     ArrOutText ( strPath_j + "JointGrid\\", "Ez_rotor_joint" , N_Ej, w.GenGrid.join_Ez_grid_pos);
     ArrOutText ( strPath_j + "JointGrid\\", "Hy_stator_joint", N_Hj, w.GenGrid.join_Hy_grid_pos);

    double end_time = clock(); // конечное время

    double time = end_time - start_time; // искомое время

    w.time_em_Label(time);
    return a.exec();
}
