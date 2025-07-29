#include "cauhoi.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QDebug>

// CauHoi Implementation
CauHoi::CauHoi() : id(0), dapAnDung('A'), mucDo(1)
{
}

CauHoi::CauHoi(int id, const QString& noiDung, const QStringList& dapAn, 
               char dapAnDung, const QString& maMon, int mucDo)
    : id(id), noiDung(noiDung), dapAn(dapAn), dapAnDung(dapAnDung), maMon(maMon), mucDo(mucDo)
{
}

CauHoi::~CauHoi()
{
}

bool CauHoi::kiemTraDapAn(char dapAnChon) const
{
    return dapAnChon == dapAnDung;
}

bool CauHoi::kiemTraHopLe() const
{
    return !noiDung.isEmpty() && dapAn.size() >= 2 && 
           (dapAnDung >= 'A' && dapAnDung <= 'D');
}

bool CauHoi::operator==(const CauHoi& other) const
{
    return id == other.id;
}

bool CauHoi::operator<(const CauHoi& other) const
{
    return id < other.id;
}

bool CauHoi::operator>(const CauHoi& other) const
{
    return id > other.id;
}

// NodeCauHoi Implementation
NodeCauHoi::NodeCauHoi(const CauHoi& cauHoi) : data(cauHoi)
{
}

NodeCauHoi::~NodeCauHoi()
{
}

// CayNhiPhan Implementation
CayNhiPhan::CayNhiPhan() : soLuongNode(0)
{
}

CayNhiPhan::~CayNhiPhan()
{
    xoaTatCa();
}

bool CayNhiPhan::them(const CauHoi& cauHoi)
{
    root = themNode(root, cauHoi);
    soLuongNode++;
    return true;
}

bool CayNhiPhan::xoa(int id)
{
    if (timKiem(id)) {
        root = xoaNode(root, id);
        soLuongNode--;
        return true;
    }
    return false;
}

CauHoi* CayNhiPhan::timKiem(int id)
{
    NodeCauHoi* node = timKiemNode(root, id);
    return node ? &(node->data) : nullptr;
}

const CauHoi* CayNhiPhan::timKiem(int id) const
{
    NodeCauHoi* node = const_cast<CayNhiPhan*>(this)->timKiemNode(root, id);
    return node ? &(node->data) : nullptr;
}

QList<CauHoi> CayNhiPhan::duyetInOrder() const
{
    QList<CauHoi> danhSach;
    duyetInOrder(root, danhSach);
    return danhSach;
}

QList<CauHoi> CayNhiPhan::layNgauNhien(int soLuong) const
{
    QList<CauHoi> tatCa = duyetInOrder();
    QList<CauHoi> ngauNhien;
    
    if (tatCa.size() <= soLuong) {
        return tatCa;
    }
    
    for (int i = 0; i < soLuong && i < tatCa.size(); ++i) {
        int index = QRandomGenerator::global()->bounded(tatCa.size());
        ngauNhien.append(tatCa[index]);
        tatCa.removeAt(index);
    }
    
    return ngauNhien;
}

void CayNhiPhan::xoaTatCa()
{
    xoaTatCaNode(root);
    root = nullptr;
    soLuongNode = 0;
}

bool CayNhiPhan::luuVaoFile(const QString& tenFile) const
{
    QFile file(tenFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QList<CauHoi> danhSach = duyetInOrder();
    QJsonArray array;
    
    for (const CauHoi& ch : danhSach) {
        QJsonObject obj;
        obj["id"] = ch.getId();
        obj["noiDung"] = ch.getNoiDung();
        obj["dapAn"] = QJsonArray::fromStringList(ch.getDapAn());
        obj["dapAnDung"] = QString(ch.getDapAnDung());
        obj["maMon"] = ch.getMaMon();
        obj["mucDo"] = ch.getMucDo();
        array.append(obj);
    }

    QJsonObject root;
    root["cauHoi"] = array;

    QJsonDocument doc(root);
    file.write(doc.toJson());
    return true;
}

bool CayNhiPhan::docTuFile(const QString& tenFile)
{
    QFile file(tenFile);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject rootObj = doc.object();
    QJsonArray array = rootObj["cauHoi"].toArray();

    xoaTatCa();
    
    for (const QJsonValue& value : array) {
        QJsonObject obj = value.toObject();
        QStringList dapAn;
        QJsonArray dapAnArray = obj["dapAn"].toArray();
        for (const QJsonValue& v : dapAnArray) {
            dapAn.append(v.toString());
        }
        
        CauHoi ch(obj["id"].toInt(),
                  obj["noiDung"].toString(),
                  dapAn,
                  obj["dapAnDung"].toString().at(0).toLatin1(),
                  obj["maMon"].toString(),
                  obj["mucDo"].toInt(1));
        them(ch);
    }

    return true;
}

// Private helper methods
std::shared_ptr<NodeCauHoi> CayNhiPhan::themNode(std::shared_ptr<NodeCauHoi> node, const CauHoi& cauHoi)
{
    if (!node) {
        return std::make_shared<NodeCauHoi>(cauHoi);
    }
    
    if (cauHoi < node->data) {
        node->left = themNode(node->left, cauHoi);
    } else if (cauHoi > node->data) {
        node->right = themNode(node->right, cauHoi);
    }
    
    return node;
}

std::shared_ptr<NodeCauHoi> CayNhiPhan::xoaNode(std::shared_ptr<NodeCauHoi> node, int id)
{
    if (!node) return node;
    
    if (id < node->data.getId()) {
        node->left = xoaNode(node->left, id);
    } else if (id > node->data.getId()) {
        node->right = xoaNode(node->right, id);
    } else {
        if (!node->left) return node->right;
        if (!node->right) return node->left;
        
        std::shared_ptr<NodeCauHoi> temp = timMin(node->right);
        node->data = temp->data;
        node->right = xoaNode(node->right, temp->data.getId());
    }
    
    return node;
}

std::shared_ptr<NodeCauHoi> CayNhiPhan::timMin(std::shared_ptr<NodeCauHoi> node)
{
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

NodeCauHoi* CayNhiPhan::timKiemNode(std::shared_ptr<NodeCauHoi> node, int id)
{
    if (!node || node->data.getId() == id) {
        return node.get();
    }
    
    if (id < node->data.getId()) {
        return timKiemNode(node->left, id);
    }
    
    return timKiemNode(node->right, id);
}

void CayNhiPhan::duyetInOrder(std::shared_ptr<NodeCauHoi> node, QList<CauHoi>& danhSach) const
{
    if (node) {
        duyetInOrder(node->left, danhSach);
        danhSach.append(node->data);
        duyetInOrder(node->right, danhSach);
    }
}

void CayNhiPhan::xoaTatCaNode(std::shared_ptr<NodeCauHoi> node)
{
    if (node) {
        xoaTatCaNode(node->left);
        xoaTatCaNode(node->right);
    }
}

// QuanLyCauHoi Implementation
QuanLyCauHoi::QuanLyCauHoi() : viTriHienTai(-1)
{
}

QuanLyCauHoi::~QuanLyCauHoi()
{
}

bool QuanLyCauHoi::themCauHoi(const CauHoi& cauHoi)
{
    return cayCauHoi.them(cauHoi);
}

bool QuanLyCauHoi::xoaCauHoi(int id)
{
    return cayCauHoi.xoa(id);
}

bool QuanLyCauHoi::capNhatCauHoi(int id, const CauHoi& cauHoiMoi)
{
    if (xoaCauHoi(id)) {
        return themCauHoi(cauHoiMoi);
    }
    return false;
}

CauHoi* QuanLyCauHoi::timCauHoi(int id)
{
    return cayCauHoi.timKiem(id);
}

bool QuanLyCauHoi::undo()
{
    // Simplified undo implementation
    return false;
}

bool QuanLyCauHoi::redo()
{
    // Simplified redo implementation
    return false;
}

bool QuanLyCauHoi::coTheUndo() const
{
    return viTriHienTai >= 0;
}

bool QuanLyCauHoi::coTheRedo() const
{
    return viTriHienTai < lichSuThayDoi.size() - 1;
}

QList<CauHoi> QuanLyCauHoi::layTatCaCauHoi() const
{
    return cayCauHoi.duyetInOrder();
}

QList<CauHoi> QuanLyCauHoi::layCauHoiTheoMon(const QString& maMon) const
{
    QList<CauHoi> tatCa = layTatCaCauHoi();
    QList<CauHoi> theoMon;
    
    for (const CauHoi& ch : tatCa) {
        if (ch.getMaMon() == maMon) {
            theoMon.append(ch);
        }
    }
    
    return theoMon;
}

QList<CauHoi> QuanLyCauHoi::layCauHoiTheoMucDo(int mucDo) const
{
    QList<CauHoi> tatCa = layTatCaCauHoi();
    QList<CauHoi> theoMucDo;
    
    for (const CauHoi& ch : tatCa) {
        if (ch.getMucDo() == mucDo) {
            theoMucDo.append(ch);
        }
    }
    
    return theoMucDo;
}

QList<CauHoi> QuanLyCauHoi::layNgauNhienCauHoi(int soLuong, const QString& maMon) const
{
    if (maMon.isEmpty()) {
        return cayCauHoi.layNgauNhien(soLuong);
    } else {
        QList<CauHoi> theoMon = layCauHoiTheoMon(maMon);
        QList<CauHoi> ngauNhien;
        
        for (int i = 0; i < soLuong && i < theoMon.size(); ++i) {
            int index = QRandomGenerator::global()->bounded(theoMon.size());
            ngauNhien.append(theoMon[index]);
            theoMon.removeAt(index);
        }
        
        return ngauNhien;
    }
}

int QuanLyCauHoi::tongSoCauHoi() const
{
    return cayCauHoi.getSoLuong();
}

int QuanLyCauHoi::soCauHoiTheoMon(const QString& maMon) const
{
    return layCauHoiTheoMon(maMon).size();
}

bool QuanLyCauHoi::luuVaoFile(const QString& tenFile) const
{
    return cayCauHoi.luuVaoFile(tenFile);
}

bool QuanLyCauHoi::docTuFile(const QString& tenFile)
{
    return cayCauHoi.docTuFile(tenFile);
}

int QuanLyCauHoi::taoIdMoi() const
{
    QList<CauHoi> tatCa = layTatCaCauHoi();
    int maxId = 0;
    for (const CauHoi& ch : tatCa) {
        if (ch.getId() > maxId) {
            maxId = ch.getId();
        }
    }
    return maxId + 1;
}

bool QuanLyCauHoi::kiemTraIdTonTai(int id) const
{
    return const_cast<CayNhiPhan&>(cayCauHoi).timKiem(id) != nullptr;
}
