/* =========================================================
 * Tên tác giả: Đinh Văn Trường
 * Mã sinh viên: B24DCVT362
 * Use Case phụ trách: UC07 (Phiếu lắp đặt), UC08 (Phiếu sửa chữa)
 * Mô tả file: Lớp quản lý đối tượng Phiếu lắp đặt, kế thừa từ LopCoSo.
 * ========================================================= */

#ifndef PHIEU_LAP_DAT_H
#define PHIEU_LAP_DAT_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>

// UC07 - Phiếu lắp đặt (Đinh Văn Trường)
class PhieuLapDat : public LopCoSo {
private:
    std::string maHopDong;
    std::string maKhachHang;
    std::string maNhanVien;
    std::string ngayLapDat;
    std::string trangThai; // Cho xu ly / Dang lap / Hoan thanh

    // Hàm hỗ trợ kiểm tra khóa ngoại
    bool kiemTraKhoaNgoai(const std::string& filePath, const std::string& ma) const {
        std::ifstream file(filePath);
        if (!file.is_open()) return true;
        std::string dong, maInFile;
        while (std::getline(file, dong)) {
            if (dong.empty()) continue;
            std::stringstream ss(dong);
            std::getline(ss, maInFile, '|');
            if (maInFile == ma) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    // Hàm chuẩn hóa mã
    std::string chuanHoaMa(std::string ma) const {
        std::string res = "";
        for (char c : ma) {
            if (!isspace(c)) res += toupper(c);
        }
        return res;
    }

    // Hàm kiểm tra chuỗi hợp lệ
    bool chuoiHopLe(const std::string& str) const {
        return !str.empty() && str.find('|') == std::string::npos;
    }

public:
    PhieuLapDat() : LopCoSo() {
        maHopDong = "";
        maKhachHang = "";
        maNhanVien = "";
        ngayLapDat = "";
        trangThai = "Cho xu ly";
    }

    // Getter
    std::string getMaPhieuLapDat() const { return maDinhDanh; }
    std::string getMaHopDong() const { return maHopDong; }
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getMaNhanVien() const { return maNhanVien; }
    std::string getNgayLapDat() const { return ngayLapDat; }
    std::string getTrangThai() const { return trangThai; }

    // Setter
    void setMaHopDong(const std::string& maHD) { maHopDong = maHD; }
    void setMaKhachHang(const std::string& maKH) { maKhachHang = maKH; }
    void setMaNhanVien(const std::string& maNV) { maNhanVien = maNV; }
    void setNgayLapDat(const std::string& ngay) { ngayLapDat = ngay; }
    void setTrangThai(const std::string& tt) { trangThai = tt; }

    void nhapThongTin() override {
        do {
            std::cout << "Nhap ma phieu lap dat: ";
            std::string input;
            std::getline(std::cin >> std::ws, input);
            maDinhDanh = chuanHoaMa(input);
            if (!chuoiHopLe(maDinhDanh)) {
                std::cout << "-> LOI: Ma phieu khong duoc rong va khong chua ky tu '|'!\n";
            } else break;
        } while (true);

        do {
            maHopDong = NhapDuLieu::nhapChuoi("Nhap ma hop dong: ");
            if (!chuoiHopLe(maHopDong)) {
                std::cout << "-> LOI: Ma hop dong khong duoc rong va khong chua ky tu '|'!\n";
            } else if (!kiemTraKhoaNgoai("hop_dong.txt", maHopDong)) {
                std::cout << "-> CANH BAO: Ma hop dong [" << maHopDong << "] chua co trong hop_dong.txt!\n";
            } else break;
        } while (true);

        do {
            maKhachHang = NhapDuLieu::nhapChuoi("Nhap ma khach hang: ");
            if (!chuoiHopLe(maKhachHang)) {
                std::cout << "-> LOI: Ma khach hang khong duoc rong va khong chua ky tu '|'!\n";
            } else if (!kiemTraKhoaNgoai("khach_hang.txt", maKhachHang)) {
                std::cout << "-> CANH BAO: Ma khach hang [" << maKhachHang << "] chua co trong khach_hang.txt!\n";
            } else break;
        } while (true);

        do {
            maNhanVien = NhapDuLieu::nhapChuoi("Nhap ma nhan vien: ");
            if (!chuoiHopLe(maNhanVien)) {
                std::cout << "-> LOI: Ma nhan vien khong duoc rong va khong chua ky tu '|'!\n";
            } else if (!kiemTraKhoaNgoai("nhan_vien.txt", maNhanVien)) {
                std::cout << "-> CANH BAO: Ma nhan vien [" << maNhanVien << "] chua co trong nhan_vien.txt!\n";
            } else break;
        } while (true);

        do {
            ngayLapDat = NhapDuLieu::nhapChuoi("Nhap ngay lap dat (DD/MM/YYYY): ");
            if (ngayLapDat.length() == 10 && ngayLapDat[2] == '/' && ngayLapDat[5] == '/') {
                break;
            } else {
                std::cout << "-> LOI: Ngay phai dung dinh dang DD/MM/YYYY!\n";
            }
        } while (true);

        std::cout << "Chon trang thai phieu:\n 1. Cho xu ly\n 2. Dang lap\n 3. Hoan thanh\n";
        int chon = NhapDuLieu::nhapSoNguyen("Chon (1-3): ");
        if (chon == 2) trangThai = "Dang lap";
        else if (chon == 3) trangThai = "Hoan thanh";
        else trangThai = "Cho xu ly";
    }

    void hienThiThongTin() const override {
        std::cout << "Ma Phieu: " << maDinhDanh 
                  << " | Ma HD: " << maHopDong 
                  << " | Ma KH: " << maKhachHang 
                  << " | Ma NV: " << maNhanVien 
                  << " | Ngay Lap: " << ngayLapDat 
                  << " | Trang Thai: " << trangThai << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + maHopDong + "|" + maKhachHang + "|" + maNhanVien + "|" + ngayLapDat + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        std::stringstream ss(dong);
        std::getline(ss, maDinhDanh, '|');
        std::getline(ss, maHopDong, '|');
        std::getline(ss, maKhachHang, '|');
        std::getline(ss, maNhanVien, '|');
        std::getline(ss, ngayLapDat, '|');
        std::getline(ss, trangThai, '|');
    }
}; // Dấu kết thúc Class BẮT BUỘC nằm ở đây!

#endif // PHIEU_LAP_DAT_H
