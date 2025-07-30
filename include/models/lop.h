#ifndef LOP_H
#define LOP_H

#include <string>

// Forward declaration
class QuanLySinhVien;

/**
 * @brief Lớp học - MALOP, TENLOP, con trỏ sinh viên
 */
class Lop {
private:
    std::string maLop;
    std::string tenLop;
    QuanLySinhVien* quanLySinhVien;  // con trỏ quản lí sinh viên

public:
    Lop();
    Lop(const std::string& ma, const std::string& ten);
    ~Lop();

    // Getters/Setters
    std::string getMaLop() const { return maLop; }
    std::string getTenLop() const { return tenLop; }
    QuanLySinhVien* getQuanLySinhVien() const { return quanLySinhVien; }
    
    void setMaLop(const std::string& ma) { maLop = ma; }
    void setTenLop(const std::string& ten) { tenLop = ten; }
    void setQuanLySinhVien(QuanLySinhVien* ql) { quanLySinhVien = ql; }

    // Basic operations
    void inThongTinLop() const;
};

#endif // LOP_H