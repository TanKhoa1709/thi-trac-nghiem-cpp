#include "TeacherDashboard.h"
#include "DetailedResultsWidget.h"
#include "../managers/quanlylop.h"
#include "../managers/quanlymonhoc.h"
#include "../managers/quanlysinhvien.h"
#include "../managers/quanlycauhoi.h"
#include "../managers/quanlydiem.h"
#include "../models/lop.h"
#include "../models/monhoc.h"
#include "../models/sinhvien.h"
#include "../models/cauhoi.h"
#include "../models/diemthi.h"
#include "../utils/DynamicArray.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextEdit>
#include <cstring>
#include "../utils/InputValidator.h"

TeacherDashboard::TeacherDashboard(QWidget *parent) :
    QWidget(parent), mainTabs(nullptr), classManager(nullptr), subjectManager(nullptr) {
    setupUI();
    setupConnections();
}

TeacherDashboard::~TeacherDashboard() {
    // Qt handles widget cleanup through parent-child relationships
}

void TeacherDashboard::setClassManager(QuanLyLop *manager) {
    classManager = manager;
    // Don't refresh immediately - will be called manually after data loading
}

void TeacherDashboard::setSubjectManager(QuanLyMonHoc *manager) {
    subjectManager = manager;
    // Don't refresh immediately - will be called manually after data loading
}

void TeacherDashboard::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Title
    QLabel *titleLabel = new QLabel("Bảng điều khiển giáo viên");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px;");

    // Create main tabs
    mainTabs = new QTabWidget();

    setupClassTab();
    setupSubjectTab();
    setupQuestionTab();
    setupReportsTab();

    // Logout button
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    QPushButton *logoutButton = new QPushButton("Đăng xuất");
    logoutButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; "
            "padding: 8px 16px; font-size: 12px; border: none; border-radius: 4px; }"
            "QPushButton:hover { background-color: #c0392b; }");
    connect(logoutButton, &QPushButton::clicked, this, &TeacherDashboard::logoutRequested);
    bottomLayout->addWidget(logoutButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(mainTabs);
    mainLayout->addLayout(bottomLayout);
}

void TeacherDashboard::setupClassTab() {
    classTab = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(classTab);

    // Left side - Classes
    QVBoxLayout *classLayout = new QVBoxLayout();
    QLabel *classLabel = new QLabel("Lớp Học");
    classLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    classTable = new QTableWidget(0, 3);
    classTable->setHorizontalHeaderLabels({"Mã Lớp", "Tên Lớp", "Số Sinh Viên"});
    classTable->horizontalHeader()->setStretchLastSection(true);
    classTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *classButtonLayout = new QHBoxLayout();
    addClassButton = new QPushButton("Thêm Lớp");
    editClassButton = new QPushButton("Sửa Lớp");
    deleteClassButton = new QPushButton("Xóa Lớp");

    addClassButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    editClassButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    deleteClassButton->setStyleSheet(
            "QPushButton { background-color: #e74c3c; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");

    classButtonLayout->addWidget(addClassButton);
    classButtonLayout->addWidget(editClassButton);
    classButtonLayout->addWidget(deleteClassButton);
    classButtonLayout->addStretch();

    classLayout->addWidget(classLabel);
    classLayout->addWidget(classTable);
    classLayout->addLayout(classButtonLayout);

    // Right side - Students in selected class
    QVBoxLayout *studentLayout = new QVBoxLayout();
    QLabel *studentLabel = new QLabel("Sinh Viên Trong Lớp");
    studentLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    studentTable = new QTableWidget(0, 4);
    studentTable->setHorizontalHeaderLabels({"Mã SV", "Họ", "Tên", "Phái"});
    studentTable->horizontalHeader()->setStretchLastSection(true);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *studentButtonLayout = new QHBoxLayout();
    addStudentButton = new QPushButton("Thêm Sinh Viên");
    editStudentButton = new QPushButton("Sửa Sinh Viên");
    deleteStudentButton = new QPushButton("Xóa Sinh Viên");

    addStudentButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    editStudentButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    deleteStudentButton->setStyleSheet(
            "QPushButton { background-color: #e74c3c; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");

    studentButtonLayout->addWidget(addStudentButton);
    studentButtonLayout->addWidget(editStudentButton);
    studentButtonLayout->addWidget(deleteStudentButton);
    studentButtonLayout->addStretch();

    studentLayout->addWidget(studentLabel);
    studentLayout->addWidget(studentTable);
    studentLayout->addLayout(studentButtonLayout);

    // Add to main layout
    layout->addLayout(classLayout, 1);
    layout->addLayout(studentLayout, 1);

    mainTabs->addTab(classTab, "Quản Lý Lớp");
}

void TeacherDashboard::setupSubjectTab() {
    subjectTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(subjectTab);

    QLabel *subjectLabel = new QLabel("Môn học");
    subjectLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    subjectTable = new QTableWidget(0, 3);
    subjectTable->setHorizontalHeaderLabels({"Mã Môn", "Tên Môn", "Chỉ Số"});
    subjectTable->horizontalHeader()->setStretchLastSection(true);
    subjectTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addSubjectButton = new QPushButton("Thêm Môn");
    editSubjectButton = new QPushButton("Sửa Môn");
    deleteSubjectButton = new QPushButton("Xóa Môn");

    addSubjectButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    editSubjectButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    deleteSubjectButton->setStyleSheet(
            "QPushButton { background-color: #e74c3c; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");

    buttonLayout->addWidget(addSubjectButton);
    buttonLayout->addWidget(editSubjectButton);
    buttonLayout->addWidget(deleteSubjectButton);
    buttonLayout->addStretch();

    layout->addWidget(subjectLabel);
    layout->addWidget(subjectTable);
    layout->addLayout(buttonLayout);

    mainTabs->addTab(subjectTab, "Quản Lý Môn");
}

void TeacherDashboard::setupQuestionTab() {
    questionTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(questionTab);

    // Subject selection
    QHBoxLayout *subjectSelectLayout = new QHBoxLayout();
    QLabel *selectLabel = new QLabel("Chọn môn học:");
    subjectComboForQuestions = new QComboBox();
    subjectSelectLayout->addWidget(selectLabel);
    subjectSelectLayout->addWidget(subjectComboForQuestions);
    subjectSelectLayout->addStretch();

    QLabel *questionLabel = new QLabel("Câu hỏi");
    questionLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    questionTable = new QTableWidget(0, 3);
    questionTable->setHorizontalHeaderLabels({"Mã câu hỏi", "Nội dung câu hỏi", "Đáp án đúng"});
    questionTable->horizontalHeader()->setStretchLastSection(true);
    questionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addQuestionButton = new QPushButton("Thêm Câu Hỏi");
    editQuestionButton = new QPushButton("Sửa Câu Hỏi");
    deleteQuestionButton = new QPushButton("Xóa Câu Hỏi");

    addQuestionButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    editQuestionButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    deleteQuestionButton->setStyleSheet(
            "QPushButton { background-color: #e74c3c; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");

    buttonLayout->addWidget(addQuestionButton);
    buttonLayout->addWidget(editQuestionButton);
    buttonLayout->addWidget(deleteQuestionButton);
    buttonLayout->addStretch();

    layout->addLayout(subjectSelectLayout);
    layout->addWidget(questionLabel);
    layout->addWidget(questionTable);
    layout->addLayout(buttonLayout);

    mainTabs->addTab(questionTab, "Quản Lý Câu Hỏi");
}

void TeacherDashboard::setupReportsTab() {
    reportsTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(reportsTab);

    QLabel *titleLabel = new QLabel("Bảng Điểm Thi Trắc Nghiệm");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; margin: 20px;");

    // Class selection
    QHBoxLayout *classSelectLayout = new QHBoxLayout();
    QLabel *classLabel = new QLabel("Chọn lớp:");
    reportClassCombo = new QComboBox();

    classSelectLayout->addWidget(classLabel);
    classSelectLayout->addWidget(reportClassCombo);
    classSelectLayout->addStretch();

    // Subject selection
    QHBoxLayout *subjectSelectLayout = new QHBoxLayout();
    QLabel *subjectLabel = new QLabel("Chọn môn học:");
    reportSubjectCombo = new QComboBox();

    subjectSelectLayout->addWidget(subjectLabel);
    subjectSelectLayout->addWidget(reportSubjectCombo);
    subjectSelectLayout->addStretch();

    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    generateReportButton = new QPushButton("Tạo bảng điểm");
    exportReportButton = new QPushButton("Xuất file");
    viewDetailsButton = new QPushButton("Xem chi tiết");

    generateReportButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 10px 20px; border: none; border-radius: 5px; }");
    exportReportButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 10px 20px; border: none; border-radius: 5px; }");
    viewDetailsButton->setStyleSheet(
            "QPushButton { background-color: #9b59b6; color: white; padding: 10px 20px; border: none; border-radius: 5px; }");

    exportReportButton->setEnabled(false); // Enabled after report generation
    viewDetailsButton->setEnabled(false); // Enabled after report generation

    buttonLayout->addWidget(generateReportButton);
    buttonLayout->addWidget(exportReportButton);
    buttonLayout->addWidget(viewDetailsButton);
    buttonLayout->addStretch();

    // Report display table
    QLabel *reportLabel = new QLabel("Kết quả:");
    reportLabel->setStyleSheet("font-weight: bold; margin-top: 20px;");

    reportTable = new QTableWidget();
    reportTable->setAlternatingRowColors(true);
    reportTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    reportTable->horizontalHeader()->setStretchLastSection(true);

    // Assembly
    layout->addWidget(titleLabel);
    layout->addLayout(classSelectLayout);
    layout->addLayout(subjectSelectLayout);
    layout->addLayout(buttonLayout);
    layout->addWidget(reportLabel);
    layout->addWidget(reportTable);

    mainTabs->addTab(reportsTab, "Bảng Điểm");
}

void TeacherDashboard::setupConnections() {
    // Class management connections
    connect(classTable, &QTableWidget::itemSelectionChanged, this, &TeacherDashboard::onClassSelected);
    connect(addClassButton, &QPushButton::clicked, this, &TeacherDashboard::addNewClass);
    connect(editClassButton, &QPushButton::clicked, this, &TeacherDashboard::editClass);
    connect(deleteClassButton, &QPushButton::clicked, this, &TeacherDashboard::deleteClass);

    // Student management connections
    connect(addStudentButton, &QPushButton::clicked, this, &TeacherDashboard::addNewStudent);
    connect(editStudentButton, &QPushButton::clicked, this, &TeacherDashboard::editStudent);
    connect(deleteStudentButton, &QPushButton::clicked, this, &TeacherDashboard::deleteStudent);

    // Subject management connections
    connect(addSubjectButton, &QPushButton::clicked, this, &TeacherDashboard::addNewSubject);
    connect(editSubjectButton, &QPushButton::clicked, this, &TeacherDashboard::editSubject);
    connect(deleteSubjectButton, &QPushButton::clicked, this, &TeacherDashboard::deleteSubject);

    // Question management connections
    connect(subjectComboForQuestions, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TeacherDashboard::onSubjectChangedForQuestions);
    connect(addQuestionButton, &QPushButton::clicked, this, &TeacherDashboard::addNewQuestion);
    connect(editQuestionButton, &QPushButton::clicked, this, &TeacherDashboard::editQuestion);
    connect(deleteQuestionButton, &QPushButton::clicked, this, &TeacherDashboard::deleteQuestion);

    // Report management connections
    connect(generateReportButton, &QPushButton::clicked, this, &TeacherDashboard::generateExamScoreReport);
    connect(exportReportButton, &QPushButton::clicked, this, &TeacherDashboard::exportReport);
    connect(viewDetailsButton, &QPushButton::clicked, this, &TeacherDashboard::viewDetailedResults);
}

void TeacherDashboard::refreshAllData() {
    refreshClassList();
    refreshSubjectList();
    populateSubjectCombo();
    populateReportCombos();
    refreshQuestionList();
}

void TeacherDashboard::refreshClassList() {
    if (!classManager)
        return;

    DynamicArray<Lop *> danhSachLop;
    classManager->danhSach(danhSachLop);

    classTable->setRowCount(danhSachLop.size());
    for (int i = 0; i < danhSachLop.size(); i++) {
        Lop *lop = danhSachLop.get(i);
        classTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(lop->getMaLop())));
        classTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(lop->getTenLop())));

        int studentCount = lop->getQuanLySinhVien() ? lop->getQuanLySinhVien()->size() : 0;
        classTable->setItem(i, 2, new QTableWidgetItem(QString::number(studentCount)));
    }
}

void TeacherDashboard::onClassSelected() {
    int row = classTable->currentRow();
    if (row >= 0) {
        QTableWidgetItem *item = classTable->item(row, 0);
        if (item) {
            currentClassCode = item->text();
            refreshStudentList();
        }
    }
}

void TeacherDashboard::refreshStudentList() {
    if (!classManager || currentClassCode.isEmpty()) {
        studentTable->setRowCount(0);
        return;
    }

    Lop *lop = classManager->tim(currentClassCode.toStdString());
    if (!lop || !lop->getQuanLySinhVien()) {
        studentTable->setRowCount(0);
        return;
    }

    DynamicArray<SinhVien *> danhSachSV;
    lop->getQuanLySinhVien()->danhSach(danhSachSV);

    studentTable->setRowCount(danhSachSV.size());
    for (int i = 0; i < danhSachSV.size(); i++) {
        SinhVien *sv = danhSachSV.get(i);
        studentTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(sv->getMaSinhVien())));
        studentTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(sv->getHo())));
        studentTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(sv->getTen())));
        studentTable->setItem(i, 3, new QTableWidgetItem(sv->getPhai() ? "Nam" : "Nữ"));
    }
}

void TeacherDashboard::addNewClass() {
    // Create custom dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm lớp mới");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Class code input
    QLabel *codeLabel = new QLabel("Mã Lớp:");
    QLineEdit *codeEdit = new QLineEdit();
    InputValidator::setupInputValidation(codeEdit, InputValidator::CODE);

    // Class name input
    QLabel *nameLabel = new QLabel("Tên Lớp:");
    QLineEdit *nameEdit = new QLineEdit();
    InputValidator::setupInputValidation(nameEdit, InputValidator::NONE);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Thêm Lớp");
    QPushButton *cancelButton = new QPushButton("Hủy");

    okButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString classCode = InputValidator::sanitizeForModel(codeEdit->text(), InputValidator::CODE);
        QString className = InputValidator::sanitizeForModel(nameEdit->text(), InputValidator::GENERAL);

        if (classCode.trimmed().isEmpty() || className.trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập đầy đủ mã lớp và tên lớp.");
            return;
        }

        if (classManager) {
            // Create new class following memory management rules
            Lop *lopMoi = new Lop(classCode.toStdString(), className.toStdString());
            if (classManager->them(*lopMoi)) {
                classManager->saveToFile();
                QMessageBox::information(&dialog, "Thành công", "Đã thêm lớp thành công!");
                dialog.accept();
            } else {
                delete lopMoi; // Clean up if adding failed
                QMessageBox::warning(&dialog, "Lỗi", "Không thể thêm lớp. Mã lớp có thể đã tồn tại.");
            }
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshClassList();
    }
}

void TeacherDashboard::addNewStudent() {
    if (currentClassCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một lớp trước.");
        return;
    }

    // Create custom dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm sinh viên mới");
    dialog.setMinimumSize(450, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Student ID input
    QLabel *idLabel = new QLabel("Mã sinh viên:");
    QLineEdit *idEdit = new QLineEdit();
    InputValidator::setupInputValidation(idEdit, InputValidator::CODE);

    // Last name input
    QLabel *lastNameLabel = new QLabel("Họ:");
    QLineEdit *lastNameEdit = new QLineEdit();
    InputValidator::setupInputValidation(lastNameEdit, InputValidator::GENERAL);

    // First name input
    QLabel *firstNameLabel = new QLabel("Tên:");
    QLineEdit *firstNameEdit = new QLineEdit();
    InputValidator::setupInputValidation(firstNameEdit, InputValidator::GENERAL);

    // Gender selection
    QLabel *genderLabel = new QLabel("Giới tính:");
    QComboBox *genderCombo = new QComboBox();
    genderCombo->addItems({"Nam", "Nữ"});

    // Password input
    QLabel *passwordLabel = new QLabel("Mật khẩu:");
    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Normal); // Show password for admin convenience
    InputValidator::setupInputValidation(passwordEdit, InputValidator::GENERAL);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Thêm Sinh Viên");
    QPushButton *cancelButton = new QPushButton("Hủy");

    okButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(idLabel);
    layout->addWidget(idEdit);
    layout->addWidget(lastNameLabel);
    layout->addWidget(lastNameEdit);
    layout->addWidget(firstNameLabel);
    layout->addWidget(firstNameEdit);
    layout->addWidget(genderLabel);
    layout->addWidget(genderCombo);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString studentId = InputValidator::sanitizeForModel(idEdit->text(), InputValidator::CODE);
        QString lastName = InputValidator::sanitizeForModel(lastNameEdit->text(), InputValidator::GENERAL);
        QString firstName = InputValidator::sanitizeForModel(firstNameEdit->text(), InputValidator::GENERAL);
        QString gender = genderCombo->currentText();
        QString password = InputValidator::sanitizeForModel(passwordEdit->text(), InputValidator::GENERAL);

        if (studentId.trimmed().isEmpty() || lastName.trimmed().isEmpty() || 
            firstName.trimmed().isEmpty() || password.trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập đầy đủ tất cả các trường.");
            return;
        }

        Lop *lop = classManager->tim(currentClassCode.toStdString());
        if (lop && lop->getQuanLySinhVien()) {
            // Create new student following memory management rules
            SinhVien *svMoi = new SinhVien(studentId.toStdString(), lastName.toStdString(),
                                           firstName.toStdString(), gender == "Nam", password.toStdString());
            if (lop->getQuanLySinhVien()->them(*svMoi)) {
                lop->getQuanLySinhVien()->saveToFile();
                QMessageBox::information(&dialog, "Thành công", "Đã thêm sinh viên thành công!");
                dialog.accept();
            } else {
                delete svMoi; // Clean up if adding failed
                QMessageBox::warning(&dialog, "Lỗi", "Không thể thêm sinh viên. Mã sinh viên có thể đã tồn tại.");
            }
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshStudentList();
        refreshClassList(); // Update student count
    }
}

void TeacherDashboard::refreshSubjectList() {
    if (!subjectManager)
        return;

    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    subjectTable->setRowCount(danhSachMon.size());
    for (int i = 0; i < danhSachMon.size(); i++) {
        MonHoc *mon = danhSachMon.get(i);
        subjectTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(mon->getMaMon())));
        subjectTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(mon->getTenMon())));

        int questionCount = mon->getQuanLyCauHoi() ? mon->getQuanLyCauHoi()->size() : 0;
        subjectTable->setItem(i, 2, new QTableWidgetItem(QString::number(questionCount)));
    }
}

void TeacherDashboard::addNewSubject() {
    // Create custom dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm môn học mới");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Subject code input
    QLabel *codeLabel = new QLabel("Mã Môn:");
    QLineEdit *codeEdit = new QLineEdit();
    InputValidator::setupInputValidation(codeEdit, InputValidator::CODE);

    // Subject name input
    QLabel *nameLabel = new QLabel("Tên Môn:");
    QLineEdit *nameEdit = new QLineEdit();
    InputValidator::setupInputValidation(nameEdit, InputValidator::NONE);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Thêm Môn");
    QPushButton *cancelButton = new QPushButton("Hủy");

    okButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString subjectCode = InputValidator::sanitizeForModel(codeEdit->text(), InputValidator::CODE);
        QString subjectName = InputValidator::sanitizeForModel(nameEdit->text(), InputValidator::GENERAL);

        if (subjectCode.trimmed().isEmpty() || subjectName.trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập đầy đủ mã môn và tên môn.");
            return;
        }

        if (subjectManager) {
            // Create new subject following memory management rules
            MonHoc *monMoi = new MonHoc(subjectCode.toStdString().c_str(), subjectName.toStdString(),
                                        subjectManager->size());
            if (subjectManager->them(*monMoi)) {
                subjectManager->saveToFile();
                QMessageBox::information(&dialog, "Thành công", "Đã thêm môn học thành công!");
                dialog.accept();
            } else {
                delete monMoi; // Clean up if adding failed
                QMessageBox::warning(&dialog, "Lỗi", "Không thể thêm môn học. Mã môn có thể đã tồn tại.");
            }
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshSubjectList();
        populateSubjectCombo();
    }
}

void TeacherDashboard::populateSubjectCombo() {
    if (!subjectManager)
        return;

    subjectComboForQuestions->clear();

    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    for (int i = 0; i < danhSachMon.size(); i++) {
        MonHoc *mon = danhSachMon.get(i);
        QString item = QString("%1 - %2").arg(QString::fromStdString(mon->getMaMon())).arg(
                QString::fromStdString(mon->getTenMon()));
        subjectComboForQuestions->addItem(item, QString::fromStdString(mon->getMaMon()));
    }
}

void TeacherDashboard::onSubjectChangedForQuestions() {
    if (subjectComboForQuestions->currentIndex() >= 0) {
        currentSubjectCode = subjectComboForQuestions->currentData().toString();
        refreshQuestionList();
    }
}

void TeacherDashboard::refreshQuestionList() {
    if (!subjectManager || currentSubjectCode.isEmpty()) {
        questionTable->setRowCount(0);
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi()) {
        questionTable->setRowCount(0);
        return;
    }

    DynamicArray<CauHoi *> danhSachCH;
    mon->getQuanLyCauHoi()->danhSach(danhSachCH);

    questionTable->setRowCount(danhSachCH.size());
    for (int i = 0; i < danhSachCH.size(); i++) {
        CauHoi *ch = danhSachCH.get(i);
        questionTable->setItem(i, 0, new QTableWidgetItem(QString::number(ch->getMaCauHoi())));

        // Truncate long questions for display
        QString content = QString::fromStdString(ch->getNoiDung());
        if (content.length() > 50) {
            content = content.left(47) + "...";
        }
        questionTable->setItem(i, 1, new QTableWidgetItem(content));
        questionTable->setItem(i, 2, new QTableWidgetItem(QString(ch->getDapAnDung())));
    }
}

void TeacherDashboard::addNewQuestion() {
    if (currentSubjectCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một môn học trước.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi()) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy môn học hoặc quản lý câu hỏi chưa được khởi tạo.");
        return;
    }

    // Create dialog for adding new question
    QDialog dialog(this);
    dialog.setWindowTitle("Thêm câu hỏi mới");
    dialog.setMinimumSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Question content
    QLabel *contentLabel = new QLabel("Nội dung câu hỏi:");
    QTextEdit *contentEdit = new QTextEdit();
    contentEdit->setPlaceholderText("Nhập nội dung câu hỏi ở đây...");

    // Answer options
    QLabel *optionsLabel = new QLabel("Các lựa chọn trả lời:");
    QLineEdit *optionA = new QLineEdit();
    QLineEdit *optionB = new QLineEdit();
    QLineEdit *optionC = new QLineEdit();
    QLineEdit *optionD = new QLineEdit();

    optionA->setPlaceholderText("Lựa chọn A");
    optionB->setPlaceholderText("Lựa chọn B");
    optionC->setPlaceholderText("Lựa chọn C");
    optionD->setPlaceholderText("Lựa chọn D");

    // Correct answer
    QLabel *correctLabel = new QLabel("Đáp án đúng:");
    QComboBox *correctCombo = new QComboBox();
    correctCombo->addItems({"A", "B", "C", "D"});

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Thêm Câu Hỏi");
    QPushButton *cancelButton = new QPushButton("Hủy");

    okButton->setStyleSheet(
            "QPushButton { background-color: #27ae60; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // Layout assembly
    layout->addWidget(contentLabel);
    layout->addWidget(contentEdit);
    layout->addWidget(optionsLabel);
    layout->addWidget(optionA);
    layout->addWidget(optionB);
    layout->addWidget(optionC);
    layout->addWidget(optionD);
    layout->addWidget(correctLabel);
    layout->addWidget(correctCombo);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString content = contentEdit->toPlainText().trimmed();
        QString a = optionA->text().trimmed();
        QString b = optionB->text().trimmed();
        QString c = optionC->text().trimmed();
        QString d = optionD->text().trimmed();
        char correct = correctCombo->currentText().at(0).toLatin1();

        if (content.isEmpty() || a.isEmpty() || b.isEmpty() || c.isEmpty() || d.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng điền đầy đủ tất cả các trường.");
            return;
        }

        // Create new question with heap allocation (following user rules)
        CauHoi *newQuestion = new CauHoi();

        // Generate unique ID
        int newId = mon->getQuanLyCauHoi()->taoMaCauHoiNgauNhien();
        newQuestion->setMaCauHoi(newId);
        newQuestion->setNoiDung(content.toStdString());
        newQuestion->setLuaChonA(a.toStdString());
        newQuestion->setLuaChonB(b.toStdString());
        newQuestion->setLuaChonC(c.toStdString());
        newQuestion->setLuaChonD(d.toStdString());
        newQuestion->setDapAnDung(correct);

        // Add to manager using dereferenced value (following user rules)
        if (mon->getQuanLyCauHoi()->them(*newQuestion)) {
            // Save changes to file immediately
            mon->getQuanLyCauHoi()->saveToFile();
            QMessageBox::information(&dialog, "Thành công", "Đã thêm câu hỏi thành công!");
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Lỗi", "Không thể thêm câu hỏi. Vui lòng kiểm tra đầu vào.");
            delete newQuestion; // Clean up on failure
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshQuestionList();
    }
}

void TeacherDashboard::editClass() {
    int row = classTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một lớp để sửa.");
        return;
    }

    QTableWidgetItem *item = classTable->item(row, 0);
    if (!item)
        return;

    QString oldClassCode = item->text();
    Lop *lop = classManager->tim(oldClassCode.toStdString());
    if (!lop) {
        QMessageBox::warning(this, "Lỗi", "Lớp đã chọn không tồn tại.");
        return;
    }

    // Create custom dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Sửa lớp");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Class code input (read-only)
    QLabel *codeLabel = new QLabel("Mã Lớp:");
    QLineEdit *codeEdit = new QLineEdit(oldClassCode);
    codeEdit->setReadOnly(true);
    codeEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; color: #666; }");

    // Class name input
    QLabel *nameLabel = new QLabel("Tên Lớp:");
    QLineEdit *nameEdit = new QLineEdit(QString::fromStdString(lop->getTenLop()));
    InputValidator::setupInputValidation(nameEdit, InputValidator::NONE);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Cập nhật");
    QPushButton *cancelButton = new QPushButton("Hủy");

    okButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString newClassName = InputValidator::sanitizeForModel(nameEdit->text(), InputValidator::GENERAL);

        if (newClassName.trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập tên lớp.");
            return;
        }

        // Update the class object
        lop->setTenLop(newClassName.toStdString());

        if (classManager->sua(*lop)) {
            classManager->saveToFile();
            refreshClassList();
            QMessageBox::information(&dialog, "Thành công", "Đã cập nhật lớp thành công!");
            dialog.accept();
        } else {
            // Restore original values if update failed
            lop->setTenLop(item ? classTable->item(row, 1)->text().toStdString() : "");
            QMessageBox::warning(&dialog, "Lỗi", "Không thể cập nhật lớp.");
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshClassList();
    }
}

void TeacherDashboard::deleteClass() {
    int row = classTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một lớp để xóa.");
        return;
    }

    QTableWidgetItem *item = classTable->item(row, 0);
    if (!item)
        return;

    QString classCode = item->text();
    QString className = classTable->item(row, 1)->text();

    // Confirmation dialog
    int result = QMessageBox::question(this, "Xác nhận xóa",
                                       QString("Bạn có chắc chắn muốn xóa lớp '%1 - %2'?\n\n"
                                               "Việc này sẽ xóa tất cả sinh viên trong lớp và kết quả thi của họ.\n"
                                               "Hành động này không thể hoàn tác.")
                                       .arg(classCode)
                                       .arg(className),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);

    if (result != QMessageBox::Yes)
        return;

    if (classManager->xoa(classCode.toStdString())) {
        classManager->saveToFile();

        // Clear current selection if it was the deleted class
        if (currentClassCode == classCode) {
            currentClassCode.clear();
            studentTable->setRowCount(0);
        }

        refreshClassList();
        QMessageBox::information(this, "Thành công", "Đã xóa lớp thành công!");
    } else {
        QMessageBox::warning(this, "Lỗi", "Không thể xóa lớp.");
    }
}

void TeacherDashboard::editStudent() {
    if (currentClassCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một lớp trước.");
        return;
    }

    int row = studentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một sinh viên để sửa.");
        return;
    }

    QTableWidgetItem *item = studentTable->item(row, 0);
    if (!item)
        return;

    QString oldStudentId = item->text();

    Lop *lop = classManager->tim(currentClassCode.toStdString());
    if (!lop || !lop->getQuanLySinhVien()) {
        QMessageBox::warning(this, "Lỗi", "Quản lý lớp hoặc sinh viên chưa được khởi tạo.");
        return;
    }

    SinhVien *sv = lop->getQuanLySinhVien()->tim(oldStudentId.toStdString());
    if (!sv) {
        QMessageBox::warning(this, "Lỗi", "Sinh viên đã chọn không tồn tại.");
        return;
    }

    // Create custom dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Sửa sinh viên");
    dialog.setMinimumSize(450, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Student ID input (read-only)
    QLabel *idLabel = new QLabel("Mã sinh viên:");
    QLineEdit *idEdit = new QLineEdit(oldStudentId);
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; color: #666; }");

    // Last name input
    QLabel *lastNameLabel = new QLabel("Họ:");
    QLineEdit *lastNameEdit = new QLineEdit(QString::fromStdString(sv->getHo()));
    InputValidator::setupInputValidation(lastNameEdit, InputValidator::GENERAL);

    // First name input
    QLabel *firstNameLabel = new QLabel("Tên:");
    QLineEdit *firstNameEdit = new QLineEdit(QString::fromStdString(sv->getTen()));
    InputValidator::setupInputValidation(firstNameEdit, InputValidator::GENERAL);

    // Gender selection
    QLabel *genderLabel = new QLabel("Giới tính:");
    QComboBox *genderCombo = new QComboBox();
    genderCombo->addItems({"Nam", "Nữ"});
    genderCombo->setCurrentText(sv->getPhai() ? "Nam" : "Nữ");

    // Password input
    QLabel *passwordLabel = new QLabel("Mật khẩu:");
    QLineEdit *passwordEdit = new QLineEdit(QString::fromStdString(sv->getPassword()));
    passwordEdit->setEchoMode(QLineEdit::Normal); // Show password for admin convenience
    InputValidator::setupInputValidation(passwordEdit, InputValidator::GENERAL);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Cập nhật");
    QPushButton *cancelButton = new QPushButton("Hủy");

    okButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(idLabel);
    layout->addWidget(idEdit);
    layout->addWidget(lastNameLabel);
    layout->addWidget(lastNameEdit);
    layout->addWidget(firstNameLabel);
    layout->addWidget(firstNameEdit);
    layout->addWidget(genderLabel);
    layout->addWidget(genderCombo);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString newLastName = InputValidator::sanitizeForModel(lastNameEdit->text(), InputValidator::GENERAL);
        QString newFirstName = InputValidator::sanitizeForModel(firstNameEdit->text(), InputValidator::GENERAL);
        QString newGender = genderCombo->currentText();
        QString newPassword = InputValidator::sanitizeForModel(passwordEdit->text(), InputValidator::GENERAL);

        if (newLastName.trimmed().isEmpty() || newFirstName.trimmed().isEmpty() || 
            newPassword.trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập đầy đủ tất cả các trường.");
            return;
        }

        // Update the student object
        sv->setHo(newLastName.toStdString());
        sv->setTen(newFirstName.toStdString());
        sv->setPhai(newGender == "Nam");
        sv->setPassword(newPassword.toStdString());

        if (lop->getQuanLySinhVien()->sua(*sv)) {
            lop->getQuanLySinhVien()->saveToFile();
            refreshStudentList();
            refreshClassList(); // Update student count if needed
            QMessageBox::information(&dialog, "Thành công", "Đã cập nhật sinh viên thành công!");
            dialog.accept();
        } else {
            // Restore original values if update failed
            sv->setHo(studentTable->item(row, 1)->text().toStdString());
            sv->setTen(studentTable->item(row, 2)->text().toStdString());
            sv->setPhai(studentTable->item(row, 3)->text() == "Nam");
            QMessageBox::warning(&dialog, "Lỗi", "Không thể cập nhật sinh viên.");
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshStudentList();
        refreshClassList(); // Update student count if needed
    }
}

void TeacherDashboard::deleteStudent() {
    if (currentClassCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một lớp trước.");
        return;
    }

    int row = studentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một sinh viên để xóa.");
        return;
    }

    QTableWidgetItem *item = studentTable->item(row, 0);
    if (!item)
        return;

    QString studentId = item->text();
    QString studentName = QString("%1 %2").arg(studentTable->item(row, 1)->text()).arg(
            studentTable->item(row, 2)->text());

    // Confirmation dialog
    int result = QMessageBox::question(this, "Xác nhận xóa",
                                       QString("Bạn có chắc chắn muốn xóa sinh viên '%1 - %2'?\n\n"
                                               "Việc này sẽ xóa tất cả kết quả thi của sinh viên này.\n"
                                               "Hành động này không thể hoàn tác.")
                                       .arg(studentId)
                                       .arg(studentName),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);

    if (result != QMessageBox::Yes)
        return;

    Lop *lop = classManager->tim(currentClassCode.toStdString());
    if (!lop || !lop->getQuanLySinhVien()) {
        QMessageBox::warning(this, "Lỗi", "Quản lý lớp hoặc sinh viên chưa được khởi tạo.");
        return;
    }

    if (lop->getQuanLySinhVien()->xoa(studentId.toStdString())) {
        lop->getQuanLySinhVien()->saveToFile();
        refreshStudentList();
        refreshClassList(); // Update student count
        QMessageBox::information(this, "Thành công", "Đã xóa sinh viên thành công!");
    } else {
        QMessageBox::warning(this, "Lỗi", "Không thể xóa sinh viên.");
    }
}

void TeacherDashboard::editSubject() {
    int row = subjectTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một môn học để sửa.");
        return;
    }

    QTableWidgetItem *item = subjectTable->item(row, 0);
    if (!item)
        return;

    QString oldSubjectCode = item->text();
    MonHoc *mon = subjectManager->tim(oldSubjectCode.toStdString().c_str());
    if (!mon) {
        QMessageBox::warning(this, "Lỗi", "Môn học đã chọn không tồn tại.");
        return;
    }

    // Create custom dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Sửa môn học");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Subject code input (read-only)
    QLabel *codeLabel = new QLabel("Mã Môn:");
    QLineEdit *codeEdit = new QLineEdit(oldSubjectCode);
    codeEdit->setReadOnly(true);
    codeEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; color: #666; }");

    // Subject name input
    QLabel *nameLabel = new QLabel("Tên Môn:");
    QLineEdit *nameEdit = new QLineEdit(QString::fromStdString(mon->getTenMon()));
    InputValidator::setupInputValidation(nameEdit, InputValidator::NONE);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Cập nhật");
    QPushButton *cancelButton = new QPushButton("Hủy");

    okButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString newSubjectName = InputValidator::sanitizeForModel(nameEdit->text(), InputValidator::GENERAL);

        if (newSubjectName.trimmed().isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng nhập tên môn.");
            return;
        }

        // Update the subject object
        mon->setTenMon(newSubjectName.toStdString());

        if (subjectManager->sua(*mon)) {
            subjectManager->saveToFile();
            refreshSubjectList();
            populateSubjectCombo();
            QMessageBox::information(&dialog, "Thành công", "Đã cập nhật môn học thành công!");
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Lỗi", "Không thể cập nhật môn học.");
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshSubjectList();
        populateSubjectCombo();
    }
}

void TeacherDashboard::deleteSubject() {
    int row = subjectTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một môn học để xóa.");
        return;
    }

    QTableWidgetItem *item = subjectTable->item(row, 0);
    if (!item)
        return;

    QString subjectCode = item->text();
    QString subjectName = subjectTable->item(row, 1)->text();

    // Confirmation dialog
    int result = QMessageBox::question(this, "Xác nhận xóa",
                                       QString("Bạn có chắc chắn muốn xóa môn '%1 - %2'?\n\n"
                                               "Việc này sẽ xóa tất cả câu hỏi của môn học này và kết quả thi liên quan.\n"
                                               "Hành động này không thể hoàn tác.")
                                       .arg(subjectCode)
                                       .arg(subjectName),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);

    if (result != QMessageBox::Yes)
        return;

    if (subjectManager->xoa(subjectCode.toStdString().c_str())) {
        subjectManager->saveToFile();

        // Clear current selection if it was the deleted subject
        if (currentSubjectCode == subjectCode) {
            currentSubjectCode.clear();
            questionTable->setRowCount(0);
        }

        refreshSubjectList();
        populateSubjectCombo();
        refreshQuestionList(); // Update question list if current subject was deleted
        QMessageBox::information(this, "Thành công", "Đã xóa môn học thành công!");
    } else {
        QMessageBox::warning(this, "Lỗi", "Không thể xóa môn học.");
    }
}

void TeacherDashboard::manageQuestions() {
    // Switch to question management tab
    mainTabs->setCurrentWidget(questionTab);
}

void TeacherDashboard::editQuestion() {
    if (currentSubjectCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một môn học trước.");
        return;
    }

    int currentRow = questionTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một câu hỏi để sửa.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi()) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy môn học hoặc quản lý câu hỏi chưa được khởi tạo.");
        return;
    }

    // Get the question ID from the selected row
    QTableWidgetItem *idItem = questionTable->item(currentRow, 0);
    if (!idItem)
        return;

    int questionId = idItem->text().toInt();
    CauHoi *existingQuestion = mon->getQuanLyCauHoi()->tim(questionId);
    if (!existingQuestion) {
        QMessageBox::warning(this, "Lỗi", "Câu hỏi không tồn tại.");
        return;
    }

    // Create dialog for editing question
    QDialog dialog(this);
    dialog.setWindowTitle("Sửa Câu Hỏi");
    dialog.setMinimumSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Question content
    QLabel *contentLabel = new QLabel("Nội dung câu hỏi:");
    QTextEdit *contentEdit = new QTextEdit();
    contentEdit->setText(QString::fromStdString(existingQuestion->getNoiDung()));

    // Answer options
    QLabel *optionsLabel = new QLabel("Các lựa chọn trả lời:");
    QLineEdit *optionA = new QLineEdit(QString::fromStdString(existingQuestion->getLuaChonA()));
    QLineEdit *optionB = new QLineEdit(QString::fromStdString(existingQuestion->getLuaChonB()));
    QLineEdit *optionC = new QLineEdit(QString::fromStdString(existingQuestion->getLuaChonC()));
    QLineEdit *optionD = new QLineEdit(QString::fromStdString(existingQuestion->getLuaChonD()));

    optionA->setPlaceholderText("Lựa chọn A");
    optionB->setPlaceholderText("Lựa chọn B");
    optionC->setPlaceholderText("Lựa chọn C");
    optionD->setPlaceholderText("Lựa chọn D");

    // Correct answer
    QLabel *correctLabel = new QLabel("Đáp án đúng:");
    QComboBox *correctCombo = new QComboBox();
    correctCombo->addItems({"A", "B", "C", "D"});

    // Set current correct answer
    char currentCorrect = existingQuestion->getDapAnDung();
    if (currentCorrect == 'A')
        correctCombo->setCurrentIndex(0);
    else if (currentCorrect == 'B')
        correctCombo->setCurrentIndex(1);
    else if (currentCorrect == 'C')
        correctCombo->setCurrentIndex(2);
    else if (currentCorrect == 'D')
        correctCombo->setCurrentIndex(3);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Cập nhật câu hỏi");
    QPushButton *cancelButton = new QPushButton("Hủy");

    okButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // Layout assembly
    layout->addWidget(contentLabel);
    layout->addWidget(contentEdit);
    layout->addWidget(optionsLabel);
    layout->addWidget(optionA);
    layout->addWidget(optionB);
    layout->addWidget(optionC);
    layout->addWidget(optionD);
    layout->addWidget(correctLabel);
    layout->addWidget(correctCombo);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString content = contentEdit->toPlainText().trimmed();
        QString a = optionA->text().trimmed();
        QString b = optionB->text().trimmed();
        QString c = optionC->text().trimmed();
        QString d = optionD->text().trimmed();
        char correct = correctCombo->currentText().at(0).toLatin1();

        if (content.isEmpty() || a.isEmpty() || b.isEmpty() || c.isEmpty() || d.isEmpty()) {
            QMessageBox::warning(&dialog, "Lỗi", "Vui lòng điền đầy đủ tất cả các trường.");
            return;
        }

        // Update the existing question
        existingQuestion->setNoiDung(content.toStdString());
        existingQuestion->setLuaChonA(a.toStdString());
        existingQuestion->setLuaChonB(b.toStdString());
        existingQuestion->setLuaChonC(c.toStdString());
        existingQuestion->setLuaChonD(d.toStdString());
        existingQuestion->setDapAnDung(correct);

        // Update in the BST using the sua method
        if (mon->getQuanLyCauHoi()->sua(*existingQuestion)) {
            // Save changes to file immediately
            mon->getQuanLyCauHoi()->saveToFile();
            QMessageBox::information(&dialog, "Thành công", "Đã cập nhật câu hỏi thành công!");
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Lỗi", "Không thể cập nhật câu hỏi.");
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshQuestionList();
    }
}

void TeacherDashboard::deleteQuestion() {
    if (currentSubjectCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một môn học trước.");
        return;
    }

    int currentRow = questionTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn một câu hỏi để xóa.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi()) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy môn học hoặc quản lý câu hỏi chưa được khởi tạo.");
        return;
    }

    // Get the question ID from the selected row
    QTableWidgetItem *idItem = questionTable->item(currentRow, 0);
    if (!idItem)
        return;

    int questionId = idItem->text().toInt();
    CauHoi *existingQuestion = mon->getQuanLyCauHoi()->tim(questionId);
    if (!existingQuestion) {
        QMessageBox::warning(this, "Lỗi", "Câu hỏi không tồn tại.");
        return;
    }

    // Check if question is used in any exam
    if (isQuestionUsedInExams(questionId, currentSubjectCode.toStdString())) {
        QMessageBox::warning(this, "Không thể xóa",
                             "Không thể xóa câu hỏi này vì nó đã được sử dụng trong bài thi của sinh viên.\n\n"
                             "Để đảm bảo tính nguyên vẹn của bài thi, câu hỏi đã được sử dụng không thể xóa.");
        return;
    }

    // Show confirmation dialog
    QString questionPreview = QString::fromStdString(existingQuestion->getNoiDung());
    if (questionPreview.length() > 100) {
        questionPreview = questionPreview.left(97) + "...";
    }

    int reply = QMessageBox::question(this, "Xác nhận xóa",
                                      QString("Bạn có chắc chắn muốn xóa câu hỏi này?\n\n\"%1\"").arg(
                                              questionPreview),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Delete from BST
        if (mon->getQuanLyCauHoi()->xoa(questionId)) {
            // Save changes to file immediately
            mon->getQuanLyCauHoi()->saveToFile();
            QMessageBox::information(this, "Thành công", "Đã xóa câu hỏi thành công!");
            refreshQuestionList();
        } else {
            QMessageBox::warning(this, "Lỗi", "Không thể xóa câu hỏi.");
        }
    }
}

MonHoc *TeacherDashboard::getCurrentSubject() {
    if (!subjectManager || currentSubjectCode.isEmpty()) {
        return nullptr;
    }
    return subjectManager->tim(currentSubjectCode.toStdString().c_str());
}

Lop *TeacherDashboard::getCurrentClass() {
    if (!classManager || currentClassCode.isEmpty()) {
        return nullptr;
    }
    return classManager->tim(currentClassCode.toStdString());
}

bool TeacherDashboard::isQuestionUsedInExams(int questionId, const std::string &subjectCode) {
    if (!classManager) {
        return false;
    }

    // Get all classes
    DynamicArray<Lop *> danhSachLop;
    classManager->danhSach(danhSachLop);

    // Check each class
    for (int i = 0; i < danhSachLop.size(); i++) {
        Lop *lop = danhSachLop.get(i);
        if (!lop || !lop->getQuanLySinhVien()) {
            continue;
        }

        // Get all students in this class
        DynamicArray<SinhVien *> danhSachSinhVien;
        lop->getQuanLySinhVien()->danhSach(danhSachSinhVien);

        // Check each student's exam results
        for (int j = 0; j < danhSachSinhVien.size(); j++) {
            SinhVien *sv = danhSachSinhVien.get(j);
            if (!sv || !sv->getQuanLyDiem()) {
                continue;
            }

            // Get all exam results for this student
            DynamicArray<DiemThi *> danhSachDiem;
            sv->getQuanLyDiem()->danhSach(danhSachDiem);

            // Check if this student has an exam result for the subject
            for (int k = 0; k < danhSachDiem.size(); k++) {
                DiemThi *diem = danhSachDiem.get(k);
                if (!diem) {
                    continue;
                }

                // Check if this exam result is for the same subject
                if (std::strcmp(diem->getMaMon(), subjectCode.c_str()) == 0) {
                    // Check if the question ID is in this exam
                    DynamicArray<int> *questionIds = diem->getDanhSachCauHoi();
                    if (questionIds) {
                        for (int l = 0; l < questionIds->size(); l++) {
                            if (questionIds->get(l) == questionId) {
                                return true; // Question is used in an exam
                            }
                        }
                    }
                }
            }
        }
    }

    return false; // Question is not used in any exam
}

// Report Management Functions Implementation

void TeacherDashboard::populateReportCombos() {
    if (!classManager || !subjectManager)
        return;

    // Populate class combo
    reportClassCombo->clear();
    DynamicArray<Lop *> danhSachLop;
    classManager->danhSach(danhSachLop);

    for (int i = 0; i < danhSachLop.size(); i++) {
        Lop *lop = danhSachLop.get(i);
        QString item = QString("%1 - %2").arg(QString::fromStdString(lop->getMaLop())).arg(
                QString::fromStdString(lop->getTenLop()));
        reportClassCombo->addItem(item, QString::fromStdString(lop->getMaLop()));
    }

    // Populate subject combo
    reportSubjectCombo->clear();
    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    for (int i = 0; i < danhSachMon.size(); i++) {
        MonHoc *mon = danhSachMon.get(i);
        QString item = QString("%1 - %2").arg(QString::fromStdString(mon->getMaMon())).arg(
                QString::fromStdString(mon->getTenMon()));
        reportSubjectCombo->addItem(item, QString::fromStdString(mon->getMaMon()));
    }
}

void TeacherDashboard::generateExamScoreReport() {
    QString classCode = reportClassCombo->currentData().toString();
    QString subjectCode = reportSubjectCombo->currentData().toString();

    if (classCode.isEmpty() || subjectCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn lớp và môn học.");
        return;
    }

    if (!classManager || !subjectManager) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy dữ liệu lớp hoặc môn học.");
        return;
    }

    Lop *lop = classManager->tim(classCode.toStdString());
    MonHoc *mon = subjectManager->tim(subjectCode.toStdString().c_str());

    if (!lop || !mon) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy lớp hoặc môn học được chọn.");
        return;
    }

    if (!lop->getQuanLySinhVien()) {
        QMessageBox::warning(this, "Lỗi", "Lớp không có danh sách sinh viên.");
        return;
    }

    // Setup table headers
    QStringList headers = {"MSSV", "Họ", "Tên", "Giới tính", "Điểm thi"};
    reportTable->setColumnCount(headers.size());
    reportTable->setHorizontalHeaderLabels(headers);

    // Get students from the class
    DynamicArray<SinhVien *> danhSachSV;
    lop->getQuanLySinhVien()->danhSach(danhSachSV);

    reportTable->setRowCount(danhSachSV.size());

    for (int i = 0; i < danhSachSV.size(); i++) {
        SinhVien *sv = danhSachSV.get(i);

        reportTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(sv->getMaSinhVien())));
        reportTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(sv->getHo())));
        reportTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(sv->getTen())));
        reportTable->setItem(i, 3, new QTableWidgetItem(sv->getPhai() ? "Nam" : "Nữ"));

        // Check if student has exam result for this subject
        QString examScore = "Chưa thi";
        if (sv->getQuanLyDiem()) {
            DynamicArray<DiemThi *> danhSachDiem;
            sv->getQuanLyDiem()->danhSach(danhSachDiem);

            // Find exam result for this subject
            for (int j = 0; j < danhSachDiem.size(); j++) {
                DiemThi *diem = danhSachDiem.get(j);
                if (diem && strcmp(diem->getMaMon(), subjectCode.toStdString().c_str()) == 0) {
                    examScore = QString::number(diem->getDiem(), 'f', 2);
                    break;
                }
            }
        }

        reportTable->setItem(i, 4, new QTableWidgetItem(examScore));
    }

    exportReportButton->setEnabled(true);
    viewDetailsButton->setEnabled(true);
}

void TeacherDashboard::exportReport() {
    if (reportTable->rowCount() == 0) {
        QMessageBox::warning(this, "Lỗi", "Không có dữ liệu để xuất. Vui lòng tạo bảng điểm trước.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Xuất bảng điểm",
                                                    QString("BangDiem_%1.txt").arg(
                                                            QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    "Tệp văn bản (*.txt);;Tệp CSV (*.csv)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Lỗi", "Không thể tạo file xuất.");
        return;
    }

    QTextStream out(&file);

    // Write header
    out << "=== BẢNG ĐIỂM THI TRẮC NGHIỆM ===\n";
    out << "Lớp: " << reportClassCombo->currentText() << "\n";
    out << "Môn học: " << reportSubjectCombo->currentText() << "\n";
    out << "Ngày tạo: " << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") << "\n\n";

    // Write table headers
    QStringList headers;
    for (int col = 0; col < reportTable->columnCount(); col++) {
        headers << reportTable->horizontalHeaderItem(col)->text();
    }

    if (fileName.endsWith(".csv")) {
        out << headers.join(",") << "\n";
    } else {
        out << headers.join("\t") << "\n";
        out << QString("-").repeated(80) << "\n";
    }

    // Write table data
    for (int row = 0; row < reportTable->rowCount(); row++) {
        QStringList rowData;
        for (int col = 0; col < reportTable->columnCount(); col++) {
            QTableWidgetItem *item = reportTable->item(row, col);
            rowData << (item ? item->text() : "");
        }

        if (fileName.endsWith(".csv")) {
            out << rowData.join(",") << "\n";
        } else {
            out << rowData.join("\t") << "\n";
        }
    }

    out << "\n=== KẾT THÚC BẢNG ĐIỂM ===\n";
    file.close();

    QMessageBox::information(this, "Thành công",
                             QString("Đã xuất bảng điểm thành công:\n%1").arg(fileName));
}

void TeacherDashboard::viewDetailedResults() {
    if (reportTable->rowCount() == 0) {
        QMessageBox::warning(this, "Lỗi", "Không có dữ liệu bảng điểm. Vui lòng tạo bảng điểm trước.");
        return;
    }

    int currentRow = reportTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Chọn sinh viên", "Vui lòng chọn một sinh viên để xem chi tiết kết quả thi.");
        return;
    }

    // Get student ID from selected row
    QTableWidgetItem *studentIdItem = reportTable->item(currentRow, 0);
    if (!studentIdItem) {
        QMessageBox::warning(this, "Lỗi", "Không thể lấy thông tin sinh viên.");
        return;
    }

    QString studentId = studentIdItem->text();

    // Get exam score from selected row
    QTableWidgetItem *scoreItem = reportTable->item(currentRow, 4);
    if (!scoreItem || scoreItem->text() == "Chưa thi") {
        QMessageBox::information(this, "Chưa có dữ liệu",
                                 QString("Sinh viên %1 chưa làm bài thi cho môn này.").arg(studentId));
        return;
    }

    // Get current subject code
    QString subjectCode = reportSubjectCombo->currentData().toString();
    QString classCode = reportClassCombo->currentData().toString();

    if (subjectCode.isEmpty() || classCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy thông tin môn học hoặc lớp học.");
        return;
    }

    // Find the student and their exam result
    Lop *lop = classManager->tim(classCode.toStdString());
    if (!lop || !lop->getQuanLySinhVien()) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy lớp học hoặc danh sách sinh viên.");
        return;
    }

    SinhVien *student = lop->getQuanLySinhVien()->tim(studentId.toStdString());
    if (!student || !student->getQuanLyDiem()) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy sinh viên hoặc dữ liệu điểm thi.");
        return;
    }

    DiemThi *examResult = student->getQuanLyDiem()->tim(subjectCode.toStdString().c_str());
    if (!examResult) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy kết quả thi của sinh viên.");
        return;
    }

    // Check if we have question IDs (for detailed view)
    DynamicArray<int> *questionIds = examResult->getDanhSachCauHoi();
    if (!questionIds || questionIds->size() == 0) {
        // Fallback to simple text display for old exam results
        QString studentName = QString("%1 %2").arg(reportTable->item(currentRow, 1)->text())
                .arg(reportTable->item(currentRow, 2)->text());
        QString details = QString("Sinh viên: %1 - %2\nMôn học: %3\nĐiểm: %4/10\nTrạng thái: %5\n\nCâu trả lời: ")
                .arg(studentId)
                .arg(studentName)
                .arg(reportSubjectCombo->currentText())
                .arg(scoreItem->text())
                .arg(scoreItem->text().toDouble() >= 5.0 ? "ĐẬU" : "RỚT");

        DynamicArray<char> *answers = examResult->getDanhSachCauTraLoi();
        if (answers) {
            for (int i = 0; i < answers->size(); i++) {
                details += QString("Câu %1:%2 ").arg(i + 1).arg(answers->get(i));
            }
        }

        QMessageBox::information(this, "Chi tiết kết quả", details);
        return;
    }

    // Use the new detailed results widget
    if (!subjectManager) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy dữ liệu môn học.");
        return;
    }

    DetailedResultsWidget *detailsDialog = new DetailedResultsWidget(this);
    detailsDialog->showResults(examResult, subjectManager);
    detailsDialog->exec();
    delete detailsDialog;
}
