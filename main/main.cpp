/* =======================================================
- Tên tác giả: Đinh Văn Trường
- Mô tả file: Chương trình chính main.cpp của hệ thống
- quản lý dịch vụ Internet FTTH.
 *
- Chức năng:
- Khởi tạo các bộ quản lý dữ liệu cho từng phân hệ.
- Hiển thị menu chính và điều hướng chức năng.
- Cung cấp menu CRUD dùng chung cho các phân hệ.
- Thống kê số lượng bản ghi của toàn hệ thống.
- ======================================================= */

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <limits>

// Khai báo 13 file header của dự án, bao gồm lớp cơ sở,
// tiện ích nhập liệu, quản lý lưu trữ và các phân hệ nghiệp vụ.
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

// Hàm nhập số nguyên trong khoảng [minVal, maxVal].
// Cho phép nhập số 0 để thoát hoặc quay lại menu.
// Nếu nhập sai kiểu dữ liệu hoặc ngoài khoảng cho phép,
// chương trình thông báo lỗi và yêu cầu nhập lại.
int nhapSoTrongKhoang(
    int minVal,
    int maxVal,
    const std::string& thongBao
) {
    int giaTri;

    while (true) {
        std::cout << thongBao;

        if (std::cin >> giaTri &&
            giaTri >= minVal &&
            giaTri <= maxVal) {

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            return giaTri;
        }

        std::cout << " -> Loi: Vui long chi nhap so nguyen tu "
                  << minVal << " den " << maxVal << "!\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }
}

// Hàm hiển thị menu chính của hệ thống.
// Người dùng có thể lựa chọn phân hệ quản lý,
// chức năng thống kê hoặc thoát chương trình.
void hienThiMenuTong() {
    std::cout << "\n=========================================================\n";
    std::cout << "         HE THONG QUAN LY THUE BAO & HA TANG VIEN THONG\n";
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
    std::cout << "10. Quan ly Hoa don cuoc (Nguyen Duc Huy)\n";
    std::cout << "11. Thong ke va xem tong hop du lieu he thong\n";
    std::cout << " 0. Thoat chuong trinh\n";
    std::cout << "=========================================================\n";
}

// Template menu CRUD dùng chung cho nhiều phân hệ.
// T là kiểu lớp đối tượng cần quản lý.
// ql là đối tượng QuanLyLuuTru tương ứng với kiểu T.
// Các chức năng gồm hiển thị, thêm, tìm kiếm, cập nhật,
// xóa đối tượng và lưu dữ liệu vào file.
template <typename T>
void menuCRUD(
    QuanLyLuuTru<T>& ql,
    const std::string& tenPhanHe
) {
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

        chon = nhapSoTrongKhoang(
            0, 6, "Chon chuc nang (0-6): "
        );

        std::cout << "\n";

        switch (chon) {
        case 1: {
            // Hiển thị danh sách đối tượng của phân hệ.
            ql.hienThi();
            break;
        }

        case 2: {
            // Tạo đối tượng mới, nhập thông tin rồi thêm vào danh sách.
            T obj;
            obj.nhapThongTin();
            ql.themMoi(obj);
            break;
        }

        case 3: {
            // Tìm đối tượng theo mã định danh và hiển thị nếu tìm thấy.
            std::string ma = NhapDuLieu::nhapMaDinhDanh(
                "Nhap ma dinh danh can tim: "
            );

            T* p = ql.timTheoMa(ma);

            if (p != nullptr) {
                std::cout << " -> Tim thay:\n";
                p->hienThiThongTin();
            }
            else {
                std::cout << " -> Khong tim thay doi tuong co ma: "
                          << ma << "\n";
            }

            break;
        }

        case 4: {
            // Cập nhật thông tin của đối tượng theo mã định danh.
            std::string ma = NhapDuLieu::nhapMaDinhDanh(
                "Nhap ma dinh danh can cap nhat: "
            );

            ql.capNhat(ma);
            break;
        }

        case 5: {
            // Xác nhận trước khi xóa đối tượng khỏi danh sách.
            std::string ma = NhapDuLieu::nhapMaDinhDanh(
                "Nhap ma dinh danh can xoa: "
            );

            if (NhapDuLieu::xacNhan(
                "Ban co chac chan muon xoa doi tuong nay?"
            )) {
                ql.xoaTheoMa(ma);
            }
            else {
                std::cout << " -> Da huy thao tac xoa.\n";
            }

            break;
        }

        case 6: {
            // Lưu dữ liệu của phân hệ vào file tương ứng.
            if (ql.luuVaoFile()) {
                std::cout << " -> Luu file thanh cong!\n";
            }
            else {
                std::cout << " -> Loi khi ghi file!\n";
            }

            break;
        }

        case 0:
            // Quay lại menu chính.
            return;
        }

        if (chon != 0) {
            std::cout << "\nNhan Enter de tiep tuc...";
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );
        }

    } while (chon != 0);
}

int main() {
    // Tối ưu thao tác nhập xuất dữ liệu bằng C++.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // Khởi tạo bộ quản lý lưu trữ cho từng phân hệ.
    // Mỗi bộ quản lý gắn với một file dữ liệu riêng trong thư mục data.
    QuanLyLuuTru<GoiInternet> qlGoiInternet(
        "data/goi_internet.txt"
    );
    qlGoiInternet.docTuFile();

    QuanLyLuuTru<TuyenCap> qlTuyenCap(
        "data/tuyen_cap.txt"
    );
    qlTuyenCap.docTuFile();

    QuanLyLuuTru<KhachHang> qlKhachHang(
        "data/khach_hang.txt"
    );
    qlKhachHang.docTuFile();

    QuanLyLuuTru<NhanVienKyThuat> qlNhanVien(
        "data/nhan_vien.txt"
    );
    qlNhanVien.docTuFile();

    QuanLyLuuTru<HopDong> qlHopDong(
        "data/hop_dong.txt"
    );
    qlHopDong.docTuFile();

    QuanLyLuuTru<ThietBi> qlThietBi(
        "data/thiet_bi.txt"
    );
    qlThietBi.docTuFile();

    QuanLyLuuTru<CongOLT> qlOLT(
        "data/cong_olt.txt"
    );
    qlOLT.docTuFile();

    QuanLyLuuTru<PhieuLapDat> qlLapDat(
        "data/phieu_lap_dat.txt"
    );
    qlLapDat.docTuFile();

    QuanLyLuuTru<PhieuSuaChua> qlSuaChua(
        "data/phieu_sua_chua.txt"
    );
    qlSuaChua.docTuFile();

    QuanLyLuuTru<HoaDonCuoc> qlHoaDon(
        "data/hoa_don_cuoc.txt"
    );
    qlHoaDon.docTuFile();

    // Thiết lập bộ quản lý khách hàng cho phân hệ Cổng OLT.
    CongOLT::setDBKhachHang(&qlKhachHang);

    int luaChon;

    // Lặp menu chính cho đến khi người dùng chọn 0 để thoát.
    do {
        hienThiMenuTong();

        luaChon = nhapSoTrongKhoang(
            0, 11, "Nhap lua chon cua ban (0-11): "
        );

        std::cout << "\n";

        switch (luaChon) {
        case 1: {
            menuCRUD(qlGoiInternet, "GOI INTERNET (UC01)");
            break;
        }

        case 2: {
            menuCRUD(qlTuyenCap, "TUYEN CAP (UC02)");
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
            // Đọc lại dữ liệu từ các file trước khi thống kê.
            qlGoiInternet.docTuFile();
            qlTuyenCap.docTuFile();
            qlKhachHang.docTuFile();
            qlNhanVien.docTuFile();
            qlHopDong.docTuFile();
            qlThietBi.docTuFile();
            qlOLT.docTuFile();
            qlLapDat.docTuFile();
            qlSuaChua.docTuFile();
            qlHoaDon.docTuFile();

            // Hiển thị số lượng bản ghi hiện có của từng phân hệ.
            std::cout << "--- THONG KE TONG HOP DU LIEU ---\n";
            std::cout << " - Tong so Goi cuoc: "
                      << qlGoiInternet.getDanhSach().size() << "\n";
            std::cout << " - Tong so Tuyen cap: "
                      << qlTuyenCap.getDanhSach().size() << "\n";
            std::cout << " - Tong so Khach hang: "
                      << qlKhachHang.getDanhSach().size() << "\n";
            std::cout << " - Tong so Nhan vien: "
                      << qlNhanVien.getDanhSach().size() << "\n";
            std::cout << " - Tong so Hop dong: "
                      << qlHopDong.getDanhSach().size() << "\n";
            std::cout << " - Tong so Thiet bi: "
                      << qlThietBi.getDanhSach().size() << "\n";
            std::cout << " - Tong so Cong OLT: "
                      << qlOLT.getDanhSach().size() << "\n";
            std::cout << " - Tong so Phieu lap dat: "
                      << qlLapDat.getDanhSach().size() << "\n";
            std::cout << " - Tong so Phieu sua chua: "
                      << qlSuaChua.getDanhSach().size() << "\n";
            std::cout << " - Tong so Hoa don: "
                      << qlHoaDon.getDanhSach().size() << "\n";

            break;
        }

        case 0: {
            std::cout << " -> Cam on ban da su dung chuong trinh. Tam biet!\n";
            break;
        }
        }

        if (luaChon != 0) {
            std::cout << "\nNhan Enter de tiep tuc...";
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );
        }

    } while (luaChon != 0);

    return 0;
}
