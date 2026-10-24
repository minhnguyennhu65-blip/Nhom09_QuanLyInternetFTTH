/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 02 - Quản lý Tuyến Cáp
 * ======================================================= */


 /* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 02 - Quản lý Tuyến Cáp (Bổ sung loaiKhuVuc, chieuDaiTuyenCap, loaiCap)
 * ======================================================= */

#ifndef TUYEN_CAP_H
#define TUYEN_CAP_H

#include <iostream>
#include <iomanip>
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

class TuyenCap : public LopCoSo {
private:
    std::string tenTuyen;
    std::string khuVuc;
    std::string loaiKhuVuc;     // "thanh thi", "nong thon", "vung nui"
    double chieuDaiTuyenCap;    // Chiều dài tuyến cáp (km), >= 0
    std::string loaiCap;        // Loại cáp sử dụng (VD: Quang Single-mode, Multi-mode...)
    std::string diaDiemDau;
    std::string diaDiemCuoi;
    std::string trangThai;      // "Hoat dong" / "Bao tri"

    static const std::vector<std::string>& danhSachTrangThai() {
        static const std::vector<std::string> ds = {"Hoat dong", "Bao tri"};
        return ds;
    }

    static const std::vector<std::string>& danhSachLoaiKhuVuc() {
        static const std::vector<std::string> ds = {"thanh thi", "nong thon", "vung nui"};
        return ds;
    }

    void nhapThuocTinh() {
        tenTuyen = NhapDuLieu::nhapChuoi("Nhap ten tuyen cap: ");
        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc phuc vu: ");
        loaiKhuVuc = chonTuDanhSalaryHelper("Chon loai khu vuic:", danhSachLoaiKhuVuc());
        chieuDaiTuyenCap = NhapDuLieu::nhapSoThucDuong("Nhap chieu dai tuyen cap (km): ");
        loaiCap = NhapDuLieu::nhapChuoi("Nhap loai cap su dung: ");
        diaDiemDau = NhapDuLieu::nhapChuoi("Nhap diem dau: ");
        diaDiemCuoi = NhapDuLieu::nhapChuoi("Nhap diem cuoi: ");
        trangThai = chonTuDanhSalaryHelper("Chon trang thai:", danhSachTrangThai());
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

    static std::string chonTuDanhSalaryHelper(const std::string& thongBao, const std::vector<std::string>& ds) {
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

    static double parseSoThucKhongAm(const std::string& s, const std::string& ten) {
        size_t daDoc = 0; double gt = 0;
        try { gt = std::stod(s, &daDoc); }
        catch (...) { throw std::runtime_error(ten + " phai la so thuc >= 0"); }
        if (daDoc != s.size() || !std::isfinite(gt) || gt < 0.0)
            throw std::runtime_error(ten + " phai la so thuc >= 0");
        return gt;
    }

public:
    TuyenCap() : LopCoSo(), tenTuyen(""), khuVuc(""), loaiKhuVuc(""), chieuDaiTuyenCap(0.0), loaiCap(""), diaDiemDau(""), diaDiemCuoi(""), trangThai("") {}

    std::string getMaTuyen() const { return maDinhDanh; }
    std::string getTenTuyen() const { return tenTuyen; }
    std::string getKhuVuc() const { return khuVuc; }
    std::string getLoaiKhuVuc() const { return loaiKhuVuc; }
    double getChieuDaiTuyenCap() const { return chieuDaiTuyenCap; }
    std::string getLoaiCap() const { return loaiCap; }
    std::string getDiaDiemDau() const { return diaDiemDau; }
    std::string getDiaDiemCuoi() const { return diaDiemCuoi; }
    std::string getTrangThai() const { return trangThai; }

    void nhapThongTin() override {
        if (maDinhDanh.empty()) {
            maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen (maTuyen): ");
        }
        nhapThuocTinh();
    }

    void capNhatThongTin() override {
        std::cout << "(Ma tuyen " << maDinhDanh << " duoc khoa, khong the thay doi)\n";
        nhapThuocTinh();
    }

    static void inTieuDeBang() {
        std::cout << std::left
                  << std::setw(10) << "Ma tuyen"
                  << std::setw(18) << "Ten tuyen"
                  << std::setw(15) << "Khu vuc"
                  << std::setw(14) << "Loai KV"
                  << std::setw(14) << "Chieu dai(km)"
                  << std::setw(16) << "Loai cap"
                  << std::setw(16) << "Diem dau"
                  << std::setw(16) << "Diem cuoi"
                  << std::setw(12) << "Trang thai" << "\n"
                  << std::string(131, '-') << "\n";
    }

    void hienThiThongTin() const override {
        std::cout << std::left
                  << std::setw(10) << catChuoi(maDinhDanh, 9)
                  << std::setw(18) << catChuoi(tenTuyen, 17)
                  << std::setw(15) << catChuoi(khuVuc, 14)
                  << std::setw(14) << catChuoi(loaiKhuVuc, 13)
                  << std::setw(14) << std::fixed << std::setprecision(2) << chieuDaiTuyenCap
                  << std::setw(16) << catChuoi(loaiCap, 15)
                  << std::setw(16) << catChuoi(diaDiemDau, 15)
                  << std::setw(16) << catChuoi(diaDiemCuoi, 15)
                  << std::setw(12) << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        std::ostringstream oss;
        oss << std::setprecision(2) << std::fixed << chieuDaiTuyenCap;
        return maDinhDanh + "|" + tenTuyen + "|" + khuVuc + "|" + loaiKhuVuc + "|" +
               oss.str() + "|" + loaiCap + "|" + diaDiemDau + "|" + diaDiemCuoi + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dongDuLieu) override {
        std::vector<std::string> f = tachTruong(dongDuLieu);
        if (f.size() != 9) {
            throw std::runtime_error("Sai so cot (can 9, co " + std::to_string(f.size()) + ")");
        }
        kiemTraKhongRong(f[0], "maTuyen");
        if (f[0].find_first_of(" \t") != std::string::npos) {
            throw std::runtime_error("maTuyen khong duoc chua khoang trang");
        }
        kiemTraKhongRong(f[1], "tenTuyen");
        kiemTraKhongRong(f[2], "khuVuc");
        if (!thuocDanhSach(f[3], danhSachLoaiKhuVuc())) {
            throw std::runtime_error("loaiKhuVuc khong hop le: " + f[3]);
        }
        double chieuDaiMoi = parseSoThucKhongAm(f[4], "chieuDaiTuyenCap");
        kiemTraKhongRong(f[5], "loaiCap");
        kiemTraKhongRong(f[6], "diaDiemDau");
        kiemTraKhongRong(f[7], "diaDiemCuoi");
        if (!thuocDanhSach(f[8], danhSachTrangThai())) {
            throw std::runtime_error("trangThai khong hop le: " + f[8]);
        }

        maDinhDanh = f[0];
        tenTuyen = f[1];
        khuVuc = f[2];
        loaiKhuVuc = f[3];
        chieuDaiTuyenCap = chieuDaiMoi;
        loaiCap = f[5];
        diaDiemDau = f[6];
        diaDiemCuoi = f[7];
        trangThai = f[8];
    }
};

namespace UC02 {
    const std::string FILE_TUYEN_CAP = "data/tuyen_cap.txt";

    inline void themTuyen(QuanLyLuuTru<TuyenCap>& ql) {
        std::cout << "\n--- THEM TUYEN CAP ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen (maTuyen): ");
        if (ql.timTheoMa(ma) != nullptr) {
            std::cout << " -> Loi: Ma tuyen '" << ma << "' da ton tai!\n";
            return;
        }
        TuyenCap tuyen;
        tuyen.setMaDinhDanh(ma);
        tuyen.nhapThongTin();
        ql.themMoi(tuyen);
    }

    inline void hienThiDanhSach(QuanLyLuuTru<TuyenCap>& ql) {
        std::cout << "\n--- DANH SACH TUYEN CAP ---\n";
        ql.docTuFile();
        const auto& ds = ql.getDanhSach();
        if (ds.empty()) {
            std::cout << " -> Danh sach rong!\n";
            return;
        }
        TuyenCap::inTieuDeBang();
        double tongChieuDai = 0;
        for (const auto& tuyen : ds) {
            tuyen.hienThiThongTin();
            tongChieuDai += tuyen.getChieuDaiTuyenCap();
        }
        std::cout << std::string(131, '-') << "\n";
        std::cout << " => Tong so tuyen cap: " << ds.size() 
                  << " | Tong chieu dai: " << std::fixed << std::setprecision(2) << tongChieuDai << " km\n";
    }

    inline void timKiemTuyen(QuanLyLuuTru<TuyenCap>& ql) {
        std::cout << "\n--- TIM KIEM TUYEN CAP ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can tim: ");
        TuyenCap* tuyen = ql.timTheoMa(ma);
        if (tuyen == nullptr) {
            std::cout << " -> Khong tim thay tuyen co ma: " << ma << "\n";
            return;
        }
        TuyenCap::inTieuDeBang();
        tuyen->hienThiThongTin();
        std::cout << std::string(131, '-') << "\n";
    }

    inline void suaTuyen(QuanLyLuuTru<TuyenCap>& ql) {
        std::cout << "\n--- SUA TUYEN CAP ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can sua: ");
        TuyenCap* tuyen = ql.timTheoMa(ma);
        if (tuyen == nullptr) {
            std::cout << " -> Khong tim thay tuyen co ma: " << ma << "\n";
            return;
        }
        std::cout << "Thong tin hien tai:\n";
        TuyenCap::inTieuDeBang();
        tuyen->hienThiThongTin();
        std::cout << "\nNhap thong tin moi:\n";
        ql.capNhat(ma);
    }

    inline void xoaTuyen(QuanLyLuuTru<TuyenCap>& ql) {
        std::cout << "\n--- XOA TUYEN CAP ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can xoa: ");
        TuyenCap* tuyen = ql.timTheoMa(ma);
        if (tuyen == nullptr) {
            std::cout << " -> Khong tim thay tuyen co ma: " << ma << "\n";
            return;
        }
        TuyenCap::inTieuDeBang();
        tuyen->hienThiThongTin();
        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa tuyen nay?")) {
            ql.xoaTheoMa(ma);
        } else {
            std::cout << " -> Da huy thao tac xoa.\n";
        }
    }

    inline void chayMenu() {
        try { std::filesystem::create_directories("data"); } catch (...) {}
        QuanLyLuuTru<TuyenCap> ql(FILE_TUYEN_CAP);
        while (true) {
            std::cout << "\n===== UC02 - QUAN LY TUYEN CAP/KHU VUC =====\n"
                      << "1. Them tuyen cap\n"
                      << "2. Hien thi danh sach\n"
                      << "3. Tim kiem theo ma\n"
                      << "4. Sua thong tin\n"
                      << "5. Xoa tuyen cap\n"
                      << "0. Quay lai\n";
            int chon = NhapDuLieu::nhapSoNguyenDuong("Chon chuc nang: ");
            switch (chon) {
                case 1: themTuyen(ql); break;
                case 2: hienThiDanhSach(ql); break;
                case 3: timKiemTuyen(ql); break;
                case 4: suaTuyen(ql); break;
                case 5: xoaTuyen(ql); break;
                case 0: return;
                default: std::cout << " -> Loi: Lua chon khong hop le!\n";
            }
        }
    }
}

#endif



  

 
         

 
        
     


         
