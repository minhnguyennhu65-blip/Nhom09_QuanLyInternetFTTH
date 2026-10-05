#ifndef KHACH_HANG_H
#define KHACH_HANG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class KhachHang : public LopCoSo {
private:
    std::string tenKhachHang;
    std::string soDienThoai;
    std::string diaChiLapDat;
    std::string email;
    std::string trangThai;  // Dang su dung / Khoa / Huy

    static const std::string TRANG_THAI_DANG_SU_DUNG;
    static const std::string TRANG_THAI_KHOA;
    static const std::string TRANG_THAI_HUY;

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == TRANG_THAI_DANG_SU_DUNG || 
               tt == TRANG_THAI_KHOA || 
               tt == TRANG_THAI_HUY;
    }

public:
    KhachHang() : LopCoSo() {
        tenKhachHang = "";
        soDienThoai = "";
        diaChiLapDat = "";
        email = "";
        trangThai = TRANG_THAI_DANG_SU_DUNG;
    }

    std::string getTenKhachHang() const { return tenKhachHang; }
    std::string getSoDienThoai() const { return soDienThoai; }
    std::string getDiaChiLapDat() const { return diaChiLapDat; }
    std::string getEmail() const { return email; }
    std::string getTrangThai() const { return trangThai; }

    void setTrangThai(const std::string& tt) {
        if (!hopLeTrangThai(tt)) {
            throw std::invalid_argument("Trang thai khach hang khong hop le!");
        }
        trangThai = tt;
    }

    void nhapThongTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang (maKH): ");
        tenKhachHang = NhapDuLieu::nhapChuoi("Nhap ten khach hang: ");
        soDienThoai = NhapDuLieu::nhapSoDienThoai("Nhap so dien thoai (10 chu so, bat dau bang 0): ");
        diaChiLapDat = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat: ");
        email = NhapDuLieu::nhapChuoi("Nhap email: ");

        while (true) {
            std::string tt = NhapDuLieu::nhapChuoi(
                "Nhap trang thai (" + TRANG_THAI_DANG_SU_DUNG + "/" + 
                TRANG_THAI_KHOA + "/" + TRANG_THAI_HUY + "): ");
            if (hopLeTrangThai(tt)) {
                trangThai = tt;
                break;
            }
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }
    }

    void hienThiThongTin() const override {
        std::cout << "Ma KH: " << maDinhDanh
                  << " | Ten: " << tenKhachHang
                  << " | SDT: " << soDienThoai
                  << " | Dia chi: " << diaChiLapDat
                  << " | Email: " << email
                  << " | Trang thai: " << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenKhachHang + "|" + soDienThoai + "|" +
               diaChiLapDat + "|" + email + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        if (std::count(dong.begin(), dong.end(), '|') != 5) {
            throw std::invalid_argument("Dong du lieu KhachHang co so cot khong dung.");
        }

        std::stringstream ss(dong);
        std::string ma, ten, sdt, diachi, em, tt;
        std::getline(ss, ma, '|');
        std::getline(ss, ten, '|');
        std::getline(ss, sdt, '|');
        std::getline(ss, diachi, '|');
        std::getline(ss, em, '|');
        std::getline(ss, tt, '|');

        if (ma.empty()) throw std::invalid_argument("Ma khach hang khong duoc de trong.");
        if (ten.empty()) throw std::invalid_argument("Ten khach hang khong duoc de trong.");
        if (sdt.length() != 10 || sdt[0] != '0') throw std::invalid_argument("So dien thoai khong hop le: " + sdt);
        if (diachi.empty()) throw std::invalid_argument("Dia chi khong duoc de trong.");
        if (em.empty()) throw std::invalid_argument("Email khong duoc de trong.");
        if (!hopLeTrangThai(tt)) throw std::invalid_argument("Trang thai khach hang khong hop le: " + tt);

        maDinhDanh = ma;
        tenKhachHang = ten;
        soDienThoai = sdt;
        diaChiLapDat = diachi;
        email = em;
        trangThai = tt;
    }
};

const std::string KhachHang::TRANG_THAI_DANG_SU_DUNG = "Dang su dung";
const std::string KhachHang::TRANG_THAI_KHOA = "Khoa";
const std::string KhachHang::TRANG_THAI_HUY = "Huy";

#endif
