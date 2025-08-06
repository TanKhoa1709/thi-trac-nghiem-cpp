#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>

// Forward declarations
class SinhVien;
class QuanLyMonHoc;
class MonHoc;
class ExamWidget;

/**
 * @brief Student Dashboard Widget
 * Provides student interface for exams and score viewing
 */
class StudentDashboard : public QWidget {
    Q_OBJECT

public:
    explicit StudentDashboard(QWidget *parent = nullptr);

    ~StudentDashboard();

    // Set current student and controllers
    void setCurrentStudent(SinhVien *student);

    void setSubjectManager(QuanLyMonHoc *manager);

public slots:
    void refreshScores();

    void refreshData();

signals:
    void logoutRequested();

    void examRequested(MonHoc *subject, int numQuestions);

private slots:
    void startExam();

    void viewDetailedScores();

    void viewProfile();

    void onExamCompleted(double score);

private:
    // UI Components
    QLabel *welcomeLabel;
    QLabel *studentInfoLabel;

    // Exam section
    QGroupBox *examGroup;
    QPushButton *takeExamButton;
    QPushButton *viewProfileButton;

    // Scores section
    QGroupBox *scoresGroup;
    QTableWidget *scoresTable;
    QPushButton *viewDetailButton;
    QLabel *statisticsLabel;

    // Logout button
    QPushButton *logoutButton;

    // Current state
    SinhVien *currentStudent;
    QuanLyMonHoc *subjectManager;
    ExamWidget *examDialog;

    // Setup methods
    void setupUI();

    void setupConnections();

    void updateStudentInfo();

    void updateStatistics();

    // Helper methods
    QString formatGrade(double score);

    QString getPassFailStatus(double score);
};

#endif // STUDENTDASHBOARD_H
