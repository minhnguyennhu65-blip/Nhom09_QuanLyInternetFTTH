/* =======================================================
 * Tên tác giả: Hoàng Trọng Khoa
 * Mã sinh viên: B24DCVT204
 * Mô tả file: Lớp quản lý Hợp Đồng
 * ======================================================= */

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

    static constexpr const char* TT_DANG_SU_DUNG = "Dang su dung";
    static constexpr const char* TT_KHOA = "Khoa";
    static constexpr const char* TT_HUY = "Huy";

    // Hỗ trợ xóa ký tự '\r' thừa nếu file ghi theo chuẩn Windows
    static void xoaKyTuXuongDong(std::string& s) {
        if (!s.empty() && s.back() == '\r') {
            s.pop_back();
        }
    }

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == TT_DANG_SU_DUNG || tt == TT_KHOA || tt == TT_HUY;
    }

    // Khớp logic NhapDuLieu::nhapMaDinhDanh: Không rỗng, không chứa '|' và khoảng trắng
    static bool hopLeMa(const std::string& ma) {
        return !ma.empty() && ma.find('|') == std::string::npos &&
               std::none_of(ma.begin(), ma.end(), [](unsigned char c) {
                   return std::isspace(c);
               });
    }

    // Khớp logic NhapDuLieu::nhapSoDienThoai (10 chữ số, bắt đầu bằng '0')
    static bool hopLeSoDienThoai(const std::string& s) {
        return s.size() == 10 && s[0] == '0' &&
               std::all_of(s.begin(), s.end(), [](unsigned char c) {
                   return std::isdigit(c) != 0;
               });
    }

    // Khớp logic NhapDuLieu::nhapChuoi: không rỗng, không '|'
    static bool hopLeChuoi(const std::string& s) {
        return !s.empty() && s.find('|') == std::string::npos;
    }

    static bool hopLeEmail(const std::string& e) {
        if (!hopLeChuoi(e)) return false;
        std::size_t at = e.find('@');
        if (at == std::string::npos || at == 0 || e.find('@', at + 1) != std::string::npos) {
            return false;
        }
        std::size_t dot = e.find('.', at + 1);
        return dot != std::string::npos && dot > at + 1 && e.back() != '.' &&
               std::none_of(e.begin(), e.end(), [](unsigned char c) {
                   return std::isspace(c) != 0;
               });
    }

public:
    KhachHang()
        : LopCoSo(),
          tenKhachHang(),
          soDienThoai(),
          diaChiLapDat(),
          email(),
          trangThai(TT_DANG_SU_DUNG) {}

    // Getters
    const std::string& getTenKhachHang() const { return tenKhachHang; }
    const std::string& getSoDienThoai() const { return soDienThoai; }
    const std::string& getDiaChiLapDat() const { return diaChiLapDat; }
    const std::string& getEmail() const { return email; }
    const std::string& getTrangThai() const { return trangThai; }

    // Setters
    void setTenKhachHang(const std::string& ten) {
        if (!hopLeChuoi(ten)) throw std::invalid_argument("Ten khach hang khong hop le.");
        tenKhachHang = ten;
    }

    void setSoDienThoai(const std::string& sdt) {
        if (!hopLeSoDienThoai(sdt)) throw std::invalid_argument("So dien thoai khong hop le.");
        soDienThoai = sdt;
    }

    void setDiaChiLapDat(const std::string& diachi) {
        if (!hopLeChuoi(diachi)) throw std::invalid_argument("Dia chi khong hop le.");
        diaChiLapDat = diachi;
    }

    void setEmail(const std::string& em) {
        if (!hopLeEmail(em)) throw std::invalid_argument("Email khong hop le.");
        email = em;
    }

    void setTrangThai(const std::string& tt) {
        if (!hopLeTrangThai(tt)) {
            throw std::invalid_argument("Trang thai khach hang khong hop le: " + tt);
        }
        trangThai = tt;
    }

    // --- Override các phương thức từ LopCoSo ---

    void nhapThongTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang (maKH): ");
        tenKhachHang = NhapDuLieu::nhapChuoi("Nhap ten khach hang: ");
        soDienThoai = NhapDuLieu::nhapSoDienThoai(
            "Nhap so dien thoai (10 chu so, bat dau bang 0): ");
        diaChiLapDat = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat: ");

        while (true) {
            email = NhapDuLieu::nhapChuoi("Nhap email: ");
            if (hopLeEmail(email)) break;
            std::cout << " -> Loi: Email khong hop le (VD: ten@congty.vn)!\n";
        }

        while (true) {
            trangThai = NhapDuLieu::nhapChuoi(
                std::string("Nhap trang thai (") + TT_DANG_SU_DUNG + "/" +
                TT_KHOA + "/" + TT_HUY + "): ");
            if (hopLeTrangThai(trangThai)) break;
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

    // [Bổ sung] Triển khai hàm capNhatThongTin() từ LopCoSo
    void capNhatThongTin() override {
        std::cout << "\n--- CAP NHAT THONG TIN KHACH HANG (" << maDinhDanh << ") ---\n";
        tenKhachHang = NhapDuLieu::nhapChuoi("Nhap ten khach hang moi: ");
        soDienThoai = NhapDuLieu::nhapSoDienThoai("Nhap so dien thoai moi (10 chu so, bat dau bang 0): ");
        diaChiLapDat = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat moi: ");

        while (true) {
            email = NhapDuLieu::nhapChuoi("Nhap email moi: ");
            if (hopLeEmail(email)) break;
            std::cout << " -> Loi: Email khong hop le (VD: ten@congty.vn)!\n";
        }

        while (true) {
            trangThai = NhapDuLieu::nhapChuoi(
                std::string("Nhap trang thai moi (") + TT_DANG_SU_DUNG + "/" +
                TT_KHOA + "/" + TT_HUY + "): ");
            if (hopLeTrangThai(trangThai)) break;
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }
        std::cout << " -> Cap nhat thong tin thanh cong!\n";
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
        std::
