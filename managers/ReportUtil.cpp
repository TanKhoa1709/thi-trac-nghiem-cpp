#include "ReportUtil.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>

#include "QuanLyDiem.h"
#include "QuanLySinhVien.h"

ReportUtil::BaoCao ReportUtil::generateExamScoreReport(Lop *classData, MonHoc *subjectData) {
    BaoCao reportData;

    if (!classData || !subjectData) {
        return reportData;
    }

    // Set basic report information
    reportData.maLop = classData->getMaLop();
    reportData.tenLop = classData->getTenLop();
    reportData.maMon = subjectData->getMaMon();
    reportData.tenMon = subjectData->getTenMon();

    // Get students from the class
    if (!classData->getQuanLySinhVien()) {
        return reportData;
    }

    DynamicArray<SinhVien *> danhSachSV;
    classData->getQuanLySinhVien()->danhSach(danhSachSV);

    for (int i = 0; i < danhSachSV.size(); i++) {
        SinhVien *sv = danhSachSV.get(i);
        if (!sv)
            continue;

        DiemThiSV studentData;
        studentData.maSinhVien = sv->getMaSinhVien();
        studentData.ho = sv->getHo();
        studentData.ten = sv->getTen();
        studentData.phai = sv->getPhai();

        // Check if student has exam result for this subject
        studentData.examScore = "Chưa thi";
        if (sv->getQuanLyDiem()) {
            DiemThi *examResult = sv->getQuanLyDiem()->tim(subjectData->getMaMon());
            if (examResult) {
                studentData.examScore = formatScore(examResult->getDiem());
            }
        }

        reportData.students.add(studentData);
    }

    return reportData;
}

std::string ReportUtil::formatScore(double score) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << score;
    return ss.str();
}

bool ReportUtil::isValidScore(const std::string &scoreStr) {
    if (scoreStr == "Chưa thi") {
        return false;
    }

    try {
        double score = std::stod(scoreStr);
        return score >= 0.0 && score <= 10.0;
    } catch (...) {
        return false;
    }
}
