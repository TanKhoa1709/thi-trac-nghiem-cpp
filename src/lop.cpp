#include "lop.h"
#include "sinhvien.h"

void khoiTaoLop() {
  soLuongLop = 0;
  for (int i = 0; i < MAX_LOP; i++) {
    danhSachLop[i].maLop = "";
    danhSachLop[i].tenLop = "";
    danhSachLop[i].danhSachSV = nullptr;
  }
}

int timLop(const string& maLop) {
  for (int i = 0; i < soLuongLop; i++) {
    if (danhSachLop[i].maLop == maLop) {
      return i;
    }
  }
  return -1;  // Không tìm thấy
}

void themLop(const string& maLop, const string& tenLop) {
  if (soLuongLop >= MAX_LOP) {
    cout << "Đã đạt giới hạn số lượng lớp!" << endl;
    return;
  }

  if (timLop(maLop) != -1) {
    cout << "Lớp " << maLop << " đã tồn tại!" << endl;
    return;
  }

  danhSachLop[soLuongLop].maLop = maLop;
  danhSachLop[soLuongLop].tenLop = tenLop;
  danhSachLop[soLuongLop].danhSachSV = nullptr;
  soLuongLop++;

  cout << "Đã thêm lớp " << maLop << " - " << tenLop << endl;
}

void xoaLop(const string& maLop) {
  int viTri = timLop(maLop);
  if (viTri == -1) {
    cout << "Không tìm thấy lớp " << maLop << endl;
    return;
  }

  // Xóa toàn bộ sinh viên trong lớp
  xoaToanBoSinhVien(danhSachLop[viTri].danhSachSV);

  // Dịch chuyển các lớp phía sau lên
  for (int i = viTri; i < soLuongLop - 1; i++) {
    danhSachLop[i] = danhSachLop[i + 1];
  }

  // Xóa lớp cuối
  danhSachLop[soLuongLop - 1].maLop = "";
  danhSachLop[soLuongLop - 1].tenLop = "";
  danhSachLop[soLuongLop - 1].danhSachSV = nullptr;
  soLuongLop--;

  cout << "Đã xóa lớp " << maLop << endl;
}

void hienThiDanhSachLop() {
  if (soLuongLop == 0) {
    cout << "Chưa có lớp nào trong hệ thống." << endl;
    return;
  }
  
  cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗" << endl;
  cout << "║                                DANH SÁCH LỚP                                         ║" << endl;
  cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣" << endl;
  cout << "║ STT │ Mã lớp │ Tên lớp                           │ Số sinh viên                    ║" << endl;
  cout << "╠═════╪════════╪═══════════════════════════════════╪═════════════════════════════════╣" << endl;
  
  for (int i = 0; i < soLuongLop; i++) {
    int soSV = demSoSinhVienTrongLop(danhSachLop[i].maLop);
    cout << "║ " << setw(3) << (i + 1) << " │ "
        << setw(6) << danhSachLop[i].maLop << " │ "
        << setw(35) << danhSachLop[i].tenLop << " │ "
        << setw(30) << soSV << " ║" << endl;
  }
  cout << "╚═════╧════════╧═══════════════════════════════════╧═════════════════════════════════╝" << endl;
}