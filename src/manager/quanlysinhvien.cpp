#include "manager/quanlysinhvien.h"
// #include <iostream>
#include <algorithm>
#include <fstream>
#include <functional>

QuanLySinhVien::QuanLySinhVien() {}

QuanLySinhVien::~QuanLySinhVien() {
    // Clean up all SinhVien pointers
    danhSachSinhVien.forEach([](SinhVien* const& sv) {
        delete sv;
    });
    danhSachSinhVien.clear();
}

bool QuanLySinhVien::themSinhVien(SinhVien* sinhVien) {
    if (sinhVien == nullptr) {
        // Removed: std::cout << "Loi: SinhVien pointer la null!" << std::endl;
        return false;
    }
    
    // Removed: std::cout << "Kiem tra sinh vien co ma: ..."
    
    // Check if student already exists
    SinhVien* existing = timSinhVien(sinhVien->getMaSinhVien());
    if (existing != nullptr) {
        // Removed: std::cout << "Sinh vien da ton tai voi ma: ..."
        return false; // Student already exists
    }
    
    // Removed: std::cout << "Dang them sinh vien vao LinkedList..."
    bool result = danhSachSinhVien.add(sinhVien);
    // Removed: std::cout << "Ket qua them: ..."
    // Removed: std::cout << "So luong sinh vien sau khi them: ..."
    
    return result;
}

bool QuanLySinhVien::xoaSinhVien(const std::string& maSinhVien) {
    SinhVien* found = timSinhVien(maSinhVien);
    if (found == nullptr) {
        return false;
    }
    
    bool removed = danhSachSinhVien.remove(found);
    if (removed) {
        delete found;
    }
    return removed;
}

SinhVien* QuanLySinhVien::timSinhVien(const std::string& maSinhVien) {
    // Manual search since we need to compare by maSinhVien
    for (int i = 0; i < danhSachSinhVien.size(); i++) {
        SinhVien** svPtr = danhSachSinhVien.get(i);
        if (svPtr && *svPtr && (*svPtr)->getMaSinhVien() == maSinhVien) {
            return *svPtr;
        }
    }
    return nullptr;
}

SinhVien* QuanLySinhVien::dangNhap(const std::string& maSinhVien, const std::string& matKhau) {
    SinhVien* sinhVien = timSinhVien(maSinhVien);
    if (sinhVien && sinhVien->xacThucMatKhau(matKhau)) {
        return sinhVien;
    }
    return nullptr;
}

void QuanLySinhVien::inDanhSachSinhVien() const {
    if (danhSachSinhVien.isEmpty()) {
        // Removed: std::cout << "Danh sach sinh vien trong!" << std::endl;
        return;
    }
    
    // Removed: std::cout << "=== DANH SACH SINH VIEN ===" << std::endl;
    danhSachSinhVien.forEach([](SinhVien* const& sv) {
        if (sv) {
            sv->inThongTin();
        }
    });
    // Removed: std::cout << "Tong so sinh vien: ..."
}

void QuanLySinhVien::saveToFile() const {
    std::ofstream file(SINHVIEN_DATA_FILE);
    if (!file.is_open()) {
        // Removed: std::cerr << "Khong the mo file ... de ghi!"
        return;
    }
    
    file << danhSachSinhVien.size() << std::endl;
    
    danhSachSinhVien.forEach([&file](SinhVien* const& sv) {
        if (sv) {
            file << sv->getMaSinhVien() << "|"
                 << sv->getHo() << "|"
                 << sv->getTen() << "|"
                 << (sv->getPhai() ? "1" : "0") << "|"
                 << sv->getPassword() << std::endl;
        }
    });
    
    file.close();
}

void QuanLySinhVien::loadFromFile() {
    std::ifstream file(SINHVIEN_DATA_FILE);
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
        
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);
        size_t pos4 = line.find('|', pos3 + 1);
        
        if (pos1 != std::string::npos && pos2 != std::string::npos && 
            pos3 != std::string::npos && pos4 != std::string::npos) {
            
            std::string maSV = line.substr(0, pos1);
            std::string ho = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string ten = line.substr(pos2 + 1, pos3 - pos2 - 1);
            bool phai = (line.substr(pos3 + 1, pos4 - pos3 - 1) == "1");
            std::string password = line.substr(pos4 + 1);
            
            SinhVien* sv = new SinhVien(maSV, ho, ten, phai, password);
            themSinhVien(sv);
        }
    }
    
    file.close();
}

void QuanLySinhVien::forEach(std::function<void(SinhVien*)> callback) const {
    danhSachSinhVien.forEach([&callback](SinhVien* const& sv) {
        callback(sv);
    });
}


// bool nhapSinhVienVaoLop(const std::string& maLop) {
//     Lop* lop = timLop(maLop);
//     if (lop == nullptr) {
//         std::cout << "Khong tim thay lop co ma: " << maLop << std::endl;
//         return false;
//     }
    
//     // Check if QuanLySinhVien is initialized
//     if (lop->getQuanLySinhVien() == nullptr) {
//         std::cout << "Loi: QuanLySinhVien chua duoc khoi tao cho lop " << maLop << std::endl;
//         return false;
//     }
    
//     std::cout << "Nhap thong tin sinh vien cho lop " << maLop << ":" << std::endl;
    
//     std::string maSV, ho, ten, password;
//     bool phai;
//     char phaiChar;
    
//     std::cout << "Ma sinh vien: ";
//     std::cin >> maSV;
//     std::cout << "Ho: ";
//     std::cin >> ho;
//     std::cout << "Ten: ";
//     std::cin >> ten;
//     std::cout << "Phai (M/F): ";
//     std::cin >> phaiChar;
//     phai = (phaiChar == 'M' || phaiChar == 'm');
//     std::cout << "Password: ";
//     std::cin >> password;
    
//     SinhVien* sinhVienMoi = new SinhVien(maSV, ho, ten, phai, password);
    
//     std::cout << "Dang them sinh vien: " << maSV << " vao lop " << maLop << std::endl;
    
//     if (lop->getQuanLySinhVien()->themSinhVien(sinhVienMoi)) {
//         std::cout << "Them sinh vien thanh cong!" << std::endl;
//         std::cout << "So sinh vien trong lop: " << lop->getQuanLySinhVien()->getSoLuongSinhVien() << std::endl;
        
//         // Auto-save student data to file
//         #ifdef _WIN32
//             std::string studentFile = "data\\students\\" + maLop + "_students.txt";
//         #else
//             std::string studentFile = "data/students/" + maLop + "_students.txt";
//         #endif
//         lop->getQuanLySinhVien()->saveToFile(studentFile);
//         std::cout << "Du lieu sinh vien da duoc luu vao file: " << studentFile << std::endl;
        
//         return true;
//     } else {
//         delete sinhVienMoi;
//         std::cout << "Them sinh vien that bai! (Co the sinh vien da ton tai)" << std::endl;
//         return false;
//     }
// }