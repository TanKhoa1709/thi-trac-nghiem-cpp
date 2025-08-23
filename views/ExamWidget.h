#ifndef EXAMWIDGET_H
#define EXAMWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include <QScrollArea>
#include <QMessageBox>
#include <QFrame>

// Forward declarations
class MonHoc;
class CauHoi;
class SinhVien;
class DiemThi;
class QuanLyMonHoc;
template<typename T>
class DynamicArray;

/**
 * @brief Exam Widget - Modal dialog for taking exams and viewing results
 * Provides exam interface with timer and question navigation
 * Can also display detailed exam results in view-only mode
 */
class ExamWidget : public QDialog {
    Q_OBJECT

public:
    explicit ExamWidget(QWidget *parent = nullptr);

    ~ExamWidget();

    // Setup exam
    void startExam(MonHoc *subject, int numQuestions, SinhVien *student);

    void showExamDetails(SinhVien *student, MonHoc *subject, DiemThi *examResult);

signals:
    void examCompleted(double score);

    void examCancelled();

private slots:
    void nextQuestion();

    void previousQuestion();

    void submitExam();

    void updateTimer();

    void onAnswerSelected();

    void goToQuestion(int questionIndex);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    // UI Components
    QLabel *titleLabel;
    QLabel *timerLabel;
    QProgressBar *progressBar;

    // Question display
    QScrollArea *questionScrollArea;
    QLabel *questionNumberLabel;
    QLabel *questionContentLabel;

    // Answer options
    QButtonGroup *answerGroup;
    QRadioButton *optionA;
    QRadioButton *optionB;
    QRadioButton *optionC;
    QRadioButton *optionD;

    // Navigation
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *submitButton;  // For submitting exam
    QPushButton *closeButton;   // For closing results view
    QPushButton *cancelButton;

    // Question navigation panel
    QFrame *navigationPanel;
    QVBoxLayout *navLayout;

    // Timer
    QTimer *examTimer;
    int timeRemaining; // in seconds
    int totalTime; // in seconds

    // Exam data
    MonHoc *currentSubject;
    SinhVien *currentStudent;
    DynamicArray<CauHoi *> *questions;
    DynamicArray<char> *studentAnswers;
    int currentQuestionIndex;
    int totalQuestions;

    // Setup methods
    void setupUI();

    void setupConnections();

    void setupTimer(int minutes = 60);

    void createNavigationPanel();

    // Exam logic
    void loadQuestion(int index);

    void saveCurrentAnswer();

    void calculateAndSaveResults();

    void updateQuestionNavigation();

    void updateProgress();

    // Helper methods
    QString formatTime(int seconds);

    char getSelectedAnswer();

    void setSelectedAnswer(char answer);

    void resetExam();

    bool confirmSubmission();

    bool confirmCancellation();

    // Results viewing methods (reusing existing UI)
    void loadQuestionsFromIds();

    void highlightAnswers(char studentAnswer, char correctAnswer);

    QString getAnswerText(char answer);

    void switchToResultsMode();

    void switchToExamMode();
};

#endif // EXAMWIDGET_H
