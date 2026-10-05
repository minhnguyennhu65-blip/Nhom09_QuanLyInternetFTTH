#ifndef KHACH_HANG_H
#define KHACH_HANG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <algorithm>
#include <cctype>
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

    // Enum trang thai hop le
    static const std::string TRANG_THAI_DANG_SU_DUNG;
    static const std::string TRANG_THAI_KHOA;
    static const std::string TRANG_THAI_HUY;

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == TRANG_THAI_DANG_SU_DUNG || 
               tt == TRANG_THAI_KHOA || 
               tt == TRANG_THAI_HUY;
    }

    // QUY TAC 4: So dien thoai phai co dung 10 chu so va bat dau bang 0
    static bool hopLeSoDienThoai(const std::string& sdt) {
        if (sdt.length() != 10) {
            throw std::invalid_argument("[Loi QT4] So dien thoai phai co dung 10 chu so!");
        }
        if (sdt[0] != '0') {
            throw std::invalid_argument("[Loi QT4] So dien thoai phai bat dau bang so 0!");
        }
        for (char c : sdt) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                throw std::invalid_argument("[Loi QT4] So dien thoai chi chua chu so!");
            }
        }
        return true;
    }

    // QUY TAC 1: Ma dinh danh - chi gom chu HOA va chu so
    static bool hopLeMa(const std::string& ma) {
        if (ma.empty()) {
            throw std::invalid_argument("[Loi QT1] Ma dinh danh khong duoc de trong!");
        }
        for (unsigned char ch : ma) {
            if (!(std::isupper(ch) || std::isdigit(ch))) {
                throw std::invalid_argument("[Loi QT1] Ma chi gom chu HOA (A-Z) va chu so (0-9)!");
            }
        }
        return true;
    }

    // QUY TAC 2: Chuoi - khong rong va khong chua '|'
    static bool hopLeChuoi(const std::string& s) {
        if (s.empty()) {
            throw std::invalid_argument("[Loi QT2] Thong tin khong duoc de trong!");
        }
        if (s.find('|') != std::string::npos) {
            throw std::invalid_argument("[Loi QT2] Khong duoc chua ky tu '|'!");
        }
        return true;
    }

public:
    KhachHang() : LopCoSo() {
        tenKhachHang = "";
        soDienThoai = "";
        diaChiLapDat = "";
        email = "";
        trangThai = TRANG_THAI_DANG_SU_DUNG;
    }

    // Getter
    std::string getTenKhachHang() const { return tenKhachHang; }
    std::string getSoDienThoai() const { return soDienThoai; }
    std::string getDiaChiLapDat() const { return diaChiLapDat; }
    std::string getEmail() const { return email; }
    std::string getTrangThai() const { return trangThai; }

    // Setter - Quy tac 1: Khoa cung ma dinh danh khi sua
    void setTrangThai(const std::string& tt) {
        if (!hopLeTrangThai(tt)) {
            throw std::invalid_argument("[Loi QT6] Trang thai khach hang phai la: " 
                + TRANG_THAI_DANG_SU_DUNG + ", " + TRANG_THAI_KHOA + " hoac " + TRANG_THAI_HUY);
        }
        trangThai = tt;
    }

    void nhapThongTin() override {
        // QUY TAC 1: Nhap ma khach hang (kiem tra sau trong he thong quan ly)
        while (true) {
            try {
                maDinhDanh = NhapDuLieu::nhapChuoi("Nhap ma khach hang (maKH, VIET HOA/chu so): ");
                hopLeMa(maDinhDanh);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // Nhap ten khach hang
        while (true) {
            try {
                tenKhachHang = NhapDuLieu::nhapChuoi("Nhap ten khach hang: ");
                hopLeChuoi(tenKhachHang);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 4: Nhap so dien thoai (10 chu so, bat dau bang 0)
        while (true) {
            try {
                soDienThoai = NhapDuLieu::nhapChuoi("Nhap so dien thoai (10 chu so, bat dau bang 0): ");
                hopLeSoDienThoai(soDienThoai);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // Nhap dia chi lap dat
        while (true) {
            try {
                diaChiLapDat = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat: ");
                hopLeChuoi(diaChiLapDat);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // Nhap email
        while (true) {
            try {
                email = NhapDuLieu::nhapChuoi("Nhap email: ");
                hopLeChuoi(email);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 6: Nhap trang thai (danh sach co dinh)
        while (true) {
            try {
                trangThai = NhapDuLieu::nhapChuoi(
                    "Nhap trang thai (" + TRANG_THAI_DANG_SU_DUNG + "/" + TRANG_THAI_KHOA + "/" + TRANG_THAI_HUY + "): ");
                hopLeTrangThai(trangThai);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }
    }

    void hienThiThongTin() const override {
        std::cout << "Ma KH: " << maDinhDanh
                  << " | Ten: " << tenKhachHang
                  << " | S\u0110T: " << soDienThoai
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
            throw std::invalid_argument(
                "[Loi doc file] Dong du lieu khong dung so cot (KhachHang). Can 5 dau '|'.");
        }

        std::stringstream ss(dong);
        std::string ma, ten, sdt, diachi, em, tt;
        std::getline(ss, ma, '|');
        std::getline(ss, ten, '|');
        std::getline(ss, sdt, '|');
        std::getline(ss, diachi, '|');
        std::getline(ss, em, '|');
        std::getline(ss, tt, '|');

        // Validate tung truong
        try {
            hopLeMa(ma);
            hopLeChuoi(ten);
            hopLeSoDienThoai(sdt);
            hopLeChuoi(diachi);
            hopLeChuoi(em);
            if (!hopLeTrangThai(tt)) {
                throw std::invalid_argument("[Loi QT6] Trang thai khong hop le: " + tt);
            }
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument(std::string(e.what()) + " (dong: " + dong + ")");
        }

        // Chi gan khi toan bo du lieu hop le
        maDinhDanh = ma;
        tenKhachHang = ten;
        soDienThoai = sdt;
        diaChiLapDat = diachi;
        email = em;
        trangThai = tt;
    }
};

// Dinh nghia cac hang so trang thai
const std::string KhachHang::TRANG_THAI_DANG_SU_DUNG = "Dang su dung";
const std::string KhachHang::TRANG_THAI_KHOA = "Khoa";
const std::string KhachHang::TRANG_THAI_HUY = "Huy";

#endif