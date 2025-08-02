#ifndef QUANLYLOP_H
#define QUANLYLOP_H

#include <string>
#include "../models/lop.h"
#include "../utils/DynamicArray.h"

/**
 * @brief Quản lý danh sách lớp - Mảng con trỏ
 * Quản lý tất cả các lớp học trong hệ thống
 */
class QuanLyLop {
private:
    DynamicArray<Lop> danhSachLop;                    // Mảng con trỏ lớp học

public:
    // Constructors & Destructor
    QuanLyLop();
    ~QuanLyLop();

    // Basic CRUD operations
    DynamicArray<Lop> danhSach();
    Lop* tim(const std::string& maLop);
    bool them(Lop& lop);
    bool sua(Lop& lop);
    bool xoa(const std::string& maLop);
    
    // Data persistence data/lop.txt
    void saveToFile();
    void loadFromFile();

    // Utility methods
    int size() { return danhSachLop.size(); }
    bool isEmpty() { return danhSachLop.isEmpty(); }
};

#endif // QUANLYLOP_H