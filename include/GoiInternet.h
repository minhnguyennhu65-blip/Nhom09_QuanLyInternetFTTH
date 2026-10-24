/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 01 - Quản lý Gói cước Internet
 * ======================================================= */

#ifndef GOI_INTERNET_H
#define GOI_INTERNET_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include <algorithm>
#include <cmath>
#include <cctype>
#include <filesystem>
#include "QuanLyLuuTru.h"

class GoiInternet : public LopCoSo {
private:
    std::string tenGoi;
    int tocDo;              // Mbps, >= 0
    double giaThang;        // VND/thang, >= 0
    std::string thoiHanSuDung; // Thời hạn sử dụng (ví dụ: 12 tháng, 24 tháng)
    std::string moTa;
    std::string trangThai;  // "Dang hoat dong" / "Ngung"

    static const std::vector<std::string>& danhSachTrangThai() {
        static const std::vector<std::string> ds = {"Dang hoat dong", "Ngung"};
        return ds;
    }

    void nhapThuocTinh() {
        tenGoi = NhapDuLieu::nhapChuoi("Nhap ten goi: ");
        tocDo = NhapDuLieu::nhapSoNguyenDuong("Nhap toc do (Mbps): ");
        giaThang = NhapDuLieu::nhapSoThucDuong("Nhap gia cuoc/thang (VND): ");
        thoiHanSuDung = NhapDuLieu::nhapChuoi("Nhap thoi han su dung (VD: 12 thang): ");
        moTa = NhapDuLieu::nhapChuoi("Nhap mo ta goi: ");
        trangThai = chonTuDanhSach("Chon trang thai:", danhSachTrangThai());
    }

public:
    static std::vector<std::string> tachTruong(std::string dong) {
        while (!dong.empty() && (dong.back() == '\r' || dong.back() == '\n')) dong.pop_back();
        std::vector<std::string> kq;
        size_t bd = 0;
        while (true) {
            size_t vt = dong.find('|', bd);
            if (vt == std::string::npos) { kq.push_back(dong.substr(bd)); break; }
            kq.push_back(dong.substr(bd, vt - bd));
            bd = vt + 1;
        }
        return kq;
    }

private:
    static void kiemTraKhongRong(const std::string& s, const std::string& ten) {
        if (s.find_first_not_of(" \t") == std::string::npos)
            throw std::runtime_error(ten + " khong duoc de trong");
    }

    static bool thuocDanhSach(const std::string& gt, const std::vector<std::string>& ds) {
        return std::find(ds.begin(), ds.end(), gt) != ds.end();
    }

    static std::string chonTuDanhSach(const std::string& thongBao, const std::vector<std::string>& ds) {
        std::cout << thongBao << "\n";
        for (size_t i = 0; i < ds.size(); i++) std::cout << "   " << (i + 1) << ". " << ds[i] << "\n";
        while (true) {
            int chon = NhapDuLieu::nhapSoNguyenDuong("   Chon (1-" + std::to_string(ds.size()) + "): ");
            if (chon >= 1 && chon <= static_cast<int>(ds.size())) return ds[chon - 1];
            std::cout << " -> Loi: Lua chon khong hop le!\n";
        }
    }

    static std::string catChuoi(const std::string& s, size_t max) {
        return s.size() <= max ? s : s.substr(0, max - 3) + "...";
    }

    static int parseSoNguyenKhongAm(const std::string& s, const std::string& ten) {
        if (s.empty() || !std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c) != 0; }))
            throw std::runtime_error(ten + " phai la so nguyen >= 0");
        try { return std::stoi(s); }
        catch (...) { throw std::runtime_error(ten + " vuot qua gioi han so nguyen"); }
    }

    static double parseSoThucKhongAm(const std::string& s, const std::string& ten) {
        size_t daDoc = 0; double gt = 0;
        try { gt = std::stod(s, &daDoc); }
        catch (...) { throw std::runtime_error(ten + " phai la so thuc >= 0"); }
        if (daDoc != s.size() || !std::isfinite(gt) || gt < 0.0)
            throw std::runtime_error(ten + " phai la so thuc >= 0");
        return gt;
    }

public:
    GoiInternet() : LopCoSo(), tenGoi(""), tocDo(0), giaThang(0.0), thoiHanSuDung(""), moTa(""), trangThai("") {}

    std::string getMaGoi() const { return maDinhDanh; }
    std::string getTenGoi() const { return tenGoi; }
    int getTocDo() const { return tocDo; }
    double getGiaThang() const { return giaThang; }
    std::string getThoiHanSuDung() const { return thoiHanSuDung; }
    std::string getMoTa() const { return moTa; }
    std::string getTrangThai() const { return trangThai; }

    void nhapThongTin() override {
        if (maDinhDanh.empty()) {
            maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi (maGoi): ");
        }
        nhapThuocTinh();
    }

    void capNhatThongTin() override {
        std::cout << "(Ma goi " << maDinhDanh << " duoc khoa, khong the thay doi)\n";
        nhapThuocTinh();
    }

    // Đã đổi tên từ inTieuDeBang thành inTieuDe để tương thích với menuCRUD chung
    static void inTieuDe() {
        std::cout << std::left
                  << std::setw(10) << "Ma goi"
                  << std::setw(18) << "Ten goi"
                  << std::setw(14) << "Toc do(Mbps)"
                  << std::setw(16) << "Gia/thang(VND)"
                  << std::setw(16) << "Thoi han"
                  << std::setw(26) << "Mo ta"
                  << std::setw(16) << "Trang thai" << "\n"
                  << std::string(116, '-') << "\n";
    }

    void hienThiThongTin() const override {
        std::cout << std::left
                  << std::setw(10) << catChuoi(maDinhDanh, 9)
                  << std::setw(18) << catChuoi(tenGoi, 17)
                  << std::setw(14) << tocDo
                  << std::setw(16) << std::fixed << std::setprecision(0) << giaThang
                  << std::setw(16) << catChuoi(thoiHanSuDung, 15)
                  << std::setw(26) << catChuoi(moTa, 25)
                  << std::setw(16) << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        std::ostringstream oss;
        oss << std::setprecision(15) << giaThang;
        return maDinhDanh + "|" + tenGoi + "|" + std::to_string(tocDo) + "|" +
               oss.str() + "|" + thoiHanSuDung + "|" + moTa + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dongDuLieu) override {
        std::vector<std::string> f = tachTruong(dongDuLieu);
        if (f.size() != 7) {
            throw std::runtime_error("Sai so cot (can 7, co " + std::to_string(f.size()) + ")");
        }
        kiemTraKhongRong(f[0], "maGoi");
        if (f[0].find_first_of(" \t") != std::string::npos) {
            throw std::runtime_error("maGoi khong duoc chua khoang trang");
        }
        kiemTraKhongRong(f[1], "tenGoi");
        int tocDoMoi = parseSoNguyenKhongAm(f[2], "tocDo");
        double giaMoi = parseSoThucKhongAm(f[3], "giaThang");
        kiemTraKhongRong(f[4], "thoiHanSuDung");
        kiemTraKhongRong(f[5], "moTa");
        if (!thuocDanhSach(f[6], danhSachTrangThai())) {
            throw std::runtime_error("trangThai khong hop le: " + f[6]);
        }

        maDinhDanh = f[0];
        tenGoi = f[1];
        tocDo = tocDoMoi;
        giaThang = giaMoi;
        thoiHanSuDung = f[4];
        moTa = f[5];
        trangThai = f[6];
    }
};

namespace UC01 {
    const std::string FILE_GOI_INTERNET = "data/goi_internet.txt";
    const std::string FILE_HOP_DONG = "data/hop_dong.txt";

    inline bool goiDangDuocThamChieu(const std::string& maGoi) {
        std::ifstream file(FILE_HOP_DONG);
        if (!file.is_open()) return false;
        std::string dong;
        while (std::getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') continue;
            std::vector<std::string> f = GoiInternet::tachTruong(dong);
            if (f.size() > 2 && f[2] == maGoi) return true;
        }
        return false;
    }

    inline void themGoi(QuanLyLuuTru<GoiInternet>& ql) {
        std::cout << "\n--- THEM GOI INTERNET ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi (maGoi): ");
        if (ql.timTheoMa(ma) != nullptr) {
            std::cout << " -> Loi: Ma goi '" << ma << "' da ton tai!\n";
            return;
        }
        GoiInternet goi;
        goi.setMaDinhDanh(ma);
        goi.nhapThongTin();
        ql.themMoi(goi);
    }

    inline void hienThiDanhSach(QuanLyLuuTru<GoiInternet>& ql) {
        std::cout << "\n--- DANH SACH GOI INTERNET ---\n";
        ql.docTuFile();
        const auto& ds = ql.getDanhSach();
        if (ds.empty()) {
            std::cout << " -> Danh sach rong!\n";
            return;
        }
        GoiInternet::inTieuDe();
        double tongGia = 0;
        for (const auto& goi : ds) {
            goi.hienThiThongTin();
            tongGia += goi.getGiaThang();
        }
        std::cout << std::string(116, '-') << "\n";
        std::cout << " => Tong so goi cước: " << ds.size() 
                  << " | Tong gia tri trung binh/thang: " << std::fixed << std::setprecision(0) << (tongGia / ds.size()) << " VND\n";
    }

    inline void timKiemGoi(QuanLyLuuTru<GoiInternet>& ql) {
        std::cout << "\n--- TIM KIEM GOI INTERNET ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can tim: ");
        GoiInternet* goi = ql.timTheoMa(ma);
        if (goi == nullptr) {
            std::cout << " -> Khong tim thay goi co ma: " << ma << "\n";
            return;
        }
        GoiInternet::inTieuDe();
        goi->hienThiThongTin();
        std::cout << std::string(116, '-') << "\n";
    }

    inline void suaGoi(QuanLyLuuTru<GoiInternet>& ql) {
        std::cout << "\n--- SUA GOI INTERNET ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can sua: ");
        GoiInternet* goi = ql.timTheoMa(ma);
        if (goi == nullptr) {
            std::cout << " -> Khong tim thay goi co ma: " << ma << "\n";
            return;
        }
        std::cout << "Thong tin hien tai:\n";
        GoiInternet::inTieuDe();
        goi->hienThiThongTin();
        std::cout << "\nNhap thong tin moi:\n";
        ql.capNhat(ma);
    }

    inline void xoaGoi(QuanLyLuuTru<GoiInternet>& ql) {
        std::cout << "\n--- XOA GOI INTERNET ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can xoa: ");
        GoiInternet* goi = ql.timTheoMa(ma);
        if (goi == nullptr) {
            std::cout << " -> Khong tim thay goi co ma: " << ma << "\n";
            return;
        }
        if (goiDangDuocThamChieu(ma)) {
            std::cout << " -> Canh bao: Goi '" << ma << "' dang duoc su dung trong Hop dong!\n";
            return;
        }
        GoiInternet::inTieuDe();
        goi->hienThiThongTin();
        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa goi nay?")) {
            ql.xoaTheoMa(ma);
        } else {
            std::cout << " -> Da huy thao tac xoa.\n";
        }
    }
}

#endif
