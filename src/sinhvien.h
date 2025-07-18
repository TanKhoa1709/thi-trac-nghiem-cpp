#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "define.h"

// Sử dụng cấu trúc từ define.h (linked list)
// struct SinhVien đã được định nghĩa trong define.h

// Hàm quản lý sinh viên cơ bản
void khoiTaoSinhVien();
SinhVien* taoSinhVien(const string& maSV, const string& ho, const string& ten,
                      const string& maLop, const string& matKhau);
void themSinhVien(SinhVien*& danhSach, SinhVien* sinhVienMoi);
SinhVien* timSinhVien(SinhVien* danhSach, const string& maSV);
void xoaSinhVien(SinhVien*& danhSach, const string& maSV);
void hienThiDanhSachSinhVien(SinhVien* danhSach);
void xoaToanBoSinhVien(SinhVien*& danhSach);
int demSoSinhVienTrongLop(const string& maLop);

// Hàm đăng nhập và xác thực
SinhVien* dangNhap(const string& maSV, const string& matKhau);
SinhVien* dangNhapSinhVien();

// Hàm quản lý sinh viên với UI
void quanLySinhVien();
void themSinhVienMoi();
void hienThiSinhVienTheoLop();
void timKiemSinhVien();
void xoaSinhVienTheoMa();
void hienThiTatCaSinhVien();

// Hàm thi và học tập
void lamBaiThi(SinhVien* sv);
void xemKetQuaThi(SinhVien* sv);
void xemChiTietBaiThi(SinhVien* sv);

// Hàm file I/O
void ghiSinhVienRaFile(const string& tenFile);
void docSinhVienTuFile(const string& tenFile);

// Hàm tiện ích cho sinh viên
SinhVien* timSinhVienTrongTatCaLop(const string& maSV);
void capNhatThongTinSinhVien(SinhVien* sv);
bool kiemTraMaSinhVienTonTai(const string& maSV);

#endif
