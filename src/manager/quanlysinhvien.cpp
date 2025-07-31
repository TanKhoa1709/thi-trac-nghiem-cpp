#include "manager/quanlysinhvien.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Constructor
QuanLySinhVien::QuanLySinhVien(const std::string& maLop) 
    : maLop(maLop) {
    loadFromFile();
}

// Destructor
QuanLySinhVien::~QuanLySinhVien() {
    saveToFile();
    
    // Clean up all student pointers
    auto current = danhSachSinhVien.first();
    while (current != nullptr) {
        SinhVien* student = *current;
        danhSachSinhVien.removeFirst();
        delete student;
        current = danhSachSinhVien.first();
    }
}

// Get all students as dynamic array
DynamicArray<SinhVien*> QuanLySinhVien::danhSach() {
    DynamicArray<SinhVien*> result;
    
    for (int i = 0; i < danhSachSinhVien.size(); i++) {
        result.push_back(danhSachSinhVien.get(i));
    }
    
    return result;
}

// Find student by ID
SinhVien* QuanLySinhVien::tim(const std::string& maSinhVien) {
    // Manually iterate through the list to compare actual objects
    for (int i = 0; i < danhSachSinhVien.size(); i++) {
        SinhVien* student = danhSachSinhVien.get(i);
        if (student && student->getMaSinhVien() == maSinhVien) {
            return student;
        }
    }
    return nullptr;
}

// Add new student
bool QuanLySinhVien::them(SinhVien* sinhVien) {
    if (!sinhVien || !sinhVien->validate()) {
        return false;
    }
    
    // Check if student ID already exists
    if (tim(sinhVien->getMaSinhVien()) != nullptr) {
        return false;
    }
    
    return danhSachSinhVien.add(sinhVien);
}

// Update existing student
bool QuanLySinhVien::sua(SinhVien* sinhVien) {
    if (!sinhVien || !sinhVien->validate()) {
        return false;
    }
    
    // Find existing student
    SinhVien* existing = tim(sinhVien->getMaSinhVien());
    if (!existing) {
        return false; // Student doesn't exist
    }
    
    // Update the existing student's data (ID cannot be changed)
    existing->setHo(sinhVien->getHo());
    existing->setTen(sinhVien->getTen());
    existing->setPhai(sinhVien->getPhai());
    existing->setPassword(sinhVien->getPassword());
    
    return true;
}

// Remove student by ID
bool QuanLySinhVien::xoa(const std::string& maSinhVien) {
    SinhVien* student = tim(maSinhVien);
    if (!student) {
        return false;
    }
    
    bool removed = danhSachSinhVien.remove(student);
    if (removed) {
        delete student;
    }
    
    return removed;
}

// Save to file
void QuanLySinhVien::saveToFile() {
    std::string filename = "data/sinhvien/sinhvien_" + maLop + ".txt";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return;
    }
    
    file << danhSachSinhVien.size() << std::endl;
    
    for (int i = 0; i < danhSachSinhVien.size(); i++) {
        SinhVien* student = danhSachSinhVien.get(i);
        if (student) {
            file << student->getMaSinhVien() << "|"
                 << student->getHo() << "|"
                 << student->getTen() << "|"
                 << (student->getPhai() ? "1" : "0") << "|"
                 << student->getPassword() << std::endl;
        }
    }
    
    file.close();
}

// Load from file
void QuanLySinhVien::loadFromFile() {
    std::string filename = "data/sinhvien/sinhvien_" + maLop + ".txt";
    std::ifstream file(filename);
    
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
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 5) {
            std::string maSV = tokens[0];
            std::string ho = tokens[1];
            std::string ten = tokens[2];
            bool phai = (tokens[3] == "1");
            std::string password = tokens[4];
            
            SinhVien* student = new SinhVien(maSV, ho, ten, phai, password);
            danhSachSinhVien.add(student);
        }
    }
    
    file.close();
}