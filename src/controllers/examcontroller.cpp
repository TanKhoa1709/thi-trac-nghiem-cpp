#include "examcontroller.h"
#include "cauhoi.h"
#include "diemthi.h"
#include "quanlythi.h"
#include <QTimer>
#include <QDebug>

ExamController::ExamController(QObject* parent)
    : QObject(parent)
    , quanLyThi(new QuanLyThi())
    , timeRemainingMinutes(60)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ExamController::updateTimer);
    setupTimer();
}

ExamController::~ExamController()
{
    delete quanLyThi;
}

void ExamController::setQuanLyCauHoi(QuanLyCauHoi* qlch)
{
    if (quanLyThi) {
        quanLyThi->setQuanLyCauHoi(qlch);
    }
}

bool ExamController::startExam(const QString& studentId, const QString& subjectId, int numQuestions)
{
    if (isExamActive()) {
        return false;
    }

    if (!quanLyThi) {
        qDebug() << "QuanLyThi not initialized";
        return false;
    }

    // Start exam with QuanLyThi
    bool success = quanLyThi->batDauThi(studentId, subjectId, numQuestions, 60);
    if (!success) {
        qDebug() << "Failed to start exam for student:" << studentId << "subject:" << subjectId;
        return false;
    }

    currentStudentId = studentId;
    currentSubjectId = subjectId;
    timeRemainingMinutes = 60; // Default 60 minutes

    timer->start(60000); // Update every minute
    
    // Signal emission removed for simplified build
    
    return true;
}

bool ExamController::finishExam()
{
    if (!isExamActive()) {
        return false;
    }

    timer->stop();
    
    // Finish exam with QuanLyThi
    bool success = false;
    if (quanLyThi) {
        success = quanLyThi->ketThucThi();
    }
    
    // Get exam results
    QVariantMap results = getExamResults();
    
    // Signal emission removed for simplified build
    
    resetExamState();
    return success;
}

bool ExamController::cancelExam()
{
    if (!isExamActive()) {
        return false;
    }

    timer->stop();
    
    // Cancel exam with QuanLyThi
    bool success = false;
    if (quanLyThi) {
        success = quanLyThi->huyThi();
    }
    
    resetExamState();
    
    // Signal emission removed for simplified build
    
    return success;
}

bool ExamController::nextQuestion()
{
    if (!quanLyThi) {
        return false;
    }
    
    bool success = quanLyThi->chuyenCauHoiTiep();
    // Signal emission removed for simplified build
    return success;
}

bool ExamController::previousQuestion()
{
    if (!quanLyThi) {
        return false;
    }
    
    bool success = quanLyThi->chuyenCauHoiTruoc();
    // Signal emission removed for simplified build
    return success;
}

bool ExamController::selectAnswer(const QString& answer)
{
    if (!quanLyThi) {
        return false;
    }
    
    bool success = quanLyThi->chonDapAn(answer);
    // Signal emission removed for simplified build
    return success;
}

bool ExamController::isExamActive() const
{
    return !currentStudentId.isEmpty() && !currentSubjectId.isEmpty();
}

int ExamController::currentQuestionIndex() const
{
    if (!quanLyThi) {
        return -1;
    }
    
    return quanLyThi->cauHoiHienTai();
}

int ExamController::totalQuestions() const
{
    if (!quanLyThi) {
        return 0;
    }
    
    return quanLyThi->soLuongCauHoi();
}

QString ExamController::currentQuestion() const
{
    if (!quanLyThi) {
        return "";
    }
    
    CauHoi* cauHoi = quanLyThi->getCauHoiHienTai();
    if (cauHoi) {
        return cauHoi->getNoiDung();
    }
    
    return "";
}

QStringList ExamController::currentAnswers() const
{
    if (!quanLyThi) {
        return QStringList();
    }
    
    CauHoi* cauHoi = quanLyThi->getCauHoiHienTai();
    if (cauHoi) {
        return cauHoi->getDapAn();
    }
    
    return QStringList();
}

QString ExamController::selectedAnswer() const
{
    if (!quanLyThi) {
        return "";
    }
    
    return quanLyThi->getDapAnDaChon();
}

int ExamController::questionsAnswered() const
{
    if (!quanLyThi) {
        return 0;
    }
    
    return quanLyThi->soCauDaTraLoi();
}

double ExamController::progressPercentage() const
{
    if (totalQuestions() == 0) return 0.0;
    return (double)questionsAnswered() / totalQuestions() * 100.0;
}

QVariantMap ExamController::getExamResults()
{
    QVariantMap results;
    results["studentId"] = currentStudentId;
    results["subjectId"] = currentSubjectId;
    
    if (quanLyThi) {
        results["score"] = quanLyThi->diemSo();
        results["totalQuestions"] = quanLyThi->soLuongCauHoi();
        results["correctAnswers"] = quanLyThi->soCauDung();
        results["answeredQuestions"] = quanLyThi->soCauDaTraLoi();
    } else {
        results["score"] = 0.0;
        results["totalQuestions"] = 0;
        results["correctAnswers"] = 0;
        results["answeredQuestions"] = 0;
    }
    
    results["timeSpent"] = 60 - timeRemainingMinutes;
    
    return results;
}

void ExamController::updateTimer()
{
    if (timeRemainingMinutes > 0) {
        timeRemainingMinutes--;
        // Signal emission removed for simplified build
        
        if (timeRemainingMinutes <= 5) {
            // Signal emission removed for simplified build
        }
        
        if (timeRemainingMinutes == 0) {
            // Signal emission removed for simplified build
            handleTimeUp();
        }
    }
}

void ExamController::handleTimeUp()
{
    finishExam();
}

void ExamController::setupTimer()
{
    timer->setSingleShot(false);
}

void ExamController::resetExamState()
{
    currentStudentId.clear();
    currentSubjectId.clear();
    timeRemainingMinutes = 60;
}
