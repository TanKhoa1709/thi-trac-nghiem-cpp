#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Exam Management System");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("CTDL&GT Project");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
