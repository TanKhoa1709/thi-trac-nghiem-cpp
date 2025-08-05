#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Forward declarations
class AuthController;
class QuanLyLop;
class QuanLyMonHoc;
class SinhVien;
class TeacherDashboard;
class StudentDashboard;
class ExamWidget;
class MonHoc;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginRequest();
    void handleLoginSuccess(QString userType, SinhVien* student = nullptr);
    void handleLoginFailed(QString errorMessage);
    void handleLogout();
    void handleExamRequest(MonHoc* subject, int numQuestions);
    void handleExamCompleted(double score);
    void handleExamCancelled();

private:
    Ui::MainWindow *ui;
    QStackedWidget* centralStack;
    
    // Login UI components
    QWidget* loginWidget;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QComboBox* userTypeCombo;
    QPushButton* loginButton;
    
    // Dashboard widgets
    TeacherDashboard* teacherDashboard;
    StudentDashboard* studentDashboard;
    
    // Exam dialog
    ExamWidget* examDialog;
    
    // Controllers
    AuthController* authController;
    QuanLyLop* classManager;
    QuanLyMonHoc* subjectManager;
    
    // Session state
    bool isTeacher;
    SinhVien* currentStudent;
    
    // Setup methods
    void initializeControllers();
    void setupUI();
    void setupConnections();
    void loadInitialData();
    void createLoginWidget();
    void createTeacherDashboard();
    void createStudentDashboard();
    void cleanupResources();
};

#endif // MAINWINDOW_H
