/* =======================================================
 * Tên tác giả: Nguyễn Đức Huy
 * Mã sinh viên: B24DCVT176
 * Mô tả file: Lớp hóa đơn cước
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
    std::string maHopDong, thangCuoc;
    double soTien = 0;
    std::string ngayLap, ngayThanhToan = "-";
    std::string trangThai = "Chua thanh toan";

    static bool hopLeMa(const std::string& s) {
        if (s.empty() || s.find('|') != std::string::npos)
            return false;
        for (unsigned char c : s)
            if (std::isspace(c) || std::islower(c)) return false;
        return true;
    }

    static bool hopLeThang(const std::string& s) {
        if (s.size() != 7 || s[2] != '/') return false;
        for (int i = 0; i < 7; i++)
            if (i != 2 && !std::isdigit(
                static_cast<unsigned char>(s[i])))
                return false;

        int thang = std::stoi(s.substr(0, 2));
        int nam = std::stoi(s.substr(3, 4));
        return thang >= 1 && thang <= 12 &&
               nam >= 1900 && nam <= 2100;
    }

    static bool hopLeNgay(const std::string& s) {
        if (s.size() != 10 || s[2] != '/' || s[5] != '/')
            return false;

        for (int i = 0; i < 10; i++)
            if (i != 2 && i != 5 &&
                !std::isdigit(
                    static_cast<unsigned char>(s[i])))
                return false;

        try {
            return NhapDuLieu::ngayHopLe(
                std::stoi(s.substr(0, 2)),
                std::stoi(s.substr(3, 2)),
                std::stoi(s.substr(6, 4))
            );
        }
        catch (...) {
            return false;
        }
    }

    static bool hopLeTrangThai(const std::string& s) {
        return s == "Chua thanh toan" ||
               s == "Da thanh toan";
    }

    static void yeuCau(bool dk, const std::string& msg) {
        if (!dk) throw std::invalid_argument(msg);
    }

public:
    HoaDonCuoc() = default;

    const std::string& getMaHopDong() const {
        return maHopDong;
    }

    const std::string& getTrangThai() const {
        return trangThai;
    }

    void nhapThongTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma hoa don: ");
        maHopDong = NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong: ");

        do {
            thangCuoc =
                NhapDuLieu::nhapChuoi("Nhap thang cuoc (MM/YYYY): ");
            if (!hopLeThang(thangCuoc))
                std::cout << " -> Loi: Thang khong hop le!\n";
        } while (!hopLeThang(thangCuoc));
        soTien = NhapDuLieu::nhapSoThucDuong("Nhap so tien: ");
        ngayLap = NhapDuLieu::nhapNgayThang("Nhap ngay lap (DD/MM/YYYY): ");
        do {
            trangThai =
                NhapDuLieu::nhapChuoi("Nhap trang thai (Chua thanh toan/Da thanh toan): ");
            if (!hopLeTrangThai(trangThai))
                std::cout << " -> Loi: Trang thai khong hop le!\n";
        } while (!hopLeTrangThai(trangThai));

        if (trangThai == "Da thanh toan")
            ngayThanhToan =
                NhapDuLieu::nhapNgayThang("Nhap ngay thanh toan (DD/MM/YYYY): ");
        else
            ngayThanhToan = "-";
    }

    void capNhatThongTin() override {
        maHopDong =
            NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong moi: ");

        do {
            thangCuoc =
                NhapDuLieu::nhapChuoi("Nhap thang cuoc moi (MM/YYYY): ");
        } while (!hopLeThang(thangCuoc));

        soTien =
            NhapDuLieu::nhapSoThucDuong("Nhap so tien moi: ");

        ngayLap =
            NhapDuLieu::nhapNgayThang( "Nhap ngay lap moi: ");

        do {
            trangThai =
                NhapDuLieu::nhapChuoi( "Nhap trang thai moi: ");
        } while (!hopLeTrangThai(trangThai));

        if (trangThai == "Da thanh toan")
            ngayThanhToan = NhapDuLieu::nhapNgayThang("Nhap ngay thanh toan moi: ");
        else
            ngayThanhToan = "-";
    }

    void hienThiThongTin() const override {
        std::cout
            << "Ma HD: " << maDinhDanh
            << " | Ma HDong: " << maHopDong
            << " | Thang: " << thangCuoc
            << " | So tien: " << std::fixed
            << std::setprecision(2) << soTien
            << " | Ngay lap: " << ngayLap
            << " | Ngay TT: " << ngayThanhToan
            << " | Trang thai: " << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        std::ostringstream os;
        os << maDinhDanh << "|" << maHopDong << "|"
           << thangCuoc << "|" << std::fixed
           << std::setprecision(2) << soTien << "|"
           << ngayLap << "|" << ngayThanhToan
           << "|" << trangThai;
        return os.str();
    }

    void docTuChuoi(const std::string& dong) override {
        if (std::count(dong.begin(), dong.end(), '|') != 6)
            throw std::invalid_argument("Sai so cot!");

        std::string f[7];
        std::stringstream ss(dong);

        for (int i = 0; i < 7; i++)
            std::getline(ss, f[i], '|');

        yeuCau(hopLeMa(f[0]), "Ma hoa don khong hop le!");
        yeuCau(hopLeMa(f[1]), "Ma hop dong khong hop le!");
        yeuCau(hopLeThang(f[2]), "Thang cuoc khong hop le!");
        yeuCau(hopLeNgay(f[4]), "Ngay lap khong hop le!");
        yeuCau(hopLeTrangThai(f[6]), "Trang thai khong hop le!");

        try {
            size_t pos;
            soTien = std::stod(f[3], &pos);
            yeuCau(pos == f[3].size() && soTien >= 0,"So tien khong hop le!");
        }
        catch (...) {
            throw std::invalid_argument("So tien khong hop le!");
        }

        if (f[6] == "Da thanh toan")
            yeuCau(hopLeNgay(f[5]),"Ngay thanh toan khong hop le!");
        else
            yeuCau(f[5] == "-","Ngay thanh toan phai la '-'!");

        maDinhDanh = f[0];
        maHopDong = f[1];
        thangCuoc = f[2];
        ngayLap = f[4];
        ngayThanhToan = f[5];
        trangThai = f[6];
    }
};

#endif
