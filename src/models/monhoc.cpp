#include "monhoc.h"
#include "cauhoi.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

// MonHoc Implementation
MonHoc::MonHoc() : soTinChi(3)
{
    cayCauHoi = std::make_shared<CayNhiPhan>();
}

MonHoc::MonHoc(const QString& ma, const QString& ten, int tinChi)
    : maMon(ma), tenMon(ten), soTinChi(tinChi)
{
    cayCauHoi = std::make_shared<CayNhiPhan>();
}

MonHoc::~MonHoc()
{
}

bool MonHoc::themCauHoi(const CauHoi& cauHoi)
{
    if (cayCauHoi) {
        return cayCauHoi->them(cauHoi);
    }
    return false;
}

bool MonHoc::xoaCauHoi(int idCauHoi)
{
    if (cayCauHoi) {
        return cayCauHoi->xoa(idCauHoi);
    }
    return false;
}

CauHoi* MonHoc::timCauHoi(int idCauHoi)
{
    if (cayCauHoi) {
        return cayCauHoi->timKiem(idCauHoi);
    }
    return nullptr;
}

QList<CauHoi> MonHoc::layDanhSachCauHoi() const
{
    if (cayCauHoi) {
        return cayCauHoi->duyetInOrder();
    }
    return QList<CauHoi>();
}

QList<CauHoi> MonHoc::layNgauNhienCauHoi(int soLuong) const
{
    if (cayCauHoi) {
        return cayCauHoi->layNgauNhien(soLuong);
    }
    return QList<CauHoi>();
}

bool MonHoc::luuVaoFile(const QString& tenFile) const
{
    QFile file(tenFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonObject obj;
    obj["maMon"] = maMon;
    obj["tenMon"] = tenMon;
    obj["soTinChi"] = soTinChi;

    QJsonDocument doc(obj);
    file.write(doc.toJson());
    return true;
}

bool MonHoc::docTuFile(const QString& tenFile)
{
    QFile file(tenFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    maMon = obj["maMon"].toString();
    tenMon = obj["tenMon"].toString();
    soTinChi = obj["soTinChi"].toInt(3);

    return true;
}

bool MonHoc::operator==(const MonHoc& other) const
{
    return maMon == other.maMon;
}

bool MonHoc::operator<(const MonHoc& other) const
{
    return maMon < other.maMon;
}

// QuanLyMonHoc Implementation
QuanLyMonHoc::QuanLyMonHoc()
{
}

QuanLyMonHoc::~QuanLyMonHoc()
{
}

bool QuanLyMonHoc::themMonHoc(const MonHoc& monHoc)
{
    if (kiemTraMaMonTonTai(monHoc.getMaMon())) {
        return false;
    }
    danhSachMonHoc.append(monHoc);
    return true;
}

bool QuanLyMonHoc::xoaMonHoc(const QString& maMon)
{
    for (int i = 0; i < danhSachMonHoc.size(); ++i) {
        if (danhSachMonHoc[i].getMaMon() == maMon) {
            danhSachMonHoc.removeAt(i);
            return true;
        }
    }
    return false;
}

bool QuanLyMonHoc::capNhatMonHoc(const QString& maMon, const MonHoc& monHocMoi)
{
    for (int i = 0; i < danhSachMonHoc.size(); ++i) {
        if (danhSachMonHoc[i].getMaMon() == maMon) {
            danhSachMonHoc[i] = monHocMoi;
            return true;
        }
    }
    return false;
}

MonHoc* QuanLyMonHoc::timMonHoc(const QString& maMon)
{
    for (int i = 0; i < danhSachMonHoc.size(); ++i) {
        if (danhSachMonHoc[i].getMaMon() == maMon) {
            return &danhSachMonHoc[i];
        }
    }
    return nullptr;
}

bool QuanLyMonHoc::luuVaoFile(const QString& tenFile) const
{
    QFile file(tenFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonArray array;
    for (const MonHoc& monHoc : danhSachMonHoc) {
        QJsonObject obj;
        obj["maMon"] = monHoc.getMaMon();
        obj["tenMon"] = monHoc.getTenMon();
        obj["soTinChi"] = monHoc.getSoTinChi();
        array.append(obj);
    }

    QJsonObject root;
    root["monHoc"] = array;

    QJsonDocument doc(root);
    file.write(doc.toJson());
    return true;
}

bool QuanLyMonHoc::docTuFile(const QString& tenFile)
{
    QFile file(tenFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray array = root["monHoc"].toArray();

    danhSachMonHoc.clear();
    for (const QJsonValue& value : array) {
        QJsonObject obj = value.toObject();
        MonHoc monHoc(obj["maMon"].toString(), 
                      obj["tenMon"].toString(), 
                      obj["soTinChi"].toInt(3));
        danhSachMonHoc.append(monHoc);
    }

    return true;
}

bool QuanLyMonHoc::kiemTraMaMonTonTai(const QString& maMon) const
{
    for (const MonHoc& monHoc : danhSachMonHoc) {
        if (monHoc.getMaMon() == maMon) {
            return true;
        }
    }
    return false;
}
