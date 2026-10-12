
#ifndef PHIEU_LAP_DAT_H
#define PHIEU_LAP_DAT_H

/*
 * Ten tac gia: Dinh Van Truong
 * Ma sinh vien: B24DCVT362
 * Use Case: UC07 - Phieu lap dat
 */

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <vector>
#include <ctime>

class PhieuLapDat : public LopCoSo {
private:
    std::string maHopDong;
    std::string maKhachHang;
    std::string maNhanVien;
    std::string ngayLapDat;
    std::string trangThai;

    // Chuan hoa ma: bo khoang trang, viet hoa
    std::string chuanHoaMa(std::string ma) const {
        std::string res;

        for (unsigned char c : ma) {
            if (!std::isspace(c)) {
                res += static_cast<char>(std::toupper(c));
            }
        }

        return res;
    }

    // Kiem tra chuoi khong rong, khong chua dau |
    bool chuoiHopLe(const std::string& str) const {
        return !str.empty()
            && str.find('|') == std::string::npos;
    }

    // Kiem tra ngay co ton tai tren lich
    bool ngayHopLe(const std::string& ngay) const {
        if (ngay.length() != 10 ||
            ngay[2] != '/' ||
            ngay[5] != '/') {
            return false;
        }

        for (int i = 0; i < 10; ++i) {
            if (i == 2 || i == 5) {
                continue;
            }

            if (!std::isdigit(
                    static_cast<unsigned char>(ngay[i]))) {
                return false;
            }
        }

        int d = std::stoi(ngay.substr(0, 2));
        int m = std::stoi(ngay.substr(3, 2));
        int y = std::stoi(ngay.substr(6, 4));

        if (y < 1 || m < 1 || m > 12) {
            return false;
        }

        int soNgay[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        // Nam nhuan: chia het cho 400,
        // hoac chia het cho 4 nhung khong chia het cho 100
        if (y % 400 == 0 ||
            (y % 4 == 0 && y % 100 != 0)) {
            soNgay[1] = 29;
        }

        return d >= 1 && d <= soNgay[m - 1];
    }

    // Kiem tra ngay khong o qua khu
    bool ngayKhongQuaKhu(
        const std::string& ngay) const {

        if (!ngayHopLe(ngay)) {
            return false;
        }

        std::time_t now = std::time(nullptr);
        std::tm* today = std::localtime(&now);

        if (today == nullptr) {
            return false;
        }

        int d = std::stoi(ngay.substr(0, 2));
        int m = std::stoi(ngay.substr(3, 2));
        int y = std::stoi(ngay.substr(6, 4));

        int namHT = today->tm_year + 1900;
        int thangHT = today->tm_mon + 1;
        int ngayHT = today->tm_mday;

        if (y < namHT) return false;

        if (y == namHT && m < thangHT) {
            return false;
        }

        if (y == namHT && m == thangHT && d < ngayHT) {
            return false;
        }

        return true;
    }

    // Kiem tra khoa ngoai trong file du lieu
    bool kiemTraKhoaNgoai(
        const std::string& filePath,
        const std::string& ma) const {

        std::ifstream file(filePath);

        if (!file.is_open()) {
            std::cerr << "LOI: Khong mo duoc file: "
                      << filePath << '\n';
            return false;
        }

        std::string dong;
        std::string maTrongFile;

        while (std::getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') {
                continue;
            }

            std::stringstream ss(dong);

            if (std::getline(ss, maTrongFile, '|')) {
                if (maTrongFile == ma) {
                    return true;
                }
            }
        }

        return false;
    }

    // Kiem tra ban ghi co dung 6 truong hay khong
    bool kiemTraBanGhiNoiBo(
        const std::string& dong) const {

        std::stringstream ss(dong);
        std::string truong;
        std::vector<std::string> fields;

        while (std::getline(ss, truong, '|')) {
            if (truong.empty()) {
                return false;
            }

            fields.push_back(truong);
        }

        if (fields.size() != 6) {
            return false;
        }

        if (!chuoiHopLe(fields[0]) ||
            !chuoiHopLe(fields[1]) ||
            !chuoiHopLe(fields[2]) ||
            !chuoiHopLe(fields[3])) {
            return false;
        }

        if (!ngayHopLe(fields[4])) {
            return false;
        }

        if (fields[5] != "Cho xu ly" &&
            fields[5] != "Dang lap" &&
            fields[5] != "Hoan thanh") {
            return false;
        }

        return true;
    }

public:
    // Constructor mac dinh
    PhieuLapDat() : LopCoSo() {
        maHopDong = "";
        maKhachHang = "";
        maNhanVien = "";
        ngayLapDat = "";
        trangThai = "Cho xu ly";
    }

    // Getter
    std::string getMaPhieuLapDat() const {
        return maDinhDanh;
    }

    std::string getMaHopDong() const {
        return maHopDong;
    }

    std::string getMaKhachHang() const {
        return maKhachHang;
    }

    std::string getMaNhanVien() const {
        return maNhanVien;
    }

    std::string getNgayLapDat() const {
        return ngayLapDat;
    }

    std::string getTrangThai() const {
        return trangThai;
    }

    // Setter
    void setMaHopDong(const std::string& maHD) {
        if (chuoiHopLe(maHD)) {
            maHopDong = chuanHoaMa(maHD);
        }
    }

    void setMaKhachHang(const std::string& maKH) {
        if (chuoiHopLe(maKH)) {
            maKhachHang = chuanHoaMa(maKH);
        }
    }

    void setMaNhanVien(const std::string& maNV) {
        if (chuoiHopLe(maNV)) {
            maNhanVien = chuanHoaMa(maNV);
        }
    }

    void setNgayLapDat(const std::string& ngay) {
        if (ngayKhongQuaKhu(ngay)) {
            ngayLapDat = ngay;
        } else {
            std::cerr
                << "Ngay lap dat khong hop le "
                << "hoac nam trong qua khu!\n";
        }
    }

    void setTrangThai(const std::string& tt) {
        if (tt == "Cho xu ly" ||
            tt == "Dang lap" ||
            tt == "Hoan thanh") {
            trangThai = tt;
        } else {
            std::cerr << "Trang thai khong hop le!\n";
        }
    }

    // Nhap thong tin phieu lap dat
    void nhapThongTin() override {
        // 1. Nhap ma phieu
        do {
            std::cout << "Nhap ma phieu lap dat: ";

            std::string input;
            std::getline(std::cin >> std::ws, input);

            maDinhDanh = chuanHoaMa(input);

            if (!chuoiHopLe(maDinhDanh)) {
                std::cout
                    << "Ma phieu khong duoc rong "
                    << "va khong chua ky tu '|'.\n";
            } else {
                break;
            }
        } while (true);

        // 2. Nhap ma hop dong
        do {
            maHopDong = chuanHoaMa(
                NhapDuLieu::nhapChuoi("Nhap ma hop dong: ")
            );

            if (!chuoiHopLe(maHopDong)) {
                std::cout << "Ma hop dong khong hop le!\n";
            } else if (!kiemTraKhoaNgoai(
                           "data/hop_dong.txt", maHopDong)) {
                std::cout
                    << "Khong tim thay ma hop dong "
                    << "hoac khong mo duoc file. Nhap lai.\n";
            } else {
                break;
            }
        } while (true);

        // 3. Nhap ma khach hang
        do {
            maKhachHang = chuanHoaMa(
                NhapDuLieu::nhapChuoi("Nhap ma khach hang: ")
            );

            if (!chuoiHopLe(maKhachHang)) {
                std::cout << "Ma khach hang khong hop le!\n";
            } else if (!kiemTraKhoaNgoai(
                           "data/khach_hang.txt", maKhachHang)) {
                std::cout
                    << "Khong tim thay ma khach hang "
                    << "hoac khong mo duoc file. Nhap lai.\n";
            } else {
                break;
            }
        } while (true);

        // 4. Nhap ma nhan vien
        do {
            maNhanVien = chuanHoaMa(
                NhapDuLieu::nhapChuoi("Nhap ma nhan vien: ")
            );

            if (!chuoiHopLe(maNhanVien)) {
                std::cout << "Ma nhan vien khong hop le!\n";
            } else if (!kiemTraKhoaNgoai(
                           "data/nhan_vien.txt", maNhanVien)) {
                std::cout
                    << "Khong tim thay ma nhan vien "
                    << "hoac khong mo duoc file. Nhap lai.\n";
            } else {
                break;
            }
        } while (true);

        // 5. Nhap ngay lap dat
        do {
            ngayLapDat = NhapDuLieu::nhapChuoi(
                "Nhap ngay lap dat (DD/MM/YYYY): "
            );

            if (!ngayHopLe(ngayLapDat)) {
                std::cout
                    << "Ngay khong ton tai tren lich! "
                    << "Vi du: 15/03/2026\n";
            } else if (!ngayKhongQuaKhu(ngayLapDat)) {
                std::cout
                    << "Ngay lap dat khong duoc o qua khu!\n";
            } else {
                break;
            }
        } while (true);

        // 6. Chon trang thai
        int chon;

        do {
            std::cout
                << "Chon trang thai phieu:\n"
                << "1. Cho xu ly\n"
                << "2. Dang lap\n"
                << "3. Hoan thanh\n";

            chon = NhapDuLieu::nhapSoNguyen("Chon (1-3): ");

            if (chon < 1 || chon > 3) {
                std::cout
                    << "Lua chon khong hop le! Nhap lai.\n";
            }
        } while (chon < 1 || chon > 3);

        if (chon == 1) {
            trangThai = "Cho xu ly";
        } else if (chon == 2) {
            trangThai = "Dang lap";
        } else {
            trangThai = "Hoan thanh";
        }
    }

    // Hien thi thong tin
    void hienThiThongTin() const override {
        std::cout
            << "Ma phieu: " << maDinhDanh
            << " | Ma HD: " << maHopDong
            << " | Ma KH: " << maKhachHang
            << " | Ma NV: " << maNhanVien
            << " | Ngay lap dat: " << ngayLapDat
            << " | Trang thai: " << trangThai
            << '\n';
    }

    // Chuyen doi tuong thanh dong van ban
    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|"
            + maHopDong + "|"
            + maKhachHang + "|"
            + maNhanVien + "|"
            + ngayLapDat + "|"
            + trangThai;
    }

    // Doc du lieu tu mot dong trong file
    void docTuChuoi(const std::string& dong) override {
        if (!kiemTraBanGhiNoiBo(dong)) {
            std::cerr
                << "Canh bao: Ban ghi phieu lap dat "
                << "khong hop le!\n";
            return;
        }

        std::stringstream ss(dong);

        std::getline(ss, maDinhDanh, '|');
        std::getline(ss, maHopDong, '|');
        std::getline(ss, maKhachHang, '|');
        std::getline(ss, maNhanVien, '|');
        std::getline(ss, ngayLapDat, '|');
        std::getline(ss, trangThai, '|');
    }
};

#endif
