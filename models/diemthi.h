#ifndef DIEMTHI_H
#define DIEMTHI_H

#include "../utils/DynamicArray.h"
#include <iostream>
#include <cstring>

/**
 * @brief Điểm thi - MAMH, Điểm, Chi Tiết Bài Thi
 */
class DiemThi {
private:
    char maMon[16]; // Mã môn học
    double diem; // Điểm số
    DynamicArray<char> *danhSachCauTraLoi; // Chi tiết bài thi

public:
    // Constructors & Destructor
    DiemThi();

    DiemThi(const char *maMon, double diem);

    ~DiemThi();

    // Getters (const methods)
    const char *getMaMon() const { return maMon; }
    double getDiem() const { return diem; }
    DynamicArray<char> *getDanhSachCauTraLoi() { return danhSachCauTraLoi; }

    // Setters
    void setMaMon(const char *ma) {
        if (ma != nullptr) {
            std::strncpy(maMon, ma, 15);
            maMon[15] = '\0'; // Ensure null termination
        } else {
            std::memset(maMon, 0, sizeof(maMon));
        }
    }

    void setDiem(double d) { diem = d; }
    void setDanhSachCauTraLoi(DynamicArray<char> *danhSach) { danhSachCauTraLoi = danhSach; }

    // Business logic methods
    bool validate() const;

    // Operators for LinkedList operations
    bool operator==(DiemThi &other) {
        return std::strcmp(maMon, other.maMon) == 0;
    }

    bool operator!=(DiemThi &other) {
        return !(*this == other);
    }
};

#endif // DIEMTHI_H
