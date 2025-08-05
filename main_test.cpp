#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <stdexcept>
#include <csignal>

// Include manager classes
#include "managers/quanlylop.h"
#include "managers/quanlymonhoc.h"
#include "managers/quanlycauhoi.h"
#include "managers/quanlysinhvien.h"
#include "managers/quanlydiem.h"

// Include model classes
#include "models/lop.h"
#include "models/monhoc.h"
#include "models/cauhoi.h"
#include "models/sinhvien.h"
#include "models/diemthi.h"

// Include utility classes
#include "utils/DynamicArray.h"

using namespace std;

void signalHandler(int signal)
{
    std::cerr << "Segmentation fault caught! Signal: " << signal << std::endl;
    std::exit(EXIT_FAILURE); // or handle as needed
}

// Global managers
QuanLyLop *quanLyLop = nullptr;
QuanLyMonHoc *quanLyMonHoc = nullptr;
QuanLySinhVien *quanLySinhVien = nullptr;

// Current user session
bool isTeacher = false;
SinhVien *currentStudent = nullptr;

// Utility functions
void clearScreen()
{
    // system("cls");
}

void pauseScreen()
{
    cout << "\nNhan phim bat ky de tiep tuc...";
    _getch();
}

void printHeader(const string &title)
{
    clearScreen();
    cout << "========================================\n";
    cout << "   " << title << "\n";
    cout << "========================================\n\n";
}

void printSeparator()
{
    cout << "----------------------------------------\n";
}

// Authentication functions
bool authenticateTeacher()
{
    string username, password;
    cout << "Dang nhap Giao vien:\n";
    cout << "Ten dang nhap: ";
    cin >> username;
    cout << "Mat khau: ";
    cin >> password;

    return (username == "GV" && password == "GV");
}

bool authenticateStudent()
{
    string masv, password;
    cout << "Dang nhap Sinh vien:\n";
    cout << "Ma sinh vien: ";
    cin >> masv;
    cout << "Mat khau: ";
    cin >> password;

    // Find student in all classes
    DynamicArray<Lop *> danhSachLop;
    quanLyLop->danhSach(danhSachLop);
    for (int i = 0; i < danhSachLop.size(); i++)
    {
        Lop *lop = danhSachLop.get(i);
        SinhVien *sv = lop->getQuanLySinhVien()->tim(masv);
        if (sv && sv->getPassword() == password)
        {
            currentStudent = sv;
            return true;
        }
    }
    return false;
}

// Teacher menu functions
void menuQuanLyLop()
{
    int choice;
    do
    {
        printHeader("QUAN LY LOP");
        cout << "1. Xem danh sach lop\n";
        cout << "2. Them lop moi\n";
        cout << "3. Sua thong tin lop\n";
        cout << "4. Xoa lop\n";
        cout << "5. Quan ly sinh vien trong lop\n";
        cout << "0. Quay lai\n";
        cout << "Lua chon: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            printHeader("DANH SACH LOP");
            DynamicArray<Lop *> danhSach;
            quanLyLop->danhSach(danhSach);
            if (danhSach.size() == 0)
            {
                cout << "Chua co lop nao!\n";
            }
            else
            {
                cout << left << setw(10) << "Ma Lop" << setw(30) << "Ten Lop" << setw(15) << "So SV" << endl;
                printSeparator();
                for (int i = 0; i < danhSach.size(); i++)
                {
                    Lop *lop = danhSach.get(i);
                    cout << left << setw(10) << lop->getMaLop()
                         << setw(30) << lop->getTenLop()
                         << setw(15) << lop->getQuanLySinhVien()->size() << endl;
                }
            }
            pauseScreen();
            break;
        }
        case 2:
        {
            printHeader("THEM LOP MOI");
            string maLop, tenLop;
            cout << "Ma lop: ";
            cin >> maLop;
            cin.ignore();
            cout << "Ten lop: ";
            getline(cin, tenLop);

            Lop *lopMoi = new Lop(maLop, tenLop);
            if (quanLyLop->them(*lopMoi))
            {
                cout << "Them lop thanh cong!\n";
                quanLyLop->saveToFile();
            }
            else
            {
                cout << "Them lop that bai! (Ma lop da ton tai)\n";
            }
            pauseScreen();
            break;
        }
        case 3:
        {
            printHeader("SUA THONG TIN LOP");
            string maLop;
            cout << "Ma lop can sua: ";
            cin >> maLop;

            Lop *lop = quanLyLop->tim(maLop);
            if (lop)
            {
                string tenLopMoi;
                cin.ignore();
                cout << "Ten lop moi (hien tai: " << lop->getTenLop() << "): ";
                getline(cin, tenLopMoi);

                lop->setTenLop(tenLopMoi);
                cout << "Sua thong tin lop thanh cong!\n";
                quanLyLop->saveToFile();
            }
            else
            {
                cout << "Khong tim thay lop!\n";
            }
            pauseScreen();
            break;
        }
        case 4:
        {
            printHeader("XOA LOP");
            string maLop;
            cout << "Ma lop can xoa: ";
            cin >> maLop;

            if (quanLyLop->xoa(maLop))
            {
                cout << "Xoa lop thanh cong!\n";
                quanLyLop->saveToFile();
            }
            else
            {
                cout << "Xoa lop that bai! (Lop khong ton tai)\n";
            }
            pauseScreen();
            break;
        }
        case 5:
        {
            printHeader("QUAN LY SINH VIEN");
            string maLop;
            cout << "Ma lop: ";
            cin >> maLop;

            Lop *lop = quanLyLop->tim(maLop);
            if (lop)
            {
                // Sub-menu for student management
                int svChoice;
                do
                {
                    printHeader("QUAN LY SINH VIEN - LOP " + lop->getTenLop());
                    cout << "1. Xem danh sach sinh vien\n";
                    cout << "2. Them sinh vien\n";
                    cout << "3. Sua thong tin sinh vien\n";
                    cout << "4. Xoa sinh vien\n";
                    cout << "0. Quay lai\n";
                    cout << "Lua chon: ";
                    cin >> svChoice;

                    switch (svChoice)
                    {
                    case 1:
                    {
                        printHeader("DANH SACH SINH VIEN - " + lop->getTenLop());
                        DynamicArray<SinhVien *> dsSV;
                        lop->getQuanLySinhVien()->danhSach(dsSV);
                        if (dsSV.size() == 0)
                        {
                            cout << "Lop chua co sinh vien nao!\n";
                        }
                        else
                        {
                            cout << left << setw(12) << "MASV" << setw(20) << "Ho"
                                 << setw(15) << "Ten" << setw(10) << "Phai" << endl;
                            printSeparator();
                            for (int i = 0; i < dsSV.size(); i++)
                            {
                                SinhVien *sv = dsSV.get(i);
                                cout << left << setw(12) << sv->getMaSinhVien()
                                     << setw(20) << sv->getHo()
                                     << setw(15) << sv->getTen()
                                     << setw(10) << sv->getPhaiBangChu() << endl;
                            }
                        }
                        pauseScreen();
                        break;
                    }
                    case 2:
                    {
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

                        SinhVien *svMoi = new SinhVien(masv, ho, ten, phai, password);
                        if (lop->getQuanLySinhVien()->them(*svMoi))
                        {
                            cout << "Them sinh vien thanh cong!\n";
                            lop->getQuanLySinhVien()->saveToFile();
                        }
                        else
                        {
                            cout << "Them sinh vien that bai! (MASV da ton tai)\n";
                        }
                        pauseScreen();
                        break;
                    }
                    }
                } while (svChoice != 0);
            }
            else
            {
                cout << "Khong tim thay lop!\n";
                pauseScreen();
            }
            break;
        }
        }
    } while (choice != 0);
}

void menuQuanLyMonHoc()
{
    std::signal(SIGSEGV, signalHandler); // Catch segmentation faults
    int choice;
    do
    {
        printHeader("QUAN LY MON HOC");
        cout << "1. Xem danh sach mon hoc\n";
        cout << "2. Them mon hoc moi\n";
        cout << "3. Sua thong tin mon hoc\n";
        cout << "4. Xoa mon hoc\n";
        cout << "5. Quan ly cau hoi theo mon\n";
        cout << "0. Quay lai\n";
        cout << "Lua chon: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            printHeader("DANH SACH MON HOC");
            DynamicArray<MonHoc *> danhSach;
            quanLyMonHoc->danhSach(danhSach);
            if (danhSach.size() == 0)
            {
                cout << "Chua co mon hoc nao!\n";
            }
            else
            {
                cout << left << setw(10) << "Ma Mon" << setw(40) << "Ten Mon" << setw(15) << "So Cau Hoi" << endl;
                printSeparator();

                MonHoc *mon = danhSach.get(0);

                for (int i = 0; i < danhSach.size(); i++)
                {
                    MonHoc *mon = danhSach.get(i);
                    cout << left << setw(10) << mon->getMaMon()
                         << setw(40) << mon->getTenMon()
                         << setw(15) << mon->getQuanLyCauHoi()->size() << endl;
                }
            }
            pauseScreen();
            break;
        }
        case 2:
        {
            printHeader("THEM MON HOC MOI");
            string maMon, tenMon;
            cout << "Ma mon hoc: ";
            cin >> maMon;
            cin.ignore();
            cout << "Ten mon hoc: ";
            getline(cin, tenMon);

            MonHoc *monMoi = new MonHoc(maMon.c_str(), tenMon);
            if (quanLyMonHoc->them(*monMoi))
            {
                cout << "Them mon hoc thanh cong!\n";
                quanLyMonHoc->saveToFile();
            }
            else
            {
                cout << "Them mon hoc that bai! (Ma mon da ton tai)\n";
            }
            pauseScreen();
            break;
        }
        case 5:
        {
            printHeader("QUAN LY CAU HOI");
            string maMon;
            cout << "Ma mon hoc: ";
            cin >> maMon;

            MonHoc *mon = quanLyMonHoc->tim(maMon.c_str());
            if (mon)
            {
                int chChoice;
                do
                {
                    printHeader("QUAN LY CAU HOI - " + mon->getTenMon());
                    cout << "1. Xem danh sach cau hoi\n";
                    cout << "2. Them cau hoi moi\n";
                    cout << "3. Sua cau hoi\n";
                    cout << "4. Xoa cau hoi\n";
                    cout << "0. Quay lai\n";
                    cout << "Lua chon: ";
                    cin >> chChoice;

                    switch (chChoice)
                    {
                    case 1:
                    {
                        printHeader("DANH SACH CAU HOI - " + mon->getTenMon());
                        DynamicArray<CauHoi *> dsCH;
                        mon->getQuanLyCauHoi()->danhSach(dsCH);
                        if (dsCH.size() == 0)
                        {
                            cout << "Mon hoc chua co cau hoi nao!\n";
                        }
                        else
                        {
                            for (int i = 0; i < dsCH.size(); i++)
                            {
                                CauHoi *ch = dsCH.get(i);
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
                    case 2:
                    {
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
                        CauHoi *cauHoiMoi = new CauHoi(maCauHoi, noiDung, luaChonA, luaChonB, luaChonC, luaChonD, dapAn);

                        if (mon->getQuanLyCauHoi()->them(*cauHoiMoi))
                        {
                            cout << "Them cau hoi thanh cong! (ID: " << maCauHoi << ")\n";
                            mon->getQuanLyCauHoi()->saveToFile();
                        }
                        else
                        {
                            cout << "Them cau hoi that bai!\n";
                        }
                        pauseScreen();
                        break;
                    }
                    }
                } while (chChoice != 0);
            }
            else
            {
                cout << "Khong tim thay mon hoc!\n";
                pauseScreen();
            }
            break;
        }
        }
    } while (choice != 0);
}

void menuBaoCao()
{
    int choice;
    do
    {
        printHeader("BAO CAO VA THONG KE");
        cout << "1. Bang diem theo lop\n";
        cout << "2. Chi tiet bai thi sinh vien\n";
        cout << "3. Thong ke tong quan\n";
        cout << "0. Quay lai\n";
        cout << "Lua chon: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            printHeader("BANG DIEM THEO LOP");
            string maLop;
            cout << "Ma lop: ";
            cin >> maLop;

            Lop *lop = quanLyLop->tim(maLop);
            if (lop)
            {
                DynamicArray<SinhVien *> dsSV;
                lop->getQuanLySinhVien()->danhSach(dsSV);

                // Get available subjects for header
                DynamicArray<MonHoc *> danhSachMon;
                quanLyMonHoc->danhSach(danhSachMon);

                cout << "\nBANG DIEM LOP: " << lop->getTenLop() << endl;
                cout << left << setw(12) << "MASV" << setw(25) << "Ho Ten";

                // Display subject headers (max 3 subjects)
                int maxSubjects = (danhSachMon.size() < 3) ? danhSachMon.size() : 3;
                for (int i = 0; i < maxSubjects; i++)
                {
                    string tenMon = danhSachMon.get(i)->getTenMon();
                    if (tenMon.length() > 12)
                        tenMon = tenMon.substr(0, 12); // Truncate if too long
                    cout << setw(15) << tenMon;
                }

                // Fill remaining columns if less than 3 subjects
                for (int i = maxSubjects; i < 3; i++)
                {
                    cout << setw(15) << "---";
                }

                cout << endl;
                printSeparator();

                for (int i = 0; i < dsSV.size(); i++)
                {
                    SinhVien *sv = dsSV.get(i);
                    cout << left << setw(12) << sv->getMaSinhVien()
                         << setw(25) << sv->getHoTen();

                    // Get available subjects and display scores
                    DynamicArray<MonHoc *> danhSachMon;
                    quanLyMonHoc->danhSach(danhSachMon);

                    // Display scores for first 3 subjects (or available subjects)
                    int maxSubjects = (danhSachMon.size() < 3) ? danhSachMon.size() : 3;
                    for (int j = 0; j < maxSubjects; j++)
                    {
                        MonHoc *mon = danhSachMon.get(j);
                        DiemThi *ketQua = sv->getQuanLyDiem()->tim(mon->getMaMon());

                        if (ketQua)
                        {
                            cout << setw(15) << fixed << setprecision(1) << ketQua->getDiem();
                        }
                        else
                        {
                            cout << setw(15) << "Chua thi";
                        }
                    }

                    // Fill remaining columns if less than 3 subjects
                    for (int j = maxSubjects; j < 3; j++)
                    {
                        cout << setw(15) << "---";
                    }

                    cout << endl;
                }
            }
            else
            {
                cout << "Khong tim thay lop!\n";
            }
            pauseScreen();
            break;
        }
        case 2:
        {
            printHeader("CHI TIET BAI THI SINH VIEN");
            string maSinhVien;
            cout << "Ma sinh vien: ";
            cin >> maSinhVien;

            // Find student in all classes
            SinhVien *sinhVien = nullptr;
            DynamicArray<Lop *> danhSachLop;
            quanLyLop->danhSach(danhSachLop);

            for (int i = 0; i < danhSachLop.size() && !sinhVien; i++)
            {
                sinhVien = danhSachLop.get(i)->getQuanLySinhVien()->tim(maSinhVien);
            }

            if (!sinhVien)
            {
                cout << "Khong tim thay sinh vien!\n";
                pauseScreen();
                break;
            }

            // Display student information
            cout << "\nTHONG TIN SINH VIEN:\n";
            cout << "Ma sinh vien: " << sinhVien->getMaSinhVien() << endl;
            cout << "Ho ten: " << sinhVien->getHoTen() << endl;
            cout << "Gioi tinh: " << sinhVien->getPhaiBangChu() << endl;

            // Get exam results
            DynamicArray<DiemThi *> danhSachKetQua;
            sinhVien->getQuanLyDiem()->danhSach(danhSachKetQua);

            if (danhSachKetQua.size() == 0)
            {
                cout << "\nSinh vien chua thi mon nao!\n";
                pauseScreen();
                break;
            }

            cout << "\nKET QUA THI CHI TIET:\n";
            cout << left << setw(12) << "Ma Mon" << setw(25) << "Ten Mon"
                 << setw(10) << "Diem" << setw(15) << "Ket Qua" << setw(25) << "Cau Tra Loi" << endl;
            printSeparator();

            for (int i = 0; i < danhSachKetQua.size(); i++)
            {
                DiemThi *ketQua = danhSachKetQua.get(i);

                // Find subject name
                string tenMon = "Khong tim thay";
                DynamicArray<MonHoc *> danhSachMon;
                quanLyMonHoc->danhSach(danhSachMon);
                MonHoc *monHoc = quanLyMonHoc->tim(ketQua->getMaMon());
                if (monHoc)
                {
                    tenMon = monHoc->getTenMon();
                }

                // Determine pass/fail status
                string ketQuaText = (ketQua->getDiem() >= 5.0) ? "DAU" : "ROT";

                // Get student answers as string
                string cauTraLoi = "";
                DynamicArray<char> *answers = ketQua->getDanhSachCauTraLoi();
                for (int k = 0; k < answers->size(); k++)
                {
                    cauTraLoi += answers->get(k);
                    if (k < answers->size() - 1)
                        cauTraLoi += ",";
                }

                // Display row
                cout << left << setw(12) << ketQua->getMaMon()
                     << setw(25) << tenMon
                     << setw(10) << fixed << setprecision(2) << ketQua->getDiem()
                     << setw(15) << ketQuaText
                     << setw(25) << cauTraLoi << endl;
            }

            printSeparator();

            // Display statistics
            cout << "\nTHONG KE TONG KET:\n";
            cout << "So mon da thi: " << sinhVien->getQuanLyDiem()->demSoMonDaThi() << endl;
            cout << "Diem trung binh: " << fixed << setprecision(2)
                 << sinhVien->getQuanLyDiem()->tinhDiemTrungBinh() << "/10" << endl;
            cout << "So mon dau: " << sinhVien->getQuanLyDiem()->demSoMonDau() << endl;
            cout << "So mon rot: " << sinhVien->getQuanLyDiem()->demSoMonRot() << endl;

            // Option to view detailed answers for a specific subject
            cout << "\nBan co muon xem chi tiet cau tra loi cho mon nao khong? (y/n): ";
            char choice;
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                cout << "Nhap ma mon hoc: ";
                string maMon;
                cin >> maMon;

                DiemThi *ketQua = sinhVien->getQuanLyDiem()->tim(maMon.c_str());
                if (ketQua)
                {
                    printHeader("CHI TIET CAU TRA LOI - " + maMon);
                    cout << "Sinh vien: " << sinhVien->getHoTen() << endl;
                    cout << "Diem: " << fixed << setprecision(2) << ketQua->getDiem() << "/10" << endl;
                    cout << "\nCau tra loi chi tiet:\n";

                    DynamicArray<char> *answers = ketQua->getDanhSachCauTraLoi();
                    for (int i = 0; i < answers->size(); i++)
                    {
                        cout << "Cau " << (i + 1) << ": " << answers->get(i) << endl;
                    }
                }
                else
                {
                    cout << "Khong tim thay ket qua thi cho mon " << maMon << endl;
                }
            }

            pauseScreen();
            break;
        }
        case 3:
        {
            printHeader("THONG KE TONG QUAN");
            cout << "So lop: " << quanLyLop->size() << endl;
            cout << "So mon hoc: " << quanLyMonHoc->size() << endl;

            int tongSinhVien = 0;
            int tongCauHoi = 0;

            DynamicArray<Lop *> danhSachLop;
            quanLyLop->danhSach(danhSachLop);
            for (int i = 0; i < danhSachLop.size(); i++)
            {
                tongSinhVien += danhSachLop.get(i)->getQuanLySinhVien()->size();
            }

            DynamicArray<MonHoc *> danhSachMon;
            quanLyMonHoc->danhSach(danhSachMon);
            for (int i = 0; i < danhSachMon.size(); i++)
            {
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

void menuGiaoVien()
{
    int choice;
    do
    {
        printHeader("MENU GIAO VIEN");
        cout << "1. Quan ly lop\n";
        cout << "2. Quan ly mon hoc\n";
        cout << "3. Bao cao va thong ke\n";
        cout << "0. Dang xuat\n";
        cout << "Lua chon: ";
        cin >> choice;

        switch (choice)
        {
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
void menuLamBaiThi()
{
    printHeader("LAM BAI THI");

    // Display available subjects
    DynamicArray<MonHoc *> danhSachMon;
    quanLyMonHoc->danhSach(danhSachMon);
    if (danhSachMon.size() == 0)
    {
        cout << "Khong co mon hoc nao de thi!\n";
        pauseScreen();
        return;
    }

    cout << "Chon mon hoc:\n";
    for (int i = 0; i < danhSachMon.size(); i++)
    {
        cout << (i + 1) << ". " << danhSachMon.get(i)->getTenMon()
             << " (" << danhSachMon.get(i)->getQuanLyCauHoi()->size() << " cau hoi)" << endl;
    }

    int monChoice;
    cout << "Lua chon mon: ";
    cin >> monChoice;

    if (monChoice < 1 || monChoice > danhSachMon.size())
    {
        cout << "Lua chon khong hop le!\n";
        pauseScreen();
        return;
    }

    MonHoc &monThi = *danhSachMon.get(monChoice - 1);

    int soCauHoi;
    cout << "So cau hoi muon thi (toi da " << monThi.getQuanLyCauHoi()->size() << "): ";
    cin >> soCauHoi;

    if (soCauHoi <= 0 || soCauHoi > monThi.getQuanLyCauHoi()->size())
    {
        cout << "So cau hoi khong hop le!\n";
        pauseScreen();
        return;
    }

    // Get random questions
    DynamicArray<CauHoi *> dsCauHoi;
    monThi.getQuanLyCauHoi()->layNgauNhien(dsCauHoi, soCauHoi);

    printHeader("BAI THI: " + monThi.getTenMon());
    cout << "Sinh vien: " << currentStudent->getHoTen() << endl;
    cout << "So cau hoi: " << soCauHoi << endl;
    cout << "Thoi gian: 60 phut (demo - khong gioi han)\n";
    printSeparator();

    int diem = 0;
    DynamicArray<char> danhSachCauTraLoi; // Store student's answers

    for (int i = 0; i < dsCauHoi.size(); i++)
    {
        CauHoi &ch = *dsCauHoi.get(i);
        cout << "\nCau " << (i + 1) << ": " << ch.getNoiDung() << endl;
        cout << "A. " << ch.getLuaChonA() << endl;
        cout << "B. " << ch.getLuaChonB() << endl;
        cout << "C. " << ch.getLuaChonC() << endl;
        cout << "D. " << ch.getLuaChonD() << endl;

        char dapAn;
        cout << "Dap an cua ban (A/B/C/D): ";
        cin >> dapAn;

        // Store the student's answer
        danhSachCauTraLoi.add(dapAn);

        if (ch.kiemTraDapAn(dapAn))
        {
            diem++;
            cout << "Dung!\n";
        }
        else
        {
            cout << "Sai! Dap an dung la: " << ch.getDapAnDung() << endl;
        }
    }

    float diemCuoi = (float)diem / soCauHoi * 10;
    cout << "\n"
         << string(50, '=') << endl;
    cout << "KET QUA BAI THI" << endl;
    cout << "Diem: " << fixed << setprecision(2) << diemCuoi << "/10" << endl;
    cout << "So cau dung: " << diem << "/" << soCauHoi << endl;
    cout << string(50, '=') << endl;

    // Save test results to file
    try
    {
        // Create DiemThi object with student's actual answers
        DiemThi *ketQuaThi = new DiemThi(monThi.getMaMon(), diemCuoi);

        for (int i = 0; i < danhSachCauTraLoi.size(); i++)
        {
            ketQuaThi->getDanhSachCauTraLoi()->add(danhSachCauTraLoi.get(i));
        }

        // Check if student already has a score for this subject
        DiemThi *existingScore = currentStudent->getQuanLyDiem()->tim(monThi.getMaMon());

        if (existingScore)
        {
            // Update existing score if new score is better
            if (diemCuoi > existingScore->getDiem())
            {
                currentStudent->getQuanLyDiem()->sua(*ketQuaThi);
                cout << "\nDiem moi cao hon da duoc cap nhat!" << endl;
            }
            else
            {
                cout << "\nDiem cu cao hon, khong cap nhat." << endl;
            }
        }
        else
        {
            // Add new score
            currentStudent->getQuanLyDiem()->them(*ketQuaThi);
            cout << "\nKet qua thi da duoc luu!" << endl;
        }

        // Force save to file immediately
        currentStudent->getQuanLyDiem()->saveToFile();
    }
    catch (const exception &e)
    {
        cout << "\nLoi khi luu ket qua: " << e.what() << endl;
    }

    pauseScreen();
}

void menuXemKetQuaThi()
{
    printHeader("KET QUA THI CUA BAN");

    // Get all exam results for current student
    DynamicArray<DiemThi *> danhSachKetQua;
    currentStudent->getQuanLyDiem()->danhSach(danhSachKetQua);

    if (danhSachKetQua.size() == 0)
    {
        cout << "Ban chua thi mon nao!\n";
        pauseScreen();
        return;
    }

    // Display results table header
    cout << left << setw(12) << "Ma Mon" << setw(25) << "Ten Mon"
         << setw(10) << "Diem" << setw(15) << "Ket Qua" << setw(20) << "Cau Tra Loi" << endl;
    printSeparator();

    double tongDiem = 0.0;
    int soMonDaThi = 0;

    for (int i = 0; i < danhSachKetQua.size(); i++)
    {
        DiemThi *ketQua = danhSachKetQua.get(i);

        // Find subject name
        string tenMon = "Khong tim thay";
        DynamicArray<MonHoc *> danhSachMon;
        quanLyMonHoc->danhSach(danhSachMon);
        MonHoc *monHoc = quanLyMonHoc->tim(ketQua->getMaMon());
        if (monHoc)
        {
            tenMon = monHoc->getTenMon();
        }

        // Determine pass/fail status
        string ketQuaText = (ketQua->getDiem() >= 5.0) ? "DAU" : "ROT";

        // Get student answers as string
        string cauTraLoi = "";
        DynamicArray<char> *answers = ketQua->getDanhSachCauTraLoi();
        for (int k = 0; k < answers->size(); k++)
        {
            cauTraLoi += answers->get(k);
        }

        // Display row
        cout << left << setw(12) << ketQua->getMaMon()
             << setw(25) << tenMon
             << setw(10) << fixed << setprecision(2) << ketQua->getDiem()
             << setw(15) << ketQuaText
             << setw(20) << cauTraLoi << endl;

        tongDiem += ketQua->getDiem();
        soMonDaThi++;
    }

    printSeparator();

    // Display statistics
    cout << "\nTHONG KE TONG KET:\n";
    cout << "So mon da thi: " << soMonDaThi << endl;
    cout << "Diem trung binh: " << fixed << setprecision(2)
         << (soMonDaThi > 0 ? tongDiem / soMonDaThi : 0.0) << "/10" << endl;
    cout << "So mon dau: " << currentStudent->getQuanLyDiem()->demSoMonDau() << endl;
    cout << "So mon rot: " << currentStudent->getQuanLyDiem()->demSoMonRot() << endl;

    // Display detailed view option
    cout << "\nBan co muon xem chi tiet bai thi nao khong? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        cout << "Nhap ma mon hoc: ";
        string maMon;
        cin >> maMon;

        DiemThi *ketQua = currentStudent->getQuanLyDiem()->tim(maMon.c_str());
        if (ketQua)
        {
            printHeader("CHI TIET BAI THI - " + maMon);
            cout << "Diem: " << fixed << setprecision(2) << ketQua->getDiem() << "/10" << endl;
            cout << "Cau tra loi cua ban: ";

            DynamicArray<char> *answers = ketQua->getDanhSachCauTraLoi();
            for (int i = 0; i < answers->size(); i++)
            {
                cout << "Cau " << (i + 1) << ": " << answers->get(i) << "  ";
            }
            cout << endl;
        }
        else
        {
            cout << "Khong tim thay ket qua thi cho mon " << maMon << endl;
        }
    }

    pauseScreen();
}

void menuSinhVien()
{
    int choice;
    do
    {
        printHeader("MENU SINH VIEN");
        cout << "Xin chao, " << currentStudent->getHoTen() << "!\n\n";
        cout << "1. Lam bai thi\n";
        cout << "2. Xem ket qua thi\n";
        cout << "3. Thong tin ca nhan\n";
        cout << "0. Dang xuat\n";
        cout << "Lua chon: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            menuLamBaiThi();
            break;
        case 2:
            menuXemKetQuaThi();
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
void initializeSystem()
{
    cout << "Initializing system...\n";
    // Initialize managers
    quanLyLop = new QuanLyLop();
    quanLyMonHoc = new QuanLyMonHoc();

    // Load data from files
    quanLyLop->loadFromFile();
    quanLyMonHoc->loadFromFile();

    // Load questions for each subject
    DynamicArray<MonHoc *> danhSachMon;
    quanLyMonHoc->danhSach(danhSachMon);
    for (int i = 0; i < danhSachMon.size(); i++)
    {
        danhSachMon.get(i)->getQuanLyCauHoi()->loadFromFile();
    }

    // Load students for each class
    DynamicArray<Lop *> danhSachLop;
    quanLyLop->danhSach(danhSachLop);
    for (int i = 0; i < danhSachLop.size(); i++)
    {
        danhSachLop.get(i)->getQuanLySinhVien()->loadFromFile();

        // Load scores for each student
        DynamicArray<SinhVien *> danhSachSinhVien;
        danhSachLop.get(i)->getQuanLySinhVien()->danhSach(danhSachSinhVien);
        for (int j = 0; j < danhSachSinhVien.size(); j++)
        {
            danhSachSinhVien.get(j)->getQuanLyDiem()->loadFromFile();
        }
    }
}

void cleanupSystem()
{
    if (quanLyLop)
    {
        delete quanLyLop;
        quanLyLop = nullptr;
    }
    if (quanLyMonHoc)
    {
        delete quanLyMonHoc;
        quanLyMonHoc = nullptr;
    }
}

int main()
{
    // Set console to UTF-8 for Vietnamese characters
    SetConsoleOutputCP(CP_UTF8);

    cout << "Khoi tao he thong...\n";
    try
    {
        initializeSystem();
    }
    catch (const std::exception &e)
    {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    int choice;
    do
    {
        printHeader("HE THONG QUAN LY THI TRAC NGHIEM");
        cout << "1. Dang nhap Giao vien\n";
        cout << "2. Dang nhap Sinh vien\n";
        cout << "0. Thoat\n";
        cout << "Lua chon: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (authenticateTeacher())
            {
                isTeacher = true;
                menuGiaoVien();
                isTeacher = false;
            }
            else
            {
                cout << "Dang nhap that bai!\n";
                pauseScreen();
            }
            break;
        case 2:
            if (authenticateStudent())
            {
                menuSinhVien();
                currentStudent = nullptr;
            }
            else
            {
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
