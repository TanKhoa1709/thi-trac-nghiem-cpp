#ifndef QUANLYTHI_H
#define QUANLYTHI_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QDateTime>
#include <memory>

// Forward declarations
class CauHoi;
class DiemThi;
class QuanLyCauHoi;

/**
 * @brief Quản lý một phiên thi trắc nghiệm
 */
class PhienThi {
private:
    QString maSinhVien;
    QString maMon;
    QList<CauHoi> danhSachCauHoi;
    QStringList dapAnDaChon;
    int cauHoiHienTai;
    QDateTime thoiGianBatDau;
    QDateTime thoiGianKetThuc;
    int thoiGianThi; // minutes
    bool daHoanThanh;

public:
    PhienThi();
    PhienThi(const QString& maSV, const QString& maMon, int soPhut = 60);
    ~PhienThi();

    // Getters
    QString getMaSinhVien() const { return maSinhVien; }
    QString getMaMon() const { return maMon; }
    QList<CauHoi> getDanhSachCauHoi() const { return danhSachCauHoi; }
    QStringList getDapAnDaChon() const { return dapAnDaChon; }
    int getChiSoCauHoiHienTai() const { return cauHoiHienTai; }
    QDateTime getThoiGianBatDau() const { return thoiGianBatDau; }
    QDateTime getThoiGianKetThuc() const { return thoiGianKetThuc; }
    int getThoiGianThi() const { return thoiGianThi; }
    bool getDaHoanThanh() const { return daHoanThanh; }

    // Setters
    void setMaSinhVien(const QString& ma) { maSinhVien = ma; }
    void setMaMon(const QString& ma) { maMon = ma; }
    void setDanhSachCauHoi(const QList<CauHoi>& ds) { danhSachCauHoi = ds; }
    void setThoiGianThi(int phut) { thoiGianThi = phut; }

    // Exam operations
    bool batDauThi();
    bool ketThucThi();
    bool chuyenCauHoiTiep();
    bool chuyenCauHoiTruoc();
    bool chuyenDenCauHoi(int index);
    bool chonDapAn(const QString& dapAn);
    bool xoaDapAn();

    // Question management
    CauHoi* getCauHoiHienTai();
    const CauHoi* getCauHoiHienTai() const;
    QString getDapAnDaChon(int index) const;
    bool daCauTraLoi(int index) const;

    // Statistics
    int soLuongCauHoi() const { return danhSachCauHoi.size(); }
    int soCauDaTraLoi() const;
    int soCauChuaTraLoi() const;
    double tiLeTienDo() const;
    int thoiGianConLai() const; // minutes

    // Results
    DiemThi tinhDiem() const;
    int soCauDung() const;
    double diemSo() const;

    // Validation
    bool hopLe() const;
    bool coTheKetThuc() const;
};

/**
 * @brief Quản lý hệ thống thi trắc nghiệm
 */
class QuanLyThi {
private:
    std::shared_ptr<PhienThi> phienThiHienTai;
    QuanLyCauHoi* quanLyCauHoi;

public:
    QuanLyThi();
    ~QuanLyThi();

    // Initialization
    void setQuanLyCauHoi(QuanLyCauHoi* qlch) { quanLyCauHoi = qlch; }

    // Exam session management
    bool batDauThi(const QString& maSinhVien, const QString& maMon, 
                   int soCauHoi = 20, int thoiGianPhut = 60);
    bool ketThucThi();
    bool huyThi();
    bool tamDungThi();
    bool tiepTucThi();

    // Current exam access
    PhienThi* getPhienThiHienTai() { return phienThiHienTai.get(); }
    const PhienThi* getPhienThiHienTai() const { return phienThiHienTai.get(); }
    bool coPhienThiDangDien() const { return phienThiHienTai != nullptr; }

    // Question navigation
    bool chuyenCauHoiTiep();
    bool chuyenCauHoiTruoc();
    bool chuyenDenCauHoi(int index);

    // Answer handling
    bool chonDapAn(const QString& dapAn);
    bool xoaDapAn();
    bool xoaDapAn(int cauHoiIndex);

    // Exam information
    CauHoi* getCauHoiHienTai();
    QString getDapAnDaChon() const;
    QString getDapAnDaChon(int cauHoiIndex) const;
    QList<int> getCauChuaTraLoi() const;

    // Statistics
    int soLuongCauHoi() const;
    int cauHoiHienTai() const;
    int soCauDaTraLoi() const;
    double tiLeTienDo() const;
    int thoiGianConLai() const;

    // Results
    DiemThi layKetQuaThi();
    int soCauDung() const;
    double diemSo() const;

    // Validation
    bool coTheKetThuc() const;
    bool hopLe() const;

private:
    // Helper methods
    QList<CauHoi> layNgauNhienCauHoi(const QString& maMon, int soLuong);
    void khởiTaoPhienThi();
    void dongPhienThi();
};

#endif // QUANLYTHI_H