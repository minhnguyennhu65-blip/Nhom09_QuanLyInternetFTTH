/* =======================================================
 * Tên tác giả: Nguyễn Đức Huy
 * Mã sinh viên: B24DCVT176
 * Mô tả file: Lớp nhân viên kỹ thuật
 * ======================================================= */

#ifndef NHAN_VIEN_KY_THUAT_H
#define NHAN_VIEN_KY_THUAT_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class NhanVienKyThuat : public LopCoSo {
private:
    std::string hoTen;
    std::string soDienThoai;
    std::string chuyenMon;
    std::string khuVucPhuTrach;
    std::string trangThai = "Dang lam";

    // Kiểm tra mã
    static bool hopLeMa(const std::string& s) {
        if (s.empty() || s.find('|') != std::string::npos)
            return false;

        for (unsigned char c : s) {
            if (std::isspace(c) || std::islower(c))
                return false;
        }

        return true;
    }

    // Kiểm tra chuỗi không rỗng
    static bool hopLeChuoi(const std::string& s) {
        return !s.empty() &&
               s.find('|') == std::string::npos;
    }

    // Kiểm tra số điện thoại khi đọc từ file
    static bool hopLeSoDienThoai(const std::string& s) {
        if (s.length() != 10 || s[0] != '0')
            return false;

        for (unsigned char c : s) {
            if (!std::isdigit(c))
                return false;
        }

        return true;
    }

    // Kiểm tra trạng thái
    static bool hopLeTrangThai(const std::string& tt) {
        return tt == "Dang lam" ||
               tt == "Nghi";
    }

    // Ném ngoại lệ nếu dữ liệu không hợp lệ
    static void yeuCau(
        bool dieuKien,
        const std::string& thongBao) {

        if (!dieuKien)
            throw std::invalid_argument(thongBao);
    }

public:
    NhanVienKyThuat() = default;

    // Getter
    const std::string& getHoTen() const {
        return hoTen;
    }

    const std::string& getSoDienThoai() const {
        return soDienThoai;
    }

    const std::string& getChuyenMon() const {
        return chuyenMon;
    }

    const std::string& getKhuVucPhuTrach() const {
        return khuVucPhuTrach;
    }

    const std::string& getTrangThai() const {
        return trangThai;
    }

    // Cập nhật trạng thái
    void setTrangThai(const std::string& tt) {
        yeuCau(
            hopLeTrangThai(tt),
            "Trang thai nhan vien khong hop le: " + tt
        );

        trangThai = tt;
    }

    // Nhập thông tin
    void nhapThongTin() override {
        maDinhDanh =
            NhapDuLieu::nhapMaDinhDanh(
                "Nhap ma nhan vien: "
            );

        hoTen =
            NhapDuLieu::nhapChuoi(
                "Nhap ho ten: "
            );

        soDienThoai =
            NhapDuLieu::nhapSoDienThoai(
                "Nhap so dien thoai: "
            );

        chuyenMon =
            NhapDuLieu::nhapChuoi(
                "Nhap chuyen mon: "
            );

        khuVucPhuTrach =
            NhapDuLieu::nhapChuoi(
                "Nhap khu vuc phu trach: "
            );

        while (true) {
            trangThai =
                NhapDuLieu::nhapChuoi(
                    "Nhap trang thai (Dang lam/Nghi): "
                );

            if (hopLeTrangThai(trangThai))
                break;

            std::cout
                << " -> Loi: Trang thai khong hop le!\n";
        }
    }

    // Cập nhật thông tin
    // Không cho phép thay đổi mã nhân viên
    void capNhatThongTin() override {
        hoTen =
            NhapDuLieu::nhapChuoi(
                "Nhap ho ten moi: "
            );

        soDienThoai =
            NhapDuLieu::nhapSoDienThoai(
                "Nhap so dien thoai moi: "
            );

        chuyenMon =
            NhapDuLieu::nhapChuoi(
                "Nhap chuyen mon moi: "
            );

        khuVucPhuTrach =
            NhapDuLieu::nhapChuoi(
                "Nhap khu vuc phu trach moi: "
            );

        while (true) {
            trangThai =
                NhapDuLieu::nhapChuoi(
                    "Nhap trang thai moi (Dang lam/Nghi): "
                );

            if (hopLeTrangThai(trangThai))
                break;

            std::cout
                << " -> Loi: Trang thai khong hop le!\n";
        }
    }

    // Hiển thị thông tin
    void hienThiThongTin() const override {
        std::cout
            << "Ma NV: " << maDinhDanh
            << " | Ho ten: " << hoTen
            << " | SDT: " << soDienThoai
            << " | Chuyen mon: " << chuyenMon
            << " | Khu vuc: " << khuVucPhuTrach
            << " | Trang thai: " << trangThai
            << "\n";
    }

    // Chuyển thành chuỗi lưu file
    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" +
               hoTen + "|" +
               soDienThoai + "|" +
               chuyenMon + "|" +
               khuVucPhuTrach + "|" +
               trangThai;
    }

    // Đọc dữ liệu từ file
    void docTuChuoi(const std::string& dong) override {
        yeuCau(
            std::count(dong.begin(), dong.end(), '|') == 5,
            "Dong du lieu NhanVienKyThuat co so cot khong dung."
        );

        std::string f[6];
        std::stringstream ss(dong);

        for (int i = 0; i < 6; ++i)
            std::getline(ss, f[i], '|');

        yeuCau(
            hopLeMa(f[0]),
            "Ma nhan vien khong hop le: " + f[0]
        );

        yeuCau(
            hopLeChuoi(f[1]),
            "Ho ten khong hop le."
        );

        yeuCau(
            hopLeSoDienThoai(f[2]),
            "So dien thoai khong hop le."
        );

        yeuCau(
            hopLeChuoi(f[3]),
            "Chuyen mon khong hop le."
        );

        yeuCau(
            hopLeChuoi(f[4]),
            "Khu vuc phu trach khong hop le."
        );

        yeuCau(
            hopLeTrangThai(f[5]),
            "Trang thai nhan vien khong hop le: " + f[5]
        );

        maDinhDanh = f[0];
        hoTen = f[1];
        soDienThoai = f[2];
        chuyenMon = f[3];
        khuVucPhuTrach = f[4];
        trangThai = f[5];
    }
};

#endif
