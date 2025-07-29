#include "sinhvien.h"
#include "diemthi.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// SinhVien Implementation
SinhVien::SinhVien() : ngaySinh(QDate::currentDate()), gioiTinh("Nam")
{
    danhSachDiem = std::make_shared<DanhSachDiemThi>();
}

SinhVien::SinhVien(const QString& ma, const QString& ho, const QString& ten, 
                   const QDate& ngaySinh, const QString& gioiTinh, 
                   const QString& sdt, const QString& email, const QString& maLop)
    : maSV(ma), ho(ho), ten(ten), ngaySinh(ngaySinh), gioiTinh(gioiTinh),
      soDienThoai(sdt), email(email), maLop(maLop)
{
    danhSachDiem = std::make_shared<DanhSachDiemThi>();
}

SinhVien::~SinhVien()
{
}

bool SinhVien::themDiemThi(const DiemThi& diem)
{
    if (danhSachDiem) {
        return danhSachDiem->them(diem);
    }
    return false;
}

bool SinhVien::xoaDiemThi(const QString& maMon, const QDate& ngayThi)
{
    if (danhSachDiem) {
        return danhSachDiem->xoa(maMon, ngayThi);
    }
    return false;
}

DiemThi* SinhVien::timDiemThi(const QString& maMon, const QDate& ngayThi)
{
    if (danhSachDiem) {
        return danhSachDiem->tim(maMon, ngayThi);
    }
    return nullptr;
}

QList<DiemThi> SinhVien::layDanhSachDiem() const
{
    if (danhSachDiem) {
        return danhSachDiem->layTatCa();
    }
    return QList<DiemThi>();
}

bool SinhVien::operator==(const SinhVien& other) const
{
    return maSV == other.maSV;
}

bool SinhVien::operator<(const SinhVien& other) const
{
    return maSV < other.maSV;
}

// NodeSinhVien Implementation
NodeSinhVien::NodeSinhVien(const SinhVien& sv) : data(sv)
{
}

NodeSinhVien::~NodeSinhVien()
{
}

// DanhSachSinhVien Implementation
DanhSachSinhVien::DanhSachSinhVien() : soLuong(0)
{
}

DanhSachSinhVien::~DanhSachSinhVien()
{
    xoaTatCa();
}

bool DanhSachSinhVien::them(const SinhVien& sv)
{
    if (tim(sv.getMaSV())) {
        return false; // Already exists
    }
    
    auto newNode = std::make_shared<NodeSinhVien>(sv);
    newNode->next = head;
    head = newNode;
    soLuong++;
    return true;
}

bool DanhSachSinhVien::xoa(const QString& maSV)
{
    if (!head) return false;
    
    if (head->data.getMaSV() == maSV) {
        head = head->next;
        soLuong--;
        return true;
    }
    
    auto current = head;
    while (current->next && current->next->data.getMaSV() != maSV) {
        current = current->next;
    }
    
    if (current->next) {
        current->next = current->next->next;
        soLuong--;
        return true;
    }
    
    return false;
}

SinhVien* DanhSachSinhVien::tim(const QString& maSV)
{
    auto node = timNode(maSV);
    return node ? &(node->data) : nullptr;
}

const SinhVien* DanhSachSinhVien::tim(const QString& maSV) const
{
    auto node = const_cast<DanhSachSinhVien*>(this)->timNode(maSV);
    return node ? &(node->data) : nullptr;
}

QList<SinhVien> DanhSachSinhVien::layTatCa() const
{
    QList<SinhVien> danhSach;
    auto current = head;
    while (current) {
        danhSach.append(current->data);
        current = current->next;
    }
    return danhSach;
}

void DanhSachSinhVien::xoaTatCa()
{
    head = nullptr;
    soLuong = 0;
}

bool DanhSachSinhVien::luuVaoFile(const QString& tenFile) const
{
    QFile file(tenFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonArray array;
    auto current = head;
    while (current) {
        QJsonObject obj;
        obj["maSV"] = current->data.getMaSV();
        obj["ho"] = current->data.getHo();
        obj["ten"] = current->data.getTen();
        obj["ngaySinh"] = current->data.getNgaySinh().toString("yyyy-MM-dd");
        obj["gioiTinh"] = current->data.getGioiTinh();
        obj["soDienThoai"] = current->data.getSoDienThoai();
        obj["email"] = current->data.getEmail();
        obj["maLop"] = current->data.getMaLop();
        array.append(obj);
        current = current->next;
    }

    QJsonObject root;
    root["sinhVien"] = array;

    QJsonDocument doc(root);
    file.write(doc.toJson());
    return true;
}

bool DanhSachSinhVien::docTuFile(const QString& tenFile)
{
    QFile file(tenFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray array = root["sinhVien"].toArray();

    xoaTatCa();
    
    for (const QJsonValue& value : array) {
        QJsonObject obj = value.toObject();
        SinhVien sv(obj["maSV"].toString(),
                    obj["ho"].toString(),
                    obj["ten"].toString(),
                    QDate::fromString(obj["ngaySinh"].toString(), "yyyy-MM-dd"),
                    obj["gioiTinh"].toString(),
                    obj["soDienThoai"].toString(),
                    obj["email"].toString(),
                    obj["maLop"].toString());
        them(sv);
    }

    return true;
}

std::shared_ptr<NodeSinhVien> DanhSachSinhVien::timNode(const QString& maSV)
{
    auto current = head;
    while (current && current->data.getMaSV() != maSV) {
        current = current->next;
    }
    return current;
}

// QuanLySinhVien Implementation
QuanLySinhVien::QuanLySinhVien() : viTriHienTai(-1)
{
}

QuanLySinhVien::~QuanLySinhVien()
{
}

bool QuanLySinhVien::themSinhVien(const SinhVien& sv)
{
    return danhSach.them(sv);
}

bool QuanLySinhVien::xoaSinhVien(const QString& maSV)
{
    return danhSach.xoa(maSV);
}

SinhVien* QuanLySinhVien::timSinhVien(const QString& maSV)
{
    return danhSach.tim(maSV);
}

bool QuanLySinhVien::luuVaoFile(const QString& tenFile) const
{
    return danhSach.luuVaoFile(tenFile);
}

bool QuanLySinhVien::docTuFile(const QString& tenFile)
{
    return danhSach.docTuFile(tenFile);
}

bool QuanLySinhVien::kiemTraMaSVTonTai(const QString& maSV) const
{
    return danhSach.tim(maSV) != nullptr;
}
