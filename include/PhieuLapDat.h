/* =========================================================
- Ten tac gia: Dinh Van Truong
- Ma sinh vien: B24DCVT362
- Use Case: UC07 - Phieu lap dat
 ========================================================= */

#ifndef PHIEU_LAP_DAT_H
#define PHIEU_LAP_DAT_H

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
        if (str.find('|') != std::string::npos) {
            return false;
        }
        for (unsigned char c : str) {
            if (!std::isspace(c)) {
                return true;
            }
        }
        return false;
    }

    // Kiem tra ngay co ton tai tren lich
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
        bool namNhuan = (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
        if (namNhuan) soNgay[1] = 29;

        return d >= 1 && d <= soNgay[m - 1];
    }

    // Kiem tra ngay khong o qua khu
    bool ngayKhongQuaKhu(const std::string& ngay) const {
        if (!ngayHopLe(ngay)) return false;

        int d = std::stoi(ngay.substr(0, 2));
        int m = std::stoi(ngay.substr(3, 2));
        int y = std::stoi(ngay.substr(6, 4));

        std::time_t now = std::time(nullptr);
        std::tm* today = std::localtime(&now);
        if (today == nullptr) return false;

        int namHT = today->tm_year + 1900;
        int thangHT = today->tm_mon + 1;
        int ngayHT = today->tm_mday;

        if (y != namHT) return y > namHT;
        if (m != thangHT) return m > thangHT;
        return d >= ngayHT;
    }

    // Kiem tra khoa ngoai trong file du lieu (Tu dong tao file neu chua co)
    bool kiemTraKhoaNgoai(const std::string& filePath, const std::string& ma) const {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::ofstream taoFileMoi(filePath);
            if (taoFileMoi.is_open()) taoFileMoi.close();
            return false; 
        }

        std::string dong, maTrongFile;
        std::string maCanTim = chuanHoaMa(ma);

        while (std::getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') continue;
            std::stringstream ss(dong);
            if (std::getline(ss, maTrongFile, '|')) {
                if (chuanHoaMa(maTrongFile) == maCanTim) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }

    // Kiem tra trung ma phieu lap dat
    bool kiemTraTrungMaPhieu(const std::string& filePath, const std::string& maPhieu) const {
        std::ifstream file(filePath);
        if (!file.is_open()) return false;

        std::string dong, maTrongFile;
        std::string maCanTim = chuanHoaMa(maPhieu);

        while (std::getline(file, dong)) {
            if (dong.empty()) continue;
            std::stringstream ss(dong);
            std::getline(ss, maTrongFile, '|');
            if (chuanHoaMa(maTrongFile) == maCanTim) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    std::vector<std::string> tachBanGhi(const std::string& dong) const {
        std::vector<std::string> fields;
        std::size_t start = 0;
        while (true) {
            std::size_t pos = dong.find('|', start);
            if (pos == std::string::npos) {
                fields.push_back(dong.substr(start));
                break;
            }
            fields.push_back(dong.substr(start, pos - start));
            start = pos + 1;
        }
        return fields;
    }

    std::string nhapMaThamChieu(const std::string& loiNhac, const std::string& filePath, const std::string& tenLoai) {
        while (true) {
            std::string ma = NhapDuLieu::nhapChuoi(loiNhac);
            ma = chuanHoaMa(ma);

            if (!chuoiHopLe(ma)) {
                std::cout << "Loi: Ma khong duoc rong hoac chua ky tu '|'.\n";
                continue;
            }

            if (!kiemTraKhoaNgoai(filePath, ma)) {
                std::cout << "Ma " << tenLoai << " [" << ma << "] khong ton tai trong " << filePath << ". Nhap lai.\n";
                continue;
            }

            return ma;
        }
    }

public:
    PhieuLapDat() : LopCoSo() {
        maHopDong = "";
        maKhachHang = "";
        maNhanVien = "";
        ngayLapDat = "";
        trangThai = "Cho xu ly";
    }

    // GETTER
    std::string getMaPhieuLapDat() const { return maDinhDanh; }
    std::string getMaHopDong() const { return maHopDong; }
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getMaNhanVien() const { return maNhanVien; }
    std::string getNgayLapDat() const { return ngayLapDat; }
    std::string getTrangThai() const { return trangThai; }

    // SETTER
    void setMaHopDong(const std::string& maHD) {
        if (chuoiHopLe(maHD)) maHopDong = chuanHoaMa(maHD);
    }
    void setMaKhachHang(const std::string& maKH) {
        if (chuoiHopLe(maKH)) maKhachHang = chuanHoaMa(maKH);
    }
    void setMaNhanVien(const std::string& maNV) {
        if (chuoiHopLe(maNV)) maNhanVien = chuanHoaMa(maNV);
    }
    void setNgayLapDat(const std::string& ngay) {
        if (ngayKhongQuaKhu(ngay)) ngayLapDat = ngay;
    }
    void setTrangThai(const std::string& tt) {
        if (tt == "Cho xu ly" || tt == "Dang lap" || tt == "Hoan thanh") trangThai = tt;
    }

    // NHAP THONG TIN
    void nhapThongTin() override {
        // 1. Nhap ma phieu
        while (true) {
            std::cout << "Nhap ma phieu lap dat: ";
            std::string input;
            std::getline(std::cin >> std::ws, input);
            input = chuanHoaMa(input);

            if (!chuoiHopLe(input)) {
                std::cout << "Loi: Ma phieu khong duoc rong hoac chua ky tu '|'.\n";
                continue;
            }
            if (kiemTraTrungMaPhieu("data/phieu_lap_dat.txt", input)) {
                std::cout << "Loi: Ma phieu lap dat [" << input << "] DA TON TAI!\n";
                continue;
            }
            maDinhDanh = input;
            break;
        }

        // 2, 3, 4. Nhap cac ma tham chieu khoa ngoai
        maHopDong = nhapMaThamChieu("Nhap ma hop dong: ", "data/hop_dong.txt", "hop dong");
        maKhachHang = nhapMaThamChieu("Nhap ma khach hang: ", "data/khach_hang.txt", "khach hang");
        maNhanVien = nhapMaThamChieu("Nhap ma nhan vien: ", "data/nhan_vien.txt", "nhan vien");

        // 5. Ngay lap dat
        while (true) {
            std::string ngay = NhapDuLieu::nhapChuoi("Nhap ngay lap dat (DD/MM/YYYY): ");
            if (!ngayHopLe(ngay)) {
                std::cout << "Loi: Ngay khong ton tai tren lich!\n";
                continue;
            }
            if (!ngayKhongQuaKhu(ngay)) {
                std::cout << "Loi: Ngay lap dat khong duoc trong qua khu!\n";
                continue;
            }
            ngayLapDat = ngay;
            break;
        }

        // 6. Trang thai
        int chon;
        do {
            std::cout << "\nChon trang thai phieu:\n1. Cho xu ly\n2. Dang lap\n3. Hoan thanh\n";
            chon = NhapDuLieu::nhapSoNguyenDuong("Chon (1-3): ");
            if (chon < 1 || chon > 3) {
                std::cout << "Loi: Chi duoc chon tu 1 den 3!\n";
            }
        } while (chon < 1 || chon > 3);

        if (chon == 1) trangThai = "Cho xu ly";
        else if (chon == 2) trangThai = "Dang lap";
        else trangThai = "Hoan thanh";
    }

    // CAP NHAT THONG TIN (Override bat buoc tu LopCoSo)
    void capNhatThongTin() override {
        std::cout << "\n--- CAP NHAT THONG TIN PHIEU LAP DAT ---\n";
        
        while (true) {
            std::string ngay = NhapDuLieu::nhapChuoi("Nhap ngay lap dat moi (DD/MM/YYYY): ");
            if (!ngayHopLe(ngay)) {
                std::cout << "Loi: Ngay khong hop le!\n";
            } else if (!ngayKhongQuaKhu(ngay)) {
                std::cout << "Loi: Ngay lap dat khong duoc trong qua khu!\n";
            } else {
                ngayLapDat = ngay;
                break;
            }
        }

        int chon;
        do {
            std::cout << "Chon trang thai moi:\n1. Cho xu ly\n2. Dang lap\n3. Hoan thanh\n";
            chon = NhapDuLieu::nhapSoNguyenDuong("Chon (1-3): ");
            if (chon < 1 || chon > 3) {
                std::cout << "Loi: Chi duoc chon tu 1 den 3!\n";
            }
        } while (chon < 1 || chon > 3);

        if (chon == 1) trangThai = "Cho xu ly";
        else if (chon == 2) trangThai = "Dang lap";
        else trangThai = "Hoan thanh";
    }

    void hienThiThongTin() const override {
        std::cout << "Ma phieu LD: " << maDinhDanh
            << " | Ma HD: " << maHopDong
            << " | Ma KH: " << maKhachHang
            << " | Ma NV: " << maNhanVien
            << " | Ngay lap: " << ngayLapDat
            << " | Trang thai: " << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + maHopDong + "|" + maKhachHang + "|" + maNhanVien + "|" + ngayLapDat + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        std::vector<std::string> f = tachBanGhi(dong);

        if (f.size() != 6 || !chuoiHopLe(f[0]) || !chuoiHopLe(f[1]) || !chuoiHopLe(f[2]) ||
            !chuoiHopLe(f[3]) || !ngayHopLe(f[4]) || 
            (f[5] != "Cho xu ly" && f[5] != "Dang lap" && f[5] != "Hoan thanh")) {
            std::cout << "-> CANH BAO: Bo qua dong du lieu loi dinh dang: " << dong << "\n";
            return;
        }

        maDinhDanh = chuanHoaMa(f[0]);
        maHopDong = chuanHoaMa(f[1]);
        maKhachHang = chuanHoaMa(f[2]);
        maNhanVien = chuanHoaMa(f[3]);
        ngayLapDat = f[4];
        trangThai = f[5];
    }
};

#endif // PHIEU_LAP_DAT_H
