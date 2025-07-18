#ifndef MONHOC_H
#define MONHOC_H

#include "define.h"
#include "cauhoi.h"

// Sử dụng cấu trúc từ define.h (linked list)
// struct MonHoc đã được định nghĩa trong define.h

// Hàm quản lý môn học
void khoiTaoMonHoc();
MonHoc* taoMonHoc(const string& maMon, const string& tenMon);
void themMonHoc(const string& maMon, const string& tenMon);
MonHoc* timMonHoc(const string& maMon);
void xoaMonHoc(const string& maMon);
void hienThiDanhSachMonHoc();
void xoaToanBoMonHoc();

// Hàm file
void ghiMonHocRaFile(const string& tenFile);
void docMonHocTuFile(const string& tenFile);

#endif