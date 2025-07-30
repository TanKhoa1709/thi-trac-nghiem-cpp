#include "models/sinhvien.h"
#include "models/diemthi.h"
#include <iostream>

SinhVien::SinhVien() : maSinhVien(""), ho(""), ten(""), phai(true), password(""), danhSachDiem(nullptr) {}

SinhVien::SinhVien(const std::string& ma, const std::string& ho, const std::string& ten, 
                   bool phai, const std::string& pass) 
    : maSinhVien(ma), ho(ho), ten(ten), phai(phai), password(pass), danhSachDiem(nullptr) {}

SinhVien::~SinhVien() {
    // danhSachDiem is managed by QuanLyDiem, not directly by SinhVien
    // No cleanup needed here as it's just a pointer to the head of the list
}

bool SinhVien::xacThucMatKhau(const std::string& matKhau) const {
    return password == matKhau;
}

void SinhVien::inThongTin() const {
    std::cout << "MASV: " << maSinhVien 
              << " | Ho: " << ho 
              << " | Ten: " << ten 
              << " | Phai: " << (phai ? "Nam" : "Nu") << std::endl;
}