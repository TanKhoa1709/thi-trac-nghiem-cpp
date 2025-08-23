#include <QHeaderView>
#include "StudentDashboard.h"
#include "ExamWidget.h"
#include "../models/SinhVien.h"
#include "../managers/QuanLyMonHoc.h"
#include "../managers/QuanLyDiem.h"
#include "../managers/QuanLyCauHoi.h"
#include "../managers/ThongKe.h"
#include "../models/MonHoc.h"
#include "../models/DiemThi.h"
#include "../utils/DynamicArray.h"

StudentDashboard::StudentDashboard(QWidget *parent) :
    QWidget(parent), currentStudent(nullptr), subjectManager(nullptr), examDialog(nullptr) {
    setupUI();
    setupConnections();
}

StudentDashboard::~StudentDashboard() {
    // Qt handles widget cleanup through parent-child relationships
}

void StudentDashboard::setCurrentStudent(SinhVien *student) {
    currentStudent = student;
    updateStudentInfo();
    refreshScores();
}

void StudentDashboard::setSubjectManager(QuanLyMonHoc *manager) {
    subjectManager = manager;
}

void StudentDashboard::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Welcome section
    welcomeLabel = new QLabel("Student Dashboard");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; margin: 10px;");

    studentInfoLabel = new QLabel("Student information will appear here");
    studentInfoLabel->setAlignment(Qt::AlignCenter);
    studentInfoLabel->setStyleSheet("font-size: 14px; color: #34495e; margin: 5px;");

    // Exam section
    examGroup = new QGroupBox("Take Exam");
    examGroup->setStyleSheet("QGroupBox { font-weight: bold; color: #2c3e50; padding-top: 10px; }");
    QVBoxLayout *examLayout = new QVBoxLayout(examGroup);

    takeExamButton = new QPushButton("Start New Exam");
    takeExamButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; "
            "padding: 12px; font-size: 14px; border: none; border-radius: 6px; }"
            "QPushButton:hover { background-color: #2980b9; }");
    takeExamButton->setMinimumHeight(40);

    examLayout->addWidget(takeExamButton);

    // Scores section
    scoresGroup = new QGroupBox("Your Exam Results");
    scoresGroup->setStyleSheet("QGroupBox { font-weight: bold; color: #2c3e50; padding-top: 10px; }");
    QVBoxLayout *scoresLayout = new QVBoxLayout(scoresGroup);

    scoresTable = new QTableWidget(0, 4);
    scoresTable->setHorizontalHeaderLabels({"Subject", "Subject Name", "Score", "Status"});
    scoresTable->horizontalHeader()->setStretchLastSection(true);
    scoresTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    scoresTable->setAlternatingRowColors(true);
    scoresTable->setMaximumHeight(200);

    QHBoxLayout *scoresButtonLayout = new QHBoxLayout();
    viewDetailButton = new QPushButton("View Detailed Results");
    viewDetailButton->setStyleSheet("QPushButton { background-color: #9b59b6; color: white; "
            "padding: 8px 16px; border: none; border-radius: 4px; }"
            "QPushButton:hover { background-color: #8e44ad; }");

    scoresButtonLayout->addWidget(viewDetailButton);
    scoresButtonLayout->addStretch();

    scoresLayout->addWidget(scoresTable);
    scoresLayout->addLayout(scoresButtonLayout);

    // Logout section
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; "
            "padding: 8px 16px; font-size: 12px; border: none; border-radius: 4px; }"
            "QPushButton:hover { background-color: #c0392b; }");
    bottomLayout->addWidget(logoutButton);

    // Add all to main layout
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addWidget(studentInfoLabel);
    mainLayout->addWidget(examGroup);
    mainLayout->addWidget(scoresGroup);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
}

void StudentDashboard::setupConnections() {
    connect(takeExamButton, &QPushButton::clicked, this, &StudentDashboard::startExam);
    connect(viewDetailButton, &QPushButton::clicked, this, &StudentDashboard::viewDetailedScores);
    connect(logoutButton, &QPushButton::clicked, this, &StudentDashboard::logoutRequested);
}

void StudentDashboard::updateStudentInfo() {
    if (!currentStudent) {
        welcomeLabel->setText("Student Dashboard");
        studentInfoLabel->setText("No student information available");
        return;
    }

    QString fullName = QString::fromStdString(currentStudent->getHo() + " " + currentStudent->getTen());
    QString studentId = QString::fromStdString(currentStudent->getMaSinhVien());
    QString gender = currentStudent->getPhai() ? "Male" : "Female";

    welcomeLabel->setText(QString("Welcome, %1!").arg(fullName));
    studentInfoLabel->setText(QString("Student ID: %1 | Gender: %2").arg(studentId, gender));
}

void StudentDashboard::refreshScores() {
    if (!currentStudent || !currentStudent->getQuanLyDiem()) {
        scoresTable->setRowCount(0);
        return;
    }

    DynamicArray<DiemThi *> danhSachKetQua;
    currentStudent->getQuanLyDiem()->danhSach(danhSachKetQua);

    scoresTable->setRowCount(danhSachKetQua.size());

    for (int i = 0; i < danhSachKetQua.size(); i++) {
        DiemThi *ketQua = danhSachKetQua.get(i);

        // Get subject name
        QString subjectName = "Unknown Subject";
        if (subjectManager) {
            MonHoc *monHoc = subjectManager->tim(ketQua->getMaMon());
            if (monHoc) {
                subjectName = QString::fromStdString(monHoc->getTenMon());
            }
        }

        scoresTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(ketQua->getMaMon())));
        scoresTable->setItem(i, 1, new QTableWidgetItem(subjectName));
        std::string gradeText = ThongKe::formatGrade(ketQua->getDiem());
        std::string statusText = ThongKe::getPassFailStatus(ketQua->getDiem());

        scoresTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(gradeText)));
        scoresTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(statusText)));

        // Color coding for status
        QTableWidgetItem *statusItem = scoresTable->item(i, 3);
        if (ThongKe::isPassingScore(ketQua->getDiem())) {
            statusItem->setBackground(QColor(200, 255, 200)); // Light green
        } else {
            statusItem->setBackground(QColor(255, 200, 200)); // Light red
        }
    }
}

void StudentDashboard::startExam() {
    if (!subjectManager) {
        QMessageBox::warning(this, "Error", "Subject manager not available.");
        return;
    }

    // Get available subjects
    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    if (danhSachMon.size() == 0) {
        QMessageBox::information(this, "No Subjects", "No subjects available for examination.");
        return;
    }

    // Create subject selection dialog
    QStringList subjectList;
    for (int i = 0; i < danhSachMon.size(); i++) {
        MonHoc *mon = danhSachMon.get(i);
        QString item = QString("%1 - %2 (%3 questions)")
                .arg(QString::fromStdString(mon->getMaMon()))
                .arg(QString::fromStdString(mon->getTenMon()))
                .arg(mon->getQuanLyCauHoi() ? mon->getQuanLyCauHoi()->size() : 0);
        subjectList.append(item);
    }

    bool ok;
    QString selectedSubject = QInputDialog::getItem(this, "Select Subject",
                                                    "Choose a subject for examination:",
                                                    subjectList, 0, false, &ok);
    if (!ok)
        return;

    // Find selected subject
    int selectedIndex = subjectList.indexOf(selectedSubject);
    if (selectedIndex < 0)
        return;

    MonHoc *selectedMon = danhSachMon.get(selectedIndex);
    if (!selectedMon->getQuanLyCauHoi() || selectedMon->getQuanLyCauHoi()->size() == 0) {
        QMessageBox::warning(this, "No Questions", "This subject has no questions available for examination.");
        return;
    }

    // Get number of questions
    int maxQuestions = selectedMon->getQuanLyCauHoi()->size();
    int numQuestions = QInputDialog::getInt(this, "Number of Questions",
                                            QString("How many questions? (Max: %1)").arg(maxQuestions),
                                            qMin(10, maxQuestions), 1, maxQuestions, 1, &ok);
    if (!ok)
        return;

    // Start exam
    emit examRequested(selectedMon, numQuestions);
}

void StudentDashboard::viewDetailedScores() {
    int currentRow = scoresTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "No Selection", "Please select an exam result to view details.");
        return;
    }

    QTableWidgetItem *subjectItem = scoresTable->item(currentRow, 0);
    if (!subjectItem)
        return;

    QString subjectCode = subjectItem->text();

    if (!currentStudent || !currentStudent->getQuanLyDiem()) {
        QMessageBox::warning(this, "Error", "Student data not available.");
        return;
    }

    // Get subject information
    MonHoc *mon = subjectManager->tim(subjectCode.toStdString().c_str());
    if (!mon) {
        QMessageBox::warning(this, "Error", "Subject not found.");
        return;
    }

    DiemThi *diem = currentStudent->getQuanLyDiem()->tim(subjectCode.toStdString().c_str());
    if (!diem) {
        QMessageBox::warning(this, "Error", "Exam result not found.");
        return;
    }

    ExamWidget *detailsDialog = new ExamWidget(this);
    detailsDialog->showExamDetails(currentStudent, mon, diem);
    detailsDialog->exec();
    delete detailsDialog;
}

void StudentDashboard::refreshData() {
    updateStudentInfo();
    refreshScores();
}

void StudentDashboard::onExamCompleted(double score) {
    std::string statusText = ThongKe::getPassFailStatus(score);
    QString message = QString("Exam completed!\nYour score: %1\nStatus: %2")
            .arg(QString::fromStdString(ThongKe::formatGrade(score)))
            .arg(QString::fromStdString(statusText));

    QMessageBox::information(this, "Exam Results", message);

    // Refresh scores to show the new result
    refreshScores();
}


