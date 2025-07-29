#ifndef CLASSMANAGEMENTWIDGET_H
#define CLASSMANAGEMENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>

class AppManager;

/**
 * @brief Widget for managing classes (lớp học)
 * Implements: NhapLop, In ds lớp
 */
class ClassManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClassManagementWidget(AppManager* appManager, QWidget *parent = nullptr);
    ~ClassManagementWidget();

public slots:
    void refreshData();

private slots:
    void onAddClass();
    void onEditClass();
    void onDeleteClass();
    void onClassSelectionChanged();
    void onSearchTextChanged();
    void onPrintClassList();

signals:
    void classAdded(const QString& classCode, const QString& className);
    void classUpdated(const QString& classCode);
    void classDeleted(const QString& classCode);

private:
    void setupUI();
    void setupConnections();
    void applyStyles();
    void updateClassTable();
    void clearForm();
    bool validateInput();

    AppManager* appManager;

    // UI components
    QVBoxLayout* mainLayout;
    
    // Search and actions section
    QGroupBox* searchGroup;
    QHBoxLayout* searchLayout;
    QLineEdit* searchEdit;
    QPushButton* refreshButton;
    QPushButton* printButton;
    
    // Class list section
    QGroupBox* listGroup;
    QVBoxLayout* listLayout;
    QTableWidget* classTable;
    
    // Action buttons
    QHBoxLayout* buttonLayout;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    
    // Form section
    QGroupBox* formGroup;
    QGridLayout* formLayout;
    QLineEdit* codeEdit;
    QLineEdit* nameEdit;
    QLineEdit* yearEdit;
    QPushButton* saveButton;
    QPushButton* cancelButton;
    
    // Current editing state
    bool isEditing;
    QString currentClassCode;
};

#endif // CLASSMANAGEMENTWIDGET_H