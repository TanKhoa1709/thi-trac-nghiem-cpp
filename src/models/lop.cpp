#include "lop.h"
#include "sinhvien.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// Lop Implementation
Lop::Lop() : nienKhoa(2024)
{
    danhSachSV = std::make_shared<DanhSachSinhVien>();
}

Lop::Lop(const QString& ma, const QString& ten, int nien, const QString& khoa)
    : maLop(ma), tenLop(ten), nienKhoa(nien), khoa(khoa)
{
    danhSachSV = std::make_shared<DanhSachSinhVien>();
}

Lop::~Lop()
{
}

bool Lop::themSinhVien(const SinhVien& sv)
{
    if (danhSachSV) {
        return danhSachSV->them(sv);
    }
    return false;
}

bool Lop::xoaSinhVien(const QString& maSV)
{
    if (danhSachSV) {
        return danhSachSV->xoa(maSV);
    }
    return false;
}

SinhVien* Lop::timSinhVien(const QString& maSV)
{
    if (danhSachSV) {
        return danhSachSV->tim(maSV);
    }
    return nullptr;
}

int Lop::soLuongSinhVien() const
{
    if (danhSachSV) {
        return danhSachSV->getSoLuong();
    }
    return 0;
}

QList<SinhVien> Lop::layDanhSachSinhVien() const
{
    if (danhSachSV) {
        return danhSachSV->layTatCa();
    }
    return QList<SinhVien>();
}

bool Lop::operator==(const Lop& other) const
{
    return maLop == other.maLop;
}

bool Lop::operator<(const Lop& other) const
{
    return maLop < other.maLop;
}

// QuanLyLop Implementation
QuanLyLop::QuanLyLop() : soLuongLop(0)
{
    for (int i = 0; i < MAX_LOP; ++i) {
        danhSachLop[i] = nullptr;
    }
}

QuanLyLop::~QuanLyLop()
{
    xoaTatCa();
}

bool QuanLyLop::themLop(const Lop& lop)
{
    if (dayDu() || kiemTraMaLopTonTai(lop.getMaLop())) {
        return false;
    }
    
    danhSachLop[soLuongLop] = new Lop(lop);
    soLuongLop++;
    return true;
}

bool QuanLyLop::xoaLop(const QString& maLop)
{
    int viTri = timViTri(maLop);
    if (viTri == -1) {
        return false;
    }
    
    delete danhSachLop[viTri];
    
    // Shift elements
    for (int i = viTri; i < soLuongLop - 1; ++i) {
        danhSachLop[i] = danhSachLop[i + 1];
    }
    
    danhSachLop[soLuongLop - 1] = nullptr;
    soLuongLop--;
    return true;
}

Lop* QuanLyLop::timLop(const QString& maLop)
{
    int viTri = timViTri(maLop);
    if (viTri != -1) {
        return danhSachLop[viTri];
    }
    return nullptr;
}

Lop* QuanLyLop::layLop(int index)
{
    if (index >= 0 && index < soLuongLop) {
        return danhSachLop[index];
    }
    return nullptr;
}

bool QuanLyLop::luuVaoFile(const QString& tenFile) const
{
    QFile file(tenFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonArray array;
    for (int i = 0; i < soLuongLop; ++i) {
        if (danhSachLop[i]) {
            QJsonObject obj;
            obj["maLop"] = danhSachLop[i]->getMaLop();
            obj["tenLop"] = danhSachLop[i]->getTenLop();
            obj["nienKhoa"] = danhSachLop[i]->getNienKhoa();
            obj["khoa"] = danhSachLop[i]->getKhoa();
            array.append(obj);
        }
    }

    QJsonObject root;
    root["lop"] = array;

    QJsonDocument doc(root);
    file.write(doc.toJson());
    return true;
}

bool QuanLyLop::docTuFile(const QString& tenFile)
{
    QFile file(tenFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray array = root["lop"].toArray();

    xoaTatCa();
    
    for (const QJsonValue& value : array) {
        QJsonObject obj = value.toObject();
        Lop lop(obj["maLop"].toString(),
                obj["tenLop"].toString(),
                obj["nienKhoa"].toInt(2024),
                obj["khoa"].toString());
        themLop(lop);
    }

    return true;
}

void QuanLyLop::xoaTatCa()
{
    for (int i = 0; i < soLuongLop; ++i) {
        delete danhSachLop[i];
        danhSachLop[i] = nullptr;
    }
    soLuongLop = 0;
}

bool QuanLyLop::kiemTraMaLopTonTai(const QString& maLop) const
{
    return timViTri(maLop) != -1;
}

int QuanLyLop::timViTri(const QString& maLop) const
{
    for (int i = 0; i < soLuongLop; ++i) {
        if (danhSachLop[i] && danhSachLop[i]->getMaLop() == maLop) {
            return i;
        }
    }
    return -1;
}
