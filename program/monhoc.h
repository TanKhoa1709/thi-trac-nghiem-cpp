#pragma once
#include "cauhoi.h"
#include "define.h"
#include <fstream>

struct MonHoc
{
  string maMH;
  string tenMH;
  CauHoi *dsCauHoi = nullptr; // tro den root cua cay nhi phan CauHoi
};

struct DSMonHoc // danh sach tuyen tinh
{
  int n = 0;
  MonHoc ds[MAX_MONHOC];
};

bool themMonHoc(DSMonHoc &dsmh, const string &maMH, const string &tenMH);
int timMonHoc(DSMonHoc &dsmh, const string &maMH);
bool xoaMonHoc(DSMonHoc &dsmh, const string &maMH);
void suaMonHoc(DSMonHoc &dsmh, const string &maMH, const string &tenMoi);
void xuatDanhSachMonHoc(const DSMonHoc &dsmh);
void giaiPhongDanhSachCauHoi(DSMonHoc &dsmh);

void ghiFileMonHoc(const DSMonHoc &dsmh, const string &filename);
void docFileMonHoc(DSMonHoc &dsmh, const string &filename);