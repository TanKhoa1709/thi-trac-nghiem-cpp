#ifndef DETAILEDRESULTSWIDGET_H
#define DETAILEDRESULTSWIDGET_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QProgressBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QFrame>
#include <QButtonGroup>
#include <QGridLayout>

// Forward declarations
class MonHoc;
class CauHoi;
class DiemThi;
class QuanLyMonHoc;
template<typename T>
class DynamicArray;

/**
 * @brief Detailed Results Widget - Modal dialog for viewing exam results
 * Reuses ExamWidget UI to show questions, answers, and correct answers
 */
class DetailedResultsWidget : public QDialog {
    Q_OBJECT

public:
    explicit DetailedResultsWidget(QWidget *parent = nullptr);
    ~DetailedResultsWidget();

    // Setup results view
    void showResults(DiemThi *examResult, QuanLyMonHoc *subjectManager);

private slots:
    void nextQuestion();
    void previousQuestion();
    void goToQuestion(int questionIndex);
    void closeDialog();

private:
    // UI Components - similar to ExamWidget
    QLabel *titleLabel;
    QLabel *scoreLabel;
    QProgressBar *progressBar;

    // Question display
    QScrollArea *questionScrollArea;
    QLabel *questionNumberLabel;
    QLabel *questionContentLabel;

    // Answer options (disabled, showing results)
    QButtonGroup *answerGroup;
    QRadioButton *optionA;
    QRadioButton *optionB;
    QRadioButton *optionC;
    QRadioButton *optionD;

    // Additional result labels
    QLabel *studentAnswerLabel;
    QLabel *correctAnswerLabel;
    QLabel *resultLabel; // Correct/Incorrect

    // Navigation
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *closeButton;

    // Question navigation panel
    QFrame *navigationPanel;
    QVBoxLayout *navLayout;

    // Results data
    DiemThi *currentResult;
    QuanLyMonHoc *subjectManager;
    DynamicArray<CauHoi *> *questions;
    DynamicArray<char> *studentAnswers;
    DynamicArray<int> *questionIds;
    int currentQuestionIndex;
    int totalQuestions;
    double examScore;

    // Setup methods
    void setupUI();
    void setupConnections();
    void createNavigationPanel();

    // Results display logic
    void loadQuestion(int index);
    void updateQuestionNavigation();
    void updateProgress();
    void loadQuestionsFromIds();

    // Helper methods
    void setAnswerStyle(QRadioButton *option, bool isStudentAnswer, bool isCorrectAnswer);
    QString getAnswerText(char answer);
    void highlightAnswers(char studentAnswer, char correctAnswer);
};

#endif // DETAILEDRESULTSWIDGET_H
