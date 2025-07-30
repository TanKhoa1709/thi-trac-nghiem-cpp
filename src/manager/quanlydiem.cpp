#include "manager/quanlydiem.h"
#include <iostream>
#include <iomanip>

QuanLyDiem::QuanLyDiem() {}

QuanLyDiem::~QuanLyDiem() {
    // Clean up all DiemThi pointers
    danhSachDiem.forEach([](DiemThi* const& dt) {
        delete dt;
    });
    danhSachDiem.clear();
}

bool QuanLyDiem::themDiem(DiemThi* diem) {
    if (diem == nullptr) {
        return false;
    }
    
    // Check if score for this subject already exists
    if (timDiem(diem->getMaMonHoc()) != nullptr) {
        return false; // Score already exists for this subject
    }
    
    return danhSachDiem.add(diem);
}

bool QuanLyDiem::xoaDiem(const std::string& maMonHoc) {
    DiemThi* found = timDiem(maMonHoc);
    if (found == nullptr) {
        return false;
    }
    
    bool removed = danhSachDiem.remove(found);
    if (removed) {
        delete found;
    }
    return removed;
}

DiemThi* QuanLyDiem::timDiem(const std::string& maMonHoc) {
    // Manual search since we need to compare by maMonHoc
    for (int i = 0; i < danhSachDiem.size(); i++) {
        DiemThi** dtPtr = danhSachDiem.get(i);
        if (dtPtr && *dtPtr && (*dtPtr)->getMaMonHoc() == maMonHoc) {
            return *dtPtr;
        }
    }
    return nullptr;
}

void QuanLyDiem::inDanhSachDiem() const {
    if (danhSachDiem.isEmpty()) {
        std::cout << "Danh sach diem trong!" << std::endl;
        return;
    }
    
    std::cout << "=== DANH SACH DIEM ===" << std::endl;
    danhSachDiem.forEach([](DiemThi* const& dt) {
        if (dt) {
            dt->inDiem();
        }
    });
    std::cout << "Tong so mon: " << danhSachDiem.size() << std::endl;
}

void QuanLyDiem::inBangDiem() const {
    if (danhSachDiem.isEmpty()) {
        std::cout << "Chua co diem thi!" << std::endl;
        return;
    }
    
    std::cout << "=== BANG DIEM ===" << std::endl;
    std::cout << std::left << std::setw(15) << "Ma Mon Hoc" 
              << std::setw(10) << "Diem" << std::endl;
    std::cout << std::string(25, '-') << std::endl;
    
    danhSachDiem.forEach([](DiemThi* const& dt) {
        if (dt) {
            std::cout << std::left << std::setw(15) << dt->getMaMonHoc()
                      << std::setw(10) << std::fixed << std::setprecision(2) 
                      << dt->getDiem() << std::endl;
        }
    });
}

double QuanLyDiem::tinhDiemTrungBinh() const {
    if (danhSachDiem.isEmpty()) {
        return 0.0;
    }
    
    double tongDiem = 0.0;
    danhSachDiem.forEach([&tongDiem](DiemThi* const& dt) {
        if (dt) {
            tongDiem += dt->getDiem();
        }
    });
    
    return tongDiem / danhSachDiem.size();
}

int QuanLyDiem::demSoMonDau() const {
    int count = 0;
    danhSachDiem.forEach([&count](DiemThi* const& dt) {
        if (dt && dt->getDiem() >= 5.0) {
            count++;
        }
    });
    return count;
}

int QuanLyDiem::demSoMonRot() const {
    int count = 0;
    danhSachDiem.forEach([&count](DiemThi* const& dt) {
        if (dt && dt->getDiem() < 5.0) {
            count++;
        }
    });
    return count;
}