#include "define.h"
#include <conio.h>
#include <windows.h>

// Biến toàn cục
Lop danhSachLop[MAX_LOP];
int soLuongLop = 0;
bool hetGio = false;
int thoiGianConLai = THOI_GIAN_THI;

// Hàm tiện ích
void xuongDong() {
    cout << endl;
}

void thoat() {
    cout << "Cảm ơn bạn đã sử dụng chương trình!" << endl;
    exit(0);
}

void dungManHinh() {
    cout << "Nhấn phím bất kỳ để tiếp tục...";
    _getch();
}

void xoaManHinh() {
    system("cls");
}

string layNgayHienTai() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string ngay(dt);
    // Loại bỏ ký tự xuống dòng cuối chuỗi
    if (!ngay.empty() && ngay.back() == '\n') {
        ngay.pop_back();
    }
    return ngay;
}

void inTieuDe(const string& tieuDe) {
    int chieuDai = tieuDe.length();
    int tongChieuDai = 80;
    int khoangTrangTrai = (tongChieuDai - chieuDai - 2) / 2;
    int khoangTrangPhai = tongChieuDai - chieuDai - 2 - khoangTrangTrai;
    
    cout << "╔";
    for (int i = 0; i < tongChieuDai - 2; i++) {
        cout << "═";
    }
    cout << "╗" << endl;
    
    cout << "║";
    for (int i = 0; i < khoangTrangTrai; i++) {
        cout << " ";
    }
    cout << tieuDe;
    for (int i = 0; i < khoangTrangPhai; i++) {
        cout << " ";
    }
    cout << "║" << endl;
    
    cout << "╚";
    for (int i = 0; i < tongChieuDai - 2; i++) {
        cout << "═";
    }
    cout << "╝" << endl;
}
