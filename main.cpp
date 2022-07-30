#include "Header.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    double maxTime = 0.04;

    w.BC.abcInit(&w.g_s); // initialize ABC
    //snapshotInit2d(go); // initialize snapshots

    /* do time stepping */
    for ( double time = 0.0; time < maxTime; time+=w.dT_em) {
        w.FDTD_R.updateH2d(&w.g_r); // update magnetic fields
        w.FDTD_R.updateE2d(&w.g_r); // update electric fields
        w.FDTD_S.updateH2d(&w.g_s); // update magnetic fields
        w.FDTD_S.updateE2d(&w.g_s); // update electric fields
        w.BC.abc(&w.g_s); // apply ABC
        //snapshot2d(go, w.strPath); // take a snapshot (if appropriate)
    } // end of time-stepping

    return a.exec();
}
