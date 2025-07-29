#include "diemthi.h"
#include "cauhoi.h"
#include "quanlythi.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// ChiTietBaiThi Implementation
ChiTietBaiThi::ChiTietBaiThi() : idCauHoi(0), dapAnChon('A'), dapAnDung('A'), dung(false)
{
}

ChiTietBaiThi::ChiTietBaiThi(int id, const QString& noiDung, const QStringList& dapAn,
                             char dapAnChon, char dapAnDung)
    : idCauHoi(id), noiDungCauHoi(noiDung), dapAn(dapAn), dapAnChon(dapAnChon), dapAnDung(dapAnDung)
{
    kiemTraDapAn();
}

ChiTietBaiThi::~ChiTietBaiThi()
{
}

QString ChiTietBaiThi::toString() const
{
    return QString("Question %1: %2 -> %3 (%4)")
           .arg(idCauHoi)
           .arg(dapAnChon)
           .arg(dapAnDung)
           .arg(dung ? "Correct" : "Wrong");
}

// DiemThi Implementation
DiemThi::DiemThi() : diem(0.0), soCauDung(0), tongSoCau(0), thoiGianLam(0)
{
    ngayThi = QDate::currentDate();
    gioThi = QTime::currentTime();
}

DiemThi::DiemThi(const QString& maSV, const QString& maMon, const QDate& ngayThi, const QTime& gioThi)
    : maSV(maSV), maMon(maMon), ngayThi(ngayThi), gioThi(gioThi), diem(0.0), soCauDung(0), tongSoCau(0), thoiGianLam(0)
{
}

DiemThi::~DiemThi()
{
}

void DiemThi::themChiTiet(const ChiTietBaiThi& chiTiet)
{
    chiTietBaiThi.append(chiTiet);
    tongSoCau = chiTietBaiThi.size();
    tinhDiem();
}

void DiemThi::tinhDiem()
{
    soCauDung = 0;
    for (const ChiTietBaiThi& ct : chiTietBaiThi) {
        if (ct.isDung()) {
            soCauDung++;
        }
    }
    
    if (tongSoCau > 0) {
        diem = (double)soCauDung / tongSoCau * 10.0;
    }
}

QString DiemThi::toString() const
{
    return QString("Student: %1, Subject: %2, Score: %3, Date: %4")
           .arg(maSV)
           .arg(maMon)
           .arg(diem, 0, 'f', 2)
           .arg(ngayThi.toString("dd/MM/yyyy"));
}

bool DiemThi::operator==(const DiemThi& other) const
{
    return maSV == other.maSV && maMon == other.maMon && ngayThi == other.ngayThi;
}

bool DiemThi::operator<(const DiemThi& other) const
{
    if (maSV != other.maSV) return maSV < other.maSV;
    if (maMon != other.maMon) return maMon < other.maMon;
    return ngayThi < other.ngayThi;
}

// NodeDiemThi Implementation
NodeDiemThi::NodeDiemThi(const DiemThi& diem) : data(diem)
{
}

NodeDiemThi::~NodeDiemThi()
{
}

// DanhSachDiemThi Implementation
DanhSachDiemThi::DanhSachDiemThi() : soLuong(0)
{
}

DanhSachDiemThi::~DanhSachDiemThi()
{
    xoaTatCa();
}

bool DanhSachDiemThi::them(const DiemThi& diem)
{
    auto newNode = std::make_shared<NodeDiemThi>(diem);
    newNode->next = head;
    head = newNode;
    soLuong++;
    return true;
}

bool DanhSachDiemThi::xoa(const QString& maMon, const QDate& ngayThi)
{
    if (!head) return false;
    
    if (head->data.getMaMon() == maMon && head->data.getNgayThi() == ngayThi) {
        head = head->next;
        soLuong--;
        return true;
    }
    
    auto current = head;
    while (current->next) {
        if (current->next->data.getMaMon() == maMon && current->next->data.getNgayThi() == ngayThi) {
            current->next = current->next->next;
            soLuong--;
            return true;
        }
        current = current->next;
    }
    
    return false;
}

DiemThi* DanhSachDiemThi::tim(const QString& maMon, const QDate& ngayThi)
{
    auto node = timNode(maMon, ngayThi);
    return node ? &(node->data) : nullptr;
}

QList<DiemThi> DanhSachDiemThi::layTatCa() const
{
    QList<DiemThi> danhSach;
    auto current = head;
    while (current) {
        danhSach.append(current->data);
        current = current->next;
    }
    return danhSach;
}

void DanhSachDiemThi::xoaTatCa()
{
    head = nullptr;
    soLuong = 0;
}

std::shared_ptr<NodeDiemThi> DanhSachDiemThi::timNode(const QString& maMon, const QDate& ngayThi)
{
    auto current = head;
    while (current) {
        if (current->data.getMaMon() == maMon && current->data.getNgayThi() == ngayThi) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// QuanLyThi implementation is in quanlythi.cpp

// QuanLyDiemThi Implementation
QuanLyDiemThi::QuanLyDiemThi()
{
}

QuanLyDiemThi::~QuanLyDiemThi()
{
}

bool QuanLyDiemThi::themDiemThi(const DiemThi& diem)
{
    tatCaDiemThi.append(diem);
    return true;
}

bool QuanLyDiemThi::xoaDiemThi(const QString& maSV, const QString& maMon, const QDate& ngayThi)
{
    for (int i = 0; i < tatCaDiemThi.size(); ++i) {
        const DiemThi& dt = tatCaDiemThi[i];
        if (dt.getMaSV() == maSV && dt.getMaMon() == maMon && dt.getNgayThi() == ngayThi) {
            tatCaDiemThi.removeAt(i);
            return true;
        }
    }
    return false;
}

DiemThi* QuanLyDiemThi::timDiemThi(const QString& maSV, const QString& maMon, const QDate& ngayThi)
{
    for (int i = 0; i < tatCaDiemThi.size(); ++i) {
        DiemThi& dt = tatCaDiemThi[i];
        if (dt.getMaSV() == maSV && dt.getMaMon() == maMon && dt.getNgayThi() == ngayThi) {
            return &dt;
        }
    }
    return nullptr;
}

QList<DiemThi> QuanLyDiemThi::layDiemTheoSinhVien(const QString& maSV) const
{
    QList<DiemThi> result;
    for (const DiemThi& dt : tatCaDiemThi) {
        if (dt.getMaSV() == maSV) {
            result.append(dt);
        }
    }
    return result;
}

QList<DiemThi> QuanLyDiemThi::layDiemTheoMon(const QString& maMon) const
{
    QList<DiemThi> result;
    for (const DiemThi& dt : tatCaDiemThi) {
        if (dt.getMaMon() == maMon) {
            result.append(dt);
        }
    }
    return result;
}

bool QuanLyDiemThi::luuVaoFile(const QString& tenFile) const
{
    QFile file(tenFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonArray array;
    for (const DiemThi& dt : tatCaDiemThi) {
        QJsonObject obj;
        obj["maSV"] = dt.getMaSV();
        obj["maMon"] = dt.getMaMon();
        obj["ngayThi"] = dt.getNgayThi().toString("yyyy-MM-dd");
        obj["gioThi"] = dt.getGioThi().toString("hh:mm:ss");
        obj["diem"] = dt.getDiem();
        obj["soCauDung"] = dt.getSoCauDung();
        obj["tongSoCau"] = dt.getTongSoCau();
        obj["thoiGianLam"] = dt.getThoiGianLam();
        array.append(obj);
    }

    QJsonObject root;
    root["diemThi"] = array;

    QJsonDocument doc(root);
    file.write(doc.toJson());
    return true;
}

bool QuanLyDiemThi::docTuFile(const QString& tenFile)
{
    QFile file(tenFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray array = root["diemThi"].toArray();

    tatCaDiemThi.clear();
    
    for (const QJsonValue& value : array) {
        QJsonObject obj = value.toObject();
        DiemThi dt(obj["maSV"].toString(),
                   obj["maMon"].toString(),
                   QDate::fromString(obj["ngayThi"].toString(), "yyyy-MM-dd"),
                   QTime::fromString(obj["gioThi"].toString(), "hh:mm:ss"));
        tatCaDiemThi.append(dt);
    }

    return true;
}
