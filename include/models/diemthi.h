#ifndef DIEMTHI_H
#define DIEMTHI_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QList>
#include <QStringList>
#include <memory>

// Forward declarations
class CauHoi;

/**
 * @brief Chi tiết bài thi - Exam details
 */
class ChiTietBaiThi {
private:
    int idCauHoi;               // Question ID
    QString noiDungCauHoi;      // Question content
    QStringList dapAn;          // Answer options
    char dapAnChon;             // Student's answer
    char dapAnDung;             // Correct answer
    bool dung;                  // Is correct?

public:
    ChiTietBaiThi();
    ChiTietBaiThi(int id, const QString& noiDung, const QStringList& dapAn,
                  char dapAnChon, char dapAnDung);
    ~ChiTietBaiThi();

    // Getters
    int getIdCauHoi() const { return idCauHoi; }
    QString getNoiDungCauHoi() const { return noiDungCauHoi; }
    QStringList getDapAn() const { return dapAn; }
    char getDapAnChon() const { return dapAnChon; }
    char getDapAnDung() const { return dapAnDung; }
    bool isDung() const { return dung; }

    // Setters
    void setIdCauHoi(int id) { idCauHoi = id; }
    void setNoiDungCauHoi(const QString& nd) { noiDungCauHoi = nd; }
    void setDapAn(const QStringList& da) { dapAn = da; }
    void setDapAnChon(char da) { dapAnChon = da; kiemTraDapAn(); }
    void setDapAnDung(char da) { dapAnDung = da; kiemTraDapAn(); }

    // Operations
    void kiemTraDapAn() { dung = (dapAnChon == dapAnDung); }
    QString toString() const;
};

/**
 * @brief Điểm thi của sinh viên
 */
class DiemThi {
private:
    QString maSV;               // Student ID
    QString maMon;              // Subject ID
    QDate ngayThi;              // Exam date
    QTime gioThi;               // Exam time
    double diem;                // Score (0-10)
    int soCauDung;              // Number of correct answers
    int tongSoCau;              // Total number of questions
    int thoiGianLam;            // Time taken (minutes)
    QList<ChiTietBaiThi> chiTietBaiThi;  // Detailed exam answers

public:
    DiemThi();
    DiemThi(const QString& maSV, const QString& maMon, const QDate& ngayThi,
            const QTime& gioThi = QTime::currentTime());
    ~DiemThi();

    // Getters
    QString getMaSV() const { return maSV; }
    QString getMaMon() const { return maMon; }
    QDate getNgayThi() const { return ngayThi; }
    QTime getGioThi() const { return gioThi; }
    double getDiem() const { return diem; }
    int getSoCauDung() const { return soCauDung; }
    int getTongSoCau() const { return tongSoCau; }
    int getThoiGianLam() const { return thoiGianLam; }
    QList<ChiTietBaiThi> getChiTietBaiThi() const { return chiTietBaiThi; }

    // Setters
    void setMaSV(const QString& ma) { maSV = ma; }
    void setMaMon(const QString& ma) { maMon = ma; }
    void setNgayThi(const QDate& ngay) { ngayThi = ngay; }
    void setGioThi(const QTime& gio) { gioThi = gio; }
    void setThoiGianLam(int phut) { thoiGianLam = phut; }

    // Exam operations
    void themChiTiet(const ChiTietBaiThi& chiTiet);
    void tinhDiem();
    void batDauThi();
    void ketThucThi();

    // Analysis
    double tyLeDung() const;
    QList<int> danhSachCauSai() const;
    QString phanLoaiKetQua() const;  // Xuất sắc, Giỏi, Khá, TB, Yếu, Kém

    // Display
    QString toString() const;
    QString toDetailedString() const;

    // Operators
    bool operator==(const DiemThi& other) const;
    bool operator<(const DiemThi& other) const;
};

/**
 * @brief Node của danh sách liên kết điểm thi
 */
class NodeDiemThi {
public:
    DiemThi data;
    std::shared_ptr<NodeDiemThi> next;

    NodeDiemThi(const DiemThi& diem);
    ~NodeDiemThi();
};

/**
 * @brief Danh sách liên kết đơn quản lý điểm thi
 */
class DanhSachDiemThi {
private:
    std::shared_ptr<NodeDiemThi> head;
    int soLuong;

public:
    DanhSachDiemThi();
    ~DanhSachDiemThi();

    // List operations
    bool them(const DiemThi& diem);
    bool xoa(const QString& maMon, const QDate& ngayThi);
    DiemThi* tim(const QString& maMon, const QDate& ngayThi);
    bool capNhat(const QString& maMon, const QDate& ngayThi, const DiemThi& diemMoi);

    // Query operations
    QList<DiemThi> layTatCa() const;
    QList<DiemThi> layTheoMon(const QString& maMon) const;
    QList<DiemThi> layTheoNgay(const QDate& ngayThi) const;
    QList<DiemThi> layTheoKhoangNgay(const QDate& tuNgay, const QDate& denNgay) const;

    // Properties
    int getSoLuong() const { return soLuong; }
    bool rong() const { return head == nullptr; }
    void xoaTatCa();

    // Statistics
    double diemTrungBinh(const QString& maMon = "") const;
    double diemCaoNhat(const QString& maMon = "") const;
    double diemThapNhat(const QString& maMon = "") const;
    int demLanThi(const QString& maMon = "") const;

    // Sorting
    void sapXepTheoDiem(bool tangDan = false);
    void sapXepTheoNgay(bool tangDan = true);
    void sapXepTheoMon();

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);

private:
    std::shared_ptr<NodeDiemThi> timNode(const QString& maMon, const QDate& ngayThi);
};

// Forward declaration - actual implementation is in quanlythi.h
class QuanLyThi;

/**
 * @brief Quản lý tổng thể điểm thi và báo cáo
 */
class QuanLyDiemThi {
private:
    QList<DiemThi> tatCaDiemThi;

public:
    QuanLyDiemThi();
    ~QuanLyDiemThi();

    // CRUD operations
    bool themDiemThi(const DiemThi& diem);
    bool xoaDiemThi(const QString& maSV, const QString& maMon, const QDate& ngayThi);
    DiemThi* timDiemThi(const QString& maSV, const QString& maMon, const QDate& ngayThi);
    bool capNhatDiemThi(const QString& maSV, const QString& maMon, 
                        const QDate& ngayThi, const DiemThi& diemMoi);

    // Query operations
    QList<DiemThi> layDiemTheoSinhVien(const QString& maSV) const;
    QList<DiemThi> layDiemTheoMon(const QString& maMon) const;
    QList<DiemThi> layDiemTheoLop(const QString& maLop) const;
    QList<DiemThi> layDiemTheoNgay(const QDate& ngayThi) const;

    // Statistics and reports
    void inBangDiemTheoLop(const QString& maLop, const QString& maMon = "") const;
    void inBangDiemTheoMon(const QString& maMon) const;
    void inThongKeChiTiet(const QString& maMon) const;
    
    // Analysis
    double diemTrungBinhMon(const QString& maMon) const;
    double diemTrungBinhLop(const QString& maLop, const QString& maMon = "") const;
    int demSinhVienDau(const QString& maMon, double diemChuan = 5.0) const;
    int demSinhVienRot(const QString& maMon, double diemChuan = 5.0) const;

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);
    bool xuatBaoCaoExcel(const QString& tenFile, const QString& maMon = "") const;

    // Data management
    void xoaTatCa() { tatCaDiemThi.clear(); }
    int tongSoBaiThi() const { return tatCaDiemThi.size(); }
};

#endif // DIEMTHI_H
