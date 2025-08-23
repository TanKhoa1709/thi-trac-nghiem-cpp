#ifndef TEACHERDASHBOARD_H
#define TEACHERDASHBOARD_H

#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

// Forward declarations
class QuanLyLop;
class QuanLyMonHoc;
class Lop;
class MonHoc;
class SinhVien;
class CauHoi;
class DiemThi;

/**
 * @brief Teacher Dashboard Widget
 * Provides tabbed interface for teacher operations
 */
class TeacherDashboard : public QWidget {
    Q_OBJECT

public:
    explicit TeacherDashboard(QWidget *parent = nullptr);

    ~TeacherDashboard();

    // Set controllers
    void setClassManager(QuanLyLop *manager);

    void setSubjectManager(QuanLyMonHoc *manager);

public slots:
    void refreshAllData();

signals:
    void logoutRequested();

private slots:
    // Class management
    void refreshClassList();

    void onClassSelected();

    void addNewClass();

    void editClass();

    void deleteClass();

    // Student management
    void refreshStudentList();

    void addNewStudent();

    void editStudent();

    void deleteStudent();

    // Subject management
    void refreshSubjectList();

    void addNewSubject();

    void editSubject();

    void deleteSubject();

    void manageQuestions();

    // Question management
    void refreshQuestionList();

    void onSubjectChangedForQuestions();

    void addNewQuestion();

    void editQuestion();

    void deleteQuestion();

private:
    // UI Components
    QTabWidget *mainTabs;

    // Class Management Tab
    QWidget *classTab;
    QTableWidget *classTable;
    QTableWidget *studentTable;
    QPushButton *addClassButton;
    QPushButton *editClassButton;
    QPushButton *deleteClassButton;
    QPushButton *addStudentButton;
    QPushButton *editStudentButton;
    QPushButton *deleteStudentButton;

    // Subject Management Tab
    QWidget *subjectTab;
    QTableWidget *subjectTable;
    QPushButton *addSubjectButton;
    QPushButton *editSubjectButton;
    QPushButton *deleteSubjectButton;
    QPushButton *manageQuestionsButton;

    // Question Management Tab
    QWidget *questionTab;
    QComboBox *subjectComboForQuestions;
    QTableWidget *questionTable;
    QPushButton *addQuestionButton;
    QPushButton *editQuestionButton;
    QPushButton *deleteQuestionButton;

    // Controllers
    QuanLyLop *classManager;
    QuanLyMonHoc *subjectManager;

    // Current selections
    QString currentClassCode;
    QString currentSubjectCode;


    // Setup methods
    void setupUI();

    void setupClassTab();

    void setupSubjectTab();

    void setupQuestionTab();

    void setupConnections();

    // Helper methods
    void populateSubjectCombo();

    MonHoc *getCurrentSubject();

    Lop *getCurrentClass();

};

#endif // TEACHERDASHBOARD_H
