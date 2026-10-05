#ifndef HOP_DONG_H
#define HOP_DONG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class HopDong : public LopCoSo {
private:
    std::string maKhachHang;
    std::string maGoiCuoc;
    std::string maThietBi;
    std::string ngayKy;
    std::string ngayHetHan;
    std::string trangThai;  // Hoat dong / Het han / Huy
    double giaThieuDung;

    static const std::string TRANG_THAI_HOAT_DONG;
    static const std::string TRANG_THAI_HET_HAN;
    static const std::string TRANG_THAI_HUY;

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == TRANG_THAI_HOAT_DONG || 
               tt == TRANG_THAI_HET_HAN || 
               tt == TRANG_THAI_HUY;
    }

    static std::string soThucThanhChuoi(double v) {
        std::ostringstream os;
        os << std::fixed << std::setprecision(2) << v;
        return os.str();
    }

    static double docSoThuc(const std::string& s) {
        if (s.empty()) {
            throw std::invalid_argument("Gia tri so thuc rong!");
        }
        try {
            size_t pos = 0;
            double v = std::stod(s, &pos);
            if (pos != s.size()) {
                throw std::invalid_argument("Gia tri so thuc khong hop le: " + s);
            }
            if (v < 0) {
                throw std::invalid_argument("Gia tri so phai >= 0!");
            }
            return v;
        } catch (const std::exception&) {
            throw std::invalid_argument("Gia tri so thuc khong hop le: " + s);
        }
    }

public:
    HopDong() : LopCoSo() {
        maKhachHang = "";
        maGoiCuoc = "";
        maThietBi = "";
        ngayKy = "";
        ngayHetHan = "";
        trangThai = TRANG_THAI_HOAT_DONG;
        giaThieuDung = 0.0;
    }

    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getMaGoiCuoc() const { return maGoiCuoc; }
    std::string getMaThietBi() const { return maThietBi; }
    std::string getNgayKy() const { return ngayKy; }
    std::string getNgayHetHan() const { return ngayHetHan; }
    std::string getTrangThai() const { return trangThai; }
    double getGiaThieuDung() const { return giaThieuDung; }

    void setTrangThai(const std::string& tt) {
        if (!hopLeTrangThai(tt)) {
            throw std::invalid_argument("Trang thai hop dong khong hop le!");
        }
        trangThai = tt;
    }

    void nhapThongTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong (maHD): ");
        maKhachHang = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang: ");
        maGoiCuoc = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi cuoc: ");
        maThietBi = NhapDuLieu::nhapMaDinhDanh("Nhap ma thiet bi: ");
        ngayKy = NhapDuLieu::nhapNgayThang("Nhap ngay ky hop dong (DD/MM/YYYY): ");
        ngayHetHan = NhapDuLieu::nhapNgayThang("Nhap ngay het han hop dong (DD/MM/YYYY): ");
        giaThieuDung = NhapDuLieu::nhapSoThucDuong("Nhap gia thieu dung hang thang (>= 0): ");

        while (true) {
            std::string tt = NhapDuLieu::nhapChuoi(
                "Nhap trang thai (" + TRANG_THAI_HOAT_DONG + "/" + 
                TRANG_THAI_HET_HAN + "/" + TRANG_THAI_HUY + "): ");
            if (hopLeTrangThai(tt)) {
                trangThai = tt;
                break;
            }
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }
    }

    void hienThiThongTin() const override {
        std::cout << "Ma HD: " << maDinhDanh
                  << " | Ma KH: " << maKhachHang
                  << " | Ma Goi: " << maGoiCuoc
                  << " | Ma TB: " << maThietBi
                  << " | Ngay ky: " << ngayKy
                  << " | Ngay het han: " << ngayHetHan
                  << " | Gia thieu dung: " << giaThieuDung
                  << " | Trang thai: " << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + maKhachHang + "|" + maGoiCuoc + "|" +
               maThietBi + "|" + ngayKy + "|" + ngayHetHan + "|" +
               soThucThanhChuoi(giaThieuDung) + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        if (std::count(dong.begin(), dong.end(), '|') != 7) {
            throw std::invalid_argument("Dong du lieu HopDong co so cot khong dung.");
        }

        std::stringstream ss(dong);
        std::string ma, makv, magoi, matb, nky, nhethan, gia, tt;
        std::getline(ss, ma, '|');
        std::getline(ss, makv, '|');
        std::getline(ss, magoi, '|');
        std::getline(ss, matb, '|');
        std::getline(ss, nky, '|');
        std::getline(ss, nhethan, '|');
        std::getline(ss, gia, '|');
        std::getline(ss, tt, '|');

        if (ma.empty() || makv.empty() || magoi.empty() || matb.empty()) {
            throw std::invalid_argument("Ma dinh danh khong duoc de trong.");
        }
        if (nky.empty() || nhethan.empty()) {
            throw std::invalid_argument("Ngay khong duoc de trong.");
        }
        if (gia.empty()) {
            throw std::invalid_argument("Gia thieu dung khong duoc de trong.");
        }
        if (!hopLeTrangThai(tt)) {
            throw std::invalid_argument("Trang thai hop dong khong hop le: " + tt);
        }

        maDinhDanh = ma;
        maKhachHang = makv;
        maGoiCuoc = magoi;
        maThietBi = matb;
        ngayKy = nky;
        ngayHetHan = nhethan;
        giaThieuDung = docSoThuc(gia);
        trangThai = tt;
    }
};

const std::string HopDong::TRANG_THAI_HOAT_DONG = "Hoat dong";
const std::string HopDong::TRANG_THAI_HET_HAN = "Het han";
const std::string HopDong::TRANG_THAI_HUY = "Huy";

#endif
