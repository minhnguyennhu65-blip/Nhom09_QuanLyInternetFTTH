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

    void nhapThuocTinh() {
        tenTuyen = NhapDuLieu::nhapChuoi("Nhap ten tuyen cap: ");
        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc phuc vu: ");
        diaDiemDau = NhapDuLieu::nhapChuoi("Nhap diem dau: ");
        diaDiemCuoi = NhapDuLieu::nhapChuoi("Nhap diem cuoi: ");
        trangThai = chonTuDanhSach("Chon trang thai:", danhSachTrangThai());
    }

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

public:
    TuyenCap() : LopCoSo(), tenTuyen(""), khuVuc(""), diaDiemDau(""), diaDiemCuoi(""), trangThai("") {}

    std::string getMaTuyen() const { return maDinhDanh; }
std::string getTenTuyen() const { return tenTuyen; }
    std::string getKhuVuc() const { return khuVuc; }
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
                  << std::setw(22) << "Ten tuyen"
                  << std::setw(18) << "Khu vuc"
                  << std::setw(20) << "Diem dau"
                  << std::setw(20) << "Diem cuoi"
                  << std::setw(12) << "Trang thai" << "\n"
                  << std::string(102, '-') << "\n";
    }

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

#endif

  




  

 
         

 
        
     


         
