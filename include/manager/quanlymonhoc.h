#ifndef QUANLYMONHOC_H
#define QUANLYMONHOC_H

#include <string>
#include "../models/monhoc.h"
#include "../utils/DynamicArray.h"

/**
 * @brief Quản lý môn học - Danh sách tuyến tính
 * Quản lý tất cả các môn học trong hệ thống
 */
class QuanLyMonHoc {
private:
    static const std::string DATA_FILE_PATH;           // "data/monhoc.txt"
    DynamicArray<MonHoc*> danhSachMonHoc;              // Danh sách tuyến tính môn học

public:
    // Constructors & Destructor
    QuanLyMonHoc();
    ~QuanLyMonHoc();

    // Basic CRUD operations
    DynamicArray<MonHoc*> danhSach();
    MonHoc* tim(const char* maMon);
    bool them(MonHoc* monHoc);
    bool sua(MonHoc* monHoc);
    bool xoa(const char* maMon);
    
    // Data persistence data/monhoc.txt
    void saveToFile();
    void loadFromFile();

    // Utility methods
    int size() { return danhSachMonHoc.getSize(); }
    bool isEmpty() { return danhSachMonHoc.isEmpty(); }
};

#endif // QUANLYMONHOC_H