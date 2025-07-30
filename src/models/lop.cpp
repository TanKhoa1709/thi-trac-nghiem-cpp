#include "models/lop.h"
#include "manager/quanlysinhvien.h"
#include <iostream>

Lop::Lop() : maLop(""), tenLop(""), quanLySinhVien(nullptr) {}

Lop::Lop(const std::string& ma, const std::string& ten) 
    : maLop(ma), tenLop(ten), quanLySinhVien(new QuanLySinhVien()) {}

Lop::~Lop() {
    delete quanLySinhVien;
}

void Lop::inThongTinLop() const {
    std::cout << "Ma Lop: " << maLop 
              << " | Ten Lop: " << tenLop;
    if (quanLySinhVien) {
        std::cout << " | So sinh vien: " << quanLySinhVien->getSoLuongSinhVien();
    }
    std::cout << std::endl;
}