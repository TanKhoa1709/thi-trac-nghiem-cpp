#include "manager/quanlymonhoc.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

// Constructor
QuanLyMonHoc::QuanLyMonHoc() {
    loadFromFile();
}

// Destructor
QuanLyMonHoc::~QuanLyMonHoc() {
    saveToFile();
    
    // Clean up all subject pointers
    for (size_t i = 0; i < danhSachMonHoc.getSize(); i++) {
        delete danhSachMonHoc.get(i);
    }
    danhSachMonHoc.clear();
}

// Get all subjects as dynamic array
DynamicArray<MonHoc*> QuanLyMonHoc::danhSach() {
    DynamicArray<MonHoc*> result;
    
    for (size_t i = 0; i < danhSachMonHoc.getSize(); i++) {
        result.push_back(danhSachMonHoc.get(i));
    }
    
    return result;
}

// Find subject by code
MonHoc* QuanLyMonHoc::tim(const char* maMon) {
    for (size_t i = 0; i < danhSachMonHoc.getSize(); i++) {
        if (std::strcmp(danhSachMonHoc.get(i)->getMaMon(), maMon) == 0) {
            return danhSachMonHoc.get(i);
        }
    }
    return nullptr;
}

// Add new subject
bool QuanLyMonHoc::them(MonHoc* monHoc) {
    if (!monHoc || !monHoc->validate()) {
        return false;
    }
    
    // Check if subject code already exists
    if (tim(monHoc->getMaMon()) != nullptr) {
        return false;
    }
    
    danhSachMonHoc.push_back(monHoc);
    return true;
}

// Update existing subject
bool QuanLyMonHoc::sua(MonHoc* monHoc) {
    if (!monHoc || !monHoc->validate()) {
        return false;
    }
    
    // Find existing subject
    MonHoc* existing = tim(monHoc->getMaMon());
    if (!existing) {
        return false; // Subject doesn't exist
    }
    
    // Update the existing subject's data (only name can be updated, not the code)
    existing->setTenMon(monHoc->getTenMon());
    
    return true;
}

// Remove subject by code
bool QuanLyMonHoc::xoa(const char* maMon) {
    for (size_t i = 0; i < danhSachMonHoc.getSize(); i++) {
        if (std::strcmp(danhSachMonHoc.get(i)->getMaMon(), maMon) == 0) {
            MonHoc* subjectToDelete = danhSachMonHoc.get(i);
            
            // Shift elements to remove the subject
            for (size_t j = i; j < danhSachMonHoc.getSize() - 1; j++) {
                danhSachMonHoc.set(j, danhSachMonHoc.get(j + 1));
            }
            danhSachMonHoc.pop_back();
            
            delete subjectToDelete;
            return true;
        }
    }
    return false;
}

// Save to file
void QuanLyMonHoc::saveToFile() {
    std::ofstream file("data/monhoc.txt");
    
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << "data/monhoc.txt" << std::endl;
        return;
    }
    
    file << danhSachMonHoc.getSize() << std::endl;
    
    for (size_t i = 0; i < danhSachMonHoc.getSize(); i++) {
        MonHoc* monHoc = danhSachMonHoc.get(i);
        file << monHoc->getMaMon() << "|" << monHoc->getTenMon() << std::endl;
    }
    
    file.close();
}

// Load from file
void QuanLyMonHoc::loadFromFile() {
    std::ifstream file("data/monhoc.txt");
    
    if (!file.is_open()) {
        return; // File doesn't exist yet, that's okay
    }
    
    int count;
    file >> count;
    file.ignore(); // Ignore newline after count
    
    for (int i = 0; i < count; i++) {
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string maMon, tenMon;
        
        if (std::getline(ss, maMon, '|') && std::getline(ss, tenMon)) {
            MonHoc* monHoc = new MonHoc(maMon.c_str(), tenMon);
            danhSachMonHoc.push_back(monHoc);
        }
    }
    
    file.close();
}
