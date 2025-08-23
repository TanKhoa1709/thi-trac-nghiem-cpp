#include "Lop.h"
#include "../managers/QuanLySinhVien.h"

// Default constructor
Lop::Lop() : maLop(""), tenLop(""), quanLySinhVien(nullptr) {
}

// Parameterized constructor
Lop::Lop(const std::string &ma, const std::string &ten)
    : maLop(ma), tenLop(ten) {
    quanLySinhVien = new QuanLySinhVien(ma);
}

// Destructor
Lop::~Lop() {
    delete quanLySinhVien;
    quanLySinhVien = nullptr;
}

// Validate class data
bool Lop::validate() const {
    // Check if class code is not empty
    if (maLop.empty()) {
        return false;
    }

    // Check if class name is not empty
    if (tenLop.empty()) {
        return false;
    }

    return true;
}
