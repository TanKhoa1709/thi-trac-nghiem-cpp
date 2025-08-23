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
#include <QCheckBox>

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

    // Score management
    void refreshScoreList();

    void onClassChangedForScores();

    void onSubjectChangedForScores();

    void onHideNotTakenChanged();

    void viewStudentExamDetails();

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

    // Score Management Tab
    QWidget *scoreTab;
    QComboBox *classComboForScores;
    QComboBox *subjectComboForScores;
    QTableWidget *scoreTable;
    QPushButton *viewDetailsButton;
    QCheckBox *hideNotTakenCheckBox;

    // Controllers
    QuanLyLop *classManager;
    QuanLyMonHoc *subjectManager;

    // Current selections
    QString currentClassCode;
    QString currentSubjectCode;
    QString currentClassCodeForScores;
    QString currentSubjectCodeForScores;

    // Setup methods
    void setupUI();

    void setupClassTab();

    void setupSubjectTab();

    void setupQuestionTab();

    void setupScoreTab();

    void setupConnections();

    // Helper methods
    void populateSubjectCombo();

    void populateClassComboForScores();

    void populateSubjectComboForScores();

    MonHoc *getCurrentSubject();

    Lop *getCurrentClass();

};

#endif // TEACHERDASHBOARD_H
