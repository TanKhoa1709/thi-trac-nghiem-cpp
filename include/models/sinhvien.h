#ifndef SINHVIEN_H
#define SINHVIEN_H

#include <QString>
#include <QDate>
#include <QList>
#include <memory>

// Forward declarations
class DiemThi;
class DanhSachDiemThi;

/**
 * @brief Thông tin sinh viên
 */
class SinhVien {
private:
    QString maSV;               // Student ID
    QString ho;                 // Last name
    QString ten;                // First name
    QDate ngaySinh;            // Date of birth
    QString gioiTinh;          // Gender
    QString soDienThoai;       // Phone number
    QString email;             // Email address
    QString maLop;             // Class ID
    std::shared_ptr<DanhSachDiemThi> danhSachDiem;  // List of exam scores

public:
    SinhVien();
    SinhVien(const QString& ma, const QString& ho, const QString& ten, 
             const QDate& ngaySinh, const QString& gioiTinh, 
             const QString& sdt = "", const QString& email = "", 
             const QString& maLop = "");
    ~SinhVien();

    // Getters
    QString getMaSV() const { return maSV; }
    QString getHo() const { return ho; }
    QString getTen() const { return ten; }
    QString getHoTen() const { return ho + " " + ten; }
    QDate getNgaySinh() const { return ngaySinh; }
    QString getGioiTinh() const { return gioiTinh; }
    QString getSoDienThoai() const { return soDienThoai; }
    QString getEmail() const { return email; }
    QString getMaLop() const { return maLop; }
    std::shared_ptr<DanhSachDiemThi> getDanhSachDiem() const { return danhSachDiem; }

    // Setters
    void setMaSV(const QString& ma) { maSV = ma; }
    void setHo(const QString& h) { ho = h; }
    void setTen(const QString& t) { ten = t; }
    void setNgaySinh(const QDate& ns) { ngaySinh = ns; }
    void setGioiTinh(const QString& gt) { gioiTinh = gt; }
    void setSoDienThoai(const QString& sdt) { soDienThoai = sdt; }
    void setEmail(const QString& e) { email = e; }
    void setMaLop(const QString& ml) { maLop = ml; }

    // Score management
    bool themDiemThi(const DiemThi& diem);
    bool xoaDiemThi(const QString& maMon, const QDate& ngayThi);
    DiemThi* timDiemThi(const QString& maMon, const QDate& ngayThi);
    QList<DiemThi> layDanhSachDiem() const;
    QList<DiemThi> layDiemTheoMon(const QString& maMon) const;

    // Statistics
    double diemTrungBinh(const QString& maMon = "") const;
    double diemCaoNhat(const QString& maMon = "") const;
    double diemThapNhat(const QString& maMon = "") const;
    int soLanThi(const QString& maMon = "") const;
    bool daDau(const QString& maMon) const;

    // Validation
    bool kiemTraThongTin() const;
    int tinhTuoi() const;

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);

    // Operators
    bool operator==(const SinhVien& other) const;
    bool operator<(const SinhVien& other) const;
};

/**
 * @brief Node của danh sách liên kết sinh viên
 */
class NodeSinhVien {
public:
    SinhVien data;
    std::shared_ptr<NodeSinhVien> next;

    NodeSinhVien(const SinhVien& sv);
    ~NodeSinhVien();
};

/**
 * @brief Danh sách liên kết đơn quản lý sinh viên
 */
class DanhSachSinhVien {
private:
    std::shared_ptr<NodeSinhVien> head;
    int soLuong;

public:
    DanhSachSinhVien();
    ~DanhSachSinhVien();

    // List operations
    bool them(const SinhVien& sv);
    bool xoa(const QString& maSV);
    bool capNhat(const QString& maSV, const SinhVien& svMoi);
    SinhVien* tim(const QString& maSV);
    const SinhVien* tim(const QString& maSV) const;

    // Traversal
    QList<SinhVien> layTatCa() const;
    SinhVien* layDauTien();
    SinhVien* layTiepTheo(const QString& maSVHienTai);

    // Properties
    int getSoLuong() const { return soLuong; }
    bool rong() const { return head == nullptr; }
    void xoaTatCa();

    // Search and filter
    QList<SinhVien> timTheoTen(const QString& ten) const;
    QList<SinhVien> timTheoLop(const QString& maLop) const;
    QList<SinhVien> timTheoGioiTinh(const QString& gioiTinh) const;

    // Sorting
    void sapXepTheoMa();
    void sapXepTheoTen();
    void sapXepTheoDiem(const QString& maMon = "");

    // Statistics
    double diemTrungBinhTatCa(const QString& maMon = "") const;
    int demSinhVienDau(const QString& maMon = "") const;
    int demSinhVienRot(const QString& maMon = "") const;

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);

private:
    // Helper methods
    std::shared_ptr<NodeSinhVien> timNode(const QString& maSV);
    void hoanDoi(std::shared_ptr<NodeSinhVien> a, std::shared_ptr<NodeSinhVien> b);
};

/**
 * @brief Quản lý tổng thể sinh viên với các chức năng nâng cao
 */
class QuanLySinhVien {
private:
    DanhSachSinhVien danhSach;
    QList<SinhVien> lichSuThayDoi;    // History for undo
    int viTriHienTai;

public:
    QuanLySinhVien();
    ~QuanLySinhVien();

    // CRUD with undo support
    bool themSinhVien(const SinhVien& sv);
    bool xoaSinhVien(const QString& maSV);
    bool capNhatSinhVien(const QString& maSV, const SinhVien& svMoi);
    SinhVien* timSinhVien(const QString& maSV);

    // Undo/Redo
    bool undo();
    bool redo();
    bool coTheUndo() const;
    bool coTheRedo() const;

    // Batch operations
    bool themNhieuSinhVien(const QList<SinhVien>& danhSachSV);
    bool xoaNhieuSinhVien(const QStringList& danhSachMaSV);

    // Advanced search
    QList<SinhVien> timKiemNangCao(const QString& tuKhoa, 
                                   const QString& truong = "all") const;
    QList<SinhVien> locTheoTieuChi(const QString& maLop = "", 
                                   const QString& gioiTinh = "",
                                   int tuoiMin = 0, int tuoiMax = 100) const;

    // Reports and statistics
    void taoReportLop(const QString& maLop, const QString& tenFile) const;
    void taoReportDiem(const QString& maMon, const QString& tenFile) const;
    
    // Data access
    QList<SinhVien> layTatCaSinhVien() const { return danhSach.layTatCa(); }
    int tongSoSinhVien() const { return danhSach.getSoLuong(); }

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);
    bool xuatExcel(const QString& tenFile) const;
    bool nhapExcel(const QString& tenFile);

    // Validation
    bool kiemTraMaSVTonTai(const QString& maSV) const;
    QString taoMaSVMoi(const QString& maLop) const;
};

#endif // SINHVIEN_H
