#ifndef QUANLYMONHOC_H
#define QUANLYMONHOC_H

#include "../models/monhoc.h"
#include "../utils/DynamicArray.h"

/**
 * @brief Quản lý môn học - Danh sách tuyến tính
 * Quản lý tất cả các môn học trong hệ thống
 */
class QuanLyMonHoc {
private:
    DynamicArray<MonHoc> danhSachMonHoc; // Danh sách tuyến tính môn học

public:
    // Constructors & Destructor
    QuanLyMonHoc();

    ~QuanLyMonHoc();

    // Basic CRUD operations
    void danhSach(DynamicArray<MonHoc *> &result);

    MonHoc *tim(const char *maMon);

    bool them(MonHoc &monHoc);

    bool sua(MonHoc &monHoc);

    bool xoa(const char *maMon);

    // Data persistence data/monhoc.txt
    void saveToFile();

    void loadFromFile();

    // Utility methods
    int size() { return danhSachMonHoc.size(); }
    bool isEmpty() { return danhSachMonHoc.isEmpty(); }
};

#endif // QUANLYMONHOC_H
