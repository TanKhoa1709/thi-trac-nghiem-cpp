#include "models/diemthi.h"
#include <iostream>
#include <iomanip>

DiemThi::DiemThi() : maMonHoc(""), diem(0.0), chiTietBaiThi("") {}

DiemThi::DiemThi(const std::string& maMon, double diem, const std::string& chiTiet)
    : maMonHoc(maMon), diem(diem), chiTietBaiThi(chiTiet) {}

DiemThi::~DiemThi() {}

void DiemThi::inDiem() const {
    std::cout << "Mon: " << maMonHoc 
              << " | Diem: " << std::fixed << std::setprecision(2) << diem 
              << " | Chi tiet: " << chiTietBaiThi << std::endl;
}