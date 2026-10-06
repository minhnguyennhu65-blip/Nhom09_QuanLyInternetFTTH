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

// Lớp KhachHang kế thừa từ LopCoSo, đại diện cho thông tin của một khách hàng
class KhachHang : public LopCoSo {
    // Các thuộc tính cơ bản của khách hàng
    std::string tenKhachHang, soDienThoai, diaChiLapDat, email;
    std::string trangThai = "Dang su dung"; // Mặc định khi tạo mới là "Dang su dung"

    // Kiểm tra mã: không rỗng, không chứa ký tự '|', không chứa khoảng trắng hay chữ thường
    static bool hopLeMa(const std::string& s) {
        if (s.empty() || s.find('|') != std::string::npos) return false;
        for (unsigned char c : s)
            if (std::isspace(c) || std::islower(c)) return false;
        return true;
    }

    // Kiểm tra SDT: Phải đúng 10 ký tự, bắt đầu bằng '0' và toàn bộ là số
    static bool hopLeSDT(const std::string& s) {
        if (s.size() != 10 || s[0] != '0') return false;
        for (unsigned char c : s) if (!std::isdigit(c)) return false;
        return true;
    }

    // Kiểm tra chuỗi chung: không rỗng và không chứa ký tự phân cách '|'
    static bool hopLeChuoi(const std::string& s) {
        return !s.empty() && s.find('|') == std::string::npos;
    }

    // Kiểm tra định dạng email cơ bản (phải có '@', '.', không chứa khoảng trắng, v.v.)
    static bool hopLeEmail(const std::string& e) {
        auto at = e.find('@'), dot = e.find('.', at == std::string::npos ? 0 : at + 1);
        if (!hopLeChuoi(e) || at == 0 || at == std::string::npos ||
            e.find('@', at + 1) != std::string::npos ||
            dot == std::string::npos || dot <= at + 1 || e.back() == '.')
            return false;
        for (unsigned char c : e) if (std::isspace(c)) return false;
        return true;
    }

    // Kiểm tra trạng thái khách hàng chỉ được phép nằm trong 3 giá trị cố định
    static bool hopLeTT(const std::string& t) {
        return t == "Dang su dung" || t == "Khoa" || t == "Huy";
    }

    // Hàm tiện ích: Ném ngoại lệ với thông báo lỗi nếu điều kiện không thỏa mãn
    static void yeuCau(bool ok, const std::string& msg) {
        if (!ok) throw std::invalid_argument(msg);
    }

public:
    // Constructor mặc định
    KhachHang() = default;

    // Các hàm Getter để lấy thông tin
    const std::string& getTenKhachHang() const { return tenKhachHang; }
    const std::string& getSoDienThoai() const { return soDienThoai; }
    const std::string& getDiaChiLapDat() const { return diaChiLapDat; }
    const std::string& getEmail() const { return email; }
    const std::string& getTrangThai() const { return trangThai; }

    // Cập nhật trạng thái kèm theo kiểm tra tính hợp lệ
    void setTrangThai(const std::string& tt) {
        yeuCau(hopLeTT(tt), "Trang thai khach hang khong hop le: " + tt);
        trangThai = tt;
    }

    // Ghi đè hàm nhapThongTin: Nhập dữ liệu từ bàn phím an toàn qua lớp NhapDuLieu
    void nhapThongTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang (maKH): ");
        tenKhachHang = NhapDuLieu::nhapChuoi("Nhap ten khach hang: ");
        soDienThoai = NhapDuLieu::nhapSoDienThoai("Nhap so dien thoai (10 chu so, bat dau bang 0): ");
        diaChiLapDat = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat: ");
        
        // Vòng lặp yêu cầu nhập lại cho đến khi email hợp lệ
        while (true) {
            email = NhapDuLieu::nhapChuoi("Nhap email: ");
            if (hopLeEmail(email)) break;
            std::cout << " -> Loi: Email khong hop le (VD: ten@congty.vn)!\n";
        }
        
        // Vòng lặp yêu cầu nhập lại cho đến khi trạng thái hợp lệ
        while (true) {
            trangThai = NhapDuLieu::nhapChuoi("Nhap trang thai (Dang su dung/Khoa/Huy): ");
            if (hopLeTT(trangThai)) break;
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }
    }

    // Ghi đè hàm hienThiThongTin: Xuất thông tin ra màn hình
    void hienThiThongTin() const override {
        std::cout << "Ma KH: " << maDinhDanh << " | Ten: " << tenKhachHang
                  << " | SDT: " << soDienThoai << " | Dia chi: " << diaChiLapDat
                  << " | Email: " << email << " | Trang thai: " << trangThai << "\n";
    }

    // Ghi đè hàm chuyenThanhChuoi: Nối các thuộc tính cách nhau bởi ký tự '|' để lưu file
    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenKhachHang + "|" + soDienThoai + "|" +
               diaChiLapDat + "|" + email + "|" + trangThai;
    }

    // Ghi đè hàm docTuChuoi: Tái tạo đối tượng từ chuỗi đọc được trong file
    void docTuChuoi(const std::string& dong) override {
        // Đảm bảo chuỗi chứa chính xác 5 dấu '|' tương đương 6 trường dữ liệu
        yeuCau(std::count(dong.begin(), dong.end(), '|') == 5,
               "Dong du lieu KhachHang co so cot khong dung.");
        
        std::string f[6];
        std::stringstream ss(dong);
        // Tách chuỗi thành mảng các trường
        for (int i = 0; i < 6; ++i) std::getline(ss, f[i], '|');

        // Xác thực tính hợp lệ của từng trường dữ liệu trước khi gán
        yeuCau(hopLeMa(f[0]), "Ma khach hang khong hop le: " + f[0]);
        yeuCau(hopLeChuoi(f[1]), "Ten khach hang khong hop le.");
        yeuCau(hopLeSDT(f[2]), "So dien thoai khong hop le: " + f[2]);
        yeuCau(hopLeChuoi(f[3]), "Dia chi khong hop le.");
        yeuCau(hopLeEmail(f[4]), "Email khong hop le: " + f[4]);
        yeuCau(hopLeTT(f[5]), "Trang thai khach hang khong hop le: " + f[5]);

        // Cập nhật giá trị cho đối tượng
        maDinhDanh = f[0]; tenKhachHang = f[1]; soDienThoai = f[2];
        diaChiLapDat = f[3]; email = f[4]; trangThai = f[5];
    }
};

#endif
