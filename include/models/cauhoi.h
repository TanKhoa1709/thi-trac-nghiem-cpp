#ifndef CAUHOI_H
#define CAUHOI_H

#include <string>

/**
 * @brief Câu hỏi trắc nghiệm - Id, Nội dung, A, B, C, D, Đáp án
 */
class CauHoi {
private:
    int maCauHoi;                // ID câu hỏi (khóa duy nhất)
    std::string noiDung;         // Nội dung
    std::string luaChonA;        // A
    std::string luaChonB;        // B
    std::string luaChonC;        // C
    std::string luaChonD;        // D
    char dapAnDung;              // Đáp án (A, B, C, D)

public:
    CauHoi();
    CauHoi(int ma, const std::string& noiDung, const std::string& a, const std::string& b,
           const std::string& c, const std::string& d, char dapAn);
    ~CauHoi();

    // Getters/Setters
    int getMaCauHoi() const { return maCauHoi; }
    std::string getNoiDung() const { return noiDung; }
    std::string getLuaChonA() const { return luaChonA; }
    std::string getLuaChonB() const { return luaChonB; }
    std::string getLuaChonC() const { return luaChonC; }
    std::string getLuaChonD() const { return luaChonD; }
    char getDapAnDung() const { return dapAnDung; }

    void setMaCauHoi(int ma) { maCauHoi = ma; }
    void setNoiDung(const std::string& nd) { noiDung = nd; }
    void setLuaChonA(const std::string& a) { luaChonA = a; }
    void setLuaChonB(const std::string& b) { luaChonB = b; }
    void setLuaChonC(const std::string& c) { luaChonC = c; }
    void setLuaChonD(const std::string& d) { luaChonD = d; }
    void setDapAnDung(char dapAn) { dapAnDung = dapAn; }

    // Basic operations
    void inCauHoi() const;
    bool kiemTraDapAn(char dapAnChon) const;

    // Operators for BST
    bool operator<(const CauHoi& other) const { return maCauHoi < other.maCauHoi; }
    bool operator==(const CauHoi& other) const { return maCauHoi == other.maCauHoi; }
};

#endif // CAUHOI_H