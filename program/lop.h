#pragma once
#include "sinhvien.h"
#include "define.h"
#include <fstream>

struct Lop
{
  string maLop;
  string tenLop;
  SinhVien *dsSinhVien = nullptr; // tro den root cua SinhVien
};

struct DSLop // mang con tro
{
  int n = 0;
  Lop *ds[MAX_LOP];
};

bool themLop(DSLop &ds, const string &maLop, const string &tenLop);
int timLop(DSLop &ds, const string &maLop);
void xuatDanhSachLop(const DSLop &ds);
void xuatBangDiemTheoMon(DSLop &ds, const string &maMH);
void giaiPhongDSLop(DSLop &ds);

void ghiFileDSLop(const DSLop &ds, const string &filename);
void docFileDSLop(DSLop &ds, const string &filename);
