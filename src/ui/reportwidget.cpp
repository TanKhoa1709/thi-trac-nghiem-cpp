#include "reportwidget.h"
#include "appmanager.h"
#include <QMessageBox>

ReportWidget::ReportWidget(AppManager* appManager, QWidget *parent)
    : QWidget(parent)
    , appManager(appManager)
{
    setupUI();
    setupConnections();
    loadClasses();
    loadSubjects();
}

ReportWidget::~ReportWidget()
{
}

void ReportWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    
    // Filter section
    filterGroup = new QGroupBox("Bộ lọc báo cáo");
    filterLayout = new QHBoxLayout(filterGroup);
    
    classCombo = new QComboBox();
    subjectCombo = new QComboBox();
    generateBtn = new QPushButton("Tạo báo cáo");
    
    filterLayout->addWidget(new QLabel("Lớp:"));
    filterLayout->addWidget(classCombo);
    filterLayout->addWidget(new QLabel("Môn:"));
    filterLayout->addWidget(subjectCombo);
    filterLayout->addWidget(generateBtn);
    filterLayout->addStretch();
    
    // Report section
    reportGroup = new QGroupBox("Bảng điểm lớp");
    reportLayout = new QVBoxLayout(reportGroup);
    
    reportTable = new QTableWidget();
    reportSummary = new QTextEdit();
    reportSummary->setMaximumHeight(80);
    reportSummary->setReadOnly(true);
    
    reportLayout->addWidget(reportTable);
    reportLayout->addWidget(reportSummary);
    
    mainLayout->addWidget(filterGroup);
    mainLayout->addWidget(reportGroup);
}

void ReportWidget::setupConnections()
{
    connect(generateBtn, &QPushButton::clicked, this, &ReportWidget::onGenerateReport);
}

void ReportWidget::loadClasses()
{
    classCombo->clear();
    classCombo->addItem("Chọn lớp...", "");
    classCombo->addItem("D21CQCN01-B", "D21CQCN01-B");
    classCombo->addItem("D21CQCN02-B", "D21CQCN02-B");
}

void ReportWidget::loadSubjects()
{
    subjectCombo->clear();
    subjectCombo->addItem("Chọn môn...", "");
    subjectCombo->addItem("CTDL&GT", "CTDL");
    subjectCombo->addItem("Toán rời rạc", "TOAN");
}

void ReportWidget::refreshData()
{
    loadClasses();
    loadSubjects();
}

void ReportWidget::onGenerateReport()
{
    generateClassReport();
}

void ReportWidget::generateClassReport()
{
    QString classCode = classCombo->currentData().toString();
    QString subjectCode = subjectCombo->currentData().toString();
    
    if (classCode.isEmpty() || subjectCode.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn lớp và môn học!");
        return;
    }
    
    // Setup table
    reportTable->setColumnCount(4);
    reportTable->setHorizontalHeaderLabels({"MSSV", "Họ tên", "Điểm", "Trạng thái"});
    reportTable->setRowCount(3);
    
    // Sample data
    reportTable->setItem(0, 0, new QTableWidgetItem("N21DCCN001"));
    reportTable->setItem(0, 1, new QTableWidgetItem("Nguyễn Văn A"));
    reportTable->setItem(0, 2, new QTableWidgetItem("8.5"));
    reportTable->setItem(0, 3, new QTableWidgetItem("Đã thi"));
    
    reportTable->setItem(1, 0, new QTableWidgetItem("N21DCCN002"));
    reportTable->setItem(1, 1, new QTableWidgetItem("Trần Thị B"));
    reportTable->setItem(1, 2, new QTableWidgetItem("7.0"));
    reportTable->setItem(1, 3, new QTableWidgetItem("Đã thi"));
    
    reportTable->setItem(2, 0, new QTableWidgetItem("N21DCCN003"));
    reportTable->setItem(2, 1, new QTableWidgetItem("Lê Văn C"));
    reportTable->setItem(2, 2, new QTableWidgetItem(""));
    reportTable->setItem(2, 3, new QTableWidgetItem("Chưa thi"));
    
    reportSummary->setText(QString("Báo cáo lớp %1 - môn %2\nSố sinh viên đã thi: 2/3\nĐiểm trung bình: 7.75")
                          .arg(classCode, subjectCode));
    
    emit reportGenerated("Class Report", QString("Generated for %1 - %2").arg(classCode, subjectCode));
}


