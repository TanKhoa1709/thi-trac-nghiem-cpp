#ifndef MONHOC_H
#define MONHOC_H

#include <string>
#include <cstring>

// Forward declaration
class QuanLyCauHoi;

/**
 * @brief Môn học - MAMH (C15), TENMH, con trỏ quản lý câu hỏi
 */
class MonHoc {
private:
    char maMon[16]; // MAMH (C15) - mã môn học
    std::string tenMon; // TENMH - tên môn học
    QuanLyCauHoi *quanLyCauHoi; // Con trỏ quản lý câu hỏi

public:
    // Constructors & Destructor
    MonHoc();

    MonHoc(const char *ma, const std::string &ten);

    ~MonHoc();

    // Getters (const methods)
    const char *getMaMon() const { return maMon; }
    const std::string &getTenMon() const { return tenMon; }
    QuanLyCauHoi *getQuanLyCauHoi() const { return quanLyCauHoi; }

    // Setters
    void setMaMon(const char *ma);

    void setTenMon(const std::string &ten) { tenMon = ten; }
    void setQuanLyCauHoi(QuanLyCauHoi *ql) { quanLyCauHoi = ql; }

    // Business logic methods
    bool validate() const;

    // Operators for linear list operations
    bool operator==(MonHoc &other) {
        return std::strcmp(maMon, other.maMon) == 0;
    }

    bool operator!=(MonHoc &other) {
        return !(*this == other);
    }
};

#endif // MONHOC_H
