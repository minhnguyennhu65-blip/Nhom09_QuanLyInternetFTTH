/* =======================================================
 * Tên tác giả: Đinh Văn Trường
 * Mô tả file: Chương trình chính (main.cpp) tổng hợp toàn bộ 
 * ======================================================= */

#include <iostream>
#include <string>
#include <vector>
#include <limits>

// Khai báo trọn bộ 13 file header của dự án
#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include "QuanLiLuuTru.h"
#include "GoiInternet.h"
#include "TuyenCap.h"
#include "KhachHang.h"
#include "NhanVienKiThuat.h"
#include "HopDong.h"
#include "ThietBi.h"
#include "CongOLT.h"
#include "PhieuLapDat.h"
#include "PhieuSuaChua.h"
#include "HoaDonCuoc.h"

// Hàm phụ trợ nhập số nguyên trong khoảng [minVal, maxVal] để cho phép nhập số 0 (thoát)
int nhapSoTrongKhoang(int minVal, int maxVal, const std::string& thongBao) {
    int giaTri;
    while (true) {
        std::cout << thongBao;
        if (std::cin >> giaTri && giaTri >= minVal && giaTri <= maxVal) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return giaTri;
        }
        std::cout << " -> Loi: Vui long chi nhap so nguyen tu " << minVal << " den " << maxVal << "!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Hàm hiển thị Menu Tổng của hệ thống BTL Viễn Thông
void hienThiMenuTong() {
    std::cout << "\n=========================================================\n";
    std::cout << "         HE THONG QUAN LY THUE BAO & HA TANG VIEN THONG   \n";
    std::cout << "=========================================================\n";
    std::cout << " 1. Quan ly Goi cuoc Internet (UC01 - Tran Ngoc Huy)\n";
    std::cout << " 2. Quan ly Tuyen cap / Khu vuc (UC02 - Tran Ngoc Huy)\n";
    std::cout << " 3. Quan ly Khach hang (Hoang Trong Khoa)\n";
    std::cout << " 4. Quan ly Nhan vien ky thuat (Nguyen Duc Huy)\n";
    std::cout << " 5. Quan ly Hop dong thue bao (Hoang Trong Khoa)\n";
    std::cout << " 6. Quan ly Thiet bi Modem/ONT (Nguyen Nhu Minh)\n";
    std::cout << " 7. Quan ly Cong OLT (UC06 - Nguyen Nhu Minh)\n";
    std::cout << " 8. Quan ly Phieu lap dat (UC07 - Dinh Van Truong)\n";
    std::cout << " 9. Quan ly Phieu sua chua (UC08 - Dinh Van Truong)\n";
    std::cout << " 10. Quan ly Hoa don cuoc (Nguyen Duc Huy)\n";
    std::cout << " 11. Thong ke va Xem tong hop du lieu he thong\n";
    std::cout << " 0. Thoat chuong trinh\n";
    std::cout << "=========================================================\n";
}

// Template hàm hiển thị menu phụ CRUD chuẩn cho các phân hệ
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
        
        // Sử dụng hàm an toàn cho phép nhận số 0 để quay lại
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
            ql.themMoi(obj);
            break;
        }
        case 3: {
            std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma dinh danh can tim: ");
            T* p = ql.timTheoMa(ma);
            if (p != nullptr) {
                std::cout << " -> Tim thay:\n";
                p->hienThiThongTin();
            } else {
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
            } else {
                std::cout << " -> Da huy thao tac xoa.\n";
            }
            break;
        }
        case 6: {
            if (ql.luuVaoFile()) {
                std::cout << " -> Luu file thanh cong!\n";
            } else {
                std::cout << " -> Loi khi ghi file!\n";
            }
            break;
        }
        case 0:
            return;
        }

        if (chon != 0) {
            std::cout << "\nNhan Enter de tiep tuc...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (chon != 0);
}

int main() {
    // Tối ưu tốc độ nhập xuất C++
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Lưu ý: Hãy tạo sẵn một thư mục con tên là "data" trong thư mục chứa source code 
    // trước khi chạy chương trình để các file văn bản được lưu trữ chính xác.

    // Khởi tạo các bộ quản lý lưu trữ tương ứng cho từng phân hệ
    QuanLyLuuTru<KhachHang>      qlKhachHang("data/khach_hang.txt");
    QuanLyLuuTru<NhanVienKyThuat> qlNhanVien("data/nhan_vien.txt");
    QuanLyLuuTru<HopDong>        qlHopDong("data/hop_dong.txt");
    QuanLyLuuTru<ThietBi>        qlThietBi("data/thiet_bi.txt");
    QuanLyLuuTru<CongOLT>        qlOLT("data/cong_olt.txt");
    QuanLyLuuTru<PhieuLapDat>    qlLapDat("data/phieu_lap_dat.txt");
    QuanLyLuuTru<PhieuSuaChua>   qlSuaChua("data/phieu_sua_chua.txt");
    QuanLyLuuTru<HoaDonCuoc>     qlHoaDon("data/hoa_don_cuoc.txt");

    // Liên kết tĩnh phụ thuộc cho Cổng OLT kiểm tra Khách hàng
    CongOLT::setDBKhachHang(&qlKhachHang);

    int luaChon;
    do {
        hienThiMenuTong();
        // Cho phép nhập từ 0 đến 11, giải quyết triệt để vấn đề không nhận số 0
        luaChon = nhapSoTrongKhoang(0, 11, "Nhap lua chon cua ban (0-11): ");
        std::cout << "\n";

        switch (luaChon) {
        case 1: {
            UC01::chayMenu(); // Gói cước Internet
            break;
        }
        case 2: {
            UC02::chayMenu(); // Tuyến cáp / Khu vực
            break;
        }
        case 3: {
            menuCRUD(qlKhachHang, "KHACH HANG");
            break;
        }
        case 4: {
            menuCRUD(qlNhanVien, "NHAN VIEN KY THUAT");
            break;
        }
        case 5: {
            menuCRUD(qlHopDong, "HOP DONG THUE BAO");
            break;
        }
        case 6: {
            menuCRUD(qlThietBi, "THIET BI MODEM/ONT");
            break;
        }
        case 7: {
            menuCRUD(qlOLT, "CONG OLT (UC06)");
            break;
        }
        case 8: {
            menuCRUD(qlLapDat, "PHIEU LAP DAT (UC07)");
            break;
        }
        case 9: {
            menuCRUD(qlSuaChua, "PHIEU SUA CHUA (UC08)");
            break;
        }
        case 10: {
            menuCRUD(qlHoaDon, "HOA DON CUOC");
            break;
        }
        case 11: {
            std::cout << "--- THONG KE TONG HOP DU LIEU ---\n";
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while (luaChon != 0);

    return 0;
}
