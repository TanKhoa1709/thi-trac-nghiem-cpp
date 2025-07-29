#include "examwidget.h"
#include "appmanager.h"
#include "cauhoi.h"
#include <QRandomGenerator>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QDateTime>
#include <QList>
#include <QString>
#include <QTime>


ExamWidget::ExamWidget(AppManager* appManager, QWidget *parent)
    : QWidget(parent)
    , appManager(appManager)
    , currentQuestionIndex(0)
    , timeRemaining(0)
    , calculatedScore(0)
    , examInProgress(false)
{
    setupUI();
    setupConnections();
    applyStyles();
    loadSubjects();
}

ExamWidget::~ExamWidget()
{
    if (examTimer) {
        examTimer->stop();
    }
}

void ExamWidget::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    stackedWidget = new QStackedWidget();
    mainLayout->addWidget(stackedWidget);
    
    setupExamSetup();
    setupExamInterface();
    setupResultInterface();
    
    // Start with setup page
    stackedWidget->setCurrentWidget(setupPage);
}

void ExamWidget::setupExamSetup()
{
    setupPage = new QWidget();
    setupLayout = new QVBoxLayout(setupPage);
    setupLayout->setSpacing(20);
    setupLayout->setAlignment(Qt::AlignCenter);
    
    // Title
    QLabel* titleLabel = new QLabel("Exam Setup");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // Setup form
    setupGroup = new QGroupBox("Configure Your Exam");
    setupFormLayout = new QGridLayout(setupGroup);
    
    setupFormLayout->addWidget(new QLabel("Subject:"), 0, 0);
    subjectCombo = new QComboBox();
    subjectCombo->setMinimumWidth(200);
    setupFormLayout->addWidget(subjectCombo, 0, 1);
    
    setupFormLayout->addWidget(new QLabel("Number of Questions:"), 1, 0);
    questionCountSpin = new QSpinBox();
    questionCountSpin->setRange(5, 50);
    questionCountSpin->setValue(10);
    setupFormLayout->addWidget(questionCountSpin, 1, 1);
    
    setupFormLayout->addWidget(new QLabel("Time Limit (minutes):"), 2, 0);
    timeLimitSpin = new QSpinBox();
    timeLimitSpin->setRange(5, 180);
    timeLimitSpin->setValue(15);
    setupFormLayout->addWidget(timeLimitSpin, 2, 1);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    startExamButton = new QPushButton("Start Exam");
    startExamButton->setObjectName("actionButton");
    viewHistoryButton = new QPushButton("View Exam History");
    viewHistoryButton->setObjectName("secondaryButton");
    
    buttonLayout->addWidget(startExamButton);
    buttonLayout->addWidget(viewHistoryButton);
    buttonLayout->addStretch();
    
    setupLayout->addWidget(titleLabel);
    setupLayout->addWidget(setupGroup);
    setupLayout->addLayout(buttonLayout);
    setupLayout->addStretch();
    
    stackedWidget->addWidget(setupPage);
}

void ExamWidget::setupExamInterface()
{
    examPage = new QWidget();
    examLayout = new QVBoxLayout(examPage);
    examLayout->setSpacing(15);
    
    // Exam info section
    examInfoGroup = new QGroupBox("Exam Information");
    QHBoxLayout* infoLayout = new QHBoxLayout(examInfoGroup);
    
    examInfoLabel = new QLabel();
    progressBar = new QProgressBar();
    progressBar->setMinimumWidth(200);
    timerLabel = new QLabel("Time: 15:00");
    timerLabel->setObjectName("timerLabel");
    
    infoLayout->addWidget(examInfoLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(progressBar);
    infoLayout->addWidget(timerLabel);
    
    // Question section
    questionGroup = new QGroupBox("Question");
    questionLayout = new QVBoxLayout(questionGroup);
    
    questionScrollArea = new QScrollArea();
    questionScrollArea->setWidgetResizable(true);
    questionScrollArea->setFrameShape(QFrame::NoFrame);
    
    questionContent = new QWidget();
    questionContentLayout = new QVBoxLayout(questionContent);
    
    questionNumberLabel = new QLabel();
    questionNumberLabel->setObjectName("questionNumber");
    questionTextLabel = new QLabel();
    questionTextLabel->setWordWrap(true);
    questionTextLabel->setObjectName("questionText");
    
    // Answer options
    answerGroup = new QButtonGroup(this);
    answerA = new QRadioButton();
    answerB = new QRadioButton();
    answerC = new QRadioButton();
    answerD = new QRadioButton();
    
    answerGroup->addButton(answerA, 0);
    answerGroup->addButton(answerB, 1);
    answerGroup->addButton(answerC, 2);
    answerGroup->addButton(answerD, 3);
    
    questionContentLayout->addWidget(questionNumberLabel);
    questionContentLayout->addWidget(questionTextLabel);
    questionContentLayout->addSpacing(20);
    questionContentLayout->addWidget(answerA);
    questionContentLayout->addWidget(answerB);
    questionContentLayout->addWidget(answerC);
    questionContentLayout->addWidget(answerD);
    questionContentLayout->addStretch();
    
    questionScrollArea->setWidget(questionContent);
    questionLayout->addWidget(questionScrollArea);
    
    // Navigation buttons
    navigationLayout = new QHBoxLayout();
    previousButton = new QPushButton("Previous");
    previousButton->setObjectName("secondaryButton");
    nextButton = new QPushButton("Next");
    nextButton->setObjectName("actionButton");
    submitButton = new QPushButton("Submit Exam");
    submitButton->setObjectName("submitButton");
    
    navigationLayout->addWidget(previousButton);
    navigationLayout->addStretch();
    navigationLayout->addWidget(nextButton);
    navigationLayout->addWidget(submitButton);
    
    examLayout->addWidget(examInfoGroup);
    examLayout->addWidget(questionGroup);
    examLayout->addLayout(navigationLayout);
    
    stackedWidget->addWidget(examPage);
}

void ExamWidget::setupResultInterface()
{
    resultPage = new QWidget();
    resultLayout = new QVBoxLayout(resultPage);
    resultLayout->setAlignment(Qt::AlignCenter);
    resultLayout->setSpacing(20);
    
    QLabel* resultTitle = new QLabel("Exam Results");
    resultTitle->setObjectName("titleLabel");
    resultTitle->setAlignment(Qt::AlignCenter);
    
    resultGroup = new QGroupBox("Your Score");
    QVBoxLayout* resultGroupLayout = new QVBoxLayout(resultGroup);
    
    scoreLabel = new QLabel();
    scoreLabel->setObjectName("scoreLabel");
    scoreLabel->setAlignment(Qt::AlignCenter);
    
    detailsLabel = new QLabel();
    detailsLabel->setAlignment(Qt::AlignCenter);
    detailsLabel->setWordWrap(true);
    
    resultGroupLayout->addWidget(scoreLabel);
    resultGroupLayout->addWidget(detailsLabel);
    
    QHBoxLayout* resultButtonLayout = new QHBoxLayout();
    newExamButton = new QPushButton("Take Another Exam");
    newExamButton->setObjectName("actionButton");
    backButton = new QPushButton("Back to Dashboard");
    backButton->setObjectName("secondaryButton");
    
    resultButtonLayout->addWidget(newExamButton);
    resultButtonLayout->addWidget(backButton);
    
    resultLayout->addWidget(resultTitle);
    resultLayout->addWidget(resultGroup);
    resultLayout->addLayout(resultButtonLayout);
    resultLayout->addStretch();
    
    stackedWidget->addWidget(resultPage);
}

void ExamWidget::setupConnections()
{
    connect(startExamButton, &QPushButton::clicked, this, &ExamWidget::onStartExam);
    connect(viewHistoryButton, &QPushButton::clicked, this, &ExamWidget::viewExamHistory);
    
    connect(previousButton, &QPushButton::clicked, this, &ExamWidget::onPreviousQuestion);
    connect(nextButton, &QPushButton::clicked, this, &ExamWidget::onNextQuestion);
    connect(submitButton, &QPushButton::clicked, this, &ExamWidget::onSubmitExam);
    
    connect(answerGroup, &QButtonGroup::buttonClicked, 
            [this](QAbstractButton* button) {
                Q_UNUSED(button);
                onQuestionAnswered();
            });
    
    connect(newExamButton, &QPushButton::clicked, this, &ExamWidget::onBackToSetup);
    connect(backButton, &QPushButton::clicked, this, &ExamWidget::onBackToSetup);
    
    // Timer
    examTimer = new QTimer(this);
    connect(examTimer, &QTimer::timeout, this, &ExamWidget::onTimerTick);
}

void ExamWidget::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f8f9fa;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        
        #titleLabel {
            font-size: 28px;
            font-weight: bold;
            color: #2c3e50;
            margin: 20px;
        }
        
        #questionNumber {
            font-size: 18px;
            font-weight: bold;
            color: #3498db;
            margin-bottom: 10px;
        }
        
        #questionText {
            font-size: 16px;
            color: #2c3e50;
            margin-bottom: 20px;
            line-height: 1.5;
        }
        
        #timerLabel {
            font-size: 16px;
            font-weight: bold;
            color: #e74c3c;
            padding: 8px 16px;
            background-color: white;
            border: 2px solid #e74c3c;
            border-radius: 4px;
        }
        
        #scoreLabel {
            font-size: 36px;
            font-weight: bold;
            color: #27ae60;
            margin: 20px;
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
        
        QRadioButton {
            font-size: 14px;
            color: #2c3e50;
            padding: 8px;
            margin: 4px 0;
        }
        
        QRadioButton::indicator {
            width: 18px;
            height: 18px;
        }
        
        QRadioButton::indicator:unchecked {
            border: 2px solid #bdc3c7;
            border-radius: 9px;
            background-color: white;
        }
        
        QRadioButton::indicator:checked {
            border: 2px solid #3498db;
            border-radius: 9px;
            background-color: #3498db;
        }
        
        QComboBox, QSpinBox {
            padding: 8px;
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            font-size: 14px;
            background-color: white;
            min-width: 150px;
            color: #2c3e50;
        }
        
        QComboBox:focus {
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
        
        QProgressBar {
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            text-align: center;
            font-weight: bold;
        }
        
        QProgressBar::chunk {
            background-color: #3498db;
            border-radius: 2px;
        }
        
        #actionButton {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 12px 24px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 120px;
        }
        
        #actionButton:hover {
            background-color: #2980b9;
        }
        
        #secondaryButton {
            background-color: #95a5a6;
            color: white;
            border: none;
            padding: 12px 24px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 120px;
        }
        
        #secondaryButton:hover {
            background-color: #7f8c8d;
        }
        
        #submitButton {
            background-color: #e74c3c;
            color: white;
            border: none;
            padding: 12px 24px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 120px;
        }
        
        #submitButton:hover {
            background-color: #c0392b;
        }
    )");
}

void ExamWidget::loadSubjects()
{
    // Load actual subjects from AppManager
    subjectCombo->clear();
    
    if (appManager && appManager->getMonHocManager()) {
        // TODO: Get actual subjects from MonHocManager
        QStringList subjects;
        
        // TODO: This should be QList<MonHoc> from MonHocManager
        // For now, using placeholder since subjects is QStringList
        for (const QString& subject : subjects) {
            // Assuming format "CODE - NAME"
            QStringList parts = subject.split(" - ");
            if (parts.size() >= 2) {
                QString subjectCode = parts[0];
                QString subjectName = parts[1];
                QString displayText = QString("%1 - %2").arg(subjectCode, subjectName);
                subjectCombo->addItem(displayText, subjectCode);
            }
        }
    } else {
        // Fallback to sample data if AppManager not available
        subjectCombo->addItem("CTDL&GT - Data Structures & Algorithms", "CTDLGT");
        subjectCombo->addItem("OOP - Object-Oriented Programming", "OOP");
        subjectCombo->addItem("DB - Database Systems", "DB");
        subjectCombo->addItem("SE - Software Engineering", "SE");
        subjectCombo->addItem("AI - Artificial Intelligence", "AI");
    }
}

void ExamWidget::onStartExam()
{
    if (subjectCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a subject.");
        return;
    }
    
    currentSubject = subjectCombo->currentText();
    totalQuestions = questionCountSpin->value();
    timeLimit = timeLimitSpin->value();
    timeRemaining = timeLimit * 60; // Convert to seconds
    
    generateExamQuestions();
    
    examInfoLabel->setText(QString("Subject: %1 | Questions: %2 | Time: %3 minutes")
                          .arg(currentSubject).arg(totalQuestions).arg(timeLimit));
    
    progressBar->setMaximum(totalQuestions);
    progressBar->setValue(0);
    
    currentQuestionIndex = 0;
    examInProgress = true;
    
    // Initialize user answers
    userAnswers.clear();
    for (int i = 0; i < totalQuestions; ++i) {
        userAnswers.append("");
    }
    
    showQuestion(0);
    updateProgress();
    
    // Start timer
    examTimer->start(1000); // Update every second
    
    stackedWidget->setCurrentWidget(examPage);
}

void ExamWidget::generateExamQuestions()
{
    // Generate actual random questions from the selected subject
    examQuestions.clear();
    correctAnswers.clear();
    
    QString selectedSubjectCode = subjectCombo->currentData().toString();
    if (selectedSubjectCode.isEmpty()) {
        // Fallback to sample questions
        for (int i = 0; i < totalQuestions; ++i) {
            examQuestions.append(QString("Sample question %1 for %2?").arg(i + 1).arg(currentSubject));
            int randomAnswer = QRandomGenerator::global()->bounded(4);
            correctAnswers.append(QString::number(randomAnswer));
        }
        return;
    }
    
    if (appManager && appManager->getCauHoiManager()) {
        // TODO: Get actual questions from CauHoiManager
        QList<CauHoi> questions = appManager->getCauHoiManager()->layCauHoiTheoMon(selectedSubjectCode);
        
        if (questions.size() < totalQuestions) {
            qDebug() << "Insufficient questions for subject" << selectedSubjectCode;
            // Fill with available questions and pad with samples if needed
            int availableQuestions = qMin(questions.size(), totalQuestions);
            
            for (int i = 0; i < availableQuestions; ++i) {
                const CauHoi& question = questions[i];
                examQuestions.append(question.getNoiDung());
                correctAnswers.append(QString(question.getDapAnDung()));
            }
            
            // Pad with sample questions if needed
            for (int i = availableQuestions; i < totalQuestions; ++i) {
                examQuestions.append(QString("Sample question %1 for %2?").arg(i + 1).arg(currentSubject));
                correctAnswers.append(QString::number(i % 4));
            }
        } else {
            // Randomly select questions
            QList<int> selectedIndices;
            while (selectedIndices.size() < totalQuestions) {
                int randomIndex = QRandomGenerator::global()->bounded(questions.size());
                if (!selectedIndices.contains(randomIndex)) {
                    selectedIndices.append(randomIndex);
                    
                    const CauHoi& question = questions[randomIndex];
                    examQuestions.append(question.getNoiDung());
                    correctAnswers.append(QString(question.getDapAnDung()));
                }
            }
        }
    } else {
        // Fallback: Sample questions for demonstration
        for (int i = 0; i < totalQuestions; ++i) {
            examQuestions.append(QString("Sample question %1 for %2?").arg(i + 1).arg(currentSubject));
            int randomAnswer = QRandomGenerator::global()->bounded(4);
            correctAnswers.append(QString::number(randomAnswer));
        }
    }
}

void ExamWidget::showQuestion(int questionIndex)
{
    if (questionIndex < 0 || questionIndex >= examQuestions.size()) return;
    
    questionNumberLabel->setText(QString("Question %1 of %2").arg(questionIndex + 1).arg(totalQuestions));
    questionTextLabel->setText(examQuestions[questionIndex]);
    
    // Sample answer options
    answerA->setText("A. Sample answer A");
    answerB->setText("B. Sample answer B");
    answerC->setText("C. Sample answer C");
    answerD->setText("D. Sample answer D");
    
    // Restore previous answer if any
    answerGroup->setExclusive(false);
    answerA->setChecked(false);
    answerB->setChecked(false);
    answerC->setChecked(false);
    answerD->setChecked(false);
    answerGroup->setExclusive(true);
    
    if (!userAnswers[questionIndex].isEmpty()) {
        int answerIndex = userAnswers[questionIndex].toInt();
        if (answerIndex >= 0 && answerIndex < 4) {
            answerGroup->button(answerIndex)->setChecked(true);
        }
    }
    
    // Update navigation buttons
    previousButton->setEnabled(questionIndex > 0);
    nextButton->setEnabled(questionIndex < totalQuestions - 1);
}

void ExamWidget::onNextQuestion()
{
    if (currentQuestionIndex < totalQuestions - 1) {
        currentQuestionIndex++;
        showQuestion(currentQuestionIndex);
        updateProgress();
    }
}

void ExamWidget::onPreviousQuestion()
{
    if (currentQuestionIndex > 0) {
        currentQuestionIndex--;
        showQuestion(currentQuestionIndex);
        updateProgress();
    }
}

void ExamWidget::onQuestionAnswered()
{
    int selectedAnswer = answerGroup->checkedId();
    if (selectedAnswer >= 0) {
        userAnswers[currentQuestionIndex] = QString::number(selectedAnswer);
        updateProgress();
    }
}

void ExamWidget::updateProgress()
{
    int answeredCount = 0;
    for (const QString& answer : userAnswers) {
        if (!answer.isEmpty()) {
            answeredCount++;
        }
    }
    progressBar->setValue(answeredCount);
}

void ExamWidget::onTimerTick()
{
    timeRemaining--;
    
    int minutes = timeRemaining / 60;
    int seconds = timeRemaining % 60;
    timerLabel->setText(QString("Time: %1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
    
    if (timeRemaining <= 0) {
        onTimeUp();
    }
}

void ExamWidget::onTimeUp()
{
    examTimer->stop();
    QMessageBox::information(this, "Time Up", "Time is up! Your exam will be submitted automatically.");
    onSubmitExam();
}

void ExamWidget::onSubmitExam()
{
    if (examInProgress) {
        int ret = QMessageBox::question(this, "Submit Exam",
            "Are you sure you want to submit your exam?\nThis action cannot be undone.",
            QMessageBox::Yes | QMessageBox::No);
            
        if (ret == QMessageBox::No) {
            return;
        }
    }
    
    examTimer->stop();
    examInProgress = false;
    
    calculateResults();
    showResults();
}

void ExamWidget::calculateResults()
{
    // Calculate actual results based on correct answers
    int correctCount = 0;
    
    for (int i = 0; i < totalQuestions && i < userAnswers.size() && i < correctAnswers.size(); ++i) {
        if (!userAnswers[i].isEmpty() && userAnswers[i] == correctAnswers[i]) {
            correctCount++;
        }
    }
    
    // Store the calculated score for use in showResults()
    calculatedScore = correctCount;
}

void ExamWidget::showResults()
{
    // Use the calculated score
    int score = calculatedScore;
    
    scoreLabel->setText(QString("%1/%2").arg(score).arg(totalQuestions));
    
    double percentage = (double)score / totalQuestions * 100;
    QString grade = percentage >= 80 ? "Excellent" : 
                   percentage >= 70 ? "Good" : 
                   percentage >= 60 ? "Average" : "Needs Improvement";
    
    detailsLabel->setText(QString("Subject: %1\nScore: %2%%\nGrade: %3\nTime taken: %4 minutes")
                         .arg(currentSubject)
                         .arg(percentage, 0, 'f', 1)
                         .arg(grade)
                         .arg(timeLimit - (timeRemaining / 60)));
    
    emit examCompleted(currentSubject, score, totalQuestions);
    
    stackedWidget->setCurrentWidget(resultPage);
}

void ExamWidget::onBackToSetup()
{
    stackedWidget->setCurrentWidget(setupPage);
}

void ExamWidget::startNewExam()
{
    stackedWidget->setCurrentWidget(setupPage);
}

void ExamWidget::viewExamHistory()
{
    // Show exam history from AppManager
    if (!appManager) {
        QMessageBox::information(this, "Exam History", 
            "Exam history functionality requires AppManager connection.\n\nThis will show all previous exam results with detailed scores and review options.");
        return;
    }
    
    // Create and show detailed exam history dialog
    QDialog* historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("Lịch sử thi chi tiết");
    historyDialog->setModal(true);
    historyDialog->resize(800, 600);
    
    QVBoxLayout* layout = new QVBoxLayout(historyDialog);
    
    // Create table for exam history
    QTableWidget* historyTable = new QTableWidget();
    historyTable->setColumnCount(6);
    QStringList headers = {"Môn học", "Ngày thi", "Điểm", "Số câu đúng", "Tổng câu", "Thời gian"};
    historyTable->setHorizontalHeaderLabels(headers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setAlternatingRowColors(true);
    historyTable->horizontalHeader()->setStretchLastSection(true);
    
    // TODO: Get actual exam history from AppManager
    // For now, add sample data
    QList<QStringList> sampleHistory = {
        {"CTDL&GT", "15/11/2024 14:30", "8.5", "17", "20", "25 phút"},
        {"Toán rời rạc", "10/11/2024 09:00", "7.0", "14", "20", "30 phút"},
        {"Lập trình C++", "05/11/2024 16:15", "9.0", "18", "20", "28 phút"}
    };
    
    historyTable->setRowCount(sampleHistory.size());
    for (int i = 0; i < sampleHistory.size(); ++i) {
        const QStringList& exam = sampleHistory[i];
        for (int j = 0; j < exam.size() && j < historyTable->columnCount(); ++j) {
            historyTable->setItem(i, j, new QTableWidgetItem(exam[j]));
        }
    }
    
    // Add details section
    QTextEdit* detailsText = new QTextEdit();
    detailsText->setReadOnly(true);
    detailsText->setMaximumHeight(150);
    detailsText->setPlainText("Chọn một bài thi để xem chi tiết câu hỏi và đáp án...");
    
    // Connect table selection to show details
    connect(historyTable, &QTableWidget::itemSelectionChanged, [detailsText, historyTable]() {
        int row = historyTable->currentRow();
        if (row >= 0) {
            QString subject = historyTable->item(row, 0)->text();
            QString details = QString("Chi tiết bài thi môn: %1\n\n").arg(subject);
            details += "Câu hỏi và đáp án:\n";
            details += "1. Câu hỏi mẫu 1? → Đáp án: A (Đúng)\n";
            details += "2. Câu hỏi mẫu 2? → Đáp án: B (Sai, đáp án đúng: C)\n";
            details += "3. Câu hỏi mẫu 3? → Đáp án: D (Đúng)\n";
            details += "...\n\nGhi chú: Đây là dữ liệu mẫu. Tích hợp với AppManager để hiển thị dữ liệu thực.";
            detailsText->setPlainText(details);
        }
    });
    
    // Add buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* exportBtn = new QPushButton("Xuất PDF");
    QPushButton* closeBtn = new QPushButton("Đóng");
    
    connect(exportBtn, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "Xuất PDF", "Chức năng xuất PDF sẽ được triển khai sau.");
    });
    connect(closeBtn, &QPushButton::clicked, historyDialog, &QDialog::accept);
    
    buttonLayout->addWidget(exportBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    
    layout->addWidget(new QLabel("Lịch sử thi của bạn:"));
    layout->addWidget(historyTable);
    layout->addWidget(new QLabel("Chi tiết bài thi:"));
    layout->addWidget(detailsText);
    layout->addLayout(buttonLayout);
    
    historyDialog->exec();
    historyDialog->deleteLater();
}
