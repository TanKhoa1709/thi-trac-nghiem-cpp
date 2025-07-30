#include "manager/quanlylop.h"
#include "manager/quanlysinhvien.h"
#include "models/sinhvien.h"
// #include <iostream>
#include <fstream>

QuanLyLop::QuanLyLop() {}

QuanLyLop::~QuanLyLop() {
    // Clean up all Lop pointers
    for (int i = 0; i < danhSachLop.size(); i++) {
        delete danhSachLop[i];
    }
}

bool QuanLyLop::themLop(const std::string& maLop, const std::string& tenLop) {
    // Check if class already exists
    if (timLop(maLop) != nullptr) {
        return false; // Class already exists
    }
    
    Lop* lopMoi = new Lop(maLop, tenLop);
    danhSachLop.add(lopMoi);

    // Auto-save class data to file
    saveToFile();
    // Removed: std::cout << "Du lieu lop da duoc luu vao file ..."
    
    return true;
}

bool QuanLyLop::xoaLop(const std::string& maLop) {
    for (int i = 0; i < danhSachLop.size(); i++) {
        if (danhSachLop[i] && danhSachLop[i]->getMaLop() == maLop) {
            delete danhSachLop[i];
            danhSachLop.removeAt(i);
            return true;
        }
    }
    return false;
}

Lop* QuanLyLop::timLop(const std::string& maLop) {
    for (int i = 0; i < danhSachLop.size(); i++) {
        if (danhSachLop[i] && danhSachLop[i]->getMaLop() == maLop) {
            return danhSachLop[i];
        }
    }
    return nullptr;
}

void QuanLyLop::inDanhSachLop() const {
    if (danhSachLop.isEmpty()) {
        // Removed: std::cout << "Danh sach lop trong!" << std::endl;
        return;
    }
    
    // Removed: std::cout << "=== DANH SACH LOP ===" << std::endl;
    for (int i = 0; i < danhSachLop.size(); i++) {
        if (danhSachLop[i]) {
            danhSachLop[i]->inThongTinLop();
        }
    }
    // Removed: std::cout << "Tong so lop: ..."
}

void QuanLyLop::saveToFile() const {
    std::ofstream file(LOP_DATA_FILE);
    if (!file.is_open()) {
        // Removed: std::cerr << "Khong the mo file ... de ghi!"
        return;
    }
    
    file << danhSachLop.size() << std::endl;
    
    for (int i = 0; i < danhSachLop.size(); i++) {
        if (danhSachLop[i]) {
            file << danhSachLop[i]->getMaLop() << "|" 
                 << danhSachLop[i]->getTenLop() << std::endl;
            
            // Save students in this class to separate file
            if (danhSachLop[i]->getQuanLySinhVien()) {
                std::string studentFile = "data/students/" + danhSachLop[i]->getMaLop() + "_students.txt";
                danhSachLop[i]->getQuanLySinhVien()->saveToFile(studentFile);
            }
        }
    }
    
    file.close();
}

void QuanLyLop::loadFromFile() {
    std::ifstream file(LOP_DATA_FILE);
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
            std::string maLop = line.substr(0, pos);
            std::string tenLop = line.substr(pos + 1);
            
            themLop(maLop, tenLop);
            
            // Load students for this class
            Lop* lop = timLop(maLop);
            if (lop && lop->getQuanLySinhVien()) {
                #ifdef _WIN32
                    std::string studentFile = "data\\students\\" + maLop + "_students.txt";
                #else
                    std::string studentFile = "data/students/" + maLop + "_students.txt";
                #endif
                lop->getQuanLySinhVien()->loadFromFile(studentFile);
            }
        }
    }
    
    file.close();
}

int QuanLyLop::getSoLuongLop() const {
    return danhSachLop.size();
}