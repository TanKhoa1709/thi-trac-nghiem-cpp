#ifndef EXAMCONTROLLER_H
#define EXAMCONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QVariantMap>
#include <QTimer>

// Forward declarations
class QuanLyThi;
class QuanLyCauHoi;
class CauHoi;
class DiemThi;

/**
 * @brief Controller cho giao diện thi trắc nghiệm
 * Kết nối giữa QML UI và logic thi
 */
class ExamController : public QObject {

private:
    QuanLyThi* quanLyThi;
    QTimer* timer;
    int timeRemainingMinutes;
    QString currentStudentId;
    QString currentSubjectId;

public:
    explicit ExamController(QObject* parent = nullptr);
    ~ExamController();

    // Initialization
    void setQuanLyCauHoi(QuanLyCauHoi* qlch);

    // Exam session management
    bool startExam(const QString& studentId, const QString& subjectId, int numQuestions = 20);
    bool finishExam();
    bool cancelExam();
    bool pauseExam();
    bool resumeExam();

    // Question navigation
    bool nextQuestion();
    bool previousQuestion();
    bool goToQuestion(int questionIndex);

    // Answer handling
    bool selectAnswer(const QString& answer);
    bool clearAnswer();
    bool isQuestionAnswered(int questionIndex = -1);

    // Exam information
    QVariantMap getExamSummary();
    QVariantList getUnansweredQuestions();
    QVariantMap getQuestionDetails(int questionIndex = -1);

    // Properties for QML
    bool isExamActive() const;
    int currentQuestionIndex() const;
    int totalQuestions() const;
    QString currentQuestion() const;
    QStringList currentAnswers() const;
    QString selectedAnswer() const;
    int timeRemaining() const { return timeRemainingMinutes; }
    int questionsAnswered() const;
    double progressPercentage() const;

    // Results
    QVariantMap getExamResults();
    QVariantList getDetailedResults();

// Note: Signals removed for simplified build - can be re-added with proper MOC setup

private:
    void updateTimer();
    void handleTimeUp();

private:
    void setupTimer();
    void resetExamState();
    QVariantMap convertExamToVariant(const DiemThi& exam);
    QVariantMap convertQuestionToVariant(const CauHoi& question, int index);
};

#endif // EXAMCONTROLLER_H
