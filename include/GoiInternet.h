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
    std::string moTa;
    std::string trangThai;  // "Dang hoat dong" / "Ngung"

    static const std::vector<std::string>& danhSachTrangThai() {
        static const std::vector<std::string> ds = { "Dang hoat dong", "Ngung" };
        return ds;
    }

    void nhapThuocTinh() {
        tenGoi = NhapDuLieu::nhapChuoi("Nhap ten goi: ");
        tocDo = NhapDuLieu::nhapSoNguyenDuong("Nhap toc do (Mbps): ");
        giaThang = NhapDuLieu::nhapSoThucDuong("Nhap gia cuoc/thang (VND): ");
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
        for (size_t i = 0; i < ds.size(); i++) std::cout << "    " << (i + 1) << ". " << ds[i] << "\n";
        while (true) {
            int chon = NhapDuLieu::nhapSoNguyenDuong("    Chon (1-" + std::to_string(ds.size()) + "): ");
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
    GoiInternet() : LopCoSo(), tenGoi(""), tocDo(0), giaThang(0.0), moTa(""), trangThai("") {}

    std::string getMaGoi() const { return maDinhDanh; }
    std::string getTenGoi() const { return tenGoi; }
    int getTocDo() const { return tocDo; }
    double getGiaThang() const { return giaThang; }
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

    static void inTieuDeBang() {
        std::cout << std::left
            << std::setw(10) << "Ma goi"
            << std::setw(20) << "Ten goi"
            << std::setw(14) << "Toc do(Mbps)"
            << std::setw(16) << "Gia/thang(VND)"
            << std::setw(32) << "Mo ta"
            << std::setw(16) << "Trang thai" << "\n"
            << std::string(108, '-') << "\n";
    }

    void hienThiThongTin() const override {
        std::cout << std::left
            << std::setw(10) << catChuoi(maDinhDanh, 9)
            << std::setw(20) << catChuoi(tenGoi, 19)
            << std::setw(14) << tocDo
            << std::setw(16) << std::fixed << std::setprecision(0) << giaThang
            << std::setw(32) << catChuoi(moTa, 31)
            << std::setw(16) << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        std::ostringstream oss;
        oss << std::setprecision(15) << giaThang;
        return maDinhDanh + "|" + tenGoi + "|" + std::to_string(tocDo) + "|" +
            oss.str() + "|" + moTa + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dongDuLieu) override {
        std::vector<std::string> f = tachTruong(dongDuLieu);
        if (f.size() != 6) {
            throw std::runtime_error("Sai so cot (can 6, co " + std::to_string(f.size()) + ")");
        }
        kiemTraKhongRong(f[0], "maGoi");
        if (f[0].find_first_of(" \t") != std::string::npos) {
            throw std::runtime_error("maGoi khong duoc chua khoang trang");
}
        kiemTraKhongRong(f[1], "tenGoi");
        int tocDoMoi = parseSoNguyenKhongAm(f[2], "tocDo");
        double giaMoi = parseSoThucKhongAm(f[3], "giaThang");
        kiemTraKhongRong(f[4], "moTa");
        if (!thuocDanhSach(f[5], danhSachTrangThai())) {
            throw std::runtime_error("trangThai khong hop le: " + f[5]);
        }

        maDinhDanh = f[0];
        tenGoi = f[1];
        tocDo = tocDoMoi;
        giaThang = giaMoi;
        moTa = f[4];
        trangThai = f[5];
    }
};

// =====================================================
// MENU USE CASE
// =====================================================
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
            std::cout << " -> Loi: Ma goi '" << ma << "' da ton tai! Khong them du lieu.\n";
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
        if (ql.getDanhSach().empty()) {
            std::cout << " -> Danh sach rong!\n";
            return;
        }
        GoiInternet::inTieuDeBang();
        ql.hienThi();
        std::cout << "Tong so goi: " << ql.getDanhSach().size() << "\n";
    }

    inline void timKiemGoi(QuanLyLuuTru<GoiInternet>& ql) {
        std::cout << "\n--- TIM KIEM GOI INTERNET ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can tim: ");
        GoiInternet* goi = ql.timTheoMa(ma);
        if (goi == nullptr) {
            std::cout << " -> Khong tim thay goi co ma: " << ma << "\n";
            return;
        }
        GoiInternet::inTieuDeBang();
        goi->hienThiThongTin();
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
GoiInternet::inTieuDeBang();
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
            std::cout << " -> Canh bao: Goi '" << ma
                << "' dang duoc su dung trong Hop dong thue bao. Khong the xoa!\n";
            return;
        }
        GoiInternet::inTieuDeBang();
        goi->hienThiThongTin();
        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa goi nay?")) {
            ql.xoaTheoMa(ma);
        }
        else {
            std::cout << " -> Da huy thao tac xoa.\n";
        }
    }

    // Nhận tham số ql từ hàm main.cpp truyền vào
    inline void chayMenu(QuanLyLuuTru<GoiInternet>& ql) {
        try { std::filesystem::create_directories("data"); }
        catch (...) {}
        ql.docTuFile(); // Đồng bộ dữ liệu mới nhất
        while (true) {
            std::cout << "\n===== UC01 - QUAN LY GOI INTERNET =====\n"
                << "1. Them goi\n"
                << "2. Hien thi danh sach\n"
                << "3. Tim kiem theo ma\n"
                << "4. Sua thong tin\n"
                << "5. Xoa goi\n"
                << "0. Quay lai\n";
            int chon = NhapDuLieu::nhapSoNguyenDuong("Chon chuc nang: ");
            switch (chon) {
            case 1: themGoi(ql); break;
            case 2: hienThiDanhSach(ql); break;
            case 3: timKiemGoi(ql); break;
            case 4: suaGoi(ql); break;
            case 5: xoaGoi(ql); break;
            case 0: return;
            default: std::cout << " -> Loi: Lua chon khong hop le!\n";
            }
        }
    }
}

#endif

  
  


 
      

   


       

              
      

 
    

