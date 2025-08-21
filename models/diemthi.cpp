#include "diemthi.h"
#include "../utils/DynamicArray.h"

// Default constructor
DiemThi::DiemThi() : diem(0.0), danhSachCauTraLoi(nullptr), danhSachCauHoi(nullptr) {
    std::memset(maMon, 0, sizeof(maMon));
}

// Parameterized constructor
DiemThi::DiemThi(const char *maMon, double diem)
    : diem(diem) {
    setMaMon(maMon);
    danhSachCauTraLoi = new DynamicArray<char>();
    danhSachCauHoi = new DynamicArray<int>();
}

DiemThi::~DiemThi() {
    delete danhSachCauTraLoi;
    danhSachCauTraLoi = nullptr;
    delete danhSachCauHoi;
    danhSachCauHoi = nullptr;
}

// Validate exam score data
bool DiemThi::validate() const {
    // Check if subject code is not empty
    if (std::strlen(maMon) == 0) {
        return false;
    }

    // Check if score is in valid range (0-10)
    if (diem < 0.0 || diem > 10.0) {
        return false;
    }

    // Answer list can be empty (for future exams)
    return true;
}
