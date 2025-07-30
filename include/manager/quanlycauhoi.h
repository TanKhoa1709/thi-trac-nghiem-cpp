#ifndef QUANLYCAUHOI_H
#define QUANLYCAUHOI_H

#include <string>
#include "../utils/BinaryTree.h"
#include "../models/cauhoi.h"

/**
 * @brief Quản lý câu hỏi - Cây nhị phân tìm kiếm
 */
class QuanLyCauHoi {
private:
    BinarySearchTree<CauHoi> cayQuanLyCauHoi;  // Cây BST với khóa ID

public:
    QuanLyCauHoi();
    ~QuanLyCauHoi();

    // Basic CRUD
    bool themCauHoi(const CauHoi& cauHoi);
    // Xoa cau hoi (ko an toàn)
    bool xoaCauHoi(int maCauHoi); 
    CauHoi* timCauHoi(int maCauHoi);
    
    // ID generation - Tự động tạo ngẫu nhiên
    int taoMaCauHoiNgauNhien();
    
    // Lấy câu hỏi ngẫu nhiên
    void layNgauNhienCauHoi(int soLuong);
    
    // Display
    void inTatCaCauHoi() const;
    
    // Không cho phép xóa câu hỏi đã có trong bài thi
    bool xoaCauHoiAnToan(int maCauHoi);
    
    // Utility methods
    int getSoLuongCauHoi() const { return cayQuanLyCauHoi.size(); }
    
    // File I/O
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif // QUANLYCAUHOI_H