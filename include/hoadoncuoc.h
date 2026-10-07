/* =======================================================
 * Tên tác giả: Nguyễn Đức Huy
 * Mã sinh viên: B24DCVT176
 * Mô tả file: Lớp hóa dơn cước
 * ======================================================= */
#ifndef HOA_DON_CUOC_H
#define HOA_DON_CUOC_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

class HoaDonCuoc : public LopCoSo {
private:
    std::string maHopDong;
    std::string thangCuoc;
    double soTien;
    std::string ngayLap;
    std::string ngayThanhToan;
    std::string trangThai;

    bool ngayHopLe(const std::string& s) const {
        if (s.length() != 10 ||
            s[2] != '/' ||
            s[5] != '/')
            return false;

        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) continue;

            if (!std::isdigit(
                static_cast<unsigned char>(s[i])))
                return false;
        }

        int ngay = std::stoi(s.substr(0, 2));
        int thang = std::stoi(s.substr(3, 2));
        int nam = std::stoi(s.substr(6, 4));
        if (thang < 1 || thang > 12 ||
            ngay < 1 || nam < 1)
            return false;
        int soNgay = 31;
        if (thang == 4 || thang == 6 ||
            thang == 9 || thang == 11)
            soNgay = 30;
        if (thang == 2)
            soNgay = (nam % 400 == 0 ||
                     (nam % 4 == 0 && nam % 100 != 0)) ? 29 : 28;

        return ngay <= soNgay;
    }

    bool thangHopLe(const std::string& s) const {
        if (s.length() != 7 || s[2] != '/')
            return false;
        for (int i = 0; i < 7; i++) {
            if (i == 2) continue;
            if (!std::isdigit(
                static_cast<unsigned char>(s[i])))
                return false;
        }

        int thang = std::stoi(s.substr(0, 2));
        int nam = std::stoi(s.substr(3, 4));

        return thang >= 1 &&
               thang <= 12 &&
               nam >= 1;
    }

    bool coKyTuPhanCach(
        const std::string& s) const {
        return s.find('|') != std::string::npos;
    }
    bool trangThaiHopLe(
        const std::string& s) const {
        return s == "Chua thanh toan" || s == "Da thanh toan";
    }
public:
    HoaDonCuoc()
        : LopCoSo(),
          soTien(0),
          ngayThanhToan("-"),
          trangThai("Chua thanh toan") {
    }
    void nhapThongTin() override {
        std::cout << "\n===== NHAP HOA DON CUOC =====\n";
        while (true) {
            maDinhDanh =
                NhapDuLieu::nhapChuoi("Nhap ma hoa don: ");
            if (!coKyTuPhanCach(maDinhDanh))
                break;
            std::cout<< " -> Loi: Ma khong duoc chua '|'.\n";
        }

        while (true) {
            maHopDong =
                NhapDuLieu::nhapChuoi("Nhap ma hop dong: ");
            if (!coKyTuPhanCach(maHopDong))
                break;

            std::cout<< " -> Loi: Ma hop dong khong duoc chua '|'.\n";
        }
        while (true) {
            thangCuoc =
                NhapDuLieu::nhapChuoi("Nhap thang cuoc (MM/YYYY): ");
            if (thangHopLe(thangCuoc))
                break;
            std::cout<< " -> Loi: Thang cuoc phai co dang MM/YYYY.\n";
        }
        while (true) {
            soTien =
                NhapDuLieu::nhapSoThuc("Nhap so tien: ");
            if (soTien >= 0)
                break;
            std::cout<< " -> Loi: So tien phai >= 0.\n";
        }
        while (true) {
            ngayLap =
                NhapDuLieu::nhapChuoi("Nhap ngay lap (DD/MM/YYYY): ");
            if (ngayHopLe(ngayLap))
                break;
            std::cout<< " -> Loi: Ngay khong hop le.\n";
        }
        if (NhapDuLieu::xacNhan("Hoa don da thanh toan?")) {
            trangThai = "Da thanh toan";
            while (true) {
                ngayThanhToan =
                    NhapDuLieu::nhapChuoi("Nhap ngay thanh toan (DD/MM/YYYY): ");
                if (ngayHopLe(ngayThanhToan))
                    break;
                std::cout<< " -> Loi: Ngay khong hop le.\n";
            }
        }
        else {
            trangThai = "Chua thanh toan";
            ngayThanhToan = "-";
        }
    }
    void hienThiThongTin() const override {
        std::cout << "\n===== HOA DON CUOC =====\n";
        std::cout << "Ma hoa don: "<< maDinhDanh << '\n';
        std::cout << "Ma hop dong: "<< maHopDong << '\n';
        std::cout << "Thang cuoc: "<< thangCuoc << '\n';
        std::cout << "So tien: "<< soTien << '\n';
        std::cout << "Ngay lap: "<< ngayLap << '\n';
        std::cout << "Ngay thanh toan: "<< ngayThanhToan << '\n';
        std::cout << "Trang thai: "<< trangThai << '\n';
    }
    std::string chuyenThanhChuoi()
        const override {

        std::ostringstream oss;

        oss << maDinhDanh << '|'
            << maHopDong << '|'
            << thangCuoc << '|'
            << soTien << '|'
            << ngayLap << '|'
            << ngayThanhToan << '|'
            << trangThai;

        return oss.str();
    }
    void docTuChuoi(
        const std::string& dong) override {
        std::stringstream ss(dong);
        std::vector<std::string> f;
        std::string x;
        while (std::getline(ss, x, '|'))
            f.push_back(x);
        if (f.size() != 7)
            throw std::invalid_argument("Dong du lieu khong hop le!");
        if (f[0].empty() ||
            f[1].empty() ||
            f[2].empty() ||
            f[4].empty())
            throw std::invalid_argument("Du lieu khong duoc de trong!");
        if (!thangHopLe(f[2]))
            throw std::invalid_argument("Thang cuoc khong hop le!");
        if (!ngayHopLe(f[4]))
            throw std::invalid_argument("Ngay lap khong hop le!");
        double tien;
        try {
            size_t pos;
            tien = std::stod(f[3], &pos);
            if (pos != f[3].size() || tien < 0)
                throw std::invalid_argument("So tien khong hop le!");
        }
        catch (...) {
            throw std::invalid_argument("So tien khong hop le!");
        }
        if (!trangThaiHopLe(f[6]))
            throw std::invalid_argument("Trang thai khong hop le!");
        if (f[6] == "Da thanh toan") {
            if (!ngayHopLe(f[5]))
                throw std::invalid_argument("Ngay thanh toan khong hop le!");
        }
        else {
            if (f[5] != "-")
                throw std::invalid_argument("Ngay thanh toan phai la '-'!");
        }

        maDinhDanh = f[0];
        maHopDong = f[1];
        thangCuoc = f[2];
        soTien = tien;
        ngayLap = f[4];
        ngayThanhToan = f[5];
        trangThai = f[6];
    }
    std::string getMaHoaDon() const {
        return maDinhDanh;
    }
    std::string getMaHopDong() const {
        return maHopDong;
    }
    std::string getThangCuoc() const {
        return thangCuoc;
    }
    double getSoTien() const {
        return soTien;
    }
    std::string getNgayLap() const {
        return ngayLap;
    }
    std::string getNgayThanhToan() const {
        return ngayThanhToan;
    }
    std::string getTrangThai() const {
        return trangThai;
    }
};

#endif
