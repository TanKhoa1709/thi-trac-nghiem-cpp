#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QStyleFactory>
#include <QMessageBox>
#include <QPalette>

// Core includes
#include "appmanager.h"
#include "authentication.h"

// Controller includes
#include "examcontroller.h"
#include "datacontroller.h"

// UI includes
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Exam Management System");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("CTDL&GT Team");
    app.setApplicationDisplayName("Hệ Thống Thi Trắc Nghiệm - CTDL&GT");
    
    // Set application style to light theme
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Force light theme palette to ensure white mode
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(245, 245, 245));          // Light gray background
    lightPalette.setColor(QPalette::WindowText, QColor(44, 62, 80));         // Dark text
    lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));            // White input backgrounds
    lightPalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));   // Alternate row color
    lightPalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));     // Light tooltip background
    lightPalette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));           // Black tooltip text
    lightPalette.setColor(QPalette::Text, QColor(44, 62, 80));               // Dark text in inputs
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));          // Light button background
    lightPalette.setColor(QPalette::ButtonText, QColor(44, 62, 80));         // Dark button text
    lightPalette.setColor(QPalette::BrightText, QColor(255, 0, 0));          // Bright text (errors)
    lightPalette.setColor(QPalette::Link, QColor(52, 152, 219));             // Blue links
    lightPalette.setColor(QPalette::Highlight, QColor(52, 152, 219));        // Blue selection
    lightPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255)); // White selected text
    app.setPalette(lightPalette);
    
    // Initialize the application manager
    AppManager* appManager = AppManager::getInstance();
    
    // Set up data directory
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!appManager->initialize(dataDir)) {
        QMessageBox::critical(nullptr, "Initialization Error", 
                            "Failed to initialize application manager.\nThe application will now exit.");
        return -1;
    }
    
    // Create and show main window
    MainWindow mainWindow;
    mainWindow.show();
    
    qDebug() << "Exam Management System started successfully";
    
    // Run the application
    int result = app.exec();
    
    // Cleanup
    appManager->shutdown();
    
    return result;
}
