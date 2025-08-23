#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include "../models/Lop.h"
#include "../models/MonHoc.h"
#include "../models/SinhVien.h"
#include "../models/DiemThi.h"
#include "../utils/DynamicArray.h"
#include <string>

/**
 * @brief Report Manager - Handles report generation and export business logic
 * Manages exam score reports, statistics, and data export
 */
class ReportUtil {
public:
    // Report data structures
    struct DiemThiSV {
        std::string maSinhVien;
        std::string ho;
        std::string ten;
        bool phai;
        std::string examScore;
    };

    struct BaoCao {
        std::string maLop;
        std::string tenLop;
        std::string maMon;
        std::string tenMon;
        DynamicArray<DiemThiSV> students;
    };

    // Report generation
    static BaoCao generateExamScoreReport(Lop *classData, MonHoc *subjectData);

    // Utility functions
    static std::string formatScore(double score);

    static bool isValidScore(const std::string &scoreStr);
};

#endif // REPORTMANAGER_H
