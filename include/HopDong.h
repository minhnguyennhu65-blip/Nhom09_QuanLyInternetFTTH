/* =======================================================
 * Tên tác giả: Hoàng Trọng Khoa
 * Mã sinh viên: B24DCVT204
 * Mô tả file: Lớp quản lý Hợp Đồng Thuê Bao
 * ======================================================= */

#ifndef HOP_DONG_H
#define HOP_DONG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include "QuanLyLuuTru.h"
#include "KhachHang.h"

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class HopDong : public LopCoSo {
private:
    std::string maKhachHang;  // Khóa ngoại tới Khách hàng
    std::string maGoiCuoc;    // Khóa ngoại tới Gói cước
    std::string maThietBi;    // Khóa ngoại tới Thiết bị
    std::string ngayKy;       // DD/MM/YYYY, không sau ngày hiện tại
    std::string ngayHetHan;   // DD/MM/YYYY, không trước ngày ký
    double giaThieuDung = 0.0;
    std::string trangThai = "Hoat dong"; // "Hoat dong", "Het han", "Huy"
    std::string loaiKhachHang;           // Lấy tự động từ Khách hàng (Ca nhan / Doanh nghiep)

    // Các cơ sở dữ liệu / hàm kiểm tra khóa ngoại (gắn từ main)
    inline static QuanLyLuuTru<KhachHang>* dbKhachHang = nullptr;
    inline static std::function<bool(const std::string&)> kiemTraGoiCuoc = nullptr;
    inline static std::function<bool(const std::string&)> kiemTraThietBi = nullptr;
    inline static std::function<bool(const std::string&)> kiemTraMaTonTai = nullptr;

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == "Hoat dong" || tt == "Het han" || tt == "Huy";
    }

    static bool hopLeLoaiKH(const std::string& l) {
        return l == "Ca nhan" || l == "Doanh nghiep";
    }

    static std::string chuanHoaMa(const std::string& ma, const std::string& ten) {
        if (ma.empty())
            throw std::invalid_argument(ten + " khong duoc rong!");
        std::string kq = ma;
        for (char& c : kq) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(std::isalnum(uc) && uc < 128) && c != '_' && c != '-')
                throw std::invalid_argument(
                    ten + " chi gom chu cai khong dau, chu so, '_' hoac '-'!");
            c = static_cast<char>(std::toupper(uc));
        }
        return kq;
    }

    static std::vector<std::string> tachTruong(const std::string& s) {
        std::vector<std::string> kq;
        std::size_t dau = 0;
        while (true) {
            std::size_t p = s.find('|', dau);
            if (p == std::string::npos) {
                kq.push_back(s.substr(dau));
                break;
            }
            kq.push_back(s.substr(dau, p - dau));
            dau = p + 1;
        }
        return kq;
    }

    static double phanTichGia(const std::string& s) {
        if (!s.empty() && s.find_first_not_of("0123456789.eE+-") == std::string::npos) {
            char* ketThuc = nullptr;
            double v = std::strtod(s.c_str(), &ketThuc);
            if (*ketThuc == '\0' && std::isfinite(v) && v >= 0.0) return v;
        }
        throw std::invalid_argument("Gia thue dung phai la so thuc >= 0!");
    }

    static std::string cat(const std::string& s, std::size_t n) {
        return s.size() <= n ? s : s.substr(0, n - 2) + "..";
    }

    static std::string docHoacHuy(const std::string& thongBao) {
        std::string s = NhapDuLieu::nhapChuoi(thongBao + " (hoac nhap 'HUY'): ");
        if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");
        return s;
    }

    static std::string docTuyChon(const std::string& thongBao) {
        std::string s = NhapDuLieu::nhapChuoiHoacGiuNguyen(
            thongBao + " (bo trong de giu nguyen, hoac nhap 'HUY'): ", "");
        if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");
        return s;
    }

    template <typename F>
    static void lap(F thucHien) {
        while (true) {
            try { thucHien(); return; }
            catch (const std::invalid_argument& e) {
                if (std::string(e.what()) == "HUY") throw;
                std::cout << " -> Loi: " << e.what() << '\n';
            }
        }
    }

    void chonTrangThai(bool giu) {
        std::string s = giu
            ? docTuyChon("Chon trang thai (1-3, hien tai: " + trangThai + ")")
            : docHoacHuy("Chon trang thai (1-3)");
        if (s.empty()) return;
        char* ketThuc = nullptr;
        long v = std::strtol(s.c_str(), &ketThuc, 10);
        if (*ketThuc != '\0' || v < 1 || v > 3)
            throw std::invalid_argument("Lua chon khong hop le (1-3)!");
        setTrangThai(v == 1 ? "Hoat dong" : v == 2 ? "Het han" : "Huy");
    }

public:
    HopDong() = default;

    // ---------- Gắn khóa ngoại (Các hàm đang bị báo thiếu) ----------
    static void setDBKhachHang(QuanLyLuuTru<KhachHang>* db) { dbKhachHang = db; }
    static void setKiemTraGoiCuoc(std::function<bool(const std::string&)> f) { kiemTraGoiCuoc = std::move(f); }
    static void setKiemTraThietBi(std::function<bool(const std::string&)> f) { kiemTraThietBi = std::move(f); }
    static void setKiemTraMaTonTai(std::function<bool(const std::string&)> f) { kiemTraMaTonTai = std::move(f); }

    static void inTieuDe() {
        std::cout << std::left
            << std::setw(8)  << "Ma HD"     << " | "
            << std::setw(8)  << "Ma KH"     << " | "
            << std::setw(8)  << "Ma Goi"    << " | "
            << std::setw(8)  << "Ma TB"     << " | "
            << std::setw(11) << "Ngay ky"   << " | "
            << std::setw(11) << "Ngay het"  << " | "
            << std::setw(13) << "Gia (VND)" << " | "
            << std::setw(13) << "Loai KH"   << " | "
            << std::setw(11) << "Trang thai" << "\n"
            << std::string(115, '-') << "\n";
    }

    std::string getMaHopDong() const { return maDinhDanh; }
    const std::string& getMaKhachHang() const { return maKhachHang; }
    const std::string& getMaGoiCuoc() const { return maGoiCuoc; }
    const std::string& getMaThietBi() const { return maThietBi; }
    const std::string& getNgayKy() const { return ngayKy; }
    const std::string& getNgayHetHan() const { return ngayHetHan; }
    double getGiaThieuDung() const { return giaThieuDung; }
    const std::string& getTrangThai() const { return trangThai; }
    const std::string& getLoaiKhachHang() const { return loaiKhachHang; }

    void setMaKhachHang(const std::string& ma) {
        std::string chuan = chuanHoaMa(ma, "Ma khach hang");
        if (dbKhachHang == nullptr)
            throw std::invalid_argument("Chua gan co so du lieu khach hang (HopDong::setDBKhachHang)!");
        const KhachHang* kh = dbKhachHang->timTheoMa(chuan);
        if (kh == nullptr)
            throw std::invalid_argument("Khach hang [" + chuan + "] khong ton tai!");
        maKhachHang = chuan;
        loaiKhachHang = kh->getLoaiKhachHang();
    }

    void setMaGoiCuoc(const std::string& ma) {
        std::string chuan = chuanHoaMa(ma, "Ma goi cuoc");
        if (kiemTraGoiCuoc && !kiemTraGoiCuoc(chuan))
            throw std::invalid_argument("Goi cuoc [" + chuan + "] khong ton tai!");
        maGoiCuoc = chuan;
    }

    void setMaThietBi(const std::string& ma) {
        std::string chuan = chuanHoaMa(ma, "Ma thiet bi");
        if (kiemTraThietBi && !kiemTraThietBi(chuan))
            throw std::invalid_argument("Thiet bi [" + chuan + "] khong ton tai!");
        maThietBi = chuan;
    }

    void setThoiHan(const std::string& ky, const std::string& hetHan) {
        if (!NhapDuLieu::laNgayThangHopLe(ky))
            throw std::invalid_argument(
                "Ngay ky khong hop le! Dung dinh dang DD/MM/YYYY (VD: 15/03/2026).");
        if (NhapDuLieu::soSanhNgay(ky, NhapDuLieu::ngayHienTai()) > 0)
            throw std::invalid_argument(
                "Ngay ky khong duoc sau ngay hien tai (" + NhapDuLieu::ngayHienTai() + ")!");
        if (!NhapDuLieu::laNgayThangHopLe(hetHan))
            throw std::invalid_argument(
                "Ngay het han khong hop le! Dung dinh dang DD/MM/YYYY (VD: 15/03/2026).");
        if (NhapDuLieu::soSanhNgay(hetHan, ky) < 0)
            throw std::invalid_argument("Ngay het han khong duoc truoc ngay ky!");
        ngayKy = ky;
        ngayHetHan = hetHan;
    }

    void setGiaThieuDung(double gia) {
        if (!std::isfinite(gia) || gia < 0.0)
            throw std::invalid_argument("Gia thue dung phai la so thuc >= 0!");
        giaThieuDung = gia;
    }

    void setTrangThai(const std::string& tt) {
        if (!hopLeTrangThai(tt))
            throw std::invalid_argument("Trang thai chi duoc la 'Hoat dong', 'Het han' hoac 'Huy'!");
        trangThai = tt;
    }

    void nhapThongTin() override {
        HopDong tam;
        try {
            lap([&] {
                std::string ma = chuanHoaMa(docHoacHuy("Nhap ma hop dong"), "Ma hop dong");
                if (kiemTraMaTonTai && kiemTraMaTonTai(ma))
                    throw std::invalid_argument("Ma hop dong da ton tai!");
                tam.setMaDinhDanh(ma);
            });
            lap([&] { tam.setMaKhachHang(docHoacHuy("Nhap ma khach hang")); });
            std::cout << " -> Loai khach hang: " << tam.loaiKhachHang << "\n";
            lap([&] { tam.setMaGoiCuoc(docHoacHuy("Nhap ma goi cuoc")); });
            lap([&] { tam.setMaThietBi(docHoacHuy("Nhap ma thiet bi")); });
            lap([&] {
                std::string ky = docHoacHuy("Nhap ngay ky (DD/MM/YYYY)");
                std::string het = docHoacHuy("Nhap ngay het han (DD/MM/YYYY)");
                tam.setThoiHan(ky, het);
            });
            lap([&] { tam.setGiaThieuDung(phanTichGia(docHoacHuy("Nhap gia thue dung (>= 0)"))); });
            std::cout << "\nTrang thai hop dong:\n1. Hoat dong\n2. Het han\n3. Huy\n";
            lap([&] { tam.chonTrangThai(false); });
        }
        catch (const std::invalid_argument& e) {
            if (std::string(e.what()) == "HUY") {
                std::cout << " -> Da huy thao tac them hop dong.\n";
                maDinhDanh = "";
                return;
            }
            throw;
        }
        *this = tam;
    }

    void capNhatThongTin() override {
        HopDong tam = *this;
        try {
            std::cout << "\n--- CAP NHAT HOP DONG " << maDinhDanh << " ---\n";
            lap([&] {
                std::string s = docTuyChon("Ma khach hang moi (hien tai: " + tam.maKhachHang + ")");
                if (!s.empty()) {
                    tam.setMaKhachHang(s);
                    std::cout << " -> Loai khach hang: " << tam.loaiKhachHang << "\n";
                }
            });
            lap([&] {
                std::string s = docTuyChon("Ma goi cuoc moi (hien tai: " + tam.maGoiCuoc + ")");
                if (!s.empty()) tam.setMaGoiCuoc(s);
            });
            lap([&] {
                std::string s = docTuyChon("Ma thiet bi moi (hien tai: " + tam.maThietBi + ")");
                if (!s.empty()) tam.setMaThietBi(s);
            });
            lap([&] {
                std::string ky = docTuyChon("Ngay ky moi (hien tai: " + tam.ngayKy + ")");
                std::string het = docTuyChon("Ngay het han moi (hien tai: " + tam.ngayHetHan + ")");
                tam.setThoiHan(ky.empty() ? tam.ngayKy : ky,
                              het.empty() ? tam.ngayHetHan : het);
            });
            lap([&] {
                std::ostringstream hienTai;
                hienTai << std::fixed << std::setprecision(0) << tam.giaThieuDung;
                std::string s = docTuyChon("Gia thue dung moi (hien tai: " + hienTai.str() + ")");
                if (!s.empty()) tam.setGiaThieuDung(phanTichGia(s));
            });
            std::cout << "\nTrang thai hop dong:\n1. Hoat dong\n2. Het han\n3. Huy\n";
            lap([&] { tam.chonTrangThai(true); });
        }
        catch (const std::invalid_argument& e) {
            if (std::string(e.what()) == "HUY")
                throw std::runtime_error("Nguoi dung da huy cap nhat!");
            throw;
        }
        *this = tam;
    }

    void hienThiThongTin() const override {
        std::ostringstream gia;
        gia << std::fixed << std::setprecision(0) << giaThieuDung;
        std::cout << std::left
            << std::setw(8)  << cat(maDinhDanh, 8)    << " | "
            << std::setw(8)  << cat(maKhachHang, 8)   << " | "
            << std::setw(8)  << cat(maGoiCuoc, 8)     << " | "
            << std::setw(8)  << cat(maThietBi, 8)     << " | "
            << std::setw(11) << ngayKy                << " | "
            << std::setw(11) << ngayHetHan            << " | "
            << std::setw(13) << gia.str()             << " | "
            << std::setw(13) << loaiKhachHang         << " | "
            << std::setw(11) << trangThai             << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        std::ostringstream ss;
        ss << maDinhDanh << '|' << maKhachHang << '|' << maGoiCuoc << '|' << maThietBi << '|'
           << ngayKy << '|' << ngayHetHan << '|'
           << std::setprecision(15) << giaThieuDung << '|'
           << loaiKhachHang << '|' << trangThai;
        return ss.str();
    }

    void docTuChuoi(const std::string& dong) override {
        std::vector<std::string> f = tachTruong(dong);
        if (f.size() != 9)
            throw std::invalid_argument("Dong du lieu HopDong co so cot khong dung (can 9).");

        HopDong tam;
        tam.setMaDinhDanh(chuanHoaMa(f[0], "Ma hop dong"));
        tam.setMaKhachHang(f[1]);
        tam.setMaGoiCuoc(f[2]);
        tam.setMaThietBi(f[3]);
        tam.setThoiHan(f[4], f[5]);
        tam.setGiaThieuDung(phanTichGia(f[6]));
        if (!hopLeLoaiKH(f[7]))
            throw std::invalid_argument("Loai khach hang khong hop le: " + f[7]);
        tam.setTrangThai(f[8]);
        *this = tam;
    }
};

#endif // HOP_DONG_H
