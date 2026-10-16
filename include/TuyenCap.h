/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 02 - Quản lý Tuyến Cáp
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
    std::string diaDiemDau;
    std::string diaDiemCuoi;
    std::string trangThai;  // "Hoat dong" / "Bao tri"

    static const std::vector<std::string>& danhSachTrangThai() {
        static const std::vector<std::string> ds = {"Hoat dong", "Bao tri"};
        return ds;
    }

    // Nhap cac thuoc tinh (khong gom ma dinh danh) - dung chung cho Them va Sua
    void nhapThuocTinh() {
        tenTuyen = NhapDuLieu::nhapChuoi("Nhap ten tuyen cap: ");
        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc phuc vu: ");
        diaDiemDau = NhapDuLieu::nhapChuoi("Nhap diem dau: ");
        diaDiemCuoi = NhapDuLieu::nhapChuoi("Nhap diem cuoi: ");
        trangThai = chonTuDanhSach("Chon trang thai:", danhSachTrangThai());
    }

    // ===== Ham tien ich noi bo (private) =====
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
    static void kiemTraKhongRong(const std::string& s, const std::string& ten) {
        if (s.find_first_not_of(" \t") == std::string::npos)
            throw std::runtime_error(ten + " khong duoc de trong");
    }
    static void kiemTraMa(const std::string& s, const std::string& ten) {
        kiemTraKhongRong(s, ten);
        if (s.find_first_of(" \t") != std::string::npos)
            throw std::runtime_error(ten + " khong duoc chua khoang trang");
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

public:
    TuyenCap() : LopCoSo(), tenTuyen(""), khuVuc(""), diaDiemDau(""), diaDiemCuoi(""), trangThai("") {}

    std::string getMaTuyen() const { return maDinhDanh; }
    std::string getTenTuyen() const { return tenTuyen; }
    std::string getKhuVuc() const { return khuVuc; }
    std::string getDiaDiemDau() const { return diaDiemDau; }
    std::string getDiaDiemCuoi() const { return diaDiemCuoi; }
    std::string getTrangThai() const { return trangThai; }

    // Nhap thong tin moi. Neu ma chua duoc gan (rong) thi hoi ma truoc.
    void nhapThongTin() override {
        if (maDinhDanh.empty()) {
            maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen (maTuyen): ");
        }
        nhapThuocTinh();
    }

    // Sua: KHOA ma dinh danh, chi nhap lai cac thuoc tinh con lai
    void capNhatThongTin() override {
        std::cout << "(Ma tuyen " << maDinhDanh << " duoc khoa, khong the thay doi)\n";
        nhapThuocTinh();
    }

    static void inTieuDeBang() {
        std::cout << std::left
                  << std::setw(10) << "Ma tuyen"
                  << std::setw(22) << "Ten tuyen"
                  << std::setw(18) << "Khu vuc"
                  << std::setw(20) << "Diem dau"
                  << std::setw(20) << "Diem cuoi"
                  << std::setw(12) << "Trang thai" << "\n"
                  << std::string(102, '-') << "\n";
    }

    // Moi doi tuong in thanh 1 dong cua bang
    void hienThiThongTin() const override {
        std::cout << std::left
                  << std::setw(10) << catChuoi(maDinhDanh, 9)
                  << std::setw(22) << catChuoi(tenTuyen, 21)
                  << std::setw(18) << catChuoi(khuVuc, 17)
                  << std::setw(20) << catChuoi(diaDiemDau, 19)
                  << std::setw(20) << catChuoi(diaDiemCuoi, 19)
                  << std::setw(12) << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenTuyen + "|" + khuVuc + "|" +
               diaDiemDau + "|" + diaDiemCuoi + "|" + trangThai;
    }

    // Doc 1 dong file. Dong loi -> throw de QuanLyLuuTru bo qua dong do.
    void docTuChuoi(const std::string& dongDuLieu) override {
        std::vector<std::string> f = tachTruong(dongDuLieu);
        if (f.size() != 6) {
            throw std::runtime_error("Sai so cot (can 6, co " + std::to_string(f.size()) + ")");
        }
        kiemTraKhongRong(f[0], "maTuyen");
        if (f[0].find_first_of(" \t") != std::string::npos) {
            throw std::runtime_error("maTuyen khong duoc chua khoang trang");
        }
        kiemTraKhongRong(f[1], "tenTuyen");
        kiemTraKhongRong(f[2], "khuVuc");
        kiemTraKhongRong(f[3], "diaDiemDau");
        kiemTraKhongRong(f[4], "diaDiemCuoi");
        if (!thuocDanhSach(f[5], danhSachTrangThai())) {
            throw std::runtime_error("trangThai khong hop le: " + f[5]);
        }

        maDinhDanh = f[0];
        tenTuyen = f[1];
        khuVuc = f[2];
        diaDiemDau = f[3];
        diaDiemCuoi = f[4];
        trangThai = f[5];
    }
};


// =====================================================
// MENU USE CASE
// =====================================================
namespace UC02 {

    const std::string FILE_TUYEN_CAP = "data/tuyen_cap.txt";

    // 1. THEM: nhap ma -> kiem tra ton tai -> nhap thuoc tinh -> them -> luu file
    inline void themTuyen(QuanLyLuuTru<TuyenCap>& ql) {
        std::cout << "\n--- THEM TUYEN CAP ---\n";
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen (maTuyen): ");
        if (ql.timTheoMa(ma) != nullptr) {
            std::cout << " -> Loi: Ma tuyen '" << ma << "' da ton tai! Khong them du lieu.\n";
            return;
        }
        TuyenCap tuyen;
        tuyen.setMaDinhDanh(ma);
        tuyen.nhapThongTin();
        ql.themMoi(tuyen);
    }

    // 2. HIEN THI: doc lai tu file -> hien thi dang bang
    inline void hienThiDanhSach(QuanLyLuuTru<TuyenCap>& ql) {
        std::cout << "\n--- DANH SACH TUYEN CAP ---\n";
        ql.docTuFile();
        if (ql.getDanhSach().empty()) {
            std::cout << " -> Danh sach rong!\n";
            return;
        }
        TuyenCap::inTieuDeBang();
        ql.hienThi();
        std::cout << "Tong so tuyen: " << ql.getDanhSach().size() << "\n";
    }

    // 3. TIM KIEM: nhap ma -> co thi hien thi, khong thi bao khong tim thay
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
    }

    // 4. SUA: nhap ma -> tim -> (khong ton tai: bao loi) -> nhap thong tin moi (khoa ma) -> luu file
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

    // 5. XOA: nhap ma -> tim -> xac nhan -> xoa -> luu file
    // (Theo bang phan cong hien tai chua co bang nao tham chieu maTuyen nen khong can kiem tra khoa ngoai)
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

    // Menu con UC02
    inline void chayMenu() {
        try { std::filesystem::create_directories("data"); } catch (...) {}   // dam bao thu muc data/ ton tai
        QuanLyLuuTru<TuyenCap> ql(FILE_TUYEN_CAP);   // doc file khi khoi dong (tao file moi neu chua co)
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

 
        
     


         
