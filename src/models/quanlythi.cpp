#include "quanlythi.h"
#include "cauhoi.h"
#include "diemthi.h"
#include <QRandomGenerator>
#include <QDebug>

// PhienThi implementation
PhienThi::PhienThi()
    : cauHoiHienTai(0)
    , thoiGianThi(60)
    , daHoanThanh(false)
{
}

PhienThi::PhienThi(const QString& maSV, const QString& maMon, int soPhut)
    : maSinhVien(maSV)
    , maMon(maMon)
    , cauHoiHienTai(0)
    , thoiGianThi(soPhut)
    , daHoanThanh(false)
{
}

PhienThi::~PhienThi()
{
}

bool PhienThi::batDauThi()
{
    if (danhSachCauHoi.isEmpty()) {
        return false;
    }
    
    thoiGianBatDau = QDateTime::currentDateTime();
    thoiGianKetThuc = thoiGianBatDau.addSecs(thoiGianThi * 60);
    cauHoiHienTai = 0;
    
    // Initialize answer array
    dapAnDaChon.clear();
    for (int i = 0; i < danhSachCauHoi.size(); ++i) {
        dapAnDaChon.append("");
    }
    
    return true;
}

bool PhienThi::ketThucThi()
{
    if (daHoanThanh) {
        return false;
    }
    
    daHoanThanh = true;
    thoiGianKetThuc = QDateTime::currentDateTime();
    return true;
}

bool PhienThi::chuyenCauHoiTiep()
{
    if (cauHoiHienTai < danhSachCauHoi.size() - 1) {
        cauHoiHienTai++;
        return true;
    }
    return false;
}

bool PhienThi::chuyenCauHoiTruoc()
{
    if (cauHoiHienTai > 0) {
        cauHoiHienTai--;
        return true;
    }
    return false;
}

bool PhienThi::chuyenDenCauHoi(int index)
{
    if (index >= 0 && index < danhSachCauHoi.size()) {
        cauHoiHienTai = index;
        return true;
    }
    return false;
}

bool PhienThi::chonDapAn(const QString& dapAn)
{
    if (cauHoiHienTai >= 0 && cauHoiHienTai < dapAnDaChon.size()) {
        dapAnDaChon[cauHoiHienTai] = dapAn;
        return true;
    }
    return false;
}

bool PhienThi::xoaDapAn()
{
    if (cauHoiHienTai >= 0 && cauHoiHienTai < dapAnDaChon.size()) {
        dapAnDaChon[cauHoiHienTai] = "";
        return true;
    }
    return false;
}

CauHoi* PhienThi::getCauHoiHienTai()
{
    if (cauHoiHienTai >= 0 && cauHoiHienTai < danhSachCauHoi.size()) {
        return &danhSachCauHoi[cauHoiHienTai];
    }
    return nullptr;
}

const CauHoi* PhienThi::getCauHoiHienTai() const
{
    if (cauHoiHienTai >= 0 && cauHoiHienTai < danhSachCauHoi.size()) {
        return &danhSachCauHoi[cauHoiHienTai];
    }
    return nullptr;
}

QString PhienThi::getDapAnDaChon(int index) const
{
    if (index >= 0 && index < dapAnDaChon.size()) {
        return dapAnDaChon[index];
    }
    return "";
}

bool PhienThi::daCauTraLoi(int index) const
{
    if (index >= 0 && index < dapAnDaChon.size()) {
        return !dapAnDaChon[index].isEmpty();
    }
    return false;
}

int PhienThi::soCauDaTraLoi() const
{
    int count = 0;
    for (const QString& answer : dapAnDaChon) {
        if (!answer.isEmpty()) {
            count++;
        }
    }
    return count;
}

int PhienThi::soCauChuaTraLoi() const
{
    return danhSachCauHoi.size() - soCauDaTraLoi();
}

double PhienThi::tiLeTienDo() const
{
    if (danhSachCauHoi.isEmpty()) return 0.0;
    return (double)soCauDaTraLoi() / danhSachCauHoi.size() * 100.0;
}

int PhienThi::thoiGianConLai() const
{
    if (daHoanThanh) return 0;
    
    QDateTime now = QDateTime::currentDateTime();
    int secondsRemaining = now.secsTo(thoiGianKetThuc);
    return qMax(0, secondsRemaining / 60);
}

DiemThi PhienThi::tinhDiem() const
{
    DiemThi diemThi;
    diemThi.setMaSV(maSinhVien);
    diemThi.setMaMon(maMon);
    diemThi.setNgayThi(thoiGianBatDau.date());
    diemThi.setGioThi(thoiGianBatDau.time());
    
    int soDung = soCauDung();
    // Score will be calculated by tinhDiem() method after adding details
    
    // Set detailed answers
    for (int i = 0; i < danhSachCauHoi.size(); ++i) {
        const CauHoi& cauHoi = danhSachCauHoi[i];
        QString dapAnChon = (i < dapAnDaChon.size()) ? dapAnDaChon[i] : "";
        
        // Create ChiTietBaiThi object
        ChiTietBaiThi chiTiet;
        chiTiet.setIdCauHoi(cauHoi.getId());
        chiTiet.setNoiDungCauHoi(cauHoi.getNoiDung());
        chiTiet.setDapAn(cauHoi.getDapAn());
        chiTiet.setDapAnChon(dapAnChon.isEmpty() ? ' ' : dapAnChon.at(0).toLatin1());
        chiTiet.setDapAnDung(cauHoi.getDapAnDung());
        
        diemThi.themChiTiet(chiTiet);
    }
    
    // Calculate final score
    diemThi.tinhDiem();
    
    return diemThi;
}

int PhienThi::soCauDung() const
{
    int count = 0;
    for (int i = 0; i < danhSachCauHoi.size() && i < dapAnDaChon.size(); ++i) {
        const CauHoi& cauHoi = danhSachCauHoi[i];
        QString dapAnChon = dapAnDaChon[i];
        if (!dapAnChon.isEmpty() && dapAnChon.at(0).toLatin1() == cauHoi.getDapAnDung()) {
            count++;
        }
    }
    return count;
}

double PhienThi::diemSo() const
{
    if (danhSachCauHoi.isEmpty()) return 0.0;
    return (double)soCauDung() / danhSachCauHoi.size() * 10.0;
}

bool PhienThi::hopLe() const
{
    return !maSinhVien.isEmpty() && !maMon.isEmpty() && !danhSachCauHoi.isEmpty();
}

bool PhienThi::coTheKetThuc() const
{
    return hopLe() && !daHoanThanh;
}

// QuanLyThi implementation
QuanLyThi::QuanLyThi()
    : quanLyCauHoi(nullptr)
{
}

QuanLyThi::~QuanLyThi()
{
}

bool QuanLyThi::batDauThi(const QString& maSinhVien, const QString& maMon, 
                          int soCauHoi, int thoiGianPhut)
{
    if (coPhienThiDangDien()) {
        return false; // Already have an active exam
    }
    
    if (!quanLyCauHoi) {
        qDebug() << "QuanLyCauHoi not set";
        return false;
    }
    
    // Get random questions for the subject
    QList<CauHoi> danhSachCauHoi = layNgauNhienCauHoi(maMon, soCauHoi);
    if (danhSachCauHoi.isEmpty()) {
        qDebug() << "No questions found for subject:" << maMon;
        return false;
    }
    
    // Create new exam session
    phienThiHienTai = std::make_shared<PhienThi>(maSinhVien, maMon, thoiGianPhut);
    phienThiHienTai->setDanhSachCauHoi(danhSachCauHoi);
    
    return phienThiHienTai->batDauThi();
}

bool QuanLyThi::ketThucThi()
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    bool success = phienThiHienTai->ketThucThi();
    if (success) {
        dongPhienThi();
    }
    
    return success;
}

bool QuanLyThi::huyThi()
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    dongPhienThi();
    return true;
}

bool QuanLyThi::chuyenCauHoiTiep()
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    return phienThiHienTai->chuyenCauHoiTiep();
}

bool QuanLyThi::chuyenCauHoiTruoc()
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    return phienThiHienTai->chuyenCauHoiTruoc();
}

bool QuanLyThi::chuyenDenCauHoi(int index)
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    return phienThiHienTai->chuyenDenCauHoi(index);
}

bool QuanLyThi::chonDapAn(const QString& dapAn)
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    return phienThiHienTai->chonDapAn(dapAn);
}

bool QuanLyThi::xoaDapAn()
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    return phienThiHienTai->xoaDapAn();
}

CauHoi* QuanLyThi::getCauHoiHienTai()
{
    if (!coPhienThiDangDien()) {
        return nullptr;
    }
    
    return phienThiHienTai->getCauHoiHienTai();
}

QString QuanLyThi::getDapAnDaChon() const
{
    if (!coPhienThiDangDien()) {
        return "";
    }
    
    return phienThiHienTai->getDapAnDaChon(phienThiHienTai->getChiSoCauHoiHienTai());
}

QString QuanLyThi::getDapAnDaChon(int cauHoiIndex) const
{
    if (!coPhienThiDangDien()) {
        return "";
    }
    
    return phienThiHienTai->getDapAnDaChon(cauHoiIndex);
}

QList<int> QuanLyThi::getCauChuaTraLoi() const
{
    QList<int> result;
    if (!coPhienThiDangDien()) {
        return result;
    }
    
    for (int i = 0; i < phienThiHienTai->soLuongCauHoi(); ++i) {
        if (!phienThiHienTai->daCauTraLoi(i)) {
            result.append(i);
        }
    }
    
    return result;
}

int QuanLyThi::soLuongCauHoi() const
{
    if (!coPhienThiDangDien()) {
        return 0;
    }
    
    return phienThiHienTai->soLuongCauHoi();
}

int QuanLyThi::cauHoiHienTai() const
{
    if (!coPhienThiDangDien()) {
        return -1;
    }
    
    return phienThiHienTai->getChiSoCauHoiHienTai();
}

int QuanLyThi::soCauDaTraLoi() const
{
    if (!coPhienThiDangDien()) {
        return 0;
    }
    
    return phienThiHienTai->soCauDaTraLoi();
}

double QuanLyThi::tiLeTienDo() const
{
    if (!coPhienThiDangDien()) {
        return 0.0;
    }
    
    return phienThiHienTai->tiLeTienDo();
}

int QuanLyThi::thoiGianConLai() const
{
    if (!coPhienThiDangDien()) {
        return 0;
    }
    
    return phienThiHienTai->thoiGianConLai();
}

DiemThi QuanLyThi::layKetQuaThi()
{
    if (!coPhienThiDangDien()) {
        return DiemThi();
    }
    
    return phienThiHienTai->tinhDiem();
}

int QuanLyThi::soCauDung() const
{
    if (!coPhienThiDangDien()) {
        return 0;
    }
    
    return phienThiHienTai->soCauDung();
}

double QuanLyThi::diemSo() const
{
    if (!coPhienThiDangDien()) {
        return 0.0;
    }
    
    return phienThiHienTai->diemSo();
}

bool QuanLyThi::coTheKetThuc() const
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    return phienThiHienTai->coTheKetThuc();
}

bool QuanLyThi::hopLe() const
{
    if (!coPhienThiDangDien()) {
        return false;
    }
    
    return phienThiHienTai->hopLe();
}

QList<CauHoi> QuanLyThi::layNgauNhienCauHoi(const QString& maMon, int soLuong)
{
    QList<CauHoi> result;
    
    if (!quanLyCauHoi) {
        return result;
    }
    
    // Get all questions for the subject
    QList<CauHoi> allQuestions = quanLyCauHoi->layCauHoiTheoMon(maMon);
    
    if (allQuestions.size() <= soLuong) {
        // If we have fewer questions than requested, return all
        return allQuestions;
    }
    
    // Randomly select questions
    QList<int> selectedIndices;
    while (selectedIndices.size() < soLuong) {
        int randomIndex = QRandomGenerator::global()->bounded(allQuestions.size());
        if (!selectedIndices.contains(randomIndex)) {
            selectedIndices.append(randomIndex);
            result.append(allQuestions[randomIndex]);
        }
    }
    
    return result;
}

void QuanLyThi::dongPhienThi()
{
    phienThiHienTai.reset();
}