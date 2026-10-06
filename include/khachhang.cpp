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

// Lớp KhachHang kế thừa LopCoSo, triển khai đủ 5 hàm thuần ảo (kể cả capNhatThongTin)
class KhachHang : public LopCoSo {
private:
    std::string tenKhachHang, soDienThoai, diaChiLapDat, email;
    std::string trangThai = "Dang su dung"; // Mặc định khi tạo mới

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

    // Cập nhật trạng thái kèm kiểm tra hợp lệ
    void setTrangThai(const std::string& tt) {
        yeuCau(hopLeTT(tt), "Trang thai khach hang khong hop le: " + tt);
        trangThai = tt;
    }

    // Ghi đè: thêm khách hàng mới (có nhập mã)
    void nhapThongTin() override { nhapNoiDung(true); }

    // Ghi đè: cập nhật thông tin phụ, không sửa maDinhDanh — bắt buộc vì LopCoSo thuần ảo
    void capNhatThongTin() override { nhapNoiDung(false); }

    // Ghi đè: in thông tin ra màn hình
    void hienThiThongTin() const override {
        std::cout << "Ma KH: " << maDinhDanh << " | Ten: " << tenKhachHang
                  << " | SDT: " << soDienThoai << " | Dia chi: " << diaChiLapDat
                  << " | Email: " << email << " | Trang thai: " << trangThai << "\n";
    }

    // Ghi đè: ghép thuộc tính bằng '|' để lưu file
    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenKhachHang + "|" + soDienThoai + "|" +
               diaChiLapDat + "|" + email + "|" + trangThai;
    }

    // Ghi đè: tách dòng file thành thuộc tính; kiểm tra hết rồi mới gán
    void docTuChuoi(const std::string& dong) override {
        yeuCau(std::count(dong.begin(), dong.end(), '|') == 5,
               "Dong du lieu KhachHang co so cot khong dung.");

        std::string f[6];
        std::stringstream ss(dong);
        for (int i = 0; i < 6; ++i) std::getline(ss, f[i], '|');

        yeuCau(hopLeMa(f[0]), "Ma khach hang khong hop le: " + f[0]);
        yeuCau(hopLeChuoi(f[1]), "Ten khach hang khong hop le.");
        yeuCau(hopLeSDT(f[2]), "So dien thoai khong hop le: " + f[2]);
        yeuCau(hopLeChuoi(f[3]), "Dia chi khong hop le.");
        yeuCau(hopLeEmail(f[4]), "Email khong hop le: " + f[4]);
        yeuCau(hopLeTT(f[5]), "Trang thai khach hang khong hop le: " + f[5]);

        maDinhDanh = f[0]; tenKhachHang = f[1]; soDienThoai = f[2];
        diaChiLapDat = f[3]; email = f[4]; trangThai = f[5];
    }
};

#endif
