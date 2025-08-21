#ifndef MONHOC_H
#define MONHOC_H

#include <string>
#include <cstring>

// Forward declaration
class QuanLyCauHoi;

// Constants for ID boundary management
const int QUESTIONS_PER_SUBJECT = 10000;  // Số lượng ID cho mỗi môn học
const int BOUNDARY_START = 1;              // ID bắt đầu cho môn học đầu tiên

/**
 * @brief Môn học - MAMH (C15), TENMH, con trỏ quản lý câu hỏi
 */
class MonHoc {
private:
    char maMon[16]; // MAMH (C15) - mã môn học
    std::string tenMon; // TENMH - tên môn học
    int subjectIndex; // Chỉ số môn học (để tính boundary)
    QuanLyCauHoi *quanLyCauHoi; // Con trỏ quản lý câu hỏi

public:
    // Constructors & Destructor
    MonHoc();
    MonHoc(const char *ma, const std::string &ten, int subjectIndex);
    ~MonHoc();

    // Getters (const methods)
    const char *getMaMon() const { return maMon; }
    const std::string &getTenMon() const { return tenMon; }
    int getSubjectIndex() const { return subjectIndex; }
    int getMinId() const;
    int getMaxId() const;
    QuanLyCauHoi *getQuanLyCauHoi() const { return quanLyCauHoi; }

    // Setters
    void setMaMon(const char *ma);

    void setTenMon(const std::string &ten) { tenMon = ten; }
    void setSubjectIndex(int index);
    void setQuanLyCauHoi(QuanLyCauHoi *ql) { quanLyCauHoi = ql; }

    // Boundary calculation
    static void calculateBoundaryForIndex(int index, int &minId, int &maxId);

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
