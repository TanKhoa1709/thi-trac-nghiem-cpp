#pragma once
#include <string>
#include <fstream>
using namespace std;

struct CauHoi // cay nhi phan
{
  int id;
  string noiDung;
  string A, B, C, D;
  char dapAn;

  CauHoi *left;
  CauHoi *right;
};

CauHoi *themCauHoi(CauHoi *root, CauHoi *node);
CauHoi *taoCauHoi(int id, const string &nd, const string &A, const string &B,
                  const string &C, const string &D, char dapAn);
CauHoi *timCauHoi(CauHoi *root, int id);
void duyetNLR(CauHoi *root);
void xuatCauHoi(const CauHoi *ch);
void giaiPhongCauHoi(CauHoi *&root);
int demSoLuongCauHoi(CauHoi *root);
void layNgauNhienCauHoi(CauHoi *root, CauHoi **arr, int &index, int soLuong);
bool cauHoiDaDuocDung(int id);

void ghiFileCauHoi(ofstream &out, CauHoi *root);
void docFileCauHoi(ifstream &in, CauHoi *&root);