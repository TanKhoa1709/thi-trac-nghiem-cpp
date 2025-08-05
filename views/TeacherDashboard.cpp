#include "TeacherDashboard.h"
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
#include <cstring>

TeacherDashboard::TeacherDashboard(QWidget *parent)
    : QWidget(parent), mainTabs(nullptr), classManager(nullptr), subjectManager(nullptr)
{
    setupUI();
    setupConnections();
}

TeacherDashboard::~TeacherDashboard()
{
    // Qt handles widget cleanup through parent-child relationships
}

void TeacherDashboard::setClassManager(QuanLyLop *manager)
{
    classManager = manager;
    // Don't refresh immediately - will be called manually after data loading
}

void TeacherDashboard::setSubjectManager(QuanLyMonHoc *manager)
{
    subjectManager = manager;
    // Don't refresh immediately - will be called manually after data loading
}

void TeacherDashboard::setupUI()
{
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
    setupReportsTab();

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

void TeacherDashboard::setupClassTab()
{
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

    addClassButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    editClassButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    deleteClassButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");

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

    addStudentButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    editStudentButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    deleteStudentButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");

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

void TeacherDashboard::setupSubjectTab()
{
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
    manageQuestionsButton = new QPushButton("Manage Questions");

    addSubjectButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    editSubjectButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    deleteSubjectButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    manageQuestionsButton->setStyleSheet("QPushButton { background-color: #9b59b6; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");

    buttonLayout->addWidget(addSubjectButton);
    buttonLayout->addWidget(editSubjectButton);
    buttonLayout->addWidget(deleteSubjectButton);
    buttonLayout->addWidget(manageQuestionsButton);
    buttonLayout->addStretch();

    layout->addWidget(subjectLabel);
    layout->addWidget(subjectTable);
    layout->addLayout(buttonLayout);

    mainTabs->addTab(subjectTab, "Quản Lý Môn");
}

void TeacherDashboard::setupQuestionTab()
{
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

    addQuestionButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    editQuestionButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");
    deleteQuestionButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 5px 10px; border: none; border-radius: 3px; }");

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

void TeacherDashboard::setupReportsTab()
{
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

    generateReportButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 10px 20px; border: none; border-radius: 5px; }");
    exportReportButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 10px 20px; border: none; border-radius: 5px; }");
    exportReportButton->setEnabled(false); // Enabled after report generation

    buttonLayout->addWidget(generateReportButton);
    buttonLayout->addWidget(exportReportButton);
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

void TeacherDashboard::setupConnections()
{
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
    connect(manageQuestionsButton, &QPushButton::clicked, this, &TeacherDashboard::manageQuestions);

    // Question management connections
    connect(subjectComboForQuestions, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TeacherDashboard::onSubjectChangedForQuestions);
    connect(addQuestionButton, &QPushButton::clicked, this, &TeacherDashboard::addNewQuestion);
    connect(editQuestionButton, &QPushButton::clicked, this, &TeacherDashboard::editQuestion);
    connect(deleteQuestionButton, &QPushButton::clicked, this, &TeacherDashboard::deleteQuestion);

    // Report management connections
    connect(generateReportButton, &QPushButton::clicked, this, &TeacherDashboard::generateExamScoreReport);
    connect(exportReportButton, &QPushButton::clicked, this, &TeacherDashboard::exportReport);
}

void TeacherDashboard::refreshAllData()
{
    refreshClassList();
    refreshSubjectList();
    populateSubjectCombo();
    populateReportCombos();
    refreshQuestionList();
}

void TeacherDashboard::refreshClassList()
{
    if (!classManager)
        return;

    DynamicArray<Lop *> danhSachLop;
    classManager->danhSach(danhSachLop);

    classTable->setRowCount(danhSachLop.size());
    for (int i = 0; i < danhSachLop.size(); i++)
    {
        Lop *lop = danhSachLop.get(i);
        classTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(lop->getMaLop())));
        classTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(lop->getTenLop())));

        int studentCount = lop->getQuanLySinhVien() ? lop->getQuanLySinhVien()->size() : 0;
        classTable->setItem(i, 2, new QTableWidgetItem(QString::number(studentCount)));
    }
}

void TeacherDashboard::onClassSelected()
{
    int row = classTable->currentRow();
    if (row >= 0)
    {
        QTableWidgetItem *item = classTable->item(row, 0);
        if (item)
        {
            currentClassCode = item->text();
            refreshStudentList();
        }
    }
}

void TeacherDashboard::refreshStudentList()
{
    if (!classManager || currentClassCode.isEmpty())
    {
        studentTable->setRowCount(0);
        return;
    }

    Lop *lop = classManager->tim(currentClassCode.toStdString());
    if (!lop || !lop->getQuanLySinhVien())
    {
        studentTable->setRowCount(0);
        return;
    }

    DynamicArray<SinhVien *> danhSachSV;
    lop->getQuanLySinhVien()->danhSach(danhSachSV);

    studentTable->setRowCount(danhSachSV.size());
    for (int i = 0; i < danhSachSV.size(); i++)
    {
        SinhVien *sv = danhSachSV.get(i);
        studentTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(sv->getMaSinhVien())));
        studentTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(sv->getHo())));
        studentTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(sv->getTen())));
        studentTable->setItem(i, 3, new QTableWidgetItem(sv->getPhai() ? "Male" : "Female"));
    }
}

void TeacherDashboard::addNewClass()
{
    bool ok;
    QString classCode = QInputDialog::getText(this, "Add New Class", "Class Code:", QLineEdit::Normal, "", &ok);
    if (!ok || classCode.isEmpty())
        return;

    QString className = QInputDialog::getText(this, "Add New Class", "Class Name:", QLineEdit::Normal, "", &ok);
    if (!ok || className.isEmpty())
        return;

    if (classManager)
    {
        // Create new class following memory management rules
        Lop *lopMoi = new Lop(classCode.toStdString(), className.toStdString());
        if (classManager->them(*lopMoi))
        {
            classManager->saveToFile();
            refreshClassList();
            QMessageBox::information(this, "Success", "Class added successfully!");
        }
        else
        {
            delete lopMoi; // Clean up if adding failed
            QMessageBox::warning(this, "Error", "Failed to add class. Class code may already exist.");
        }
    }
}

void TeacherDashboard::addNewStudent()
{
    if (currentClassCode.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please select a class first.");
        return;
    }

    bool ok;
    QString studentId = QInputDialog::getText(this, "Add New Student", "Student ID:", QLineEdit::Normal, "", &ok);
    if (!ok || studentId.isEmpty())
        return;

    QString lastName = QInputDialog::getText(this, "Add New Student", "Last Name:", QLineEdit::Normal, "", &ok);
    if (!ok || lastName.isEmpty())
        return;

    QString firstName = QInputDialog::getText(this, "Add New Student", "First Name:", QLineEdit::Normal, "", &ok);
    if (!ok || firstName.isEmpty())
        return;

    QStringList genders = {"Male", "Female"};
    QString gender = QInputDialog::getItem(this, "Add New Student", "Gender:", genders, 0, false, &ok);
    if (!ok)
        return;

    QString password = QInputDialog::getText(this, "Add New Student", "Password:", QLineEdit::Normal, studentId, &ok);
    if (!ok || password.isEmpty())
        return;

    Lop *lop = classManager->tim(currentClassCode.toStdString());
    if (lop && lop->getQuanLySinhVien())
    {
        // Create new student following memory management rules
        SinhVien *svMoi = new SinhVien(studentId.toStdString(), lastName.toStdString(),
                                       firstName.toStdString(), gender == "Male", password.toStdString());
        if (lop->getQuanLySinhVien()->them(*svMoi))
        {
            lop->getQuanLySinhVien()->saveToFile();
            refreshStudentList();
            refreshClassList(); // Update student count
            QMessageBox::information(this, "Success", "Student added successfully!");
        }
        else
        {
            delete svMoi; // Clean up if adding failed
            QMessageBox::warning(this, "Error", "Failed to add student. Student ID may already exist.");
        }
    }
}

void TeacherDashboard::refreshSubjectList()
{
    if (!subjectManager)
        return;

    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    subjectTable->setRowCount(danhSachMon.size());
    for (int i = 0; i < danhSachMon.size(); i++)
    {
        MonHoc *mon = danhSachMon.get(i);
        subjectTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(mon->getMaMon())));
        subjectTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(mon->getTenMon())));

        int questionCount = mon->getQuanLyCauHoi() ? mon->getQuanLyCauHoi()->size() : 0;
        subjectTable->setItem(i, 2, new QTableWidgetItem(QString::number(questionCount)));
    }
}

void TeacherDashboard::addNewSubject()
{
    bool ok;
    QString subjectCode = QInputDialog::getText(this, "Add New Subject", "Subject Code:", QLineEdit::Normal, "", &ok);
    if (!ok || subjectCode.isEmpty())
        return;

    QString subjectName = QInputDialog::getText(this, "Add New Subject", "Subject Name:", QLineEdit::Normal, "", &ok);
    if (!ok || subjectName.isEmpty())
        return;

    if (subjectManager)
    {
        // Create new subject following memory management rules
        MonHoc *monMoi = new MonHoc(subjectCode.toStdString().c_str(), subjectName.toStdString());
        if (subjectManager->them(*monMoi))
        {
            subjectManager->saveToFile();
            refreshSubjectList();
            populateSubjectCombo();
            QMessageBox::information(this, "Success", "Subject added successfully!");
        }
        else
        {
            delete monMoi; // Clean up if adding failed
            QMessageBox::warning(this, "Error", "Failed to add subject. Subject code may already exist.");
        }
    }
}

void TeacherDashboard::populateSubjectCombo()
{
    if (!subjectManager)
        return;

    subjectComboForQuestions->clear();

    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    for (int i = 0; i < danhSachMon.size(); i++)
    {
        MonHoc *mon = danhSachMon.get(i);
        QString item = QString("%1 - %2").arg(QString::fromStdString(mon->getMaMon())).arg(QString::fromStdString(mon->getTenMon()));
        subjectComboForQuestions->addItem(item, QString::fromStdString(mon->getMaMon()));
    }
}

void TeacherDashboard::onSubjectChangedForQuestions()
{
    if (subjectComboForQuestions->currentIndex() >= 0)
    {
        currentSubjectCode = subjectComboForQuestions->currentData().toString();
        refreshQuestionList();
    }
}

void TeacherDashboard::refreshQuestionList()
{
    if (!subjectManager || currentSubjectCode.isEmpty())
    {
        questionTable->setRowCount(0);
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi())
    {
        questionTable->setRowCount(0);
        return;
    }

    DynamicArray<CauHoi *> danhSachCH;
    mon->getQuanLyCauHoi()->danhSach(danhSachCH);

    questionTable->setRowCount(danhSachCH.size());
    for (int i = 0; i < danhSachCH.size(); i++)
    {
        CauHoi *ch = danhSachCH.get(i);
        questionTable->setItem(i, 0, new QTableWidgetItem(QString::number(ch->getMaCauHoi())));

        // Truncate long questions for display
        QString content = QString::fromStdString(ch->getNoiDung());
        if (content.length() > 50)
        {
            content = content.left(47) + "...";
        }
        questionTable->setItem(i, 1, new QTableWidgetItem(content));
        questionTable->setItem(i, 2, new QTableWidgetItem(QString(ch->getDapAnDung())));
    }
}

void TeacherDashboard::addNewQuestion()
{
    if (currentSubjectCode.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please select a subject first.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi())
    {
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

    okButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

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
    connect(okButton, &QPushButton::clicked, [&]()
            {
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
        CauHoi* newQuestion = new CauHoi();
        
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
        } });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        refreshQuestionList();
    }
}

void TeacherDashboard::editClass()
{
    int row = classTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a class to edit.");
        return;
    }

    QTableWidgetItem *item = classTable->item(row, 0);
    if (!item)
        return;

    QString oldClassCode = item->text();
    Lop *lop = classManager->tim(oldClassCode.toStdString());
    if (!lop)
    {
        QMessageBox::warning(this, "Error", "Selected class not found.");
        return;
    }

    bool ok;
    QString newClassCode = QInputDialog::getText(this, "Edit Class", "Class Code:",
                                                 QLineEdit::Normal, oldClassCode, &ok);
    if (!ok || newClassCode.isEmpty())
        return;

    QString newClassName = QInputDialog::getText(this, "Edit Class", "Class Name:",
                                                 QLineEdit::Normal, QString::fromStdString(lop->getTenLop()), &ok);
    if (!ok || newClassName.isEmpty())
        return;

    // Update the class object
    lop->setMaLop(newClassCode.toStdString());
    lop->setTenLop(newClassName.toStdString());

    if (classManager->sua(*lop))
    {
        classManager->saveToFile();
        refreshClassList();
        // Update current selection if it was the edited class
        if (currentClassCode == oldClassCode)
        {
            currentClassCode = newClassCode;
        }
        QMessageBox::information(this, "Success", "Class updated successfully!");
    }
    else
    {
        // Restore original values if update failed
        lop->setMaLop(oldClassCode.toStdString());
        lop->setTenLop(item ? classTable->item(row, 1)->text().toStdString() : "");
        QMessageBox::warning(this, "Error", "Failed to update class. Class code may already exist.");
    }
}

void TeacherDashboard::deleteClass()
{
    int row = classTable->currentRow();
    if (row < 0)
    {
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

    if (classManager->xoa(classCode.toStdString()))
    {
        classManager->saveToFile();

        // Clear current selection if it was the deleted class
        if (currentClassCode == classCode)
        {
            currentClassCode.clear();
            studentTable->setRowCount(0);
        }

        refreshClassList();
        QMessageBox::information(this, "Success", "Class deleted successfully!");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to delete class.");
    }
}

void TeacherDashboard::editStudent()
{
    if (currentClassCode.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please select a class first.");
        return;
    }

    int row = studentTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a student to edit.");
        return;
    }

    QTableWidgetItem *item = studentTable->item(row, 0);
    if (!item)
        return;

    QString oldStudentId = item->text();

    Lop *lop = classManager->tim(currentClassCode.toStdString());
    if (!lop || !lop->getQuanLySinhVien())
    {
        QMessageBox::warning(this, "Error", "Class or student manager not found.");
        return;
    }

    SinhVien *sv = lop->getQuanLySinhVien()->tim(oldStudentId.toStdString());
    if (!sv)
    {
        QMessageBox::warning(this, "Error", "Selected student not found.");
        return;
    }

    bool ok;
    QString newStudentId = QInputDialog::getText(this, "Edit Student", "Student ID:",
                                                 QLineEdit::Normal, oldStudentId, &ok);
    if (!ok || newStudentId.isEmpty())
        return;

    QString newLastName = QInputDialog::getText(this, "Edit Student", "Last Name:",
                                                QLineEdit::Normal, QString::fromStdString(sv->getHo()), &ok);
    if (!ok || newLastName.isEmpty())
        return;

    QString newFirstName = QInputDialog::getText(this, "Edit Student", "First Name:",
                                                 QLineEdit::Normal, QString::fromStdString(sv->getTen()), &ok);
    if (!ok || newFirstName.isEmpty())
        return;

    QStringList genders = {"Male", "Female"};
    QString currentGender = sv->getPhai() ? "Male" : "Female";
    QString newGender = QInputDialog::getItem(this, "Edit Student", "Gender:", genders,
                                              genders.indexOf(currentGender), false, &ok);
    if (!ok)
        return;

    QString newPassword = QInputDialog::getText(this, "Edit Student", "Password:",
                                                QLineEdit::Normal, QString::fromStdString(sv->getPassword()), &ok);
    if (!ok || newPassword.isEmpty())
        return;

    // Update the student object
    sv->setMaSinhVien(newStudentId.toStdString());
    sv->setHo(newLastName.toStdString());
    sv->setTen(newFirstName.toStdString());
    sv->setPhai(newGender == "Male");
    sv->setPassword(newPassword.toStdString());

    if (lop->getQuanLySinhVien()->sua(*sv))
    {
        lop->getQuanLySinhVien()->saveToFile();
        refreshStudentList();
        refreshClassList(); // Update student count if needed
        QMessageBox::information(this, "Success", "Student updated successfully!");
    }
    else
    {
        // Restore original values if update failed
        sv->setMaSinhVien(oldStudentId.toStdString());
        sv->setHo(studentTable->item(row, 1)->text().toStdString());
        sv->setTen(studentTable->item(row, 2)->text().toStdString());
        sv->setPhai(studentTable->item(row, 3)->text() == "Male");
        QMessageBox::warning(this, "Error", "Failed to update student. Student ID may already exist.");
    }
}

void TeacherDashboard::deleteStudent()
{
    if (currentClassCode.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please select a class first.");
        return;
    }

    int row = studentTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a student to delete.");
        return;
    }

    QTableWidgetItem *item = studentTable->item(row, 0);
    if (!item)
        return;

    QString studentId = item->text();
    QString studentName = QString("%1 %2").arg(studentTable->item(row, 1)->text()).arg(studentTable->item(row, 2)->text());

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
    if (!lop || !lop->getQuanLySinhVien())
    {
        QMessageBox::warning(this, "Error", "Class or student manager not found.");
        return;
    }

    if (lop->getQuanLySinhVien()->xoa(studentId.toStdString()))
    {
        lop->getQuanLySinhVien()->saveToFile();
        refreshStudentList();
        refreshClassList(); // Update student count
        QMessageBox::information(this, "Success", "Student deleted successfully!");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to delete student.");
    }
}

void TeacherDashboard::editSubject()
{
    int row = subjectTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a subject to edit.");
        return;
    }

    QTableWidgetItem *item = subjectTable->item(row, 0);
    if (!item)
        return;

    QString oldSubjectCode = item->text();
    MonHoc *mon = subjectManager->tim(oldSubjectCode.toStdString().c_str());
    if (!mon)
    {
        QMessageBox::warning(this, "Error", "Selected subject not found.");
        return;
    }

    bool ok;
    QString newSubjectCode = QInputDialog::getText(this, "Edit Subject", "Subject Code:",
                                                   QLineEdit::Normal, oldSubjectCode, &ok);
    if (!ok || newSubjectCode.isEmpty())
        return;

    QString newSubjectName = QInputDialog::getText(this, "Edit Subject", "Subject Name:",
                                                   QLineEdit::Normal, QString::fromStdString(mon->getTenMon()), &ok);
    if (!ok || newSubjectName.isEmpty())
        return;

    // Store original values for potential rollback
    char originalCode[16];
    strcpy(originalCode, mon->getMaMon());
    std::string originalName = mon->getTenMon();

    // Update the subject object
    mon->setMaMon(newSubjectCode.toStdString().c_str());
    mon->setTenMon(newSubjectName.toStdString());

    if (subjectManager->sua(*mon))
    {
        subjectManager->saveToFile();
        refreshSubjectList();
        populateSubjectCombo();

        // Update current selection if it was the edited subject
        if (currentSubjectCode == oldSubjectCode)
        {
            currentSubjectCode = newSubjectCode;
        }

        QMessageBox::information(this, "Success", "Subject updated successfully!");
    }
    else
    {
        // Restore original values if update failed
        mon->setMaMon(originalCode);
        mon->setTenMon(originalName);
        QMessageBox::warning(this, "Error", "Failed to update subject. Subject code may already exist.");
    }
}

void TeacherDashboard::deleteSubject()
{
    int row = subjectTable->currentRow();
    if (row < 0)
    {
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

    if (subjectManager->xoa(subjectCode.toStdString().c_str()))
    {
        subjectManager->saveToFile();

        // Clear current selection if it was the deleted subject
        if (currentSubjectCode == subjectCode)
        {
            currentSubjectCode.clear();
            questionTable->setRowCount(0);
        }

        refreshSubjectList();
        populateSubjectCombo();
        refreshQuestionList(); // Update question list if current subject was deleted
        QMessageBox::information(this, "Success", "Subject deleted successfully!");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to delete subject.");
    }
}

void TeacherDashboard::manageQuestions()
{
    // Switch to question management tab
    mainTabs->setCurrentWidget(questionTab);
}

void TeacherDashboard::editQuestion()
{
    if (currentSubjectCode.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please select a subject first.");
        return;
    }

    int currentRow = questionTable->currentRow();
    if (currentRow < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a question to edit.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi())
    {
        QMessageBox::warning(this, "Error", "Subject not found or question manager not initialized.");
        return;
    }

    // Get the question ID from the selected row
    QTableWidgetItem *idItem = questionTable->item(currentRow, 0);
    if (!idItem)
        return;

    int questionId = idItem->text().toInt();
    CauHoi *existingQuestion = mon->getQuanLyCauHoi()->tim(questionId);
    if (!existingQuestion)
    {
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

    okButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");
    cancelButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; padding: 8px 16px; border: none; border-radius: 4px; }");

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
    connect(okButton, &QPushButton::clicked, [&]()
            {
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
        } });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        refreshQuestionList();
    }
}

void TeacherDashboard::deleteQuestion()
{
    if (currentSubjectCode.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please select a subject first.");
        return;
    }

    int currentRow = questionTable->currentRow();
    if (currentRow < 0)
    {
        QMessageBox::warning(this, "Error", "Please select a question to delete.");
        return;
    }

    MonHoc *mon = subjectManager->tim(currentSubjectCode.toStdString().c_str());
    if (!mon || !mon->getQuanLyCauHoi())
    {
        QMessageBox::warning(this, "Error", "Subject not found or question manager not initialized.");
        return;
    }

    // Get the question ID from the selected row
    QTableWidgetItem *idItem = questionTable->item(currentRow, 0);
    if (!idItem)
        return;

    int questionId = idItem->text().toInt();
    CauHoi *existingQuestion = mon->getQuanLyCauHoi()->tim(questionId);
    if (!existingQuestion)
    {
        QMessageBox::warning(this, "Error", "Question not found.");
        return;
    }

    // Show confirmation dialog
    QString questionPreview = QString::fromStdString(existingQuestion->getNoiDung());
    if (questionPreview.length() > 100)
    {
        questionPreview = questionPreview.left(97) + "...";
    }

    int reply = QMessageBox::question(this, "Confirm Delete",
                                      QString("Are you sure you want to delete this question?\n\n\"%1\"").arg(questionPreview),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Delete from BST
        if (mon->getQuanLyCauHoi()->xoa(questionId))
        {
            // Save changes to file immediately
            mon->getQuanLyCauHoi()->saveToFile();
            QMessageBox::information(this, "Success", "Question deleted successfully!");
            refreshQuestionList();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to delete question.");
        }
    }
}

MonHoc *TeacherDashboard::getCurrentSubject()
{
    if (!subjectManager || currentSubjectCode.isEmpty())
    {
        return nullptr;
    }
    return subjectManager->tim(currentSubjectCode.toStdString().c_str());
}

Lop *TeacherDashboard::getCurrentClass()
{
    if (!classManager || currentClassCode.isEmpty())
    {
        return nullptr;
    }
    return classManager->tim(currentClassCode.toStdString());
}

// Report Management Functions Implementation

void TeacherDashboard::populateReportCombos()
{
    if (!classManager || !subjectManager)
        return;

    // Populate class combo
    reportClassCombo->clear();
    DynamicArray<Lop *> danhSachLop;
    classManager->danhSach(danhSachLop);

    for (int i = 0; i < danhSachLop.size(); i++)
    {
        Lop *lop = danhSachLop.get(i);
        QString item = QString("%1 - %2").arg(QString::fromStdString(lop->getMaLop())).arg(QString::fromStdString(lop->getTenLop()));
        reportClassCombo->addItem(item, QString::fromStdString(lop->getMaLop()));
    }

    // Populate subject combo
    reportSubjectCombo->clear();
    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    for (int i = 0; i < danhSachMon.size(); i++)
    {
        MonHoc *mon = danhSachMon.get(i);
        QString item = QString("%1 - %2").arg(QString::fromStdString(mon->getMaMon())).arg(QString::fromStdString(mon->getTenMon()));
        reportSubjectCombo->addItem(item, QString::fromStdString(mon->getMaMon()));
    }
}

void TeacherDashboard::generateExamScoreReport()
{
    QString classCode = reportClassCombo->currentData().toString();
    QString subjectCode = reportSubjectCombo->currentData().toString();

    if (classCode.isEmpty() || subjectCode.isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn lớp và môn học.");
        return;
    }

    if (!classManager || !subjectManager)
    {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy dữ liệu lớp hoặc môn học.");
        return;
    }

    Lop *lop = classManager->tim(classCode.toStdString());
    MonHoc *mon = subjectManager->tim(subjectCode.toStdString().c_str());

    if (!lop || !mon)
    {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy lớp hoặc môn học được chọn.");
        return;
    }

    if (!lop->getQuanLySinhVien())
    {
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

    for (int i = 0; i < danhSachSV.size(); i++)
    {
        SinhVien *sv = danhSachSV.get(i);

        reportTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(sv->getMaSinhVien())));
        reportTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(sv->getHo())));
        reportTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(sv->getTen())));
        reportTable->setItem(i, 3, new QTableWidgetItem(sv->getPhai() ? "Nam" : "Nữ"));

        // Check if student has exam result for this subject
        QString examScore = "Chưa thi";
        if (sv->getQuanLyDiem())
        {
            DynamicArray<DiemThi *> danhSachDiem;
            sv->getQuanLyDiem()->danhSach(danhSachDiem);

            // Find exam result for this subject
            for (int j = 0; j < danhSachDiem.size(); j++)
            {
                DiemThi *diem = danhSachDiem.get(j);
                if (diem && strcmp(diem->getMaMon(), subjectCode.toStdString().c_str()) == 0)
                {
                    examScore = QString::number(diem->getDiem(), 'f', 2);
                    break;
                }
            }
        }

        reportTable->setItem(i, 4, new QTableWidgetItem(examScore));
    }

    exportReportButton->setEnabled(true);

    QMessageBox::information(this, "Thành công",
                             QString("Đã tạo bảng điểm cho %1 sinh viên trong lớp %2 - môn %3.")
                                 .arg(danhSachSV.size())
                                 .arg(reportClassCombo->currentText())
                                 .arg(reportSubjectCombo->currentText()));
}

void TeacherDashboard::exportReport()
{
    if (reportTable->rowCount() == 0)
    {
        QMessageBox::warning(this, "Lỗi", "Không có dữ liệu để xuất. Vui lòng tạo bảng điểm trước.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Xuất bảng điểm",
                                                    QString("BangDiem_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    "Text Files (*.txt);;CSV Files (*.csv)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
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
    for (int col = 0; col < reportTable->columnCount(); col++)
    {
        headers << reportTable->horizontalHeaderItem(col)->text();
    }

    if (fileName.endsWith(".csv"))
    {
        out << headers.join(",") << "\n";
    }
    else
    {
        out << headers.join("\t") << "\n";
        out << QString("-").repeated(80) << "\n";
    }

    // Write table data
    for (int row = 0; row < reportTable->rowCount(); row++)
    {
        QStringList rowData;
        for (int col = 0; col < reportTable->columnCount(); col++)
        {
            QTableWidgetItem *item = reportTable->item(row, col);
            rowData << (item ? item->text() : "");
        }

        if (fileName.endsWith(".csv"))
        {
            out << rowData.join(",") << "\n";
        }
        else
        {
            out << rowData.join("\t") << "\n";
        }
    }

    out << "\n=== KẾT THÚC BẢNG ĐIỂM ===\n";
    file.close();

    QMessageBox::information(this, "Thành công",
                             QString("Đã xuất bảng điểm thành công:\n%1").arg(fileName));
}
