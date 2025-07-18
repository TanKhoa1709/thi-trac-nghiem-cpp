#include "sinhvien.h"
#include "lop.h"
#include "diemthi.h"
#include "monhoc.h"
#include "cauhoi.h"

void khoiTaoSinhVien() {
    // Không cần khởi tạo gì đặc biệt
}

SinhVien* taoSinhVien(const string& maSV, const string& ho, const string& ten,
                      const string& maLop, const string& matKhau) {
    SinhVien* moi = new SinhVien;
    moi->maSV = maSV;
    moi->ho = ho;
    moi->ten = ten;
    moi->maLop = maLop;
    moi->matKhau = matKhau;
    moi->danhSachDiem = nullptr;
    moi->tiep = nullptr;
    return moi;
}

void themSinhVien(SinhVien*& danhSach, SinhVien* sinhVienMoi) {
    if (danhSach == nullptr) {
        danhSach = sinhVienMoi;
    } else {
        SinhVien* temp = danhSach;
        while (temp->tiep != nullptr) {
            temp = temp->tiep;
        }
        temp->tiep = sinhVienMoi;
    }
}

SinhVien* timSinhVien(SinhVien* danhSach, const string& maSV) {
    SinhVien* temp = danhSach;
    while (temp != nullptr) {
        if (temp->maSV == maSV) {
            return temp;
        }
        temp = temp->tiep;
    }
    return nullptr;
}

void xoaSinhVien(SinhVien*& danhSach, const string& maSV) {
    if (danhSach == nullptr) {
        cout << "Danh sách sinh viên rỗng!" << endl;
        return;
    }
    
    if (danhSach->maSV == maSV) {
        SinhVien* temp = danhSach;
        danhSach = danhSach->tiep;
        xoaDanhSachDiem(temp->danhSachDiem);
        delete temp;
        cout << "Đã xóa sinh viên " << maSV << endl;
        return;
    }
    
    SinhVien* temp = danhSach;
    while (temp->tiep != nullptr && temp->tiep->maSV != maSV) {
        temp = temp->tiep;
    }
    
    if (temp->tiep != nullptr) {
        SinhVien* canXoa = temp->tiep;
        temp->tiep = canXoa->tiep;
        xoaDanhSachDiem(canXoa->danhSachDiem);
        delete canXoa;
        cout << "Đã xóa sinh viên " << maSV << endl;
    } else {
        cout << "Không tìm thấy sinh viên " << maSV << endl;
    }
}

void hienThiDanhSachSinhVien(SinhVien* danhSach) {
    if (danhSach == nullptr) {
        cout << "Chưa có sinh viên nào trong lớp." << endl;
        return;
    }
    
    cout << "╔══════════════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                                DANH SÁCH SINH VIÊN                                   ║" << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║ STT │ Mã SV │ Họ                    │ Tên               │ Lớp                      ║" << endl;
    cout << "╠═════╪═══════╪═══════════════════════╪═══════════════════╪══════════════════════════╣" << endl;
    
    SinhVien* temp = danhSach;
    int stt = 1;
    while (temp != nullptr) {
        cout << "║ " << setw(3) << stt++ << " │ " << setw(5) << temp->maSV << " │ " 
             << setw(21) << temp->ho << " │ " << setw(17) << temp->ten << " │ " 
             << setw(24) << temp->maLop << " ║" << endl;
        temp = temp->tiep;
    }
    
    cout << "╚═════╧═══════╧═══════════════════════╧═══════════════════╧══════════════════════════╝" << endl;
}

void xoaToanBoSinhVien(SinhVien*& danhSach) {
    while (danhSach != nullptr) {
        SinhVien* temp = danhSach;
        danhSach = danhSach->tiep;
        xoaDanhSachDiem(temp->danhSachDiem);
        delete temp;
    }
}

int demSoSinhVienTrongLop(const string& maLop) {
    int viTriLop = timLop(maLop);
    if (viTriLop == -1) return 0;
    
    int dem = 0;
    SinhVien* temp = danhSachLop[viTriLop].danhSachSV;
    while (temp != nullptr) {
        dem++;
        temp = temp->tiep;
    }
    return dem;
}

SinhVien* dangNhap(const string& maSV, const string& matKhau) {
    for (int i = 0; i < soLuongLop; i++) {
        SinhVien* sv = timSinhVien(danhSachLop[i].danhSachSV, maSV);
        if (sv != nullptr && sv->matKhau == matKhau) {
            return sv;
        }
    }
    return nullptr;
}

// Hàm đăng nhập sinh viên với UI
SinhVien* dangNhapSinhVien() {
    string maSV, matKhau;
    cout << "Nhập mã sinh viên: ";
    cin >> maSV;
    cout << "Nhập mật khẩu: ";
    cin >> matKhau;
    
    SinhVien* sv = dangNhap(maSV, matKhau);
    if (sv != nullptr) {
        cout << "Đăng nhập thành công! Chào mừng " << sv->ho << " " << sv->ten << endl;
        dungManHinh();
        return sv;
    } else {
        cout << "Mã sinh viên hoặc mật khẩu không đúng!" << endl;
        dungManHinh();
        return nullptr;
    }
}

// Hàm quản lý sinh viên với UI đầy đủ
void quanLySinhVien() {
    int luaChon;
    do {
        xoaManHinh();
        inTieuDe("QUẢN LÝ SINH VIÊN");
        cout << "1. Thêm sinh viên" << endl;
        cout << "2. Hiển thị danh sách sinh viên theo lớp" << endl;
        cout << "3. Tìm sinh viên" << endl;
        cout << "4. Xóa sinh viên" << endl;
        cout << "5. Hiển thị tất cả sinh viên" << endl;
        cout << "0. Quay lại" << endl;
        cout << "Nhập lựa chọn: ";
        cin >> luaChon;
        
        switch (luaChon) {
            case 1:
                themSinhVienMoi();
                break;
            case 2:
                hienThiSinhVienTheoLop();
                break;
            case 3:
                timKiemSinhVien();
                break;
            case 4:
                xoaSinhVienTheoMa();
                break;
            case 5:
                hienThiTatCaSinhVien();
                break;
        }
    } while (luaChon != 0);
}

void themSinhVienMoi() {
    string maSV, ho, ten, maLop, matKhau;
    bool gioiTinh;
    string sdt;
    
    cout << "Nhập mã sinh viên: ";
    cin >> maSV;
    
    // Kiểm tra mã sinh viên đã tồn tại
    if (kiemTraMaSinhVienTonTai(maSV)) {
        cout << "Mã sinh viên đã tồn tại!" << endl;
        dungManHinh();
        return;
    }
    
    cin.ignore();
    cout << "Nhập họ: ";
    getline(cin, ho);
    cout << "Nhập tên: ";
    getline(cin, ten);
    cout << "Nhập giới tính (1-Nam, 0-Nữ): ";
    cin >> gioiTinh;
    cout << "Nhập số điện thoại: ";
    cin >> sdt;
    cout << "Nhập mã lớp: ";
    cin >> maLop;
    cout << "Nhập mật khẩu: ";
    cin >> matKhau;
    
    // Kiểm tra lớp có tồn tại không
    int viTriLop = timLop(maLop);
    if (viTriLop == -1) {
        cout << "Lớp không tồn tại!" << endl;
    } else {
        SinhVien* svMoi = taoSinhVien(maSV, ho, ten, maLop, matKhau);
        svMoi->gioiTinh = gioiTinh;
        svMoi->sdt = sdt;
        themSinhVien(danhSachLop[viTriLop].danhSachSV, svMoi);
        cout << "Đã thêm sinh viên thành công!" << endl;
    }
    dungManHinh();
}

void hienThiSinhVienTheoLop() {
    string maLop;
    cout << "Nhập mã lớp: ";
    cin >> maLop;
    int viTriLop = timLop(maLop);
    if (viTriLop != -1) {
        cout << "Danh sách sinh viên lớp " << maLop << ":" << endl;
        hienThiDanhSachSinhVien(danhSachLop[viTriLop].danhSachSV);
    } else {
        cout << "Không tìm thấy lớp!" << endl;
    }
    dungManHinh();
}

void timKiemSinhVien() {
    string maSV;
    cout << "Nhập mã sinh viên cần tìm: ";
    cin >> maSV;
    
    SinhVien* sv = timSinhVienTrongTatCaLop(maSV);
    if (sv != nullptr) {
        cout << "Tìm thấy sinh viên:" << endl;
        cout << "Mã SV: " << sv->maSV << endl;
        cout << "Họ tên: " << sv->ho << " " << sv->ten << endl;
        cout << "Giới tính: " << (sv->gioiTinh ? "Nam" : "Nữ") << endl;
        cout << "SĐT: " << sv->sdt << endl;
        cout << "Lớp: " << sv->maLop << endl;
    } else {
        cout << "Không tìm thấy sinh viên!" << endl;
    }
    dungManHinh();
}

void xoaSinhVienTheoMa() {
    string maSV;
    cout << "Nhập mã sinh viên cần xóa: ";
    cin >> maSV;
    
    bool daXoa = false;
    for (int i = 0; i < soLuongLop; i++) {
        SinhVien* sv = timSinhVien(danhSachLop[i].danhSachSV, maSV);
        if (sv != nullptr) {
            xoaSinhVien(danhSachLop[i].danhSachSV, maSV);
            daXoa = true;
            break;
        }
    }
    if (!daXoa) {
        cout << "Không tìm thấy sinh viên!" << endl;
    }
    dungManHinh();
}

void hienThiTatCaSinhVien() {
    cout << "Tất cả sinh viên trong hệ thống:" << endl;
    for (int i = 0; i < soLuongLop; i++) {
        if (danhSachLop[i].danhSachSV != nullptr) {
            cout << "\n=== Lớp " << danhSachLop[i].maLop << " - " << danhSachLop[i].tenLop << " ===" << endl;
            hienThiDanhSachSinhVien(danhSachLop[i].danhSachSV);
        }
    }
    dungManHinh();
}

// Hàm làm bài thi
void lamBaiThi(SinhVien* sv) {
    string maMon;
    cout << "Nhập mã môn thi: ";
    cin >> maMon;
    
    MonHoc* mon = timMonHoc(maMon);
    if (mon == nullptr) {
        cout << "Không tìm thấy môn học!" << endl;
        dungManHinh();
        return;
    }
    
    if (mon->gocCauHoi == nullptr) {
        cout << "Môn học chưa có câu hỏi nào!" << endl;
        dungManHinh();
        return;
    }
    
    // Lấy danh sách câu hỏi ngẫu nhiên
    vector<CauHoi*> dsCauHoi;
    layTatCaCauHoi(mon->gocCauHoi, dsCauHoi);
    
    if (dsCauHoi.empty()) {
        cout << "Không có câu hỏi nào trong môn học!" << endl;
        dungManHinh();
        return;
    }
    
    // Trộn câu hỏi
    random_device rd;
    mt19937 g(rd());
    shuffle(dsCauHoi.begin(), dsCauHoi.end(), g);
    
    // Giới hạn số câu hỏi (tối đa 10 câu)
    int soCauHoi = min(10, (int)dsCauHoi.size());
    
    ChiTietBaiThi* chiTietDau = nullptr;
    int soCauDung = 0;
    
    xoaManHinh();
    inTieuDe("BÀI THI MÔN " + mon->tenMon);
    cout << "Số câu hỏi: " << soCauHoi << endl;
    cout << "Thời gian: 15 phút" << endl;
    cout << "Nhấn Enter để bắt đầu...";
    cin.ignore();
    cin.get();
    
    for (int i = 0; i < soCauHoi; i++) {
        xoaManHinh();
        cout << "Câu " << (i + 1) << "/" << soCauHoi << ":" << endl;
        cout << dsCauHoi[i]->noiDung << endl;
        cout << "A. " << dsCauHoi[i]->dapAnA << endl;
        cout << "B. " << dsCauHoi[i]->dapAnB << endl;
        cout << "C. " << dsCauHoi[i]->dapAnC << endl;
        cout << "D. " << dsCauHoi[i]->dapAnD << endl;
        
        char dapAn;
        do {
            cout << "Chọn đáp án (A/B/C/D): ";
            cin >> dapAn;
            dapAn = toupper(dapAn);
        } while (dapAn != 'A' && dapAn != 'B' && dapAn != 'C' && dapAn != 'D');
        
        bool dung = (dapAn == dsCauHoi[i]->dapAnDung);
        if (dung) soCauDung++;
        
        ChiTietBaiThi* chiTiet = taoChiTietBaiThi(dsCauHoi[i]->id, dapAn, dung);
        themChiTietBaiThi(chiTietDau, chiTiet);
    }
    
    // Tính điểm
    float diem = (float)soCauDung / soCauHoi * 10;
    
    // Lưu kết quả
    luuKetQuaThi(sv, maMon, chiTietDau);
    
    xoaManHinh();
    inTieuDe("KẾT QUẢ BÀI THI");
    cout << "Số câu đúng: " << soCauDung << "/" << soCauHoi << endl;
    cout << "Điểm: " << fixed << setprecision(2) << diem << endl;
    if (diem >= 5.0) {
        cout << "Kết quả: ĐẠT" << endl;
    } else {
        cout << "Kết quả: KHÔNG ĐẠT" << endl;
    }
    dungManHinh();
}

// Hàm xem kết quả thi
void xemKetQuaThi(SinhVien* sv) {
    if (sv->danhSachDiem == nullptr) {
        cout << "Bạn chưa thi môn nào!" << endl;
        dungManHinh();
        return;
    }
    
    xoaManHinh();
    inTieuDe("KẾT QUẢ THI CỦA " + sv->ho + " " + sv->ten);
    hienThiDanhSachDiem(sv->danhSachDiem);
    dungManHinh();
}

// Hàm xem chi tiết bài thi
void xemChiTietBaiThi(SinhVien* sv) {
    if (sv->danhSachDiem == nullptr) {
        cout << "Bạn chưa thi môn nào!" << endl;
        dungManHinh();
        return;
    }
    
    string maMon;
    cout << "Nhập mã môn muốn xem chi tiết: ";
    cin >> maMon;
    
    DiemThi* diem = timDiemThi(sv->danhSachDiem, maMon);
    if (diem == nullptr) {
        cout << "Bạn chưa thi môn này!" << endl;
        dungManHinh();
        return;
    }
    
    xoaManHinh();
    inTieuDe("CHI TIẾT BÀI THI MÔN " + maMon);
    cout << "Điểm: " << fixed << setprecision(2) << diem->diem << endl;
    cout << "Ngày thi: " << diem->ngayThi << endl;
    cout << "\nChi tiết từng câu:" << endl;
    
    hienThiChiTietBaiThi(diem->chiTiet);
    dungManHinh();
}

// Hàm tiện ích cho sinh viên
SinhVien* timSinhVienTrongTatCaLop(const string& maSV) {
    for (int i = 0; i < soLuongLop; i++) {
        SinhVien* sv = timSinhVien(danhSachLop[i].danhSachSV, maSV);
        if (sv != nullptr) {
            return sv;
        }
    }
    return nullptr;
}

void capNhatThongTinSinhVien(SinhVien* sv) {
    if (sv == nullptr) return;
    
    string ho, ten, sdt, matKhau;
    bool gioiTinh;
    
    cout << "Thông tin hiện tại:" << endl;
    cout << "Mã SV: " << sv->maSV << endl;
    cout << "Họ tên: " << sv->ho << " " << sv->ten << endl;
    cout << "Giới tính: " << (sv->gioiTinh ? "Nam" : "Nữ") << endl;
    cout << "SĐT: " << sv->sdt << endl;
    cout << "Lớp: " << sv->maLop << endl;
    
    cout << "\nNhập thông tin mới (Enter để giữ nguyên):" << endl;
    
    cin.ignore();
    cout << "Họ mới: ";
    getline(cin, ho);
    if (!ho.empty()) sv->ho = ho;
    
    cout << "Tên mới: ";
    getline(cin, ten);
    if (!ten.empty()) sv->ten = ten;
    
    cout << "SĐT mới: ";
    getline(cin, sdt);
    if (!sdt.empty()) sv->sdt = sdt;
    
    cout << "Mật khẩu mới: ";
    getline(cin, matKhau);
    if (!matKhau.empty()) sv->matKhau = matKhau;
    
    cout << "Cập nhật thành công!" << endl;
}

bool kiemTraMaSinhVienTonTai(const string& maSV) {
    return timSinhVienTrongTatCaLop(maSV) != nullptr;
}

// Hàm file I/O
void ghiSinhVienRaFile(const string& tenFile) {
    ofstream file(tenFile);
    if (!file.is_open()) {
        cout << "Không thể mở file để ghi!" << endl;
        return;
    }
    
    for (int i = 0; i < soLuongLop; i++) {
        SinhVien* sv = danhSachLop[i].danhSachSV;
        while (sv != nullptr) {
            file << sv->maSV << "|" << sv->ho << "|" << sv->ten << "|" 
                 << sv->gioiTinh << "|" << sv->sdt << "|" << sv->maLop << "|" 
                 << sv->matKhau << endl;
            sv = sv->tiep;
        }
    }
    
    file.close();
    cout << "Đã ghi danh sách sinh viên ra file " << tenFile << endl;
}

void docSinhVienTuFile(const string& tenFile) {
    ifstream file(tenFile);
    if (!file.is_open()) {
        cout << "Không thể mở file để đọc!" << endl;
        return;
    }
    
    string line;
    int soSinhVienDoc = 0;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        // Phân tích dữ liệu từ dòng
        size_t pos = 0;
        vector<string> data;
        string token;
        
        while ((pos = line.find('|')) != string::npos) {
            token = line.substr(0, pos);
            data.push_back(token);
            line.erase(0, pos + 1);
        }
        data.push_back(line); // Phần cuối cùng
        
        if (data.size() >= 7) {
            string maSV = data[0];
            string ho = data[1];
            string ten = data[2];
            bool gioiTinh = (data[3] == "1");
            string sdt = data[4];
            string maLop = data[5];
            string matKhau = data[6];
            
            // Kiểm tra lớp có tồn tại
            int viTriLop = timLop(maLop);
            if (viTriLop != -1 && !kiemTraMaSinhVienTonTai(maSV)) {
                SinhVien* svMoi = taoSinhVien(maSV, ho, ten, maLop, matKhau);
                svMoi->gioiTinh = gioiTinh;
                svMoi->sdt = sdt;
                themSinhVien(danhSachLop[viTriLop].danhSachSV, svMoi);
                soSinhVienDoc++;
            }
        }
    }
    
    file.close();
    cout << "Đã đọc " << soSinhVienDoc << " sinh viên từ file " << tenFile << endl;
}
