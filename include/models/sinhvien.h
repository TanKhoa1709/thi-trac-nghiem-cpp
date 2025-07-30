#ifndef SINHVIEN_H
#define SINHVIEN_H

#include <string>

// Forward declaration
class DiemThi;

/**
 * @brief Sinh viên - MASV, HO, TEN, PHAI, password, con trỏ điểm
 */
class SinhVien {
private:
    std::string maSinhVien;
    std::string ho;
    std::string ten;
    bool phai;                    // true = Nam, false = Nữ
    std::string password;
    DiemThi* danhSachDiem;       // con trỏ điểm

public:
    SinhVien();
    SinhVien(const std::string& ma, const std::string& ho, const std::string& ten, 
             bool phai, const std::string& pass);
    ~SinhVien();

    // Getters/Setters
    std::string getMaSinhVien() const { return maSinhVien; }
    std::string getHo() const { return ho; }
    std::string getTen() const { return ten; }
    bool getPhai() const { return phai; }
    std::string getPassword() const { return password; }
    DiemThi* getDanhSachDiem() const { return danhSachDiem; }

    void setMaSinhVien(const std::string& ma) { maSinhVien = ma; }
    void setHo(const std::string& h) { ho = h; }
    void setTen(const std::string& t) { ten = t; }
    void setPhai(bool p) { phai = p; }
    void setPassword(const std::string& pass) { password = pass; }
    void setDanhSachDiem(DiemThi* ds) { danhSachDiem = ds; }

    // Authentication
    bool xacThucMatKhau(const std::string& matKhau) const;
    
    // Display
    void inThongTin() const;
    
    // Equality operator for LinkedList operations
    bool operator==(const SinhVien& other) const {
        return maSinhVien == other.maSinhVien;
    }
    
    // Equality operator for pointer comparisons in LinkedList
    bool operator==(const SinhVien* other) const {
        return other && maSinhVien == other->maSinhVien;
    }
};

#endif // SINHVIEN_H