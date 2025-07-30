#include "models/monhoc.h"
#include "manager/quanlycauhoi.h"
#include <iostream>
#include <cstring>

MonHoc::MonHoc() : tenMon(""), quanLyCauHoi(nullptr) {
    maMon[0] = '\0';
}

MonHoc::MonHoc(const char* ma, const std::string& ten) 
    : tenMon(ten), quanLyCauHoi(new QuanLyCauHoi()) {
    setMaMon(ma);
}

MonHoc::~MonHoc() {
    delete quanLyCauHoi;
}

void MonHoc::setMaMon(const char* ma) {
    if (ma != nullptr) {
        strncpy(maMon, ma, 15);
        maMon[15] = '\0'; // Ensure null termination
    }
}

void MonHoc::inThongTinMonHoc() const {
    std::cout << "Ma Mon: " << maMon 
              << " | Ten Mon: " << tenMon;
    if (quanLyCauHoi) {
        std::cout << " | So cau hoi: " << quanLyCauHoi->getSoLuongCauHoi();
    }
    std::cout << std::endl;
}