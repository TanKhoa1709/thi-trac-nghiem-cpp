#include "../../include/manager/quanlydiem.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Constructor
QuanLyDiem::QuanLyDiem(const std::string& maSinhVien) 
    : maSinhVien(maSinhVien) {
    danhSachDiem = DynamicArray<DiemThi>();
    
    loadFromFile();
}

// Destructor
QuanLyDiem::~QuanLyDiem() {
    saveToFile();
    danhSachDiem.clear();
}

// Get all scores as dynamic array
DynamicArray<DiemThi> QuanLyDiem::danhSach() {
    DynamicArray<DiemThi> result;
    
    for (int i = 0; i < danhSachDiem.size(); i++) {
        result.add(danhSachDiem.get(i));
    }
    
    return result;
}

// Find score by subject code
DiemThi* QuanLyDiem::tim(const std::string& maMonHoc) {
    for (int i = 0; i < danhSachDiem.size(); i++) {
        if (danhSachDiem.get(i).getMaMonHoc() == maMonHoc) {
            return &danhSachDiem.get(i);
        }
    }
    return nullptr;
}

// Add new score
bool QuanLyDiem::them(DiemThi& diem) {
    if (!diem.validate()) {
        return false;
    }
    
    // Check if score for this subject already exists
    if (tim(diem.getMaMonHoc()) != nullptr) {
        return false;
    }
    
    danhSachDiem.add(diem);
    return true;
}

// Update existing score
bool QuanLyDiem::sua(DiemThi& diem) {
    if (!diem.validate()) {
        return false;
    }
    
    // Find existing score
    DiemThi* existing = tim(diem.getMaMonHoc());
    if (!existing) {
        return false; // Score doesn't exist
    }
    
    // Update the existing score's data (subject code cannot be changed)
    existing->setDiem(diem.getDiem());
    existing->setChiTietBaiThi(diem.getChiTietBaiThi());

    return true;
}

// Remove score by subject code
bool QuanLyDiem::xoa(const std::string& maMonHoc) {
    DiemThi* score = tim(maMonHoc);
    if (!score) {
        return false;
    }
    
    danhSachDiem.remove(*score);
    return true;
}

// Calculate average score
double QuanLyDiem::tinhDiemTrungBinh() {
    if (danhSachDiem.isEmpty()) {
        return 0.0;
    }
    
    double total = 0.0;
    int count = 0;
    
    for (int i = 0; i < danhSachDiem.size(); i++) {
        DiemThi score = danhSachDiem.get(i);
        total += score.getDiem();
        count++;
    }
    
    return count > 0 ? total / count : 0.0;
}

// Count passed subjects (score >= 5.0)
int QuanLyDiem::demSoMonDau() {
    int count = 0;
    
    for (int i = 0; i < danhSachDiem.size(); i++) {
        DiemThi score = danhSachDiem.get(i);
        if (score.getDiem() >= 5.0) {
            count++;
        }
    }
    
    return count;
}

// Count failed subjects (score < 5.0)
int QuanLyDiem::demSoMonRot() {
    int count = 0;
    
    for (int i = 0; i < danhSachDiem.size(); i++) {
        DiemThi score = danhSachDiem.get(i);
        if (score.getDiem() < 5.0) {
            count++;
        }
    }
    
    return count;
}

// Count total subjects taken
int QuanLyDiem::demSoMonDaThi() {
    return danhSachDiem.size();
}

// Save to file
void QuanLyDiem::saveToFile() {
    std::string filename = "data/diemthi/diemthi_" + maSinhVien + ".txt";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return;
    }
    
    file << danhSachDiem.size() << std::endl;
    
    for (int i = 0; i < danhSachDiem.size(); i++) {
        DiemThi score = danhSachDiem.get(i);
        file << score.getMaMonHoc() << "|"
             << score.getDiem() << "|"
             << score.getChiTietBaiThi() << std::endl;
    }
    
    file.close();
}

// Load from file
void QuanLyDiem::loadFromFile() {
    std::string filename = "data/diemthi/diemthi_" + maSinhVien + ".txt";
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
        DynamicArray<std::string> tokens;
        
        while (std::getline(ss, token, '|')) {
            tokens.add(token);
        }
        
        if (tokens.size() >= 2) {
            std::string subjectCode = tokens[0];
            double score = std::stod(tokens[1]);
            std::string details = tokens.size() > 2 ? tokens[2] : "";

            DiemThi examScore(subjectCode, score, details);
            danhSachDiem.add(examScore);
        }
    }
    
    file.close();
}