#include "mainwindow.h"
#include "loginwidget.h"
#include "dashboardwidget.h"
#include "subjectmanagementwidget.h"
#include "questionmanagementwidget.h"
#include "classmanagementwidget.h"
#include "studentmanagementwidget.h"
#include "examwidget.h"
#include "reportwidget.h"
#include "appmanager.h"
#include "examcontroller.h"
#include "datacontroller.h"

#include <QApplication>
#include <QScreen>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , appManager(AppManager::getInstance())
    , examController(new ExamController(this))
    , dataController(new DataController(this))
    , centralStack(nullptr)
    , loginWidget(nullptr)
    , dashboardWidget(nullptr)
    , subjectWidget(nullptr)
    , questionWidget(nullptr)
    , classWidget(nullptr)
    , studentWidget(nullptr)
    , examWidget(nullptr)
    , reportWidget(nullptr)
    , statusTimer(new QTimer(this))
{
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupConnections();
    
    // Initialize with login screen
    showLoginWidget();
    
    // Set window properties
    setWindowTitle("Hệ Thống Thi Trắc Nghiệm - CTDL&GT");
    setWindowIcon(QIcon(":/icons/app_icon.png")); // You can add an icon later
    resize(1200, 800);
    setMinimumSize(800, 600);
    
    // Center window on screen
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        move(screenGeometry.center() - rect().center());
    }
}

MainWindow::~MainWindow()
{
    // Cleanup is handled by Qt's parent-child relationship
}

void MainWindow::setupUI()
{
    // Create central stacked widget
    centralStack = new QStackedWidget(this);
    setCentralWidget(centralStack);
    
    // Create login widget
    loginWidget = new LoginWidget(appManager, this);
    centralStack->addWidget(loginWidget);
    
    // Other widgets will be created on demand
}

void MainWindow::setupMenuBar()
{
    // File Menu
    fileMenu = menuBar()->addMenu("&File");
    
    loginAction = fileMenu->addAction("&Login");
    loginAction->setShortcut(QKeySequence::New);
    connect(loginAction, &QAction::triggered, this, &MainWindow::showLoginWidget);
    
    logoutAction = fileMenu->addAction("&Logout");
    logoutAction->setShortcut(QKeySequence("Ctrl+L"));
    connect(logoutAction, &QAction::triggered, this, &MainWindow::onLogout);
    logoutAction->setEnabled(false);
    
    fileMenu->addSeparator();
    
    importAction = fileMenu->addAction("&Import Data...");
    importAction->setShortcut(QKeySequence("Ctrl+I"));
    importAction->setEnabled(false);
    
    exportAction = fileMenu->addAction("&Export Data...");
    exportAction->setShortcut(QKeySequence("Ctrl+E"));
    exportAction->setEnabled(false);
    
    fileMenu->addSeparator();
    
    backupAction = fileMenu->addAction("&Backup...");
    restoreAction = fileMenu->addAction("&Restore...");
    backupAction->setEnabled(false);
    restoreAction->setEnabled(false);
    
    fileMenu->addSeparator();
    
    exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExit);
    
    // Data Menu (Teacher only)
    dataMenu = menuBar()->addMenu("&Data");
    
    subjectAction = dataMenu->addAction("&Subjects");
    subjectAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(subjectAction, &QAction::triggered, this, &MainWindow::showSubjectManagement);
    
    questionAction = dataMenu->addAction("&Questions");
    questionAction->setShortcut(QKeySequence("Ctrl+Q"));
    connect(questionAction, &QAction::triggered, this, &MainWindow::showQuestionManagement);
    
    classAction = dataMenu->addAction("&Classes");
    classAction->setShortcut(QKeySequence("Ctrl+C"));
    connect(classAction, &QAction::triggered, this, &MainWindow::showClassManagement);
    
    studentAction = dataMenu->addAction("S&tudents");
    studentAction->setShortcut(QKeySequence("Ctrl+T"));
    connect(studentAction, &QAction::triggered, this, &MainWindow::showStudentManagement);
    
    dataMenu->setEnabled(false);
    
    // Exam Menu
    examMenu = menuBar()->addMenu("&Exam");
    
    takeExamAction = examMenu->addAction("&Take Exam");
    takeExamAction->setShortcut(QKeySequence("Ctrl+X"));
    connect(takeExamAction, &QAction::triggered, this, &MainWindow::showExamWidget);
    
    viewScoresAction = examMenu->addAction("&View Scores");
    viewScoresAction->setShortcut(QKeySequence("Ctrl+V"));
    
    examMenu->setEnabled(false);
    
    // Report Menu (Teacher only)
    reportMenu = menuBar()->addMenu("&Reports");
    
    printReportAction = reportMenu->addAction("&Print Reports");
    printReportAction->setShortcut(QKeySequence("Ctrl+P"));
    connect(printReportAction, &QAction::triggered, this, &MainWindow::showReportWidget);
    
    reportMenu->setEnabled(false);
    
    // Help Menu
    helpMenu = menuBar()->addMenu("&Help");
    
    aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::setupStatusBar()
{
    statusLabel = new QLabel("Ready");
    userLabel = new QLabel("Not logged in");
    timeLabel = new QLabel();
    
    statusBar()->addWidget(statusLabel, 1);
    statusBar()->addPermanentWidget(userLabel);
    statusBar()->addPermanentWidget(timeLabel);
    
    // Update time every second
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::updateStatusBar);
    statusTimer->start(1000);
    updateStatusBar();
}

void MainWindow::setupConnections()
{
    // Connect to AppManager signals
    connect(appManager, &AppManager::loginStatusChanged, 
            this, &MainWindow::onLoginStatusChanged);
    connect(appManager, &AppManager::userRoleChanged, 
            this, &MainWindow::onUserRoleChanged);
    
    // Connect login widget
    connect(loginWidget, &LoginWidget::loginSuccessful, 
            this, &MainWindow::showDashboard);
}

void MainWindow::onLoginStatusChanged()
{
    bool loggedIn = appManager->isLoggedIn();
    
    // Update menu states
    loginAction->setEnabled(!loggedIn);
    logoutAction->setEnabled(loggedIn);
    examMenu->setEnabled(loggedIn);
    importAction->setEnabled(loggedIn);
    exportAction->setEnabled(loggedIn);
    backupAction->setEnabled(loggedIn);
    restoreAction->setEnabled(loggedIn);
    
    // Update status
    updateStatusBar();
    
    if (loggedIn) {
        updateMenusForRole();
        showDashboard();
    } else {
        showLoginWidget();
    }
}

void MainWindow::onUserRoleChanged()
{
    updateMenusForRole();
    updateStatusBar();
}

void MainWindow::updateMenusForRole()
{
    bool isTeacher = appManager->isTeacher();
    
    dataMenu->setEnabled(isTeacher);
    reportMenu->setEnabled(isTeacher);
    
    // Students can only take exams and view their scores
    if (!isTeacher) {
        takeExamAction->setText("Take &Exam");
        viewScoresAction->setText("My &Scores");
    } else {
        takeExamAction->setText("Monitor &Exams");
        viewScoresAction->setText("All &Scores");
    }
}

void MainWindow::updateStatusBar()
{
    // Update time
    timeLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss dd/MM/yyyy"));
    
    // Update user info
    if (appManager->isLoggedIn()) {
        QString role = appManager->isTeacher() ? "Teacher" : "Student";
        userLabel->setText(QString("%1 (%2)").arg(appManager->getCurrentUser(), role));
    } else {
        userLabel->setText("Not logged in");
    }
}

void MainWindow::showLoginWidget()
{
    if (!loginWidget) {
        loginWidget = new LoginWidget(appManager, this);
        centralStack->addWidget(loginWidget);
        connect(loginWidget, &LoginWidget::loginSuccessful, 
                this, &MainWindow::showDashboard);
    }
    
    // Reset login widget state
    loginWidget->clearFields();
    
    centralStack->setCurrentWidget(loginWidget);
    statusLabel->setText("Please log in to continue");
}

void MainWindow::showDashboard()
{
    if (!dashboardWidget) {
        // Create dashboard widget on demand
        dashboardWidget = new DashboardWidget(appManager, this);
        centralStack->addWidget(dashboardWidget);
        
        // Connect dashboard signals to main window slots
        connect(dashboardWidget, &DashboardWidget::subjectManagementRequested,
                this, &MainWindow::showSubjectManagement);
        connect(dashboardWidget, &DashboardWidget::questionManagementRequested,
                this, &MainWindow::showQuestionManagement);
        connect(dashboardWidget, &DashboardWidget::classManagementRequested,
                this, &MainWindow::showClassManagement);
        connect(dashboardWidget, &DashboardWidget::studentManagementRequested,
                this, &MainWindow::showStudentManagement);
        connect(dashboardWidget, &DashboardWidget::examRequested,
                this, &MainWindow::showExamWidget);
        connect(dashboardWidget, &DashboardWidget::reportRequested,
                this, &MainWindow::showReportWidget);
    }
    
    // Refresh dashboard data
    dashboardWidget->refreshData();
    centralStack->setCurrentWidget(dashboardWidget);
    statusLabel->setText("Dashboard");
}

void MainWindow::showSubjectManagement()
{
    if (!subjectWidget) {
        // Create subject management widget on demand
        subjectWidget = new SubjectManagementWidget(appManager, this);
        centralStack->addWidget(subjectWidget);
        
        // Connect subject management signals if needed
        connect(subjectWidget, &SubjectManagementWidget::subjectAdded,
                [this](const QString& code, const QString& name) {
                    statusLabel->setText(QString("Subject added: %1 - %2").arg(code, name));
                });
        connect(subjectWidget, &SubjectManagementWidget::subjectUpdated,
                [this](const QString& code) {
                    statusLabel->setText(QString("Subject updated: %1").arg(code));
                });
        connect(subjectWidget, &SubjectManagementWidget::subjectDeleted,
                [this](const QString& code) {
                    statusLabel->setText(QString("Subject deleted: %1").arg(code));
                });
    }
    
    // Refresh data and show widget
    subjectWidget->refreshData();
    centralStack->setCurrentWidget(subjectWidget);
    statusLabel->setText("Subject Management");
}

void MainWindow::showQuestionManagement()
{
    if (!questionWidget) {
        // Create question management widget on demand
        questionWidget = new QuestionManagementWidget(appManager, this);
        centralStack->addWidget(questionWidget);
        
        // Connect question management signals
        connect(questionWidget, &QuestionManagementWidget::questionAdded,
                [this](int questionId, const QString& subjectCode) {
                    statusLabel->setText(QString("Question %1 added to subject %2").arg(questionId).arg(subjectCode));
                });
        connect(questionWidget, &QuestionManagementWidget::questionUpdated,
                [this](int questionId) {
                    statusLabel->setText(QString("Question %1 updated").arg(questionId));
                });
        connect(questionWidget, &QuestionManagementWidget::questionDeleted,
                [this](int questionId) {
                    statusLabel->setText(QString("Question %1 deleted").arg(questionId));
                });
    }
    
    // Refresh data and show widget
    questionWidget->refreshData();
    centralStack->setCurrentWidget(questionWidget);
    statusLabel->setText("Question Management");
}

void MainWindow::showClassManagement()
{
    if (!classWidget) {
        // Create class management widget on demand
        classWidget = new ClassManagementWidget(appManager, this);
        centralStack->addWidget(classWidget);
        
        // Connect class management signals
        connect(classWidget, &ClassManagementWidget::classAdded,
                [this](const QString& code, const QString& name) {
                    statusLabel->setText(QString("Class added: %1 - %2").arg(code, name));
                });
        connect(classWidget, &ClassManagementWidget::classUpdated,
                [this](const QString& code) {
                    statusLabel->setText(QString("Class updated: %1").arg(code));
                });
        connect(classWidget, &ClassManagementWidget::classDeleted,
                [this](const QString& code) {
                    statusLabel->setText(QString("Class deleted: %1").arg(code));
                });
    }
    
    // Refresh data and show widget
    classWidget->refreshData();
    centralStack->setCurrentWidget(classWidget);
    statusLabel->setText("Class Management");
}

void MainWindow::showStudentManagement()
{
    if (!studentWidget) {
        // Create student management widget on demand
        studentWidget = new StudentManagementWidget(appManager, this);
        centralStack->addWidget(studentWidget);
        
        // Connect student management signals
        connect(studentWidget, &StudentManagementWidget::studentAdded,
                [this](const QString& studentId, const QString& classCode) {
                    statusLabel->setText(QString("Student added: %1 to class %2").arg(studentId, classCode));
                });
        connect(studentWidget, &StudentManagementWidget::studentUpdated,
                [this](const QString& studentId) {
                    statusLabel->setText(QString("Student updated: %1").arg(studentId));
                });
        connect(studentWidget, &StudentManagementWidget::studentDeleted,
                [this](const QString& studentId) {
                    statusLabel->setText(QString("Student deleted: %1").arg(studentId));
                });
    }
    
    // Refresh data and show widget
    studentWidget->refreshData();
    centralStack->setCurrentWidget(studentWidget);
    statusLabel->setText("Student Management");
}

void MainWindow::showExamWidget()
{
    if (!examWidget) {
        // Create exam widget on demand
        examWidget = new ExamWidget(appManager, this);
        centralStack->addWidget(examWidget);
        
        // Connect exam signals
        connect(examWidget, &ExamWidget::examCompleted,
                [this](const QString& subject, int score, int total) {
                    statusLabel->setText(QString("Exam completed: %1 - Score: %2/%3")
                                       .arg(subject).arg(score).arg(total));
                });
    }
    
    // Show widget
    centralStack->setCurrentWidget(examWidget);
    statusLabel->setText("Exam System");
}

void MainWindow::showReportWidget()
{
    if (!reportWidget) {
        // Create report widget on demand
        reportWidget = new ReportWidget(appManager, this);
        centralStack->addWidget(reportWidget);
        
        // Connect report signals
        connect(reportWidget, &ReportWidget::reportGenerated,
                [this](const QString& reportType, const QString& details) {
                    statusLabel->setText(QString("Report: %1").arg(reportType));
                });
    }
    
    // Refresh data and show widget
    reportWidget->refreshData();
    centralStack->setCurrentWidget(reportWidget);
    statusLabel->setText("Report Management");
}

void MainWindow::onLogout()
{
    appManager->logout();
    loginWidget->clearFields();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About", 
        "Exam Management System - CTDL&GT\n\n"
        "Version 1.0.0\n"
        "A comprehensive exam management system for multiple choice tests.\n\n"
        "Features:\n"
        "• User authentication (Teacher/Student)\n"
        "• Subject and question management\n"
        "• Class and student management\n"
        "• Exam taking and scoring\n"
        "• Comprehensive reporting\n\n"
        "Built with Qt Widgets");
}

void MainWindow::onExit()
{
    if (appManager->isLoggedIn()) {
        int ret = QMessageBox::question(this, "Exit", 
            "Are you sure you want to exit?",
            QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            QApplication::quit();
        }
    } else {
        QApplication::quit();
    }
}