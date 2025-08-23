#include "ThongKe.h"
#include <sstream>
#include <iomanip>

#include "QuanLyDiem.h"

std::string ThongKe::formatGrade(double score) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << score << "/10";
    return ss.str();
}

std::string ThongKe::getPassFailStatus(double score) {
    return (score >= 5.0) ? "PASSED" : "FAILED";
}

bool ThongKe::isPassingScore(double score) {
    return score >= 5.0;
}
