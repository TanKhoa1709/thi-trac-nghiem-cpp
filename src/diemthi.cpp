#include "diemthi.h"
#include "lop.h"
#include "monhoc.h"
#include "sinhvien.h"

ChiTietBaiThi* taoChiTietBaiThi(int idCauHoi, char dapAnChon, bool dung) {
  ChiTietBaiThi* moi = new ChiTietBaiThi;
  moi->idCauHoi = idCauHoi;
  moi->dapAnChon = dapAnChon;
  moi->dung = dung;
  moi->tiep = nullptr;
  return moi;
}

void themChiTietBaiThi(ChiTietBaiThi*& danhSach, ChiTietBaiThi* chiTietMoi) {
  if (danhSach == nullptr) {
    danhSach = chiTietMoi;
  } else {
    ChiTietBaiThi* temp = danhSach;
    while (temp->tiep != nullptr) {
      temp = temp->tiep;
    }
    temp->tiep = chiTietMoi;
  }
}

void hienThiChiTietBaiThi(ChiTietBaiThi* danhSach) {
  if (danhSach == nullptr) {
    cout << "Chưa có chi tiết bài thi nào." << endl;
    return;
  }

  cout << "╔═══════════════════════════════════════════════════════════════════"
          "═══════════════════╗"
       << endl;
  cout << "║                            CHI TIẾT BÀI THI                       "
          "                   ║"
       << endl;
  cout << "╠═══════════════════════════════════════════════════════════════════"
          "═══════════════════╣"
       << endl;
  cout << "║ ID Câu Hỏi │ Đáp Án Chọn │ Kết Quả                                "
          "                 ║"
       << endl;
  cout << "╠════════════╪═════════════╪════════════════════════════════════════"
          "════════════════════╣"
       << endl;

  ChiTietBaiThi* temp = danhSach;
  while (temp != nullptr) {
    cout << "║ " << setw(10) << temp->idCauHoi << " │ " << setw(11)
         << temp->dapAnChon << " │ " << setw(54)
         << (temp->dung ? "Đúng" : "Sai") << " ║" << endl;
    temp = temp->tiep;
  }
  cout << "╚════════════╧═════════════╧════════════════════════════════════════"
          "════════════════════╝"
       << endl;
}

void xoaChiTietBaiThi(ChiTietBaiThi*& danhSach) {
  while (danhSach != nullptr) {
    ChiTietBaiThi* temp = danhSach;
    danhSach = danhSach->tiep;
    delete temp;
  }
}

DiemThi* taoDiemThi(const string& maMon, float diem, const string& ngayThi) {
  DiemThi* moi = new DiemThi;
  moi->maMon = maMon;
  moi->diem = diem;
  moi->ngayThi = ngayThi;
  moi->chiTiet = nullptr;
  moi->tiep = nullptr;
  return moi;
}

void themDiemThi(DiemThi*& danhSach, DiemThi* diemMoi) {
  if (danhSach == nullptr) {
    danhSach = diemMoi;
  } else {
    DiemThi* temp = danhSach;
    while (temp->tiep != nullptr) {
      temp = temp->tiep;
    }
    temp->tiep = diemMoi;
  }
}

DiemThi* timDiemThi(DiemThi* danhSach, const string& maMon) {
  DiemThi* temp = danhSach;
  while (temp != nullptr) {
    if (temp->maMon == maMon) {
      return temp;
    }
    temp = temp->tiep;
  }
  return nullptr;
}

void hienThiDanhSachDiem(DiemThi* danhSach) {
  if (danhSach == nullptr) {
    cout << "Chưa có điểm thi nào." << endl;
    return;
  }

  cout << "╔═══════════════════════════════════════════════════════════════════"
          "═══════════════════╗"
       << endl;
  cout << "║                            DANH SÁCH ĐIỂM THI                     "
          "                   ║"
       << endl;
  cout << "╠═══════════════════════════════════════════════════════════════════"
          "═══════════════════╣"
       << endl;
  cout << "║ Mã Môn │ Điểm │ Ngày Thi                                          "
          "                  ║"
       << endl;
  cout << "╠════════╪══════╪═══════════════════════════════════════════════════"
          "══════════════════╣"
       << endl;

  DiemThi* temp = danhSach;
  while (temp != nullptr) {
    cout << "║ " << setw(6) << temp->maMon << " │ " << setw(4) << fixed
         << setprecision(1) << temp->diem << " │ " << setw(67) << temp->ngayThi
         << " ║" << endl;
    temp = temp->tiep;
  }
  cout << "╚════════╧══════╧═══════════════════════════════════════════════════"
          "══════════════════╝"
       << endl;
}

void xoaDanhSachDiem(DiemThi*& danhSach) {
  while (danhSach != nullptr) {
    DiemThi* temp = danhSach;
    danhSach = danhSach->tiep;
    xoaChiTietBaiThi(temp->chiTiet);
    delete temp;
  }
}

float tinhDiem(ChiTietBaiThi* chiTiet) {
  if (chiTiet == nullptr) return 0.0f;

  int tongCau = 0;
  int cauDung = 0;

  ChiTietBaiThi* temp = chiTiet;
  while (temp != nullptr) {
    tongCau++;
    if (temp->dung) {
      cauDung++;
    }
    temp = temp->tiep;
  }

  if (tongCau == 0) return 0.0f;
  return (float)cauDung / tongCau * 10.0f;
}

void luuKetQuaThi(SinhVien* sv, const string& maMon, ChiTietBaiThi* chiTiet) {
  if (sv == nullptr) return;

  // Tính điểm
  float diem = tinhDiem(chiTiet);

  // Tạo bản ghi điểm thi mới
  DiemThi* diemMoi = taoDiemThi(maMon, diem, layNgayHienTai());
  diemMoi->chiTiet = chiTiet;

  // Thêm vào danh sách điểm của sinh viên
  themDiemThi(sv->danhSachDiem, diemMoi);

  cout << "Đã lưu kết quả thi. Điểm của bạn: " << fixed << setprecision(1)
       << diem << endl;
}

void xemChiTietBaiThi(SinhVien* sv, const string& maMon) {
  if (sv == nullptr) {
    cout << "Sinh viên không tồn tại." << endl;
    return;
  }

  DiemThi* diemThi = timDiemThi(sv->danhSachDiem, maMon);
  if (diemThi == nullptr) {
    cout << "Chưa thi môn " << maMon << endl;
    return;
  }

  cout << "╔═══════════════════════════════════════════════════════════════════"
          "═══════════════════╗"
       << endl;
  cout << "║                            KẾT QUẢ THI CHI TIẾT                   "
          "                   ║"
       << endl;
  cout << "╠═══════════════════════════════════════════════════════════════════"
          "═══════════════════╣"
       << endl;
  cout << "║ Mã môn: " << setw(15) << maMon << " │ Điểm: " << setw(10) << fixed
       << setprecision(1) << diemThi->diem << " │ Ngày thi: " << setw(25)
       << diemThi->ngayThi << " ║" << endl;
  cout << "╚═══════════════════════════════════════════════════════════════════"
          "═══════════════════╝"
       << endl;

  hienThiChiTietBaiThi(diemThi->chiTiet);
}

void inBangDiemTheoLop(const string& maLop, const string& maMon) {
  int viTriLop = timLop(maLop);
  if (viTriLop == -1) {
    cout << "Không tìm thấy lớp " << maLop << endl;
    return;
  }

  cout << "╔═══════════════════════════════════════════════════════════════════"
          "═══════════════════╗"
       << endl;
  cout << "║                        BẢNG ĐIỂM LỚP " << setw(10) << maLop
       << " - MÔN " << setw(10) << maMon << "                    ║" << endl;
  cout << "╠═══════════════════════════════════════════════════════════════════"
          "═══════════════════╣"
       << endl;
  cout << "║ STT │ Mã SV │ Họ tên                    │ Điểm │ Ngày thi         "
          "             ║"
       << endl;
  cout << "╠═════╪═══════╪═══════════════════════════╪══════╪══════════════════"
          "═════════════╣"
       << endl;

  SinhVien* sv = danhSachLop[viTriLop].danhSachSV;
  int stt = 1;

  while (sv != nullptr) {
    DiemThi* diemThi = timDiemThi(sv->danhSachDiem, maMon);
    if (diemThi != nullptr) {
      cout << "║ " << setw(3) << stt++ << " │ " << setw(5) << sv->maSV << " │ "
           << setw(25) << (sv->ho + " " + sv->ten) << " │ " << setw(4) << fixed
           << setprecision(1) << diemThi->diem << " │ " << setw(29)
           << diemThi->ngayThi << " ║" << endl;
    }
    sv = sv->tiep;
  }

  cout << "╚═════╧═══════╧═══════════════════════════╧══════╧══════════════════"
          "═════════════╝"
       << endl;
}

void inBangDiemTongKet(const string& maLop) {
  int viTriLop = timLop(maLop);
  if (viTriLop == -1) {
    cout << "Không tìm thấy lớp " << maLop << endl;
    return;
  }

  cout << "╔═══════════════════════════════════════════════════════════════════"
          "═══════════════════╗"
       << endl;
  cout << "║                        BẢNG ĐIỂM TỔNG KẾT LỚP " << setw(10)
       << maLop << "                           ║" << endl;
  cout << "╠═══════════════════════════════════════════════════════════════════"
          "═══════════════════╣"
       << endl;
  cout << "║ STT │ Mã SV │ Họ tên                    │ Số môn đã thi │ Điểm "
          "trung bình   ║"
       << endl;
  cout << "╠═════╪═══════╪═══════════════════════════╪═══════════════╪═════════"
          "══════════╣"
       << endl;

  SinhVien* sv = danhSachLop[viTriLop].danhSachSV;
  int stt = 1;

  while (sv != nullptr) {
    int soMon = 0;
    float tongDiem = 0.0f;

    DiemThi* diemThi = sv->danhSachDiem;
    while (diemThi != nullptr) {
      soMon++;
      tongDiem += diemThi->diem;
      diemThi = diemThi->tiep;
    }

    float diemTrungBinh = (soMon > 0) ? tongDiem / soMon : 0.0f;

    cout << "║ " << setw(3) << stt++ << " │ " << setw(5) << sv->maSV << " │ "
         << setw(25) << (sv->ho + " " + sv->ten) << " │ " << setw(13) << soMon
         << " │ " << setw(17) << fixed << setprecision(2) << diemTrungBinh
         << " ║" << endl;

    sv = sv->tiep;
  }

  cout << "╚═════╧═══════╧═══════════════════════════╧═══════════════╧═════════"
          "══════════╝"
       << endl;
}

void thongKeDiemTheoMon(const string& maMon) {
  int tongSV = 0;
  float tongDiem = 0.0f;
  int svDat = 0;

  for (int i = 0; i < soLuongLop; i++) {
    SinhVien* sv = danhSachLop[i].danhSachSV;
    while (sv != nullptr) {
      DiemThi* diemThi = timDiemThi(sv->danhSachDiem, maMon);
      if (diemThi != nullptr) {
        tongSV++;
        tongDiem += diemThi->diem;
        if (diemThi->diem >= 5.0f) {
          svDat++;
        }
      }
      sv = sv->tiep;
    }
  }

  cout << "╔═══════════════════════════════════════════════════════════════════"
          "═══════════════════╗"
       << endl;
  cout << "║                        THỐNG KÊ ĐIỂM MÔN " << setw(15) << maMon
       << "                             ║" << endl;
  cout << "╠═══════════════════════════════════════════════════════════════════"
          "═══════════════════╣"
       << endl;
  cout << "║ Tổng số sinh viên thi: " << setw(10) << tongSV
       << "                                        ║" << endl;
  cout << "║ Số sinh viên đạt (>=5): " << setw(10) << svDat
       << "                                       ║" << endl;
  cout << "║ Tỷ lệ đạt: " << setw(10) << fixed << setprecision(1)
       << (tongSV > 0 ? (float)svDat / tongSV * 100 : 0)
       << "%                                              ║" << endl;
  cout << "║ Điểm trung bình: " << setw(10) << fixed << setprecision(2)
       << (tongSV > 0 ? tongDiem / tongSV : 0)
       << "                                           ║" << endl;
  cout << "╚═══════════════════════════════════════════════════════════════════"
          "═══════════════════╝"
       << endl;
}

void ghiDiemRaFile(DiemThi* danhSach, const string& tenFile) {
  ofstream file(tenFile);
  if (!file.is_open()) {
    cout << "Không thể mở file " << tenFile << endl;
    return;
  }

  DiemThi* temp = danhSach;
  while (temp != nullptr) {
    file << temp->maMon << "|" << temp->diem << "|" << temp->ngayThi << endl;

    // Ghi chi tiết bài thi
    ChiTietBaiThi* chiTiet = temp->chiTiet;
    while (chiTiet != nullptr) {
      file << chiTiet->idCauHoi << "," << chiTiet->dapAnChon << ","
           << chiTiet->dung << endl;
      chiTiet = chiTiet->tiep;
    }
    file << "---" << endl;  // Dấu phân cách

    temp = temp->tiep;
  }

  file.close();
  cout << "Đã ghi danh sách điểm ra file " << tenFile << endl;
}

void docDiemTuFile(DiemThi*& danhSach, const string& tenFile) {
  ifstream file(tenFile);
  if (!file.is_open()) {
    cout << "Không thể mở file " << tenFile << endl;
    return;
  }

  string line;
  while (getline(file, line)) {
    if (line == "---") continue;

    // Đọc thông tin điểm thi
    size_t pos1 = line.find('|');
    size_t pos2 = line.find('|', pos1 + 1);

    if (pos1 != string::npos && pos2 != string::npos) {
      string maMon = line.substr(0, pos1);
      float diem = stof(line.substr(pos1 + 1, pos2 - pos1 - 1));
      string ngayThi = line.substr(pos2 + 1);

      DiemThi* diemMoi = taoDiemThi(maMon, diem, ngayThi);

      // Đọc chi tiết bài thi
      while (getline(file, line) && line != "---") {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        if (pos1 != string::npos && pos2 != string::npos) {
          int idCauHoi = stoi(line.substr(0, pos1));
          char dapAnChon = line.substr(pos1 + 1, 1)[0];
          bool dung = (line.substr(pos2 + 1) == "1");

          ChiTietBaiThi* chiTietMoi =
              taoChiTietBaiThi(idCauHoi, dapAnChon, dung);
          themChiTietBaiThi(diemMoi->chiTiet, chiTietMoi);
        }
      }

      themDiemThi(danhSach, diemMoi);
    }
  }

  file.close();
  cout << "Đã đọc danh sách điểm từ file " << tenFile << endl;
}