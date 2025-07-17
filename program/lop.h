#ifndef LOP_H
#define LOP_H

#include "define.h"

// Hàm quản lý lớp
void khoiTaoLop();
int timLop(const string& maLop);
void themLop(const string& maLop, const string& tenLop);
void xoaLop(const string& maLop);
void hienThiDanhSachLop();
void nhapLop();
void suaLop();

// Hàm file
void ghiLopRaFile(const string& tenFile);
void docLopTuFile(const string& tenFile);

// Hàm hiển thị thông tin lớp
void hienThiThongTinLop(const string& maLop);
void hienThiSinhVienTheoLop(const string& maLop);

// Hàm tiện ích
bool kiemTraLopRong(const string& maLop);
int demSoSinhVienTrongLop(const string& maLop);

#endif