/* =======================================================
- Tên tác giả: Hoàng Trọng Khoa
- Mã sinh viên: B24DCVT204
- Mô tả file: Lớp quản lý Khách Hàng
 *
- Định dạng một dòng trong file (8 trường, ngăn cách bởi '|'):
-   maKH|tenKhachHang|soDienThoai|diaChiLapDat|email|loaiKhachHang|ngayDangKy|trangThai
- ======================================================= */

#ifndef KHACH_HANG_H
#define KHACH_HANG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iomanip>

// Lớp KhachHang kế thừa LopCoSo, triển khai đủ 5 hàm thuần ảo
class KhachHang : public LopCoSo {
private:
    std::string tenKhachHang, soDienThoai, diaChiLapDat, email;
    std::string trangThai = "Dang su dung"; // Mặc định khi tạo mới
    std::string loaiKhachHang = "Ca nhan";  // Ca nhan / Doanh nghiep
    std::string ngayDangKy;                 // DD/MM/YYYY, không sau ngày hiện tại

    // ---------- Hàm kiểm tra / tiện ích nội bộ ----------

    // Chuỗi dữ liệu hợp lệ: không rỗng, không chứa '|' và ký tự điều khiển
    static void kiemTraChuoi(const std::string& s, const std::string& ten) {
        if (s.empty())
            throw std::invalid_argument(ten + " khong duoc rong!");
        if (s.find('|') != std::string::npos)
            throw std::invalid_argument(ten + " khong duoc chua ky tu '|'!");
        for (char c : s) {
            if (std::iscntrl(static_cast<unsigned char>(c)))
                throw std::invalid_argument(ten + " khong duoc chua ky tu dieu khien!");
        }
    }

    // Chuẩn hóa mã: chỉ chữ cái không dấu, số, '_' hoặc '-'; viết hoa
    static std::string chuanHoaMa(const std::string& ma, const std::string& ten) {
        if (ma.empty())
            throw std::invalid_argument(ten + " khong duoc rong!");
        std::string kq = ma;
        for (char& c : kq) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(std::isalnum(uc) && uc < 128) && c != '_' && c != '-')
                throw std::invalid_argument(
                    ten + " chi gom chu cai khong dau, chu so, '_' hoac '-'!");
            c = static_cast<char>(std::toupper(uc));
        }
        return kq;
    }

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

    static bool hopLeSDT(const std::string& s) {
        if (s.size() != 10 || s[0] != '0') return false;
        for (char c : s) if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        return true;
    }

    static bool hopLeTT(const std::string& t) {
        return t == "Dang su dung" || t == "Khoa" || t == "Huy";
    }

    static bool hopLeLoaiKH(const std::string& l) {
        return l == "Ca nhan" || l == "Doanh nghiep";
    }

    // Tách theo '|', giữ lại cả trường rỗng ở giữa và cuối
    static std::vector<std::string> tachTruong(const std::string& s) {
        std::vector<std::string> kq;
        std::size_t dau = 0;
        while (true) {
            std::size_t p = s.find('|', dau);
            if (p == std::string::npos) {
                kq.push_back(s.substr(dau));
                break;
            }
            kq.push_back(s.substr(dau, p - dau));
            dau = p + 1;
        }
        return kq;
    }

    // Cắt chuỗi dài để không làm lệch cột khi in bảng
    static std::string cat(const std::string& s, std::size_t n) {
        return s.size() <= n ? s : s.substr(0, n - 2) + "..";
    }

    // Lặp lại thao tác nhập cho đến khi setter không ném ngoại lệ
    template <typename F>
    static void lapDenKhiHopLe(F thucHien) {
        while (true) {
            try { thucHien(); return; }
            catch (const std::invalid_argument& e) {
                std::cout << " -> Loi: " << e.what() << '\n';
            }
        }
    }

    // Nhập các trường (trừ mã). giuNguyen = true: bỏ trống = giữ nguyên giá trị cũ.
    // Mỗi lần gọi trên một đối tượng tạm nên nhập dở dang không làm hỏng dữ liệu thật.
    void nhapCacTruong(bool giuNguyen) {
        const std::string ghiChu = giuNguyen ? " (bo trong de giu nguyen)" : "";

        lapDenKhiHopLe([&] {
            setTenKhachHang(giuNguyen
                ? NhapDuLieu::nhapChuoiHoacGiuNguyen("Nhap ten khach hang" + ghiChu + ": ", tenKhachHang)
                : NhapDuLieu::nhapChuoi("Nhap ten khach hang: "));
            });

        lapDenKhiHopLe([&] {
            setSoDienThoai(giuNguyen
                ? NhapDuLieu::nhapSoDienThoaiHoacGiuNguyen(
                    "Nhap so dien thoai (10 so, bat dau bang 0)" + ghiChu + ": ", soDienThoai)
                : NhapDuLieu::nhapSoDienThoai("Nhap so dien thoai (10 so, bat dau bang 0): "));
            });

        lapDenKhiHopLe([&] {
            setDiaChiLapDat(giuNguyen
                ? NhapDuLieu::nhapChuoiHoacGiuNguyen("Nhap dia chi lap dat" + ghiChu + ": ", diaChiLapDat)
                : NhapDuLieu::nhapChuoi("Nhap dia chi lap dat: "));
            });

        lapDenKhiHopLe([&] {
            setEmail(giuNguyen
                ? NhapDuLieu::nhapChuoiHoacGiuNguyen("Nhap email" + ghiChu + ": ", email)
                : NhapDuLieu::nhapChuoi("Nhap email: "));
            });

        // Loại khách hàng
        std::cout << "\nLoai khach hang:\n1. Ca nhan\n2. Doanh nghiep\n";
        int loai = giuNguyen
            ? NhapDuLieu::nhapSoNguyenHoacGiuNguyen("Chon loai (bo trong de giu nguyen): ", 0, 1, 2)
            : NhapDuLieu::nhapSoNguyenTrongKhoang("Chon loai (1-2): ", 1, 2);
        if (loai == 1) setLoaiKhachHang("Ca nhan");
        else if (loai == 2) setLoaiKhachHang("Doanh nghiep");

        // Ngày đăng ký: không được sau ngày hiện tại
        lapDenKhiHopLe([&] {
            setNgayDangKy(giuNguyen
                ? NhapDuLieu::nhapNgayThangHoacGiuNguyen(
                    "Nhap ngay dang ky (DD/MM/YYYY)" + ghiChu + ": ", ngayDangKy, true)
                : NhapDuLieu::nhapNgayThang("Nhap ngay dang ky (DD/MM/YYYY): ", true));
            });

        // Trạng thái
        std::cout << "\nTrang thai khach hang:\n1. Dang su dung\n2. Khoa\n3. Huy\n";
        int tt = giuNguyen
            ? NhapDuLieu::nhapSoNguyenHoacGiuNguyen("Chon trang thai (bo trong de giu nguyen): ", 0, 1, 3)
            : NhapDuLieu::nhapSoNguyenTrongKhoang("Chon trang thai (1-3): ", 1, 3);
        if (tt == 1) setTrangThai("Dang su dung");
        else if (tt == 2) setTrangThai("Khoa");
        else if (tt == 3) setTrangThai("Huy");
    }

public:
    KhachHang() = default;

    // In tiêu đề bảng chuẩn có dấu phân cách ' | '
    static void inTieuDe() {
        std::cout << std::left
            << std::setw(7) << "Ma KH" << " | "
            << std::setw(18) << "Ho ten" << " | "
            << std::setw(12) << "SDT" << " | "
            << std::setw(22) << "Dia chi" << " | "
            << std::setw(22) << "Email" << " | "
            << std::setw(13) << "Loai KH" << " | "
            << std::setw(11) << "Ngay DK" << " | "
            << std::setw(13) << "Trang thai" << "\n"
            << std::string(139, '-') << "\n";
    }

    // ---------- Getters ----------
    const std::string& getTenKhachHang() const { return tenKhachHang; }
    const std::string& getSoDienThoai() const { return soDienThoai; }
    const std::string& getDiaChiLapDat() const { return diaChiLapDat; }
    const std::string& getEmail() const { return email; }
    const std::string& getTrangThai() const { return trangThai; }
    const std::string& getLoaiKhachHang() const { return loaiKhachHang; }
    const std::string& getNgayDangKy() const { return ngayDangKy; }

    // ---------- Setters có kiểm tra hợp lệ (ném invalid_argument) ----------

    void setTenKhachHang(const std::string& ten) {
        kiemTraChuoi(ten, "Ten khach hang");
        tenKhachHang = ten;
    }

    void setSoDienThoai(const std::string& sdt) {
        if (!hopLeSDT(sdt))
            throw std::invalid_argument("So dien thoai phai co 10 chu so va bat dau bang '0'!");
        soDienThoai = sdt;
    }

    void setDiaChiLapDat(const std::string& dc) {
        kiemTraChuoi(dc, "Dia chi lap dat");
        diaChiLapDat = dc;
    }

    void setEmail(const std::string& e) {
        kiemTraChuoi(e, "Email");
        if (!hopLeEmail(e))
            throw std::invalid_argument("Email khong hop le (VD: ten@congty.vn)!");
        email = e;
    }

    void setLoaiKhachHang(const std::string& loai) {
        if (!hopLeLoaiKH(loai))
            throw std::invalid_argument("Loai khach hang chi duoc la 'Ca nhan' hoac 'Doanh nghiep'!");
        loaiKhachHang = loai;
    }

    // Ngày đăng ký: đúng dạng DD/MM/YYYY, là ngày thật, không sau ngày hiện tại
    void setNgayDangKy(const std::string& ngay) {
        if (!NhapDuLieu::laNgayThangHopLe(ngay))
            throw std::invalid_argument(
                "Ngay dang ky khong hop le! Dung dinh dang DD/MM/YYYY (VD: 15/03/2026).");
        if (NhapDuLieu::soSanhNgay(ngay, NhapDuLieu::ngayHienTai()) > 0)
            throw std::invalid_argument(
                "Ngay dang ky khong duoc sau ngay hien tai (" + NhapDuLieu::ngayHienTai() + ")!");
        ngayDangKy = ngay;
    }

    void setTrangThai(const std::string& tt) {
        if (!hopLeTT(tt))
            throw std::invalid_argument("Trang thai chi duoc la 'Dang su dung', 'Khoa' hoac 'Huy'!");
        trangThai = tt;
    }

    // ---------- Các hàm ghi đè từ LopCoSo ----------

    // Thêm khách hàng mới: dựng trên đối tượng tạm, xong mới gán
    void nhapThongTin() override {
        KhachHang tam;
        tam.setMaDinhDanh(NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang (maKH): "));
        tam.nhapCacTruong(false);
        *this = tam;
    }

    // Cập nhật: bỏ trống = giữ nguyên, không sửa mã; lỗi giữa chừng thì không thay đổi gì
    void capNhatThongTin() override {
        KhachHang tam = *this;
        tam.nhapCacTruong(true);
        *this = tam;
    }

    // Một dòng căn cột, khớp với inTieuDe()
    void hienThiThongTin() const override {
        std::cout << std::left
            << std::setw(7) << cat(maDinhDanh, 7) << " | "
            << std::setw(18) << cat(tenKhachHang, 17) << " | "
            << std::setw(12) << soDienThoai << " | "
            << std::setw(22) << cat(diaChiLapDat, 21) << " | "
            << std::setw(22) << cat(email, 21) << " | "
            << std::setw(13) << loaiKhachHang << " | "
            << std::setw(11) << ngayDangKy << " | "
            << std::setw(13) << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenKhachHang + "|" + soDienThoai + "|" +
            diaChiLapDat + "|" + email + "|" + loaiKhachHang + "|" + ngayDangKy + "|" + trangThai;
    }

    // Đọc một dòng file; sai thì ném ngoại lệ và KHÔNG làm thay đổi đối tượng
    void docTuChuoi(const std::string& dong) override {
        std::vector<std::string> f = tachTruong(dong);
        if (f.size() != 8)
            throw std::invalid_argument("Dong du lieu KhachHang co so cot khong dung (can 8).");

        KhachHang tam;
        tam.setMaDinhDanh(chuanHoaMa(f[0], "Ma khach hang"));
        tam.setTenKhachHang(f[1]);
        tam.setSoDienThoai(f[2]);
        tam.setDiaChiLapDat(f[3]);
        tam.setEmail(f[4]);
        tam.setLoaiKhachHang(f[5]);
        tam.setNgayDangKy(f[6]);
        tam.setTrangThai(f[7]);
        *this = tam;
    }
};

#endif 
