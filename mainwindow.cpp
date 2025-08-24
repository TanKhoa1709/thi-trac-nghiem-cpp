#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "InputValidator.h"
#include "utils/InputValidator.h"
#include "controllers/AuthController.h"
#include "managers/quanlylop.h"
#include "managers/quanlymonhoc.h"
#include "managers/quanlycauhoi.h"
#include "managers/quanlysinhvien.h"
#include "models/sinhvien.h"
#include "models/monhoc.h"
#include "views/TeacherDashboard.h"
#include "views/StudentDashboard.h"
#include "views/ExamWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), centralStack(nullptr), loginWidget(nullptr),
    teacherDashboard(nullptr), studentDashboard(nullptr), examDialog(nullptr), authController(nullptr),
    classManager(nullptr), subjectManager(nullptr), isTeacher(false), currentStudent(nullptr) {
    ui->setupUi(this);

    // Set window properties
    setWindowTitle("Hệ thống quản lý thi trắc nghiệm - CTDL&GT");
    resize(1024, 768);

    // Initialize the application
    initializeControllers();
    setupUI();
    setupConnections();
    loadInitialData();
}

MainWindow::~MainWindow() {
    cleanupResources();
    delete ui;
}

void MainWindow::initializeControllers() {
    // Initialize managers (they will become controllers)
    classManager = new QuanLyLop();
    subjectManager = new QuanLyMonHoc();

    // Initialize authentication controller
    authController = new AuthController(this);
    authController->setClassManager(classManager);
}

void MainWindow::setupUI() {
    // Create central stacked widget
    centralStack = new QStackedWidget(this);
    setCentralWidget(centralStack);

    // Create and setup widgets
    createLoginWidget();
    createTeacherDashboard();
    createStudentDashboard();

    // Start with login screen
    centralStack->setCurrentWidget(loginWidget);
}

void MainWindow::setupConnections() {
    // Connect authentication signals
    connect(authController, &AuthController::loginSuccess,
            this, &MainWindow::handleLoginSuccess);
    connect(authController, &AuthController::loginFailed,
            this, &MainWindow::handleLoginFailed);

    // Connect login button
    connect(loginButton, &QPushButton::clicked,
            this, &MainWindow::handleLoginRequest);
}

void MainWindow::loadInitialData() {
    try {
        // Load class data
        classManager->loadFromFile();

        // Load subject data
        subjectManager->loadFromFile();

        // Load questions for each subject
        DynamicArray<MonHoc *> danhSachMon;
        subjectManager->danhSach(danhSachMon);
        for (int i = 0; i < danhSachMon.size(); i++) {
            if (danhSachMon.get(i)->getQuanLyCauHoi()) {
                danhSachMon.get(i)->getQuanLyCauHoi()->loadFromFile();
            }
        }

        // Load students for each class
        DynamicArray<Lop *> danhSachLop;
        classManager->danhSach(danhSachLop);
        for (int i = 0; i < danhSachLop.size(); i++) {
            if (danhSachLop.get(i)->getQuanLySinhVien()) {
                danhSachLop.get(i)->getQuanLySinhVien()->loadFromFile();

                // Load scores for each student
                DynamicArray<SinhVien *> danhSachSinhVien;
                danhSachLop.get(i)->getQuanLySinhVien()->danhSach(danhSachSinhVien);
                for (int j = 0; j < danhSachSinhVien.size(); j++) {
                    if (danhSachSinhVien.get(j)->getQuanLyDiem()) {
                        danhSachSinhVien.get(j)->getQuanLyDiem()->loadFromFile();
                    }
                }
            }
        }

        statusBar()->showMessage("Data loaded successfully", 3000);

        // Refresh TeacherDashboard if it exists
        if (teacherDashboard) {
            teacherDashboard->refreshAllData();
        }
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Data Loading Error",
                             QString("Error loading data: %1").arg(e.what()));
    }
}

void MainWindow::createLoginWidget() {
    loginWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginWidget);

    // Title
    QLabel *titleLabel = new QLabel("Hệ thống quản lý thi trắc nghiệm");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin: 20px;");

    // Form layout
    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->setSpacing(5);

    // User type selection
    QLabel *userTypeLabel = new QLabel("Loại người dùng:");
    userTypeCombo = new QComboBox();
    userTypeCombo->addItem("Teacher");
    userTypeCombo->addItem("Student");
    userTypeCombo->setStyleSheet("QComboBox { min-width: 200px; padding: 8px; }"
            "QComboBox QAbstractItemView { max-height: 60px; padding: 8px; }");

    // Username field
    QLabel *usernameLabel = new QLabel("Tên đăng nhập:");
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Nhập tên đăng nhập/mã sinh viên");
    usernameEdit->setStyleSheet("padding: 8px;");

    // Apply input validation
    InputValidator::setupInputValidation(usernameEdit, InputValidator::CODE);

    connect(usernameEdit, &QLineEdit::textChanged, this, &MainWindow::onUsernameTextChanged);

    // Password field
    QLabel *passwordLabel = new QLabel("Mật khẩu:");
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Nhập mật khẩu");
    passwordEdit->setStyleSheet("padding: 8px;");

    // Login button
    loginButton = new QPushButton("Đăng nhập");
    loginButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; "
            "padding: 10px; font-size: 14px; border: none; border-radius: 5px; "
            "min-width: 350px; margin-right: 2px; }"
            "QPushButton:hover { background-color: #2980b9; }");

    // Add widgets to form
    // formLayout->addWidget(userTypeLabel);
    formLayout->addWidget(userTypeCombo);
    // formLayout->addWidget(usernameLabel);
    formLayout->addWidget(usernameEdit);
    // formLayout->addWidget(passwordLabel);
    formLayout->addWidget(passwordEdit);
    formLayout->addWidget(loginButton);

    // Center the form
    QWidget *formContainer = new QWidget();
    formContainer->setLayout(formLayout);
    formContainer->setMaximumWidth(400);
    formContainer->setStyleSheet("QWidget { background-color: white; padding: 30px; "
            "border-radius: 10px; border: 1px solid #bdc3c7; }");

    // Main layout
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(formContainer, 0, Qt::AlignCenter);
    layout->addStretch();

    loginWidget->setStyleSheet("background-color: #ecf0f1;");
    centralStack->addWidget(loginWidget);
}

void MainWindow::createTeacherDashboard() {
    teacherDashboard = new TeacherDashboard(this);

    // Set up the controllers for the teacher dashboard
    teacherDashboard->setClassManager(classManager);
    teacherDashboard->setSubjectManager(subjectManager);

    // Connect logout signal
    connect(teacherDashboard, &TeacherDashboard::logoutRequested, this, &MainWindow::handleLogout);

    centralStack->addWidget(teacherDashboard);
}

void MainWindow::createStudentDashboard() {
    studentDashboard = new StudentDashboard(this);

    // Set up the controllers for the student dashboard
    studentDashboard->setSubjectManager(subjectManager);

    // Connect signals
    connect(studentDashboard, &StudentDashboard::logoutRequested, this, &MainWindow::handleLogout);
    connect(studentDashboard, &StudentDashboard::examRequested, this, &MainWindow::handleExamRequest);

    centralStack->addWidget(studentDashboard);
}

void MainWindow::handleLoginRequest() {
    QString username = InputValidator::sanitizeForModel(usernameEdit->text(), InputValidator::CODE);
    QString password = passwordEdit->text();
    QString userType = userTypeCombo->currentText();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Please enter both username and password.");
        return;
    }

    // Validate username format
    if (!InputValidator::isValidCode(username)) {
        InputValidator::showValidationError(this, "Username", "Username must contain only letters and numbers.");
        return;
    }

    // Disable login button during authentication
    loginButton->setEnabled(false);
    loginButton->setText("Logging in...");

    // Authenticate
    authController->authenticate(username, password, userType);
}

void MainWindow::handleLoginSuccess(QString userType, SinhVien *student) {
    // Reset login form
    loginButton->setEnabled(true);
    loginButton->setText("Login");
    usernameEdit->clear();
    passwordEdit->clear();

    // Set session state
    if (userType == "Teacher") {
        isTeacher = true;
        currentStudent = nullptr;
        centralStack->setCurrentWidget(teacherDashboard);
        statusBar()->showMessage("Logged in as Teacher", 3000);
    } else if (userType == "Student") {
        isTeacher = false;
        currentStudent = student;

        // Set current student in dashboard
        if (studentDashboard && student) {
            studentDashboard->setCurrentStudent(student);
        }

        centralStack->setCurrentWidget(studentDashboard);
        if (student) {
            statusBar()->showMessage(QString("Logged in as %1 %2")
                                     .arg(QString::fromStdString(student->getHo()))
                                     .arg(QString::fromStdString(student->getTen())),
                                     3000);
        }
    }
}

void MainWindow::handleLoginFailed(QString errorMessage) {
    // Reset login button
    loginButton->setEnabled(true);
    loginButton->setText("Login");

    // Show error message
    QMessageBox::warning(this, "Đăng nhập thất bại", errorMessage);

    // Clear password field
    passwordEdit->clear();
    passwordEdit->setFocus();
}

void MainWindow::handleLogout() {
    // Reset session state
    isTeacher = false;
    currentStudent = nullptr;

    // Clear login form
    usernameEdit->clear();
    passwordEdit->clear();
    userTypeCombo->setCurrentIndex(0);

    // Return to login screen
    centralStack->setCurrentWidget(loginWidget);
    usernameEdit->setFocus();

    statusBar()->showMessage("Logged out successfully", 3000);
}

void MainWindow::cleanupResources() {
    if (classManager) {
        delete classManager;
        classManager = nullptr;
    }

    if (subjectManager) {
        delete subjectManager;
        subjectManager = nullptr;
    }

    // authController is managed by Qt's parent-child system
}

void MainWindow::handleExamRequest(MonHoc *subject, int numQuestions) {
    if (!currentStudent || !subject) {
        QMessageBox::warning(this, "Lỗi", "Tham số yêu cầu bài thi không hợp lệ.");
        return;
    }

    // Create exam dialog if it doesn't exist
    if (!examDialog) {
        examDialog = new ExamWidget(this);
        connect(examDialog, &ExamWidget::examCompleted, this, &MainWindow::handleExamCompleted);
        connect(examDialog, &ExamWidget::examCancelled, this, &MainWindow::handleExamCancelled);
    }

    // Start the exam
    examDialog->startExam(subject, numQuestions, currentStudent);
    examDialog->exec();
}

void MainWindow::handleExamCompleted(double score) {
    // Refresh student dashboard to show new score
    if (studentDashboard) {
        studentDashboard->refreshScores();
    }

    // Show completion message
    QString message = QString("Exam completed!\nYour score: %1/10")
            .arg(QString::number(score, 'f', 2));

    QMessageBox::information(this, "Exam Results", message);
}

void MainWindow::handleExamCancelled() {
    // Just show a message
    statusBar()->showMessage("Exam was cancelled", 3000);
}

void MainWindow::onUsernameTextChanged(const QString &text) {
    QString upperText = text.toUpper();
    if (text != upperText) {
        int cursorPos = usernameEdit->cursorPosition();
        usernameEdit->blockSignals(true);
        usernameEdit->setText(upperText);
        usernameEdit->blockSignals(false);
        usernameEdit->setCursorPosition(cursorPos);
    }
}
