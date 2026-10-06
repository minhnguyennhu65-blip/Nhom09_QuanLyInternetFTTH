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

// Lớp HopDong kế thừa từ LopCoSo, lưu trữ thông tin giao dịch giữa khách hàng và dịch vụ
class HopDong : public LopCoSo {
    // Khóa ngoại liên kết đối tượng và dữ liệu hợp đồng
    std::string maKhachHang, maGoiCuoc, maThietBi, ngayKy, ngayHetHan;
    std::string trangThai = "Hoat dong"; // Mặc định hợp đồng mới là đang hoạt động
    double giaThieuDung = 0;

    // Kiểm tra mã định danh tương tự lớp KhachHang
    static bool hopLeMa(const std::string& s) {
        if (s.empty() || s.find('|') != std::string::npos) return false;
        for (unsigned char c : s)
            if (std::isspace(c) || std::islower(c)) return false;
        return true;
    }

    // Kiểm tra trạng thái hợp đồng chỉ cho phép 3 giá trị cụ thể
    static bool hopLeTT(const std::string& t) {
        return t == "Hoat dong" || t == "Het han" || t == "Huy";
    }

    // Chuyển chuỗi ngày "DD/MM/YYYY" thành số nguyên YYYYMMDD để dễ so sánh tính trước/sau
    static int ngayThanhSo(const std::string& s) {
        if (s.size() != 10 || s[2] != '/' || s[5] != '/') return -1;
        for (int i = 0; i < 10; ++i)
            if (i != 2 && i != 5 && !std::isdigit(static_cast<unsigned char>(s[i])))
                return -1;
        int d = std::stoi(s.substr(0, 2)), m = std::stoi(s.substr(3, 2)), y = std::stoi(s.substr(6, 4));
        return NhapDuLieu::ngayHopLe(d, m, y) ? y * 10000 + m * 100 + d : -1;
    }

    // Hàm tiện ích: Ném ngoại lệ với thông báo lỗi nếu không đạt yêu cầu
    static void yeuCau(bool ok, const std::string& msg) {
        if (!ok) throw std::invalid_argument(msg);
    }

public:
    HopDong() = default;

    // Các hàm Getter
    const std::string& getMaKhachHang() const { return maKhachHang; }
    const std::string& getMaGoiCuoc() const { return maGoiCuoc; }
    const std::string& getMaThietBi() const { return maThietBi; }
    const std::string& getNgayKy() const { return ngayKy; }
    const std::string& getNgayHetHan() const { return ngayHetHan; }
    const std::string& getTrangThai() const { return trangThai; }
    double getGiaThieuDung() const { return giaThieuDung; }

    // Thiết lập trạng thái mới có kiểm tra hợp lệ
    void setTrangThai(const std::string& tt) {
        yeuCau(hopLeTT(tt), "Trang thai hop dong khong hop le: " + tt);
        trangThai = tt;
    }

    // Ghi đè phương thức nhập từ bàn phím
    void nhapThongTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong (maHD): ");
        maKhachHang = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang: ");
        maGoiCuoc = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi cuoc: ");
        maThietBi = NhapDuLieu::nhapMaDinhDanh("Nhap ma thiet bi: ");
        ngayKy = NhapDuLieu::nhapNgayThang("Nhap ngay ky hop dong (DD/MM/YYYY): ");
        
        // Ràng buộc nghiệp vụ: Ngày hết hạn không được sớm hơn ngày ký
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

    void hienThiThongTin() const override {
        std::cout << "Ma HD: " << maDinhDanh << " | Ma KH: " << maKhachHang
                  << " | Ma Goi: " << maGoiCuoc << " | Ma TB: " << maThietBi
                  << " | Ngay ky: " << ngayKy << " | Ngay het han: " << ngayHetHan
                  << " | Gia thieu dung: " << giaThieuDung
                  << " | Trang thai: " << trangThai << "\n";
    }

    // Chuẩn bị dữ liệu để ghi xuống file, ghép bằng '|'
    std::string chuyenThanhChuoi() const override {
        std::ostringstream os;
        os << maDinhDanh << '|' << maKhachHang << '|' << maGoiCuoc << '|'
           << maThietBi << '|' << ngayKy << '|' << ngayHetHan << '|'
           << std::setprecision(15) << giaThieuDung << '|' << trangThai; // Tránh sai số dấu phẩy động
        return os.str();
    }

    // Phân tích chuỗi đọc từ file thành thuộc tính của đối tượng
    void docTuChuoi(const std::string& dong) override {
        // Đảm bảo chuỗi chứa chính xác 7 dấu '|' tương đương 8 trường dữ liệu
        yeuCau(std::count(dong.begin(), dong.end(), '|') == 7,
               "Dong du lieu HopDong co so cot khong dung.");
        
        std::string f[8];
        std::stringstream ss(dong);
        for (int i = 0; i < 8; ++i) std::getline(ss, f[i], '|');

        // Xác thực các mã định danh
        yeuCau(hopLeMa(f[0]), "Ma hop dong khong hop le: " + f[0]);
        yeuCau(hopLeMa(f[1]), "Ma khach hang khong hop le: " + f[1]);
        yeuCau(hopLeMa(f[2]), "Ma goi cuoc khong hop le: " + f[2]);
        yeuCau(hopLeMa(f[3]), "Ma thiet bi khong hop le: " + f[3]);
        
        // Xác thực ngày tháng và ràng buộc logic
        int soKy = ngayThanhSo(f[4]), soHet = ngayThanhSo(f[5]);
        yeuCau(soKy >= 0, "Ngay ky khong hop le: " + f[4]);
        yeuCau(soHet >= 0, "Ngay het han khong hop le: " + f[5]);
        yeuCau(soHet >= soKy, "Ngay het han phai sau hoac bang ngay ky.");
        yeuCau(hopLeTT(f[7]), "Trang thai hop dong khong hop le: " + f[7]);

        // Phân tích và xác thực giá thuê dùng an toàn
        std::size_t pos = 0;
        double gia = std::stod(f[6], &pos);
        // Kiểm tra xem toàn bộ chuỗi có được chuyển đổi thành số hợp lệ và không âm hay không
        yeuCau(pos == f[6].size() && std::isfinite(gia) && gia >= 0,
               "Gia thieu dung khong hop le: " + f[6]);

        // Cập nhật giá trị cho đối tượng
        maDinhDanh = f[0]; maKhachHang = f[1]; maGoiCuoc = f[2]; maThietBi = f[3];
        ngayKy = f[4]; ngayHetHan = f[5]; giaThieuDung = gia; trangThai = f[7];
    }
};

#endif
