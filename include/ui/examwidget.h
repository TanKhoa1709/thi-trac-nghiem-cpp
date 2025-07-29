#ifndef EXAMWIDGET_H
#define EXAMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QGroupBox>
#include <QStackedWidget>
#include <QProgressBar>
#include <QTimer>
#include <QMessageBox>
#include <QScrollArea>

class AppManager;

/**
 * @brief Widget for taking exams
 * Implements: Thi Trắc nghiệm (trước khi thi hỏi người thi môn thi, số câu hỏi thi, số phút thi)
 */
class ExamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExamWidget(AppManager* appManager, QWidget *parent = nullptr);
    ~ExamWidget();

public slots:
    void startNewExam();
    void viewExamHistory();

private slots:
    void onStartExam();
    void onSubmitExam();
    void onNextQuestion();
    void onPreviousQuestion();
    void onQuestionAnswered();
    void onTimerTick();
    void onTimeUp();
    void onBackToSetup();

signals:
    void examCompleted(const QString& subjectCode, int score, int totalQuestions);

private:
    void setupUI();
    void setupExamSetup();
    void setupExamInterface();
    void setupResultInterface();
    void setupConnections();
    void applyStyles();
    void loadSubjects();
    void generateExamQuestions();
    void showQuestion(int questionIndex);
    void updateProgress();
    void calculateResults();
    void showResults();

    AppManager* appManager;

    // UI components
    QVBoxLayout* mainLayout;
    QStackedWidget* stackedWidget;
    
    // Exam setup page
    QWidget* setupPage;
    QVBoxLayout* setupLayout;
    QGroupBox* setupGroup;
    QGridLayout* setupFormLayout;
    QComboBox* subjectCombo;
    QSpinBox* questionCountSpin;
    QSpinBox* timeLimitSpin;
    QPushButton* startExamButton;
    QPushButton* viewHistoryButton;
    
    // Exam interface page
    QWidget* examPage;
    QVBoxLayout* examLayout;
    QGroupBox* examInfoGroup;
    QLabel* examInfoLabel;
    QProgressBar* progressBar;
    QLabel* timerLabel;
    QTimer* examTimer;
    
    QGroupBox* questionGroup;
    QVBoxLayout* questionLayout;
    QScrollArea* questionScrollArea;
    QWidget* questionContent;
    QVBoxLayout* questionContentLayout;
    QLabel* questionNumberLabel;
    QLabel* questionTextLabel;
    QButtonGroup* answerGroup;
    QRadioButton* answerA;
    QRadioButton* answerB;
    QRadioButton* answerC;
    QRadioButton* answerD;
    
    QHBoxLayout* navigationLayout;
    QPushButton* previousButton;
    QPushButton* nextButton;
    QPushButton* submitButton;
    
    // Result page
    QWidget* resultPage;
    QVBoxLayout* resultLayout;
    QGroupBox* resultGroup;
    QLabel* scoreLabel;
    QLabel* detailsLabel;
    QPushButton* newExamButton;
    QPushButton* backButton;
    
    // Exam state
    QString currentSubject;
    int totalQuestions;
    int timeLimit; // in minutes
    int currentQuestionIndex;
    int timeRemaining; // in seconds
    int calculatedScore; // stores the calculated exam score
    QStringList examQuestions;
    QStringList userAnswers;
    QStringList correctAnswers;
    bool examInProgress;
};

#endif // EXAMWIDGET_H