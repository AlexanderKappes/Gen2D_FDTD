QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BoundCond.cpp \
    FdtdCalc.cpp \
    ImageField.cpp \
    ImageWork.cpp \
    GenGeom2D.cpp \
    GenGraph2D.cpp \
    GenGrid2D.cpp \
    Materials.cpp   \
    CurrentSource.cpp \
    main.cpp \
    mainwindow.cpp \
    qPlot/qcustomplot.cpp

HEADERS += \
    BoundCond.hpp \
    Constant.hpp    \
    FdtdAlloc.hpp \
    FdtdCalc.hpp \
    Grid.hpp \
    Header.hpp \
    ImageField.hpp \
    ImageWork.hpp \
    GenGeom2D.hpp \
    GenGraph2D.hpp \
    GenGrid2D.hpp \
    Materials.hpp \
    CurrentSource.hpp \
    mainwindow.hpp \
    qPlot/qcustomplot.h

#include <QMainWindow>
#include <QInputDialog>
#include "qPlot/qcustomplot.h"
#include "Constant.hpp"
#include "MatCalc.hpp"
#include "GenGeom2D.hpp"
#include "Grid.hpp"
#include "GenGrid2D.hpp"
#include "GridShow2D.hpp"

#include "GenGraph2D.hpp"
#include "CurrentSource.hpp"
#include "Fdtd_calc.hpp"
#include "Materials.hpp"
#include <ctime>

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
