#pragma once
#include "diemthi.h"
#include <string>
#include <fstream>
using namespace std;

struct SinhVien // danh sach lien ket don
{
  string maSV;
  string ho;
  string ten;
  string phai;
  string password;

  DiemThi *dsDiem = nullptr; // tro den root cua DiemThi
  SinhVien *next = nullptr;
};

void themSinhVien(SinhVien *&head, const SinhVien &sv);
SinhVien *timSinhVien(SinhVien *head, const string &maSV);
void xuatDanhSachSinhVien(SinhVien *head);
void giaiPhongSinhVien(SinhVien *&head);
bool dangNhap(SinhVien *ds, const string &maSV, const string &pass);

void ghiFileSinhVien(ofstream &out, SinhVien *head);
void docFileSinhVien(ifstream &in, SinhVien *&head);
