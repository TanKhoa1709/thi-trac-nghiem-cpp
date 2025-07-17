#pragma once
#include <string>
#include <fstream>
using namespace std;

struct BaiThiChiTiet // danh sach lien ket don
{
  int idCauHoi;
  char luaChon;
  BaiThiChiTiet *next;
};

struct DiemThi // danh sach lien ket don
{
  string maMH;
  float diem;

  BaiThiChiTiet *chiTiet = nullptr; // tro den root cua BaiThiChiTiet
  DiemThi *next;
};

void themDiemThi(DiemThi *&head, const string &maMH, float diem);
DiemThi *timDiemThi(DiemThi *head, const string &maMH);
void themChiTiet(BaiThiChiTiet *&head, int idCauHoi, char luaChon);
void xuatChiTietBaiThi(BaiThiChiTiet *head);
void giaiPhongChiTiet(BaiThiChiTiet *&head);
void giaiPhongDiemThi(DiemThi *&head);

void ghiFileDiemThi(ofstream &out, DiemThi *head);
void docFileDiemThi(ifstream &in, DiemThi *&head);
