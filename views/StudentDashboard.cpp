#include <QHeaderView>
#include "StudentDashboard.h"
#include "DetailedResultsWidget.h"
#include "../models/sinhvien.h"
#include "../managers/quanlymonhoc.h"
#include "../managers/quanlydiem.h"
#include "../managers/quanlycauhoi.h"
#include "../models/monhoc.h"
#include "../models/diemthi.h"
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
    welcomeLabel = new QLabel("Bảng điều khiển sinh viên");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c3e50; margin: 10px;");

    studentInfoLabel = new QLabel("Thông tin sinh viên sẽ hiển thị ở đây");
    studentInfoLabel->setAlignment(Qt::AlignCenter);
    studentInfoLabel->setStyleSheet("font-size: 14px; color: #34495e; margin: 5px;");

    // Exam section
    examGroup = new QGroupBox("Làm bài thi");
    examGroup->setStyleSheet("QGroupBox { font-weight: bold; color: #2c3e50; padding-top: 10px; }");
    QVBoxLayout *examLayout = new QVBoxLayout(examGroup);

    takeExamButton = new QPushButton("Bắt đầu bài thi mới");
    takeExamButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; "
            "padding: 12px; font-size: 14px; border: none; border-radius: 6px; }"
            "QPushButton:hover { background-color: #2980b9; }");
    takeExamButton->setMinimumHeight(40);

    examLayout->addWidget(takeExamButton);

    // Scores section
    scoresGroup = new QGroupBox("Kết quả bài thi của bạn");
    scoresGroup->setStyleSheet("QGroupBox { font-weight: bold; color: #2c3e50; padding-top: 10px; }");
    QVBoxLayout *scoresLayout = new QVBoxLayout(scoresGroup);

    scoresTable = new QTableWidget(0, 4);
    scoresTable->setHorizontalHeaderLabels({"Môn học", "Tên môn", "Điểm", "Trạng thái"});
    scoresTable->horizontalHeader()->setStretchLastSection(true);
    scoresTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    scoresTable->setAlternatingRowColors(true);
    scoresTable->setMaximumHeight(200);

    QHBoxLayout *scoresButtonLayout = new QHBoxLayout();
    viewDetailButton = new QPushButton("Chi tiết bài thi");
    viewDetailButton->setStyleSheet("QPushButton { background-color: #9b59b6; color: white; "
            "padding: 8px 16px; border: none; border-radius: 4px; }"
            "QPushButton:hover { background-color: #8e44ad; }");


    scoresButtonLayout->addWidget(viewDetailButton);
    scoresButtonLayout->addStretch();

    statisticsLabel = new QLabel("Thống kê sẽ hiển thị ở đây");
    statisticsLabel->setStyleSheet("font-size: 12px; color: #7f8c8d; margin: 10px;");

    scoresLayout->addWidget(scoresTable);
    scoresLayout->addLayout(scoresButtonLayout);
    scoresLayout->addWidget(statisticsLabel);

    // Logout section
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    logoutButton = new QPushButton("Đăng xuất");
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
        welcomeLabel->setText("Bảng điều khiển sinh viên");
        studentInfoLabel->setText("Không có thông tin sinh viên");
        return;
    }

    QString fullName = QString::fromStdString(currentStudent->getHo() + " " + currentStudent->getTen());
    QString studentId = QString::fromStdString(currentStudent->getMaSinhVien());
    QString gender = currentStudent->getPhai() ? "Nam" : "Nữ";

    welcomeLabel->setText(fullName + " " + studentId);
    studentInfoLabel->setText(QString("Mã sinh viên: %1 | Giới tính: %2").arg(studentId).arg(gender));
}

void StudentDashboard::refreshScores() {
    if (!currentStudent || !currentStudent->getQuanLyDiem()) {
        scoresTable->setRowCount(0);
        statisticsLabel->setText("No exam results available");
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
        scoresTable->setItem(i, 2, new QTableWidgetItem(formatGrade(ketQua->getDiem())));
        scoresTable->setItem(i, 3, new QTableWidgetItem(getPassFailStatus(ketQua->getDiem())));

        // Color coding for status
        QTableWidgetItem *statusItem = scoresTable->item(i, 3);
        if (ketQua->getDiem() >= 5.0) {
            statusItem->setBackground(QColor(200, 255, 200)); // Light green
        } else {
            statusItem->setBackground(QColor(255, 200, 200)); // Light red
        }
    }

    updateStatistics();
}

void StudentDashboard::updateStatistics() {
    if (!currentStudent || !currentStudent->getQuanLyDiem()) {
        statisticsLabel->setText("Không có thống kê");
        return;
    }

    int totalExams = currentStudent->getQuanLyDiem()->demSoMonDaThi();
    int passed = currentStudent->getQuanLyDiem()->demSoMonDau();
    int failed = currentStudent->getQuanLyDiem()->demSoMonRot();
    double average = currentStudent->getQuanLyDiem()->tinhDiemTrungBinh();

    QString stats = QString("Tổng bài thi: %1 | Đậu: %2 | Rớt: %3 | Trung bình: %4/10")
            .arg(totalExams)
            .arg(passed)
            .arg(failed)
            .arg(QString::number(average, 'f', 2));

    statisticsLabel->setText(stats);
}

void StudentDashboard::startExam() {
    if (!subjectManager) {
        QMessageBox::warning(this, "Lỗi", "Quản lý môn học không khả dụng.");
        return;
    }

    // Get available subjects
    DynamicArray<MonHoc *> danhSachMon;
    subjectManager->danhSach(danhSachMon);

    if (danhSachMon.size() == 0) {
        QMessageBox::information(this, "Không có môn học", "Không có môn học nào để thi.");
        return;
    }

    // Create subject selection dialog
    QStringList subjectList;
    for (int i = 0; i < danhSachMon.size(); i++) {
        MonHoc *mon = danhSachMon.get(i);
        QString item = QString("%1 - %2 (%3 câu hỏi)")
                .arg(QString::fromStdString(mon->getMaMon()))
                .arg(QString::fromStdString(mon->getTenMon()))
                .arg(mon->getQuanLyCauHoi() ? mon->getQuanLyCauHoi()->size() : 0);
        subjectList.append(item);
    }

    bool ok;
    QString selectedSubject = QInputDialog::getItem(this, "Chọn Môn",
                                                    "Chọn một môn để thi:",
                                                    subjectList, 0, false, &ok);
    if (!ok)
        return;

    // Find selected subject
    int selectedIndex = subjectList.indexOf(selectedSubject);
    if (selectedIndex < 0)
        return;

    MonHoc *selectedMon = danhSachMon.get(selectedIndex);
    if (!selectedMon->getQuanLyCauHoi() || selectedMon->getQuanLyCauHoi()->size() == 0) {
        QMessageBox::warning(this, "Không có câu hỏi", "Môn học này không có câu hỏi để thi.");
        return;
    }

    // Get number of questions
    int maxQuestions = selectedMon->getQuanLyCauHoi()->size();
    int numQuestions = QInputDialog::getInt(this, "Số câu hỏi",
                                            QString("Bao nhiêu câu hỏi? (Tối đa: %1)").arg(maxQuestions),
                                            qMin(10, maxQuestions), 1, maxQuestions, 1, &ok);
    if (!ok)
        return;

    // Start exam
    emit examRequested(selectedMon, numQuestions);
}

void StudentDashboard::viewDetailedScores() {
    int currentRow = scoresTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Không có lựa chọn", "Vui lòng chọn một kết quả bài thi để xem chi tiết.");
        return;
    }

    QTableWidgetItem *subjectItem = scoresTable->item(currentRow, 0);
    if (!subjectItem)
        return;

    QString subjectCode = subjectItem->text();

    if (!currentStudent || !currentStudent->getQuanLyDiem()) {
        QMessageBox::warning(this, "Lỗi", "Dữ liệu sinh viên không khả dụng.");
        return;
    }

    DiemThi *ketQua = currentStudent->getQuanLyDiem()->tim(subjectCode.toStdString().c_str());
    if (!ketQua) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy kết quả bài thi.");
        return;
    }

    // Check if we have question IDs (for detailed view)
    DynamicArray<int> *questionIds = ketQua->getDanhSachCauHoi();
    if (!questionIds || questionIds->size() == 0) {
        // Fallback to simple text display for old exam results
        QString details = QString("Môn học: %1\nĐiểm: %2/10\nTrạng thái: %3\n\nCâu trả lời: ")
                .arg(subjectCode)
                .arg(QString::number(ketQua->getDiem(), 'f', 2))
                .arg(getPassFailStatus(ketQua->getDiem()));

        DynamicArray<char> *answers = ketQua->getDanhSachCauTraLoi();
        if (answers) {
            for (int i = 0; i < answers->size(); i++) {
                details += QString("C%1:%2 ").arg(i + 1).arg(answers->get(i));
            }
        }

        QMessageBox::information(this, "Kết quả chi tiết", details);
        return;
    }

    // Use the new detailed results widget
    if (!subjectManager) {
        QMessageBox::warning(this, "Lỗi", "Quản lý môn học không khả dụng.");
        return;
    }

    DetailedResultsWidget *detailsDialog = new DetailedResultsWidget(this);
    detailsDialog->showResults(ketQua, subjectManager);
    detailsDialog->exec();
    delete detailsDialog;
}

void StudentDashboard::refreshData() {
    updateStudentInfo();
    refreshScores();
}

void StudentDashboard::onExamCompleted(double score) {
    QString message = QString("Bài thi hoàn thành!\nĐiểm của bạn: %1/10\nTrạng thái: %2")
            .arg(QString::number(score, 'f', 2))
            .arg(getPassFailStatus(score));

    QMessageBox::information(this, "Kết quả bài thi", message);

    // Refresh scores to show the new result
    refreshScores();
}

QString StudentDashboard::formatGrade(double score) {
    return QString::number(score, 'f', 2) + "/10";
}

QString StudentDashboard::getPassFailStatus(double score) {
    return (score >= 5.0) ? "ĐẬU" : "RỚT";
}
