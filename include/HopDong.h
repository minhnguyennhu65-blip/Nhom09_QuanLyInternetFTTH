#ifndef HOP_DONG_H
#define HOP_DONG_H

/* =======================================================
 * Tên tác giả: Hoàng Trọng Khoa
 * Mã sinh viên: B24DCVT204
 * Mô tả file: Lớp quản lý Hợp Đồng
 * ======================================================= */


#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <vector>

class HopDong : public LopCoSo {
private:
    std::string maKhachHang;
    std::string maGoiCuoc;
    std::string maThietBi;
    std::string ngayKy;
    std::string ngayHetHan;
    double giaThieuDung;
    std::string trangThai;

    std::string chuanHoaMa(std::string ma) const {
        std::string res;
        for (unsigned char c : ma) {
            if (!std::isspace(c)) {
                res += static_cast<char>(std::toupper(c));
            }
        }
        return res;
    }

    bool chuoiHopLe(const std::string& str) const {
        return !str.empty() && str.find('|') == std::string::npos;
    }

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

        if (y < 1900 || y > 2100 || m < 1 || m > 12) return false;

        int soNgay[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)) {
            soNgay[1] = 29;
        }

        return d >= 1 && d <= soNgay[m - 1];
    }

    int ngayThanhSo(const std::string& ngay) const {
        if (!ngayHopLe(ngay)) return -1;
        int d = std::stoi(ngay.substr(0, 2));
        int m = std::stoi(ngay.substr(3, 2));
        int y = std::stoi(ngay.substr(6, 4));
        return y * 10000 + m * 100 + d;
    }

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

    bool kiemTraBanGhiNoiBo(const std::string& dong) const {
        std::stringstream ss(dong);
        std::string truong;
        std::vector<std::string> fields;

        while (std::getline(ss, truong, '|')) {
            fields.push_back(truong);
        }

        if (!fields.empty() && !fields.back().empty() && fields.back().back() == '\r') {
            fields.back().pop_back();
        }

        if (fields.size() != 8) return false;
        if (!chuoiHopLe(fields[0]) || !chuoiHopLe(fields[1]) || !chuoiHopLe(fields[2]) || !chuoiHopLe(fields[3])) return false;
        if (!ngayHopLe(fields[4]) || !ngayHopLe(fields[5])) return false;
        if (ngayThanhSo(fields[5]) < ngayThanhSo(fields[4])) return false;
        if (fields[7] != "Hoat dong" && fields[7] != "Het han" && fields[7] != "Huy") return false;

        return true;
    }

public:
    HopDong() : LopCoSo() {
        maKhachHang = "";
        maGoiCuoc = "";
        maThietBi = "";
        ngayKy = "";
        ngayHetHan = "";
        giaThieuDung = 0.0;
        trangThai = "Hoat dong";
    }

    std::string getMaHopDong() const { return maDinhDanh; }
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getMaGoiCuoc() const { return maGoiCuoc; }
    std::string getMaThietBi() const { return maThietBi; }
    std::string getNgayKy() const { return ngayKy; }
    std::string getNgayHetHan() const { return ngayHetHan; }
    double getGiaThieuDung() const { return giaThieuDung; }
    std::string getTrangThai() const { return trangThai; }

    void setTrangThai(const std::string& tt) { trangThai = tt; }

    void nhapThongTin() override {
        // 1. Nhap ma hop dong
        do {
            std::string input = NhapDuLieu::nhapChuoi("Nhap ma hop dong (nhap 0 de huy): ");
            maDinhDanh = chuanHoaMa(input);

            if (maDinhDanh == "0") {
                std::cout << " -> Da huy thao tac them hop dong.\n";
                maDinhDanh = ""; // Xoa ma dinh danh de danh dau huy
                return;
            }

            if (!chuoiHopLe(maDinhDanh)) {
                std::cout << "Ma hop dong khong duoc rong va khong chua ky tu '|'.\n";
            }
            else if (kiemTraKhoaNgoai("data/hop_dong.txt", maDinhDanh)) {
                std::cout << "Ma hop dong da ton tai trong he thong!\n";
            }
            else {
                break;
            }
        } while (true);

        // 2. Nhap ma khach hang
        do {
            std::string inputKH = NhapDuLieu::nhapChuoi("Nhap ma khach hang (nhap 0 de huy): ");
            maKhachHang = chuanHoaMa(inputKH);

            if (maKhachHang == "0") {
                std::cout << " -> Da huy thao tac them hop dong.\n";
                maDinhDanh = "";
                return;
            }

            if (!chuoiHopLe(maKhachHang)) {
                std::cout << "Ma khach hang khong hop le!\n";
            }
            else if (!kiemTraKhoaNgoai("data/khach_hang.txt", maKhachHang)) {
                std::cout << "Khong tim thay ma khach hang hoac chua co file du lieu. Nhap lai hoac nhap 0 de huy.\n";
            }
            else {
                break;
            }
        } while (true);

        // 3. Nhap ma goi cuoc
        do {
            std::string inputGoi = NhapDuLieu::nhapChuoi("Nhap ma goi cuoc (nhap 0 de huy): ");
            maGoiCuoc = chuanHoaMa(inputGoi);

            if (maGoiCuoc == "0") {
                std::cout << " -> Da huy thao tac them hop dong.\n";
                maDinhDanh = "";
                return;
            }

            if (!chuoiHopLe(maGoiCuoc)) {
                std::cout << "Ma goi cuoc khong hop le!\n";
            }
            else if (!kiemTraKhoaNgoai("data/goi_internet.txt", maGoiCuoc)) {
                std::cout << "Khong tim thay ma goi cuoc hoac chua co file du lieu. Nhap lai hoac nhap 0 de huy.\n";
            }
            else {
                break;
            }
        } while (true);

        // 4. Nhap ma thiet bi
        do {
            std::string inputTB = NhapDuLieu::nhapChuoi("Nhap ma thiet bi (nhap 0 de huy): ");
            maThietBi = chuanHoaMa(inputTB);

            if (maThietBi == "0") {
                std::cout << " -> Da huy thao tac them hop dong.\n";
                maDinhDanh = "";
                return;
            }

            if (!chuoiHopLe(maThietBi)) {
                std::cout << "Ma thiet bi khong hop le!\n";
            }
            else if (!kiemTraKhoaNgoai("data/thiet_bi.txt", maThietBi)) {
                std::cout << "Khong tim thay ma thiet bi hoac chua co file du lieu. Nhap lai hoac nhap 0 de huy.\n";
            }
            else {
                break;
            }
        } while (true);

        // 5. Nhap ngay ky va ngay het han
        do {
            ngayKy = NhapDuLieu::nhapChuoi("Nhap ngay ky hop dong (DD/MM/YYYY): ");
            if (!ngayHopLe(ngayKy)) {
                std::cout << "Ngay ky khong ton tai tren lich! Vi du: 15/03/2026\n";
                continue;
            }

            ngayHetHan = NhapDuLieu::nhapChuoi("Nhap ngay het han hop dong (DD/MM/YYYY): ");
            if (!ngayHopLe(ngayHetHan)) {
                std::cout << "Ngay het han khong ton tai tren lich! Vi du: 15/03/2027\n";
                continue;
            }

            if (ngayThanhSo(ngayHetHan) < ngayThanhSo(ngayKy)) {
                std::cout << "Ngay het han phai sau hoac bang ngay ky! Vui long nhap lai.\n";
                continue;
            }

            break;
        } while (true);

        // 6. Nhap gia thieu dung
        giaThieuDung = NhapDuLieu::nhapSoThucDuong("Nhap gia thieu dung hang thang (>= 0): ");

        // 7. Chon trang thai
        int chon;
        do {
            std::cout << "Chon trang thai hop dong:\n1. Hoat dong\n2. Het han\n3. Huy\n";
            chon = NhapDuLieu::nhapSoNguyenDuong("Chon (1-3): ");
            if (chon < 1 || chon > 3) std::cout << "Lua chon khong hop le! Nhap lai.\n";
        } while (chon < 1 || chon > 3);

        if (chon == 1) trangThai = "Hoat dong";
        else if (chon == 2) trangThai = "Het han";
        else trangThai = "Huy";
    }

    void capNhatThongTin() override {
        std::cout << "\n--- CAP NHAT THONG TIN HOP DONG ---\n";
        do {
            ngayKy = NhapDuLieu::nhapChuoi("Nhap ngay ky hop dong moi (DD/MM/YYYY): ");
            if (!ngayHopLe(ngayKy)) {
                std::cout << "Ngay ky khong ton tai tren lich!\n";
                continue;
            }

            ngayHetHan = NhapDuLieu::nhapChuoi("Nhap ngay het han hop dong moi (DD/MM/YYYY): ");
            if (!ngayHopLe(ngayHetHan)) {
                std::cout << "Ngay het han khong ton tai tren lich!\n";
                continue;
            }

            if (ngayThanhSo(ngayHetHan) < ngayThanhSo(ngayKy)) {
                std::cout << "Ngay het han phai sau hoac bang ngay ky!\n";
                continue;
            }

            break;
        } while (true);

        giaThieuDung = NhapDuLieu::nhapSoThucDuong("Nhap gia thieu dung hang thang moi (>= 0): ");

        int chon;
        do {
            std::cout << "Chon trang thai moi:\n1. Hoat dong\n2. Het han\n3. Huy\n";
            chon = NhapDuLieu::nhapSoNguyenDuong("Chon (1-3): ");
            if (chon < 1 || chon > 3) std::cout << "Lua chon khong hop le! Nhap lai.\n";
        } while (chon < 1 || chon > 3);

        if (chon == 1) trangThai = "Hoat dong";
        else if (chon == 2) trangThai = "Het han";
        else trangThai = "Huy";

        std::cout << "-> Cap nhat hop dong thanh cong!\n";
    }

    void hienThiThongTin() const override {
        std::cout << "Ma HD: " << maDinhDanh
            << " | Ma KH: " << maKhachHang
            << " | Ma Goi: " << maGoiCuoc
            << " | Ma TB: " << maThietBi
            << " | Ngay ky: " << ngayKy
            << " | Ngay het han: " << ngayHetHan
            << " | Gia thieu dung: " << giaThieuDung << " VND"
            << " | Trang thai: " << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        std::stringstream ss;
        ss << maDinhDanh << "|" << maKhachHang << "|" << maGoiCuoc << "|"
           << maThietBi << "|" << ngayKy << "|" << ngayHetHan << "|"
           << giaThieuDung << "|" << trangThai;
        return ss.str();
    }

    void docTuChuoi(const std::string& dong) override {
        if (!kiemTraBanGhiNoiBo(dong)) {
            return; // Bo qua dong loi/trong am tham khong in roi man hinh
        }
        std::stringstream ss(dong);
        std::string giaStr;
        std::getline(ss, maDinhDanh, '|');
        std::getline(ss, maKhachHang, '|');
        std::getline(ss, maGoiCuoc, '|');
        std::getline(ss, maThietBi, '|');
        std::getline(ss, ngayKy, '|');
        std::getline(ss, ngayHetHan, '|');
        std::getline(ss, giaStr, '|');
        std::getline(ss, trangThai, '|');

        try {
            giaThieuDung = std::stod(giaStr);
        } catch (...) {
            giaThieuDung = 0.0;
        }
    }
};

#endif
