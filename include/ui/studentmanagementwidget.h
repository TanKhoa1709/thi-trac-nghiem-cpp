#ifndef STUDENTMANAGEMENTWIDGET_H
#define STUDENTMANAGEMENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGroupBox>
#include <QSplitter>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateEdit>
#include <QDate>

class AppManager;
class SinhVien;

/**
 * @brief Widget for managing students
 * Implements full CRUD operations for students with class integration
 */
class StudentManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StudentManagementWidget(AppManager* appManager, QWidget *parent = nullptr);
    ~StudentManagementWidget();

public slots:
    void refreshData();
    void onClassChanged();

signals:
    void studentAdded(const QString& studentId, const QString& classCode);
    void studentUpdated(const QString& studentId);
    void studentDeleted(const QString& studentId);

private slots:
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onStudentSelectionChanged();
    void onSearchTextChanged();
    void onClearForm();
    void onSaveStudent();
    void onCancelEdit();
    void onViewScores();

private:
    void setupUI();
    void setupStudentList();
    void setupStudentForm();
    void setupConnections();
    void applyStyles();
    
    void loadClasses();
    void loadStudents();
    void filterStudents();
    void populateStudentTable();
    void clearStudentForm();
    void loadStudentToForm(const SinhVien& student);
    bool validateStudentForm();
    void saveStudentFromForm();
    void updateStudentInForm();
    bool canDeleteStudent(const QString& studentId);
    
    AppManager* appManager;
    
    // UI Components
    QVBoxLayout* mainLayout;
    QSplitter* mainSplitter;
    
    // Left panel - Student list
    QWidget* listPanel;
    QVBoxLayout* listLayout;
    QGroupBox* filterGroup;
    QHBoxLayout* filterLayout;
    QComboBox* classFilterCombo;
    QLineEdit* searchEdit;
    QPushButton* refreshButton;
    
    QGroupBox* studentListGroup;
    QVBoxLayout* studentListLayout;
    QTableWidget* studentTable;
    
    QHBoxLayout* listButtonLayout;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* viewScoresButton;
    
    // Right panel - Student form
    QWidget* formPanel;
    QVBoxLayout* formLayout;
    QGroupBox* studentFormGroup;
    QGridLayout* formGridLayout;
    
    QLabel* idLabel;
    QLineEdit* idEdit;
    QLabel* classLabel;
    QComboBox* classCombo;
    QLabel* nameLabel;
    QLineEdit* nameEdit;
    QLabel* passwordLabel;
    QLineEdit* passwordEdit;
    QLabel* emailLabel;
    QLineEdit* emailEdit;
    QLabel* phoneLabel;
    QLineEdit* phoneEdit;
    QLabel* birthdateLabel;
    QDateEdit* birthdateEdit;
    
    QHBoxLayout* formButtonLayout;
    QPushButton* saveButton;
    QPushButton* cancelButton;
    QPushButton* clearButton;
    
    // State
    bool isEditing;
    QString currentStudentId;
    QString currentClassFilter;
};

#endif // STUDENTMANAGEMENTWIDGET_H