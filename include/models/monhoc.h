#ifndef MONHOC_H
#define MONHOC_H

#include <string>

// Forward declaration
class QuanLyCauHoi;

/**
 * @brief Môn học - MAMH (C15), TENMH, con trỏ câu hỏi
 */
class MonHoc {
private:
    char maMon[16];              // MAMH (C15)
    std::string tenMon;          // TENMH
    QuanLyCauHoi* quanLyCauHoi;      // con trỏ quản lý câu hỏi

public:
    MonHoc();
    MonHoc(const char* ma, const std::string& ten);
    ~MonHoc();

    // Getters/Setters
    const char* getMaMon() const { return maMon; }
    std::string getTenMon() const { return tenMon; }
    QuanLyCauHoi* getQuanLyCauHoi() const { return quanLyCauHoi; }

    void setMaMon(const char* ma);
    void setTenMon(const std::string& ten) { tenMon = ten; }
    void setQuanLyCauHoi(QuanLyCauHoi* ql) { quanLyCauHoi = ql; }

    // Basic operations
    void inThongTinMonHoc() const;
};

#endif // MONHOC_H