#include "QuanLyThi.h"
#include "../managers/QuanLyDiem.h"

double QuanLyThi::calculateScore(DynamicArray<CauHoi *> &questions,
                                 DynamicArray<char> &studentAnswers) {
    if (questions.size() == 0) {
        return 0.0;
    }

    int correctAnswers = countCorrectAnswers(questions, studentAnswers);
    return (static_cast<double>(correctAnswers) / questions.size()) * 10.0;
}

DiemThi *QuanLyThi::createExamResult(const char *subjectCode, double score,
                                     DynamicArray<char> &studentAnswers,
                                     DynamicArray<CauHoi *> &questions) {
    DiemThi *examResult = new DiemThi(subjectCode, score);

    // Copy student answers
    for (int i = 0; i < studentAnswers.size(); i++) {
        examResult->getDanhSachCauTraLoi()->add(studentAnswers.get(i));
    }

    // Copy question IDs
    for (int i = 0; i < questions.size(); i++) {
        CauHoi *question = questions.get(i);
        if (question) {
            int ma = question->getMaCauHoi();
            examResult->getDanhSachCauHoi()->add(ma);
        }
    }

    return examResult;
}

bool QuanLyThi::saveExamResult(SinhVien *student, DiemThi *examResult) {
    if (!student || !student->getQuanLyDiem() || !examResult) {
        return false;
    }

    // Check if student already has a score for this subject
    DiemThi *existingScore = student->getQuanLyDiem()->tim(examResult->getMaMon());

    if (existingScore) {
        // Update existing score if new score is better
        if (examResult->getDiem() > existingScore->getDiem()) {
            student->getQuanLyDiem()->sua(*examResult);
        }
    } else {
        // Add new score
        student->getQuanLyDiem()->them(*examResult);
    }

    // Save to file
    student->getQuanLyDiem()->saveToFile();
    return true;
}

int QuanLyThi::countCorrectAnswers(DynamicArray<CauHoi *> &questions,
                                   DynamicArray<char> &studentAnswers) {
    int correctAnswers = 0;

    for (int i = 0; i < questions.size() && i < studentAnswers.size(); i++) {
        CauHoi *question = questions.get(i);
        char studentAnswer = studentAnswers.get(i);

        if (question && question->kiemTraDapAn(studentAnswer)) {
            correctAnswers++;
        }
    }

    return correctAnswers;
}

int QuanLyThi::countAnsweredQuestions(DynamicArray<char> &studentAnswers) {
    int answeredQuestions = 0;

    for (int i = 0; i < studentAnswers.size(); i++) {
        if (studentAnswers.get(i) != ' ') {
            answeredQuestions++;
        }
    }

    return answeredQuestions;
}

double QuanLyThi::calculatePercentage(int correct, int total) {
    if (total == 0) {
        return 0.0;
    }
    return (static_cast<double>(correct) / total) * 100.0;
}
