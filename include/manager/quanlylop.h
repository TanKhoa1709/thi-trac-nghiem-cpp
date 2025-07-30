#ifndef QUANLYLOP_H
#define QUANLYLOP_H

#include <string>
#include <vector>
#include "../models/lop.h"
#include "../utils/Array.h"

// File name constant for class data
const std::string LOP_DATA_FILE = "data/classes.txt";

// Forward declaration
class SinhVien;

/**
 * @brief Quản lý danh sách lớp - Mảng con trỏ
 */
class QuanLyLop {
private:
    Array<Lop*> danhSachLop;  // Mảng con trỏ

public:
    QuanLyLop();
    ~QuanLyLop();

    // Basic CRUD
    bool themLop(const std::string& maLop, const std::string& tenLop);
    bool xoaLop(const std::string& maLop);
    Lop* timLop(const std::string& maLop);
    
    // Display
    void inDanhSachLop() const;
    
    // File I/O
    void saveToFile() const;
    void loadFromFile();
    int getSoLuongLop() const;
};

#endif // QUANLYLOP_H