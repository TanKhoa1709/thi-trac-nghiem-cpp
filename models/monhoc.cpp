#include "monhoc.h"
#include "../managers/quanlycauhoi.h"
#include <cstring>
#include <iostream>

// Default constructor
MonHoc::MonHoc() : tenMon(""), quanLyCauHoi(nullptr) {
    std::memset(maMon, 0, sizeof(maMon));
}

// Parameterized constructor
MonHoc::MonHoc(const char *ma, const std::string &ten)
    : tenMon(ten) {
    setMaMon(ma);
    quanLyCauHoi = new QuanLyCauHoi(ma);
}

// Destructor
MonHoc::~MonHoc() {
    delete quanLyCauHoi;
    quanLyCauHoi = nullptr;
}

// Set subject code with validation
void MonHoc::setMaMon(const char *ma) {
    if (ma != nullptr) {
        std::strncpy(maMon, ma, 15);
        maMon[15] = '\0'; // Ensure null termination
    } else {
        std::memset(maMon, 0, sizeof(maMon));
    }
}

// Validate subject data
bool MonHoc::validate() const {
    // Check if subject code is not empty
    if (std::strlen(maMon) == 0) {
        return false;
    }

    // Check if subject name is not empty
    if (tenMon.empty()) {
        return false;
    }

    return true;
}
