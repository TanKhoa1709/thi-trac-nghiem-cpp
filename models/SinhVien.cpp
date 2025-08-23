#include "SinhVien.h"
#include "../managers/QuanLyDiem.h"

// Default constructor
SinhVien::SinhVien() : maSinhVien(""), ho(""), ten(""), phai(true),
                       password(""), quanLyDiem(nullptr) {
}

// Parameterized constructor
SinhVien::SinhVien(const std::string &ma, const std::string &ho,
                   const std::string &ten, bool phai, const std::string &pass)
    : maSinhVien(ma), ho(ho), ten(ten), phai(phai), password(pass) {
    quanLyDiem = new QuanLyDiem(ma);
}

// Destructor
SinhVien::~SinhVien() {
    delete quanLyDiem;
    quanLyDiem = nullptr;
}

// Validate student data
bool SinhVien::validate() const {
    // Check if student ID is not empty
    if (maSinhVien.empty()) {
        return false;
    }

    // Check if last name is not empty
    if (ho.empty()) {
        return false;
    }

    // Check if first name is not empty
    if (ten.empty()) {
        return false;
    }

    // Check if password is not empty
    if (password.empty()) {
        return false;
    }

    return true;
}
