#ifndef DIEMTHI_H
#define DIEMTHI_H

#include <string>

/**
 * @brief Điểm thi - MAMH, Điểm, Chi Tiết Bài Thi
 */
class DiemThi {
private:
    std::string maMonHoc;        // Mã môn học
    double diem;                 // Điểm số
    std::string chiTietBaiThi;   // Chi tiết bài thi (câu trả lời)

public:
    // Constructors & Destructor
    DiemThi();
    DiemThi(const std::string& maMon, double diem, const std::string& chiTiet);
    ~DiemThi() = default;

    // Getters (const methods)
    const std::string& getMaMonHoc() const { return maMonHoc; }
    double getDiem() const { return diem; }
    const std::string& getChiTietBaiThi() const { return chiTietBaiThi; }

    // Setters
    void setMaMonHoc(const std::string& ma) { maMonHoc = ma; }
    void setDiem(double d) { diem = d; }
    void setChiTietBaiThi(const std::string& chiTiet) { chiTietBaiThi = chiTiet; }

    // Business logic methods
    bool validate() const;
    
    // Operators for LinkedList operations
    bool operator==(const DiemThi& other) const {
        return maMonHoc == other.maMonHoc;
    }
    
    bool operator!=(const DiemThi& other) const {
        return !(*this == other);
    }
};

#endif // DIEMTHI_H