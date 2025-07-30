#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <vector>
#include "manager/quanlycauhoi.h"

QuanLyCauHoi::QuanLyCauHoi() {
    // Seed random number generator
    std::srand(std::time(nullptr));
}

QuanLyCauHoi::~QuanLyCauHoi() {}

bool QuanLyCauHoi::themCauHoi(const CauHoi& cauHoi) {
    bool result = cayQuanLyCauHoi.insert(cauHoi);
    
    if (result) {
        std::cout << "Cau hoi da duoc them vao BST thanh cong!" << std::endl;
    }
    
    return result;
}

bool QuanLyCauHoi::xoaCauHoi(int maCauHoi) {
    CauHoi temp;
    temp.setMaCauHoi(maCauHoi);
    return cayQuanLyCauHoi.remove(temp);
}

CauHoi* QuanLyCauHoi::timCauHoi(int maCauHoi) {
    CauHoi temp;
    temp.setMaCauHoi(maCauHoi);
    return cayQuanLyCauHoi.search(temp);
}

int QuanLyCauHoi::taoMaCauHoiNgauNhien() {
    int maCauHoi;
    do {
        maCauHoi = std::rand() % 100000 + 1; // Random ID from 1 to 100000
    } while (timCauHoi(maCauHoi) != nullptr); // Ensure uniqueness
    
    return maCauHoi;
}

void QuanLyCauHoi::layNgauNhienCauHoi(int soLuong) {
    if (cayQuanLyCauHoi.isEmpty()) {
        std::cout << "Khong co cau hoi nao!" << std::endl;
        return;
    }
    
    std::vector<CauHoi> tatCaCauHoi = cayQuanLyCauHoi.inOrder();
    
    if (soLuong > static_cast<int>(tatCaCauHoi.size())) {
        std::cout << "Khong du cau hoi! Chi co " << tatCaCauHoi.size() << " cau hoi." << std::endl;
        soLuong = tatCaCauHoi.size();
    }
    
    // Shuffle and select random questions
    std::random_shuffle(tatCaCauHoi.begin(), tatCaCauHoi.end());
    
    std::cout << "=== " << soLuong << " CAU HOI NGAU NHIEN ===" << std::endl;
    for (int i = 0; i < soLuong; i++) {
        std::cout << "\n--- Cau " << (i + 1) << " ---" << std::endl;
        tatCaCauHoi[i].inCauHoi();
    }
}

void QuanLyCauHoi::inTatCaCauHoi() const {
    if (cayQuanLyCauHoi.isEmpty()) {
        std::cout << "Khong co cau hoi nao!" << std::endl;
        return;
    }
    
    std::vector<CauHoi> tatCaCauHoi = cayQuanLyCauHoi.inOrder();
    
    std::cout << "=== TAT CA CAU HOI ===" << std::endl;
    for (size_t i = 0; i < tatCaCauHoi.size(); i++) {
        std::cout << "\n--- Cau " << (i + 1) << " ---" << std::endl;
        tatCaCauHoi[i].inCauHoi();
    }
    std::cout << "\nTong so cau hoi: " << cayQuanLyCauHoi.size() << std::endl;
}

bool QuanLyCauHoi::xoaCauHoiAnToan(int maCauHoi) {
    // TODO: Check if question is used in any exam before deleting
    // For now, just delete normally
    std::cout << "Canh bao: Kiem tra xem cau hoi co dang duoc su dung trong bai thi khong!" << std::endl;
    return xoaCauHoi(maCauHoi);
}

void QuanLyCauHoi::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    std::vector<CauHoi> allQuestions = cayQuanLyCauHoi.inOrder();
    file << allQuestions.size() << std::endl;
    
    for (const auto& ch : allQuestions) {
        file << ch.getMaCauHoi() << "|"
             << ch.getNoiDung() << "|"
             << ch.getLuaChonA() << "|"
             << ch.getLuaChonB() << "|"
             << ch.getLuaChonC() << "|"
             << ch.getLuaChonD() << "|"
             << ch.getDapAnDung() << std::endl;
    }
    
    file.close();
}

void QuanLyCauHoi::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    
    int count;
    file >> count;
    file.ignore();
    
    for (int i = 0; i < count; i++) {
        std::string line;
        std::getline(file, line);
        
        // Parse the line with multiple delimiters
        std::vector<std::string> parts;
        std::string current = "";
        
        for (char c : line) {
            if (c == '|') {
                parts.push_back(current);
                current = "";
            } else {
                current += c;
            }
        }
        parts.push_back(current); // Add the last part
        
        if (parts.size() >= 7) {
            int maCauHoi = std::stoi(parts[0]);
            std::string noiDung = parts[1];
            std::string luaChonA = parts[2];
            std::string luaChonB = parts[3];
            std::string luaChonC = parts[4];
            std::string luaChonD = parts[5];
            char dapAn = parts[6][0];
            
            CauHoi ch(maCauHoi, noiDung, luaChonA, luaChonB, luaChonC, luaChonD, dapAn);
            themCauHoi(ch);
        }
    }
    
    file.close();
}