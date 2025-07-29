#ifndef LOP_H
#define LOP_H

#include <QString>
#include <QList>
#include <memory>

// Forward declaration
class SinhVien;
class DanhSachSinhVien;

/**
 * @brief Lớp học - Class information
 */
class Lop {
private:
    QString maLop;              // Class ID
    QString tenLop;             // Class name
    int nienKhoa;              // Academic year
    QString khoa;              // Department/Faculty
    std::shared_ptr<DanhSachSinhVien> danhSachSV;  // List of students

public:
    Lop();
    Lop(const QString& ma, const QString& ten, int nien, const QString& khoa);
    ~Lop();

    // Getters
    QString getMaLop() const { return maLop; }
    QString getTenLop() const { return tenLop; }
    int getNienKhoa() const { return nienKhoa; }
    QString getKhoa() const { return khoa; }
    std::shared_ptr<DanhSachSinhVien> getDanhSachSV() const { return danhSachSV; }

    // Setters
    void setMaLop(const QString& ma) { maLop = ma; }
    void setTenLop(const QString& ten) { tenLop = ten; }
    void setNienKhoa(int nien) { nienKhoa = nien; }
    void setKhoa(const QString& k) { khoa = k; }

    // Student management
    bool themSinhVien(const SinhVien& sv);
    bool xoaSinhVien(const QString& maSV);
    SinhVien* timSinhVien(const QString& maSV);
    int soLuongSinhVien() const;
    QList<SinhVien> layDanhSachSinhVien() const;

    // Statistics
    double diemTrungBinhLop(const QString& maMon = "") const;
    int soSinhVienDau() const;
    int soSinhVienRot() const;

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);

    // Operators
    bool operator==(const Lop& other) const;
    bool operator<(const Lop& other) const;
};

/**
 * @brief Quản lý mảng các lớp học
 */
class QuanLyLop {
private:
    static const int MAX_LOP = 100;     // Maximum number of classes
    Lop* danhSachLop[MAX_LOP];         // Array of class pointers
    int soLuongLop;                    // Current number of classes

public:
    QuanLyLop();
    ~QuanLyLop();

    // CRUD operations
    bool themLop(const Lop& lop);
    bool xoaLop(const QString& maLop);
    bool capNhatLop(const QString& maLop, const Lop& lopMoi);
    Lop* timLop(const QString& maLop);

    // Array operations
    Lop* layLop(int index);
    int getSoLuongLop() const { return soLuongLop; }
    bool dayDu() const { return soLuongLop >= MAX_LOP; }
    bool rong() const { return soLuongLop == 0; }

    // Search and filter
    QList<Lop*> timLopTheoKhoa(const QString& khoa);
    QList<Lop*> timLopTheoNienKhoa(int nienKhoa);
    QList<Lop*> timLopTheoTen(const QString& tenLop);

    // Statistics
    int tongSoSinhVien() const;
    double diemTrungBinhTatCaLop(const QString& maMon = "") const;
    
    // Reporting
    void inBangDiemTheoLop(const QString& maLop, const QString& maMon = "") const;
    void inThongKeTongQuat() const;

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);

    // Memory management
    void xoaTatCa();
    
    // Validation
    bool kiemTraMaLopTonTai(const QString& maLop) const;
    QString taoMaLopMoi() const;

private:
    // Helper methods
    int timViTri(const QString& maLop) const;
    void sapXepTheoMa();
    void sapXepTheoTen();
};

/**
 * @brief Thông tin thống kê lớp
 */
class ThongKeLop {
public:
    QString maLop;
    QString tenLop;
    int soSinhVien;
    double diemTrungBinh;
    int soSinhVienDau;
    int soSinhVienRot;
    double tyLeDau;

    ThongKeLop();
    ThongKeLop(const QString& ma, const QString& ten);
    
    void tinhToan(const Lop& lop, const QString& maMon = "");
    QString toString() const;
};

#endif // LOP_H
