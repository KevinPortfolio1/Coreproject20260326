#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
    // Linux/X11 專屬硬體加速優化：強制開啟具有 4x MSAA 抗鋸齒的 OpenGL 渲染
    QSurfaceFormat format;
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
