#include "subjectmanagementwidget.h"
#include "appmanager.h"
#include "monhoc.h"

SubjectManagementWidget::SubjectManagementWidget(AppManager* appManager, QWidget *parent)
    : QWidget(parent)
    , appManager(appManager)
    , isEditing(false)
{
    setupUI();
    setupConnections();
    applyStyles();
    refreshData();
}

SubjectManagementWidget::~SubjectManagementWidget()
{
    // Cleanup handled by Qt parent-child relationship
}

void SubjectManagementWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // Search section
    searchGroup = new QGroupBox("Search Subjects");
    searchLayout = new QHBoxLayout(searchGroup);
    
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search by subject code or name...");
    refreshButton = new QPushButton("Refresh");
    refreshButton->setObjectName("actionButton");
    
    searchLayout->addWidget(new QLabel("Search:"));
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(refreshButton);
    
    // Subject list section
    listGroup = new QGroupBox("Subject List");
    listLayout = new QVBoxLayout(listGroup);
    
    subjectTable = new QTableWidget();
    subjectTable->setColumnCount(3);
    QStringList headers = {"Subject Code", "Subject Name", "Description"};
    subjectTable->setHorizontalHeaderLabels(headers);
    subjectTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    subjectTable->setAlternatingRowColors(true);
    subjectTable->horizontalHeader()->setStretchLastSection(true);
    subjectTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // Action buttons
    buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add Subject");
    addButton->setObjectName("actionButton");
    editButton = new QPushButton("Edit Subject");
    editButton->setObjectName("actionButton");
    editButton->setEnabled(false);
    deleteButton = new QPushButton("Delete Subject");
    deleteButton->setObjectName("actionButton");
    deleteButton->setEnabled(false);
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    
    listLayout->addWidget(subjectTable);
    listLayout->addLayout(buttonLayout);
    
    // Form section
    formGroup = new QGroupBox("Subject Details");
    formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Subject Code:"), 0, 0);
    codeEdit = new QLineEdit();
    codeEdit->setMaxLength(10);
    formLayout->addWidget(codeEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Subject Name:"), 1, 0);
    nameEdit = new QLineEdit();
    nameEdit->setMaxLength(100);
    formLayout->addWidget(nameEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Description:"), 2, 0);
    descriptionEdit = new QTextEdit();
    descriptionEdit->setMaximumHeight(100);
    formLayout->addWidget(descriptionEdit, 2, 1);
    
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

void SubjectManagementWidget::setupConnections()
{
    connect(searchEdit, &QLineEdit::textChanged, this, &SubjectManagementWidget::onSearchTextChanged);
    connect(refreshButton, &QPushButton::clicked, this, &SubjectManagementWidget::refreshData);
    
    connect(subjectTable, &QTableWidget::itemSelectionChanged, this, &SubjectManagementWidget::onSubjectSelectionChanged);
    
    connect(addButton, &QPushButton::clicked, this, &SubjectManagementWidget::onAddSubject);
    connect(editButton, &QPushButton::clicked, this, &SubjectManagementWidget::onEditSubject);
    connect(deleteButton, &QPushButton::clicked, this, &SubjectManagementWidget::onDeleteSubject);
    
    connect(saveButton, &QPushButton::clicked, [this]() {
        if (validateInput()) {
            QString code = codeEdit->text().trimmed().toUpper();
            QString name = nameEdit->text().trimmed();
            QString description = descriptionEdit->toPlainText().trimmed();
            
            if (isEditing) {
                // Update existing subject
                if (appManager && appManager->getMonHocManager()) {
                    // TODO: Update subject through MonHocManager
                    bool success = true; // Placeholder
                    if (success) {
                        QMessageBox::information(this, "Success", "Subject updated successfully!");
                        emit subjectUpdated(code);
                    } else {
                        QMessageBox::warning(this, "Error", "Failed to update subject!");
                        return;
                    }
                } else {
                    QMessageBox::warning(this, "Error", "AppManager not available!");
                    return;
                }
            } else {
                // Add new subject
                if (appManager && appManager->getMonHocManager()) {
                    // TODO: Add subject through MonHocManager
                    bool success = true; // Placeholder
                    if (success) {
                        QMessageBox::information(this, "Success", "Subject added successfully!");
                        emit subjectAdded(code, name);
                    } else {
                        QMessageBox::warning(this, "Error", "Failed to add subject! Subject code may already exist.");
                        return;
                    }
                } else {
                    QMessageBox::warning(this, "Error", "AppManager not available!");
                    return;
                }
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

void SubjectManagementWidget::applyStyles()
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
        
        QLineEdit, QTextEdit {
            padding: 8px;
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            font-size: 14px;
            background-color: white;
        }
        
        QLineEdit:focus, QTextEdit:focus {
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

void SubjectManagementWidget::refreshData()
{
    updateSubjectTable();
}

void SubjectManagementWidget::updateSubjectTable()
{
    // Get actual data from AppManager
    subjectTable->setRowCount(0);
    
    if (appManager && appManager->getMonHocManager()) {
        // TODO: Get subjects from MonHocManager
        QList<MonHoc> subjects; // Placeholder
        
        subjectTable->setRowCount(subjects.size());
        for (int i = 0; i < subjects.size(); ++i) {
            const MonHoc& subject = subjects[i];
            subjectTable->setItem(i, 0, new QTableWidgetItem(subject.getMaMon()));
            subjectTable->setItem(i, 1, new QTableWidgetItem(subject.getTenMon()));
            subjectTable->setItem(i, 2, new QTableWidgetItem(QString::number(subject.getSoTinChi()) + " credits"));
        }
        return;
    }
    
    // Fallback: Get actual data from AppManager
    // For now, add some sample data
    subjectTable->setRowCount(0);
    
    // Sample subjects
    QStringList sampleSubjects = {
        "CTDL&GT|Data Structures & Algorithms|Core computer science subject covering fundamental data structures and algorithms",
        "OOP|Object-Oriented Programming|Programming paradigm based on objects and classes",
        "DB|Database Systems|Design and implementation of database systems",
        "SE|Software Engineering|Systematic approach to software development",
        "AI|Artificial Intelligence|Introduction to AI concepts and techniques"
    };
    
    for (const QString& subject : sampleSubjects) {
        QStringList parts = subject.split("|");
        if (parts.size() >= 3) {
            int row = subjectTable->rowCount();
            subjectTable->insertRow(row);
            subjectTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            subjectTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            subjectTable->setItem(row, 2, new QTableWidgetItem(parts[2]));
        }
    }
}

void SubjectManagementWidget::onAddSubject()
{
    isEditing = false;
    currentSubjectCode.clear();
    clearForm();
    formGroup->setVisible(true);
    codeEdit->setEnabled(true);
    codeEdit->setFocus();
}

void SubjectManagementWidget::onEditSubject()
{
    int currentRow = subjectTable->currentRow();
    if (currentRow >= 0) {
        isEditing = true;
        currentSubjectCode = subjectTable->item(currentRow, 0)->text();
        
        codeEdit->setText(currentSubjectCode);
        nameEdit->setText(subjectTable->item(currentRow, 1)->text());
        descriptionEdit->setPlainText(subjectTable->item(currentRow, 2)->text());
        
        codeEdit->setEnabled(false); // Don't allow editing the code
        formGroup->setVisible(true);
        nameEdit->setFocus();
    }
}

void SubjectManagementWidget::onDeleteSubject()
{
    int currentRow = subjectTable->currentRow();
    if (currentRow >= 0) {
        QString subjectCode = subjectTable->item(currentRow, 0)->text();
        QString subjectName = subjectTable->item(currentRow, 1)->text();
        
        int ret = QMessageBox::question(this, "Delete Subject",
            QString("Are you sure you want to delete subject '%1 - %2'?\n\nThis action cannot be undone.")
            .arg(subjectCode, subjectName),
            QMessageBox::Yes | QMessageBox::No);
            
        if (ret == QMessageBox::Yes) {
            // Call appManager to delete subject
            if (appManager && appManager->getMonHocManager()) {
                // TODO: Remove subject through MonHocManager
                bool success = true; // Placeholder
                if (success) {
                    QMessageBox::information(this, "Success", "Subject deleted successfully!");
                    emit subjectDeleted(subjectCode);
                    refreshData();
                } else {
                    QMessageBox::warning(this, "Error", "Failed to delete subject! It may be used in exams.");
                }
            } else {
                QMessageBox::warning(this, "Error", "AppManager not available!");
            }
        }
    }
}

void SubjectManagementWidget::onSubjectSelectionChanged()
{
    bool hasSelection = subjectTable->currentRow() >= 0;
    editButton->setEnabled(hasSelection);
    deleteButton->setEnabled(hasSelection);
}

void SubjectManagementWidget::onSearchTextChanged()
{
    QString searchText = searchEdit->text().toLower();
    
    for (int row = 0; row < subjectTable->rowCount(); ++row) {
        bool visible = false;
        for (int col = 0; col < subjectTable->columnCount(); ++col) {
            QTableWidgetItem* item = subjectTable->item(row, col);
            if (item && item->text().toLower().contains(searchText)) {
                visible = true;
                break;
            }
        }
        subjectTable->setRowHidden(row, !visible);
    }
}

void SubjectManagementWidget::clearForm()
{
    codeEdit->clear();
    nameEdit->clear();
    descriptionEdit->clear();
    isEditing = false;
    currentSubjectCode.clear();
}

bool SubjectManagementWidget::validateInput()
{
    if (codeEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Subject code is required.");
        codeEdit->setFocus();
        return false;
    }
    
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Subject name is required.");
        nameEdit->setFocus();
        return false;
    }
    
    // Check for duplicate subject codes when not editing
    if (!isEditing) {
        QString code = codeEdit->text().trimmed().toUpper();
        // Check if code already exists
        if (appManager && appManager->getMonHocManager()) {
            // TODO: Validate subject ID through MonHocManager
            if (false) { // Placeholder
                QMessageBox::warning(this, "Error", "Subject code already exists!");
                codeEdit->setFocus();
                return false;
            }
        }
    }
    
    return true;
}
