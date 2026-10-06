#ifndef NHAN_VIEN_KY_THUAT_H
#define NHAN_VIEN_KY_THUAT_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

class NhanVienKyThuat : public LopCoSo {
private:
    std::string hoTen;
    std::string soDienThoai;
    std::string chuyenMon;
    std::string khuVucPhuTrach;
    std::string trangThai;

    bool soDienThoaiHopLe(const std::string& s) const {
        if (s.length() != 10 || s[0] != '0')
            return false;

        for (char c : s) {
            if (!std::isdigit(static_cast<unsigned char>(c)))
                return false;
        }

        return true;
    }

    bool trangThaiHopLe(const std::string& s) const {
        return s == "Dang lam" || s == "Nghi";
    }

public:
    NhanVienKyThuat()
        : LopCoSo(),
          trangThai("Dang lam") {
    }

    void nhapThongTin() override {
        std::cout << "\n===== NHAP NHAN VIEN KY THUAT =====\n";

        while (true) {
            maDinhDanh = NhapDuLieu::nhapChuoi(
                "Nhap ma nhan vien: ");

            if (maDinhDanh.find('|') == std::string::npos)
                break;

            std::cout << " -> Loi: Ma khong duoc chua '|'.\n";
        }

        hoTen = NhapDuLieu::nhapChuoi("Nhap ho ten: ");

        while (true) {
            soDienThoai = NhapDuLieu::nhapChuoi("Nhap so dien thoai: ");

            if (soDienThoaiHopLe(soDienThoai))
                break;

            std::cout
                << " -> Loi: So dien thoai phai co 10 chu so "
                   "va bat dau bang 0.\n";
        }

        chuyenMon = NhapDuLieu::nhapChuoi("Nhap chuyen mon: ");
        khuVucPhuTrach = NhapDuLieu::nhapChuoi("Nhap khu vuc phu trach: ");
        while (true) {
            trangThai = NhapDuLieu::nhapChuoi(
                "Nhap trang thai (Dang lam/Nghi): ");
            if (trangThaiHopLe(trangThai))
                break;
            std::cout
                << " -> Loi: Trang thai khong hop le!\n";
        }
    }

    void hienThiThongTin() const override {
        std::cout << "\n===== NHAN VIEN KY THUAT =====\n";
        std::cout << "Ma nhan vien: " << maDinhDanh << '\n';
        std::cout << "Ho ten: " << hoTen << '\n';
        std::cout << "So dien thoai: " << soDienThoai << '\n';
        std::cout << "Chuyen mon: " << chuyenMon << '\n';
        std::cout << "Khu vuc phu trach: "<< khuVucPhuTrach << '\n';
        std::cout << "Trang thai: " << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" +
               hoTen + "|" +
               soDienThoai + "|" +
               chuyenMon + "|" +
               khuVucPhuTrach + "|" +
               trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        std::stringstream ss(dong);
        std::vector<std::string> f;
        std::string x;

        while (std::getline(ss, x, '|'))
            f.push_back(x);

        if (f.size() != 6)
            throw std::invalid_argument("Dong du lieu khong hop le!");

        for (int i = 0; i < 5; i++) {
            if (f[i].empty())
                throw std::invalid_argument("Du lieu khong duoc de trong!");
        }
        if (!soDienThoaiHopLe(f[2]))
            throw std::invalid_argument("So dien thoai khong hop le!");

        if (!trangThaiHopLe(f[5]))
            throw std::invalid_argument("Trang thai khong hop le!");

        maDinhDanh = f[0];
        hoTen = f[1];
        soDienThoai = f[2];
        chuyenMon = f[3];
        khuVucPhuTrach = f[4];
        trangThai = f[5];
    }
    std::string getMaNhanVien() const {
        return maDinhDanh;
    }
    std::string getHoTen() const {
        return hoTen;
    }
    std::string getSoDienThoai() const {
        return soDienThoai;
    }
    std::string getChuyenMon() const {
        return chuyenMon;
    }
    std::string getKhuVucPhuTrach() const {
        return khuVucPhuTrach;
    }
    std::string getTrangThai() const {
        return trangThai;
    }
};

#endif
