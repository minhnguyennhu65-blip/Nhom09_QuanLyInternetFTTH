/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp quản lý thiết bị ONT/Modem cấp phát
 * ======================================================= */

#ifndef THIET_BI_H
#define THIET_BI_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class ThietBi : public LopCoSo {
private:
    std::string soSerial;
    std::string loaiThietBi;
    std::string hangSanXuat;
    std::string maHopDong;
    std::string trangThai = "Kho";

    static bool hopLeMa(const std::string& s) {
        if (s.empty() || s.find('|') != std::string::npos)
            return false;

        for (unsigned char c : s) {
            if (std::isspace(c) || std::islower(c))
                return false;
        }

        return true;
    }

    static bool hopLeChuoi(const std::string& s) {
        return !s.empty() && s.find('|') == std::string::npos;
    }

    static bool hopLeLoai(const std::string& loai) {
        return loai == "ONT" || loai == "Modem";
    }

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == "Dang cap phat" ||
               tt == "Kho" ||
               tt == "Hong";
    }

    static void yeuCau(bool dieuKien, const std::string& thongBao) {
        if (!dieuKien)
            throw std::invalid_argument(thongBao);
    }

public:
    ThietBi() = default;

    const std::string& getSoSerial() const { return soSerial; }
    const std::string& getLoaiThietBi() const { return loaiThietBi; }
    const std::string& getHangSanXuat() const { return hangSanXuat; }
    const std::string& getMaHopDong() const { return maHopDong; }
    const std::string& getTrangThai() const { return trangThai; }

    void setTrangThai(const std::string& tt) {
        yeuCau(hopLeTrangThai(tt), "Trang thai thiet bi khong hop le: " + tt);
        trangThai = tt;
    }

    void nhapThongTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma thiet bi: ");
        soSerial = NhapDuLieu::nhapChuoi("Nhap so serial: ");

        while (true) {
            loaiThietBi = NhapDuLieu::nhapChuoi("Nhap loai thiet bi (ONT/Modem): ");
            if (hopLeLoai(loaiThietBi)) break;
            std::cout << " -> Loi: Loai thiet bi chi duoc la ONT hoac Modem!\n";
        }

        hangSanXuat = NhapDuLieu::nhapChuoi("Nhap hang san xuat: ");

        while (true) {
            trangThai = NhapDuLieu::nhapChuoi("Nhap trang thai (Dang cap phat/Kho/Hong): ");
            if (hopLeTrangThai(trangThai)) break;
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }

        if (trangThai == "Dang cap phat") {
            maHopDong = NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong: ");
        } else {
            maHopDong = "";
        }
    }

    void capNhatThongTin() override {
        soSerial = NhapDuLieu::nhapChuoi("Nhap so serial moi: ");

        while (true) {
            loaiThietBi = NhapDuLieu::nhapChuoi("Nhap loai thiet bi moi (ONT/Modem): ");
            if (hopLeLoai(loaiThietBi)) break;
            std::cout << " -> Loi: Loai thiet bi khong hop le!\n";
        }

        hangSanXuat = NhapDuLieu::nhapChuoi("Nhap hang san xuat moi: ");

        while (true) {
            trangThai = NhapDuLieu::nhapChuoi("Nhap trang thai moi (Dang cap phat/Kho/Hong): ");
            if (hopLeTrangThai(trangThai)) break;
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }

        if (trangThai == "Dang cap phat") {
            maHopDong = NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong moi: ");
        } else {
            maHopDong = "";
        }
    }

    void hienThiThongTin() const override {
        std::cout << "Ma TB: " << maDinhDanh
                  << " | Serial: " << soSerial
                  << " | Loai: " << loaiThietBi
                  << " | Hang SX: " << hangSanXuat
                  << " | Ma HD: " << maHopDong
                  << " | Trang thai: " << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + soSerial + "|" + loaiThietBi + "|" +
               hangSanXuat + "|" + maHopDong + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        yeuCau(std::count(dong.begin(), dong.end(), '|') == 5, "Dong du lieu ThietBi co so cot khong dung.");

        std::string f[6];
        std::stringstream ss(dong);
        for (int i = 0; i < 6; ++i) {
            std::getline(ss, f[i], '|');
        }

        yeuCau(hopLeMa(f[0]), "Ma thiet bi khong hop le: " + f[0]);
        yeuCau(hopLeChuoi(f[1]), "So serial khong hop le.");
        yeuCau(hopLeLoai(f[2]), "Loai thiet bi khong hop le: " + f[2]);
        yeuCau(hopLeChuoi(f[3]), "Hang san xuat khong hop le.");
        yeuCau(hopLeTrangThai(f[5]), "Trang thai thiet bi khong hop le: " + f[5]);

        if (f[5] == "Dang cap phat") {
            yeuCau(hopLeMa(f[4]), "Thiet bi dang cap phat phai co ma hop dong.");
        } else {
            yeuCau(f[4].empty(), "Thiet bi Kho/Hong khong duoc co ma hop dong.");
        }

        maDinhDanh = f[0];
        soSerial = f[1];
        loaiThietBi = f[2];
        hangSanXuat = f[3];
        maHopDong = f[4];
        trangThai = f[5];
    }
};

#endif
