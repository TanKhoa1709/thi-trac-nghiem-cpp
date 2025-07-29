#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include <QScrollArea>

class AppManager;

/**
 * @brief Dashboard widget showing system overview and quick actions
 */
class DashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWidget(AppManager* appManager, QWidget *parent = nullptr);
    ~DashboardWidget();

public slots:
    void refreshData();
    void updateStatistics();

private slots:
    void onQuickActionClicked();

signals:
    void subjectManagementRequested();
    void questionManagementRequested();
    void classManagementRequested();
    void studentManagementRequested();
    void examRequested();
    void reportRequested();

private:
    void setupUI();
    void setupTeacherDashboard();
    void setupStudentDashboard();
    void createStatisticsSection();
    void createQuickActionsSection();
    void createRecentActivitySection();
    void applyStyles();

    AppManager* appManager;

    // UI components
    QVBoxLayout* mainLayout;
    QScrollArea* scrollArea;
    QWidget* contentWidget;
    
    // Statistics section
    QGroupBox* statsGroup;
    QGridLayout* statsLayout;
    QLabel* subjectCountLabel;
    QLabel* questionCountLabel;
    QLabel* classCountLabel;
    QLabel* studentCountLabel;
    QLabel* examCountLabel;
    
    // Quick actions section
    QGroupBox* actionsGroup;
    QGridLayout* actionsLayout;
    QPushButton* subjectButton;
    QPushButton* questionButton;
    QPushButton* classButton;
    QPushButton* studentButton;
    QPushButton* examButton;
    QPushButton* reportButton;
    
    // Recent activity section
    QGroupBox* activityGroup;
    QVBoxLayout* activityLayout;
    QLabel* activityLabel;
    
    // Welcome section
    QLabel* welcomeLabel;
    QLabel* roleLabel;
    QFrame* separator;
};

#endif // DASHBOARDWIDGET_H