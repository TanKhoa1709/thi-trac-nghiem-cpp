#ifndef CAUHOI_H
#define CAUHOI_H

#include "define.h"

// Hàm quản lý cây câu hỏi
CauHoi* taoCauHoi(int id, const string& noiDung, const string& a,
                  const string& b, const string& c, const string& d,
                  char dapAnDung);
CauHoi* themCauHoi(CauHoi* goc, CauHoi* cauHoiMoi);
CauHoi* timCauHoi(CauHoi* goc, int id);
CauHoi* xoaCauHoi(CauHoi* goc, int id);
CauHoi* timMin(CauHoi* goc);
void huyBoCay(CauHoi* goc);

// Hàm duyệt cây
void duyetTruoc(CauHoi* goc);
void duyetGiua(CauHoi* goc);
void duyetSau(CauHoi* goc);

// Hàm hiển thị và xử lý câu hỏi
void hienThiCauHoi(CauHoi* cauHoi);
void nhapCauHoi(CauHoi*& goc);
void suaCauHoi(CauHoi* goc);
void hienThiTatCaCauHoi(CauHoi* goc);
int demSoCauHoi(CauHoi* goc);

// Hàm ngẫu nhiên câu hỏi
void layCauHoiNgauNhien(CauHoi* goc, vector<CauHoi*>& dsNgauNhien, int soLuong);
void layTatCaCauHoi(CauHoi* goc, vector<CauHoi*>& danhSach);

// Hàm file
void ghiCauHoiRaFile(CauHoi* goc, const string& tenFile);
void docCauHoiTuFile(CauHoi*& goc, const string& tenFile);

#endif