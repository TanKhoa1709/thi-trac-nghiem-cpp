#include "cauhoi.h"
#include "monhoc.h"

MonHoc* danhSachMonHoc = nullptr;

void khoiTaoMonHoc() { danhSachMonHoc = nullptr; }

MonHoc* taoMonHoc(const string& maMon, const string& tenMon) {
  MonHoc* moi = new MonHoc;
  moi->maMon = maMon;
  moi->tenMon = tenMon;
  moi->gocCauHoi = nullptr;
  moi->tiep = nullptr;
  return moi;
}

void themMonHoc(const string& maMon, const string& tenMon) {
  if (timMonHoc(maMon) != nullptr) {
    cout << "Môn học " << maMon << " đã tồn tại!" << endl;
    return;
  }

  MonHoc* monMoi = taoMonHoc(maMon, tenMon);

  if (danhSachMonHoc == nullptr) {
    danhSachMonHoc = monMoi;
  } else {
    MonHoc* temp = danhSachMonHoc;
    while (temp->tiep != nullptr) {
      temp = temp->tiep;
    }
    temp->tiep = monMoi;
  }

  cout << "Đã thêm môn học " << maMon << " - " << tenMon << endl;
}

MonHoc* timMonHoc(const string& maMon) {
  MonHoc* temp = danhSachMonHoc;
  while (temp != nullptr) {
    if (temp->maMon == maMon) {
      return temp;
    }
    temp = temp->tiep;
  }
  return nullptr;
}

void xoaMonHoc(const string& maMon) {
  if (danhSachMonHoc == nullptr) {
    cout << "Danh sách môn học rỗng!" << endl;
    return;
  }

  if (danhSachMonHoc->maMon == maMon) {
    MonHoc* temp = danhSachMonHoc;
    danhSachMonHoc = danhSachMonHoc->tiep;
    huyBoCay(temp->gocCauHoi);
    delete temp;
    cout << "Đã xóa môn học " << maMon << endl;
    return;
  }

  MonHoc* temp = danhSachMonHoc;
  while (temp->tiep != nullptr && temp->tiep->maMon != maMon) {
    temp = temp->tiep;
  }

  if (temp->tiep != nullptr) {
    MonHoc* canXoa = temp->tiep;
    temp->tiep = canXoa->tiep;
    huyBoCay(canXoa->gocCauHoi);
    delete canXoa;
    cout << "Đã xóa môn học " << maMon << endl;
  } else {
    cout << "Không tìm thấy môn học " << maMon << endl;
  }
}

void hienThiDanhSachMonHoc() {
    if (danhSachMonHoc == nullptr) {
        cout << "Chưa có môn học nào trong hệ thống." << endl;
        return;
    }
    
    cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                                DANH SÁCH MÔN HỌC                                     ║" << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║ STT │ Mã Môn │ Tên Môn                                      │ Số Câu Hỏi          ║" << endl;
    cout << "╠═════╪════════╪══════════════════════════════════════════════╪══════════════════════╣" << endl;
    
    MonHoc* temp = danhSachMonHoc;
    int stt = 1;
    while (temp != nullptr) {
        int soCauHoi = demSoCauHoi(temp->gocCauHoi);
        cout << "║ " << setw(3) << stt++ << " │ " << setw(6) << temp->maMon << " │ " 
             << setw(44) << temp->tenMon << " │ " << setw(20) << soCauHoi << " ║" << endl;
        temp = temp->tiep;
    }
    
    cout << "╚═════╧════════╧══════════════════════════════════════════════╧══════════════════════╝" << endl;
}

void xoaToanBoMonHoc() {
    while (danhSachMonHoc != nullptr) {
        MonHoc* temp = danhSachMonHoc;
        danhSachMonHoc = danhSachMonHoc->tiep;
        huyBoCay(temp->gocCauHoi);
        delete temp;
    }
}