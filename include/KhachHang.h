/* =======================================================
 * Tên tác giả: Hoàng Trọng Khoa
 * Mã sinh viên: B24DCVT204
 * Mô tả file: Lớp quản lý Khách Hàng
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
#include <iomanip>

 // Lớp KhachHang kế thừa LopCoSo, triển khai đủ 5 hàm thuần ảo (kể cả capNhatThongTin)
class KhachHang : public LopCoSo {
private:
    std::string tenKhachHang, soDienThoai, diaChiLapDat, email;
    std::string trangThai = "Dang su dung"; // Mặc định khi tạo mới
    std::string loaiKhachHang = "Ca nhan"; // Mặc định: Ca nhan / Doanh nghiep
    std::string ngayDangKy; // Ngày tháng năm tạo hồ sơ (DD/MM/YYYY)

    // Kiểm tra Email cơ bản
    static bool hopLeEmail(const std::string& e) {
        auto at = e.find('@'), dot = e.find('.', at == std::string::npos ? 0 : at + 1);
        if (e.empty() || at == 0 || at == std::string::npos ||
            e.find('@', at + 1) != std::string::npos ||
            dot == std::string::npos || dot <= at + 1 || e.back() == '.')
            return false;
        for (unsigned char c : e) if (std::isspace(c)) return false;
        return true;
    }

    // Trạng thái chỉ nhận 3 giá trị cố định
    static bool hopLeTT(const std::string& t) {
        return t == "Dang su dung" || t == "Khoa" || t == "Huy";
    }

    // Loại khách hàng chỉ nhận 2 giá trị
    static bool hopLeLoaiKH(const std::string& l) {
        return l == "Ca nhan" || l == "Doanh nghiep";
    }

    // Dùng chung cho thêm mới (nhapMa = true) và cập nhật (nhapMa = false, giữ nguyên mã)
    void nhapNoiDung(bool nhapMa) {
        if (nhapMa) {
            setMaDinhDanh(NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang (maKH): "));
        }
        tenKhachHang = NhapDuLieu::nhapChuoi("Nhap ten khach hang: ");
        soDienThoai = NhapDuLieu::nhapSoDienThoai("Nhap so dien thoai (10 chu so, bat dau bang 0): ");
        diaChiLapDat = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat: ");

        // Nhập email với validation riêng
        while (true) {
            email = NhapDuLieu::nhapChuoi("Nhap email: ");
            if (hopLeEmail(email)) break;
            std::cout << " -> Loi: Email khong hop le (VD: ten@congty.vn)!\n";
        }

        // Nhập loại khách hàng (Ca nhan / Doanh nghiep)
        while (true) {
            loaiKhachHang = NhapDuLieu::nhapChuoi("Nhap loai khach hang (Ca nhan/Doanh nghiep): ");
            if (hopLeLoaiKH(loaiKhachHang)) break;
            std::cout << " -> Loi: Loai khach hang khong hop le!\n";
        }

        // Nhập ngày đăng ký (DD/MM/YYYY)
        ngayDangKy = NhapDuLieu::nhapNgayThang("Nhap ngay dang ky (DD/MM/YYYY): ");

        // Nhập trạng thái
        while (true) {
            trangThai = NhapDuLieu::nhapChuoi("Nhap trang thai (Dang su dung/Khoa/Huy): ");
            if (hopLeTT(trangThai)) break;
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }
    }

public:
    KhachHang() = default;

    // In tiêu đề bảng chuẩn có dấu phân cách ' | '
    static void inTieuDe() {
        std::cout << std::left
            << std::setw(7) << "Ma KH" << " | "
            << std::setw(20) << "Ho ten" << " | "
            << std::setw(12) << "SDT" << " | "
            << std::setw(25) << "Dia chi" << " | "
            << std::setw(22) << "Email" << " | "
            << std::setw(15) << "Loai KH" << " | "
            << std::setw(12) << "Ngay DK" << " | "
            << std::setw(15) << "Trang thai" << "\n"
            << std::string(142, '-') << "\n";
    }

    const std::string& getTenKhachHang() const { return tenKhachHang; }
    const std::string& getSoDienThoai() const { return soDienThoai; }
    const std::string& getDiaChiLapDat() const { return diaChiLapDat; }
    const std::string& getEmail() const { return email; }
    const std::string& getTrangThai() const { return trangThai; }
    const std::string& getLoaiKhachHang() const { return loaiKhachHang; }
    const std::string& getNgayDangKy() const { return ngayDangKy; }

    // Ghi đè: thêm khách hàng mới (có nhập mã)
    void nhapThongTin() override { nhapNoiDung(true); }

    // Ghi đè: cập nhật thông tin phụ, không sửa maDinhDanh — bắt buộc vì LopCoSo thuần ảo
    void capNhatThongTin() override { nhapNoiDung(false); }

    // Ghi đè: in thông tin ra màn hình (một dòng căn cột)
    void hienThiThongTin() const override {
        std::cout << std::left
            << std::setw(7) << maDinhDanh << " | "
            << std::setw(20) << tenKhachHang << " | "
            << std::setw(12) << soDienThoai << " | "
            << std::setw(25) << diaChiLapDat << " | "
            << std::setw(22) << email << " | "
            << std::setw(15) << loaiKhachHang << " | "
            << std::setw(12) << ngayDangKy << " | "
            << std::setw(15) << trangThai << "\n";
    }

    // Ghi đè: ghép thuộc tính bằng '|' để lưu file
    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenKhachHang + "|" + soDienThoai + "|" +
            diaChiLapDat + "|" + email + "|" + loaiKhachHang + "|" + ngayDangKy + "|" + trangThai;
    }

    // Ghi đè: tách dòng file thành thuộc tính; ném ngoại lệ nếu sai
    void docTuChuoi(const std::string& dong) override {
        std::stringstream ss(dong);
        std::string f[8];
        for (int i = 0; i < 8; ++i) {
            if (!std::getline(ss, f[i], '|')) {
                throw std::invalid_argument("Dong du lieu KhachHang co so cot khong dung.");
            }
        }

        if (!f[7].empty() && f[7].back() == '\r') f[7].pop_back();

        // Kiểm tra email
        if (!hopLeEmail(f[4])) {
            throw std::invalid_argument("Email khong hop le: " + f[4]);
        }

        // Kiểm tra loại khách hàng
        if (!hopLeLoaiKH(f[5])) {
            throw std::invalid_argument("Loai khach hang khong hop le: " + f[5]);
        }

        // Kiểm tra ngày
        if (!NhapDuLieu::laNgayThangHopLe(f[6])) {
            throw std::invalid_argument("Ngay dang ky khong hop le: " + f[6]);
        }

        // Kiểm tra trạng thái
        if (!hopLeTT(f[7])) {
            throw std::invalid_argument("Trang thai khach hang khong hop le: " + f[7]);
        }

        // Gán giá trị
        setMaDinhDanh(f[0]);
        tenKhachHang = f[1];
        soDienThoai = f[2];
        diaChiLapDat = f[3];
        email = f[4];
        loaiKhachHang = f[5];
        ngayDangKy = f[6];
        trangThai = f[7];
    }
};

#endif
