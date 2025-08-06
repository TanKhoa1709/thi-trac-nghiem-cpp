#ifndef SINHVIEN_H
#define SINHVIEN_H

#include <string>

// Forward declaration
class QuanLyDiem;

/**
 * @brief Sinh viên - MASV, HO, TEN, PHAI, password, con trỏ quản lý điểm
 */
class SinhVien {
private:
    std::string maSinhVien; // Mã sinh viên (khóa duy nhất)
    std::string ho; // Họ
    std::string ten; // Tên
    bool phai; // Giới tính (true = Nam, false = Nữ)
    std::string password; // Mật khẩu
    QuanLyDiem *quanLyDiem; // Con trỏ quản lý điểm

public:
    // Constructors & Destructor
    SinhVien();

    SinhVien(const std::string &ma, const std::string &ho, const std::string &ten,
             bool phai, const std::string &pass);

    ~SinhVien();

    // Getters (const methods)
    const std::string &getMaSinhVien() const { return maSinhVien; }
    const std::string &getHo() const { return ho; }
    const std::string &getTen() const { return ten; }
    bool getPhai() const { return phai; }
    const std::string &getPassword() const { return password; }
    QuanLyDiem *getQuanLyDiem() const { return quanLyDiem; }

    // Setters
    void setMaSinhVien(const std::string &ma) { maSinhVien = ma; }
    void setHo(const std::string &h) { ho = h; }
    void setTen(const std::string &t) { ten = t; }
    void setPhai(bool p) { phai = p; }
    void setPassword(const std::string &pass) { password = pass; }
    void setQuanLyDiem(QuanLyDiem *ql) { quanLyDiem = ql; }

    // Business logic methods
    bool validate() const;

    std::string getHoTen() const { return ho + " " + ten; }
    std::string getPhaiBangChu() const { return phai ? "Nam" : "Nữ"; }

    // Operators for LinkedList operations
    bool operator==(SinhVien &other) {
        return maSinhVien == other.maSinhVien;
    }

    bool operator!=(SinhVien &other) {
        return !(*this == other);
    }
};

#endif // SINHVIEN_H
