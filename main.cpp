#include "Header.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ImageField  FI(w.ui->CP_EM_Field, "Electromagnetic field");
    double start_time =  clock(); // начальное время

    w.im_Out = 2;
    w.Snapshot_step = 2;
    w.MaxStep = 4;
    // при dT_em = 0.00000001;//10^(-8) // im_Out = 10000000 // maxTime = 0.1 сек
    double maxTime = w.dT_em*w.im_Out;

    grid_fdtd   g_r;
    grid_fdtd   g_s;
    Fdtd_calc   FDTD_R;
    Fdtd_calc   FDTD_S;
    w.SP_next   = 0;

    std::string strPath_j = "D:\\work\\Gen2D_FDTD\\TextFiles\\";
    int N_Ej = w.GenGrid.rotor_grid_par.Col/2;
    int N_Hj = w.GenGrid.stator_grid_par.Col/2;

    w.Mat.gridInit(&g_r, &w.GenGrid, w.dT_em, true);
    w.Mat.gridInit(&g_s, &w.GenGrid, w.dT_em, false);

    w.S_Er.SourceE (&g_r, &w.GenGrid, true);
    w.S_Es.SourceE (&g_s, &w.GenGrid, false);

    ArrOutText ( strPath_j + "JointGrid\\", "Ez_rotor_joint" , N_Ej, w.GenGrid.join_Ez_grid_pos);
    ArrOutText ( strPath_j + "JointGrid\\", "Hy_stator_joint", N_Hj, w.GenGrid.join_Hy_grid_pos);

    int step =0;

    std::string strPath;
    int M, N;

    for ( double time = 0.0; time <= maxTime; time+=w.dT_em) {

        FDTD_R.updateH2d(&g_r, &w.GenGrid, true); // update magnetic fields
        FDTD_R.updateE2d(&g_r, &w.GenGrid, true); // update electric fields

        FDTD_S.updateH2d(&g_s, &w.GenGrid, false); // update magnetic fields
        FDTD_S.updateE2d(&g_s, &w.GenGrid, false); // update electric fields
        step++;

        if (step % w.Snapshot_step == 0)
        {
            strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\EH\\Rotor\\";
            M = w.GenGrid.rotor_grid_par.Row;
            N = w.GenGrid.rotor_grid_par.Col;
            ArrOutText (strPath, "Hy_Rotor" + std::to_string(step), N, M, w.GenGrid.rotor_grid_par.Np_s,  w.GenGrid.rotor_grid_par.Np_p, g_r.hy);
            ArrOutText (strPath, "Hx_Rotor" + std::to_string(step), N, M, w.GenGrid.rotor_grid_par.Np_s,  w.GenGrid.rotor_grid_par.Np_p, g_r.hx);
            ArrOutText (strPath, "Ez_Rotor" + std::to_string(step), N, M, w.GenGrid.rotor_grid_par.Np_s,  w.GenGrid.rotor_grid_par.Np_p, g_r.ez);

            strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\EH\\Stator\\";
            M = w.GenGrid.stator_grid_par.Row;
            N = w.GenGrid.stator_grid_par.Col;
            ArrOutText (strPath, "Hy_Stator" + std::to_string(step), N, M, w.GenGrid.stator_grid_par.Np_s,  w.GenGrid.stator_grid_par.Np_p, g_s.hy);
            ArrOutText (strPath, "Hx_Stator" + std::to_string(step), N, M, w.GenGrid.stator_grid_par.Np_s,  w.GenGrid.stator_grid_par.Np_p, g_s.hx);
            ArrOutText (strPath, "Ez_Stator" + std::to_string(step), N, M, w.GenGrid.stator_grid_par.Np_s,  w.GenGrid.stator_grid_par.Np_p, g_s.ez);
        }

    } // end of time-stepping

     FI.addSnapshot(w.ui->CP_EM_Field, w.im_Out, &w.GenGrid, &w.GenGeom);

     ArrOutText ( strPath_j + "JointGrid\\", "Ez_rotor_joint" , N_Ej, w.GenGrid.join_Ez_grid_pos);
     ArrOutText ( strPath_j + "JointGrid\\", "Hy_stator_joint", N_Hj, w.GenGrid.join_Hy_grid_pos);

    double end_time = clock(); // конечное время

    double time = end_time - start_time; // искомое время

    w.time_em_Label(time);

    return a.exec();
}
