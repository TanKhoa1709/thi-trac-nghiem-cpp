#include "models/cauhoi.h"
#include <iostream>

CauHoi::CauHoi() : maCauHoi(0), noiDung(""), luaChonA(""), luaChonB(""), 
                   luaChonC(""), luaChonD(""), dapAnDung('A') {}

CauHoi::CauHoi(int ma, const std::string& noiDung, const std::string& a, const std::string& b,
               const std::string& c, const std::string& d, char dapAn)
    : maCauHoi(ma), noiDung(noiDung), luaChonA(a), luaChonB(b), 
      luaChonC(c), luaChonD(d), dapAnDung(dapAn) {}

CauHoi::~CauHoi() {}

void CauHoi::inCauHoi() const {
    std::cout << "Cau " << maCauHoi << ": " << noiDung << std::endl;
    std::cout << "A. " << luaChonA << std::endl;
    std::cout << "B. " << luaChonB << std::endl;
    std::cout << "C. " << luaChonC << std::endl;
    std::cout << "D. " << luaChonD << std::endl;
    std::cout << "Dap an dung: " << dapAnDung << std::endl;
}

bool CauHoi::kiemTraDapAn(char dapAnChon) const {
    return (dapAnChon == dapAnDung);
}