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

    static bool daInHeader; // Flag để chỉ in header 1 lần

    // Khớp NhapDuLieu::nhapMaDinhDanh: không rỗng, không '|', không khoảng trắng, không chữ thường
    static bool hopLeMa(const std::string& s) {
        if (s.empty() || s.find('|') != std::string::npos) return false;
        for (unsigned char c : s)
            if (std::isspace(c) || std::islower(c)) return false;
        return true;
    }

    // Khớp NhapDuLieu::nhapSoDienThoai: đúng 10 ký tự, bắt đầu bằng '0', toàn chữ số
    static bool hopLeSDT(const std::string& s) {
        if (s.size() != 10 || s[0] != '0') return false;
        for (unsigned char c : s) if (!std::isdigit(c)) return false;
        return true;
    }

    // Khớp NhapDuLieu::nhapChuoi: không rỗng, không chứa ký tự phân cách '|'
    static bool hopLeChuoi(const std::string& s) {
        return !s.empty() && s.find('|') == std::string::npos;
    }

    // Email cơ bản: có đúng một '@', miền có '.', không khoảng trắng
    static bool hopLeEmail(const std::string& e) {
        auto at = e.find('@'), dot = e.find('.', at == std::string::npos ? 0 : at + 1);
        if (!hopLeChuoi(e) || at == 0 || at == std::string::npos ||
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

    // Kiểm tra ngày DD/MM/YYYY
    static bool hopLeNgay(const std::string& ngay) {
        if (ngay.size() != 10) return false;
        if (ngay[2] != '/' || ngay[5] != '/') return false;
        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) continue;
            if (!std::isdigit(ngay[i])) return false;
        }
        int d = std::stoi(ngay.substr(0, 2));
        int m = std::stoi(ngay.substr(3, 2));
        int y = std::stoi(ngay.substr(6, 4));
        if (y < 1900 || y > 2100 || m < 1 || m > 12 || d < 1) return false;
        int soNgayTrongThang[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        bool namNhuan = (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
        if (namNhuan) soNgayTrongThang[1] = 29;
        return d <= soNgayTrongThang[m - 1];
    }

    // Ném ngoại lệ nếu điều kiện không thỏa (dùng khi đọc file)
    static void yeuCau(bool ok, const std::string& msg) {
        if (!ok) throw std::invalid_argument(msg);
    }

    // Dùng chung cho thêm mới (nhapMa = true) và cập nhật (nhapMa = false, giữ nguyên mã)
    void nhapNoiDung(bool nhapMa) {
        if (nhapMa) maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang (maKH): ");
        tenKhachHang = NhapDuLieu::nhapChuoi("Nhap ten khach hang: ");
        soDienThoai = NhapDuLieu::nhapSoDienThoai("Nhap so dien thoai (10 chu so, bat dau bang 0): ");
        diaChiLapDat = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat: ");

        // NhapDuLieu chưa kiểm email nên phải lặp đến khi đúng định dạng
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
        while (true) {
            ngayDangKy = NhapDuLieu::nhapChuoi("Nhap ngay dang ky (DD/MM/YYYY): ");
            if (hopLeNgay(ngayDangKy)) break;
            std::cout << " -> Loi: Ngay khong hop le (VD: 23/10/2026)!\n";
        }

        while (true) {
            trangThai = NhapDuLieu::nhapChuoi("Nhap trang thai (Dang su dung/Khoa/Huy): ");
            if (hopLeTT(trangThai)) break;
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }
    }

public:
    KhachHang() = default;

    const std::string& getTenKhachHang() const { return tenKhachHang; }
    const std::string& getSoDienThoai() const { return soDienThoai; }
    const std::string& getDiaChiLapDat() const { return diaChiLapDat; }
    const std::string& getEmail() const { return email; }
    const std::string& getTrangThai() const { return trangThai; }
    const std::string& getLoaiKhachHang() const { return loaiKhachHang; }
    const std::string& getNgayDangKy() const { return ngayDangKy; }

    // Cập nhật trạng thái kèm kiểm tra hợp lệ
    void setTrangThai(const std::string& tt) {
        yeuCau(hopLeTT(tt), "Trang thai khach hang khong hop le: " + tt);
        trangThai = tt;
    }

    // Ghi đè: thêm khách hàng mới (có nhập mã)
    void nhapThongTin() override { nhapNoiDung(true); }

    // Ghi đè: cập nhật thông tin phụ, không sửa maDinhDanh — bắt buộc vì LopCoSo thuần ảo
    void capNhatThongTin() override { nhapNoiDung(false); }

    // Ghi đè: in thông tin ra màn hình (Tự động vẽ bảng)
    void hienThiThongTin() const override {
        // Chỉ in header lần đầu tiên
        if (!daInHeader) {
            std::cout << "\n" << std::string(142, '-') << "\n";
            std::cout << std::left
                << std::setw(7) << "Ma KH" << " | "
                << std::setw(20) << "Ho ten" << " | "
                << std::setw(12) << "SDT" << " | "
                << std::setw(25) << "Dia chi" << " | "
                << std::setw(22) << "Email" << " | "
                << std::setw(15) << "Loai KH" << " | "
                << std::setw(12) << "Ngay DK" << " | "
                << std::setw(15) << "Trang thai" << "\n";
            std::cout << std::string(142, '-') << "\n";
            daInHeader = true;
        }

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

    // Ghi đè: tách dòng file thành thuộc tính; kiểm tra hết rồi mới gán
    void docTuChuoi(const std::string& dong) override {
        yeuCau(std::count(dong.begin(), dong.end(), '|') == 7,
            "Dong du lieu KhachHang co so cot khong dung.");

        std::string f[8];
        std::stringstream ss(dong);
        for (int i = 0; i < 8; ++i) std::getline(ss, f[i], '|');

        if (!f[7].empty() && f[7].back() == '\r') f[7].pop_back();

        yeuCau(hopLeMa(f[0]), "Ma khach hang khong hop le: " + f[0]);
        yeuCau(hopLeChuoi(f[1]), "Ten khach hang khong hop le.");
        yeuCau(hopLeSDT(f[2]), "So dien thoai khong hop le: " + f[2]);
        yeuCau(hopLeChuoi(f[3]), "Dia chi khong hop le.");
        yeuCau(hopLeEmail(f[4]), "Email khong hop le: " + f[4]);
        yeuCau(hopLeLoaiKH(f[5]), "Loai khach hang khong hop le: " + f[5]);
        yeuCau(hopLeNgay(f[6]), "Ngay dang ky khong hop le: " + f[6]);
        yeuCau(hopLeTT(f[7]), "Trang thai khach hang khong hop le: " + f[7]);

        maDinhDanh = f[0]; tenKhachHang = f[1]; soDienThoai = f[2];
        diaChiLapDat = f[3]; email = f[4]; loaiKhachHang = f[5];
        ngayDangKy = f[6]; trangThai = f[7];
    }

    // Reset flag khi cần hiển thị lại từ đầu
    static void resetHeader() { daInHeader = false; }
};

// Khởi tạo biến static
bool KhachHang::daInHeader = false;

#endif
