#ifndef QUANLYSINHVIEN_H
#define QUANLYSINHVIEN_H

#include <string>
#include "../models/sinhvien.h"
#include "../utils/LinkedList.h"
#include "../utils/DynamicArray.h"

/**
 * @brief Quản lý sinh viên - Danh sách liên kết
 * Quản lý danh sách sinh viên trong một lớp học
 */
class QuanLySinhVien {
private:
    std::string maLop; // Mã lớp học
    LinkedList<SinhVien> danhSachSinhVien; // Danh sách liên kết sinh viên

public:
    // Constructors & Destructor
    explicit QuanLySinhVien(const std::string &maLop);

    ~QuanLySinhVien();

    // Basic CRUD operations
    void danhSach(DynamicArray<SinhVien *> &result);

    SinhVien *tim(const std::string &maSinhVien);

    bool them(SinhVien &sinhVien);

    bool sua(SinhVien &sinhVien);

    bool xoa(const std::string &maSinhVien);

    // Data persistence data/sinhvien/sinhvien_<maLop>.txt
    void saveToFile();

    void loadFromFile();

    // Utility methods
    int size() { return danhSachSinhVien.size(); }
    bool isEmpty() { return danhSachSinhVien.isEmpty(); }
};

#endif // QUANLYSINHVIEN_H
