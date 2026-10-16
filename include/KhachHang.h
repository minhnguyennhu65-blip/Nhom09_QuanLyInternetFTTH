/* =======================================================
 * Tên tác giả: Hoàng Trọng Khoa
 * Mã sinh viên: B24DCVT204
 * Mô tả file: Lớp quản lý Hợp Đồng
 * ======================================================= */

#ifndef HOP_DONG_H
#define HOP_DONG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Lớp HopDong kế thừa LopCoSo; maDinhDanh chính là maHopDong
class HopDong : public LopCoSo {
private:
    std::string maKhachHang, maGoiCuoc, maThietBi, ngayKy, ngayHetHan;
    std::string trangThai = "Hoat dong"; // Mặc định hợp đồng mới
    double giaThieuDung = 0;

    // Khớp NhapDuLieu::nhapMaDinhDanh
    static bool hopLeMa(const std::string& s) {
        if (s.empty() || s.find('|') != std::string::npos) return false;
        for (unsigned char c : s)
            if (std::isspace(c) || std::islower(c)) return false;
        return true;
    }

    // Trạng thái chỉ nhận 3 giá trị cố định
    static bool hopLeTT(const std::string& t) {
        return t == "Hoat dong" || t == "Het han" || t == "Huy";
    }

    // Đổi "DD/MM/YYYY" thành YYYYMMDD để so sánh; dùng NhapDuLieu::ngayHopLe (năm 1900-2100)
    static int ngayThanhSo(const std::string& s) {
        if (s.size() != 10 || s[2] != '/' || s[5] != '/') return -1;
        for (int i = 0; i < 10; ++i)
            if (i != 2 && i != 5 && !std::isdigit(static_cast<unsigned char>(s[i])))
                return -1;
        int d = std::stoi(s.substr(0, 2)), m = std::stoi(s.substr(3, 2)), y = std::stoi(s.substr(6, 4));
        return NhapDuLieu::ngayHopLe(d, m, y) ? y * 10000 + m * 100 + d : -1;
    }

    // Định dạng tiền VND: số nguyên, không dạng khoa học (VD: 10000000)
    static std::string dinhDangTien(double v) {
        std::ostringstream os;
        os << std::fixed << std::setprecision(0) << v;
        return os.str();
    }

    static void yeuCau(bool ok, const std::string& msg) {
        if (!ok) throw std::invalid_argument(msg);
    }

    // Dùng chung cho thêm mới và cập nhật (cập nhật không đổi mã hợp đồng)
    void nhapNoiDung(bool nhapMa) {
        if (nhapMa) maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong (maHD): ");
        maKhachHang = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang: ");
        maGoiCuoc = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi cuoc: ");
        maThietBi = NhapDuLieu::nhapMaDinhDanh("Nhap ma thiet bi: ");
        ngayKy = NhapDuLieu::nhapNgayThang("Nhap ngay ky hop dong (DD/MM/YYYY): ");

        // Ràng buộc nghiệp vụ: ngày hết hạn không được trước ngày ký
        while (true) {
            ngayHetHan = NhapDuLieu::nhapNgayThang("Nhap ngay het han hop dong (DD/MM/YYYY): ");
            if (ngayThanhSo(ngayHetHan) >= ngayThanhSo(ngayKy)) break;
            std::cout << " -> Loi: Ngay het han phai sau hoac bang ngay ky!\n";
        }

        giaThieuDung = NhapDuLieu::nhapSoThucDuong("Nhap gia thieu dung hang thang (>= 0): ");

        while (true) {
            trangThai = NhapDuLieu::nhapChuoi("Nhap trang thai (Hoat dong/Het han/Huy): ");
            if (hopLeTT(trangThai)) break;
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }
    }

public:
    HopDong() = default;

    const std::string& getMaKhachHang() const { return maKhachHang; }
    const std::string& getMaGoiCuoc() const { return maGoiCuoc; }
    const std::string& getMaThietBi() const { return maThietBi; }
    const std::string& getNgayKy() const { return ngayKy; }
    const std::string& getNgayHetHan() const { return ngayHetHan; }
    const std::string& getTrangThai() const { return trangThai; }
    double getGiaThieuDung() const { return giaThieuDung; }

    void setTrangThai(const std::string& tt) {
        yeuCau(hopLeTT(tt), "Trang thai hop dong khong hop le: " + tt);
        trangThai = tt;
    }

    // Ghi đè: thêm hợp đồng mới (có nhập mã)
    void nhapThongTin() override { nhapNoiDung(true); }

    // Ghi đè: cập nhật thông tin phụ, không sửa maDinhDanh — bắt buộc vì LopCoSo thuần ảo
    void capNhatThongTin() override { nhapNoiDung(false); }

    void hienThiThongTin() const override {
        std::cout << "Ma HD: " << maDinhDanh << " | Ma KH: " << maKhachHang
                  << " | Ma Goi: " << maGoiCuoc << " | Ma TB: " << maThietBi
                  << " | Ngay ky: " << ngayKy << " | Ngay het han: " << ngayHetHan
                  << " | Gia thieu dung: " << dinhDangTien(giaThieuDung) << " VND"
                  << " | Trang thai: " << trangThai << "\n";
    }

    // Ghép chuỗi lưu file; tiền VND ghi dạng số nguyên (fixed, 0 chữ số thập phân)
    std::string chuyenThanhChuoi() const override {
        std::ostringstream os;
        os << maDinhDanh << '|' << maKhachHang << '|' << maGoiCuoc << '|'
           << maThietBi << '|' << ngayKy << '|' << ngayHetHan << '|'
           << dinhDangTien(giaThieuDung) << '|' << trangThai;
        return os.str();
    }

    // Tách 8 trường (7 dấu '|'); kiểm tra hết rồi mới gán
    void docTuChuoi(const std::string& dong) override {
        yeuCau(std::count(dong.begin(), dong.end(), '|') == 7,
               "Dong du lieu HopDong co so cot khong dung.");

        std::string f[8];
        std::stringstream ss(dong);
        for (int i = 0; i < 8; ++i) std::getline(ss, f[i], '|');

        yeuCau(hopLeMa(f[0]), "Ma hop dong khong hop le: " + f[0]);
        yeuCau(hopLeMa(f[1]), "Ma khach hang khong hop le: " + f[1]);
        yeuCau(hopLeMa(f[2]), "Ma goi cuoc khong hop le: " + f[2]);
        yeuCau(hopLeMa(f[3]), "Ma thiet bi khong hop le: " + f[3]);

        int soKy = ngayThanhSo(f[4]), soHet = ngayThanhSo(f[5]);
        yeuCau(soKy >= 0, "Ngay ky khong hop le: " + f[4]);
        yeuCau(soHet >= 0, "Ngay het han khong hop le: " + f[5]);
        yeuCau(soHet >= soKy, "Ngay het han phai sau hoac bang ngay ky.");
        yeuCau(hopLeTT(f[7]), "Trang thai hop dong khong hop le: " + f[7]);

        std::size_t pos = 0;
        double gia = std::stod(f[6], &pos);
        yeuCau(pos == f[6].size() && std::isfinite(gia) && gia >= 0,
               "Gia thieu dung khong hop le: " + f[6]);

        maDinhDanh = f[0]; maKhachHang = f[1]; maGoiCuoc = f[2]; maThietBi = f[3];
        ngayKy = f[4]; ngayHetHan = f[5]; giaThieuDung = gia; trangThai = f[7];
    }
};

#endif
