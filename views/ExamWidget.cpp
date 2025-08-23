#include "ExamWidget.h"
#include "../models/MonHoc.h"
#include "../models/CauHoi.h"
#include "../models/SinhVien.h"
#include "../models/DiemThi.h"
#include "../managers/QuanLyCauHoi.h"
#include "../managers/QuanLyDiem.h"
#include "../managers/QuanLyThi.h"
#include "../utils/DynamicArray.h"
#include <QCloseEvent>
#include <QGridLayout>
#include <QSpacerItem>
#include <QButtonGroup>
#include <QBoxLayout>
#include <cstdlib>
#include <ctime>
#include <iostream>

ExamWidget::ExamWidget(QWidget *parent) : QDialog(parent), currentSubject(nullptr), currentStudent(nullptr),
                                          questions(nullptr), studentAnswers(nullptr),
                                          currentQuestionIndex(0), totalQuestions(0),
                                          timeRemaining(3600) // 60 minutes default
                                          ,
                                          totalTime(3600) {
    setModal(true);
    setWindowTitle("Exam - Multiple Choice Test");
    resize(1000, 700);

    setupUI();
    setupConnections();

    // Initialize random seed for question selection
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

ExamWidget::~ExamWidget() {
    if (questions) {
        delete questions;
    }
    if (studentAnswers) {
        delete studentAnswers;
    }
}

void ExamWidget::setupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Left side - Exam content
    QVBoxLayout *examLayout = new QVBoxLayout();

    // Header section
    QHBoxLayout *headerLayout = new QHBoxLayout();

    titleLabel = new QLabel("Exam Title");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50;");

    timerLabel = new QLabel("Time: 60:00");
    timerLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #e74c3c; "
        "background-color: #ecf0f1; padding: 8px 12px; border-radius: 4px;");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(timerLabel);

    // Progress bar
    progressBar = new QProgressBar();
    progressBar->setStyleSheet("QProgressBar { border: 2px solid #bdc3c7; border-radius: 5px; "
        "text-align: center; } "
        "QProgressBar::chunk { background-color: #3498db; }");

    // Question display area
    questionScrollArea = new QScrollArea();
    questionScrollArea->setWidgetResizable(true);
    questionScrollArea->setMinimumHeight(400);

    QWidget *questionWidget = new QWidget();
    QVBoxLayout *questionLayout = new QVBoxLayout(questionWidget);

    questionNumberLabel = new QLabel("Question 1 of 10");
    questionNumberLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #34495e; margin-bottom: 10px;");

    questionContentLabel = new QLabel("Question content will appear here...");
    questionContentLabel->setWordWrap(true);
    questionContentLabel->setStyleSheet("font-size: 14px; color: #2c3e50; padding: 15px; "
        "background-color: #f8f9fa; border: 1px solid #dee2e6; "
        "border-radius: 6px; margin-bottom: 20px;");

    // Answer options
    answerGroup = new QButtonGroup();

    optionA = new QRadioButton("A. Option A");
    optionB = new QRadioButton("B. Option B");
    optionC = new QRadioButton("C. Option C");
    optionD = new QRadioButton("D. Option D");

    answerGroup->addButton(optionA, 0);
    answerGroup->addButton(optionB, 1);
    answerGroup->addButton(optionC, 2);
    answerGroup->addButton(optionD, 3);

    // Style radio buttons
    QString radioStyle = "QRadioButton { font-size: 13px; color: #2c3e50; padding: 8px; "
            "margin: 4px 0px; } "
            "QRadioButton::indicator { width: 18px; height: 18px; } "
            "QRadioButton::indicator:unchecked { border: 2px solid #bdc3c7; "
            "border-radius: 9px; background-color: white; } "
            "QRadioButton::indicator:checked { border: 2px solid #3498db; "
            "border-radius: 9px; background-color: #3498db; }";

    optionA->setStyleSheet(radioStyle);
    optionB->setStyleSheet(radioStyle);
    optionC->setStyleSheet(radioStyle);
    optionD->setStyleSheet(radioStyle);

    questionLayout->addWidget(questionNumberLabel);
    questionLayout->addWidget(questionContentLabel);
    questionLayout->addWidget(optionA);
    questionLayout->addWidget(optionB);
    questionLayout->addWidget(optionC);
    questionLayout->addWidget(optionD);
    questionLayout->addStretch();

    questionScrollArea->setWidget(questionWidget);

    // Navigation buttons
    QHBoxLayout *navButtonLayout = new QHBoxLayout();

    previousButton = new QPushButton("← Previous");
    nextButton = new QPushButton("Next →");
    submitButton = new QPushButton("Submit Exam");
    cancelButton = new QPushButton("Cancel");

    previousButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; "
        "padding: 8px 16px; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #7f8c8d; }"
        "QPushButton:disabled { background-color: #bdc3c7; }");

    nextButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; "
        "padding: 8px 16px; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #2980b9; }");

    submitButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; "
        "padding: 8px 16px; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #229954; }");

    cancelButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; "
        "padding: 8px 16px; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #c0392b; }");

    navButtonLayout->addWidget(previousButton);
    navButtonLayout->addWidget(nextButton);
    navButtonLayout->addStretch();
    navButtonLayout->addWidget(submitButton);
    navButtonLayout->addWidget(cancelButton);

    // Add to exam layout
    examLayout->addLayout(headerLayout);
    examLayout->addWidget(progressBar);
    examLayout->addWidget(questionScrollArea);
    examLayout->addLayout(navButtonLayout);

    // Right side - Navigation panel
    createNavigationPanel();

    // Add to main layout
    mainLayout->addLayout(examLayout, 3);
    mainLayout->addWidget(navigationPanel, 1);

    // Setup timer
    examTimer = new QTimer(this);
    connect(examTimer, &QTimer::timeout, this, &ExamWidget::updateTimer);
}

void ExamWidget::setupConnections() {
    connect(previousButton, &QPushButton::clicked, this, &ExamWidget::previousQuestion);
    connect(nextButton, &QPushButton::clicked, this, &ExamWidget::nextQuestion);
    connect(submitButton, &QPushButton::clicked, this, &ExamWidget::submitExam);
    connect(cancelButton, &QPushButton::clicked, this, &ExamWidget::reject);

    // Answer selection
    connect(optionA, &QRadioButton::clicked, this, &ExamWidget::onAnswerSelected);
    connect(optionB, &QRadioButton::clicked, this, &ExamWidget::onAnswerSelected);
    connect(optionC, &QRadioButton::clicked, this, &ExamWidget::onAnswerSelected);
    connect(optionD, &QRadioButton::clicked, this, &ExamWidget::onAnswerSelected);
}

void ExamWidget::createNavigationPanel() {
    navigationPanel = new QFrame();
    navigationPanel->setFrameStyle(QFrame::Box);
    navigationPanel->setStyleSheet("QFrame { background-color: #f8f9fa; border: 1px solid #dee2e6; }");
    navigationPanel->setMaximumWidth(200);

    navLayout = new QVBoxLayout(navigationPanel);

    QLabel *navTitle = new QLabel("Question Navigation");
    navTitle->setAlignment(Qt::AlignCenter);
    navTitle->setStyleSheet("font-weight: bold; color: #2c3e50; margin: 10px;");

    navLayout->addWidget(navTitle);
    navLayout->addStretch();
}

void ExamWidget::startExam(MonHoc *subject, int numQuestions, SinhVien *student) {
    if (!subject || !student || !subject->getQuanLyCauHoi()) {
        QMessageBox::critical(this, "Error", "Invalid exam parameters.");
        reject();
        return;
    }

    currentSubject = subject;
    currentStudent = student;
    totalQuestions = numQuestions;
    currentQuestionIndex = 0;

    // Setup timer (60 minutes default)
    setupTimer(60);

    // Update title
    titleLabel->setText(QString("Exam: %1").arg(QString::fromStdString(subject->getTenMon())));
    setWindowTitle(QString("Exam - %1").arg(QString::fromStdString(subject->getTenMon())));

    // Get random questions from BST
    questions = new DynamicArray<CauHoi *>();
    subject->getQuanLyCauHoi()->layNgauNhien(*questions, numQuestions);

    if (questions->size() == 0) {
        QMessageBox::critical(this, "Error", "No questions available for this exam.");
        reject();
        return;
    }

    // Initialize answers array
    studentAnswers = new DynamicArray<char>();
    for (int i = 0; i < questions->size(); i++) {
        char empty = ' ';
        studentAnswers->add(empty); // Empty answer initially
    }

    // Update navigation panel
    updateQuestionNavigation();

    // Load first question
    loadQuestion(0);

    // Start timer
    examTimer->start(1000); // Update every second

    updateProgress();
}

void ExamWidget::setupTimer(int minutes) {
    totalTime = minutes * 60;
    timeRemaining = totalTime;
    timerLabel->setText(formatTime(timeRemaining));
}

void ExamWidget::loadQuestion(int index) {
    if (!questions || index < 0 || index >= questions->size()) {
        return;
    }

    currentQuestionIndex = index;
    CauHoi *question = questions->get(index);

    // Update question display
    questionNumberLabel->setText(QString("Question %1 of %2").arg(index + 1).arg(questions->size()));
    questionContentLabel->setText(QString::fromStdString(question->getNoiDung()));

    // Update answer options
    optionA->setText(QString("A. %1").arg(QString::fromStdString(question->getLuaChonA())));
    optionB->setText(QString("B. %1").arg(QString::fromStdString(question->getLuaChonB())));
    optionC->setText(QString("C. %1").arg(QString::fromStdString(question->getLuaChonC())));
    optionD->setText(QString("D. %1").arg(QString::fromStdString(question->getLuaChonD())));

    // Load previous answer if exists
    if (studentAnswers &&index<studentAnswers->size()) {
        setSelectedAnswer(studentAnswers->get(index));
    } else {
        answerGroup->setExclusive(false);
        optionA->setChecked(false);
        optionB->setChecked(false);
        optionC->setChecked(false);
        optionD->setChecked(false);
        answerGroup->setExclusive(true);
    }

    // Update navigation buttons
    previousButton->setEnabled(index > 0);
    nextButton->setEnabled(index < questions->size() - 1);

    updateProgress();
}

void ExamWidget::saveCurrentAnswer() {
    if (!studentAnswers || currentQuestionIndex >= studentAnswers->size()) {
        return;
    }

    char answer = getSelectedAnswer();
    studentAnswers->set(currentQuestionIndex, answer);
    updateQuestionNavigation();
}

void ExamWidget::nextQuestion() {
    saveCurrentAnswer();
    if (currentQuestionIndex < questions->size() - 1) {
        loadQuestion(currentQuestionIndex + 1);
    }
}

void ExamWidget::previousQuestion() {
    saveCurrentAnswer();
    if (currentQuestionIndex > 0) {
        loadQuestion(currentQuestionIndex - 1);
    }
}

void ExamWidget::submitExam() {
    if (!confirmSubmission()) {
        return;
    }

    saveCurrentAnswer();
    examTimer->stop();

    calculateAndSaveResults();
    accept();
}

void ExamWidget::updateTimer() {
    timeRemaining--;
    timerLabel->setText(formatTime(timeRemaining));

    // Change color when time is running low (last 5 minutes)
    if (timeRemaining <= 300) {
        timerLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: white; "
            "background-color: #e74c3c; padding: 8px 12px; border-radius: 4px;");
    }

    // Auto-submit when time runs out
    if (timeRemaining <= 0) {
        examTimer->stop();
        QMessageBox::information(this, "Time Up", "Time has expired. Your exam will be submitted automatically.");
        saveCurrentAnswer();
        calculateAndSaveResults();
        accept();
    }
}

void ExamWidget::onAnswerSelected() {
    // Save answer immediately when selected
    saveCurrentAnswer();
    updateProgress();
}

void ExamWidget::calculateAndSaveResults() {
    if (!questions || !studentAnswers) {
        emit examCompleted(0.0);
        return;
    }

    double score = QuanLyThi::calculateScore(*questions, *studentAnswers);

    DiemThi *examResult = QuanLyThi::createExamResult(
        currentSubject->getMaMon(),
        score,
        *studentAnswers,
        *questions
    );

    QuanLyThi::saveExamResult(currentStudent, examResult);

    delete examResult;
    emit examCompleted(score);
}

void ExamWidget::updateQuestionNavigation() {
    // Clear existing buttons
    while (navLayout->count() > 2) {
        // Keep title and stretch
        QLayoutItem *item = navLayout->takeAt(1);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    if (!questions)
        return;

    // Create grid layout for question buttons
    QGridLayout *gridLayout = new QGridLayout();

    for (int i = 0; i < questions->size(); i++) {
        QPushButton *questionBtn = new QPushButton(QString::number(i + 1));
        questionBtn->setFixedSize(30, 30);

        // Color coding based on answer status
        if (i < studentAnswers->size() && studentAnswers->get(i) != ' ') {
            questionBtn->setStyleSheet(
                "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 4px; }");
        } else {
            questionBtn->setStyleSheet(
                "QPushButton { background-color: #ecf0f1; color: #2c3e50; border: 1px solid #bdc3c7; border-radius: 4px; }");
        }

        // Highlight current question
        if (i == currentQuestionIndex) {
            questionBtn->setStyleSheet(
                "QPushButton { background-color: #3498db; color: white; border: none; border-radius: 4px; }");
        }

        connect(questionBtn, &QPushButton::clicked, [this, i]() { goToQuestion(i); });

        gridLayout->addWidget(questionBtn, i / 5, i % 5);
    }

    QWidget *gridWidget = new QWidget();
    gridWidget->setLayout(gridLayout);

    navLayout->insertWidget(1, gridWidget);
}

void ExamWidget::updateProgress() {
    if (!questions || !studentAnswers)
        return;

    int answeredQuestions = QuanLyThi::countAnsweredQuestions(*studentAnswers);
    int percentage = QuanLyThi::calculatePercentage(answeredQuestions, questions->size());

    progressBar->setValue(percentage);
    progressBar->setFormat(QString("Progress: %1/%2 questions answered (%p%)")
        .arg(answeredQuestions)
        .arg(questions->size()));
}

void ExamWidget::goToQuestion(int questionIndex) {
    if (questionIndex >= 0 && questionIndex < questions->size()) {
        saveCurrentAnswer();
        loadQuestion(questionIndex);
    }
}

char ExamWidget::getSelectedAnswer() {
    if (optionA->isChecked())
        return 'A';
    if (optionB->isChecked())
        return 'B';
    if (optionC->isChecked())
        return 'C';
    if (optionD->isChecked())
        return 'D';
    return ' '; // No answer selected
}

void ExamWidget::setSelectedAnswer(char answer) {
    answerGroup->setExclusive(false);
    optionA->setChecked(answer == 'A');
    optionB->setChecked(answer == 'B');
    optionC->setChecked(answer == 'C');
    optionD->setChecked(answer == 'D');
    answerGroup->setExclusive(true);
}

QString ExamWidget::formatTime(int seconds) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));
}

bool ExamWidget::confirmSubmission() {
    int unanswered = 0;
    if (studentAnswers) {
        for (int i = 0; i < studentAnswers->size(); i++) {
            if (studentAnswers->get(i) == ' ') {
                unanswered++;
            }
        }
    }

    QString message = "Are you sure you want to submit your exam?";
    if (unanswered > 0) {
        message += QString("\n\nYou have %1 unanswered questions.").arg(unanswered);
    }

    return QMessageBox::question(this, "Submit Exam", message,
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

bool ExamWidget::confirmCancellation() {
    return QMessageBox::question(this, "Cancel Exam",
                                 "Are you sure you want to cancel the exam? All progress will be lost.",
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

void ExamWidget::closeEvent(QCloseEvent *event) {
    if (examTimer &&examTimer
    ->
    isActive()
    ) {
        if (confirmCancellation()) {
            examTimer->stop();
            emit examCancelled();
            event->accept();
        } else {
            event->ignore();
        }
    }
    else {
        event->accept();
    }
}

void ExamWidget::resetExam() {
    if (questions) {
        delete questions;
        questions = nullptr;
    }
    if (studentAnswers) {
        delete studentAnswers;
        studentAnswers = nullptr;
    }

    currentQuestionIndex = 0;
    totalQuestions = 0;
    currentSubject = nullptr;
    currentStudent = nullptr;

    if (examTimer) {
        examTimer->stop();
    }
}