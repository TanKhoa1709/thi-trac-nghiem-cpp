#ifndef EXAMMANAGER_H
#define EXAMMANAGER_H

#include "../models/DiemThi.h"
#include "../models/MonHoc.h"
#include "../models/SinhVien.h"
#include "../models/CauHoi.h"
#include "../utils/DynamicArray.h"

/**
 * @brief Exam Manager - Handles exam-related business logic
 * Manages exam creation, scoring, and result processing
 */
class QuanLyThi {
public:
    // Exam scoring and result management
    static double calculateScore(DynamicArray<CauHoi *> &questions,
                                 DynamicArray<char> &studentAnswers);

    static DiemThi *createExamResult(const char *subjectCode, double score,
                                     DynamicArray<char> &studentAnswers,
                                     DynamicArray<CauHoi *> &questions);

    static bool saveExamResult(SinhVien *student, DiemThi *examResult);

    // Statistics and analysis
    static int countCorrectAnswers(DynamicArray<CauHoi *> &questions,
                                   DynamicArray<char> &studentAnswers);

    static int countAnsweredQuestions(DynamicArray<char> &studentAnswers);

    static double calculatePercentage(int correct, int total);
};

#endif // EXAMMANAGER_H
