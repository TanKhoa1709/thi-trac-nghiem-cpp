#ifndef QUANLYCAUHOI_H
#define QUANLYCAUHOI_H

#include "../utils/BinaryTree.h"
#include "../utils/DynamicArray.h"
#include "../models/cauhoi.h"

/**
 * @brief Quản lý câu hỏi - Cây nhị phân tìm kiếm
 * Sử dụng BST với khóa là ID câu hỏi để tối ưu tìm kiếm
 */
class QuanLyCauHoi {
private:
    char maMon[16];                                    // Mã môn học
    BinarySearchTree<CauHoi> cayQuanLyCauHoi;         // Cây BST quản lý câu hỏi

public:
    // Constructors & Destructor
    explicit QuanLyCauHoi(const char* maMon);
    ~QuanLyCauHoi();

    // Basic CRUD operations
    DynamicArray<CauHoi> danhSach();
    CauHoi* tim(int maCauHoi);
    bool them(CauHoi& cauHoi);
    bool sua(CauHoi& cauHoi);
    bool xoa(int maCauHoi);
    
    // ID generation - Tự động tạo ngẫu nhiên và duy nhất
    int taoMaCauHoiNgauNhien();
    
    // Random question selection
    DynamicArray<CauHoi> layNgauNhien(int soLuong);
    
    // Data persistence data/cauhoi/cauhoi_<maMon>.txt
    void saveToFile();
    void loadFromFile();

    // Utility methods
    int size() { return cayQuanLyCauHoi.size(); }
    bool isEmpty() { return cayQuanLyCauHoi.isEmpty(); }
    
    // Validation
    bool kiemTraCauHoiDaSuDung(int maCauHoi); // TODO
};

#endif // QUANLYCAUHOI_H