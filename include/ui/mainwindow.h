#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QTreeWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QSplitter>
#include <QProgressBar>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

// Forward declarations
class AppManager;
class ExamController;
class DataController;
class LoginWidget;
class DashboardWidget;
class SubjectManagementWidget;
class QuestionManagementWidget;
class ClassManagementWidget;
class StudentManagementWidget;
class ExamWidget;
class ReportWidget;

/**
 * @brief Main application window using Qt Widgets
 * Replaces the QML-based interface with native Qt Widgets
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginStatusChanged();
    void onUserRoleChanged();
    void showLoginWidget();
    void showDashboard();
    void showSubjectManagement();
    void showQuestionManagement();
    void showClassManagement();
    void showStudentManagement();
    void showExamWidget();
    void showReportWidget();
    void onLogout();
    void onAbout();
    void onExit();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupConnections();
    void updateMenusForRole();
    void updateStatusBar();

    // Core components
    AppManager* appManager;
    ExamController* examController;
    DataController* dataController;

    // UI components
    QStackedWidget* centralStack;
    QLabel* statusLabel;
    QLabel* userLabel;
    QLabel* timeLabel;
    QTimer* statusTimer;

    // Widgets
    LoginWidget* loginWidget;
    DashboardWidget* dashboardWidget;
    SubjectManagementWidget* subjectWidget;
    QuestionManagementWidget* questionWidget;
    ClassManagementWidget* classWidget;
    StudentManagementWidget* studentWidget;
    ExamWidget* examWidget;
    ReportWidget* reportWidget;

    // Menus
    QMenu* fileMenu;
    QMenu* dataMenu;
    QMenu* examMenu;
    QMenu* reportMenu;
    QMenu* helpMenu;

    // Actions
    QAction* loginAction;
    QAction* logoutAction;
    QAction* exitAction;
    QAction* importAction;
    QAction* exportAction;
    QAction* backupAction;
    QAction* restoreAction;
    QAction* subjectAction;
    QAction* questionAction;
    QAction* classAction;
    QAction* studentAction;
    QAction* takeExamAction;
    QAction* viewScoresAction;
    QAction* printReportAction;
    QAction* aboutAction;
};

#endif // MAINWINDOW_H