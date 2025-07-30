#ifndef QUANLYMONHOC_H
#define QUANLYMONHOC_H

#include <string>
#include <vector>
#include "../models/monhoc.h"
#include "../utils/Array.h"

// File name constant for subject data
const std::string MONHOC_DATA_FILE = "data/subjects.txt";

/**
 * @brief Quản lý môn học - Danh sách tuyến tính
 */
class QuanLyMonHoc {
private:
    Array<MonHoc*> danhSachMonHoc;  // Danh sách tuyến tính

public:
    QuanLyMonHoc();
    ~QuanLyMonHoc();

    // Basic CRUD
    bool themMonHoc(const char* maMon, const std::string& tenMon);
    bool xoaMonHoc(const char* maMon);
    MonHoc* timMonHoc(const char* maMon);
    
    // Display
    void inDanhSachMonHoc() const;
    
    // File I/O
    void saveToFile() const;
    void loadFromFile();
    int getSoLuongMonHoc() const;
};

#endif // QUANLYMONHOC_H