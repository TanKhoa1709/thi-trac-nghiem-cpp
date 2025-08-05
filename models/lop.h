#ifndef LOP_H
#define LOP_H

#include <string>

// Forward declaration
class QuanLySinhVien;

/**
 * @brief Lớp học - MALOP, TENLOP, con trỏ quản lý sinh viên
 */
class Lop {
private:
    std::string maLop;                   // Mã lớp học (khóa duy nhất)
    std::string tenLop;                  // Tên lớp học
    QuanLySinhVien* quanLySinhVien;      // Con trỏ quản lý sinh viên

public:
    // Constructors & Destructor
    Lop();
    Lop(const std::string& ma, const std::string& ten);
    ~Lop();

    // Getters (const methods)
    const std::string& getMaLop() const { return maLop; }
    const std::string& getTenLop() const { return tenLop; }
    QuanLySinhVien* getQuanLySinhVien() const { return quanLySinhVien; }
    
    // Setters
    void setMaLop(const std::string& ma) { maLop = ma; }
    void setTenLop(const std::string& ten) { tenLop = ten; }
    void setQuanLySinhVien(QuanLySinhVien* ql) { quanLySinhVien = ql; }

    // Business logic methods
    bool validate() const;
    
    // Operators for array operations
    bool operator==(Lop& other) { return maLop == other.maLop; }
    bool operator!=(Lop& other) { return !(*this == other); }
};

#endif // LOP_H