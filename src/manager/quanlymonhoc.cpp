#include "manager/quanlymonhoc.h"
#include "manager/quanlycauhoi.h"
// #include <iostream>
#include <cstring>
#include <fstream>

const std::string MONHOC_DATA_FILE = "data/subjects.txt";

QuanLyMonHoc::QuanLyMonHoc() {}

QuanLyMonHoc::~QuanLyMonHoc() {
    // Clean up all MonHoc pointers
    for (int i = 0; i < danhSachMonHoc.size(); i++) {
        delete danhSachMonHoc[i];
    }
}

bool QuanLyMonHoc::themMonHoc(const char* maMon, const std::string& tenMon) {
    // Check if subject already exists
    if (timMonHoc(maMon) != nullptr) {
        return false; // Subject already exists
    }
    
    MonHoc* monHocMoi = new MonHoc(maMon, tenMon);
    danhSachMonHoc.add(monHocMoi);
    
        // Auto-save subject data to file
        saveToFile();
        // Removed: std::cout << "Du lieu mon hoc da duoc luu vao file ..."
    
    return true;
}

bool QuanLyMonHoc::xoaMonHoc(const char* maMon) {
    for (int i = 0; i < danhSachMonHoc.size(); i++) {
        if (danhSachMonHoc[i] && strcmp(danhSachMonHoc[i]->getMaMon(), maMon) == 0) {
            delete danhSachMonHoc[i];
            danhSachMonHoc.removeAt(i);
            return true;
        }
    }
    return false;
}

MonHoc* QuanLyMonHoc::timMonHoc(const char* maMon) {
    for (int i = 0; i < danhSachMonHoc.size(); i++) {
        if (danhSachMonHoc[i] && strcmp(danhSachMonHoc[i]->getMaMon(), maMon) == 0) {
            return danhSachMonHoc[i];
        }
    }
    return nullptr;
}

void QuanLyMonHoc::inDanhSachMonHoc() const {
    if (danhSachMonHoc.isEmpty()) {
        // Removed: std::cout << "Danh sach mon hoc trong!" << std::endl;
        return;
    }
    
    // Removed: std::cout << "=== DANH SACH MON HOC ===" << std::endl;
    for (int i = 0; i < danhSachMonHoc.size(); i++) {
        if (danhSachMonHoc[i]) {
            danhSachMonHoc[i]->inThongTinMonHoc();
        }
    }
    // Removed: std::cout << "Tong so mon hoc: ..."
}

void QuanLyMonHoc::saveToFile() const {
    std::ofstream file(MONHOC_DATA_FILE);
    if (!file.is_open()) {
        // Removed: std::cerr << "Khong the mo file ... de ghi!"
        return;
    }
    
    file << danhSachMonHoc.size() << std::endl;
    
    for (int i = 0; i < danhSachMonHoc.size(); i++) {
        if (danhSachMonHoc[i]) {
            file << danhSachMonHoc[i]->getMaMon() << "|" 
                 << danhSachMonHoc[i]->getTenMon() << std::endl;
            
            // Save questions for this subject to separate file
            if (danhSachMonHoc[i]->getQuanLyCauHoi()) {
                std::string questionFile = "data/questions/" + std::string(danhSachMonHoc[i]->getMaMon()) + "_questions.txt";
                danhSachMonHoc[i]->getQuanLyCauHoi()->saveToFile(questionFile);
            }
        }
    }
    
    file.close();
}

void QuanLyMonHoc::loadFromFile() {
    std::ifstream file(MONHOC_DATA_FILE);
    if (!file.is_open()) {
        // Removed: std::cerr << "Khong the mo file ... de doc!"
        return;
    }
    
    int count;
    file >> count;
    file.ignore();
    
    for (int i = 0; i < count; i++) {
        std::string line;
        std::getline(file, line);
        
        size_t pos = line.find('|');
        if (pos != std::string::npos) {
            std::string maMon = line.substr(0, pos);
            std::string tenMon = line.substr(pos + 1);
            
            themMonHoc(maMon.c_str(), tenMon);
            
            // Load questions for this subject
            MonHoc* monHoc = timMonHoc(maMon.c_str());
            if (monHoc && monHoc->getQuanLyCauHoi()) {
                std::string questionFile = "data/questions/" + maMon + "_questions.txt";
                monHoc->getQuanLyCauHoi()->loadFromFile(questionFile);
            }
        }
    }
    
    file.close();
}

int QuanLyMonHoc::getSoLuongMonHoc() const {
    return danhSachMonHoc.size();
}