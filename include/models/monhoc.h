#ifndef MONHOC_H
#define MONHOC_H

#include <QString>
#include <QList>
#include <memory>

// Forward declarations
class CauHoi;
class CayNhiPhan;

/**
 * @brief Môn học - Subject class
 * Quản lý danh sách tuyến tính các môn học, mỗi môn có con trỏ đến cây câu hỏi
 */
class MonHoc {
private:
    QString maMon;          // Subject ID
    QString tenMon;         // Subject name
    int soTinChi;          // Credit hours
    std::shared_ptr<CayNhiPhan> cayCauHoi;  // Binary tree of questions

public:
    MonHoc();
    MonHoc(const QString& ma, const QString& ten, int tinChi = 3);
    ~MonHoc();

    // Getters
    QString getMaMon() const { return maMon; }
    QString getTenMon() const { return tenMon; }
    int getSoTinChi() const { return soTinChi; }
    std::shared_ptr<CayNhiPhan> getCayCauHoi() const { return cayCauHoi; }

    // Setters
    void setMaMon(const QString& ma) { maMon = ma; }
    void setTenMon(const QString& ten) { tenMon = ten; }
    void setSoTinChi(int tinChi) { soTinChi = tinChi; }
    void setCayCauHoi(std::shared_ptr<CayNhiPhan> cay) { cayCauHoi = cay; }

    // Operations
    bool themCauHoi(const CauHoi& cauHoi);
    bool xoaCauHoi(int idCauHoi);
    CauHoi* timCauHoi(int idCauHoi);
    QList<CauHoi> layDanhSachCauHoi() const;
    QList<CauHoi> layNgauNhienCauHoi(int soLuong) const;

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);

    // Operators
    bool operator==(const MonHoc& other) const;
    bool operator<(const MonHoc& other) const;
};

/**
 * @brief Quản lý danh sách môn học
 */
class QuanLyMonHoc {
private:
    QList<MonHoc> danhSachMonHoc;

public:
    QuanLyMonHoc();
    ~QuanLyMonHoc();

    // CRUD operations
    bool themMonHoc(const MonHoc& monHoc);
    bool xoaMonHoc(const QString& maMon);
    bool capNhatMonHoc(const QString& maMon, const MonHoc& monHocMoi);
    MonHoc* timMonHoc(const QString& maMon);
    
    // List operations
    QList<MonHoc> layDanhSachMonHoc() const { return danhSachMonHoc; }
    int soLuongMonHoc() const { return danhSachMonHoc.size(); }
    void xoaTatCa() { danhSachMonHoc.clear(); }

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);

    // Validation
    bool kiemTraMaMonTonTai(const QString& maMon) const;
};

#endif // MONHOC_H
