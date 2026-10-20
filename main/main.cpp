/* =======================================================
- Tên tác giả: Đinh Văn Trường
- Mô tả file: Chương trình chính (main.cpp) tổng hợp toàn bộ
- ======================================================= */

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include "QuanLyLuuTru.h"
#include "GoiInternet.h"
#include "TuyenCap.h"
#include "KhachHang.h"
#include "NhanVienKyThuat.h"
#include "HopDong.h"
#include "ThietBi.h"
#include "CongOLT.h"
#include "PhieuLapDat.h"
#include "PhieuSuaChua.h"
#include "HoaDonCuoc.h"

// Hàm nhập số nguyên chuẩn xác, chặn tuyệt đối trường hợp gõ số kèm chữ hoặc ký tự phía sau (vd: 8vshjkdj)
int nhapSoTrongKhoang(int minVal, int maxVal, const std::string& thongBao) {
    std::string line;
    while (true) {
        std::cout << thongBao;
        if (std::getline(std::cin, line)) {
            if (line.empty()) {
                std::cout << " -> Loi: Khong duoc de trong! Vui long nhap lai.\n";
                continue;
            }

            std::stringstream ss(line);
            int giaTri;
            char extra;

            // Đọc số nguyên, sau đó kiểm tra xem sau số đó còn bất kỳ ký tự nào khác không
            if (ss >> giaTri && !(ss >> extra)) {
                if (giaTri >= minVal && giaTri <= maxVal) {
                    return giaTri;
                }
            }
        }
        std::cout << " -> Loi: Vui long chi nhap so nguyen tu " << minVal << " den " << maxVal << "!\n";
    }
}

void hienThiMenuTong() {
    std::cout << "\n=========================================================\n";
    std::cout << "         HE THONG QUAN LY THUE BAO & HA TANG VIEN THONG   \n";
    std::cout << "=========================================================\n";
    std::cout << " 1. Quan ly Goi cuoc Internet (UC01 - Tran Ngoc Huy)\n";
    std::cout << " 2. Quan ly Tuyen cap / Khu vuc (UC02 - Tran Ngoc Huy)\n";
    std::cout << " 3. Quan ly Khach hang (UC03 - Hoang Trong Khoa)\n";
    std::cout << " 4. Quan ly Nhan vien ky thuat (UC04 - Hoang Trong Khoa)\n";
    std::cout << " 5. Quan ly Hop dong thue bao (UC05 - Hoang Trong Khoa)\n";
    std::cout << " 6. Quan ly Thiet bi Modem/ONT (UC05 - Nguyen Nhu Minh)\n";
    std::cout << " 7. Quan ly Cong OLT (UC06 - Nguyen Nhu Minh)\n";
    std::cout << " 8. Quan ly Phieu lap dat (UC07 - Dinh Van Truong)\n";
    std::cout << " 9. Quan ly Phieu sua chua (UC08 - Dinh Van Truong)\n";
    std::cout << " 10. Quan ly Hoa don cuoc (UC09 - Nguyen Duc Huy)\n";
    std::cout << " 11. Thong ke va Xem tong hop du lieu he thong\n";
    std::cout << " 0. Thoat chuong trinh\n";
    std::cout << "=========================================================\n";
}

template <typename T>
void menuCRUD(QuanLyLuuTru<T>& ql, const std::string& tenPhanHe) {
    int chon;
    do {
        std::cout << "\n--- QUAN LY " << tenPhanHe << " ---\n";
        std::cout << "1. Hien thi danh sach\n";
        std::cout << "2. Them moi doi tuong\n";
        std::cout << "3. Tim kiem theo ma\n";
        std::cout << "4. Cap nhat thong tin theo ma\n";
        std::cout << "5. Xoa theo ma\n";
        std::cout << "6. Luu du lieu vao file\n";
        std::cout << "0. Quay lai menu chinh\n";

        chon = nhapSoTrongKhoang(0, 6, "Chon chuc nang (0-6): ");
        std::cout << "\n";

        switch (chon) {
        case 1: {
            ql.hienThi();
            break;
        }
        case 2: {
            T obj;
            obj.nhapThongTin();

            if (obj.getMaDinhDanh().empty()) {
                std::cout << " -> Thao tac da duoc huy, khong them vao danh sach.\n";
            }
            else {
                ql.themMoi(obj);
            }
            break;
        }
        case 3: {
            std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma dinh danh can tim: ");
            T* p = ql.timTheoMa(ma);
            if (p != nullptr) {
                std::cout << " -> Tim thay:\n";
                p->hienThiThongTin();
            }
            else {
                std::cout << " -> Khong tim thay doi tuong co ma: " << ma << "\n";
            }
            break;
        }
        case 4: {
            std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma dinh danh can cap nhat: ");
            ql.capNhat(ma);
            break;
        }
        case 5: {
            std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma dinh danh can xoa: ");
            if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa doi tuong nay?")) {
                ql.xoaTheoMa(ma);
            }
            else {
                std::cout << " -> Da huy thao tac xoa.\n";
            }
            break;
        }
        case 6: {
            if (ql.luuVaoFile()) {
                std::cout << " -> Luu file thanh cong!\n";
            }
            else {
                std::cout << " -> Loi khi ghi file!\n";
            }
            break;
        }
        case 0:
            return;
        }

        if (chon != 0) {
            std::cout << "\nNhan Enter de tiep tuc...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    } while (chon != 0);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    QuanLyLuuTru<GoiInternet>     qlGoiInternet("data/goi_internet.txt");
    qlGoiInternet.docTuFile();

    QuanLyLuuTru<TuyenCap>        qlTuyenCap("data/tuyen_cap.txt");
    qlTuyenCap.docTuFile();

    QuanLyLuuTru<KhachHang>       qlKhachHang("data/khach_hang.txt");
    qlKhachHang.docTuFile();

    QuanLyLuuTru<NhanVienKyThuat> qlNhanVien("data/nhan_vien.txt");
    qlNhanVien.docTuFile();

    QuanLyLuuTru<HopDong>         qlHopDong("data/hop_dong.txt");
    qlHopDong.docTuFile();

    QuanLyLuuTru<ThietBi>         qlThietBi("data/thiet_bi.txt");
    qlThietBi.docTuFile();

    QuanLyLuuTru<CongOLT>         qlOLT("data/cong_olt.txt");
    qlOLT.docTuFile();

    QuanLyLuuTru<PhieuLapDat>     qlLapDat("data/phieu_lap_dat.txt");
    qlLapDat.docTuFile();

    QuanLyLuuTru<PhieuSuaChua>    qlSuaChua("data/phieu_sua_chua.txt");
    qlSuaChua.docTuFile();

    QuanLyLuuTru<HoaDonCuoc>      qlHoaDon("data/hoa_don_cuoc.txt");
    qlHoaDon.docTuFile();

    CongOLT::setDBKhachHang(&qlKhachHang);

    int luaChon;
    do {
        hienThiMenuTong();
        luaChon = nhapSoTrongKhoang(0, 11, "Nhap lua chon cua ban (0-11): ");
        std::cout << "\n";

        switch (luaChon) {
        case 1: menuCRUD(qlGoiInternet, "GOI INTERNET (UC01 - Tran Ngoc Huy)"); break;
        case 2: menuCRUD(qlTuyenCap, "TUYEN CAP (UC02 - Tran Ngoc Huy)"); break;
        case 3: menuCRUD(qlKhachHang, "KHACH HANG (UC03 - Hoang Trong Khoa)"); break;
        case 4: menuCRUD(qlNhanVien, "NHAN VIEN KY THUAT (UC04 - Hoang Trong Khoa)"); break;
        case 5: menuCRUD(qlHopDong, "HOP DONG THUE BAO (UC05 - Hoang Trong Khoa)"); break;
        case 6: menuCRUD(qlThietBi, "THIET BI MODEM/ONT (UC05 - Nguyen Nhu Minh)"); break;
        case 7: menuCRUD(qlOLT, "CONG OLT (UC06 - Nguyen Nhu Minh)"); break;
        case 8: menuCRUD(qlLapDat, "PHIEU LAP DAT (UC07 - Dinh Van Truong)"); break;
        case 9: menuCRUD(qlSuaChua, "PHIEU SUA CHUA (UC08 - Dinh Van Truong)"); break;
        case 10: menuCRUD(qlHoaDon, "HOA DON CUOC (UC09 - Nguyen Duc Huy)"); break;
        case 11: {
            std::cout << "--- THONG KE TONG HOP DU LIEU ---\n";
            std::cout << " - Tong so Goi cuoc: " << qlGoiInternet.getDanhSach().size() << "\n";
            std::cout << " - Tong so Tuyen cap: " << qlTuyenCap.getDanhSach().size() << "\n";
            std::cout << " - Tong so Khach hang: " << qlKhachHang.getDanhSach().size() << "\n";
            std::cout << " - Tong so Nhan vien: " << qlNhanVien.getDanhSach().size() << "\n";
            std::cout << " - Tong so Hop dong: " << qlHopDong.getDanhSach().size() << "\n";
            std::cout << " - Tong so Thiet bi: " << qlThietBi.getDanhSach().size() << "\n";
            std::cout << " - Tong so Cong OLT: " << qlOLT.getDanhSach().size() << "\n";
            std::cout << " - Tong so Phieu lap dat: " << qlLapDat.getDanhSach().size() << "\n";
            std::cout << " - Tong so Phieu sua chua: " << qlSuaChua.getDanhSach().size() << "\n";
            std::cout << " - Tong so Hoa don: " << qlHoaDon.getDanhSach().size() << "\n";
            break;
        }
        case 0: {
            std::cout << " -> Cam on ban da su dung chuong trinh. Tam biet!\n";
            break;
        }
        }

        if (luaChon != 0) {
            std::cout << "\nNhan Enter de tiep tuc...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }

    } while (luaChon != 0);

    return 0;
}
