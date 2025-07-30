#ifndef QUANLYSINHVIEN_H
#define QUANLYSINHVIEN_H

#include <string>
#include "../models/sinhvien.h"
#include "../utils/LinkedList.h"

// File name constant for student data
const std::string SINHVIEN_DATA_FILE = "data/sinhvien.txt";

/**
 * @brief Quản lý sinh viên - Danh sách liên kết
 */
class QuanLySinhVien {
private:
    LinkedList<SinhVien*> danhSachSinhVien;  // Danh sách liên kết sinh viên

public:
    QuanLySinhVien();
    ~QuanLySinhVien();

    // Basic CRUD
    bool themSinhVien(SinhVien* sinhVien);
    bool xoaSinhVien(const std::string& maSinhVien);
    SinhVien* timSinhVien(const std::string& maSinhVien);
    
    // Authentication
    SinhVien* dangNhap(const std::string& maSinhVien, const std::string& matKhau);
    
    // Display
    void inDanhSachSinhVien() const;
    
    // Additional utility methods
    int getSoLuongSinhVien() const { return danhSachSinhVien.size(); }
    bool isEmpty() const { return danhSachSinhVien.isEmpty(); }
    
    // File I/O
    void saveToFile() const;
    void loadFromFile();
    
    // Iterator support for file operations
    void forEach(std::function<void(SinhVien*)> callback) const;
};

#endif // QUANLYSINHVIEN_H