#ifndef DIEMTHI_H
#define DIEMTHI_H

#include <string>

/**
 * @brief Điểm thi - MAMH, Điểm, Chi Tiết Bài Thi
 */
class DiemThi {
private:
    std::string maMonHoc;        // MAMH
    double diem;                 // Điểm
    std::string chiTietBaiThi;   // Chi Tiết Bài Thi

public:
    DiemThi();
    DiemThi(const std::string& maMon, double diem, const std::string& chiTiet);
    ~DiemThi();

    // Getters/Setters
    std::string getMaMonHoc() const { return maMonHoc; }
    double getDiem() const { return diem; }
    std::string getChiTietBaiThi() const { return chiTietBaiThi; }

    void setMaMonHoc(const std::string& ma) { maMonHoc = ma; }
    void setDiem(double d) { diem = d; }
    void setChiTietBaiThi(const std::string& chiTiet) { chiTietBaiThi = chiTiet; }

    // Display
    void inDiem() const;
    
    // Equality operator for LinkedList operations
    bool operator==(const DiemThi& other) const {
        return maMonHoc == other.maMonHoc;
    }
    
    // Equality operator for pointer comparisons in LinkedList
    bool operator==(const DiemThi* other) const {
        return other && maMonHoc == other->maMonHoc;
    }
};

#endif // DIEMTHI_H