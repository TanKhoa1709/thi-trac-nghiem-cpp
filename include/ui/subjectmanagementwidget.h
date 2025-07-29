#ifndef SUBJECTMANAGEMENTWIDGET_H
#define SUBJECTMANAGEMENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>

class AppManager;

/**
 * @brief Widget for managing subjects (môn học)
 * Implements: Nhập môn học - cho phép cập nhật (thêm / xóa / hiệu chỉnh) thông tin của môn học
 */
class SubjectManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubjectManagementWidget(AppManager* appManager, QWidget *parent = nullptr);
    ~SubjectManagementWidget();

public slots:
    void refreshData();

private slots:
    void onAddSubject();
    void onEditSubject();
    void onDeleteSubject();
    void onSubjectSelectionChanged();
    void onSearchTextChanged();

signals:
    void subjectAdded(const QString& subjectCode, const QString& subjectName);
    void subjectUpdated(const QString& subjectCode);
    void subjectDeleted(const QString& subjectCode);

private:
    void setupUI();
    void setupConnections();
    void applyStyles();
    void updateSubjectTable();
    void clearForm();
    bool validateInput();

    AppManager* appManager;

    // UI components
    QVBoxLayout* mainLayout;
    
    // Search section
    QGroupBox* searchGroup;
    QHBoxLayout* searchLayout;
    QLineEdit* searchEdit;
    QPushButton* refreshButton;
    
    // Subject list section
    QGroupBox* listGroup;
    QVBoxLayout* listLayout;
    QTableWidget* subjectTable;
    
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
    QTextEdit* descriptionEdit;
    QPushButton* saveButton;
    QPushButton* cancelButton;
    
    // Current editing state
    bool isEditing;
    QString currentSubjectCode;
};

#endif // SUBJECTMANAGEMENTWIDGET_H