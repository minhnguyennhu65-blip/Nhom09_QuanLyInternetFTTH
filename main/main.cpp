#include <iostream>
#include <string>

// --- Include toàn bộ các module / use case từ thư mục include/ của nhóm ---
#include "include/LopCoSo.h"
#include "include/NhapDuLieu.h"
#include "include/QuanlyLuuTru.h"
#include "include/ThietBi.h"
#include "include/CongOLT.h"
#include "include/GoiInternet.h"
#include "include/TuyenCap.h"
#include "include/KhachHang.h"
#include "include/NhanVienKyThuat.h"
#include "include/HopDong.h"
#include "include/HoaDonCuoc.h"
#include "include/PhieuLapDat.h"    
#include "include/PhieuSuaChua.h"  

using namespace std;

// Hàm hiển thị menu tổng của toàn bộ Bài tập lớn nhóm 09
void hienThiMenuChinh() {
    cout << "\n=================================================================\n";
    cout << "         HE THONG QUAN LY DICH VU INTERNET FTTH - NHOM 09         \n";
    cout << "=================================================================\n";
    cout << " 1. Quan ly Thiet bi (thietbi.h)\n";
    cout << " 2. Quan ly Cong OLT (congolt.h)\n";
    cout << " 3. Quan ly Goi cuoc Internet (goiinternet.h)\n";
    cout << " 4. Quan ly Tuyen cap (tuyencap.h)\n";
    cout << " 5. Quan ly Khach hang (khachhang.h)\n";
    cout << " 6. Quan ly Nhan vien ky thuat (nhanvienkythuat.h)\n";
    cout << " 7. Quan ly Hop dong (hopdong.h)\n";
    cout << " 8. Quan ly Hoa don cuoc (hoadoncuoc.h)\n";
    cout << " 9. Quan ly Phieu lap dat [Use Case cua ban] (phieulapdat.h)\n";
    cout << "10. Quan ly Phieu sua chua [Use Case cua Truong] (phieusuachua.h)\n";
    cout << " 0. Thoat chuong trinh\n";
    cout << "=================================================================\n";
    cout << "Nhap lua chon cua ban (0-10): ";
}

int main() {
    // Khởi tạo các đối tượng quản lý lưu trữ tương ứng cho từng file trong thư mục data/
    QuanLyLuuTru<PhieuLapDat> qlPhieuLapDat("data/phieu_lap_dat.txt");
    QuanLyLuuTru<PhieuSuaChua> qlPhieuSuaChua("data/phieu_sua_chua.txt");
    // Các module khác nếu nhóm đã viết class tương tự, bạn có thể khởi tạo ở đây:
    // QuanLyLuuTru<KhachHang> qlKhachHang("data/khach_hang.txt");

    int luaChon;
    do {
        hienThiMenuChinh();
        luaChon = NhapDuLieu::nhapSoNguyenDuong("");

        switch (luaChon) {
            case 1: {
                cout << "\n--- CHUC NANG: QUAN LY THIET BI ---\n";
                // Gọi menu hoặc hàm xử lý thiết bị của thành viên phụ trách
                break;
            }
            case 2: {
                cout << "\n--- CHUC NANG: QUAN LY CONG OLT ---\n";
                break;
            }
            case 3: {
                cout << "\n--- CHUC NANG: QUAN LY GOI INTERNET ---\n";
                break;
            }
            case 4: {
                cout << "\n--- CHUC NANG: QUAN LY TUYEN CAP ---\n";
                break;
            }
            case 5: {
                cout << "\n--- CHUC NANG: QUAN LY KHACH HANG ---\n";
                break;
            }
            case 6: {
                cout << "\n--- CHUC NANG: QUAN LY NHAN VIEN KY THUAT ---\n";
                break;
            }
            case 7: {
                cout << "\n--- CHUC NANG: QUAN LY HOP DONG ---\n";
                break;
            }
            case 8: {
                cout << "\n--- CHUC NANG: QUAN LY HOA DON CUOC ---\n";
                break;
            }
            case 9: {
                // --- SUB-MENU CHO PHIẾU LẮP ĐẶT CỦA BẠN ---
                int chonLD;
                do {
                    cout << "\n--- MENU QUAN LY PHIEU LAP DAT ---\n";
                    cout << "1. Xem danh sach phieu lap dat\n";
                    cout << "2. Them moi phieu lap dat\n";
                    cout << "3. Cap nhat thong tin phieu lap dat\n";
                    cout << "4. Xoa phieu lap dat\n";
                    cout << "5. Tim kiem phieu lap dat theo ma\n";
                    cout << "0. Quay lai menu chinh\n";
                    cout << "Lua chon cua ban: ";
                    chonLD = NhapDuLieu::nhapSoNguyenDuong("");

                    if (chonLD == 1) {
                        cout << "\n[DANH SACH PHIEU LAP DAT]\n";
                        qlPhieuLapDat.hienThi();
                    } else if (chonLD == 2) {
                        cout << "\n[THEM MOI PHIEU LAP DAT]\n";
                        PhieuLapDat pMoi;
                        pMoi.nhapThongTin();
                        qlPhieuLapDat.themMoi(pMoi);
                    } else if (chonLD == 3) {
                        cout << "\n[CAP NHAT PHIEU LAP DAT]\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu lap dat can cap nhat: ");
                        qlPhieuLapDat.capNhat(ma);
                    } else if (chonLD == 4) {
                        cout << "\n[XOA PHIEU LAP DAT]\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu lap dat can xoa: ");
                        if (NhapDuLieu::xacNhan("Ban co chac muon xoa phieu nay?")) {
                            qlPhieuLapDat.xoaTheoMa(ma);
                        }
                    } else if (chonLD == 5) {
                        cout << "\n[TIM KIEM PHIEU LAP DAT]\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu lap dat can tim: ");
                        PhieuLapDat* p = qlPhieuLapDat.timTheoMa(ma);
                        if (p != nullptr) {
                            p->hienThiThongTin();
                        } else {
                            cout << " -> Khong tìm thay phieu co ma: " << ma << "\n";
                        }
                    }
                } while (chonLD != 0);
                break;
            }
            case 10: {
                // --- SUB-MENU CHO PHIẾU SỬA CHỮA ---
                int chonSC;
                do {
                    cout << "\n--- MENU QUAN LY PHIEU SUA CHUA ---\n";
                    cout << "1. Xem danh sach phieu sua chua\n";
                    cout << "2. Them moi phieu sua chua\n";
                    cout << "3. Cap nhat thong tin phieu sua chua\n";
                    cout << "4. Xoa phieu sua chua\n";
                    cout << "5. Tim kiem phieu sua chua theo ma\n";
                    cout << "0. Quay lai menu chinh\n";
                    cout << "Lua chon cua ban: ";
                    chonSC = NhapDuLieu::nhapSoNguyenDuong("");

                    if (chonSC == 1) {
                        cout << "\n[DANH SACH PHIEU SUA CHUA]\n";
                        qlPhieuSuaChua.hienThi();
                    } else if (chonSC == 2) {
                        cout << "\n[THEM MOI PHIEU SUA CHUA]\n";
                        PhieuSuaChua pMoi;
                        pMoi.nhapThongTin();
                        qlPhieuSuaChua.themMoi(pMoi);
                    } else if (chonSC == 3) {
                        cout << "\n[CAP NHAT PHIEU SUA CHUA]\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu sua chua can cap nhat: ");
                        qlPhieuSuaChua.capNhat(ma);
                    } else if (chonSC == 4) {
                        cout << "\n[XOA PHIEU SUA CHUA]\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu sua chua can xoa: ");
                        if (NhapDuLieu::xacNhan("Ban co chac muon xoa phieu nay?")) {
                            qlPhieuSuaChua.xoaTheoMa(ma);
                        }
                    } else if (chonSC == 5) {
                        cout << "\n[TIM KIEM PHIEU SUA CHUA]\n";
                        string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma phieu sua chua can tim: ");
                        PhieuSuaChua* p = qlPhieuSuaChua.timTheoMa(ma);
                        if (p != nullptr) {
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
                cout << "\n[!] Lua chon khong hop le! Vui long nhap tu 0 den 10.\n";
        }

        if (luaChon != 0) {
            cout << "\nNhan Enter de tiep tuc...";
            cin.get();
        }

    } while (luaChon != 0);

    return 0;
}
