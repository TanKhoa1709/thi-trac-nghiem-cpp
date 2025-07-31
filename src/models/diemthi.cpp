#include "models/diemthi.h"

// Default constructor
DiemThi::DiemThi() : maMonHoc(""), diem(0.0), chiTietBaiThi("") {}

// Parameterized constructor
DiemThi::DiemThi(const std::string& maMon, double diem, const std::string& chiTiet)
    : maMonHoc(maMon), diem(diem), chiTietBaiThi(chiTiet) {}

// Validate exam score data
bool DiemThi::validate() const {
    // Check if subject code is not empty
    if (maMonHoc.empty()) {
        return false;
    }
    
    // Check if score is in valid range (0-10)
    if (diem < 0.0 || diem > 10.0) {
        return false;
    }
    
    // Detail can be empty (for future exams)
    return true;
}