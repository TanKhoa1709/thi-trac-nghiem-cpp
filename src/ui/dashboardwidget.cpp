#include "dashboardwidget.h"
#include "appmanager.h"
#include "monhoc.h"
#include "cauhoi.h"
#include "lop.h"
#include "sinhvien.h"
#include "diemthi.h"
#include "quanlythi.h"

#include <QDateTime>

DashboardWidget::DashboardWidget(AppManager* appManager, QWidget *parent)
    : QWidget(parent)
    , appManager(appManager)
    , mainLayout(nullptr)
    , scrollArea(nullptr)
    , contentWidget(nullptr)
{
    setupUI();
    applyStyles();
    refreshData();
}

DashboardWidget::~DashboardWidget()
{
    // Cleanup handled by Qt parent-child relationship
}

void DashboardWidget::setupUI()
{
    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // Scroll area for content
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    contentWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(20);
    
    // Welcome section
    welcomeLabel = new QLabel(QString("Welcome, %1!").arg(appManager->getCurrentUser()));
    welcomeLabel->setObjectName("welcomeLabel");
    
    QString roleText = appManager->isTeacher() ? "Teacher Dashboard" : "Student Dashboard";
    roleLabel = new QLabel(roleText);
    roleLabel->setObjectName("roleLabel");
    
    separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    
    contentLayout->addWidget(welcomeLabel);
    contentLayout->addWidget(roleLabel);
    contentLayout->addWidget(separator);
    
    // Create sections based on user role
    if (appManager->isTeacher()) {
        setupTeacherDashboard();
    } else {
        setupStudentDashboard();
    }
    
    createStatisticsSection();
    createQuickActionsSection();
    createRecentActivitySection();
    
    contentLayout->addWidget(statsGroup);
    contentLayout->addWidget(actionsGroup);
    contentLayout->addWidget(activityGroup);
    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void DashboardWidget::setupTeacherDashboard()
{
    // Teacher-specific setup will be added here
}

void DashboardWidget::setupStudentDashboard()
{
    // Student-specific setup will be added here
}

void DashboardWidget::createStatisticsSection()
{
    statsGroup = new QGroupBox("System Statistics");
    statsGroup->setObjectName("statsGroup");
    
    statsLayout = new QGridLayout(statsGroup);
    statsLayout->setSpacing(15);
    
    // Create statistic labels
    QLabel* subjectLabel = new QLabel("Subjects:");
    subjectCountLabel = new QLabel("0");
    subjectCountLabel->setObjectName("statValue");
    
    QLabel* questionLabel = new QLabel("Questions:");
    questionCountLabel = new QLabel("0");
    questionCountLabel->setObjectName("statValue");
    
    QLabel* classLabel = new QLabel("Classes:");
    classCountLabel = new QLabel("0");
    classCountLabel->setObjectName("statValue");
    
    QLabel* studentLabel = new QLabel("Students:");
    studentCountLabel = new QLabel("0");
    studentCountLabel->setObjectName("statValue");
    
    QLabel* examLabel = new QLabel("Exams Taken:");
    examCountLabel = new QLabel("0");
    examCountLabel->setObjectName("statValue");
    
    // Layout statistics
    statsLayout->addWidget(subjectLabel, 0, 0);
    statsLayout->addWidget(subjectCountLabel, 0, 1);
    statsLayout->addWidget(questionLabel, 0, 2);
    statsLayout->addWidget(questionCountLabel, 0, 3);
    
    statsLayout->addWidget(classLabel, 1, 0);
    statsLayout->addWidget(classCountLabel, 1, 1);
    statsLayout->addWidget(studentLabel, 1, 2);
    statsLayout->addWidget(studentCountLabel, 1, 3);
    
    statsLayout->addWidget(examLabel, 2, 0);
    statsLayout->addWidget(examCountLabel, 2, 1);
}

void DashboardWidget::createQuickActionsSection()
{
    actionsGroup = new QGroupBox("Quick Actions");
    actionsGroup->setObjectName("actionsGroup");
    
    actionsLayout = new QGridLayout(actionsGroup);
    actionsLayout->setSpacing(15);
    
    if (appManager->isTeacher()) {
        // Teacher actions
        subjectButton = new QPushButton("Manage Subjects");
        subjectButton->setObjectName("actionButton");
        connect(subjectButton, &QPushButton::clicked, this, &DashboardWidget::subjectManagementRequested);
        
        questionButton = new QPushButton("Manage Questions");
        questionButton->setObjectName("actionButton");
        connect(questionButton, &QPushButton::clicked, this, &DashboardWidget::questionManagementRequested);
        
        classButton = new QPushButton("Manage Classes");
        classButton->setObjectName("actionButton");
        connect(classButton, &QPushButton::clicked, this, &DashboardWidget::classManagementRequested);
        
        studentButton = new QPushButton("Manage Students");
        studentButton->setObjectName("actionButton");
        connect(studentButton, &QPushButton::clicked, this, &DashboardWidget::studentManagementRequested);
        
        examButton = new QPushButton("Monitor Exams");
        examButton->setObjectName("actionButton");
        connect(examButton, &QPushButton::clicked, this, &DashboardWidget::examRequested);
        
        reportButton = new QPushButton("Generate Reports");
        reportButton->setObjectName("actionButton");
        connect(reportButton, &QPushButton::clicked, this, &DashboardWidget::reportRequested);
        
        actionsLayout->addWidget(subjectButton, 0, 0);
        actionsLayout->addWidget(questionButton, 0, 1);
        actionsLayout->addWidget(classButton, 0, 2);
        actionsLayout->addWidget(studentButton, 1, 0);
        actionsLayout->addWidget(examButton, 1, 1);
        actionsLayout->addWidget(reportButton, 1, 2);
    } else {
        // Student actions
        examButton = new QPushButton("Take Exam");
        examButton->setObjectName("actionButton");
        connect(examButton, &QPushButton::clicked, this, &DashboardWidget::examRequested);
        
        QPushButton* scoresButton = new QPushButton("View My Scores");
        scoresButton->setObjectName("actionButton");
        
        QPushButton* profileButton = new QPushButton("My Profile");
        profileButton->setObjectName("actionButton");
        
        actionsLayout->addWidget(examButton, 0, 0);
        actionsLayout->addWidget(scoresButton, 0, 1);
        actionsLayout->addWidget(profileButton, 0, 2);
    }
}

void DashboardWidget::createRecentActivitySection()
{
    activityGroup = new QGroupBox("Recent Activity");
    activityGroup->setObjectName("activityGroup");
    
    activityLayout = new QVBoxLayout(activityGroup);
    
    activityLabel = new QLabel("No recent activity");
    activityLabel->setObjectName("activityText");
    activityLabel->setAlignment(Qt::AlignCenter);
    
    activityLayout->addWidget(activityLabel);
}

void DashboardWidget::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f8f9fa;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        
        #welcomeLabel {
            font-size: 32px;
            font-weight: bold;
            color: #2c3e50;
            margin: 10px 0;
        }
        
        #roleLabel {
            font-size: 18px;
            color: #7f8c8d;
            margin-bottom: 20px;
        }
        
        QGroupBox {
            font-size: 16px;
            font-weight: bold;
            color: #2c3e50;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background-color: white;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
            background-color: white;
        }
        
        #statsGroup {
            min-height: 120px;
        }
        
        #actionsGroup {
            min-height: 150px;
        }
        
        #activityGroup {
            min-height: 100px;
        }
        
        #statValue {
            font-size: 24px;
            font-weight: bold;
            color: #3498db;
            text-align: center;
        }
        
        #actionButton {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 15px 20px;
            border-radius: 6px;
            font-size: 14px;
            font-weight: bold;
            min-height: 50px;
        }
        
        #actionButton:hover {
            background-color: #2980b9;
        }
        
        #actionButton:pressed {
            background-color: #21618c;
        }
        
        #activityText {
            color: #7f8c8d;
            font-style: italic;
            padding: 20px;
        }
        
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        
        QFrame[frameShape="4"] {
            color: #bdc3c7;
        }
    )");
}

void DashboardWidget::refreshData()
{
    updateStatistics();
    
    // Update recent activity
    QString activityText = QString("Last login: %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"));
    activityLabel->setText(activityText);
}

void DashboardWidget::updateStatistics()
{
    // Get actual statistics from managers
    if (appManager) {
        // Get statistics from individual managers
        int subjectCount = appManager->getMonHocManager() ? appManager->getMonHocManager()->soLuongMonHoc() : 0;
        int questionCount = appManager->getCauHoiManager() ? appManager->getCauHoiManager()->tongSoCauHoi() : 0;
        int classCount = appManager->getLopManager() ? appManager->getLopManager()->getSoLuongLop() : 0;
        int studentCount = appManager->getSinhVienManager() ? appManager->getSinhVienManager()->tongSoSinhVien() : 0;
        int examCount = appManager->getDiemThiManager() ? appManager->getDiemThiManager()->tongSoBaiThi() : 0;
        
        subjectCountLabel->setText(QString::number(subjectCount));
        questionCountLabel->setText(QString::number(questionCount));
        classCountLabel->setText(QString::number(classCount));
        studentCountLabel->setText(QString::number(studentCount));
        examCountLabel->setText(QString::number(examCount));
    } else {
        // Fallback to placeholder values if AppManager not available
        subjectCountLabel->setText("0");
        questionCountLabel->setText("0");
        classCountLabel->setText("0");
        studentCountLabel->setText("0");
        examCountLabel->setText("0");
    }
}

void DashboardWidget::onQuickActionClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    // Handle quick action clicks
    // This is a placeholder - specific actions will be handled by individual button connections
}