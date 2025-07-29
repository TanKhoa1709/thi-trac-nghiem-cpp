#ifndef CAUHOI_H
#define CAUHOI_H

#include <QString>
#include <QStringList>
#include <QList>
#include <memory>

/**
 * @brief Câu hỏi trắc nghiệm - Multiple choice question
 */
class CauHoi {
private:
    int id;                     // Unique question ID
    QString noiDung;            // Question content
    QStringList dapAn;          // Answer options (A, B, C, D)
    char dapAnDung;             // Correct answer (A, B, C, D)
    QString maMon;              // Subject code
    int mucDo;                  // Difficulty level (1-5)

public:
    CauHoi();
    CauHoi(int id, const QString& noiDung, const QStringList& dapAn, 
           char dapAnDung, const QString& maMon, int mucDo = 1);
    ~CauHoi();

    // Getters
    int getId() const { return id; }
    QString getNoiDung() const { return noiDung; }
    QStringList getDapAn() const { return dapAn; }
    char getDapAnDung() const { return dapAnDung; }
    QString getMaMon() const { return maMon; }
    int getMucDo() const { return mucDo; }

    // Setters
    void setId(int newId) { id = newId; }
    void setNoiDung(const QString& nd) { noiDung = nd; }
    void setDapAn(const QStringList& da) { dapAn = da; }
    void setDapAnDung(char da) { dapAnDung = da; }
    void setMaMon(const QString& ma) { maMon = ma; }
    void setMucDo(int md) { mucDo = md; }

    // Validation
    bool kiemTraDapAn(char dapAnChon) const;
    bool kiemTraHopLe() const;

    // Operators
    bool operator==(const CauHoi& other) const;
    bool operator<(const CauHoi& other) const;
    bool operator>(const CauHoi& other) const;
};

/**
 * @brief Node của cây nhị phân tìm kiếm câu hỏi
 */
class NodeCauHoi {
public:
    CauHoi data;
    std::shared_ptr<NodeCauHoi> left;
    std::shared_ptr<NodeCauHoi> right;

    NodeCauHoi(const CauHoi& cauHoi);
    ~NodeCauHoi();
};

/**
 * @brief Cây nhị phân tìm kiếm để quản lý câu hỏi theo ID
 */
class CayNhiPhan {
private:
    std::shared_ptr<NodeCauHoi> root;
    int soLuongNode;

    // Helper methods
    std::shared_ptr<NodeCauHoi> themNode(std::shared_ptr<NodeCauHoi> node, const CauHoi& cauHoi);
    std::shared_ptr<NodeCauHoi> xoaNode(std::shared_ptr<NodeCauHoi> node, int id);
    std::shared_ptr<NodeCauHoi> timMin(std::shared_ptr<NodeCauHoi> node);
    NodeCauHoi* timKiemNode(std::shared_ptr<NodeCauHoi> node, int id);
    void duyetInOrder(std::shared_ptr<NodeCauHoi> node, QList<CauHoi>& danhSach) const;
    void xoaTatCaNode(std::shared_ptr<NodeCauHoi> node);

public:
    CayNhiPhan();
    ~CayNhiPhan();

    // Tree operations
    bool them(const CauHoi& cauHoi);
    bool xoa(int id);
    CauHoi* timKiem(int id);
    const CauHoi* timKiem(int id) const;
    
    // Traversal
    QList<CauHoi> duyetInOrder() const;
    QList<CauHoi> layNgauNhien(int soLuong) const;
    
    // Properties
    int getSoLuong() const { return soLuongNode; }
    bool isEmpty() const { return root == nullptr; }
    void xoaTatCa();

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);
};

/**
 * @brief Quản lý tổng thể câu hỏi với hỗ trợ undo
 */
class QuanLyCauHoi {
private:
    CayNhiPhan cayCauHoi;
    QList<CauHoi> lichSuThayDoi;    // History for undo functionality
    int viTriHienTai;               // Current position in history

public:
    QuanLyCauHoi();
    ~QuanLyCauHoi();

    // CRUD with undo support
    bool themCauHoi(const CauHoi& cauHoi);
    bool xoaCauHoi(int id);
    bool capNhatCauHoi(int id, const CauHoi& cauHoiMoi);
    CauHoi* timCauHoi(int id);

    // Undo/Redo functionality
    bool undo();
    bool redo();
    bool coTheUndo() const;
    bool coTheRedo() const;

    // Query operations
    QList<CauHoi> layTatCaCauHoi() const;
    QList<CauHoi> layCauHoiTheoMon(const QString& maMon) const;
    QList<CauHoi> layCauHoiTheoMucDo(int mucDo) const;
    QList<CauHoi> layNgauNhienCauHoi(int soLuong, const QString& maMon = "") const;

    // Statistics
    int tongSoCauHoi() const;
    int soCauHoiTheoMon(const QString& maMon) const;

    // File operations
    bool luuVaoFile(const QString& tenFile) const;
    bool docTuFile(const QString& tenFile);

    // Validation
    int taoIdMoi() const;
    bool kiemTraIdTonTai(int id) const;
};

#endif // CAUHOI_H
