#include "cauhoi.h"
#include <cctype>

// Default constructor
CauHoi::CauHoi() : maCauHoi(0), noiDung(""), luaChonA(""), luaChonB(""),
                   luaChonC(""), luaChonD(""), dapAnDung('A') {
}

// Parameterized constructor
CauHoi::CauHoi(int ma, const std::string &noiDung, const std::string &a,
               const std::string &b, const std::string &c, const std::string &d,
               char dapAn)
    : maCauHoi(ma), noiDung(noiDung), luaChonA(a), luaChonB(b),
      luaChonC(c), luaChonD(d), dapAnDung(std::toupper(dapAn)) {
}

// Validate question data
bool CauHoi::validate() const {
    // Check if ID is positive
    if (maCauHoi <= 0) {
        return false;
    }

    // Check if content is not empty
    if (noiDung.empty()) {
        return false;
    }

    // Check if all choices are not empty
    if (luaChonA.empty() || luaChonB.empty() || luaChonC.empty() || luaChonD.empty()) {
        return false;
    }

    // Check if answer is valid (A, B, C, or D)
    char upperAnswer = std::toupper(dapAnDung);
    if (upperAnswer != 'A' && upperAnswer != 'B' && upperAnswer != 'C' && upperAnswer != 'D') {
        return false;
    }

    return true;
}

// Check if the chosen answer is correct
bool CauHoi::kiemTraDapAn(char dapAnChon) const {
    return std::toupper(dapAnChon) == std::toupper(dapAnDung);
}
