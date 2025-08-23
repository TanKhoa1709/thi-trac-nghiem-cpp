#include "DetailedResultsWidget.h"
#include "../models/DiemThi.h"
#include "../models/MonHoc.h"
#include "../models/CauHoi.h"
#include "../managers/QuanLyMonHoc.h"
#include "../managers/QuanLyCauHoi.h"
#include "../managers/ThongKe.h"
#include "../managers/QuanLyThi.h"
#include "../utils/DynamicArray.h"
#include <QCloseEvent>
#include <QGridLayout>
#include <QSpacerItem>
#include <QButtonGroup>
#include <QBoxLayout>
#include <iostream>

DetailedResultsWidget::DetailedResultsWidget(QWidget *parent) : QDialog(parent), currentResult(nullptr),
                                                                subjectManager(nullptr), questions(nullptr),
                                                                studentAnswers(nullptr), questionIds(nullptr),
                                                                currentQuestionIndex(0),
                                                                totalQuestions(0), examScore(0.0) {
    setModal(true);
    setWindowTitle("Detailed Exam Results");
    resize(1000, 700);

    setupUI();
    setupConnections();
}

DetailedResultsWidget::~DetailedResultsWidget() {
    if (questions) {
        delete questions;
    }
    if (studentAnswers) {
        delete studentAnswers;
    }
    if (questionIds) {
        delete questionIds;
    }
}

void DetailedResultsWidget::setupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Left side - Results content
    QVBoxLayout *resultsLayout = new QVBoxLayout();

    // Header section
    QHBoxLayout *headerLayout = new QHBoxLayout();

    titleLabel = new QLabel("Exam Results");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50;");

    scoreLabel = new QLabel("Score: 0/10");
    scoreLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #27ae60; "
        "background-color: #ecf0f1; padding: 8px 12px; border-radius: 4px;");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(scoreLabel);

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

    // Answer options (disabled for viewing)
    answerGroup = new QButtonGroup();

    optionA = new QRadioButton("A. Option A");
    optionB = new QRadioButton("B. Option B");
    optionC = new QRadioButton("C. Option C");
    optionD = new QRadioButton("D. Option D");

    // Disable all options since this is view-only
    optionA->setEnabled(false);
    optionB->setEnabled(false);
    optionC->setEnabled(false);
    optionD->setEnabled(false);

    answerGroup->addButton(optionA, 0);
    answerGroup->addButton(optionB, 1);
    answerGroup->addButton(optionC, 2);
    answerGroup->addButton(optionD, 3);

    // Result information labels
    studentAnswerLabel = new QLabel("Your Answer: ");
    studentAnswerLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #3498db; margin: 8px 0px;");

    correctAnswerLabel = new QLabel("Correct Answer: ");
    correctAnswerLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #27ae60; margin: 8px 0px;");

    resultLabel = new QLabel("Result: ");
    resultLabel->setStyleSheet("font-size: 14px; font-weight: bold; margin: 8px 0px;");

    questionLayout->addWidget(questionNumberLabel);
    questionLayout->addWidget(questionContentLabel);
    questionLayout->addWidget(optionA);
    questionLayout->addWidget(optionB);
    questionLayout->addWidget(optionC);
    questionLayout->addWidget(optionD);
    questionLayout->addWidget(studentAnswerLabel);
    questionLayout->addWidget(correctAnswerLabel);
    questionLayout->addWidget(resultLabel);
    questionLayout->addStretch();

    questionScrollArea->setWidget(questionWidget);

    // Navigation buttons
    QHBoxLayout *navButtonLayout = new QHBoxLayout();

    previousButton = new QPushButton("← Previous");
    nextButton = new QPushButton("Next →");
    closeButton = new QPushButton("Close");

    previousButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; "
        "padding: 8px 16px; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #7f8c8d; }"
        "QPushButton:disabled { background-color: #bdc3c7; }");

    nextButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; "
        "padding: 8px 16px; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #2980b9; }");

    closeButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; "
        "padding: 8px 16px; border: none; border-radius: 4px; }"
        "QPushButton:hover { background-color: #c0392b; }");

    navButtonLayout->addWidget(previousButton);
    navButtonLayout->addWidget(nextButton);
    navButtonLayout->addStretch();
    navButtonLayout->addWidget(closeButton);

    // Add to results layout
    resultsLayout->addLayout(headerLayout);
    resultsLayout->addWidget(progressBar);
    resultsLayout->addWidget(questionScrollArea);
    resultsLayout->addLayout(navButtonLayout);

    // Right side - Navigation panel
    createNavigationPanel();

    // Add to main layout
    mainLayout->addLayout(resultsLayout, 3);
    mainLayout->addWidget(navigationPanel, 1);
}

void DetailedResultsWidget::setupConnections() {
    connect(previousButton, &QPushButton::clicked, this, &DetailedResultsWidget::previousQuestion);
    connect(nextButton, &QPushButton::clicked, this, &DetailedResultsWidget::nextQuestion);
    connect(closeButton, &QPushButton::clicked, this, &DetailedResultsWidget::closeDialog);
}

void DetailedResultsWidget::createNavigationPanel() {
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

void DetailedResultsWidget::showResults(DiemThi *examResult, QuanLyMonHoc *manager) {
    if (!examResult || !manager) {
        QMessageBox::critical(this, "Error", "Invalid exam result data.");
        reject();
        return;
    }

    currentResult = examResult;
    subjectManager = manager;
    examScore = examResult->getDiem();

    // Get subject information
    MonHoc *subject = subjectManager->tim(examResult->getMaMon());
    if (!subject) {
        QMessageBox::critical(this, "Error", "Subject not found.");
        reject();
        return;
    }

    // Update title and score
    titleLabel->setText(QString("Exam Results: %1").arg(QString::fromStdString(subject->getTenMon())));
    setWindowTitle(QString("Detailed Results - %1").arg(QString::fromStdString(subject->getTenMon())));
    scoreLabel->setText(QString("Score: %1/10").arg(QString::number(examScore, 'f', 2)));

    // Set score label color based on pass/fail
    if (ThongKe::isPassingScore(examScore)) {
        scoreLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #27ae60; "
            "background-color: #d5f4e6; padding: 8px 12px; border-radius: 4px;");
    } else {
        scoreLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #e74c3c; "
            "background-color: #fdf2f2; padding: 8px 12px; border-radius: 4px;");
    }

    // Copy student answers
    studentAnswers = new DynamicArray<char>();
    DynamicArray<char> *resultAnswers = examResult->getDanhSachCauTraLoi();
    if (resultAnswers) {
        for (int i = 0; i < resultAnswers->size(); i++) {
            studentAnswers->add(resultAnswers->get(i));
        }
    }

    // Copy question IDs
    questionIds = new DynamicArray<int>();
    DynamicArray<int> *resultQuestionIds = examResult->getDanhSachCauHoi();
    if (resultQuestionIds) {
        for (int i = 0; i < resultQuestionIds->size(); i++) {
            questionIds->add(resultQuestionIds->get(i));
        }
    }

    totalQuestions = questionIds->size();
    currentQuestionIndex = 0;

    // Load questions based on IDs
    loadQuestionsFromIds();

    if (questions->size() == 0) {
        QMessageBox::warning(this, "Warning", "No questions found for this exam result.");
        reject();
        return;
    }

    // Update navigation panel
    updateQuestionNavigation();

    // Load first question
    loadQuestion(0);

    updateProgress();
}

void DetailedResultsWidget::loadQuestionsFromIds() {
    if (!subjectManager || !questionIds) {
        return;
    }

    questions = new DynamicArray<CauHoi *>();

    // Get subject
    MonHoc *subject = subjectManager->tim(currentResult->getMaMon());
    if (!subject || !subject->getQuanLyCauHoi()) {
        return;
    }

    // Load questions by ID
    for (int i = 0; i < questionIds->size(); i++) {
        int questionId = questionIds->get(i);
        CauHoi *question = subject->getQuanLyCauHoi()->tim(questionId);
        if (question) {
            questions->add(question);
        }
    }
}

void DetailedResultsWidget::loadQuestion(int index) {
    if (!questions || index < 0 || index >= questions->size()) {
        return;
    }

    currentQuestionIndex = index;
    CauHoi *question = questions->get(index);

    if (!question) {
        questionContentLabel->setText("Question not found in database.");
        return;
    }

    // Update question display
    questionNumberLabel->setText(QString("Question %1 of %2").arg(index + 1).arg(questions->size()));
    questionContentLabel->setText(QString::fromStdString(question->getNoiDung()));

    // Update answer options
    optionA->setText(QString("A. %1").arg(QString::fromStdString(question->getLuaChonA())));
    optionB->setText(QString("B. %1").arg(QString::fromStdString(question->getLuaChonB())));
    optionC->setText(QString("C. %1").arg(QString::fromStdString(question->getLuaChonC())));
    optionD->setText(QString("D. %1").arg(QString::fromStdString(question->getLuaChonD())));

    // Get student answer and correct answer
    char studentAnswer = (index < studentAnswers->size()) ? studentAnswers->get(index) : ' ';
    char correctAnswer = question->getDapAnDung();

    // Update result information
    studentAnswerLabel->setText(QString("Your Answer: %1").arg(getAnswerText(studentAnswer)));
    correctAnswerLabel->setText(QString("Correct Answer: %1").arg(getAnswerText(correctAnswer)));

    bool isCorrect = (studentAnswer == correctAnswer && studentAnswer != ' ');
    if (isCorrect) {
        resultLabel->setText("Result: ✓ CORRECT");
        resultLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #27ae60; margin: 8px 0px;");
    } else {
        resultLabel->setText("Result: ✗ INCORRECT");
        resultLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #e74c3c; margin: 8px 0px;");
    }

    // Highlight answers
    highlightAnswers(studentAnswer, correctAnswer);

    // Update navigation buttons
    previousButton->setEnabled(index > 0);
    nextButton->setEnabled(index < questions->size() - 1);

    updateProgress();
}

void DetailedResultsWidget::highlightAnswers(char studentAnswer, char correctAnswer) {
    // Reset all styles first
    QString defaultStyle = "QRadioButton { font-size: 13px; color: #2c3e50; padding: 8px; margin: 4px 0px; }";
    optionA->setStyleSheet(defaultStyle);
    optionB->setStyleSheet(defaultStyle);
    optionC->setStyleSheet(defaultStyle);
    optionD->setStyleSheet(defaultStyle);

    // Clear all selections
    optionA->setChecked(false);
    optionB->setChecked(false);
    optionC->setChecked(false);
    optionD->setChecked(false);

    // Highlight correct answer (green)
    QRadioButton *correctOption = nullptr;
    if (correctAnswer == 'A')
        correctOption = optionA;
    else if (correctAnswer == 'B')
        correctOption = optionB;
    else if (correctAnswer == 'C')
        correctOption = optionC;
    else if (correctAnswer == 'D')
        correctOption = optionD;

    if (correctOption) {
        correctOption->setStyleSheet("QRadioButton { font-size: 13px; color: white; padding: 8px; margin: 4px 0px; "
            "background-color: #27ae60; border-radius: 4px; }");
        correctOption->setChecked(true);
    }

    // Highlight student answer if different from correct (red for wrong, already green if correct)
    if (studentAnswer != correctAnswer && studentAnswer != ' ') {
        QRadioButton *studentOption = nullptr;
        if (studentAnswer == 'A')
            studentOption = optionA;
        else if (studentAnswer == 'B')
            studentOption = optionB;
        else if (studentAnswer == 'C')
            studentOption = optionC;
        else if (studentAnswer == 'D')
            studentOption = optionD;

        if (studentOption && studentOption != correctOption) {
            studentOption->setStyleSheet("QRadioButton { font-size: 13px; color: white; padding: 8px; margin: 4px 0px; "
                "background-color: #e74c3c; border-radius: 4px; }");
        }
    }
}

void DetailedResultsWidget::nextQuestion() {
    if (currentQuestionIndex < questions->size() - 1) {
        loadQuestion(currentQuestionIndex + 1);
    }
}

void DetailedResultsWidget::previousQuestion() {
    if (currentQuestionIndex > 0) {
        loadQuestion(currentQuestionIndex - 1);
    }
}

void DetailedResultsWidget::updateQuestionNavigation() {
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

        // Color coding based on correctness
        char studentAnswer = (i < studentAnswers->size()) ? studentAnswers->get(i) : ' ';
        CauHoi *question = questions->get(i);
        char correctAnswer = question ? question->getDapAnDung() : ' ';

        bool isCorrect = (studentAnswer == correctAnswer && studentAnswer != ' ');
        bool isAnswered = (studentAnswer != ' ');

        if (isCorrect) {
            // Correct answer - green
            questionBtn->setStyleSheet(
                "QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 4px; }");
        } else if (isAnswered) {
            // Wrong answer - red
            questionBtn->setStyleSheet(
                "QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 4px; }");
        } else {
            // No answer - gray
            questionBtn->setStyleSheet(
                "QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 4px; }");
        }

        // Highlight current question
        if (i == currentQuestionIndex) {
            questionBtn->setStyleSheet(
                "QPushButton { background-color: #3498db; color: white; border: 2px solid #2980b9; border-radius: 4px; }");
        }

        connect(questionBtn, &QPushButton::clicked, [this, i]() { goToQuestion(i); });

        gridLayout->addWidget(questionBtn, i / 5, i % 5);
    }

    QWidget *gridWidget = new QWidget();
    gridWidget->setLayout(gridLayout);

    navLayout->insertWidget(1, gridWidget);
}

void DetailedResultsWidget::updateProgress() {
    if (!questions || !studentAnswers)
        return;

    int correctAnswers = QuanLyThi::countCorrectAnswers(*questions, *studentAnswers);
    int percentage = QuanLyThi::calculatePercentage(correctAnswers, questions->size());

    progressBar->setValue(percentage);
    progressBar->setFormat(QString("Correct: %1/%2 questions (%p%)")
        .arg(correctAnswers)
        .arg(questions->size()));
}

void DetailedResultsWidget::goToQuestion(int questionIndex) {
    if (questionIndex >= 0 && questionIndex < questions->size()) {
        loadQuestion(questionIndex);
    }
}

QString DetailedResultsWidget::getAnswerText(char answer) {
    switch (answer) {
        case 'A':
            return "A";
        case 'B':
            return "B";
        case 'C':
            return "C";
        case 'D':
            return "D";
        default:
            return "No Answer";
    }
}

void DetailedResultsWidget::closeDialog() {
    accept();
}