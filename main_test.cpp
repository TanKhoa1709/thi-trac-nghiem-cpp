#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h>

// Include domain models
#include "models/sinhvien.h"
#include "models/lop.h"
#include "models/monhoc.h"
#include "models/cauhoi.h"
#include "models/diemthi.h"

// Include managers
#include "manager/quanlylop.h"
#include "manager/quanlymonhoc.h"
#include "manager/quanlysinhvien.h"
#include "manager/quanlycauhoi.h"
#include "manager/quanlydiem.h"

class SimpleExamSystem {
private:
    QuanLyLop* quanLyLop;
    QuanLyMonHoc* quanLyMonHocManager;
    std::string currentUser;
    bool isTeacher;

public:
    SimpleExamSystem() {
      SetConsoleOutputCP(CP_UTF8);
        quanLyLop = new QuanLyLop();
        quanLyMonHocManager = new QuanLyMonHoc();
        currentUser = "";
        isTeacher = false;
    }

    ~SimpleExamSystem() {
        delete quanLyLop;
        delete quanLyMonHocManager;
    }

    // Đăng nhập theo yêu cầu README.md
    bool login() {
        std::cout << "\n=== ĐĂNG NHẬP HỆ THỐNG ===" << std::endl;
        std::cout << "1. Đăng nhập Giáo viên (GV/GV)" << std::endl;
        std::cout << "2. Đăng nhập Sinh viên (MASV/password)" << std::endl;
        std::cout << "Chọn: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            std::string username, password;
            std::cout << "Tài khoản: ";
            std::cin >> username;
            std::cout << "Mật khẩu: ";
            std::cin >> password;
            
            if (username == "GV" && password == "GV") {
                currentUser = "Giáo viên";
                isTeacher = true;
                std::cout << "✓ Đăng nhập thành công với quyền Giáo viên!" << std::endl;
                return true;
            } else {
                std::cout << "✗ Sai tài khoản hoặc mật khẩu!" << std::endl;
                return false;
            }
        } else if (choice == 2) {
            std::string masv, password;
            std::cout << "Mã sinh viên: ";
            std::cin >> masv;
            std::cout << "Mật khẩu: ";
            std::cin >> password;
            
            // Tìm sinh viên trong tất cả các lớp
            SinhVien* sv = findStudent(masv);
            if (sv && sv->getPassword() == password) {
                currentUser = sv->getHoTen();
                isTeacher = false;
                std::cout << "✓ Đăng nhập thành công! Xin chào " << currentUser << std::endl;
                return true;
            } else {
                std::cout << "✗ Sai mã sinh viên hoặc mật khẩu!" << std::endl;
                return false;
            }
        }
        
        return false;
    }

    // Tìm sinh viên trong tất cả các lớp
    SinhVien* findStudent(const std::string& masv) {
        DynamicArray<Lop*> danhSachLop = quanLyLop->danhSach();
        for (size_t i = 0; i < danhSachLop.getSize(); i++) {
            QuanLySinhVien* qlsv = danhSachLop.get(i)->getQuanLySinhVien();
            SinhVien* sv = qlsv->tim(masv);
            if (sv) return sv;
        }
        return nullptr;
    }

    // Menu chính
    void showMainMenu() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "  HỆ THỐNG QUẢN LÝ THI TRẮC NGHIỆM" << std::endl;
        std::cout << "  Người dùng: " << currentUser << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        
        if (isTeacher) {
            std::cout << "1. Quản lý Lớp học" << std::endl;
            std::cout << "2. Quản lý Môn học" << std::endl;
            std::cout << "3. Quản lý Sinh viên" << std::endl;
            std::cout << "4. Quản lý Câu hỏi" << std::endl;
            std::cout << "5. Quản lý Điểm thi" << std::endl;
            std::cout << "6. Xem báo cáo điểm" << std::endl;
            std::cout << "7. Thiết lập bài thi" << std::endl;
        } else {
            std::cout << "1. Làm bài thi" << std::endl;
            std::cout << "2. Xem điểm của tôi" << std::endl;
        }
        std::cout << "0. Đăng xuất" << std::endl;
        std::cout << "Chọn: ";
    }

    // Quản lý lớp (chỉ giáo viên)
    void quanLyLopHoc() {
        std::cout << "\n=== QUẢN LÝ LỚP HỌC ===" << std::endl;
        std::cout << "1. Thêm lớp mới" << std::endl;
        std::cout << "2. Xem danh sách lớp" << std::endl;
        std::cout << "3. Tìm lớp" << std::endl;
        std::cout << "4. Sửa thông tin lớp" << std::endl;
        std::cout << "5. Xóa lớp" << std::endl;
        std::cout << "Chọn: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                std::string maLop, tenLop;
                std::cout << "Mã lớp: ";
                std::cin >> maLop;
                std::cin.ignore();
                std::cout << "Tên lớp: ";
                std::getline(std::cin, tenLop);
                
                Lop* lop = new Lop(maLop, tenLop);
                if (quanLyLop->them(lop)) {
                    std::cout << "✓ Thêm lớp thành công!" << std::endl;
                } else {
                    std::cout << "✗ Thêm lớp thất bại (có thể đã tồn tại)!" << std::endl;
                    delete lop;
                }
                break;
            }
            case 2: {
                DynamicArray<Lop*> danhSach = quanLyLop->danhSach();
                std::cout << "\nDanh sách lớp (" << danhSach.getSize() << " lớp):" << std::endl;
                for (size_t i = 0; i < danhSach.getSize(); i++) {
                    std::cout << "- " << danhSach.get(i)->getMaLop() << ": " << danhSach.get(i)->getTenLop() << std::endl;
                }
                break;
            }
            case 3: {
                std::string maLop;
                std::cout << "Nhập mã lớp cần tìm: ";
                std::cin >> maLop;
                
                Lop* lop = quanLyLop->tim(maLop);
                if (lop) {
                    std::cout << "✓ Tìm thấy: " << lop->getMaLop() << " - " << lop->getTenLop() << std::endl;
                } else {
                    std::cout << "✗ Không tìm thấy lớp!" << std::endl;
                }
                break;
            }
            case 4: {
                std::string maLop;
                std::cout << "Nhập mã lớp cần sửa: ";
                std::cin >> maLop;
                
                Lop* lop = quanLyLop->tim(maLop);
                if (!lop) {
                    std::cout << "✗ Không tìm thấy lớp!" << std::endl;
                    break;
                }
                
                std::cout << "Thông tin hiện tại: " << lop->getMaLop() << " - " << lop->getTenLop() << std::endl;
                std::cout << "Tên lớp mới: ";
                std::cin.ignore();
                std::string tenLopMoi;
                std::getline(std::cin, tenLopMoi);
                
                // Tạo đối tượng lớp mới với thông tin cập nhật
                Lop lopMoi(maLop, tenLopMoi);
                if (quanLyLop->sua(&lopMoi)) {
                    std::cout << "✓ Sửa thông tin lớp thành công!" << std::endl;
                } else {
                    std::cout << "✗ Sửa thông tin lớp thất bại!" << std::endl;
                }
                break;
            }
            case 5: {
                std::string maLop;
                std::cout << "Nhập mã lớp cần xóa: ";
                std::cin >> maLop;
                
                if (quanLyLop->xoa(maLop)) {
                    std::cout << "✓ Xóa lớp thành công!" << std::endl;
                } else {
                    std::cout << "✗ Xóa lớp thất bại!" << std::endl;
                }
                break;
            }
        }
    }

    // Quản lý môn học (chỉ giáo viên)
    void quanLyMonHoc() {
        std::cout << "\n=== QUẢN LÝ MÔN HỌC ===" << std::endl;
        std::cout << "1. Thêm môn học" << std::endl;
        std::cout << "2. Xem danh sách môn học" << std::endl;
        std::cout << "3. Tìm môn học" << std::endl;
        std::cout << "4. Sửa thông tin môn học" << std::endl;
        std::cout << "5. Xóa môn học" << std::endl;
        std::cout << "Chọn: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                std::string maMon, tenMon;
                std::cout << "Mã môn (tối đa 15 ký tự): ";
                std::cin >> maMon;
                std::cin.ignore();
                std::cout << "Tên môn: ";
                std::getline(std::cin, tenMon);
                
                MonHoc* mon = new MonHoc(maMon.c_str(), tenMon);
                if (quanLyMonHocManager->them(mon)) {
                    std::cout << "✓ Thêm môn học thành công!" << std::endl;
                } else {
                    std::cout << "✗ Thêm môn học thất bại (có thể đã tồn tại)!" << std::endl;
                    delete mon;
                }
                break;
            }
            case 2: {
                DynamicArray<MonHoc*> danhSach = quanLyMonHocManager->danhSach();
                std::cout << "\nDanh sách môn học (" << danhSach.getSize() << " môn):" << std::endl;
                for (size_t i = 0; i < danhSach.getSize(); i++) {
                    std::cout << "- " << danhSach.get(i)->getMaMon() << ": " << danhSach.get(i)->getTenMon() << std::endl;
                }
                break;
            }
            case 3: {
                std::string maMon;
                std::cout << "Nhập mã môn cần tìm: ";
                std::cin >> maMon;
                
                MonHoc* mon = quanLyMonHocManager->tim(maMon.c_str());
                if (mon) {
                    std::cout << "✓ Tìm thấy: " << mon->getMaMon() << " - " << mon->getTenMon() << std::endl;
                } else {
                    std::cout << "✗ Không tìm thấy môn học!" << std::endl;
                }
                break;
            }
            case 4: {
                std::string maMon;
                std::cout << "Nhập mã môn cần sửa: ";
                std::cin >> maMon;
                
                MonHoc* mon = quanLyMonHocManager->tim(maMon.c_str());
                if (!mon) {
                    std::cout << "✗ Không tìm thấy môn học!" << std::endl;
                    break;
                }
                
                std::cout << "Thông tin hiện tại: " << mon->getMaMon() << " - " << mon->getTenMon() << std::endl;
                std::cout << "Tên môn học mới: ";
                std::cin.ignore();
                std::string tenMonMoi;
                std::getline(std::cin, tenMonMoi);
                
                // Tạo đối tượng môn học mới với thông tin cập nhật
                MonHoc monMoi(maMon.c_str(), tenMonMoi);
                if (quanLyMonHocManager->sua(&monMoi)) {
                    std::cout << "✓ Sửa thông tin môn học thành công!" << std::endl;
                } else {
                    std::cout << "✗ Sửa thông tin môn học thất bại!" << std::endl;
                }
                break;
            }
            case 5: {
                std::string maMon;
                std::cout << "Nhập mã môn cần xóa: ";
                std::cin >> maMon;
                
                if (quanLyMonHocManager->xoa(maMon.c_str())) {
                    std::cout << "✓ Xóa môn học thành công!" << std::endl;
                } else {
                    std::cout << "✗ Xóa môn học thất bại!" << std::endl;
                }
                break;
            }
        }
    }

    // Quản lý sinh viên (chỉ giáo viên)
    void quanLySinhVien() {
        std::cout << "\n=== QUẢN LÝ SINH VIÊN ===" << std::endl;
        
        // Chọn lớp trước
        DynamicArray<Lop*> danhSachLop = quanLyLop->danhSach();
        if (danhSachLop.getSize() == 0) {
            std::cout << "✗ Chưa có lớp nào! Vui lòng tạo lớp trước." << std::endl;
            return;
        }
        
        std::cout << "Chọn lớp:" << std::endl;
        for (size_t i = 0; i < danhSachLop.getSize(); i++) {
            std::cout << (i+1) << ". " << danhSachLop.get(i)->getMaLop() << " - " << danhSachLop.get(i)->getTenLop() << std::endl;
        }
        std::cout << "Chọn: ";
        
        int lopChoice;
        std::cin >> lopChoice;
        
        if (lopChoice < 1 || lopChoice > (int)danhSachLop.getSize()) {
            std::cout << "✗ Lựa chọn không hợp lệ!" << std::endl;
            return;
        }
        
        Lop* lop = danhSachLop.get(lopChoice - 1);
        QuanLySinhVien* qlsv = lop->getQuanLySinhVien();
        
        std::cout << "\n1. Thêm sinh viên" << std::endl;
        std::cout << "2. Xem danh sách sinh viên" << std::endl;
        std::cout << "3. Tìm sinh viên" << std::endl;
        std::cout << "4. Sửa thông tin sinh viên" << std::endl;
        std::cout << "5. Xóa sinh viên" << std::endl;
        std::cout << "Chọn: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                std::string masv, ho, ten, password;
                bool phai;
                
                std::cout << "Mã sinh viên: ";
                std::cin >> masv;
                std::cout << "Họ: ";
                std::cin >> ho;
                std::cout << "Tên: ";
                std::cin >> ten;
                std::cout << "Giới tính (1-Nam, 0-Nữ): ";
                std::cin >> phai;
                std::cout << "Mật khẩu: ";
                std::cin >> password;
                
                SinhVien* sv = new SinhVien(masv, ho, ten, phai, password);
                if (qlsv->them(sv)) {
                    std::cout << "✓ Thêm sinh viên thành công!" << std::endl;
                } else {
                    std::cout << "✗ Thêm sinh viên thất bại (có thể đã tồn tại)!" << std::endl;
                    delete sv;
                }
                break;
            }
            case 2: {
                DynamicArray<SinhVien*> danhSach = qlsv->danhSach();
                std::cout << "\nDanh sách sinh viên lớp " << lop->getMaLop() << " (" << danhSach.getSize() << " sinh viên):" << std::endl;
                for (size_t i = 0; i < danhSach.getSize(); i++) {
                    SinhVien* sv = danhSach.get(i);
                    std::cout << "- " << sv->getMaSinhVien() << ": " << sv->getHoTen() 
                              << " (" << sv->getPhaiBangChu() << ")" << std::endl;
                }
                break;
            }
            case 3: {
                std::string masv;
                std::cout << "Nhập mã sinh viên cần tìm: ";
                std::cin >> masv;
                
                SinhVien* sv = qlsv->tim(masv);
                if (sv) {
                    std::cout << "✓ Tìm thấy: " << sv->getMaSinhVien() << " - " << sv->getHoTen() 
                              << " (" << sv->getPhaiBangChu() << ")" << std::endl;
                } else {
                    std::cout << "✗ Không tìm thấy sinh viên!" << std::endl;
                }
                break;
            }
            case 4: {
                std::string masv;
                std::cout << "Nhập mã sinh viên cần sửa: ";
                std::cin >> masv;
                
                SinhVien* sv = qlsv->tim(masv);
                if (!sv) {
                    std::cout << "✗ Không tìm thấy sinh viên!" << std::endl;
                    break;
                }
                
                std::cout << "Thông tin hiện tại: " << sv->getMaSinhVien() << " - " << sv->getHoTen() 
                          << " (" << sv->getPhaiBangChu() << ")" << std::endl;
                
                std::string ho, ten, password;
                bool phai;
                
                std::cout << "Họ mới: ";
                std::cin >> ho;
                std::cout << "Tên mới: ";
                std::cin >> ten;
                std::cout << "Giới tính mới (1-Nam, 0-Nữ): ";
                std::cin >> phai;
                std::cout << "Mật khẩu mới: ";
                std::cin >> password;
                
                // Tạo đối tượng sinh viên mới với thông tin cập nhật
                SinhVien svMoi(masv, ho, ten, phai, password);
                if (qlsv->sua(&svMoi)) {
                    std::cout << "✓ Sửa thông tin sinh viên thành công!" << std::endl;
                } else {
                    std::cout << "✗ Sửa thông tin sinh viên thất bại!" << std::endl;
                }
                break;
            }
            case 5: {
                std::string masv;
                std::cout << "Nhập mã sinh viên cần xóa: ";
                std::cin >> masv;
                
                if (qlsv->xoa(masv)) {
                    std::cout << "✓ Xóa sinh viên thành công!" << std::endl;
                } else {
                    std::cout << "✗ Xóa sinh viên thất bại!" << std::endl;
                }
                break;
            }
        }
    }

    // Quản lý câu hỏi (chỉ giáo viên)
    void quanLyCauHoi() {
        std::cout << "\n=== QUẢN LÝ CÂU HỎI ===" << std::endl;
        
        // Chọn môn học
        DynamicArray<MonHoc*> danhSachMon = quanLyMonHocManager->danhSach();
        if (danhSachMon.getSize() == 0) {
            std::cout << "✗ Chưa có môn học nào! Vui lòng tạo môn học trước." << std::endl;
            return;
        }
        
        std::cout << "Chọn môn học:" << std::endl;
        for (size_t i = 0; i < danhSachMon.getSize(); i++) {
            std::cout << (i+1) << ". " << danhSachMon.get(i)->getMaMon() << " - " << danhSachMon.get(i)->getTenMon() << std::endl;
        }
        std::cout << "Chọn: ";
        
        int monChoice;
        std::cin >> monChoice;
        
        if (monChoice < 1 || monChoice > (int)danhSachMon.getSize()) {
            std::cout << "✗ Lựa chọn không hợp lệ!" << std::endl;
            return;
        }
        
        MonHoc* mon = danhSachMon.get(monChoice - 1);
        QuanLyCauHoi* qlch = mon->getQuanLyCauHoi();
        
        std::cout << "\n1. Thêm câu hỏi" << std::endl;
        std::cout << "2. Xem danh sách câu hỏi" << std::endl;
        std::cout << "3. Lấy câu hỏi ngẫu nhiên" << std::endl;
        std::cout << "4. Sửa câu hỏi" << std::endl;
        std::cout << "5. Xóa câu hỏi" << std::endl;
        std::cout << "Chọn: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                std::string noiDung, luaChonA, luaChonB, luaChonC, luaChonD;
                char dapAn;
                
                std::cin.ignore();
                std::cout << "Nội dung câu hỏi: ";
                std::getline(std::cin, noiDung);
                std::cout << "Lựa chọn A: ";
                std::getline(std::cin, luaChonA);
                std::cout << "Lựa chọn B: ";
                std::getline(std::cin, luaChonB);
                std::cout << "Lựa chọn C: ";
                std::getline(std::cin, luaChonC);
                std::cout << "Lựa chọn D: ";
                std::getline(std::cin, luaChonD);
                std::cout << "Đáp án đúng (A/B/C/D): ";
                std::cin >> dapAn;
                
                // Tạo ID ngẫu nhiên theo yêu cầu README.md
                int id = qlch->taoMaCauHoiNgauNhien();
                
                CauHoi* ch = new CauHoi(id, noiDung, luaChonA, luaChonB, luaChonC, luaChonD, dapAn);
                if (qlch->them(ch)) {
                    std::cout << "✓ Thêm câu hỏi thành công! ID: " << id << std::endl;
                } else {
                    std::cout << "✗ Thêm câu hỏi thất bại!" << std::endl;
                    delete ch;
                }
                break;
            }
            case 2: {
                DynamicArray<CauHoi*> danhSach = qlch->danhSach();
                std::cout << "\nDanh sách câu hỏi môn " << mon->getTenMon() << " (" << danhSach.getSize() << " câu):" << std::endl;
                for (size_t i = 0; i < danhSach.getSize(); i++) {
                    CauHoi* ch = danhSach.get(i);
                    std::cout << "ID " << ch->getMaCauHoi() << ": " << ch->getNoiDung() << std::endl;
                    std::cout << "  A. " << ch->getLuaChonA() << std::endl;
                    std::cout << "  B. " << ch->getLuaChonB() << std::endl;
                    std::cout << "  C. " << ch->getLuaChonC() << std::endl;
                    std::cout << "  D. " << ch->getLuaChonD() << std::endl;
                    std::cout << "  Đáp án: " << ch->getDapAnDung() << std::endl << std::endl;
                }
                break;
            }
            case 3: {
                int soLuong;
                std::cout << "Số lượng câu hỏi ngẫu nhiên: ";
                std::cin >> soLuong;
                
                DynamicArray<CauHoi*> cauHoiNgauNhien = qlch->layNgauNhien(soLuong);
                std::cout << "\nCâu hỏi ngẫu nhiên (" << cauHoiNgauNhien.getSize() << " câu):" << std::endl;
                for (size_t i = 0; i < cauHoiNgauNhien.getSize(); i++) {
                    CauHoi* ch = cauHoiNgauNhien.get(i);
                    std::cout << (i+1) << ". " << ch->getNoiDung() << " (ID: " << ch->getMaCauHoi() << ")" << std::endl;
                }
                break;
            }
            case 4: {
                int maCauHoi;
                std::cout << "Nhập ID câu hỏi cần sửa: ";
                std::cin >> maCauHoi;
                
                CauHoi* ch = qlch->tim(maCauHoi);
                if (!ch) {
                    std::cout << "✗ Không tìm thấy câu hỏi!" << std::endl;
                    break;
                }
                
                std::cout << "Thông tin hiện tại:" << std::endl;
                std::cout << "ID: " << ch->getMaCauHoi() << std::endl;
                std::cout << "Nội dung: " << ch->getNoiDung() << std::endl;
                std::cout << "A. " << ch->getLuaChonA() << std::endl;
                std::cout << "B. " << ch->getLuaChonB() << std::endl;
                std::cout << "C. " << ch->getLuaChonC() << std::endl;
                std::cout << "D. " << ch->getLuaChonD() << std::endl;
                std::cout << "Đáp án: " << ch->getDapAnDung() << std::endl;
                
                std::string noiDung, luaChonA, luaChonB, luaChonC, luaChonD;
                char dapAn;
                
                std::cin.ignore();
                std::cout << "\nNhập thông tin mới:" << std::endl;
                std::cout << "Nội dung câu hỏi: ";
                std::getline(std::cin, noiDung);
                std::cout << "Lựa chọn A: ";
                std::getline(std::cin, luaChonA);
                std::cout << "Lựa chọn B: ";
                std::getline(std::cin, luaChonB);
                std::cout << "Lựa chọn C: ";
                std::getline(std::cin, luaChonC);
                std::cout << "Lựa chọn D: ";
                std::getline(std::cin, luaChonD);
                std::cout << "Đáp án đúng (A/B/C/D): ";
                std::cin >> dapAn;
                
                // Tạo đối tượng câu hỏi mới với thông tin cập nhật
                CauHoi chMoi(maCauHoi, noiDung, luaChonA, luaChonB, luaChonC, luaChonD, dapAn);
                if (qlch->sua(&chMoi)) {
                    std::cout << "✓ Sửa câu hỏi thành công!" << std::endl;
                } else {
                    std::cout << "✗ Sửa câu hỏi thất bại!" << std::endl;
                }
                break;
            }
            case 5: {
                int maCauHoi;
                std::cout << "Nhập ID câu hỏi cần xóa: ";
                std::cin >> maCauHoi;
                
                if (qlch->xoa(maCauHoi)) {
                    std::cout << "✓ Xóa câu hỏi thành công!" << std::endl;
                } else {
                    std::cout << "✗ Xóa câu hỏi thất bại!" << std::endl;
                }
                break;
            }
        }
    }

    // Quản lý điểm thi (chỉ giáo viên)
    void quanLyDiemThi() {
        std::cout << "\n=== QUẢN LÝ ĐIỂM THI ===" << std::endl;
        
        // Chọn sinh viên trước
        std::string masv;
        std::cout << "Nhập mã sinh viên: ";
        std::cin >> masv;
        
        SinhVien* sv = findStudent(masv);
        if (!sv) {
            std::cout << "✗ Không tìm thấy sinh viên!" << std::endl;
            return;
        }
        
        std::cout << "Sinh viên: " << sv->getHoTen() << " (" << sv->getMaSinhVien() << ")" << std::endl;
        QuanLyDiem* qld = sv->getDanhSachDiem();
        
        std::cout << "\n1. Thêm điểm thi" << std::endl;
        std::cout << "2. Xem danh sách điểm" << std::endl;
        std::cout << "3. Tìm điểm theo môn" << std::endl;
        std::cout << "4. Sửa điểm thi" << std::endl;
        std::cout << "5. Xóa điểm thi" << std::endl;
        std::cout << "6. Xem thống kê điểm" << std::endl;
        std::cout << "Chọn: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                // Chọn môn học
                DynamicArray<MonHoc*> danhSachMon = quanLyMonHocManager->danhSach();
                if (danhSachMon.getSize() == 0) {
                    std::cout << "✗ Chưa có môn học nào!" << std::endl;
                    break;
                }
                
                std::cout << "Chọn môn học:" << std::endl;
                for (size_t i = 0; i < danhSachMon.getSize(); i++) {
                    std::cout << (i+1) << ". " << danhSachMon.get(i)->getMaMon() << " - " << danhSachMon.get(i)->getTenMon() << std::endl;
                }
                std::cout << "Chọn: ";
                
                int monChoice;
                std::cin >> monChoice;
                
                if (monChoice < 1 || monChoice > (int)danhSachMon.getSize()) {
                    std::cout << "✗ Lựa chọn không hợp lệ!" << std::endl;
                    break;
                }
                
                MonHoc* mon = danhSachMon.get(monChoice - 1);
                
                double diem;
                std::string chiTiet;
                
                std::cout << "Điểm (0-10): ";
                std::cin >> diem;
                std::cin.ignore();
                std::cout << "Chi tiết bài thi: ";
                std::getline(std::cin, chiTiet);
                
                DiemThi* dt = new DiemThi(mon->getMaMon(), diem, chiTiet);
                if (qld->them(dt)) {
                    std::cout << "✓ Thêm điểm thi thành công!" << std::endl;
                } else {
                    std::cout << "✗ Thêm điểm thi thất bại (có thể đã có điểm môn này)!" << std::endl;
                    delete dt;
                }
                break;
            }
            case 2: {
                DynamicArray<DiemThi*> danhSach = qld->danhSach();
                std::cout << "\nDanh sách điểm của " << sv->getHoTen() << " (" << danhSach.getSize() << " môn):" << std::endl;
                for (size_t i = 0; i < danhSach.getSize(); i++) {
                    DiemThi* dt = danhSach.get(i);
                    MonHoc* mon = quanLyMonHocManager->tim(dt->getMaMonHoc().c_str());
                    std::string tenMon = mon ? mon->getTenMon() : "Không xác định";
                    std::cout << "- " << dt->getMaMonHoc() << " (" << tenMon << "): " 
                              << std::fixed << std::setprecision(1) << dt->getDiem() 
                              << " - " << dt->getChiTietBaiThi() << std::endl;
                }
                break;
            }
            case 3: {
                std::string maMon;
                std::cout << "Nhập mã môn cần tìm: ";
                std::cin >> maMon;
                
                DiemThi* dt = qld->tim(maMon);
                if (dt) {
                    MonHoc* mon = quanLyMonHocManager->tim(maMon.c_str());
                    std::string tenMon = mon ? mon->getTenMon() : "Không xác định";
                    std::cout << "✓ Tìm thấy: " << dt->getMaMonHoc() << " (" << tenMon << "): " 
                              << std::fixed << std::setprecision(1) << dt->getDiem() 
                              << " - " << dt->getChiTietBaiThi() << std::endl;
                } else {
                    std::cout << "✗ Không tìm thấy điểm môn học này!" << std::endl;
                }
                break;
            }
            case 4: {
                std::string maMon;
                std::cout << "Nhập mã môn cần sửa điểm: ";
                std::cin >> maMon;
                
                DiemThi* dt = qld->tim(maMon);
                if (!dt) {
                    std::cout << "✗ Không tìm thấy điểm môn học này!" << std::endl;
                    break;
                }
                
                MonHoc* mon = quanLyMonHocManager->tim(maMon.c_str());
                std::string tenMon = mon ? mon->getTenMon() : "Không xác định";
                std::cout << "Thông tin hiện tại: " << dt->getMaMonHoc() << " (" << tenMon << "): " 
                          << std::fixed << std::setprecision(1) << dt->getDiem() 
                          << " - " << dt->getChiTietBaiThi() << std::endl;
                
                double diemMoi;
                std::string chiTietMoi;
                
                std::cout << "Điểm mới (0-10): ";
                std::cin >> diemMoi;
                std::cin.ignore();
                std::cout << "Chi tiết bài thi mới: ";
                std::getline(std::cin, chiTietMoi);
                
                // Tạo đối tượng điểm thi mới với thông tin cập nhật
                DiemThi dtMoi(maMon, diemMoi, chiTietMoi);
                if (qld->sua(&dtMoi)) {
                    std::cout << "✓ Sửa điểm thi thành công!" << std::endl;
                } else {
                    std::cout << "✗ Sửa điểm thi thất bại!" << std::endl;
                }
                break;
            }
            case 5: {
                std::string maMon;
                std::cout << "Nhập mã môn cần xóa điểm: ";
                std::cin >> maMon;
                
                if (qld->xoa(maMon)) {
                    std::cout << "✓ Xóa điểm thi thành công!" << std::endl;
                } else {
                    std::cout << "✗ Xóa điểm thi thất bại!" << std::endl;
                }
                break;
            }
            case 6: {
                std::cout << "\n=== THỐNG KÊ ĐIỂM ===" << std::endl;
                std::cout << "Số môn đã thi: " << qld->demSoMonDaThi() << std::endl;
                std::cout << "Số môn đậu: " << qld->demSoMonDau() << std::endl;
                std::cout << "Số môn rớt: " << qld->demSoMonRot() << std::endl;
                std::cout << "Điểm trung bình: " << std::fixed << std::setprecision(2) << qld->tinhDiemTrungBinh() << std::endl;
                break;
            }
        }
    }

    // Làm bài thi đơn giản (sinh viên)
    void lamBaiThi() {
        std::cout << "\n=== LÀM BÀI THI ===" << std::endl;
        
        // Chọn môn học
        DynamicArray<MonHoc*> danhSachMon = quanLyMonHocManager->danhSach();
        if (danhSachMon.getSize() == 0) {
            std::cout << "✗ Chưa có môn học nào!" << std::endl;
            return;
        }
        
        std::cout << "Chọn môn thi:" << std::endl;
        for (size_t i = 0; i < danhSachMon.getSize(); i++) {
            std::cout << (i+1) << ". " << danhSachMon.get(i)->getTenMon() << std::endl;
        }
        std::cout << "Chọn: ";
        
        int monChoice;
        std::cin >> monChoice;
        
        if (monChoice < 1 || monChoice > (int)danhSachMon.getSize()) {
            std::cout << "✗ Lựa chọn không hợp lệ!" << std::endl;
            return;
        }
        
        MonHoc* mon = danhSachMon.get(monChoice - 1);
        QuanLyCauHoi* qlch = mon->getQuanLyCauHoi();
        
        // Lấy câu hỏi ngẫu nhiên
        int soCau = 3; // Đơn giản hóa: 3 câu
        DynamicArray<CauHoi*> cauHoiThi = qlch->layNgauNhien(soCau);
        
        if (cauHoiThi.getSize() == 0) {
            std::cout << "✗ Môn này chưa có câu hỏi!" << std::endl;
            return;
        }
        
        std::cout << "\n=== BÀI THI MÔN " << mon->getTenMon() << " ===" << std::endl;
        std::cout << "Số câu hỏi: " << cauHoiThi.getSize() << std::endl;
        
        std::string chiTietBaiThi = "";
        int diem = 0;
        
        for (size_t i = 0; i < cauHoiThi.getSize(); i++) {
            CauHoi* ch = cauHoiThi.get(i);
            std::cout << "\nCâu " << (i+1) << ": " << ch->getNoiDung() << std::endl;
            std::cout << "A. " << ch->getLuaChonA() << std::endl;
            std::cout << "B. " << ch->getLuaChonB() << std::endl;
            std::cout << "C. " << ch->getLuaChonC() << std::endl;
            std::cout << "D. " << ch->getLuaChonD() << std::endl;
            std::cout << "Chọn đáp án: ";
            
            char traLoi;
            std::cin >> traLoi;
            traLoi = toupper(traLoi);
            
            if (i > 0) chiTietBaiThi += ",";
            chiTietBaiThi += traLoi;
            
            if (traLoi == ch->getDapAnDung()) {
                diem++;
            }
        }
        
        // Tính điểm (thang 10)
        float diemCuoi = (float)diem * 10.0f / cauHoiThi.getSize();
        
        std::cout << "\n=== KẾT QUẢ THI ===" << std::endl;
        std::cout << "Số câu đúng: " << diem << "/" << cauHoiThi.getSize() << std::endl;
        std::cout << "Điểm: " << std::fixed << std::setprecision(1) << diemCuoi << std::endl;
        std::cout << "Chi tiết: " << chiTietBaiThi << std::endl;
        
        // Lưu điểm vào hệ thống
        // Tìm sinh viên hiện tại
        SinhVien* sv = nullptr;
        DynamicArray<Lop*> danhSachLop = quanLyLop->danhSach();
        
        for (size_t i = 0; i < danhSachLop.getSize() && !sv; i++) {
            QuanLySinhVien* qlsv = danhSachLop.get(i)->getQuanLySinhVien();
            DynamicArray<SinhVien*> danhSachSV = qlsv->danhSach();
            
            for (size_t j = 0; j < danhSachSV.getSize(); j++) {
                if (danhSachSV.get(j)->getHoTen() == currentUser) {
                    sv = danhSachSV.get(j);
                    break;
                }
            }
        }
        
        if (sv) {
            QuanLyDiem* qld = sv->getDanhSachDiem();
            
            // Kiểm tra xem đã có điểm môn này chưa
            DiemThi* diemCu = qld->tim(mon->getMaMon());
            if (diemCu) {
                // Cập nhật điểm cũ
                DiemThi diemMoi(mon->getMaMon(), diemCuoi, chiTietBaiThi);
                if (qld->sua(&diemMoi)) {
                    std::cout << "\n✓ Điểm đã được cập nhật vào hệ thống!" << std::endl;
                } else {
                    std::cout << "\n✗ Lỗi khi cập nhật điểm!" << std::endl;
                }
            } else {
                // Thêm điểm mới
                DiemThi* diemMoi = new DiemThi(mon->getMaMon(), diemCuoi, chiTietBaiThi);
                if (qld->them(diemMoi)) {
                    std::cout << "\n✓ Điểm đã được lưu vào hệ thống!" << std::endl;
                } else {
                    std::cout << "\n✗ Lỗi khi lưu điểm!" << std::endl;
                    delete diemMoi;
                }
            }
        } else {
            std::cout << "\n✗ Lỗi: Không tìm thấy thông tin sinh viên!" << std::endl;
        }
    }

    // Xem điểm của sinh viên hiện tại
    void xemDiemCuaToi() {
        std::cout << "\n=== ĐIỂM CỦA TÔI ===" << std::endl;
        
        // Tìm sinh viên hiện tại dựa trên currentUser
        SinhVien* sv = nullptr;
        DynamicArray<Lop*> danhSachLop = quanLyLop->danhSach();
        
        for (size_t i = 0; i < danhSachLop.getSize() && !sv; i++) {
            QuanLySinhVien* qlsv = danhSachLop.get(i)->getQuanLySinhVien();
            DynamicArray<SinhVien*> danhSachSV = qlsv->danhSach();
            
            for (size_t j = 0; j < danhSachSV.getSize(); j++) {
                if (danhSachSV.get(j)->getHoTen() == currentUser) {
                    sv = danhSachSV.get(j);
                    break;
                }
            }
        }
        
        if (!sv) {
            std::cout << "✗ Không tìm thấy thông tin sinh viên!" << std::endl;
            return;
        }
        
        std::cout << "Sinh viên: " << sv->getHoTen() << " (" << sv->getMaSinhVien() << ")" << std::endl;
        QuanLyDiem* qld = sv->getDanhSachDiem();
        
        std::cout << "\n1. Xem tất cả điểm" << std::endl;
        std::cout << "2. Tìm điểm theo môn" << std::endl;
        std::cout << "3. Xem thống kê điểm" << std::endl;
        std::cout << "Chọn: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                DynamicArray<DiemThi*> danhSach = qld->danhSach();
                if (danhSach.getSize() == 0) {
                    std::cout << "\n✗ Bạn chưa có điểm nào!" << std::endl;
                    break;
                }
                
                std::cout << "\nDanh sách điểm của bạn (" << danhSach.getSize() << " môn):" << std::endl;
                std::cout << std::string(60, '-') << std::endl;
                std::cout << std::left << std::setw(15) << "Mã môn" 
                          << std::setw(25) << "Tên môn" 
                          << std::setw(8) << "Điểm" 
                          << "Chi tiết" << std::endl;
                std::cout << std::string(60, '-') << std::endl;
                
                for (size_t i = 0; i < danhSach.getSize(); i++) {
                    DiemThi* dt = danhSach.get(i);
                    MonHoc* mon = quanLyMonHocManager->tim(dt->getMaMonHoc().c_str());
                    std::string tenMon = mon ? mon->getTenMon() : "Không xác định";
                    
                    std::cout << std::left << std::setw(15) << dt->getMaMonHoc()
                              << std::setw(25) << tenMon
                              << std::setw(8) << std::fixed << std::setprecision(1) << dt->getDiem()
                              << dt->getChiTietBaiThi() << std::endl;
                }
                std::cout << std::string(60, '-') << std::endl;
                break;
            }
            case 2: {
                std::string maMon;
                std::cout << "Nhập mã môn cần xem: ";
                std::cin >> maMon;
                
                DiemThi* dt = qld->tim(maMon);
                if (dt) {
                    MonHoc* mon = quanLyMonHocManager->tim(maMon.c_str());
                    std::string tenMon = mon ? mon->getTenMon() : "Không xác định";
                    
                    std::cout << "\n=== ĐIỂM MÔN " << tenMon << " ===" << std::endl;
                    std::cout << "Mã môn: " << dt->getMaMonHoc() << std::endl;
                    std::cout << "Điểm: " << std::fixed << std::setprecision(1) << dt->getDiem() << std::endl;
                    std::cout << "Chi tiết bài thi: " << dt->getChiTietBaiThi() << std::endl;
                    
                    if (dt->getDiem() >= 5.0) {
                        std::cout << "Kết quả: ✓ ĐẬU" << std::endl;
                    } else {
                        std::cout << "Kết quả: ✗ RỚT" << std::endl;
                    }
                } else {
                    std::cout << "✗ Bạn chưa có điểm môn học này!" << std::endl;
                }
                break;
            }
            case 3: {
                std::cout << "\n=== THỐNG KÊ ĐIỂM CỦA BẠN ===" << std::endl;
                std::cout << "Số môn đã thi: " << qld->demSoMonDaThi() << std::endl;
                std::cout << "Số môn đậu: " << qld->demSoMonDau() << std::endl;
                std::cout << "Số môn rớt: " << qld->demSoMonRot() << std::endl;
                std::cout << "Điểm trung bình: " << std::fixed << std::setprecision(2) << qld->tinhDiemTrungBinh() << std::endl;
                
                double dtb = qld->tinhDiemTrungBinh();
                std::cout << "Xếp loại: ";
                if (dtb >= 8.5) {
                    std::cout << "Xuất sắc" << std::endl;
                } else if (dtb >= 7.0) {
                    std::cout << "Khá" << std::endl;
                } else if (dtb >= 5.5) {
                    std::cout << "Trung bình" << std::endl;
                } else if (dtb >= 4.0) {
                    std::cout << "Yếu" << std::endl;
                } else {
                    std::cout << "Kém" << std::endl;
                }
                break;
            }
        }
    }

    // Chạy hệ thống
    void run() {
        std::cout << "=== HỆ THỐNG QUẢN LÝ THI TRẮC NGHIỆM ===" << std::endl;
        std::cout << "Theo yêu cầu README.md - Sử dụng cấu trúc dữ liệu từ /include" << std::endl;
        
        // Đăng nhập
        if (!login()) {
            std::cout << "Đăng nhập thất bại. Thoát chương trình." << std::endl;
            return;
        }
        
        int choice;
        do {
            showMainMenu();
            std::cin >> choice;
            
            if (isTeacher) {
                switch (choice) {
                    case 1: quanLyLopHoc(); break;
                    case 2: quanLyMonHoc(); break;
                    case 3: quanLySinhVien(); break;
                    case 4: quanLyCauHoi(); break;
                    case 5: quanLyDiemThi(); break;
                    case 6: std::cout << "Chức năng báo cáo đang phát triển..." << std::endl; break;
                    case 7: std::cout << "Chức năng thiết lập bài thi đang phát triển..." << std::endl; break;
                    case 0: std::cout << "Đăng xuất..." << std::endl; break;
                    default: std::cout << "Lựa chọn không hợp lệ!" << std::endl; break;
                }
            } else {
                switch (choice) {
                    case 1: lamBaiThi(); break;
                    case 2: xemDiemCuaToi(); break;
                    case 0: std::cout << "Đăng xuất..." << std::endl; break;
                    default: std::cout << "Lựa chọn không hợp lệ!" << std::endl; break;
                }
            }
            
            if (choice != 0) {
                std::cout << "\nNhấn Enter để tiếp tục...";
                std::cin.ignore();
                std::cin.get();
            }
            
        } while (choice != 0);
    }
};

int main() {
    SimpleExamSystem system;
    system.run();
    return 0;
}