#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGroupBox>
#include <QTextEdit>
#include <QMessageBox>

class AppManager;

/**
 * @brief Simple report widget for class grades
 */
class ReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReportWidget(AppManager* appManager, QWidget *parent = nullptr);
    ~ReportWidget();

public slots:
    void refreshData();

signals:
    void reportGenerated(const QString& reportType, const QString& details);

private slots:
    void onGenerateReport();

private:
    void setupUI();
    void setupConnections();
    void loadClasses();
    void loadSubjects();
    void generateClassReport();
    
    AppManager* appManager;
    
    // UI Components
    QVBoxLayout* mainLayout;
    QGroupBox* filterGroup;
    QHBoxLayout* filterLayout;
    QComboBox* classCombo;
    QComboBox* subjectCombo;
    QPushButton* generateBtn;
    
    QGroupBox* reportGroup;
    QVBoxLayout* reportLayout;
    QTableWidget* reportTable;
    QTextEdit* reportSummary;
};

#endif // REPORTWIDGET_H