#include <iostream>
#include <string>

// Include các file cơ sở và tiện ích
#include "include/LopCoSo.h"
#include "include/NhapDuLieu.h"
#include "include/QuanlyLuuTru.h"

// Include 2 Use Case của bạn
#include "include/PhieuLapDat.h"
#include "include/PhieuSuaChua.h"

using namespace std;

void hienThiMenuChinh() {
    cout << "\n=========================================================\n";
    cout << "     HE THONG QUAN LY DICH VU INTERNET FTTH - NHOM 09     \n";
    cout << "=========================================================\n";
    cout << " 1. Quan ly Phieu Lap Dat (Use Case UC07 - phieu_lap_dat.txt)\n";
    cout << " 2. Quan ly Phieu Sua Chua (Use Case UC08 - phieu_sua_chua.txt)\n";
    cout << " 0. Thoat chuong trinh\n";
    cout << "=========================================================\n";
    cout << "Nhap lua chon cua ban (0-2): ";
}

int main() {
    // Khởi tạo bộ lưu trữ cho 2 file dữ liệu tương ứng trong thư mục data/
    QuanLyLuuTru<PhieuLapDat> qlPhieuLapDat("data/phieu_lap_dat.txt");
    QuanLyLuuTru<PhieuSuaChua> qlPhieuSuaChua("data/phieu_sua_chua.txt");

    int luaChon;
    do {
        hienThiMenuChinh();
        luaChon = NhapDuLieu::nhapSoNguyenDuong("");

        switch (luaChon) {
            case 1: {
                int chonLD;
                do {
                    cout << "\n--- MENU QUAN LY PHIEU LAP DAT ---\n";
                    cout << "1. Xem danh sach phieu lap dat\n";
                    cout << "2. Them moi phieu lap dat\n";
                    cout << "3. Cap nhat phieu lap dat\n";
                    cout << "4. Xoa phieu lap dat\n";
                    cout << "5. Tim kiem phieu lap dat theo ma\n";
                    cout << "0. Quay lai menu chinh\n";
                    cout << "Lua chon cua ban: ";
                    chonLD = NhapDuLieu::nhapSoNguyenDuong("");

                    if (chonLD == 1) {
                        cout << "\n--- DANH SACH PHIEU LAP DAT ---\n";
                        qlPhieuLapDat.hienThi();
                    } 
                    else if (chonLD == 2) {
                        cout << "\n--- THEM MOI PHIEU LAP DAT ---\n";
                        PhieuLapDat pMoi;
                        pMoi.nhapThongTin();
                        qlPhieuLapDat.themMoi(pMoi);
                    } 
                    else if (chonLD == 3) {
                        cout << "\n--- CAP NHAT PHIEU LAP DAT ---\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu lap dat can cap nhat: ");
                        qlPhieuLapDat.capNhat(ma);
                    } 
                    else if (chonLD == 4) {
                        cout << "\n--- XOA PHIEU LAP DAT ---\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu lap dat can xoa: ");
                        if (NhapDuLieu::xacNhan("Ban co chac muon xoa phieu nay?")) {
                            qlPhieuLapDat.xoaTheoMa(ma);
                        }
                    } 
                    else if (chonLD == 5) {
                        cout << "\n--- TIM KIEM PHIEU LAP DAT ---\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu lap dat can tim: ");
                        PhieuLapDat* p = qlPhieuLapDat.timTheoMa(ma);
                        if (p != nullptr) {
                            cout << " -> Tim thay:\n";
                            p->hienThiThongTin();
                        } else {
                            cout << " -> Khong tim thay phieu co ma: " << ma << "\n";
                        }
                    }
                } while (chonLD != 0);
                break;
            }
            case 2: {
                int chonSC;
                do {
                    cout << "\n--- MENU QUAN LY PHIEU SUA CHUA ---\n";
                    cout << "1. Xem danh sach phieu sua chua\n";
                    cout << "2. Them moi phieu sua chua\n";
                    cout << "3. Cap nhat phieu sua chua\n";
                    cout << "4. Xoa phieu sua chua\n";
                    cout << "5. Tim kiem phieu sua chua theo ma\n";
                    cout << "0. Quay lai menu chinh\n";
                    cout << "Lua chon cua ban: ";
                    chonSC = NhapDuLieu::nhapSoNguyenDuong("");

                    if (chonSC == 1) {
                        cout << "\n--- DANH SACH PHIEU SUA CHUA ---\n";
                        qlPhieuSuaChua.hienThi();
                    } 
                    else if (chonSC == 2) {
                        cout << "\n--- THEM MOI PHIEU SUA CHUA ---\n";
                        PhieuSuaChua pMoi;
                        pMoi.nhapThongTin();
                        qlPhieuSuaChua.themMoi(pMoi);
                    } 
                    else if (chonSC == 3) {
                        cout << "\n--- CAP NHAT PHIEU SUA CHUA ---\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu sua chua can cap nhat: ");
                        qlPhieuSuaChua.capNhat(ma);
                    } 
                    else if (chonSC == 4) {
                        cout << "\n--- XOA PHIEU SUA CHUA ---\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu sua chua can xoa: ");
                        if (NhapDuLieu::xacNhan("Ban co chac muon xoa phieu nay?")) {
                            qlPhieuSuaChua.xoaTheoMa(ma);
                        }
                    } 
                    else if (chonSC == 5) {
                        cout << "\n--- TIM KIEM PHIEU SUA CHUA ---\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu sua chua can tim: ");
                        PhieuSuaChua* p = qlPhieuSuaChua.timTheoMa(ma);
                        if (p != nullptr) {
                            cout << " -> Tim thay:\n";
                            p->hienThiThongTin();
                        } else {
                            cout << " -> Khong tim thay phieu co ma: " << ma << "\n";
                        }
                    }
                } while (chonSC != 0);
                break;
            }
            case 0:
                cout << "\nCam on ban da su dung chuong trinh. Tam biet!\n";
                break;
            default:
                cout << "\n[!] Lua chon khong hop le! Vui long nhap tu 0 den 2.\n";
        }

        if (luaChon != 0) {
            cout << "\nNhan Enter de tiep tuc...";
            cin.get();
        }

    } while (luaChon != 0);

    return 0;
}
