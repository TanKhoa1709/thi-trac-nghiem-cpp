#ifndef DIEMTHI_H
#define DIEMTHI_H

#include "define.h"

// Hàm quản lý chi tiết bài thi
ChiTietBaiThi* taoChiTietBaiThi(int idCauHoi, char dapAnChon, bool dung);
void themChiTietBaiThi(ChiTietBaiThi*& danhSach, ChiTietBaiThi* chiTietMoi);
void hienThiChiTietBaiThi(ChiTietBaiThi* danhSach);
void xoaChiTietBaiThi(ChiTietBaiThi*& danhSach);

// Hàm quản lý điểm thi
DiemThi* taoDiemThi(const string& maMon, float diem, const string& ngayThi);
void themDiemThi(DiemThi*& danhSach, DiemThi* diemMoi);
DiemThi* timDiemThi(DiemThi* danhSach, const string& maMon);
void hienThiDanhSachDiem(DiemThi* danhSach);
void xoaDanhSachDiem(DiemThi*& danhSach);

// Hàm tính điểm và xử lý bài thi
float tinhDiem(ChiTietBaiThi* chiTiet);
void luuKetQuaThi(SinhVien* sv, const string& maMon, ChiTietBaiThi* chiTiet);
void xemChiTietBaiThi(SinhVien* sv, const string& maMon);

// Hàm báo cáo và thống kê
void inBangDiemTheoLop(const string& maLop, const string& maMon);
void inBangDiemTongKet(const string& maLop);
void thongKeDiemTheoMon(const string& maMon);

// Hàm file
void ghiDiemRaFile(DiemThi* danhSach, const string& tenFile);
void docDiemTuFile(DiemThi*& danhSach, const string& tenFile);

#endif