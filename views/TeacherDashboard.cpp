#include "TeacherDashboard.h"
#include "ExamWidget.h"
#include "../managers/QuanLyLop.h"
#include "../managers/QuanLyMonHoc.h"
#include "../managers/QuanLySinhVien.h"
#include "../managers/QuanLyCauHoi.h"
#include "../managers/QuanLyDiem.h"
#include "../models/Lop.h"
#include "../models/MonHoc.h"
#include "../models/SinhVien.h"
#include "../models/CauHoi.h"
#include "../models/DiemThi.h"
#include "../utils/DynamicArray.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextEdit>
#include <cstring>
#include "../utils/InputValidator.h"
#include "../managers/ThongKe.h"

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
    QLabel *titleLabel = new QLabel("Teacher Dashboard");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50; margin: 10px;");

    // Create main tabs
    mainTabs = new QTabWidget();

    setupClassTab();
    setupSubjectTab();
    setupQuestionTab();
    setupScoreTab();

    // Logout button
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    QPushButton *logoutButton = new QPushButton("Logout");
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
    QLabel *classLabel = new QLabel("Classes");
    classLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    classTable = new QTableWidget(0, 3);
    classTable->setHorizontalHeaderLabels({"Class Code", "Class Name", "Students"});
    classTable->horizontalHeader()->setStretchLastSection(true);
    classTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *classButtonLayout = new QHBoxLayout();
    addClassButton = new QPushButton("Add Class");
    editClassButton = new QPushButton("Edit Class");
    deleteClassButton = new QPushButton("Delete Class");

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
    QLabel *studentLabel = new QLabel("Students in Selected Class");
    studentLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    studentTable = new QTableWidget(0, 4);
    studentTable->setHorizontalHeaderLabels({"Student ID", "Last Name", "First Name", "Gender"});
    studentTable->horizontalHeader()->setStretchLastSection(true);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *studentButtonLayout = new QHBoxLayout();
    addStudentButton = new QPushButton("Add Student");
    editStudentButton = new QPushButton("Edit Student");
    deleteStudentButton = new QPushButton("Delete Student");

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

    QLabel *subjectLabel = new QLabel("Subjects");
    subjectLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    subjectTable = new QTableWidget(0, 3);
    subjectTable->setHorizontalHeaderLabels({"Subject Code", "Subject Name", "Questions"});
    subjectTable->horizontalHeader()->setStretchLastSection(true);
    subjectTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addSubjectButton = new QPushButton("Add Subject");
    editSubjectButton = new QPushButton("Edit Subject");
    deleteSubjectButton = new QPushButton("Delete Subject");

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
    QLabel *selectLabel = new QLabel("Select Subject:");
    subjectComboForQuestions = new QComboBox();
    subjectSelectLayout->addWidget(selectLabel);
    subjectSelectLayout->addWidget(subjectComboForQuestions);
    subjectSelectLayout->addStretch();

    QLabel *questionLabel = new QLabel("Questions");
    questionLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    questionTable = new QTableWidget(0, 3);
    questionTable->setHorizontalHeaderLabels({"Question ID", "Question Content", "Correct Answer"});
    questionTable->horizontalHeader()->setStretchLastSection(true);
    questionTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addQuestionButton = new QPushButton("Add Question");
    editQuestionButton = new QPushButton("Edit Question");
    deleteQuestionButton = new QPushButton("Delete Question");

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

    // Score management connections
    connect(classComboForScores, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TeacherDashboard::onClassChangedForScores);
    connect(subjectComboForScores, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TeacherDashboard::onSubjectChangedForScores);
    connect(hideNotTakenCheckBox, &QCheckBox::toggled, this, &TeacherDashboard::onHideNotTakenChanged);
    connect(viewDetailsButton, &QPushButton::clicked, this, &TeacherDashboard::viewStudentExamDetails);
}

void TeacherDashboard::refreshAllData() {
    refreshClassList();
    refreshSubjectList();
    populateSubjectCombo();
    refreshQuestionList();
    populateClassComboForScores();
    populateSubjectComboForScores();
    refreshScoreList();
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
        studentTable->setItem(i, 3, new QTableWidgetItem(sv->getPhai() ? "Male" : "Female"));
    }
}

void TeacherDashboard::addNewClass() {
    // Create custom dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Class");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Class code input
    QLabel *codeLabel = new QLabel("Class Code:");
    QLineEdit *codeEdit = new QLineEdit();
    InputValidator::setupInputValidation(codeEdit, InputValidator::CODE);

    // Class name input
    QLabel *nameLabel = new QLabel("Class Name:");
    QLineEdit *nameEdit = new QLineEdit();
    InputValidator::setupInputValidation(nameEdit, InputValidator::GENERAL);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Add Class");
    QPushButton *cancelButton = new QPushButton("Cancel");

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
        QString classCode = InputValidator::filterInput(codeEdit->text(), InputValidator::CODE);
        QString className = InputValidator::filterInput(nameEdit->text(), InputValidator::GENERAL);

        if (!InputValidator::validateClassData(classCode, className)) {
            InputValidator::showValidationError(&dialog, "Class Data", "Please check class code and name format.");
            return;
        }

        if (classManager) {
            // Create new class following memory management rules
            Lop *lopMoi = new Lop(classCode.toStdString(), className.toStdString());
            if (classManager->them(*lopMoi)) {
                classManager->saveToFile();
                QMessageBox::information(&dialog, "Success", "Class added successfully!");
                dialog.accept();
            } else {
                delete lopMoi; // Clean up if adding failed
                QMessageBox::warning(&dialog, "Error", "Failed to add class. Class code may already exist.");
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
        QMessageBox::warning(this, "Error", "Please select a class first.");
        return;
    }

    // Create custom dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Student");
    dialog.setMinimumSize(450, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Student ID input
    QLabel *idLabel = new QLabel("Student ID:");
    QLineEdit *idEdit = new QLineEdit();
    InputValidator::setupInputValidation(idEdit, InputValidator::CODE);

    // Last name input
    QLabel *lastNameLabel = new QLabel("Last Name:");
    QLineEdit *lastNameEdit = new QLineEdit();
    InputValidator::setupInputValidation(lastNameEdit, InputValidator::GENERAL);

    // First name input
    QLabel *firstNameLabel = new QLabel("First Name:");
    QLineEdit *firstNameEdit = new QLineEdit();
    InputValidator::setupInputValidation(firstNameEdit, InputValidator::GENERAL);

    // Gender selection
    QLabel *genderLabel = new QLabel("Gender:");
    QComboBox *genderCombo = new QComboBox();
    genderCombo->addItems({"Male", "Female"});

    // Password input
    QLabel *passwordLabel = new QLabel("Password:");
    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Normal); // Show password for admin convenience
    InputValidator::setupInputValidation(passwordEdit, InputValidator::GENERAL);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Add Student");
    QPushButton *cancelButton = new QPushButton("Cancel");

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
        QString studentId = InputValidator::filterInput(idEdit->text(), InputValidator::CODE);
        QString lastName = InputValidator::filterInput(lastNameEdit->text(), InputValidator::GENERAL);
        QString firstName = InputValidator::filterInput(firstNameEdit->text(), InputValidator::GENERAL);
        QString gender = genderCombo->currentText();
        QString password = InputValidator::filterInput(passwordEdit->text(), InputValidator::GENERAL);

        if (!InputValidator::validateStudentData(studentId, lastName, firstName, password)) {
            InputValidator::showValidationError(&dialog, "Student Data", "Please check all fields format.");
            return;
        }

        Lop *lop = classManager->tim(currentClassCode.toStdString());
        if (lop && lop->getQuanLySinhVien()) {
            // Create new student following memory management rules
            SinhVien *svMoi = new SinhVien(studentId.toStdString(), lastName.toStdString(),
                                           firstName.toStdString(), gender == "Male", password.toStdString());
            if (lop->getQuanLySinhVien()->them(*svMoi)) {
                lop->getQuanLySinhVien()->saveToFile();
                QMessageBox::information(&dialog, "Success", "Student added successfully!");
                dialog.accept();
            } else {
                delete svMoi; // Clean up if adding failed
                QMessageBox::warning(&dialog, "Error", "Failed to add student. Student ID may already exist.");
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
    dialog.setWindowTitle("Add New Subject");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Subject code input
    QLabel *codeLabel = new QLabel("Subject Code:");
    QLineEdit *codeEdit = new QLineEdit();
    InputValidator::setupInputValidation(codeEdit, InputValidator::CODE);

    // Subject name input
    QLabel *nameLabel = new QLabel("Subject Name:");
    QLineEdit *nameEdit = new QLineEdit();
    InputValidator::setupInputValidation(nameEdit, InputValidator::GENERAL);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Add Subject");
    QPushButton *cancelButton = new QPushButton("Cancel");

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
        QString subjectCode = InputValidator::filterInput(codeEdit->text(), InputValidator::CODE);
        QString subjectName = InputValidator::filterInput(nameEdit->text(), InputValidator::GENERAL);

        if (!InputValidator::validateSubjectData(subjectCode, subjectName)) {
            InputValidator::showValidationError(&dialog, "Subject Data",
                                                "Please check subject code and name format.");
            return;
        }

        if (subjectManager) {
            // Create new subject following memory management rules
            MonHoc *monMoi = new MonHoc(subjectCode.toStdString().c_str(), subjectName.toStdString(),
                                        subjectManager->size());
            if (subjectManager->them(*monMoi)) {
                subjectManager->saveToFile();
                QMessageBox::information(&dialog, "Success", "Subject added successfully!");
                dialog.accept();
            } else {
                delete monMoi; // Clean up if adding failed
                QMessageBox::warning(&dialog, "Error", "Failed to add subject. Subject code may already exist.");
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
        QMessageBox::warning(this, "Error", "Please select a subject first.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi()) {
        QMessageBox::warning(this, "Error", "Subject not found or question manager not initialized.");
        return;
    }

    // Create dialog for adding new question
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Question");
    dialog.setMinimumSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Question content
    QLabel *contentLabel = new QLabel("Question Content:");
    QTextEdit *contentEdit = new QTextEdit();
    contentEdit->setPlaceholderText("Enter the question content here...");

    // Answer options
    QLabel *optionsLabel = new QLabel("Answer Options:");
    QLineEdit *optionA = new QLineEdit();
    QLineEdit *optionB = new QLineEdit();
    QLineEdit *optionC = new QLineEdit();
    QLineEdit *optionD = new QLineEdit();

    optionA->setPlaceholderText("Option A");
    optionB->setPlaceholderText("Option B");
    optionC->setPlaceholderText("Option C");
    optionD->setPlaceholderText("Option D");

    // Correct answer
    QLabel *correctLabel = new QLabel("Correct Answer:");
    QComboBox *correctCombo = new QComboBox();
    correctCombo->addItems({"A", "B", "C", "D"});

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Add Question");
    QPushButton *cancelButton = new QPushButton("Cancel");

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
            QMessageBox::warning(&dialog, "Error", "Please fill in all fields.");
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
            QMessageBox::information(&dialog, "Success", "Question added successfully!");
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Error", "Failed to add question. Please check the input.");
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
        QMessageBox::warning(this, "Error", "Please select a class to edit.");
        return;
    }

    QTableWidgetItem *item = classTable->item(row, 0);
    if (!item)
        return;

    QString oldClassCode = item->text();
    Lop *lop = classManager->tim(oldClassCode.toStdString());
    if (!lop) {
        QMessageBox::warning(this, "Error", "Selected class not found.");
        return;
    }

    // Create dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Class");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Class code input
    QLabel *codeLabel = new QLabel("Class Code:");
    QLineEdit *codeEdit = new QLineEdit();
    codeEdit->setText(oldClassCode);
    InputValidator::setupInputValidation(codeEdit, InputValidator::CODE);

    // Class name input
    QLabel *nameLabel = new QLabel("Class Name:");
    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setText(QString::fromStdString(lop->getTenLop()));
    InputValidator::setupInputValidation(nameEdit, InputValidator::GENERAL);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Update Class");
    QPushButton *cancelButton = new QPushButton("Cancel");

    okButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // Layout assembly
    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString classCode = InputValidator::filterInput(codeEdit->text(), InputValidator::CODE);
        QString className = InputValidator::filterInput(nameEdit->text(), InputValidator::GENERAL);

        if (!InputValidator::validateClassData(classCode, className)) {
            InputValidator::showValidationError(&dialog, "Class Data", "Please check all fields format.");
            return;
        }

        // Store original values for potential rollback
        std::string originalCode = lop->getMaLop();
        std::string originalName = lop->getTenLop();

        // Update the class object
        lop->setMaLop(classCode.toStdString());
        lop->setTenLop(className.toStdString());

        if (classManager->sua(*lop)) {
            classManager->saveToFile();

            // Update current selection if it was the edited class
            if (currentClassCode == oldClassCode) {
                currentClassCode = classCode;
            }

            QMessageBox::information(&dialog, "Success", "Class updated successfully!");
            dialog.accept();
        } else {
            // Restore original values if update failed
            lop->setMaLop(originalCode);
            lop->setTenLop(originalName);
            QMessageBox::warning(&dialog, "Error", "Failed to update class. Class code may already exist.");
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
        QMessageBox::warning(this, "Error", "Please select a class to delete.");
        return;
    }

    QTableWidgetItem *item = classTable->item(row, 0);
    if (!item)
        return;

    QString classCode = item->text();
    QString className = classTable->item(row, 1)->text();

    // Confirmation dialog
    int result = QMessageBox::question(this, "Confirm Delete",
                                       QString("Are you sure you want to delete class '%1 - %2'?\n\n"
                                               "This will also delete all students in this class and their exam results.\n"
                                               "This action cannot be undone.")
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
        QMessageBox::information(this, "Success", "Class deleted successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete class.");
    }
}

void TeacherDashboard::editStudent() {
    if (currentClassCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a class first.");
        return;
    }

    int row = studentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a student to edit.");
        return;
    }

    QTableWidgetItem *item = studentTable->item(row, 0);
    if (!item)
        return;

    QString oldStudentId = item->text();

    Lop *lop = classManager->tim(currentClassCode.toStdString());
    if (!lop || !lop->getQuanLySinhVien()) {
        QMessageBox::warning(this, "Error", "Class or student manager not found.");
        return;
    }

    SinhVien *sv = lop->getQuanLySinhVien()->tim(oldStudentId.toStdString());
    if (!sv) {
        QMessageBox::warning(this, "Error", "Selected student not found.");
        return;
    }

    // Create dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Student");
    dialog.setMinimumSize(450, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Student ID input
    QLabel *idLabel = new QLabel("Student ID:");
    QLineEdit *idEdit = new QLineEdit();
    idEdit->setText(oldStudentId);
    InputValidator::setupInputValidation(idEdit, InputValidator::CODE);

    // Last name input
    QLabel *lastNameLabel = new QLabel("Last Name:");
    QLineEdit *lastNameEdit = new QLineEdit();
    lastNameEdit->setText(QString::fromStdString(sv->getHo()));
    InputValidator::setupInputValidation(lastNameEdit, InputValidator::GENERAL);

    // First name input
    QLabel *firstNameLabel = new QLabel("First Name:");
    QLineEdit *firstNameEdit = new QLineEdit();
    firstNameEdit->setText(QString::fromStdString(sv->getTen()));
    InputValidator::setupInputValidation(firstNameEdit, InputValidator::GENERAL);

    // Gender selection
    QLabel *genderLabel = new QLabel("Gender:");
    QComboBox *genderCombo = new QComboBox();
    genderCombo->addItems({"Male", "Female"});
    genderCombo->setCurrentText(sv->getPhai() ? "Male" : "Female");

    // Password input
    QLabel *passwordLabel = new QLabel("Password:");
    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setText(QString::fromStdString(sv->getPassword()));
    passwordEdit->setEchoMode(QLineEdit::Normal); // Show password for admin convenience
    InputValidator::setupInputValidation(passwordEdit, InputValidator::GENERAL);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Update Student");
    QPushButton *cancelButton = new QPushButton("Cancel");

    okButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // Layout assembly
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
        QString studentId = InputValidator::filterInput(idEdit->text(), InputValidator::CODE);
        QString lastName = InputValidator::filterInput(lastNameEdit->text(), InputValidator::GENERAL);
        QString firstName = InputValidator::filterInput(firstNameEdit->text(), InputValidator::GENERAL);
        QString gender = genderCombo->currentText();
        QString password = InputValidator::filterInput(passwordEdit->text(), InputValidator::GENERAL);

        if (!InputValidator::validateStudentData(studentId, lastName, firstName, password)) {
            InputValidator::showValidationError(&dialog, "Student Data", "Please check all fields format.");
            return;
        }

        // Store original values for potential rollback
        std::string originalId = sv->getMaSinhVien();
        std::string originalLastName = sv->getHo();
        std::string originalFirstName = sv->getTen();
        bool originalGender = sv->getPhai();
        std::string originalPassword = sv->getPassword();

        // Update the student object
        sv->setMaSinhVien(studentId.toStdString());
        sv->setHo(lastName.toStdString());
        sv->setTen(firstName.toStdString());
        sv->setPhai(gender == "Male");
        sv->setPassword(password.toStdString());

        if (lop->getQuanLySinhVien()->sua(*sv)) {
            lop->getQuanLySinhVien()->saveToFile();
            QMessageBox::information(&dialog, "Success", "Student updated successfully!");
            dialog.accept();
        } else {
            // Restore original values if update failed
            sv->setMaSinhVien(originalId);
            sv->setHo(originalLastName);
            sv->setTen(originalFirstName);
            sv->setPhai(originalGender);
            sv->setPassword(originalPassword);
            QMessageBox::warning(&dialog, "Error", "Failed to update student. Student ID may already exist.");
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
        QMessageBox::warning(this, "Error", "Please select a class first.");
        return;
    }

    int row = studentTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a student to delete.");
        return;
    }

    QTableWidgetItem *item = studentTable->item(row, 0);
    if (!item)
        return;

    QString studentId = item->text();
    QString studentName = QString("%1 %2").arg(studentTable->item(row, 1)->text()).arg(
            studentTable->item(row, 2)->text());

    // Confirmation dialog
    int result = QMessageBox::question(this, "Confirm Delete",
                                       QString("Are you sure you want to delete student '%1 - %2'?\n\n"
                                               "This will also delete all exam results for this student.\n"
                                               "This action cannot be undone.")
                                       .arg(studentId)
                                       .arg(studentName),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);

    if (result != QMessageBox::Yes)
        return;

    Lop *lop = classManager->tim(currentClassCode.toStdString());
    if (!lop || !lop->getQuanLySinhVien()) {
        QMessageBox::warning(this, "Error", "Class or student manager not found.");
        return;
    }

    if (lop->getQuanLySinhVien()->xoa(studentId.toStdString())) {
        lop->getQuanLySinhVien()->saveToFile();
        refreshStudentList();
        refreshClassList(); // Update student count
        QMessageBox::information(this, "Success", "Student deleted successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete student.");
    }
}

void TeacherDashboard::editSubject() {
    int row = subjectTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a subject to edit.");
        return;
    }

    QTableWidgetItem *item = subjectTable->item(row, 0);
    if (!item)
        return;

    QString oldSubjectCode = item->text();
    MonHoc *mon = subjectManager->tim(oldSubjectCode.toStdString().c_str());
    if (!mon) {
        QMessageBox::warning(this, "Error", "Selected subject not found.");
        return;
    }

    // Create dialog with validation
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Subject");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Subject code input
    QLabel *codeLabel = new QLabel("Subject Code:");
    QLineEdit *codeEdit = new QLineEdit();
    codeEdit->setText(oldSubjectCode);
    InputValidator::setupInputValidation(codeEdit, InputValidator::CODE);

    // Subject name input
    QLabel *nameLabel = new QLabel("Subject Name:");
    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setText(QString::fromStdString(mon->getTenMon()));
    InputValidator::setupInputValidation(nameEdit, InputValidator::GENERAL);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Update Subject");
    QPushButton *cancelButton = new QPushButton("Cancel");

    okButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet(
            "QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // Layout assembly
    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [&]() {
        QString subjectCode = InputValidator::filterInput(codeEdit->text(), InputValidator::CODE);
        QString subjectName = InputValidator::filterInput(nameEdit->text(), InputValidator::GENERAL);

        if (!InputValidator::validateSubjectData(subjectCode, subjectName)) {
            InputValidator::showValidationError(&dialog, "Subject Data", "Please check all fields format.");
            return;
        }

        // Store original values for potential rollback
        char originalCode[16];
        strcpy(originalCode, mon->getMaMon());
        std::string originalName = mon->getTenMon();

        // Update the subject object
        mon->setMaMon(subjectCode.toStdString().c_str());
        mon->setTenMon(subjectName.toStdString());

        if (subjectManager->sua(*mon)) {
            subjectManager->saveToFile();
            populateSubjectCombo();

            // Update current selection if it was the edited subject
            if (currentSubjectCode == oldSubjectCode) {
                currentSubjectCode = subjectCode;
            }

            QMessageBox::information(&dialog, "Success", "Subject updated successfully!");
            dialog.accept();
        } else {
            // Restore original values if update failed
            mon->setMaMon(originalCode);
            mon->setTenMon(originalName);
            QMessageBox::warning(&dialog, "Error", "Failed to update subject. Subject code may already exist.");
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshSubjectList();
    }
}

void TeacherDashboard::deleteSubject() {
    int row = subjectTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a subject to delete.");
        return;
    }

    QTableWidgetItem *item = subjectTable->item(row, 0);
    if (!item)
        return;

    QString subjectCode = item->text();
    QString subjectName = subjectTable->item(row, 1)->text();

    // Confirmation dialog
    int result = QMessageBox::question(this, "Confirm Delete",
                                       QString("Are you sure you want to delete subject '%1 - %2'?\n\n"
                                               "This will also delete all questions for this subject and related exam results.\n"
                                               "This action cannot be undone.")
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
        QMessageBox::information(this, "Success", "Subject deleted successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete subject.");
    }
}

void TeacherDashboard::manageQuestions() {
    // Switch to question management tab
    mainTabs->setCurrentWidget(questionTab);
}

void TeacherDashboard::editQuestion() {
    if (currentSubjectCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a subject first.");
        return;
    }

    int currentRow = questionTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Error", "Please select a question to edit.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi()) {
        QMessageBox::warning(this, "Error", "Subject not found or question manager not initialized.");
        return;
    }

    // Get the question ID from the selected row
    QTableWidgetItem *idItem = questionTable->item(currentRow, 0);
    if (!idItem)
        return;

    int questionId = idItem->text().toInt();
    CauHoi *existingQuestion = mon->getQuanLyCauHoi()->tim(questionId);
    if (!existingQuestion) {
        QMessageBox::warning(this, "Error", "Question not found.");
        return;
    }

    // Create dialog for editing question
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Question");
    dialog.setMinimumSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Question content
    QLabel *contentLabel = new QLabel("Question Content:");
    QTextEdit *contentEdit = new QTextEdit();
    contentEdit->setText(QString::fromStdString(existingQuestion->getNoiDung()));

    // Answer options
    QLabel *optionsLabel = new QLabel("Answer Options:");
    QLineEdit *optionA = new QLineEdit(QString::fromStdString(existingQuestion->getLuaChonA()));
    QLineEdit *optionB = new QLineEdit(QString::fromStdString(existingQuestion->getLuaChonB()));
    QLineEdit *optionC = new QLineEdit(QString::fromStdString(existingQuestion->getLuaChonC()));
    QLineEdit *optionD = new QLineEdit(QString::fromStdString(existingQuestion->getLuaChonD()));

    optionA->setPlaceholderText("Option A");
    optionB->setPlaceholderText("Option B");
    optionC->setPlaceholderText("Option C");
    optionD->setPlaceholderText("Option D");

    // Correct answer
    QLabel *correctLabel = new QLabel("Correct Answer:");
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
    QPushButton *okButton = new QPushButton("Update Question");
    QPushButton *cancelButton = new QPushButton("Cancel");

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
            QMessageBox::warning(&dialog, "Error", "Please fill in all fields.");
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
            QMessageBox::information(&dialog, "Success", "Question updated successfully!");
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Error", "Failed to update question.");
        }
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        refreshQuestionList();
    }
}

void TeacherDashboard::deleteQuestion() {
    if (currentSubjectCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a subject first.");
        return;
    }

    int currentRow = questionTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Error", "Please select a question to delete.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi()) {
        QMessageBox::warning(this, "Error", "Subject not found or question manager not initialized.");
        return;
    }

    // Get the question ID from the selected row
    QTableWidgetItem *idItem = questionTable->item(currentRow, 0);
    if (!idItem)
        return;

    int questionId = idItem->text().toInt();
    CauHoi *existingQuestion = mon->getQuanLyCauHoi()->tim(questionId);
    if (!existingQuestion) {
        QMessageBox::warning(this, "Error", "Question not found.");
        return;
    }

    // Show confirmation dialog
    QString questionPreview = QString::fromStdString(existingQuestion->getNoiDung());
    if (questionPreview.length() > 100) {
        questionPreview = questionPreview.left(97) + "...";
    }

    int reply = QMessageBox::question(this, "Confirm Delete",
                                      QString("Are you sure you want to delete this question?\n\n\"%1\"").arg(
                                              questionPreview),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Delete from BST
        if (mon->getQuanLyCauHoi()->xoa(questionId)) {
            // Save changes to file immediately
            mon->getQuanLyCauHoi()->saveToFile();
            QMessageBox::information(this, "Success", "Question deleted successfully!");
            refreshQuestionList();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete question.");
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

void TeacherDashboard::setupScoreTab() {
    scoreTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(scoreTab);

    // Selection controls
    QHBoxLayout *selectionLayout = new QHBoxLayout();

    // Class selection
    QLabel *classLabel = new QLabel("Lớp học:");
    classComboForScores = new QComboBox();
    selectionLayout->addWidget(classLabel);
    selectionLayout->addWidget(classComboForScores);

    // Subject selection
    QLabel *subjectLabel = new QLabel("Môn học:");
    subjectComboForScores = new QComboBox();
    selectionLayout->addWidget(subjectLabel);
    selectionLayout->addWidget(subjectComboForScores);

    // Hide not taken checkbox
    hideNotTakenCheckBox = new QCheckBox("Chỉ hiện đã thi");
    selectionLayout->addWidget(hideNotTakenCheckBox);

    selectionLayout->addStretch();

    // Score table
    QLabel *scoreLabel = new QLabel("Exam Results");
    scoreLabel->setStyleSheet("font-weight: bold; margin: 5px;");

    scoreTable = new QTableWidget(0, 5);
    scoreTable->setHorizontalHeaderLabels({"Student ID", "Last Name", "First Name", "Score", "Status"});
    scoreTable->horizontalHeader()->setStretchLastSection(true);
    scoreTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // View details button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    viewDetailsButton = new QPushButton("View Exam Details");
    viewDetailsButton->setStyleSheet(
            "QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    buttonLayout->addWidget(viewDetailsButton);
    buttonLayout->addStretch();

    layout->addLayout(selectionLayout);
    layout->addWidget(scoreLabel);
    layout->addWidget(scoreTable);
    layout->addLayout(buttonLayout);

    mainTabs->addTab(scoreTab, "Bảng Điểm");
}

void TeacherDashboard::populateClassComboForScores() {
    if (!classManager)
        return;

    classComboForScores->clear();
    classComboForScores->addItem("Select a class", "");

    DynamicArray<Lop *> danhSachLop;
    classManager->danhSach(danhSachLop);

    for (int i = 0; i < danhSachLop.size(); i++) {
        Lop *lop = danhSachLop.get(i);
        QString item = QString("%1 - %2").arg(QString::fromStdString(lop->getMaLop())).arg(
                QString::fromStdString(lop->getTenLop()));
        classComboForScores->addItem(item, QString::fromStdString(lop->getMaLop()));
    }
}

void TeacherDashboard::populateSubjectComboForScores() {
    if (!subjectManager)
        return;

    subjectComboForScores->clear();
    subjectComboForScores->addItem("Select a subject", "");

    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    for (int i = 0; i < danhSachMon.size(); i++) {
        MonHoc *mon = danhSachMon.get(i);
        QString item = QString("%1 - %2").arg(QString::fromStdString(mon->getMaMon())).arg(
                QString::fromStdString(mon->getTenMon()));
        subjectComboForScores->addItem(item, QString::fromStdString(mon->getMaMon()));
    }
}

void TeacherDashboard::onClassChangedForScores() {
    if (classComboForScores->currentIndex() > 0) {
        currentClassCodeForScores = classComboForScores->currentData().toString();
        refreshScoreList();
    } else {
        currentClassCodeForScores.clear();
        scoreTable->setRowCount(0);
    }
}

void TeacherDashboard::onSubjectChangedForScores() {
    if (subjectComboForScores->currentIndex() > 0) {
        currentSubjectCodeForScores = subjectComboForScores->currentData().toString();
        refreshScoreList();
    } else {
        currentSubjectCodeForScores.clear();
        scoreTable->setRowCount(0);
    }
}

void TeacherDashboard::onHideNotTakenChanged() {
    refreshScoreList();
}

void TeacherDashboard::refreshScoreList() {
    scoreTable->setRowCount(0);
    if (!classManager || !subjectManager ||
        currentClassCodeForScores.isEmpty() || currentSubjectCodeForScores.isEmpty()) {
        return;
    }

    Lop *lop = classManager->tim(currentClassCodeForScores.toStdString());
    if (!lop || !lop->getQuanLySinhVien()) {
        return;
    }


    DynamicArray<SinhVien *> danhSachSV;
    lop->getQuanLySinhVien()->danhSach(danhSachSV);

    int row = 0;
    for (int i = 0; i < danhSachSV.size(); i++) {
        SinhVien *sv = danhSachSV.get(i);
        DiemThi *diem = nullptr;

        if (sv->getQuanLyDiem()) {
            diem = sv->getQuanLyDiem()->tim(currentSubjectCodeForScores.toStdString().c_str());
        }

        // Skip if hiding not taken and student hasn't taken the exam
        if (hideNotTakenCheckBox->isChecked() && !diem) {
            continue;
        }

        scoreTable->insertRow(row);

        // Student ID
        scoreTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(sv->getMaSinhVien())));

        // Last Name
        scoreTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(sv->getHo())));

        // First Name
        scoreTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(sv->getTen())));

        // Score
        if (diem) {
            QString scoreText = QString::number(diem->getDiem(), 'f', 2);
            scoreTable->setItem(row, 3, new QTableWidgetItem(scoreText));

            // Status
            QString statusText = diem->getDiem() >= 5.0 ? "Pass" : "Fail";
            QTableWidgetItem *statusItem = new QTableWidgetItem(statusText);

            // Color coding
            if (diem->getDiem() >= 5.0) {
                statusItem->setBackground(QColor(200, 255, 200)); // Light green
            } else {
                statusItem->setBackground(QColor(255, 200, 200)); // Light red
            }
            scoreTable->setItem(row, 4, statusItem);
        } else {
            scoreTable->setItem(row, 3, new QTableWidgetItem("Chưa thi"));
            scoreTable->setItem(row, 4, new QTableWidgetItem("Not taken"));
        }
        row++;
    }
}

void TeacherDashboard::viewStudentExamDetails() {
    int currentRow = scoreTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Error", "Please select a student to view exam details.");
        return;
    }

    QTableWidgetItem *studentIdItem = scoreTable->item(currentRow, 0);
    if (!studentIdItem) {
        QMessageBox::warning(this, "Error", "Student ID not found.");
        return;
    }

    QString studentId = studentIdItem->text();
    QTableWidgetItem *scoreItem = scoreTable->item(currentRow, 3);

    if (!scoreItem || scoreItem->text() == "Chưa thi") {
        QMessageBox::information(this, "No Exam Taken",
                                 QString("Student %1 has not taken the exam for this subject yet.").arg(studentId));
        return;
    }

    // Get student and exam data
    Lop *lop = classManager->tim(currentClassCodeForScores.toStdString());
    if (!lop || !lop->getQuanLySinhVien()) {
        QMessageBox::warning(this, "Error", "Class or student manager not found.");
        return;
    }

    SinhVien *sv = lop->getQuanLySinhVien()->tim(studentId.toStdString());
    if (!sv || !sv->getQuanLyDiem()) {
        QMessageBox::warning(this, "Error", "Student or score manager not found.");
        return;
    }

    DiemThi *diem = sv->getQuanLyDiem()->tim(currentSubjectCodeForScores.toStdString().c_str());
    if (!diem) {
        QMessageBox::warning(this, "Error", "Exam result not found.");
        return;
    }

    // Get subject information
    MonHoc *mon = subjectManager->tim(currentSubjectCodeForScores.toStdString().c_str());
    if (!mon) {
        QMessageBox::warning(this, "Error", "Subject not found.");
        return;
    }

    // Create detailed results dialog using ExamWidget
    ExamWidget *detailsDialog = new ExamWidget(this);
    detailsDialog->showExamDetails(sv, mon, diem);
    detailsDialog->exec();
    delete detailsDialog;
}
