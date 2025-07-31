#include "manager/quanlylop.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Constructor
QuanLyLop::QuanLyLop() {
    loadFromFile();
}

// Destructor
QuanLyLop::~QuanLyLop() {
    saveToFile();
    
    // Clean up all class pointers
    for (size_t i = 0; i < danhSachLop.getSize(); i++) {
        delete danhSachLop.get(i);
    }
    danhSachLop.clear();
}

// Get all classes as dynamic array
DynamicArray<Lop*> QuanLyLop::danhSach() {
    DynamicArray<Lop*> result;
    
    for (size_t i = 0; i < danhSachLop.getSize(); i++) {
        result.push_back(danhSachLop.get(i));
    }
    
    return result;
}

// Find class by code
Lop* QuanLyLop::tim(const std::string& maLop) {
    for (size_t i = 0; i < danhSachLop.getSize(); i++) {
        if (danhSachLop.get(i)->getMaLop() == maLop) {
            return danhSachLop.get(i);
        }
    }
    return nullptr;
}

// Add new class
bool QuanLyLop::them(Lop* lop) {
    if (!lop || !lop->validate()) {
        return false;
    }
    
    // Check if class code already exists
    if (tim(lop->getMaLop()) != nullptr) {
        return false;
    }
    
    danhSachLop.push_back(lop);
    return true;
}

// Update existing class
bool QuanLyLop::sua(Lop* lop) {
    if (!lop || !lop->validate()) {
        return false;
    }
    
    // Find existing class
    Lop* existing = tim(lop->getMaLop());
    if (!existing) {
        return false; // Class doesn't exist
    }
    
    // Update the existing class's data (only name can be updated, not the code)
    existing->setTenLop(lop->getTenLop());
    
    return true;
}

// Remove class by code
bool QuanLyLop::xoa(const std::string& maLop) {
    for (size_t i = 0; i < danhSachLop.getSize(); i++) {
        if (danhSachLop.get(i)->getMaLop() == maLop) {
            Lop* classToDelete = danhSachLop.get(i);
            
            // Shift elements to remove the class
            for (size_t j = i; j < danhSachLop.getSize() - 1; j++) {
                danhSachLop.set(j, danhSachLop.get(j + 1));
            }
            danhSachLop.pop_back();
            
            delete classToDelete;
            return true;
        }
    }
    return false;
}

// Save to file
void QuanLyLop::saveToFile() {
    std::ofstream file("data/lop.txt");
    
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << "data/lop.txt" << std::endl;
        return;
    }
    
    file << danhSachLop.getSize() << std::endl;
    
    for (size_t i = 0; i < danhSachLop.getSize(); i++) {
        Lop* lop = danhSachLop.get(i);
        file << lop->getMaLop() << "|" << lop->getTenLop() << std::endl;
    }
    
    file.close();
}

// Load from file
void QuanLyLop::loadFromFile() {
    std::ifstream file("data/lop.txt");
    
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
        std::string maLop, tenLop;
        
        if (std::getline(ss, maLop, '|') && std::getline(ss, tenLop)) {
            Lop* lop = new Lop(maLop, tenLop);
            danhSachLop.push_back(lop);
        }
    }
    
    file.close();
}