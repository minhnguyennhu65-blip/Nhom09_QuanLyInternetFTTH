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
#include <iomanip>
#include <algorithm>

class PhieuLapDat : public LopCoSo {
private:
    std::string maHopDong;
    std::string maKhachHang;
    std::string maNhanVien;
    std::string ngayLapPhieu;
    std::string ngayLapDat;
    std::string diaChiLapDat;
    std::string trangThai;

    // Chuan hoa ma: xoa khoang trang, chuyen thanh chu hoa
    std::string chuanHoaMa(std::string ma) const {
        std::string res;
        for (unsigned char c : ma) {
            if (!std::isspace(c)) {
                res += static_cast<char>(std::toupper(c));
            }
        }
        return res;
    }

    // Kiem tra chuoi khong rong va khong chua ky tu |
    bool chuoiHopLe(const std::string& str) const {
        return !str.empty() && str.find('|') == std::string::npos;
    }

    // Kiem tra ngay DD/MM/YYYY co hop le tren lich
    bool ngayHopLe(const std::string& ngay) const {
        if (ngay.length() != 10 || ngay[2] != '/' || ngay[5] != '/') {
            return false;
        }

        for (int i = 0; i < 10; ++i) {
            if (i == 2 || i == 5) continue;
            if (!std::isdigit(static_cast<unsigned char>(ngay[i]))) {
                return false;
            }
        }

        int d = std::stoi(ngay.substr(0, 2));
        int m = std::stoi(ngay.substr(3, 2));
        int y = std::stoi(ngay.substr(6, 4));

        if (y < 1 || m < 1 || m > 12) return false;

        int soNgay[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)) {
            soNgay[1] = 29;
        }

        return d >= 1 && d <= soNgay[m - 1];
    }

    // Chuyen DD/MM/YYYY thanh YYYYMMDD de so sanh
    long long chuyenDinhDangSo(const std::string& ngay) const {
        if (!ngayHopLe(ngay)) return 0;
        long long d = std::stoll(ngay.substr(0, 2));
        long long m = std::stoll(ngay.substr(3, 2));
        long long y = std::stoll(ngay.substr(6, 4));
        return y * 10000 + m * 100 + d;
    }

    // Kiem tra ma phieu da ton tai trong file hay chua
    bool kiemTraMaPhieuDaTonTai(const std::string& ma) const {
        std::ifstream file("data/phieu_lap_dat.txt");
        if (!file.is_open()) return false;

        std::string dong, maTrongFile;
        while (std::getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') continue;
            std::stringstream ss(dong);
            if (std::getline(ss, maTrongFile, '|')) {
                if (chuanHoaMa(maTrongFile) == chuanHoaMa(ma)) return true;
            }
        }
        return false;
    }

    // Kiem tra khoa ngoai trong file du lieu lien quan
    bool kiemTraKhoaNgoai(const std::string& filePath, const std::string& ma) const {
        std::ifstream file(filePath);
        if (!file.is_open()) return false;

        std::string dong, maTrongFile;
        while (std::getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') continue;
            std::stringstream ss(dong);
            if (std::getline(ss, maTrongFile, '|')) {
                if (chuanHoaMa(maTrongFile) == chuanHoaMa(ma)) return true;
            }
        }
        return false;
    }

    // Kiem tra cau truc ban ghi doc tu file
    bool kiemTraBanGhiNoiBo(const std::string& dong) const {
        std::stringstream ss(dong);
        std::string truong;
        std::vector<std::string> fields;

        while (std::getline(ss, truong, '|')) {
            if (truong.empty()) return false;
            fields.push_back(truong);
        }

        if (fields.size() != 8) return false;
        for (int i = 0; i < 4; ++i) {
            if (!chuoiHopLe(fields[i])) return false;
        }

        if (!ngayHopLe(fields[4]) || !ngayHopLe(fields[5])) return false;

        if (chuyenDinhDangSo(fields[5]) < chuyenDinhDangSo(fields[4])) return false;
        if (!chuoiHopLe(fields[6])) return false;

        if (fields[7] != "Cho xu ly" && fields[7] != "Dang lap" && fields[7] != "Hoan thanh") {
            return false;
        }

        return true;
    }

    // Cat chuoi dai de khong lam lech cot khi in bang 
    static std::string cat(const std::string& s, std::size_t n) {
        return s.size() <= n ? s : s.substr(0, n - 2) + "..";
    }

public:
    PhieuLapDat() : LopCoSo() {
        maHopDong = "";
        maKhachHang = "";
        maNhanVien = "";
        ngayLapPhieu = "";
        ngayLapDat = "";
        diaChiLapDat = "";
        trangThai = "Cho xu ly";
    }

    // Cac ham getter
    std::string getMaPhieuLapDat() const { return maDinhDanh; }
    std::string getMaHopDong() const { return maHopDong; }
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getMaNhanVien() const { return maNhanVien; }
    std::string getNgayLapPhieu() const { return ngayLapPhieu; }
    std::string getNgayLapDat() const { return ngayLapDat; }
    std::string getDiaChiLapDat() const { return diaChiLapDat; }
    std::string getTrangThai() const { return trangThai; }

    void nhapThongTin() override {
        do {
            std::string input = NhapDuLieu::nhapChuoi("Nhap ma phieu lap dat (nhap 0 de huy): ");
            maDinhDanh = chuanHoaMa(input);

            if (maDinhDanh == "0") {
                std::cout << "Da huy thao tac them phieu lap dat.\n";
                maDinhDanh = "";
                return;
            }

            if (!chuoiHopLe(maDinhDanh)) {
                std::cout << "Ma phieu khong hop le! Vui long nhap lai.\n";
            }
            else if (kiemTraMaPhieuDaTonTai(maDinhDanh)) {
                std::cout << "Ma phieu lap dat da ton tai! Vui long nhap ma khac.\n";
            }
            else {
                break;
            }
        } while (true);

        do {
            std::string inputHD = NhapDuLieu::nhapChuoi("Nhap ma hop dong (nhap 0 de huy): ");
            maHopDong = chuanHoaMa(inputHD);

            if (maHopDong == "0") {
                std::cout << "Da huy thao tac them phieu lap dat.\n";
                maDinhDanh = "";
                return;
            }

            if (!chuoiHopLe(maHopDong)) {
                std::cout << "Ma hop dong khong hop le!\n";
            }
            else if (!kiemTraKhoaNgoai("data/hop_dong.txt", maHopDong)) {
                std::cout << "Khong tim thay ma hop dong. Nhap lai hoac nhap 0 de huy.\n";
            }
            else {
                break;
            }
        } while (true);

        do {
            std::string inputKH = NhapDuLieu::nhapChuoi("Nhap ma khach hang (nhap 0 de huy): ");
            maKhachHang = chuanHoaMa(inputKH);

            if (maKhachHang == "0") {
                std::cout << "Da huy thao tac them phieu lap dat.\n";
                maDinhDanh = "";
                return;
            }

            if (!chuoiHopLe(maKhachHang)) {
                std::cout << "Ma khach hang khong hop le!\n";
            }
            else if (!kiemTraKhoaNgoai("data/khach_hang.txt", maKhachHang)) {
                std::cout << "Khong tim thay ma khach hang. Nhap lai hoac nhap 0 de huy.\n";
            }
            else {
                break;
            }
        } while (true);

        do {
            std::string inputNV = NhapDuLieu::nhapChuoi("Nhap ma nhan vien (nhap 0 de huy): ");
            maNhanVien = chuanHoaMa(inputNV);

            if (maNhanVien == "0") {
                std::cout << "Da huy thao tac them phieu lap dat.\n";
                maDinhDanh = "";
                return;
            }

            if (!chuoiHopLe(maNhanVien)) {
                std::cout << "Ma nhan vien khong hop le!\n";
            }
            else if (!kiemTraKhoaNgoai("data/nhan_vien.txt", maNhanVien)) {
                std::cout << "Khong tim thay ma nhan vien. Nhap lai hoac nhap 0 de huy.\n";
            }
            else {
                break;
            }
        } while (true);

        do {
            ngayLapPhieu = NhapDuLieu::nhapChuoi("Nhap ngay lap phieu (DD/MM/YYYY): ");
            if (!ngayHopLe(ngayLapPhieu)) {
                std::cout << "Ngay khong hop le! Vi du: 15/03/2026\n";
            }
            else {
                break;
            }
        } while (true);

        do {
            ngayLapDat = NhapDuLieu::nhapChuoi("Nhap ngay lap dat (DD/MM/YYYY): ");
            if (!ngayHopLe(ngayLapDat)) {
                std::cout << "Ngay khong hop le! Vi du: 15/03/2026\n";
            }
            else if (chuyenDinhDangSo(ngayLapDat) < chuyenDinhDangSo(ngayLapPhieu)) {
                std::cout << "Ngay lap dat khong duoc truoc ngay lap phieu (" << ngayLapPhieu << ")!\n";
            }
            else {
                break;
            }
        } while (true);

        do {
            diaChiLapDat = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat: ");
            if (!chuoiHopLe(diaChiLapDat)) {
                std::cout << "Dia chi khong hop le hoac chua ky tu '|'.\n";
            }
            else {
                break;
            }
        } while (true);

        int chon;
        do {
            std::cout << "Chon trang thai phieu:\n"
                << "1. Cho xu ly\n"
                << "2. Dang lap\n"
                << "3. Hoan thanh\n";
            chon = NhapDuLieu::nhapSoNguyenDuong("Chon (1-3): ");
            if (chon < 1 || chon > 3) {
                std::cout << "Lua chon khong hop le! Nhap lai.\n";
            }
        } while (chon < 1 || chon > 3);

        if (chon == 1) trangThai = "Cho xu ly";
        else if (chon == 2) trangThai = "Dang lap";
        else trangThai = "Hoan thanh";
    }

    void capNhatThongTin() override {
        std::cout << "\n--- CAP NHAT THONG TIN PHIEU LAP DAT ---\n";

        while (true) {
            std::string ngayMoi = NhapDuLieu::nhapChuoi("Nhap ngay lap dat moi (DD/MM/YYYY): ");
            if (!ngayHopLe(ngayMoi)) {
                std::cout << "Ngay khong hop le!\n";
            }
            else if (chuyenDinhDangSo(ngayMoi) < chuyenDinhDangSo(ngayLapPhieu)) {
                std::cout << "Ngay lap dat khong duoc truoc ngay lap phieu (" << ngayLapPhieu << ")!\n";
            }
            else {
                ngayLapDat = ngayMoi;
                break;
            }
        }

        while (true) {
            std::string dcMoi = NhapDuLieu::nhapChuoi("Nhap dia chi lap dat moi: ");
            if (chuoiHopLe(dcMoi)) {
                diaChiLapDat = dcMoi;
                break;
            }
            std::cout << "Dia chi khong hop le!\n";
        }

        int chon;
        do {
            std::cout << "Chon trang thai moi:\n"
                << "1. Cho xu ly\n"
                << "2. Dang lap\n"
                << "3. Hoan thanh\n";
            chon = NhapDuLieu::nhapSoNguyenDuong("Chon (1-3): ");
        } while (chon < 1 || chon > 3);

        if (chon == 1) trangThai = "Cho xu ly";
        else if (chon == 2) trangThai = "Dang lap";
        else trangThai = "Hoan thanh";

        std::cout << "Cap nhat phieu lap dat thanh cong!\n";
    }

    static void inTieuDe() {
        std::cout << std::left
            << std::setw(12) << "Ma phieu" << " | "
            << std::setw(13) << "Ma hop dong" << " | "
            << std::setw(13) << "Ma khach hang" << " | "
            << std::setw(12) << "Ma nhan vien" << " | "
            << std::setw(14) << "Ngay lap" << " | "
            << std::setw(14) << "Ngay dat" << " | "
            << std::setw(20) << "Dia chi" << " | "
            << std::setw(14) << "Trang thai" << '\n'
            << std::string(128, '-') << '\n';
    }


    void hienThiThongTin() const override {
        std::cout << std::left
            << std::setw(12) << cat(maDinhDanh, 11) << " | "
            << std::setw(13) << cat(maHopDong, 12) << " | "
            << std::setw(13) << cat(maKhachHang, 12) << " | "
            << std::setw(12) << cat(maNhanVien, 11) << " | "
            << std::setw(14) << ngayLapPhieu << " | "
            << std::setw(14) << ngayLapDat << " | "
            << std::setw(20) << cat(diaChiLapDat, 19) << " | "
            << std::setw(14) << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + maHopDong + "|" + maKhachHang + "|" + maNhanVien + "|" + ngayLapPhieu + "|" + ngayLapDat + "|" + diaChiLapDat + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        if (!kiemTraBanGhiNoiBo(dong)) {
            return;
        }

        std::stringstream ss(dong);
        std::getline(ss, maDinhDanh, '|');
        std::getline(ss, maHopDong, '|');
        std::getline(ss, maKhachHang, '|');
        std::getline(ss, maNhanVien, '|');
        std::getline(ss, ngayLapPhieu, '|');
        std::getline(ss, ngayLapDat, '|');
        std::getline(ss, diaChiLapDat, '|');
        std::getline(ss, trangThai, '|');
    }
};

#endif
