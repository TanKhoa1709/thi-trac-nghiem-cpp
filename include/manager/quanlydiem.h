#ifndef QUANLYDIEM_H
#define QUANLYDIEM_H

#include <string>
#include "../models/diemthi.h"
#include "../utils/LinkedList.h"

/**
 * @brief Quản lý điểm thi - Danh sách liên kết đơn
 */
class QuanLyDiem {
private:
    LinkedList<DiemThi*> danhSachDiem;  // Danh sách liên kết đơn điểm thi

public:
    QuanLyDiem();
    ~QuanLyDiem();

    // Basic CRUD
    bool themDiem(DiemThi* diem);
    bool xoaDiem(const std::string& maMonHoc);
    DiemThi* timDiem(const std::string& maMonHoc);
    
    // Display
    void inDanhSachDiem() const;
    void inBangDiem() const;
    
    // Statistics
    double tinhDiemTrungBinh() const;
    int demSoMonDau() const;
    int demSoMonRot() const;
    
    // Additional utility methods
    int getSoLuongDiem() const { return danhSachDiem.size(); }
    bool isEmpty() const { return danhSachDiem.isEmpty(); }
};

#endif // QUANLYDIEM_H