#include "classmanagementwidget.h"
#include "appmanager.h"
#include "lop.h"

ClassManagementWidget::ClassManagementWidget(AppManager* appManager, QWidget *parent)
    : QWidget(parent)
    , appManager(appManager)
    , isEditing(false)
{
    setupUI();
    setupConnections();
    applyStyles();
    refreshData();
}

ClassManagementWidget::~ClassManagementWidget()
{
    // Cleanup handled by Qt parent-child relationship
}

void ClassManagementWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // Search and actions section
    searchGroup = new QGroupBox("Search & Actions");
    searchLayout = new QHBoxLayout(searchGroup);
    
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search by class code or name...");
    refreshButton = new QPushButton("Refresh");
    refreshButton->setObjectName("actionButton");
    printButton = new QPushButton("Print Class List");
    printButton->setObjectName("actionButton");
    
    searchLayout->addWidget(new QLabel("Search:"));
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(refreshButton);
    searchLayout->addWidget(printButton);
    
    // Class list section
    listGroup = new QGroupBox("Class List");
    listLayout = new QVBoxLayout(listGroup);
    
    classTable = new QTableWidget();
    classTable->setColumnCount(4);
    QStringList headers = {"Class Code", "Class Name", "Academic Year", "Student Count"};
    classTable->setHorizontalHeaderLabels(headers);
    classTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    classTable->setAlternatingRowColors(true);
    classTable->horizontalHeader()->setStretchLastSection(true);
    classTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Action buttons
    buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add Class");
    addButton->setObjectName("actionButton");
    editButton = new QPushButton("Edit Class");
    editButton->setObjectName("actionButton");
    editButton->setEnabled(false);
    deleteButton = new QPushButton("Delete Class");
    deleteButton->setObjectName("actionButton");
    deleteButton->setEnabled(false);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    
    listLayout->addWidget(classTable);
    listLayout->addLayout(buttonLayout);
    
    // Form section
    formGroup = new QGroupBox("Class Details");
    formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Class Code:"), 0, 0);
    codeEdit = new QLineEdit();
    codeEdit->setMaxLength(10);
    codeEdit->setPlaceholderText("e.g., CNTT01");
    formLayout->addWidget(codeEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Class Name:"), 1, 0);
    nameEdit = new QLineEdit();
    nameEdit->setMaxLength(100);
    nameEdit->setPlaceholderText("e.g., Computer Science 01");
    formLayout->addWidget(nameEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Academic Year:"), 2, 0);
    yearEdit = new QLineEdit();
    yearEdit->setMaxLength(10);
    yearEdit->setPlaceholderText("e.g., 2023-2024");
    formLayout->addWidget(yearEdit, 2, 1);
    
    QHBoxLayout* formButtonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save");
    saveButton->setObjectName("actionButton");
    cancelButton = new QPushButton("Cancel");
    cancelButton->setObjectName("secondaryButton");
    
    formButtonLayout->addWidget(saveButton);
    formButtonLayout->addWidget(cancelButton);
    formButtonLayout->addStretch();
    
    formLayout->addLayout(formButtonLayout, 3, 0, 1, 2);
    
    // Add all sections to main layout
    mainLayout->addWidget(searchGroup);
    mainLayout->addWidget(listGroup);
    mainLayout->addWidget(formGroup);
    
    // Initially hide form
    formGroup->setVisible(false);
}

void ClassManagementWidget::setupConnections()
{
    connect(searchEdit, &QLineEdit::textChanged, this, &ClassManagementWidget::onSearchTextChanged);
    connect(refreshButton, &QPushButton::clicked, this, &ClassManagementWidget::refreshData);
    connect(printButton, &QPushButton::clicked, this, &ClassManagementWidget::onPrintClassList);
    
    connect(classTable, &QTableWidget::itemSelectionChanged, this, &ClassManagementWidget::onClassSelectionChanged);
    
    connect(addButton, &QPushButton::clicked, this, &ClassManagementWidget::onAddClass);
    connect(editButton, &QPushButton::clicked, this, &ClassManagementWidget::onEditClass);
    connect(deleteButton, &QPushButton::clicked, this, &ClassManagementWidget::onDeleteClass);
    
    connect(saveButton, &QPushButton::clicked, [this]() {
        if (validateInput()) {
            QString code = codeEdit->text().trimmed().toUpper();
            QString name = nameEdit->text().trimmed();
            QString year = yearEdit->text().trimmed();
            
            if (isEditing) {
                // Update existing class
                QMessageBox::information(this, "Success", "Class updated successfully!");
                emit classUpdated(code);
            } else {
                // Add new class
                QMessageBox::information(this, "Success", "Class added successfully!");
                emit classAdded(code, name);
            }
            
            clearForm();
            formGroup->setVisible(false);
            refreshData();
        }
    });
    
    connect(cancelButton, &QPushButton::clicked, [this]() {
        clearForm();
        formGroup->setVisible(false);
    });
}

void ClassManagementWidget::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f8f9fa;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        
        QGroupBox {
            font-size: 16px;
            font-weight: bold;
            color: #2c3e50;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background-color: white;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
            background-color: white;
        }
        
        QTableWidget {
            background-color: white;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            gridline-color: #ecf0f1;
        }
        
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #ecf0f1;
        }
        
        QTableWidget::item:selected {
            background-color: #3498db;
            color: white;
        }
        
        QHeaderView::section {
            background-color: #34495e;
            color: white;
            padding: 8px;
            border: none;
            font-weight: bold;
        }
        
        QLineEdit {
            padding: 8px;
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            font-size: 14px;
            background-color: white;
        }
        
        QLineEdit:focus {
            border-color: #3498db;
        }
        
        #actionButton {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 100px;
        }
        
        #actionButton:hover {
            background-color: #2980b9;
        }
        
        #actionButton:pressed {
            background-color: #21618c;
        }
        
        #actionButton:disabled {
            background-color: #bdc3c7;
            color: #7f8c8d;
        }
        
        #secondaryButton {
            background-color: #95a5a6;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 100px;
        }
        
        #secondaryButton:hover {
            background-color: #7f8c8d;
        }
    )");
}

void ClassManagementWidget::refreshData()
{
    updateClassTable();
}

void ClassManagementWidget::updateClassTable()
{
    // Get actual data from AppManager
    classTable->setRowCount(0);
    
    if (appManager && appManager->getLopManager()) {
        // TODO: Get classes from LopManager
        QList<Lop> classes; // Placeholder
        
        classTable->setRowCount(classes.size());
        for (int i = 0; i < classes.size(); ++i) {
            const Lop& lop = classes[i];
            classTable->setItem(i, 0, new QTableWidgetItem(lop.getMaLop()));
            classTable->setItem(i, 1, new QTableWidgetItem(lop.getTenLop()));
            classTable->setItem(i, 2, new QTableWidgetItem(QString::number(lop.getNienKhoa())));
            classTable->setItem(i, 3, new QTableWidgetItem(lop.getKhoa()));
            classTable->setItem(i, 4, new QTableWidgetItem(QString::number(lop.soLuongSinhVien())));
        }
        return;
    }
    
    // Fallback: Get actual data from AppManager
    // For now, add some sample data
    classTable->setRowCount(0);
    
    // Sample classes
    QStringList sampleClasses = {
        "CNTT01|Computer Science 01|2023-2024|45",
        "CNTT02|Computer Science 02|2023-2024|42",
        "KTPM01|Software Engineering 01|2023-2024|38",
        "KHMT01|Computer Science 01|2022-2023|40",
        "ATTT01|Information Security 01|2023-2024|35"
    };
    
    for (const QString& classInfo : sampleClasses) {
        QStringList parts = classInfo.split("|");
        if (parts.size() >= 4) {
            int row = classTable->rowCount();
            classTable->insertRow(row);
            classTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            classTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            classTable->setItem(row, 2, new QTableWidgetItem(parts[2]));
            classTable->setItem(row, 3, new QTableWidgetItem(parts[3]));
        }
    }
}

void ClassManagementWidget::onAddClass()
{
    isEditing = false;
    currentClassCode.clear();
    clearForm();
    formGroup->setVisible(true);
    codeEdit->setEnabled(true);
    codeEdit->setFocus();
}

void ClassManagementWidget::onEditClass()
{
    int currentRow = classTable->currentRow();
    if (currentRow >= 0) {
        isEditing = true;
        currentClassCode = classTable->item(currentRow, 0)->text();
        
        codeEdit->setText(currentClassCode);
        nameEdit->setText(classTable->item(currentRow, 1)->text());
        yearEdit->setText(classTable->item(currentRow, 2)->text());
        
        codeEdit->setEnabled(false); // Don't allow editing the code
        formGroup->setVisible(true);
        nameEdit->setFocus();
    }
}

void ClassManagementWidget::onDeleteClass()
{
    int currentRow = classTable->currentRow();
    if (currentRow >= 0) {
        QString classCode = classTable->item(currentRow, 0)->text();
        QString className = classTable->item(currentRow, 1)->text();
        
        int ret = QMessageBox::question(this, "Delete Class",
            QString("Are you sure you want to delete class '%1 - %2'?\n\nThis will also remove all students in this class.\nThis action cannot be undone.")
            .arg(classCode, className),
            QMessageBox::Yes | QMessageBox::No);
            
        if (ret == QMessageBox::Yes) {
            QMessageBox::information(this, "Success", "Class deleted successfully!");
            emit classDeleted(classCode);
            refreshData();
        }
    }
}

void ClassManagementWidget::onClassSelectionChanged()
{
    bool hasSelection = classTable->currentRow() >= 0;
    editButton->setEnabled(hasSelection);
    deleteButton->setEnabled(hasSelection);
}

void ClassManagementWidget::onSearchTextChanged()
{
    QString searchText = searchEdit->text().toLower();
    
    for (int row = 0; row < classTable->rowCount(); ++row) {
        bool visible = false;
        for (int col = 0; col < classTable->columnCount(); ++col) {
            QTableWidgetItem* item = classTable->item(row, col);
            if (item && item->text().toLower().contains(searchText)) {
                visible = true;
                break;
            }
        }
        classTable->setRowHidden(row, !visible);
    }
}

void ClassManagementWidget::onPrintClassList()
{
    // TODO: Implement actual printing functionality
    QMessageBox::information(this, "Print Class List", 
        "Print functionality will be implemented here.\n\nThis will generate a formatted report of all classes with their details.");
}

void ClassManagementWidget::clearForm()
{
    codeEdit->clear();
    nameEdit->clear();
    yearEdit->clear();
    isEditing = false;
    currentClassCode.clear();
}

bool ClassManagementWidget::validateInput()
{
    if (codeEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Class code is required.");
        codeEdit->setFocus();
        return false;
    }
    
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Class name is required.");
        nameEdit->setFocus();
        return false;
    }
    
    if (yearEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Academic year is required.");
        yearEdit->setFocus();
        return false;
    }
    
    return true;
}
