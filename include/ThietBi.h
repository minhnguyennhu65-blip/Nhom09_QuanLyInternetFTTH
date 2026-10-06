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
    // Các thuộc tính của thiết bị
    std::string soSerial;
    std::string loaiThietBi;
    std::string hangSanXuat;
    std::string maHopDong;
    std::string trangThai = "Kho";

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
        return !s.empty() && s.find('|') == std::string::npos;
    }

    // Kiểm tra loại thiết bị
    static bool hopLeLoai(const std::string& loai) {
        return loai == "ONT" || loai == "Modem";
    }

    // Kiểm tra trạng thái
    static bool hopLeTrangThai(const std::string& tt) {
        return tt == "Dang cap phat" ||
               tt == "Kho" ||
               tt == "Hong";
    }

    // Ném ngoại lệ khi dữ liệu không hợp lệ
    static void yeuCau(bool dieuKien, const std::string& thongBao) {
        if (!dieuKien)
            throw std::invalid_argument(thongBao);
    }

public:
    // Constructor mặc định
    ThietBi() = default;

    // Getter
    const std::string& getSoSerial() const {
        return soSerial;
    }

    const std::string& getLoaiThietBi() const {
        return loaiThietBi;
    }

    const std::string& getHangSanXuat() const {
        return hangSanXuat;
    }

    const std::string& getMaHopDong() const {
        return maHopDong;
    }

    const std::string& getTrangThai() const {
        return trangThai;
    }

    // Cập nhật trạng thái
    void setTrangThai(const std::string& tt) {
        yeuCau(
            hopLeTrangThai(tt),
            "Trang thai thiet bi khong hop le: " + tt
        );

        trangThai = tt;
    }

    // Nhập thông tin thiết bị
    void nhapThongTin() override {
        maDinhDanh =
            NhapDuLieu::nhapMaDinhDanh("Nhap ma thiet bi: ");

        soSerial =
            NhapDuLieu::nhapChuoi("Nhap so serial: ");

        // Nhập loại thiết bị
        while (true) {
            loaiThietBi =
                NhapDuLieu::nhapChuoi(
                    "Nhap loai thiet bi (ONT/Modem): "
                );

            if (hopLeLoai(loaiThietBi))
                break;

            std::cout
                << " -> Loi: Loai thiet bi chi duoc la ONT hoac Modem!\n";
        }

        hangSanXuat =
            NhapDuLieu::nhapChuoi("Nhap hang san xuat: ");

        // Chọn trạng thái trước
        while (true) {
            trangThai =
                NhapDuLieu::nhapChuoi(
                    "Nhap trang thai (Dang cap phat/Kho/Hong): "
                );

            if (hopLeTrangThai(trangThai))
                break;

            std::cout
                << " -> Loi: Trang thai khong hop le!\n";
        }

        // Chỉ thiết bị đang cấp phát mới cần mã hợp đồng
        if (trangThai == "Dang cap phat") {
            maHopDong =
                NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong: ");
        }
        else {
            maHopDong = "";
        }
    }

    // Cập nhật thông tin
    // Không cho phép thay đổi mã thiết bị
    void capNhatThongTin() override {
        soSerial =
            NhapDuLieu::nhapChuoi("Nhap so serial moi: ");

        // Cập nhật loại thiết bị
        while (true) {
            loaiThietBi =
                NhapDuLieu::nhapChuoi(
                    "Nhap loai thiet bi moi (ONT/Modem): "
                );

            if (hopLeLoai(loaiThietBi))
                break;

            std::cout
                << " -> Loi: Loai thiet bi khong hop le!\n";
        }

        hangSanXuat =
            NhapDuLieu::nhapChuoi(
                "Nhap hang san xuat moi: "
            );

        // Chọn trạng thái trước
        while (true) {
            trangThai =
                NhapDuLieu::nhapChuoi(
                    "Nhap trang thai moi (Dang cap phat/Kho/Hong): "
                );

            if (hopLeTrangThai(trangThai))
                break;

            std::cout
                << " -> Loi: Trang thai khong hop le!\n";
        }

        // Nếu đang cấp phát thì phải có mã hợp đồng
        if (trangThai == "Dang cap phat") {
            maHopDong =
                NhapDuLieu::nhapMaDinhDanh(
                    "Nhap ma hop dong moi: "
                );
        }
        else {
            // Nếu trong kho hoặc hỏng thì không có hợp đồng
            maHopDong = "";
        }
    }

    // Hiển thị thông tin
    void hienThiThongTin() const override {
        std::cout
            << "Ma TB: " << maDinhDanh
            << " | Serial: " << soSerial
            << " | Loai: " << loaiThietBi
            << " | Hang SX: " << hangSanXuat
            << " | Ma HD: " << maHopDong
            << " | Trang thai: " << trangThai
            << "\n";
    }

    // Chuyển đối tượng thành chuỗi để lưu file
    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" +
               soSerial + "|" +
               loaiThietBi + "|" +
               hangSanXuat + "|" +
               maHopDong + "|" +
               trangThai;
    }

    // Đọc dữ liệu từ file
    void docTuChuoi(const std::string& dong) override {
        // 6 trường dữ liệu cần 5 dấu '|'
        yeuCau(
            std::count(dong.begin(), dong.end(), '|') == 5,
            "Dong du lieu ThietBi co so cot khong dung."
        );

        std::string f[6];
        std::stringstream ss(dong);

        for (int i = 0; i < 6; ++i)
            std::getline(ss, f[i], '|');

        // Kiểm tra mã thiết bị
        yeuCau(
            hopLeMa(f[0]),
            "Ma thiet bi khong hop le: " + f[0]
        );

        // Kiểm tra serial
        yeuCau(
            hopLeChuoi(f[1]),
            "So serial khong hop le."
        );

        // Kiểm tra loại thiết bị
        yeuCau(
            hopLeLoai(f[2]),
            "Loai thiet bi khong hop le: " + f[2]
        );

        // Kiểm tra hãng sản xuất
        yeuCau(
            hopLeChuoi(f[3]),
            "Hang san xuat khong hop le."
        );

        // Kiểm tra trạng thái
        yeuCau(
            hopLeTrangThai(f[5]),
            "Trang thai thiet bi khong hop le: " + f[5]
        );

        // Nếu đang cấp phát thì bắt buộc phải có mã hợp đồng
        if (f[5] == "Dang cap phat") {
            yeuCau(
                hopLeMa(f[4]),
                "Thiet bi dang cap phat phai co ma hop dong."
            );
        }
        else {
            // Nếu Kho hoặc Hong thì mã hợp đồng phải để trống
            yeuCau(
                f[4].empty(),
                "Thiet bi Kho/Hong khong duoc co ma hop dong."
            );
        }

        // Gán dữ liệu
        maDinhDanh = f[0];
        soSerial = f[1];
        loaiThietBi = f[2];
        hangSanXuat = f[3];
        maHopDong = f[4];
        trangThai = f[5];
    }
};

#endif
