#include "studentmanagementwidget.h"
#include "appmanager.h"
#include "sinhvien.h"
#include "lop.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

StudentManagementWidget::StudentManagementWidget(AppManager* appManager, QWidget *parent)
    : QWidget(parent)
    , appManager(appManager)
    , isEditing(false)
    , currentStudentId("")
    , currentClassFilter("")
{
    setupUI();
    setupConnections();
    applyStyles();
    loadClasses();
    loadStudents();
}

StudentManagementWidget::~StudentManagementWidget()
{
}

void StudentManagementWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    setupStudentList();
    setupStudentForm();
    
    mainSplitter->addWidget(listPanel);
    mainSplitter->addWidget(formPanel);
    mainSplitter->setSizes({400, 300});
    
    mainLayout->addWidget(mainSplitter);
}

void StudentManagementWidget::setupStudentList()
{
    listPanel = new QWidget();
    listLayout = new QVBoxLayout(listPanel);
    
    // Filter section
    filterGroup = new QGroupBox("Tìm kiếm và Lọc");
    filterLayout = new QHBoxLayout(filterGroup);
    
    classFilterCombo = new QComboBox();
    classFilterCombo->addItem("Tất cả lớp", "");
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Tìm kiếm theo tên hoặc MSSV...");
    refreshButton = new QPushButton("Làm mới");
    
    filterLayout->addWidget(new QLabel("Lớp:"));
    filterLayout->addWidget(classFilterCombo);
    filterLayout->addWidget(new QLabel("Tìm kiếm:"));
    filterLayout->addWidget(searchEdit);
    filterLayout->addWidget(refreshButton);
    
    // Student list section
    studentListGroup = new QGroupBox("Danh sách Sinh viên");
    studentListLayout = new QVBoxLayout(studentListGroup);
    
    studentTable = new QTableWidget();
    studentTable->setColumnCount(6);
    QStringList headers = {"MSSV", "Họ tên", "Lớp", "Email", "SĐT", "Ngày sinh"};
    studentTable->setHorizontalHeaderLabels(headers);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTable->setAlternatingRowColors(true);
    studentTable->horizontalHeader()->setStretchLastSection(true);
    studentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    studentListLayout->addWidget(studentTable);
    
    // Action buttons
    listButtonLayout = new QHBoxLayout();
    addButton = new QPushButton("Thêm SV");
    editButton = new QPushButton("Sửa");
    deleteButton = new QPushButton("Xóa");
    viewScoresButton = new QPushButton("Xem điểm");
    
    editButton->setEnabled(false);
    deleteButton->setEnabled(false);
    viewScoresButton->setEnabled(false);
    
    listButtonLayout->addWidget(addButton);
    listButtonLayout->addWidget(editButton);
    listButtonLayout->addWidget(deleteButton);
    listButtonLayout->addWidget(viewScoresButton);
    listButtonLayout->addStretch();
    
    listLayout->addWidget(filterGroup);
    listLayout->addWidget(studentListGroup);
    listLayout->addLayout(listButtonLayout);
}

void StudentManagementWidget::setupStudentForm()
{
    formPanel = new QWidget();
    formLayout = new QVBoxLayout(formPanel);
    
    studentFormGroup = new QGroupBox("Thông tin Sinh viên");
    formGridLayout = new QGridLayout(studentFormGroup);
    
    // Form fields
    idLabel = new QLabel("MSSV:");
    idEdit = new QLineEdit();
    
    classLabel = new QLabel("Lớp:");
    classCombo = new QComboBox();
    
    nameLabel = new QLabel("Họ tên:");
    nameEdit = new QLineEdit();
    
    passwordLabel = new QLabel("Mật khẩu:");
    passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::Password);
    
    emailLabel = new QLabel("Email:");
    emailEdit = new QLineEdit();
    
    phoneLabel = new QLabel("SĐT:");
    phoneEdit = new QLineEdit();
    
    birthdateLabel = new QLabel("Ngày sinh:");
    birthdateEdit = new QDateEdit();
    birthdateEdit->setDate(QDate::currentDate().addYears(-18));
    birthdateEdit->setCalendarPopup(true);
    
    // Layout form fields
    formGridLayout->addWidget(idLabel, 0, 0);
    formGridLayout->addWidget(idEdit, 0, 1);
    formGridLayout->addWidget(classLabel, 1, 0);
    formGridLayout->addWidget(classCombo, 1, 1);
    formGridLayout->addWidget(nameLabel, 2, 0);
    formGridLayout->addWidget(nameEdit, 2, 1);
    formGridLayout->addWidget(passwordLabel, 3, 0);
    formGridLayout->addWidget(passwordEdit, 3, 1);
    formGridLayout->addWidget(emailLabel, 4, 0);
    formGridLayout->addWidget(emailEdit, 4, 1);
    formGridLayout->addWidget(phoneLabel, 5, 0);
    formGridLayout->addWidget(phoneEdit, 5, 1);
    formGridLayout->addWidget(birthdateLabel, 6, 0);
    formGridLayout->addWidget(birthdateEdit, 6, 1);
    
    // Form buttons
    formButtonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Lưu");
    cancelButton = new QPushButton("Hủy");
    clearButton = new QPushButton("Xóa form");
    
    saveButton->setEnabled(false);
    cancelButton->setEnabled(false);
    
    formButtonLayout->addWidget(saveButton);
    formButtonLayout->addWidget(cancelButton);
    formButtonLayout->addWidget(clearButton);
    formButtonLayout->addStretch();
    
    formLayout->addWidget(studentFormGroup);
    formLayout->addLayout(formButtonLayout);
    formLayout->addStretch();
}

void StudentManagementWidget::setupConnections()
{
    // Filter connections
    connect(classFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StudentManagementWidget::onClassChanged);
    connect(searchEdit, &QLineEdit::textChanged,
            this, &StudentManagementWidget::onSearchTextChanged);
    connect(refreshButton, &QPushButton::clicked,
            this, &StudentManagementWidget::refreshData);
    
    // Table connections
    connect(studentTable, &QTableWidget::itemSelectionChanged,
            this, &StudentManagementWidget::onStudentSelectionChanged);
    
    // Button connections
    connect(addButton, &QPushButton::clicked,
            this, &StudentManagementWidget::onAddStudent);
    connect(editButton, &QPushButton::clicked,
            this, &StudentManagementWidget::onEditStudent);
    connect(deleteButton, &QPushButton::clicked,
            this, &StudentManagementWidget::onDeleteStudent);
    connect(viewScoresButton, &QPushButton::clicked,
            this, &StudentManagementWidget::onViewScores);
    
    // Form connections
    connect(saveButton, &QPushButton::clicked,
            this, &StudentManagementWidget::onSaveStudent);
    connect(cancelButton, &QPushButton::clicked,
            this, &StudentManagementWidget::onCancelEdit);
    connect(clearButton, &QPushButton::clicked,
            this, &StudentManagementWidget::onClearForm);
}

void StudentManagementWidget::applyStyles()
{
    setStyleSheet(R"(
        QGroupBox {
            font-weight: bold;
            border: 2px solid #cccccc;
            border-radius: 5px;
            margin-top: 1ex;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        QPushButton {
            background-color: #f0f0f0;
            border: 1px solid #cccccc;
            border-radius: 4px;
            padding: 6px 12px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: #e0e0e0;
        }
        QPushButton:pressed {
            background-color: #d0d0d0;
        }
        QPushButton:disabled {
            background-color: #f5f5f5;
            color: #999999;
        }
        QTableWidget {
            gridline-color: #e0e0e0;
            selection-background-color: #3498db;
        }
        QTableWidget::item:selected {
            background-color: #3498db;
            color: white;
        }
    )");
}

void StudentManagementWidget::loadClasses()
{
    classFilterCombo->clear();
    classCombo->clear();
    
    classFilterCombo->addItem("Tất cả lớp", "");
    classCombo->addItem("Chọn lớp...", "");
    
    if (appManager) {
        // TODO: Get actual classes from AppManager
        // For now, add some sample classes
        QStringList sampleClasses = {"D21CQCN01-B", "D21CQCN02-B", "D21CQCN03-B"};
        for (const QString& className : sampleClasses) {
            classFilterCombo->addItem(className, className);
            classCombo->addItem(className, className);
        }
    }
}

void StudentManagementWidget::loadStudents()
{
    if (!appManager) {
        return;
    }
    
    // TODO: Get actual students from AppManager
    // For now, clear the table and show placeholder message
    studentTable->setRowCount(0);
    
    // Add sample data for demonstration
    QList<QStringList> sampleStudents = {
        {"N21DCCN001", "Nguyễn Văn A", "D21CQCN01-B", "nva@email.com", "0123456789", "01/01/2003"},
        {"N21DCCN002", "Trần Thị B", "D21CQCN01-B", "ttb@email.com", "0987654321", "15/05/2003"},
        {"N21DCCN003", "Lê Văn C", "D21CQCN02-B", "lvc@email.com", "0111222333", "20/08/2003"}
    };
    
    studentTable->setRowCount(sampleStudents.size());
    for (int i = 0; i < sampleStudents.size(); ++i) {
        const QStringList& student = sampleStudents[i];
        for (int j = 0; j < student.size() && j < studentTable->columnCount(); ++j) {
            studentTable->setItem(i, j, new QTableWidgetItem(student[j]));
        }
    }
    
    filterStudents();
}

void StudentManagementWidget::filterStudents()
{
    QString classFilter = classFilterCombo->currentData().toString();
    QString searchText = searchEdit->text().toLower();
    
    for (int i = 0; i < studentTable->rowCount(); ++i) {
        bool showRow = true;
        
        // Class filter
        if (!classFilter.isEmpty()) {
            QTableWidgetItem* classItem = studentTable->item(i, 2);
            if (!classItem || classItem->text() != classFilter) {
                showRow = false;
            }
        }
        
        // Search filter
        if (showRow && !searchText.isEmpty()) {
            bool matchFound = false;
            for (int j = 0; j < studentTable->columnCount(); ++j) {
                QTableWidgetItem* item = studentTable->item(i, j);
                if (item && item->text().toLower().contains(searchText)) {
                    matchFound = true;
                    break;
                }
            }
            if (!matchFound) {
                showRow = false;
            }
        }
        
        studentTable->setRowHidden(i, !showRow);
    }
}

void StudentManagementWidget::populateStudentTable()
{
    loadStudents();
}

void StudentManagementWidget::clearStudentForm()
{
    idEdit->clear();
    classCombo->setCurrentIndex(0);
    nameEdit->clear();
    passwordEdit->clear();
    emailEdit->clear();
    phoneEdit->clear();
    birthdateEdit->setDate(QDate::currentDate().addYears(-18));
    
    isEditing = false;
    currentStudentId.clear();
    
    idEdit->setEnabled(true);
    saveButton->setEnabled(false);
    cancelButton->setEnabled(false);
}

void StudentManagementWidget::loadStudentToForm(const SinhVien& student)
{
    idEdit->setText(student.getMaSV());
    nameEdit->setText(student.getHoTen());
    // Note: Password field removed as SinhVien class doesn't have password field
    emailEdit->setText(student.getEmail());
    phoneEdit->setText(student.getSoDienThoai());
    birthdateEdit->setDate(student.getNgaySinh());
    
    // Set class combo
    QString classCode = student.getMaLop();
    for (int i = 0; i < classCombo->count(); ++i) {
        if (classCombo->itemData(i).toString() == classCode) {
            classCombo->setCurrentIndex(i);
            break;
        }
    }
}

bool StudentManagementWidget::validateStudentForm()
{
    if (idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập MSSV!");
        idEdit->setFocus();
        return false;
    }
    
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập họ tên!");
        nameEdit->setFocus();
        return false;
    }
    
    if (classCombo->currentData().toString().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn lớp!");
        classCombo->setFocus();
        return false;
    }
    
    if (passwordEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng nhập mật khẩu!");
        passwordEdit->setFocus();
        return false;
    }
    
    // Check for duplicate student ID when adding new student
    if (!isEditing) {
        // TODO: Check with AppManager for duplicate student ID
        // For now, just check against table data
        QString newId = idEdit->text().trimmed();
        for (int i = 0; i < studentTable->rowCount(); ++i) {
            QTableWidgetItem* item = studentTable->item(i, 0);
            if (item && item->text() == newId) {
                QMessageBox::warning(this, "Lỗi", "MSSV đã tồn tại!");
                idEdit->setFocus();
                return false;
            }
        }
    }
    
    return true;
}

void StudentManagementWidget::saveStudentFromForm()
{
    if (!validateStudentForm()) {
        return;
    }
    
    // TODO: Create SinhVien object and save through AppManager
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString classCode = classCombo->currentData().toString();
    QString password = passwordEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QDate birthdate = birthdateEdit->date();
    
    // For now, just add to table
    if (!isEditing) {
        int row = studentTable->rowCount();
        studentTable->insertRow(row);
        studentTable->setItem(row, 0, new QTableWidgetItem(id));
        studentTable->setItem(row, 1, new QTableWidgetItem(name));
        studentTable->setItem(row, 2, new QTableWidgetItem(classCode));
        studentTable->setItem(row, 3, new QTableWidgetItem(email));
        studentTable->setItem(row, 4, new QTableWidgetItem(phone));
        studentTable->setItem(row, 5, new QTableWidgetItem(birthdate.toString("dd/MM/yyyy")));
        
        emit studentAdded(id, classCode);
        QMessageBox::information(this, "Thành công", "Đã thêm sinh viên thành công!");
    } else {
        updateStudentInForm();
    }
    
    clearStudentForm();
    filterStudents();
}

void StudentManagementWidget::updateStudentInForm()
{
    // TODO: Update student through AppManager
    // For now, update table row
    int currentRow = studentTable->currentRow();
    if (currentRow >= 0) {
        studentTable->item(currentRow, 0)->setText(idEdit->text().trimmed());
        studentTable->item(currentRow, 1)->setText(nameEdit->text().trimmed());
        studentTable->item(currentRow, 2)->setText(classCombo->currentData().toString());
        studentTable->item(currentRow, 3)->setText(emailEdit->text().trimmed());
        studentTable->item(currentRow, 4)->setText(phoneEdit->text().trimmed());
        studentTable->item(currentRow, 5)->setText(birthdateEdit->date().toString("dd/MM/yyyy"));
        
        emit studentUpdated(currentStudentId);
        QMessageBox::information(this, "Thành công", "Đã cập nhật sinh viên thành công!");
    }
}

bool StudentManagementWidget::canDeleteStudent(const QString& studentId)
{
    // TODO: Check if student has exam records
    // For now, always allow deletion
    return true;
}

// Slots implementation
void StudentManagementWidget::refreshData()
{
    loadClasses();
    loadStudents();
}

void StudentManagementWidget::onClassChanged()
{
    filterStudents();
}

void StudentManagementWidget::onAddStudent()
{
    clearStudentForm();
    isEditing = false;
    saveButton->setEnabled(true);
    cancelButton->setEnabled(true);
    idEdit->setFocus();
}

void StudentManagementWidget::onEditStudent()
{
    int currentRow = studentTable->currentRow();
    if (currentRow < 0) return;
    
    QTableWidgetItem* idItem = studentTable->item(currentRow, 0);
    if (!idItem) return;
    
    currentStudentId = idItem->text();
    isEditing = true;
    
    // Load data to form
    idEdit->setText(studentTable->item(currentRow, 0)->text());
    nameEdit->setText(studentTable->item(currentRow, 1)->text());
    emailEdit->setText(studentTable->item(currentRow, 3)->text());
    phoneEdit->setText(studentTable->item(currentRow, 4)->text());
    
    // Set class
    QString classCode = studentTable->item(currentRow, 2)->text();
    for (int i = 0; i < classCombo->count(); ++i) {
        if (classCombo->itemData(i).toString() == classCode) {
            classCombo->setCurrentIndex(i);
            break;
        }
    }
    
    // Parse birthdate
    QString birthdateStr = studentTable->item(currentRow, 5)->text();
    QDate birthdate = QDate::fromString(birthdateStr, "dd/MM/yyyy");
    if (birthdate.isValid()) {
        birthdateEdit->setDate(birthdate);
    }
    
    idEdit->setEnabled(false);
    saveButton->setEnabled(true);
    cancelButton->setEnabled(true);
    nameEdit->setFocus();
}

void StudentManagementWidget::onDeleteStudent()
{
    int currentRow = studentTable->currentRow();
    if (currentRow < 0) return;
    
    QTableWidgetItem* idItem = studentTable->item(currentRow, 0);
    QTableWidgetItem* nameItem = studentTable->item(currentRow, 1);
    if (!idItem || !nameItem) return;
    
    QString studentId = idItem->text();
    QString studentName = nameItem->text();
    
    if (!canDeleteStudent(studentId)) {
        QMessageBox::warning(this, "Không thể xóa", 
                           "Không thể xóa sinh viên này vì đã có dữ liệu thi!");
        return;
    }
    
    int ret = QMessageBox::question(this, "Xác nhận xóa",
                                   QString("Bạn có chắc chắn muốn xóa sinh viên:\n%1 - %2?")
                                   .arg(studentId, studentName),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        // TODO: Delete through AppManager
        studentTable->removeRow(currentRow);
        emit studentDeleted(studentId);
        QMessageBox::information(this, "Thành công", "Đã xóa sinh viên thành công!");
        clearStudentForm();
    }
}

void StudentManagementWidget::onStudentSelectionChanged()
{
    bool hasSelection = studentTable->currentRow() >= 0;
    editButton->setEnabled(hasSelection);
    deleteButton->setEnabled(hasSelection);
    viewScoresButton->setEnabled(hasSelection);
}

void StudentManagementWidget::onSearchTextChanged()
{
    filterStudents();
}

void StudentManagementWidget::onClearForm()
{
    clearStudentForm();
}

void StudentManagementWidget::onSaveStudent()
{
    saveStudentFromForm();
}

void StudentManagementWidget::onCancelEdit()
{
    clearStudentForm();
}

void StudentManagementWidget::onViewScores()
{
    int currentRow = studentTable->currentRow();
    if (currentRow < 0) return;
    
    QTableWidgetItem* idItem = studentTable->item(currentRow, 0);
    QTableWidgetItem* nameItem = studentTable->item(currentRow, 1);
    if (!idItem || !nameItem) return;
    
    QString studentId = idItem->text();
    QString studentName = nameItem->text();
    
    // TODO: Implement score viewing dialog
    QMessageBox::information(this, "Xem điểm", 
                           QString("Chức năng xem điểm cho sinh viên:\n%1 - %2\n\nSẽ được triển khai sau.")
                           .arg(studentId, studentName));
}