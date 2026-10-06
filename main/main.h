#include <iostream>
#include <vector>

#include "HopDong.h"
#include "KhachHang.h"
#include "QuanLyLuuTru.h"

using namespace std;

void menuKhachHang() {
    QuanLyLuuTru<KhachHang> luuTru("data/khach_hang.txt");
    vector<KhachHang> danhSach;

    luuTru.docTuFile(danhSach);

    int luaChon;

    do {
        cout << "\n========== QUAN LY KHACH HANG ==========\n";
        cout << "1. Them khach hang\n";
        cout << "2. Hien thi danh sach\n";
        cout << "3. Tim khach hang theo ma\n";
        cout << "4. Sua khach hang\n";
        cout << "5. Xoa khach hang\n";
        cout << "0. Quay lai\n";
        cout << "========================================\n";
        cout << "Nhap lua chon: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1) {
            KhachHang khachHang;
            khachHang.nhapThongTin();

            bool trung = false;

            for (const auto& kh : danhSach) {
                if (kh.getMaDinhDanh() == khachHang.getMaDinhDanh()) {
                    trung = true;
                    break;
                }
            }

            if (trung) {
                cout << " -> Loi: Ma khach hang da ton tai!\n";
            } else {
                danhSach.push_back(khachHang);
                luuTru.luuVaoFile(danhSach);
                cout << " -> Them thanh cong!\n";
            }
        }

        else if (luaChon == 2) {
            cout << "\n========== DANH SACH KHACH HANG ==========\n";

            if (danhSach.empty()) {
                cout << "Danh sach rong!\n";
            } else {
                for (const auto& kh : danhSach) {
                    kh.hienThiThongTin();
                }
            }
        }

        else if (luaChon == 3) {
            string ma;
            cout << "Nhap ma khach hang can tim: ";
            getline(cin, ma);

            bool timThay = false;

            for (const auto& kh : danhSach) {
                if (kh.getMaDinhDanh() == ma) {
                    kh.hienThiThongTin();
                    timThay = true;
                    break;
                }
            }

            if (!timThay) {
                cout << " -> Khong tim thay khach hang!\n";
            }
        }

        else if (luaChon == 4) {
            string ma;
            cout << "Nhap ma khach hang can sua: ";
            getline(cin, ma);

            bool timThay = false;

            for (auto& kh : danhSach) {
                if (kh.getMaDinhDanh() == ma) {
                    cout << "\nNhap thong tin moi:\n";

                    // Giữ nguyên mã cũ
                    string maCu = kh.getMaDinhDanh();

                    kh.nhapThongTin();

                    // Không cho đổi mã
                    if (kh.getMaDinhDanh() != maCu) {
                        cout << " -> Khong duoc thay doi ma khach hang!\n";
                    }

                    timThay = true;
                    break;
                }
            }

            if (timThay) {
                luuTru.luuVaoFile(danhSach);
                cout << " -> Cap nhat thanh cong!\n";
            } else {
                cout << " -> Khong tim thay khach hang!\n";
            }
        }

        else if (luaChon == 5) {
            string ma;
            cout << "Nhap ma khach hang can xoa: ";
            getline(cin, ma);

            bool timThay = false;

            for (auto it = danhSach.begin(); it != danhSach.end(); ++it) {
                if (it->getMaDinhDanh() == ma) {
                    danhSach.erase(it);
                    luuTru.luuVaoFile(danhSach);

                    cout << " -> Xoa thanh cong!\n";
                    timThay = true;
                    break;
                }
            }

            if (!timThay) {
                cout << " -> Khong tim thay khach hang!\n";
            }
        }

    } while (luaChon != 0);
}


void menuHopDong() {
    QuanLyLuuTru<HopDong> luuTru("data/hop_dong.txt");
    vector<HopDong> danhSach;

    luuTru.docTuFile(danhSach);

    int luaChon;

    do {
        cout << "\n========== QUAN LY HOP DONG ==========\n";
        cout << "1. Them hop dong\n";
        cout << "2. Hien thi danh sach\n";
        cout << "3. Tim hop dong theo ma\n";
        cout << "4. Sua hop dong\n";
        cout << "5. Xoa hop dong\n";
        cout << "0. Quay lai\n";
        cout << "======================================\n";
        cout << "Nhap lua chon: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1) {
            HopDong hopDong;
            hopDong.nhapThongTin();

            bool trung = false;

            for (const auto& hd : danhSach) {
                if (hd.getMaDinhDanh() == hopDong.getMaDinhDanh()) {
                    trung = true;
                    break;
                }
            }

            if (trung) {
                cout << " -> Loi: Ma hop dong da ton tai!\n";
            } else {
                danhSach.push_back(hopDong);
                luuTru.luuVaoFile(danhSach);
                cout << " -> Them thanh cong!\n";
            }
        }

        else if (luaChon == 2) {
            cout << "\n========== DANH SACH HOP DONG ==========\n";

            if (danhSach.empty()) {
                cout << "Danh sach rong!\n";
            } else {
                for (const auto& hd : danhSach) {
                    hd.hienThiThongTin();
                }
            }
        }

        else if (luaChon == 3) {
            string ma;
            cout << "Nhap ma hop dong can tim: ";
            getline(cin, ma);

            bool timThay = false;

            for (const auto& hd : danhSach) {
                if (hd.getMaDinhDanh() == ma) {
                    hd.hienThiThongTin();
                    timThay = true;
                    break;
                }
            }

            if (!timThay) {
                cout << " -> Khong tim thay hop dong!\n";
            }
        }

        else if (luaChon == 4) {
            string ma;
            cout << "Nhap ma hop dong can sua: ";
            getline(cin, ma);

            bool timThay = false;

            for (auto& hd : danhSach) {
                if (hd.getMaDinhDanh() == ma) {
                    cout << "\nNhap thong tin moi:\n";

                    string maCu = hd.getMaDinhDanh();

                    hd.nhapThongTin();

                    if (hd.getMaDinhDanh() != maCu) {
                        cout << " -> Khong duoc thay doi ma hop dong!\n";
                    }

                    timThay = true;
                    break;
                }
            }

            if (timThay) {
                luuTru.luuVaoFile(danhSach);
                cout << " -> Cap nhat thanh cong!\n";
            } else {
                cout << " -> Khong tim thay hop dong!\n";
            }
        }

        else if (luaChon == 5) {
            string ma;
            cout << "Nhap ma hop dong can xoa: ";
            getline(cin, ma);

            bool timThay = false;

            for (auto it = danhSach.begin(); it != danhSach.end(); ++it) {
                if (it->getMaDinhDanh() == ma) {
                    danhSach.erase(it);
                    luuTru.luuVaoFile(danhSach);

                    cout << " -> Xoa thanh cong!\n";
                    timThay = true;
                    break;
                }
            }

            if (!timThay) {
                cout << " -> Khong tim thay hop dong!\n";
            }
        }

    } while (luaChon != 0);
}


int main() {
    int luaChon;

    do {
        cout << "\n========================================\n";
        cout << "     QUAN LY HE THONG INTERNET FTTH\n";
        cout << "========================================\n";
        cout << "1. Quan ly Khach hang\n";
        cout << "2. Quan ly Hop dong\n";
        cout << "0. Thoat\n";
        cout << "========================================\n";
        cout << "Nhap lua chon: ";
        cin >> luaChon;
        cin.ignore();

        if (luaChon == 1) {
            menuKhachHang();
        }
        else if (luaChon == 2) {
            menuHopDong();
        }

    } while (luaChon != 0);

    cout << "\nDa thoat chuong trinh!\n";

    return 0;
}
