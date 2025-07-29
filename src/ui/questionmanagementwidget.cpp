#include "questionmanagementwidget.h"
#include "appmanager.h"
#include "cauhoi.h"
#include "monhoc.h"

QuestionManagementWidget::QuestionManagementWidget(AppManager* appManager, QWidget *parent)
    : QWidget(parent)
    , appManager(appManager)
    , isEditing(false)
    , currentQuestionId(-1)
{
    setupUI();
    setupConnections();
    applyStyles();
    loadSubjects();
    refreshData();
}

QuestionManagementWidget::~QuestionManagementWidget()
{
    // Cleanup handled by Qt parent-child relationship
}

void QuestionManagementWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // Create main splitter
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(mainSplitter);
    
    setupQuestionList();
    setupQuestionForm();
    
    // Set splitter proportions
    mainSplitter->setStretchFactor(0, 2); // List panel takes 2/3
    mainSplitter->setStretchFactor(1, 1); // Form panel takes 1/3
}

void QuestionManagementWidget::setupQuestionList()
{
    listPanel = new QWidget();
    listLayout = new QVBoxLayout(listPanel);
    listLayout->setSpacing(15);
    
    // Filter section
    filterGroup = new QGroupBox("Filter Questions");
    filterLayout = new QHBoxLayout(filterGroup);
    
    filterLayout->addWidget(new QLabel("Subject:"));
    subjectFilterCombo = new QComboBox();
    subjectFilterCombo->setMinimumWidth(200);
    filterLayout->addWidget(subjectFilterCombo);
    
    filterLayout->addWidget(new QLabel("Search:"));
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search questions...");
    filterLayout->addWidget(searchEdit);
    
    refreshButton = new QPushButton("Refresh");
    refreshButton->setObjectName("secondaryButton");
    filterLayout->addWidget(refreshButton);
    
    listLayout->addWidget(filterGroup);
    
    // Question list
    questionListGroup = new QGroupBox("Questions");
    questionListLayout = new QVBoxLayout(questionListGroup);
    
    questionTable = new QTableWidget();
    questionTable->setColumnCount(4);
    QStringList headers = {"ID", "Subject", "Question", "Correct Answer"};
    questionTable->setHorizontalHeaderLabels(headers);
    
    // Configure table
    questionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    questionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    questionTable->setAlternatingRowColors(true);
    questionTable->setSortingEnabled(true);
    
    // Set column widths
    questionTable->horizontalHeader()->setStretchLastSection(true);
    questionTable->setColumnWidth(0, 80);  // ID
    questionTable->setColumnWidth(1, 120); // Subject
    questionTable->setColumnWidth(2, 300); // Question
    
    questionListLayout->addWidget(questionTable);
    
    // List buttons
    listButtonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add Question");
    addButton->setObjectName("actionButton");
    editButton = new QPushButton("Edit");
    editButton->setObjectName("secondaryButton");
    editButton->setEnabled(false);
    deleteButton = new QPushButton("Delete");
    deleteButton->setObjectName("deleteButton");
    deleteButton->setEnabled(false);
    
    listButtonLayout->addWidget(addButton);
    listButtonLayout->addWidget(editButton);
    listButtonLayout->addWidget(deleteButton);
    listButtonLayout->addStretch();
    
    questionListLayout->addLayout(listButtonLayout);
    listLayout->addWidget(questionListGroup);
    
    mainSplitter->addWidget(listPanel);
}

void QuestionManagementWidget::setupQuestionForm()
{
    formPanel = new QWidget();
    formLayout = new QVBoxLayout(formPanel);
    formLayout->setSpacing(15);
    
    // Question form
    questionFormGroup = new QGroupBox("Question Details");
    formGridLayout = new QGridLayout(questionFormGroup);
    formGridLayout->setSpacing(10);
    
    int row = 0;
    
    // Question ID (read-only when editing)
    idLabel = new QLabel("Question ID:");
    idEdit = new QLineEdit();
    idEdit->setReadOnly(true);
    idEdit->setPlaceholderText("Auto-generated");
    formGridLayout->addWidget(idLabel, row, 0);
    formGridLayout->addWidget(idEdit, row++, 1);
    
    // Subject selection
    subjectLabel = new QLabel("Subject:");
    subjectCombo = new QComboBox();
    subjectCombo->setMinimumWidth(200);
    formGridLayout->addWidget(subjectLabel, row, 0);
    formGridLayout->addWidget(subjectCombo, row++, 1);
    
    // Question text
    questionLabel = new QLabel("Question:");
    questionEdit = new QTextEdit();
    questionEdit->setMaximumHeight(100);
    questionEdit->setPlaceholderText("Enter the question text...");
    formGridLayout->addWidget(questionLabel, row, 0, Qt::AlignTop);
    formGridLayout->addWidget(questionEdit, row++, 1);
    
    // Answer options
    answerALabel = new QLabel("Answer A:");
    answerAEdit = new QLineEdit();
    answerAEdit->setPlaceholderText("Enter answer option A");
    formGridLayout->addWidget(answerALabel, row, 0);
    formGridLayout->addWidget(answerAEdit, row++, 1);
    
    answerBLabel = new QLabel("Answer B:");
    answerBEdit = new QLineEdit();
    answerBEdit->setPlaceholderText("Enter answer option B");
    formGridLayout->addWidget(answerBLabel, row, 0);
    formGridLayout->addWidget(answerBEdit, row++, 1);
    
    answerCLabel = new QLabel("Answer C:");
    answerCEdit = new QLineEdit();
    answerCEdit->setPlaceholderText("Enter answer option C");
    formGridLayout->addWidget(answerCLabel, row, 0);
    formGridLayout->addWidget(answerCEdit, row++, 1);
    
    answerDLabel = new QLabel("Answer D:");
    answerDEdit = new QLineEdit();
    answerDEdit->setPlaceholderText("Enter answer option D");
    formGridLayout->addWidget(answerDLabel, row, 0);
    formGridLayout->addWidget(answerDEdit, row++, 1);
    
    // Correct answer
    correctAnswerLabel = new QLabel("Correct Answer:");
    correctAnswerCombo = new QComboBox();
    correctAnswerCombo->addItems({"A", "B", "C", "D"});
    formGridLayout->addWidget(correctAnswerLabel, row, 0);
    formGridLayout->addWidget(correctAnswerCombo, row++, 1);
    
    formLayout->addWidget(questionFormGroup);
    
    // Form buttons
    formButtonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save");
    saveButton->setObjectName("actionButton");
    saveButton->setEnabled(false);
    cancelButton = new QPushButton("Cancel");
    cancelButton->setObjectName("secondaryButton");
    cancelButton->setEnabled(false);
    clearButton = new QPushButton("Clear");
    clearButton->setObjectName("clearButton");
    
    formButtonLayout->addWidget(saveButton);
    formButtonLayout->addWidget(cancelButton);
    formButtonLayout->addWidget(clearButton);
    formButtonLayout->addStretch();
    
    formLayout->addLayout(formButtonLayout);
    formLayout->addStretch();
    
    mainSplitter->addWidget(formPanel);
}

void QuestionManagementWidget::setupConnections()
{
    // Filter connections
    connect(subjectFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &QuestionManagementWidget::onSubjectChanged);
    connect(searchEdit, &QLineEdit::textChanged,
            this, &QuestionManagementWidget::onSearchTextChanged);
    connect(refreshButton, &QPushButton::clicked,
            this, &QuestionManagementWidget::refreshData);
    
    // Table connections
    connect(questionTable, &QTableWidget::itemSelectionChanged,
            this, &QuestionManagementWidget::onQuestionSelectionChanged);
    connect(questionTable, &QTableWidget::itemDoubleClicked,
            this, &QuestionManagementWidget::onEditQuestion);
    
    // Button connections
    connect(addButton, &QPushButton::clicked,
            this, &QuestionManagementWidget::onAddQuestion);
    connect(editButton, &QPushButton::clicked,
            this, &QuestionManagementWidget::onEditQuestion);
    connect(deleteButton, &QPushButton::clicked,
            this, &QuestionManagementWidget::onDeleteQuestion);
    
    // Form connections
    connect(saveButton, &QPushButton::clicked,
            this, &QuestionManagementWidget::onSaveQuestion);
    connect(cancelButton, &QPushButton::clicked,
            this, &QuestionManagementWidget::onCancelEdit);
    connect(clearButton, &QPushButton::clicked,
            this, &QuestionManagementWidget::onClearForm);
    
    // Form validation
    connect(questionEdit, &QTextEdit::textChanged, [this]() {
        saveButton->setEnabled(validateQuestionForm());
    });
    connect(answerAEdit, &QLineEdit::textChanged, [this]() {
        saveButton->setEnabled(validateQuestionForm());
    });
    connect(answerBEdit, &QLineEdit::textChanged, [this]() {
        saveButton->setEnabled(validateQuestionForm());
    });
    connect(answerCEdit, &QLineEdit::textChanged, [this]() {
        saveButton->setEnabled(validateQuestionForm());
    });
    connect(answerDEdit, &QLineEdit::textChanged, [this]() {
        saveButton->setEnabled(validateQuestionForm());
    });
}

void QuestionManagementWidget::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f8f9fa;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        
        QGroupBox {
            font-size: 14px;
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
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            background-color: white;
            alternate-background-color: #f8f9fa;
            selection-background-color: #3498db;
            selection-color: white;
        }
        
        QTableWidget::item {
            padding: 8px;
            border: none;
        }
        
        QHeaderView::section {
            background-color: #ecf0f1;
            color: #2c3e50;
            padding: 8px;
            border: 1px solid #bdc3c7;
            font-weight: bold;
        }
        
        QLineEdit, QTextEdit, QComboBox {
            padding: 8px;
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            font-size: 14px;
            background-color: white;
            color: #2c3e50;
        }
        
        QLineEdit:focus, QTextEdit:focus, QComboBox:focus {
            border-color: #3498db;
        }
        
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 20px;
            border-left-width: 1px;
            border-left-color: #bdc3c7;
            border-left-style: solid;
            border-top-right-radius: 3px;
            border-bottom-right-radius: 3px;
            background-color: #ecf0f1;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 6px solid #7f8c8d;
            width: 0;
            height: 0;
        }
        
        QComboBox QAbstractItemView {
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            background-color: white;
            selection-background-color: #3498db;
            selection-color: white;
            color: #2c3e50;
            padding: 4px;
        }
        
        QComboBox QAbstractItemView::item {
            padding: 8px;
            border: none;
            background-color: transparent;
            color: #2c3e50;
        }
        
        QComboBox QAbstractItemView::item:hover {
            background-color: #ecf0f1;
            color: #2c3e50;
        }
        
        QComboBox QAbstractItemView::item:selected {
            background-color: #3498db;
            color: white;
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
        
        #secondaryButton:disabled {
            background-color: #bdc3c7;
            color: #7f8c8d;
        }
        
        #deleteButton {
            background-color: #e74c3c;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 100px;
        }
        
        #deleteButton:hover {
            background-color: #c0392b;
        }
        
        #deleteButton:disabled {
            background-color: #bdc3c7;
            color: #7f8c8d;
        }
        
        #clearButton {
            background-color: #f39c12;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 100px;
        }
        
        #clearButton:hover {
            background-color: #e67e22;
        }
    )");
}

void QuestionManagementWidget::loadSubjects()
{
    subjectFilterCombo->clear();
    subjectCombo->clear();
    
    subjectFilterCombo->addItem("All Subjects", "");
    
    if (appManager && appManager->getMonHocManager()) {
        QList<MonHoc> subjects = appManager->getMonHocManager()->layDanhSachMonHoc();
        for (const MonHoc& subject : subjects) {
            QString displayText = QString("%1 - %2").arg(subject.getMaMon(), subject.getTenMon());
            subjectFilterCombo->addItem(displayText, subject.getMaMon());
            subjectCombo->addItem(displayText, subject.getMaMon());
        }
    }
}

void QuestionManagementWidget::refreshData()
{
    loadSubjects();
    loadQuestions();
}

void QuestionManagementWidget::loadQuestions()
{
    populateQuestionTable();
}

void QuestionManagementWidget::populateQuestionTable()
{
    questionTable->setRowCount(0);
    
    if (!appManager || !appManager->getMonHocManager()) {
        return;
    }
    
    QString selectedSubject = subjectFilterCombo->currentData().toString();
    QString searchText = searchEdit->text().toLower();
    
    QStringList subjects;
    if (selectedSubject.isEmpty()) {
        QList<MonHoc> allSubjects = appManager->getMonHocManager()->layDanhSachMonHoc();
        for (const MonHoc& subject : allSubjects) {
            subjects << subject.getMaMon();
        }
    } else {
        subjects << selectedSubject;
    }
    
    int row = 0;
    for (const QString& subjectCode : subjects) {
        MonHoc* subject = appManager->getMonHocManager()->timMonHoc(subjectCode);
        if (!subject) continue;
        
        QList<CauHoi> questions = subject->layDanhSachCauHoi();
        for (const CauHoi& question : questions) {
            // Apply search filter
            if (!searchText.isEmpty()) {
                if (!question.getNoiDung().toLower().contains(searchText)) {
                    continue;
                }
            }
            
            questionTable->insertRow(row);
            
            // ID
            QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(question.getId()));
            idItem->setData(Qt::UserRole, question.getId());
            questionTable->setItem(row, 0, idItem);
            
            // Subject
            QTableWidgetItem* subjectItem = new QTableWidgetItem(subjectCode);
            questionTable->setItem(row, 1, subjectItem);
            
            // Question text (truncated)
            QString questionText = question.getNoiDung();
            if (questionText.length() > 100) {
                questionText = questionText.left(97) + "...";
            }
            QTableWidgetItem* questionItem = new QTableWidgetItem(questionText);
            questionItem->setToolTip(question.getNoiDung());
            questionTable->setItem(row, 2, questionItem);
            
            // Correct answer
            QStringList answers = {"A", "B", "C", "D"};
            QString correctAnswer = (question.getDapAnDung() >= 0 && question.getDapAnDung() < 4) 
                                  ? answers[question.getDapAnDung()] : "?";
            QTableWidgetItem* answerItem = new QTableWidgetItem(correctAnswer);
            questionTable->setItem(row, 3, answerItem);
            
            row++;
        }
    }
    
    questionTable->resizeRowsToContents();
}

void QuestionManagementWidget::onSubjectChanged()
{
    currentSubjectFilter = subjectFilterCombo->currentData().toString();
    filterQuestions();
}

void QuestionManagementWidget::onSearchTextChanged()
{
    filterQuestions();
}

void QuestionManagementWidget::filterQuestions()
{
    populateQuestionTable();
}

void QuestionManagementWidget::onQuestionSelectionChanged()
{
    bool hasSelection = questionTable->currentRow() >= 0;
    editButton->setEnabled(hasSelection);
    deleteButton->setEnabled(hasSelection);
    
    if (hasSelection && !isEditing) {
        // Load selected question to form for viewing
        QTableWidgetItem* idItem = questionTable->item(questionTable->currentRow(), 0);
        if (idItem) {
            int questionId = idItem->data(Qt::UserRole).toInt();
            QString subjectCode = questionTable->item(questionTable->currentRow(), 1)->text();
            
            MonHoc* subject = appManager->getMonHocManager()->timMonHoc(subjectCode);
            if (subject) {
                CauHoi* question = subject->timCauHoi(questionId);
                if (question) {
                    loadQuestionToForm(*question);
                }
            }
        }
    }
}

void QuestionManagementWidget::onAddQuestion()
{
    isEditing = false;
    currentQuestionId = -1;
    clearQuestionForm();
    
    // Enable form for editing
    questionEdit->setEnabled(true);
    answerAEdit->setEnabled(true);
    answerBEdit->setEnabled(true);
    answerCEdit->setEnabled(true);
    answerDEdit->setEnabled(true);
    correctAnswerCombo->setEnabled(true);
    subjectCombo->setEnabled(true);
    
    saveButton->setEnabled(false);
    cancelButton->setEnabled(true);
    
    questionEdit->setFocus();
}

void QuestionManagementWidget::onEditQuestion()
{
    if (questionTable->currentRow() < 0) return;
    
    QTableWidgetItem* idItem = questionTable->item(questionTable->currentRow(), 0);
    if (!idItem) return;
    
    int questionId = idItem->data(Qt::UserRole).toInt();
    QString subjectCode = questionTable->item(questionTable->currentRow(), 1)->text();
    
    // Check if question can be edited (not used in exams)
    if (!canDeleteQuestion(questionId)) {
        QMessageBox::warning(this, "Cannot Edit", 
            "This question cannot be edited because it has been used in student exams.\n"
            "You can create a new question instead.");
        return;
    }
    
    MonHoc* subject = appManager->getMonHocManager()->timMonHoc(subjectCode);
    if (!subject) return;
    
    CauHoi* question = subject->timCauHoi(questionId);
    if (!question) return;
    
    isEditing = true;
    currentQuestionId = questionId;
    loadQuestionToForm(*question);
    
    // Enable form for editing
    questionEdit->setEnabled(true);
    answerAEdit->setEnabled(true);
    answerBEdit->setEnabled(true);
    answerCEdit->setEnabled(true);
    answerDEdit->setEnabled(true);
    correctAnswerCombo->setEnabled(true);
    subjectCombo->setEnabled(false); // Don't allow changing subject when editing
    
    saveButton->setEnabled(validateQuestionForm());
    cancelButton->setEnabled(true);
    
    questionEdit->setFocus();
}

void QuestionManagementWidget::onDeleteQuestion()
{
    if (questionTable->currentRow() < 0) return;
    
    QTableWidgetItem* idItem = questionTable->item(questionTable->currentRow(), 0);
    if (!idItem) return;
    
    int questionId = idItem->data(Qt::UserRole).toInt();
    QString subjectCode = questionTable->item(questionTable->currentRow(), 1)->text();
    
    // Check if question can be deleted
    if (!canDeleteQuestion(questionId)) {
        QMessageBox::warning(this, "Cannot Delete", 
            "This question cannot be deleted because it has been used in student exams.");
        return;
    }
    
    int ret = QMessageBox::question(this, "Delete Question",
        QString("Are you sure you want to delete question ID %1?\nThis action cannot be undone.")
        .arg(questionId),
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        MonHoc* subject = appManager->getMonHocManager()->timMonHoc(subjectCode);
        if (subject && subject->xoaCauHoi(questionId)) {
            emit questionDeleted(questionId);
            refreshData();
            clearQuestionForm();
            QMessageBox::information(this, "Success", "Question deleted successfully.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete question.");
        }
    }
}

void QuestionManagementWidget::onSaveQuestion()
{
    if (!validateQuestionForm()) {
        QMessageBox::warning(this, "Validation Error", 
            "Please fill in all required fields.");
        return;
    }
    
    if (isEditing) {
        updateQuestionInForm();
    } else {
        saveQuestionFromForm();
    }
}

void QuestionManagementWidget::onCancelEdit()
{
    isEditing = false;
    currentQuestionId = -1;
    
    // Disable form editing
    questionEdit->setEnabled(false);
    answerAEdit->setEnabled(false);
    answerBEdit->setEnabled(false);
    answerCEdit->setEnabled(false);
    answerDEdit->setEnabled(false);
    correctAnswerCombo->setEnabled(false);
    subjectCombo->setEnabled(false);
    
    saveButton->setEnabled(false);
    cancelButton->setEnabled(false);
    
    // Reload current selection if any
    onQuestionSelectionChanged();
}

void QuestionManagementWidget::onClearForm()
{
    clearQuestionForm();
}

void QuestionManagementWidget::clearQuestionForm()
{
    idEdit->clear();
    questionEdit->clear();
    answerAEdit->clear();
    answerBEdit->clear();
    answerCEdit->clear();
    answerDEdit->clear();
    correctAnswerCombo->setCurrentIndex(0);
    
    if (subjectCombo->count() > 0) {
        subjectCombo->setCurrentIndex(0);
    }
    
    // Disable form editing
    questionEdit->setEnabled(false);
    answerAEdit->setEnabled(false);
    answerBEdit->setEnabled(false);
    answerCEdit->setEnabled(false);
    answerDEdit->setEnabled(false);
    correctAnswerCombo->setEnabled(false);
    subjectCombo->setEnabled(false);
    
    saveButton->setEnabled(false);
    cancelButton->setEnabled(false);
}

void QuestionManagementWidget::loadQuestionToForm(const CauHoi& question)
{
    idEdit->setText(QString::number(question.getId()));
    questionEdit->setPlainText(question.getNoiDung());
    
    QStringList answers = question.getDapAn();
    if (answers.size() >= 4) {
        answerAEdit->setText(answers[0]);
        answerBEdit->setText(answers[1]);
        answerCEdit->setText(answers[2]);
        answerDEdit->setText(answers[3]);
    }
    
    correctAnswerCombo->setCurrentIndex(question.getDapAnDung());
    
    // Find and set subject
    // Note: We need to find which subject this question belongs to
    // This is a bit complex since we need to search through all subjects
    // For now, we'll disable subject combo when viewing/editing
}

bool QuestionManagementWidget::validateQuestionForm()
{
    return !questionEdit->toPlainText().trimmed().isEmpty() &&
           !answerAEdit->text().trimmed().isEmpty() &&
           !answerBEdit->text().trimmed().isEmpty() &&
           !answerCEdit->text().trimmed().isEmpty() &&
           !answerDEdit->text().trimmed().isEmpty() &&
           subjectCombo->currentIndex() >= 0;
}

void QuestionManagementWidget::saveQuestionFromForm()
{
    QString subjectCode = subjectCombo->currentData().toString();
    if (subjectCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a subject.");
        return;
    }
    
    MonHoc* subject = appManager->getMonHocManager()->timMonHoc(subjectCode);
    if (!subject) {
        QMessageBox::critical(this, "Error", "Selected subject not found.");
        return;
    }
    
    // Create new question
    CauHoi newQuestion;
    newQuestion.setNoiDung(questionEdit->toPlainText().trimmed());
    
    QStringList answers;
    answers << answerAEdit->text().trimmed()
            << answerBEdit->text().trimmed()
            << answerCEdit->text().trimmed()
            << answerDEdit->text().trimmed();
    newQuestion.setDapAn(answers);
    newQuestion.setDapAnDung(correctAnswerCombo->currentIndex());
    
    if (subject->themCauHoi(newQuestion)) {
        emit questionAdded(newQuestion.getId(), subjectCode);
        refreshData();
        onCancelEdit();
        QMessageBox::information(this, "Success", 
            QString("Question added successfully with ID: %1").arg(newQuestion.getId()));
    } else {
        QMessageBox::critical(this, "Error", "Failed to add question.");
    }
}

void QuestionManagementWidget::updateQuestionInForm()
{
    if (currentQuestionId < 0) return;
    
    QString subjectCode = questionTable->item(questionTable->currentRow(), 1)->text();
    MonHoc* subject = appManager->getMonHocManager()->timMonHoc(subjectCode);
    if (!subject) return;
    
    CauHoi* question = subject->timCauHoi(currentQuestionId);
    if (!question) return;
    
    // Update question
    question->setNoiDung(questionEdit->toPlainText().trimmed());
    
    QStringList answers;
    answers << answerAEdit->text().trimmed()
            << answerBEdit->text().trimmed()
            << answerCEdit->text().trimmed()
            << answerDEdit->text().trimmed();
    question->setDapAn(answers);
    question->setDapAnDung(correctAnswerCombo->currentIndex());
    
    emit questionUpdated(currentQuestionId);
    refreshData();
    onCancelEdit();
    QMessageBox::information(this, "Success", "Question updated successfully.");
}

bool QuestionManagementWidget::canDeleteQuestion(int questionId)
{
    // Check if question is used in any student exams
    if (!appManager || !appManager->getDiemThiManager()) {
        return true; // If no exam manager, allow deletion
    }
    
    // Check if this question ID appears in any exam records
    // This would require iterating through all DiemThi records
    // For now, we'll implement a basic check
    
    // TODO: Implement more sophisticated check when DiemThi has question tracking
    // For now, return true but log the check
    qDebug() << "Checking if question" << questionId << "can be deleted";
    
    return true; // Allow deletion for now
}