#ifndef CAUHOI_H
#define CAUHOI_H

#include <string>

/**
 * @brief Câu hỏi trắc nghiệm - Id, Nội dung, A, B, C, D, Đáp án
 */
class CauHoi {
private:
    int maCauHoi;                // ID câu hỏi (khóa duy nhất)
    std::string noiDung;         // Nội dung câu hỏi
    std::string luaChonA;        // Lựa chọn A
    std::string luaChonB;        // Lựa chọn B
    std::string luaChonC;        // Lựa chọn C
    std::string luaChonD;        // Lựa chọn D
    char dapAnDung;              // Đáp án đúng (A, B, C, D)

public:
    // Constructors & Destructor
    CauHoi();
    CauHoi(int ma, const std::string& noiDung, const std::string& a, const std::string& b,
           const std::string& c, const std::string& d, char dapAn);
    ~CauHoi() = default;

    // Getters (const methods)
    int getMaCauHoi() const { return maCauHoi; }
    const std::string& getNoiDung() const { return noiDung; }
    const std::string& getLuaChonA() const { return luaChonA; }
    const std::string& getLuaChonB() const { return luaChonB; }
    const std::string& getLuaChonC() const { return luaChonC; }
    const std::string& getLuaChonD() const { return luaChonD; }
    char getDapAnDung() const { return dapAnDung; }

    // Setters
    void setMaCauHoi(int ma) { maCauHoi = ma; }
    void setNoiDung(const std::string& nd) { noiDung = nd; }
    void setLuaChonA(const std::string& a) { luaChonA = a; }
    void setLuaChonB(const std::string& b) { luaChonB = b; }
    void setLuaChonC(const std::string& c) { luaChonC = c; }
    void setLuaChonD(const std::string& d) { luaChonD = d; }
    void setDapAnDung(char dapAn) { dapAnDung = dapAn; }

    // Business logic methods
    bool validate() const;
    bool kiemTraDapAn(char dapAnChon) const;

    // Operators for BST operations
    bool operator<(const CauHoi& other) const { return maCauHoi < other.maCauHoi; }
    bool operator==(const CauHoi& other) const { return maCauHoi == other.maCauHoi; }
    bool operator>(const CauHoi& other) const { return maCauHoi > other.maCauHoi; }
};

#endif // CAUHOI_H