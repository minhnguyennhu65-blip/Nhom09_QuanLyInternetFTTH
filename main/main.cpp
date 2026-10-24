/* =======================================================
 * Tên tác giả: Đinh Văn Trường
 * Mô tả file: Chương trình chính (main.cpp) tổng hợp toàn bộ
 * Yêu cầu: C++17 trở lên
 * ======================================================= */

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <type_traits>

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

 // ---------- Tiện ích dùng chung (C++17) ----------

 // Kiểm tra lớp T có hàm tĩnh inTieuDe() hay không
template <typename T, typename = void>
struct CoInTieuDe : std::false_type {};
template <typename T>
struct CoInTieuDe<T, std::void_t<decltype(T::inTieuDe())>> : std::true_type {};

template <typename T>
void inTieuDeNeuCo() {
    if constexpr (CoInTieuDe<T>::value) T::inTieuDe();
}

// Hàm kiểm tra ràng buộc khi xóa: trả về "" nếu được xóa, ngược lại trả về lý do
template <typename T>
using RangBuocXoa = std::function<std::string(const T&)>;

void hienThiMenuTong() {
    std::cout << "\n=========================================================\n";
    std::cout << "         HE THONG QUAN LY THUE BAO & HA TANG VIEN THONG   \n";
    std::cout << "=========================================================\n";
    std::cout << " 1. Quan ly Goi cuoc Internet (UC01 - Tran Ngoc Huy)\n";
    std::cout << " 2. Quan ly Tuyen cap / Khu vuc (UC02 - Tran Ngoc Huy)\n";
    std::cout << " 3. Quan ly Khach hang (UC03 - Hoang Trong Khoa)\n";
    std::cout << " 4. Quan ly Hop dong thue bao (UC04 - Hoang Trong Khoa)\n";
    std::cout << " 5. Quan ly Thiet bi ONT/Modem (UC05 - Nguyen Nhu Minh)\n";
    std::cout << " 6. Quan ly Cong OLT (UC06 - Nguyen Nhu Minh)\n";
    std::cout << " 7. Quan ly Phieu lap dat (UC07 - Dinh Van Truong)\n";
    std::cout << " 8. Quan ly Phieu sua chua (UC08 - Dinh Van Truong)\n";
    std::cout << " 9. Quan ly Hoa don cuoc (UC09 - Nguyen Duc Huy)\n";
    std::cout << " 10. Quan ly Nhan vien ky thuat (UC10 - Nguyen Duc Huy)\n";
    std::cout << " 11. Thong ke va Xem tong hop du lieu he thong\n";
    std::cout << " 0. Thoat chuong trinh\n";
    std::cout << "=========================================================\n";
}

// Menu CRUD dùng chung cho mọi phân hệ (đã lược bỏ tìm kiếm từ khóa và sắp xếp)
template <typename T>
void menuCRUD(QuanLyLuuTru<T>& ql, const std::string& tenPhanHe,
    RangBuocXoa<T> rangBuoc = nullptr) {
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

        chon = NhapDuLieu::nhapSoNguyenTrongKhoang("Chon chuc nang (0-6): ", 0, 6);
        std::cout << "\n";

        switch (chon) {
        case 1: {
            ql.hienThi([] { inTieuDeNeuCo<T>(); });
            break;
        }
        case 2: {
            T obj;
            try {
                obj.nhapThongTin();
                if (obj.getMaDinhDanh().empty()) {
                    std::cout << " -> Thao tac da duoc huy, khong them vao danh sach.\n";
                }
                else {
                    ql.themMoi(obj);
                }
            }
            catch (const std::invalid_argument& e) {
                std::cout << "\n -> [LOI NHAP DU LIEU]: " << e.what() << "\n";
                std::cout << " -> DA HUY THAO TAC THEM MOI!\n";
            }
            break;
        }
        case 3: {
            std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma dinh danh can tim: ");
            T* p = ql.timTheoMa(ma);
            if (p != nullptr) {
                std::cout << " -> Tim thay:\n";
                inTieuDeNeuCo<T>();
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
            if (ql.timTheoMa(ma) == nullptr) {
                std::cout << " -> Khong tim thay ma: " << ma << "\n";
            }
            else if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa doi tuong nay?")) {
                ql.xoaTheoMa(ma, rangBuoc);
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
    try {
        // Khởi tạo các repository độc lập
        QuanLyLuuTru<GoiInternet>     qlGoiInternet("data/goi_internet.txt");
        QuanLyLuuTru<TuyenCap>         qlTuyenCap("data/tuyen_cap.txt");
        QuanLyLuuTru<KhachHang>        qlKhachHang("data/khach_hang.txt");
        QuanLyLuuTru<NhanVienKyThuat>  qlNhanVien("data/nhan_vien.txt");
        QuanLyLuuTru<ThietBi>          qlThietBi("data/thiet_bi.txt");

        // HopDong tham chiếu: Khach hang, Goi cuoc, Thiet bi
        HopDong::setDBKhachHang(&qlKhachHang);
        HopDong::setKiemTraGoiCuoc([&](const std::string& ma) {
            return qlGoiInternet.timTheoMa(ma) != nullptr;
            });
        HopDong::setKiemTraThietBi([&](const std::string& ma) {
            return qlThietBi.timTheoMa(ma) != nullptr;
            });
        QuanLyLuuTru<HopDong>          qlHopDong("data/hop_dong.txt");
        HopDong::setKiemTraMaTonTai([&](const std::string& ma) {
            return qlHopDong.timTheoMa(ma) != nullptr;
            });

        // ThietBi tham chiếu: Hop dong
        ThietBi::setKiemTraHopDong([&](const std::string& ma) {
            return qlHopDong.timTheoMa(ma) != nullptr;
            });

        // CongOLT tham chiếu: Khach hang, Tuyen cap
        CongOLT::setDBKhachHang(&qlKhachHang);
        CongOLT::setKiemTraTuyenCap([&](const std::string& ma) {
            return qlTuyenCap.timTheoMa(ma) != nullptr;
            });
        QuanLyLuuTru<CongOLT>          qlOLT("data/cong_olt.txt");

        // Các phiếu / hóa đơn
        QuanLyLuuTru<PhieuLapDat>      qlLapDat("data/phieu_lap_dat.txt");
        QuanLyLuuTru<PhieuSuaChua>     qlSuaChua("data/phieu_sua_chua.txt");
        QuanLyLuuTru<HoaDonCuoc>       qlHoaDon("data/hoa_don_cuoc.txt");

        // ---------- Ràng buộc khi xóa ----------
        RangBuocXoa<GoiInternet> rbGoi = [&](const GoiInternet& g) -> std::string {
            for (const auto& h : qlHopDong.getDanhSach())
                if (h.getMaGoiCuoc() == g.getMaDinhDanh())
                    return "Goi cuoc dang duoc dung trong hop dong " + h.getMaDinhDanh();
            return "";
            };
        RangBuocXoa<TuyenCap> rbTuyenCap = [&](const TuyenCap& t) -> std::string {
            for (const auto& c : qlOLT.getDanhSach())
                if (c.getMaTuyenCap() == t.getMaDinhDanh())
                    return "Tuyen cap dang co cong OLT " + c.getMaDinhDanh();
            return "";
            };
        RangBuocXoa<KhachHang> rbKhachHang = [&](const KhachHang& k) -> std::string {
            for (const auto& h : qlHopDong.getDanhSach())
                if (h.getMaKhachHang() == k.getMaDinhDanh())
                    return "Khach hang dang co hop dong " + h.getMaDinhDanh();
            for (const auto& c : qlOLT.getDanhSach())
                if (c.getMaKhachHang() == k.getMaDinhDanh())
                    return "Khach hang dang duoc gan cho cong OLT " + c.getMaDinhDanh();
            return "";
            };
        RangBuocXoa<HopDong> rbHopDong = [&](const HopDong& h) -> std::string {
            for (const auto& t : qlThietBi.getDanhSach())
                if (t.getMaHopDong() == h.getMaDinhDanh())
                    return "Hop dong dang gan voi thiet bi " + t.getMaDinhDanh();
            return "";
            };
        RangBuocXoa<ThietBi> rbThietBi = [&](const ThietBi& t) -> std::string {
            for (const auto& h : qlHopDong.getDanhSach())
                if (h.getMaThietBi() == t.getMaDinhDanh())
                    return "Thiet bi dang duoc dung trong hop dong " + h.getMaDinhDanh();
            return "";
            };

        int luaChon;
        do {
            hienThiMenuTong();
            luaChon = NhapDuLieu::nhapSoNguyenTrongKhoang("Nhap lua chon cua ban (0-11): ", 0, 11);
            std::cout << "\n";

            switch (luaChon) {
            case 1: menuCRUD<GoiInternet>(qlGoiInternet, "GOI INTERNET (UC01 - Tran Ngoc Huy)", rbGoi); break;
            case 2: menuCRUD<TuyenCap>(qlTuyenCap, "TUYEN CAP (UC02 - Tran Ngoc Huy)", rbTuyenCap); break;
            case 3: menuCRUD<KhachHang>(qlKhachHang, "KHACH HANG (UC03 - Hoang Trong Khoa)", rbKhachHang); break;
            case 4: menuCRUD<HopDong>(qlHopDong, "HOP DONG THUE BAO (UC04 - Hoang Trong Khoa)", rbHopDong); break;
            case 5: menuCRUD<ThietBi>(qlThietBi, "THIET BI ONT/MODEM (UC05 - Nguyen Nhu Minh)", rbThietBi); break;
            case 6: menuCRUD<CongOLT>(qlOLT, "CONG OLT (UC06 - Nguyen Nhu Minh)"); break;
            case 7: menuCRUD<PhieuLapDat>(qlLapDat, "PHIEU LAP DAT (UC07 - Dinh Van Truong)"); break;
            case 8: menuCRUD<PhieuSuaChua>(qlSuaChua, "PHIEU SUA CHUA (UC08 - Dinh Van Truong)"); break;
            case 9: menuCRUD<HoaDonCuoc>(qlHoaDon, "HOA DON CUOC (UC09 - Nguyen Duc Huy)"); break;
            case 10: menuCRUD<NhanVienKyThuat>(qlNhanVien, "NHAN VIEN KY THUAT (UC10 - Nguyen Duc Huy)"); break;
            case 11: {
                std::cout << "--- THONG KE TONG HOP DU LIEU ---\n";
                std::cout << " - Tong so Goi cuoc: " << qlGoiInternet.getDanhSach().size() << "\n";
                std::cout << " - Tong so Tuyen cap: " << qlTuyenCap.getDanhSach().size() << "\n";
                std::cout << " - Tong so Khach hang: " << qlKhachHang.getDanhSach().size() << "\n";
                std::cout << " - Tong so Hop dong: " << qlHopDong.getDanhSach().size() << "\n";
                std::cout << " - Tong so Thiet bi: " << qlThietBi.getDanhSach().size() << "\n";
                std::cout << " - Tong so Cong OLT: " << qlOLT.getDanhSach().size() << "\n";
                std::cout << " - Tong so Phieu lap dat: " << qlLapDat.getDanhSach().size() << "\n";
                std::cout << " - Tong so Phieu sua chua: " << qlSuaChua.getDanhSach().size() << "\n";
                std::cout << " - Tong so Hoa don: " << qlHoaDon.getDanhSach().size() << "\n";
                std::cout << " - Tong so Nhan vien: " << qlNhanVien.getDanhSach().size() << "\n";
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

    }
    catch (const std::exception& e) {
        std::cerr << "\n -> Loi he thong (Exception): " << e.what() << "\n";
        std::cerr << " -> Chuong trinh buoc phai thoat an toan.\n";
        return 1;
    }

    return 0;
}
