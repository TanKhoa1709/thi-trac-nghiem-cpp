#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <stdexcept>
#include <csignal>


// Include manager classes
#include "include/manager/quanlylop.h"
#include "include/manager/quanlymonhoc.h"
#include "include/manager/quanlycauhoi.h"
#include "include/manager/quanlysinhvien.h"
#include "include/manager/quanlydiem.h"

// Include model classes
#include "include/models/lop.h"
#include "include/models/monhoc.h"
#include "include/models/cauhoi.h"
#include "include/models/sinhvien.h"
#include "include/models/diemthi.h"

using namespace std;

void signalHandler(int signal) {
    std::cerr << "Segmentation fault caught! Signal: " << signal << std::endl;
    std::exit(EXIT_FAILURE); // or handle as needed
}

// Global managers
QuanLyLop* quanLyLop = nullptr;
QuanLyMonHoc* quanLyMonHoc = nullptr;
QuanLySinhVien* quanLySinhVien = nullptr;

// Current user session
bool isTeacher = false;
SinhVien* currentStudent = nullptr;

// Utility functions
void clearScreen() {
    // system("cls");
}

void pauseScreen() {
    cout << "\nNhan phim bat ky de tiep tuc...";
    _getch();
}

void printHeader(const string& title) {
    clearScreen();
    cout << "========================================\n";
    cout << "   " << title << "\n";
    cout << "========================================\n\n";
}

void printSeparator() {
    cout << "----------------------------------------\n";
}

// Authentication functions
bool authenticateTeacher() {
    string username, password;
    cout << "Dang nhap Giao vien:\n";
    cout << "Ten dang nhap: ";
    cin >> username;
    cout << "Mat khau: ";
    cin >> password;
    
    return (username == "GV" && password == "GV");
}

bool authenticateStudent() {
    string masv, password;
    cout << "Dang nhap Sinh vien:\n";
    cout << "Ma sinh vien: ";
    cin >> masv;
    cout << "Mat khau: ";
    cin >> password;
    
    // Find student in all classes
    DynamicArray<Lop*> danhSachLop;
    quanLyLop->danhSach(danhSachLop);
    for (int i = 0; i < danhSachLop.size(); i++) {
        Lop* lop = danhSachLop.get(i);
        SinhVien* sv = lop->getQuanLySinhVien()->tim(masv);
        if (sv && sv->getPassword() == password) {
            currentStudent = sv;
            return true;
        }
    }
    return false;
}

// Teacher menu functions
void menuQuanLyLop() {
    int choice;
    do {
        printHeader("QUAN LY LOP");
        cout << "1. Xem danh sach lop\n";
        cout << "2. Them lop moi\n";
        cout << "3. Sua thong tin lop\n";
        cout << "4. Xoa lop\n";
        cout << "5. Quan ly sinh vien trong lop\n";
        cout << "0. Quay lai\n";
        cout << "Lua chon: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                printHeader("DANH SACH LOP");
                DynamicArray<Lop*> danhSach;
                quanLyLop->danhSach(danhSach);
                if (danhSach.size() == 0) {
                    cout << "Chua co lop nao!\n";
                } else {
                    cout << left << setw(10) << "Ma Lop" << setw(30) << "Ten Lop" << setw(15) << "So SV" << endl;
                    printSeparator();
                    for (int i = 0; i < danhSach.size(); i++) {
                        Lop* lop = danhSach.get(i);
                        cout << left << setw(10) << lop->getMaLop() 
                             << setw(30) << lop->getTenLop()
                             << setw(15) << lop->getQuanLySinhVien()->size() << endl;
                    }
                }
                pauseScreen();
                break;
            }
            case 2: {
                printHeader("THEM LOP MOI");
                string maLop, tenLop;
                cout << "Ma lop: ";
                cin >> maLop;
                cin.ignore();
                cout << "Ten lop: ";
                getline(cin, tenLop);
                
                Lop lopMoi(maLop, tenLop);
                if (quanLyLop->them(lopMoi)) {
                    cout << "Them lop thanh cong!\n";
                    quanLyLop->saveToFile();
                } else {
                    cout << "Them lop that bai! (Ma lop da ton tai)\n";
                }
                pauseScreen();
                break;
            }
            case 3: {
                printHeader("SUA THONG TIN LOP");
                string maLop;
                cout << "Ma lop can sua: ";
                cin >> maLop;
                
                Lop* lop = quanLyLop->tim(maLop);
                if (lop) {
                    string tenLopMoi;
                    cin.ignore();
                    cout << "Ten lop moi (hien tai: " << lop->getTenLop() << "): ";
                    getline(cin, tenLopMoi);
                    
                    lop->setTenLop(tenLopMoi);
                    cout << "Sua thong tin lop thanh cong!\n";
                    quanLyLop->saveToFile();
                } else {
                    cout << "Khong tim thay lop!\n";
                }
                pauseScreen();
                break;
            }
            case 4: {
                printHeader("XOA LOP");
                string maLop;
                cout << "Ma lop can xoa: ";
                cin >> maLop;
                
                if (quanLyLop->xoa(maLop)) {
                    cout << "Xoa lop thanh cong!\n";
                    quanLyLop->saveToFile();
                } else {
                    cout << "Xoa lop that bai! (Lop khong ton tai)\n";
                }
                pauseScreen();
                break;
            }
            case 5: {
                printHeader("QUAN LY SINH VIEN");
                string maLop;
                cout << "Ma lop: ";
                cin >> maLop;
                
                Lop* lop = quanLyLop->tim(maLop);
                if (lop) {
                    // Sub-menu for student management
                    int svChoice;
                    do {
                        printHeader("QUAN LY SINH VIEN - LOP " + lop->getTenLop());
                        cout << "1. Xem danh sach sinh vien\n";
                        cout << "2. Them sinh vien\n";
                        cout << "3. Sua thong tin sinh vien\n";
                        cout << "4. Xoa sinh vien\n";
                        cout << "0. Quay lai\n";
                        cout << "Lua chon: ";
                        cin >> svChoice;
                        
                        switch (svChoice) {
                            case 1: {
                                printHeader("DANH SACH SINH VIEN - " + lop->getTenLop());
                                DynamicArray<SinhVien*> dsSV;
                                lop->getQuanLySinhVien()->danhSach(dsSV);
                                if (dsSV.size() == 0) {
                                    cout << "Lop chua co sinh vien nao!\n";
                                } else {
                                    cout << left << setw(12) << "MASV" << setw(20) << "Ho" 
                                         << setw(15) << "Ten" << setw(10) << "Phai" << endl;
                                    printSeparator();
                                    for (int i = 0; i < dsSV.size(); i++) {
                                        SinhVien* sv = dsSV.get(i);
                                        cout << left << setw(12) << sv->getMaSinhVien()
                                             << setw(20) << sv->getHo()
                                             << setw(15) << sv->getTen()
                                             << setw(10) << sv->getPhaiBangChu() << endl;
                                    }
                                }
                                pauseScreen();
                                break;
                            }
                            case 2: {
                                printHeader("THEM SINH VIEN");
                                string masv, ho, ten, password;
                                bool phai;
                                int phaiChoice;
                                
                                cout << "Ma sinh vien: ";
                                cin >> masv;
                                cin.ignore();
                                cout << "Ho: ";
                                getline(cin, ho);
                                cout << "Ten: ";
                                getline(cin, ten);
                                cout << "Gioi tinh (1-Nam, 0-Nu): ";
                                cin >> phaiChoice;
                                phai = (phaiChoice == 1);
                                cout << "Mat khau: ";
                                cin >> password;
                                
                                SinhVien svMoi(masv, ho, ten, phai, password);
                                if (lop->getQuanLySinhVien()->them(svMoi)) {
                                    cout << "Them sinh vien thanh cong!\n";
                                    lop->getQuanLySinhVien()->saveToFile();
                                } else {
                                    cout << "Them sinh vien that bai! (MASV da ton tai)\n";
                                }
                                pauseScreen();
                                break;
                            }
                        }
                    } while (svChoice != 0);
                } else {
                    cout << "Khong tim thay lop!\n";
                    pauseScreen();
                }
                break;
            }
        }
    } while (choice != 0);
}

void menuQuanLyMonHoc() {
   std::signal(SIGSEGV, signalHandler); // Catch segmentation faults
    int choice;
    do {
        printHeader("QUAN LY MON HOC");
        cout << "1. Xem danh sach mon hoc\n";
        cout << "2. Them mon hoc moi\n";
        cout << "3. Sua thong tin mon hoc\n";
        cout << "4. Xoa mon hoc\n";
        cout << "5. Quan ly cau hoi theo mon\n";
        cout << "0. Quay lai\n";
        cout << "Lua chon: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                printHeader("DANH SACH MON HOC");
                DynamicArray<MonHoc*> danhSach;
                quanLyMonHoc->danhSach(danhSach);
                if (danhSach.size() == 0) {
                    cout << "Chua co mon hoc nao!\n";
                } else {
                    cout << left << setw(10) << "Ma Mon" << setw(40) << "Ten Mon" << setw(15) << "So Cau Hoi" << endl;
                    printSeparator();

                    MonHoc* mon = danhSach.get(0);

                    for (int i = 0; i < danhSach.size(); i++) {
                        MonHoc* mon = danhSach.get(i);
                        cout << left << setw(10) << mon->getMaMon()
                             << setw(40) << mon->getTenMon()
                             << setw(15) << mon->getQuanLyCauHoi()->size() << endl;
                    }
                }
                pauseScreen();
                break;
            }
            case 2: {
                printHeader("THEM MON HOC MOI");
                string maMon, tenMon;
                cout << "Ma mon hoc: ";
                cin >> maMon;
                cin.ignore();
                cout << "Ten mon hoc: ";
                getline(cin, tenMon);
                
                MonHoc monMoi(maMon.c_str(), tenMon);
                if (quanLyMonHoc->them(monMoi)) {
                    cout << "Them mon hoc thanh cong!\n";
                    quanLyMonHoc->saveToFile();
                } else {
                    cout << "Them mon hoc that bai! (Ma mon da ton tai)\n";
                }
                pauseScreen();
                break;
            }
            case 5: {
                printHeader("QUAN LY CAU HOI");
                string maMon;
                cout << "Ma mon hoc: ";
                cin >> maMon;
                
                MonHoc* mon = quanLyMonHoc->tim(maMon.c_str());
                if (mon) {
                    int chChoice;
                    do {
                        printHeader("QUAN LY CAU HOI - " + mon->getTenMon());
                        cout << "1. Xem danh sach cau hoi\n";
                        cout << "2. Them cau hoi moi\n";
                        cout << "3. Sua cau hoi\n";
                        cout << "4. Xoa cau hoi\n";
                        cout << "0. Quay lai\n";
                        cout << "Lua chon: ";
                        cin >> chChoice;
                        
                        switch (chChoice) {
                            case 1: {
                                printHeader("DANH SACH CAU HOI - " + mon->getTenMon());
                                DynamicArray<CauHoi*> dsCH;
                                mon->getQuanLyCauHoi()->danhSach(dsCH);
                                if (dsCH.size() == 0) {
                                    cout << "Mon hoc chua co cau hoi nao!\n";
                                } else {
                                    for (int i = 0; i < dsCH.size(); i++) {
                                        CauHoi* ch = dsCH.get(i);
                                        cout << "ID: " << ch->getMaCauHoi() << endl;
                                        cout << "Cau hoi: " << ch->getNoiDung() << endl;
                                        cout << "A. " << ch->getLuaChonA() << endl;
                                        cout << "B. " << ch->getLuaChonB() << endl;
                                        cout << "C. " << ch->getLuaChonC() << endl;
                                        cout << "D. " << ch->getLuaChonD() << endl;
                                        cout << "Dap an: " << ch->getDapAnDung() << endl;
                                        printSeparator();
                                    }
                                }
                                pauseScreen();
                                break;
                            }
                            case 2: {
                                printHeader("THEM CAU HOI MOI");
                                string noiDung, luaChonA, luaChonB, luaChonC, luaChonD;
                                char dapAn;
                                
                                cin.ignore();
                                cout << "Noi dung cau hoi: ";
                                getline(cin, noiDung);
                                cout << "Lua chon A: ";
                                getline(cin, luaChonA);
                                cout << "Lua chon B: ";
                                getline(cin, luaChonB);
                                cout << "Lua chon C: ";
                                getline(cin, luaChonC);
                                cout << "Lua chon D: ";
                                getline(cin, luaChonD);
                                cout << "Dap an dung (A/B/C/D): ";
                                cin >> dapAn;
                                
                                int maCauHoi = mon->getQuanLyCauHoi()->taoMaCauHoiNgauNhien();
                                CauHoi cauHoiMoi(maCauHoi, noiDung, luaChonA, luaChonB, luaChonC, luaChonD, dapAn);
                                
                                if (mon->getQuanLyCauHoi()->them(cauHoiMoi)) {
                                    cout << "Them cau hoi thanh cong! (ID: " << maCauHoi << ")\n";
                                    mon->getQuanLyCauHoi()->saveToFile();
                                } else {
                                    cout << "Them cau hoi that bai!\n";
                                }
                                pauseScreen();
                                break;
                            }
                        }
                    } while (chChoice != 0);
                } else {
                    cout << "Khong tim thay mon hoc!\n";
                    pauseScreen();
                }
                break;
            }
        }
    } while (choice != 0);
}

void menuBaoCao() {
    int choice;
    do {
        printHeader("BAO CAO VA THONG KE");
        cout << "1. Bang diem theo lop\n";
        cout << "2. Chi tiet bai thi sinh vien\n";
        cout << "3. Thong ke tong quan\n";
        cout << "0. Quay lai\n";
        cout << "Lua chon: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                printHeader("BANG DIEM THEO LOP");
                string maLop;
                cout << "Ma lop: ";
                cin >> maLop;
                
                Lop* lop = quanLyLop->tim(maLop);
                if (lop) {
                    DynamicArray<SinhVien*> dsSV;
                    lop->getQuanLySinhVien()->danhSach(dsSV);
                    cout << "\nBANG DIEM LOP: " << lop->getTenLop() << endl;
                    cout << left << setw(12) << "MASV" << setw(25) << "Ho Ten" << setw(15) << "Toan" 
                         << setw(15) << "Ly" << setw(15) << "Hoa" << endl;
                    printSeparator();
                    
                    for (int i = 0; i < dsSV.size(); i++) {
                        SinhVien* sv = dsSV.get(i);
                        cout << left << setw(12) << sv->getMaSinhVien()
                             << setw(25) << sv->getHoTen();
                        
                        // Display scores for each subject (simplified)
                        cout << setw(15) << "Chua thi"
                             << setw(15) << "Chua thi"
                             << setw(15) << "Chua thi" << endl;
                    }
                } else {
                    cout << "Khong tim thay lop!\n";
                }
                pauseScreen();
                break;
            }
            case 3: {
                printHeader("THONG KE TONG QUAN");
                cout << "So lop: " << quanLyLop->size() << endl;
                cout << "So mon hoc: " << quanLyMonHoc->size() << endl;
                
                int tongSinhVien = 0;
                int tongCauHoi = 0;

                DynamicArray<Lop*> danhSachLop;
                quanLyLop->danhSach(danhSachLop);
                for (int i = 0; i < danhSachLop.size(); i++) {
                    tongSinhVien += danhSachLop.get(i)->getQuanLySinhVien()->size();
                }

                DynamicArray<MonHoc*> danhSachMon;
                quanLyMonHoc->danhSach(danhSachMon);
                for (int i = 0; i < danhSachMon.size(); i++) {
                    tongCauHoi += danhSachMon.get(i)->getQuanLyCauHoi()->size();
                }
                
                cout << "Tong so sinh vien: " << tongSinhVien << endl;
                cout << "Tong so cau hoi: " << tongCauHoi << endl;
                pauseScreen();
                break;
            }
        }
    } while (choice != 0);
}

void menuGiaoVien() {
    int choice;
    do {
        printHeader("MENU GIAO VIEN");
        cout << "1. Quan ly lop\n";
        cout << "2. Quan ly mon hoc\n";
        cout << "3. Bao cao va thong ke\n";
        cout << "0. Dang xuat\n";
        cout << "Lua chon: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                menuQuanLyLop();
                break;
            case 2:
                menuQuanLyMonHoc();
                break;
            case 3:
                menuBaoCao();
                break;
        }
    } while (choice != 0);
}

// Student menu functions
void menuLamBaiThi() {
    printHeader("LAM BAI THI");
    
    // Display available subjects
    DynamicArray<MonHoc*> danhSachMon;
    quanLyMonHoc->danhSach(danhSachMon);
    if (danhSachMon.size() == 0) {
        cout << "Khong co mon hoc nao de thi!\n";
        pauseScreen();
        return;
    }
    
    cout << "Chon mon hoc:\n";
    for (int i = 0; i < danhSachMon.size(); i++) {
        cout << (i + 1) << ". " << danhSachMon.get(i)->getTenMon() 
             << " (" << danhSachMon.get(i)->getQuanLyCauHoi()->size() << " cau hoi)" << endl;
    }
    
    int monChoice;
    cout << "Lua chon mon: ";
    cin >> monChoice;
    
    if (monChoice < 1 || monChoice > danhSachMon.size()) {
        cout << "Lua chon khong hop le!\n";
        pauseScreen();
        return;
    }
    
    MonHoc& monThi = *danhSachMon.get(monChoice - 1);
    
    int soCauHoi;
    cout << "So cau hoi muon thi (toi da " << monThi.getQuanLyCauHoi()->size() << "): ";
    cin >> soCauHoi;
    
    if (soCauHoi <= 0 || soCauHoi > monThi.getQuanLyCauHoi()->size()) {
        cout << "So cau hoi khong hop le!\n";
        pauseScreen();
        return;
    }
    
    // Get random questions
    DynamicArray<CauHoi*> dsCauHoi = monThi.getQuanLyCauHoi()->layNgauNhien(soCauHoi);
    
    printHeader("BAI THI: " + monThi.getTenMon());
    cout << "Sinh vien: " << currentStudent->getHoTen() << endl;
    cout << "So cau hoi: " << soCauHoi << endl;
    cout << "Thoi gian: 60 phut (demo - khong gioi han)\n";
    printSeparator();
    
    int diem = 0;
    for (int i = 0; i < dsCauHoi.size(); i++) {
        CauHoi& ch = *dsCauHoi.get(i);
        cout << "\nCau " << (i + 1) << ": " << ch.getNoiDung() << endl;
        cout << "A. " << ch.getLuaChonA() << endl;
        cout << "B. " << ch.getLuaChonB() << endl;
        cout << "C. " << ch.getLuaChonC() << endl;
        cout << "D. " << ch.getLuaChonD() << endl;
        
        char dapAn;
        cout << "Dap an cua ban (A/B/C/D): ";
        cin >> dapAn;
        
        if (ch.kiemTraDapAn(dapAn)) {
            diem++;
            cout << "Dung!\n";
        } else {
            cout << "Sai! Dap an dung la: " << ch.getDapAnDung() << endl;
        }
    }
    
    float diemCuoi = (float)diem / soCauHoi * 10;
    cout << "\n" << string(50, '=') << endl;
    cout << "KET QUA BAI THI" << endl;
    cout << "Diem: " << fixed << setprecision(2) << diemCuoi << "/10" << endl;
    cout << "So cau dung: " << diem << "/" << soCauHoi << endl;
    cout << string(50, '=') << endl;
    
    pauseScreen();
}

void menuSinhVien() {
    int choice;
    do {
        printHeader("MENU SINH VIEN");
        cout << "Xin chao, " << currentStudent->getHoTen() << "!\n\n";
        cout << "1. Lam bai thi\n";
        cout << "2. Xem ket qua thi\n";
        cout << "3. Thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "Lua chon: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                menuLamBaiThi();
                break;
            case 2:
                printHeader("KET QUA THI");
                cout << "Chuc nang dang phat trien...\n";
                pauseScreen();
                break;
            case 3:
                printHeader("THONG TIN CA NHAN");
                cout << "Ma sinh vien: " << currentStudent->getMaSinhVien() << endl;
                cout << "Ho ten: " << currentStudent->getHoTen() << endl;
                cout << "Gioi tinh: " << currentStudent->getPhaiBangChu() << endl;
                pauseScreen();
                break;
        }
    } while (choice != 0);
}

// Main application functions
void initializeSystem() {
    cout << "Initializing system...\n";
    // Initialize managers
    quanLyLop = new QuanLyLop();
    quanLyMonHoc = new QuanLyMonHoc();
    
    cout << "Loading data from files...\n";

    // Load data from files
    quanLyLop->loadFromFile();
    quanLyMonHoc->loadFromFile();

    cout << "Loading questions for each subject...\n";
    
    // Load questions for each subject
    DynamicArray<MonHoc*> danhSachMon;
    quanLyMonHoc->danhSach(danhSachMon);
    for (int i = 0; i < danhSachMon.size(); i++) {
        danhSachMon.get(i)->getQuanLyCauHoi()->loadFromFile();
    }

    cout << "Loading students for each class...\n";
    
    // Load students for each class
    DynamicArray<Lop*> danhSachLop;
    quanLyLop->danhSach(danhSachLop);
    for (int i = 0; i < danhSachLop.size(); i++) {
        danhSachLop.get(i)->getQuanLySinhVien()->loadFromFile();
    }
}

void cleanupSystem() {
    if (quanLyLop) {
        delete quanLyLop;
        quanLyLop = nullptr;
    }
    if (quanLyMonHoc) {
        delete quanLyMonHoc;
        quanLyMonHoc = nullptr;
    }
}

int main() {
    // Set console to UTF-8 for Vietnamese characters
    SetConsoleOutputCP(CP_UTF8);
    
    cout << "Khoi tao he thong...\n";
    try {
        initializeSystem();
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    int choice;
    do {
        printHeader("HE THONG QUAN LY THI TRAC NGHIEM");
        cout << "1. Dang nhap Giao vien\n";
        cout << "2. Dang nhap Sinh vien\n";
        cout << "0. Thoat\n";
        cout << "Lua chon: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                if (authenticateTeacher()) {
                    isTeacher = true;
                    menuGiaoVien();
                    isTeacher = false;
                } else {
                    cout << "Dang nhap that bai!\n";
                    pauseScreen();
                }
                break;
            case 2:
                if (authenticateStudent()) {
                    menuSinhVien();
                    currentStudent = nullptr;
                } else {
                    cout << "Dang nhap that bai!\n";
                    pauseScreen();
                }
                break;
            case 0:
                cout << "Cam on ban da su dung he thong!\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
                pauseScreen();
        }
    } while (choice != 0);
    
    cleanupSystem();
    return 0;
}
