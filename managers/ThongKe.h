#ifndef STATISTICSMANAGER_H
#define STATISTICSMANAGER_H

#include "../models/SinhVien.h"
#include "../models/DiemThi.h"
#include "../utils/DynamicArray.h"
#include <string>

/**
 * @brief Statistics Manager - Handles statistics and analysis business logic
 * Manages student statistics, grade formatting, and pass/fail calculations
 */
class ThongKe {
public:
    // Student statistics
    struct StudentStatistics {
        int totalExams;
        int passedExams;
        int failedExams;
        double averageScore;
        double passRate;
    };

    // Grade formatting and status
    static std::string formatGrade(double score);

    static std::string getPassFailStatus(double score);

    static bool isPassingScore(double score);

};

#endif // STATISTICSMANAGER_H
