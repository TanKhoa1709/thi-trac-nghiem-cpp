#include "manager/quanlysinhvien.h"
#include <fstream>
#include <sstream>
#include <vector>

// Constructor
QuanLySinhVien::QuanLySinhVien(const std::string& maLop) 
    : maLop(maLop) {
}

// Destructor
QuanLySinhVien::~QuanLySinhVien() {
    saveToFile();
    danhSachSinhVien.clear();
}

// Get all students as dynamic array
void QuanLySinhVien::danhSach(DynamicArray<SinhVien*>& result) {
    // Clear the result array before adding
    result.clear();
    
    for (int i = 0; i < danhSachSinhVien.size(); i++) {
      SinhVien* student = &danhSachSinhVien.get(i);
      result.add(student);
    }
}

// Find student by ID
SinhVien* QuanLySinhVien::tim(const std::string& maSinhVien) {
    // Manually iterate through the list to compare actual objects
    for (int i = 0; i < danhSachSinhVien.size(); i++) {
        if (danhSachSinhVien.get(i).getMaSinhVien() == maSinhVien) {
            return &danhSachSinhVien.get(i);
        }
    }
    return nullptr;
}

// Add new student
bool QuanLySinhVien::them(SinhVien& sinhVien) {
    if (!sinhVien.validate()) {
        return false;
    }
    
    // Check if student ID already exists
    if (tim(sinhVien.getMaSinhVien()) != nullptr) {
        return false;
    }
    
    danhSachSinhVien.add(sinhVien);
    return true;
}

// Update existing student
bool QuanLySinhVien::sua(SinhVien& sinhVien) {
    if (!sinhVien.validate()) {
        return false;
    }
    
    // Find existing student
    SinhVien* existing = tim(sinhVien.getMaSinhVien());
    if (!existing) {
        return false; // Student doesn't exist
    }
    
    // Update the existing student's data (ID cannot be changed)
    existing->setHo(sinhVien.getHo());
    existing->setTen(sinhVien.getTen());
    existing->setPhai(sinhVien.getPhai());
    existing->setPassword(sinhVien.getPassword());

    return true;
}

// Remove student by ID
bool QuanLySinhVien::xoa(const std::string& maSinhVien) {
    SinhVien* student = tim(maSinhVien);
    if (!student) {
        return false;
    }
    
    danhSachSinhVien.remove(*student);
    return true;
}

// Save to file
void QuanLySinhVien::saveToFile() {
    std::string filename = "data/sinhvien/sinhvien_" + maLop + ".txt";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    file << danhSachSinhVien.size() << std::endl;
    
    for (int i = 0; i < danhSachSinhVien.size(); i++) {
        SinhVien& student = danhSachSinhVien.get(i);
        file << student.getMaSinhVien() << "|"
             << student.getHo() << "|"
             << student.getTen() << "|"
             << (student.getPhai() ? "1" : "0") << "|"
             << student.getPassword() << std::endl;
    }
    
    file.close();
}

// Load from file
void QuanLySinhVien::loadFromFile() {
    std::string filename = "data/sinhvien/sinhvien_" + maLop + ".txt";
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
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
            danhSachSinhVien.add(*student);
        }
    }
    
    file.close();
}