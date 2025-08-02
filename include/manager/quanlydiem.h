#ifndef QUANLYDIEM_H
#define QUANLYDIEM_H

#include <string>
#include "../utils/DynamicArray.h"
#include "../utils/LinkedList.h"
#include "../models/diemthi.h"

/**
 * @brief Quản lý điểm thi - Danh sách liên kết đơn
 * Lưu trữ điểm thi của một sinh viên theo từng môn học
 */
class QuanLyDiem {
private:
    std::string maSinhVien;                    // Mã sinh viên
    LinkedList<DiemThi> danhSachDiem;         // Danh sách liên kết điểm thi

public:
    // Constructors & Destructor
    explicit QuanLyDiem(const std::string& maSinhVien);
    ~QuanLyDiem();

    // Basic CRUD operations
    void danhSach(DynamicArray<DiemThi>& result);
    DiemThi* tim(const std::string& maMonHoc);
    bool them(DiemThi& diem);
    bool sua(DiemThi& diem);
    bool xoa(const std::string& maMonHoc);

    // Statistics and analysis
    double tinhDiemTrungBinh();
    int demSoMonDau();
    int demSoMonRot();
    int demSoMonDaThi();

    // Data persistence data/diemthi/diemthi_<maSinhVien>.txt
    void saveToFile();
    void loadFromFile();
    
    // Utility methods
    int size() { return danhSachDiem.size(); }
    bool isEmpty() { return danhSachDiem.isEmpty(); }
};

#endif // QUANLYDIEM_H