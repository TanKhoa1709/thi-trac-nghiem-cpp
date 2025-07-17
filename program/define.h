#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

using namespace std;

// Định nghĩa hằng số
const int MAX_LOP = 100;
const int MAX_CAU_HOI = 4;
const int THOI_GIAN_THI = 900; // 15 phút = 900 giây

// Cấu trúc câu hỏi
struct CauHoi {
    int id;
    string noiDung;
    string dapAnA, dapAnB, dapAnC, dapAnD;
    char dapAnDung;
    CauHoi* trai;
    CauHoi* phai;
    
    CauHoi() : id(0), dapAnDung('A'), trai(nullptr), phai(nullptr) {}
};

// Cấu trúc chi tiết bài thi
struct ChiTietBaiThi {
    int idCauHoi;
    char dapAnChon;
    bool dung;
    ChiTietBaiThi* tiep;
    
    ChiTietBaiThi() : idCauHoi(0), dapAnChon('A'), dung(false), tiep(nullptr) {}
};

// Cấu trúc điểm thi
struct DiemThi {
    string maMon;
    float diem;
    string ngayThi;
    ChiTietBaiThi* chiTiet;
    DiemThi* tiep;
    
    DiemThi() : diem(0.0f), chiTiet(nullptr), tiep(nullptr) {}
};

// Cấu trúc sinh viên
struct SinhVien {
    string maSV;
    string ho;
    string ten;
    bool gioiTinh; // true = nam, false = nữ
    string sdt;
    DiemThi* danhSachDiem;
    SinhVien* tiep;
    
    SinhVien() : gioiTinh(true), danhSachDiem(nullptr), tiep(nullptr) {}
};

// Cấu trúc lớp
struct Lop {
    string maLop;
    string tenLop;
    SinhVien* danhSachSV;
    
    Lop() : danhSachSV(nullptr) {}
};

// Cấu trúc môn học
struct MonHoc {
    string maMon;
    string tenMon;
    CauHoi* gocCauHoi;
    MonHoc* tiep;
    
    MonHoc() : gocCauHoi(nullptr), tiep(nullptr) {}
};

// Biến toàn cục
extern Lop danhSachLop[MAX_LOP];
extern int soLuongLop;
extern MonHoc* danhSachMonHoc;
extern bool hetGio;
extern int thoiGianConLai;

// Hàm tiện ích
void xuongDong();
void thoat();
void dungManHinh();
void xoaManHinh();
string layNgayHienTai();
void inTieuDe(const string& tieuDe);

#endif