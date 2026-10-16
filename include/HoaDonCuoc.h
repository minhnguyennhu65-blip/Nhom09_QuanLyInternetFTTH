/* =======================================================
 * Tên tác giả: Nguyễn Đức Huy
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp quản lý hóa đơn cước
 * ======================================================= */

#ifndef HOA_DON_CUOC_H
#define HOA_DON_CUOC_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class HoaDonCuoc : public LopCoSo {
private:
    // Các thuộc tính của hóa đơn
    std::string maHopDong;
    std::string thangCuoc;
    double soTien = 0.0;
    std::string ngayLap;
    std::string ngayThanhToan = "-";
    std::string trangThai = "Chua thanh toan";

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

    // Kiểm tra tháng cước dạng MM/YYYY
    static bool hopLeThang(const std::string& s) {
        if (s.length() != 7 || s[2] != '/')
            return false;

        for (int i = 0; i < 7; ++i) {
            if (i == 2)
                continue;

            if (!std::isdigit(
                    static_cast<unsigned char>(s[i])))
                return false;
        }

        int thang = std::stoi(s.substr(0, 2));
        int nam = std::stoi(s.substr(3, 4));

        return thang >= 1 &&
               thang <= 12 &&
               nam >= 1900 &&
               nam <= 2100;
    }

    // Kiểm tra ngày bằng hàm dùng chung của nhóm
    static bool hopLeNgay(const std::string& s) {
        if (s.length() != 10 ||
            s[2] != '/' ||
            s[5] != '/')
            return false;

        for (int i = 0; i < 10; ++i) {
            if (i == 2 || i == 5)
                continue;

            if (!std::isdigit(
                    static_cast<unsigned char>(s[i])))
                return false;
        }

        try {
            int ngay = std::stoi(s.substr(0, 2));
            int thang = std::stoi(s.substr(3, 2));
            int nam = std::stoi(s.substr(6, 4));

            return NhapDuLieu::ngayHopLe(
                ngay, thang, nam
            );
        }
        catch (...) {
            return false;
        }
    }

    // Kiểm tra trạng thái
    static bool hopLeTrangThai(const std::string& tt) {
        return tt == "Chua thanh toan" ||
               tt == "Da thanh toan";
    }

    // Ném ngoại lệ khi dữ liệu không hợp lệ
    static void yeuCau(
        bool dieuKien,
        const std::string& thongBao) {

        if (!dieuKien)
            throw std::invalid_argument(thongBao);
    }

public:
    // Constructor mặc định
    HoaDonCuoc() = default;

    // Getter
    const std::string& getMaHopDong() const {
        return maHopDong;
    }

    const std::string& getThangCuoc() const {
        return thangCuoc;
    }

    double getSoTien() const {
        return soTien;
    }

    const std::string& getNgayLap() const {
        return ngayLap;
    }

    const std::string& getNgayThanhToan() const {
        return ngayThanhToan;
    }

    const std::string& getTrangThai() const {
        return trangThai;
    }

    // Cập nhật trạng thái
    void setTrangThai(const std::string& tt) {
        yeuCau(
            hopLeTrangThai(tt),"Trang thai hoa don khong hop le: " + tt);

        trangThai = tt;

        if (trangThai == "Chua thanh toan")
            ngayThanhToan = "-";
    }

    // Nhập thông tin hóa đơn
    void nhapThongTin() override {
        maDinhDanh =
            NhapDuLieu::nhapMaDinhDanh("Nhap ma hoa don: ");
        maHopDong =
            NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong: ");

        // Nhập tháng cước
        while (true) {
            thangCuoc =
                NhapDuLieu::nhapChuoi("Nhap thang cuoc (MM/YYYY): ");

            if (hopLeThang(thangCuoc))
                break;

            std::cout << " -> Loi: Thang cuoc phai co dang MM/YYYY!\n";
        }

        soTien =
            NhapDuLieu::nhapSoThucDuong("Nhap so tien: ");
        ngayLap =
            NhapDuLieu::nhapNgayThang("Nhap ngay lap (DD/MM/YYYY): ");

        // Nhập trạng thái
        while (true) {
            trangThai =
                NhapDuLieu::nhapChuoi("Nhap trang thai (Chua thanh toan/Da thanh toan): ");

            if (hopLeTrangThai(trangThai))
                break;

            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }

        // Nếu đã thanh toán thì nhập ngày thanh toán
        if (trangThai == "Da thanh toan") {
            ngayThanhToan =
                NhapDuLieu::nhapNgayThang("Nhap ngay thanh toan (DD/MM/YYYY): ");
        }
        else {
            ngayThanhToan = "-";
        }
    }

    // Cập nhật thông tin
    // Không cho phép thay đổi mã hóa đơn
    void capNhatThongTin() override {
        maHopDong =
            NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong moi: ");

        while (true) {
            thangCuoc = NhapDuLieu::nhapChuoi("Nhap thang cuoc moi (MM/YYYY): ");

            if (hopLeThang(thangCuoc))
                break;
            std::cout << " -> Loi: Thang cuoc khong hop le!\n";
        }
        soTien =
            NhapDuLieu::nhapSoThucDuong("Nhap so tien moi: ");
        ngayLap =
            NhapDuLieu::nhapNgayThang("Nhap ngay lap moi (DD/MM/YYYY): ");

        while (true) {
            trangThai =
                NhapDuLieu::nhapChuoi(
                    "Nhap trang thai moi "
                    "(Chua thanh toan/Da thanh toan): ");

            if (hopLeTrangThai(trangThai))
                break;

            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }

        if (trangThai == "Da thanh toan") {
            ngayThanhToan = NhapDuLieu::nhapNgayThang("Nhap ngay thanh toan moi (DD/MM/YYYY): ");
        }
        else {
            ngayThanhToan = "-";
        }
    }

    // Hiển thị thông tin trên một dòng
    void hienThiThongTin() const override {
        std::cout
            << "Ma HD: " << maDinhDanh
            << " | Ma HDong: " << maHopDong
            << " | Thang cuoc: " << thangCuoc
            << " | So tien: "
            << std::fixed << std::setprecision(2)
            << soTien
            << " | Ngay lap: " << ngayLap
            << " | Ngay thanh toan: " << ngayThanhToan
            << " | Trang thai: " << trangThai
            << "\n";
    }

    // Chuyển đối tượng thành chuỗi để lưu file
    std::string chuyenThanhChuoi() const override {
        std::ostringstream oss;

        oss << maDinhDanh << "|"
            << maHopDong << "|"
            << thangCuoc << "|"
            << std::fixed << std::setprecision(2)
            << soTien << "|"
            << ngayLap << "|"
            << ngayThanhToan << "|"
            << trangThai;

        return oss.str();
    }

    // Đọc dữ liệu từ file
    void docTuChuoi(const std::string& dong) override {
        // 7 trường dữ liệu cần 6 dấu '|'
        yeuCau(
            std::count(
                dong.begin(),
                dong.end(),
                '|'
            ) == 6,
            "Dong du lieu HoaDonCuoc co so cot khong dung."
        );

        std::string f[7];
        std::stringstream ss(dong);

        for (int i = 0; i < 7; ++i)
            std::getline(ss, f[i], '|');

        // Kiểm tra mã hóa đơn
        yeuCau(
            hopLeMa(f[0]),
            "Ma hoa don khong hop le: " + f[0]
        );

        // Kiểm tra mã hợp đồng
        yeuCau(
            hopLeMa(f[1]),
            "Ma hop dong khong hop le: " + f[1]
        );

        // Kiểm tra tháng cước
        yeuCau(
            hopLeThang(f[2]),
            "Thang cuoc khong hop le: " + f[2]
        );

        // Đọc và kiểm tra số tiền
        double tien;

        try {
            size_t viTri = 0;
            tien = std::stod(f[3], &viTri);

            yeuCau(
                viTri == f[3].size() && tien >= 0,
                "So tien khong hop le."
            );
        }
        catch (...) {
            throw std::invalid_argument(
                "So tien khong hop le."
            );
        }

        // Kiểm tra ngày lập
        yeuCau(
            hopLeNgay(f[4]),
            "Ngay lap khong hop le: " + f[4]
        );

        // Kiểm tra trạng thái
        yeuCau(
            hopLeTrangThai(f[6]),
            "Trang thai khong hop le: " + f[6]
        );

        // Nếu đã thanh toán thì phải có ngày thanh toán
        if (f[6] == "Da thanh toan") {
            yeuCau(
                hopLeNgay(f[5]),
                "Ngay thanh toan khong hop le: " + f[5]
            );
        }
        else {
            yeuCau(
                f[5] == "-",
                "Hoa don chua thanh toan phai co ngay thanh toan la '-'."
            );
        }

        // Gán dữ liệu
        maDinhDanh = f[0];
        maHopDong = f[1];
        thangCuoc = f[2];
        soTien = tien;
        ngayLap = f[4];
        ngayThanhToan = f[5];
        trangThai = f[6];
    }
};

#endif
