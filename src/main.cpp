#include "define.h"
#include "cauhoi.h"
#include "diemthi.h"
#include "lop.h"
#include "monhoc.h"
#include "sinhvien.h"

// Hàm hiển thị menu chính
void hienThiMenuChinh() {
    xoaManHinh();
    inTieuDe("HỆ THỐNG THI TRẮC NGHIỆM");
    cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                                    MENU CHÍNH                                        ║" << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║  1. Đăng nhập Giáo viên                                                              ║" << endl;
    cout << "║  2. Đăng nhập Sinh viên                                                              ║" << endl;
    cout << "║  3. Thoát chương trình                                                               ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════════════════════╝" << endl;
    cout << "Nhập lựa chọn của bạn: ";
}

// Hàm hiển thị menu giáo viên
void hienThiMenuGiaoVien() {
    xoaManHinh();
    inTieuDe("MENU GIÁO VIÊN");
    cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                                   MENU GIÁO VIÊN                                     ║" << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║  1. Quản lý lớp học                                                                  ║" << endl;
    cout << "║  2. Quản lý sinh viên                                                                ║" << endl;
    cout << "║  3. Quản lý môn học                                                                  ║" << endl;
    cout << "║  4. Quản lý câu hỏi                                                                  ║" << endl;
    cout << "║  5. Xem báo cáo điểm                                                                 ║" << endl;
    cout << "║  6. Thống kê                                                                         ║" << endl;
    cout << "║  0. Quay lại menu chính                                                              ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════════════════════╝" << endl;
    cout << "Nhập lựa chọn của bạn: ";
}

// Hàm hiển thị menu sinh viên
void hienThiMenuSinhVien() {
    xoaManHinh();
    inTieuDe("MENU SINH VIÊN");
    cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                                   MENU SINH VIÊN                                     ║" << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║  1. Làm bài thi                                                                      ║" << endl;
    cout << "║  2. Xem điểm thi                                                                     ║" << endl;
    cout << "║  3. Xem chi tiết bài thi                                                             ║" << endl;
    cout << "║  0. Đăng xuất                                                                        ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════════════════════╝" << endl;
    cout << "Nhập lựa chọn của bạn: ";
}

// Hàm đăng nhập giáo viên
bool dangNhapGiaoVien() {
    string taiKhoan, matKhau;
    cout << "Nhập tài khoản giáo viên: ";
    cin >> taiKhoan;
    cout << "Nhập mật khẩu: ";
    cin >> matKhau;
    
    // Mật khẩu mặc định cho giáo viên
    if (taiKhoan == "admin" && matKhau == "123456") {
        cout << "Đăng nhập thành công!" << endl;
        dungManHinh();
        return true;
    } else {
        cout << "Tài khoản hoặc mật khẩu không đúng!" << endl;
        dungManHinh();
        return false;
    }
}



// Hàm quản lý lớp học
void quanLyLop() {
    int luaChon;
    do {
        xoaManHinh();
        inTieuDe("QUẢN LÝ LỚP HỌC");
        cout << "1. Thêm lớp" << endl;
        cout << "2. Hiển thị danh sách lớp" << endl;
        cout << "3. Xóa lớp" << endl;
        cout << "0. Quay lại" << endl;
        cout << "Nhập lựa chọn: ";
        cin >> luaChon;
        
        switch (luaChon) {
            case 1: {
                string maLop, tenLop;
                cout << "Nhập mã lớp: ";
                cin >> maLop;
                cin.ignore();
                cout << "Nhập tên lớp: ";
                getline(cin, tenLop);
                themLop(maLop, tenLop);
                dungManHinh();
                break;
            }
            case 2:
                hienThiDanhSachLop();
                dungManHinh();
                break;
            case 3: {
                string maLop;
                cout << "Nhập mã lớp cần xóa: ";
                cin >> maLop;
                xoaLop(maLop);
                dungManHinh();
                break;
            }
        }
    } while (luaChon != 0);
}

// Hàm quản lý môn học
void quanLyMonHoc() {
    int luaChon;
    do {
        xoaManHinh();
        inTieuDe("QUẢN LÝ MÔN HỌC");
        cout << "1. Thêm môn học" << endl;
        cout << "2. Hiển thị danh sách môn học" << endl;
        cout << "3. Xóa môn học" << endl;
        cout << "0. Quay lại" << endl;
        cout << "Nhập lựa chọn: ";
        cin >> luaChon;
        
        switch (luaChon) {
            case 1: {
                string maMon, tenMon;
                cout << "Nhập mã môn: ";
                cin >> maMon;
                cin.ignore();
                cout << "Nhập tên môn: ";
                getline(cin, tenMon);
                themMonHoc(maMon, tenMon);
                dungManHinh();
                break;
            }
            case 2:
                hienThiDanhSachMonHoc();
                dungManHinh();
                break;
            case 3: {
                string maMon;
                cout << "Nhập mã môn cần xóa: ";
                cin >> maMon;
                xoaMonHoc(maMon);
                dungManHinh();
                break;
            }
        }
    } while (luaChon != 0);
}

// Hàm quản lý câu hỏi
void quanLyCauHoi() {
    int luaChon;
    do {
        xoaManHinh();
        inTieuDe("QUẢN LÝ CÂU HỎI");
        cout << "1. Thêm câu hỏi" << endl;
        cout << "2. Hiển thị câu hỏi theo môn" << endl;
        cout << "3. Xóa câu hỏi" << endl;
        cout << "0. Quay lại" << endl;
        cout << "Nhập lựa chọn: ";
        cin >> luaChon;
        
        switch (luaChon) {
            case 1: {
                string maMon;
                cout << "Nhập mã môn: ";
                cin >> maMon;
                MonHoc* mon = timMonHoc(maMon);
                if (mon != nullptr) {
                    nhapCauHoi(mon->gocCauHoi);
                } else {
                    cout << "Không tìm thấy môn học!" << endl;
                }
                dungManHinh();
                break;
            }
            case 2: {
                string maMon;
                cout << "Nhập mã môn: ";
                cin >> maMon;
                MonHoc* mon = timMonHoc(maMon);
                if (mon != nullptr) {
                    hienThiTatCaCauHoi(mon->gocCauHoi);
                } else {
                    cout << "Không tìm thấy môn học!" << endl;
                }
                dungManHinh();
                break;
            }
            case 3: {
                string maMon;
                int idCauHoi;
                cout << "Nhập mã môn: ";
                cin >> maMon;
                cout << "Nhập ID câu hỏi cần xóa: ";
                cin >> idCauHoi;
                MonHoc* mon = timMonHoc(maMon);
                if (mon != nullptr) {
                    mon->gocCauHoi = xoaCauHoi(mon->gocCauHoi, idCauHoi);
                    cout << "Đã xóa câu hỏi ID " << idCauHoi << endl;
                } else {
                    cout << "Không tìm thấy môn học!" << endl;
                }
                dungManHinh();
                break;
            }
        }
    } while (luaChon != 0);
}



// Hàm xem báo cáo điểm
void xemBaoCaoDiem() {
    int luaChon;
    do {
        xoaManHinh();
        inTieuDe("BÁO CÁO ĐIỂM");
        cout << "1. Báo cáo điểm theo lớp và môn" << endl;
        cout << "2. Báo cáo tổng kết theo lớp" << endl;
        cout << "3. Danh sách điểm của một sinh viên" << endl;
        cout << "0. Quay lại" << endl;
        cout << "Nhập lựa chọn: ";
        cin >> luaChon;
        
        switch (luaChon) {
            case 1: {
                string maLop, maMon;
                cout << "Nhập mã lớp: ";
                cin >> maLop;
                cout << "Nhập mã môn: ";
                cin >> maMon;
                inBangDiemTheoLop(maLop, maMon);
                dungManHinh();
                break;
            }
            case 2: {
                string maLop;
                cout << "Nhập mã lớp: ";
                cin >> maLop;
                inBangDiemTongKet(maLop);
                dungManHinh();
                break;
            }
            case 3: {
                string maSV;
                cout << "Nhập mã sinh viên: ";
                cin >> maSV;
                
                bool timThay = false;
                for (int i = 0; i < soLuongLop; i++) {
                    SinhVien* sv = timSinhVien(danhSachLop[i].danhSachSV, maSV);
                    if (sv != nullptr) {
                        cout << "Bảng điểm của sinh viên " << sv->ho << " " << sv->ten << " (" << maSV << "):" << endl;
                        hienThiDanhSachDiem(sv->danhSachDiem);
                        timThay = true;
                        break;
                    }
                }
                if (!timThay) {
                    cout << "Không tìm thấy sinh viên!" << endl;
                }
                dungManHinh();
                break;
            }
        }
    } while (luaChon != 0);
}

// Hàm xem thống kê
void xemThongKe() {
    int luaChon;
    do {
        xoaManHinh();
        inTieuDe("THỐNG KÊ");
        cout << "1. Thống kê theo môn học" << endl;
        cout << "2. Thống kê tổng quát hệ thống" << endl;
        cout << "3. Thống kê theo lớp" << endl;
        cout << "0. Quay lại" << endl;
        cout << "Nhập lựa chọn: ";
        cin >> luaChon;
        
        switch (luaChon) {
            case 1: {
                string maMon;
                cout << "Nhập mã môn: ";
                cin >> maMon;
                thongKeDiemTheoMon(maMon);
                dungManHinh();
                break;
            }
            case 2: {
                cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗" << endl;
                cout << "║                            THỐNG KÊ TỔNG QUÁT HỆ THỐNG                            ║" << endl;
                cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣" << endl;
                
                // Thống kê số lượng
                int tongSinhVien = 0;
                for (int i = 0; i < soLuongLop; i++) {
                    tongSinhVien += demSoSinhVienTrongLop(danhSachLop[i].maLop);
                }
                
                int tongMonHoc = 0;
                MonHoc* temp = danhSachMonHoc;
                while (temp != nullptr) {
                    tongMonHoc++;
                    temp = temp->tiep;
                }
                
                int tongCauHoi = 0;
                temp = danhSachMonHoc;
                while (temp != nullptr) {
                    tongCauHoi += demSoCauHoi(temp->gocCauHoi);
                    temp = temp->tiep;
                }
                
                cout << "║ Số lớp học: " << setw(10) << soLuongLop << "                                                    ║" << endl;
                cout << "║ Số sinh viên: " << setw(10) << tongSinhVien << "                                                   ║" << endl;
                cout << "║ Số môn học: " << setw(10) << tongMonHoc << "                                                    ║" << endl;
                cout << "║ Tổng số câu hỏi: " << setw(10) << tongCauHoi << "                                               ║" << endl;
                cout << "╚══════════════════════════════════════════════════════════════════════════════════════╝" << endl;
                dungManHinh();
                break;
            }
            case 3: {
                string maLop;
                cout << "Nhập mã lớp: ";
                cin >> maLop;
                
                int viTriLop = timLop(maLop);
                if (viTriLop != -1) {
                    int soSV = demSoSinhVienTrongLop(maLop);
                    cout << "Thống kê lớp " << maLop << " - " << danhSachLop[viTriLop].tenLop << ":" << endl;
                    cout << "Số sinh viên: " << soSV << endl;
                    
                    // Đếm số bài thi đã làm
                    int tongBaiThi = 0;
                    SinhVien* sv = danhSachLop[viTriLop].danhSachSV;
                    while (sv != nullptr) {
                        DiemThi* diem = sv->danhSachDiem;
                        while (diem != nullptr) {
                            tongBaiThi++;
                            diem = diem->tiep;
                        }
                        sv = sv->tiep;
                    }
                    cout << "Tổng số bài thi đã làm: " << tongBaiThi << endl;
                } else {
                    cout << "Không tìm thấy lớp!" << endl;
                }
                dungManHinh();
                break;
            }
        }
    } while (luaChon != 0);
}



int main() {
    // Khởi tạo dữ liệu
    khoiTaoLop();
    khoiTaoMonHoc();
    khoiTaoSinhVien();
    
    int luaChon;
    do {
        hienThiMenuChinh();
        cin >> luaChon;
        
        switch (luaChon) {
            case 1: {
                if (dangNhapGiaoVien()) {
                    int luaChonGV;
                    do {
                        hienThiMenuGiaoVien();
                        cin >> luaChonGV;
                        
                        switch (luaChonGV) {
                            case 1:
                                quanLyLop();
                                break;
                            case 2:
                                quanLySinhVien();
                                break;
                            case 3:
                                quanLyMonHoc();
                                break;
                            case 4:
                                quanLyCauHoi();
                                break;
                            case 5:
                                xemBaoCaoDiem();
                                break;
                            case 6:
                                xemThongKe();
                                break;
                        }
                    } while (luaChonGV != 0);
                }
                break;
            }
            case 2: {
                SinhVien* sv = dangNhapSinhVien();
                if (sv != nullptr) {
                    int luaChonSV;
                    do {
                        hienThiMenuSinhVien();
                        cin >> luaChonSV;
                        
                        switch (luaChonSV) {
                            case 1:
                                lamBaiThi(sv);
                                break;
                            case 2:
                                xemKetQuaThi(sv);
                                break;
                            case 3:
                                xemChiTietBaiThi(sv);
                                break;
                        }
                    } while (luaChonSV != 0);
                }
                break;
            }
            case 3:
                thoat();
                break;
            default:
                cout << "Lựa chọn không hợp lệ!" << endl;
                dungManHinh();
        }
    } while (luaChon != 3);
    
    return 0;
}
