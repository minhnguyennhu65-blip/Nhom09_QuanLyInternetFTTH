/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp quản lý Cổng OLT (Entity - UC06 - Single Header)
 * ======================================================= */

#ifndef CONG_OLT_H
#define CONG_OLT_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include "QuanLyLuuTru.h"
#include "KhachHang.h"

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <limits>

class CongOLT : public LopCoSo {
private:
    std::string tenOLT;
    int soCong;
    std::string khuVuc;
    std::string maKhachHang;
    std::string trangThai;

    // Dùng inline static để khai báo và khởi tạo trực tiếp ngay trong file .h mà không sợ lỗi Multiple Definition
    inline static QuanLyLuuTru<KhachHang>* dbKhachHang = nullptr;

    bool hopLeTrangThai(const std::string& tt) const {
        return tt == "Trong" ||
               tt == "Dang su dung" ||
               tt == "Hong";
    }

    // Yêu cầu: Trả về false nếu chưa kết nối DB để ép buộc kiểm tra khóa ngoại nghiêm ngặt
    bool khachHangTonTai(const std::string& ma) const {
        if (dbKhachHang == nullptr)
            return false;

        return dbKhachHang->timTheoMa(ma) != nullptr;
    }

    void nhapTrangThai() {
        int chon;
        do {
            std::cout << "\nTrang thai cong:\n";
            std::cout << "1. Trong\n";
            std::cout << "2. Dang su dung\n";
            std::cout << "3. Hong\n";

            chon = NhapDuLieu::nhapSoNguyenDuong("Chon trang thai: ");

            if (chon < 1 || chon > 3)
                std::cout << "Lua chon khong hop le!\n";
        } while (chon < 1 || chon > 3);

        if (chon == 1) {
            trangThai = "Trong";
            maKhachHang = "";
        } else if (chon == 2) {
            trangThai = "Dang su dung";
            do {
                maKhachHang = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang: ");
                if (!khachHangTonTai(maKhachHang))
                    std::cout << "Khach hang khong ton tai!\n";
            } while (!khachHangTonTai(maKhachHang));
        } else {
            trangThai = "Hong";
            maKhachHang = "";
        }
    }

public:
    CongOLT() : LopCoSo(), soCong(1), trangThai("Trong") {}

    static void setDBKhachHang(QuanLyLuuTru<KhachHang>* db) {
        dbKhachHang = db;
    }

    void nhapThongTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma cong: ");
        tenOLT = NhapDuLieu::nhapChuoi("Nhap ten OLT: ");

        do {
            soCong = NhapDuLieu::nhapSoNguyenDuong("Nhap so cong: ");
            if (soCong < 1) std::cout << "So cong phai lon hon 0!\n";
        } while (soCong < 1);

        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc: ");
        nhapTrangThai();
    }

    void capNhatThongTin() override {
        tenOLT = NhapDuLieu::nhapChuoi("Nhap ten OLT moi: ");

        do {
            soCong = NhapDuLieu::nhapSoNguyenDuong("Nhap so cong moi: ");
            if (soCong < 1) std::cout << "So cong phai lon hon 0!\n";
        } while (soCong < 1);

        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc moi: ");
        nhapTrangThai();
    }

    void hienThiThongTin() const override {
        std::cout << "Ma cong: " << maDinhDanh
                  << " | OLT: " << tenOLT
                  << " | So cong: " << soCong
                  << " | Khu vuc: " << khuVuc
                  << " | Ma khach hang: "
                  << (maKhachHang.empty() ? "Khong co" : maKhachHang)
                  << " | Trang thai: " << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenOLT + "|" +
               std::to_string(soCong) + "|" + khuVuc + "|" +
               maKhachHang + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dongDuLieu) override {
        std::istringstream dong(dongDuLieu);
        std::string ma, ten, so, kv, maKH, tt, thua;

        if (!std::getline(dong, ma, '|') ||
            !std::getline(dong, ten, '|') ||
            !std::getline(dong, so, '|') ||
            !std::getline(dong, kv, '|') ||
            !std::getline(dong, maKH, '|') ||
            !std::getline(dong, tt) ||
            std::getline(dong, thua, '|')) {
            throw std::runtime_error("Du lieu khong hop le!");
        }

        std::istringstream kiemTraSo(so);
        int soMoi;

        if (!(kiemTraSo >> soMoi) || soMoi < 1 ||
            (kiemTraSo >> std::ws, !kiemTraSo.eof())) {
            throw std::runtime_error("So cong khong hop le!");
        }

        if (ma.empty() || ten.empty() || kv.empty() || !hopLeTrangThai(tt)) {
            throw std::runtime_error("Thong tin khong hop le!");
        }

        if (tt == "Dang su dung") {
            if (maKH.empty() || !khachHangTonTai(maKH))
                throw std::runtime_error("Ma khach hang khong ton tai!");
        } else if (!maKH.empty()) {
            throw std::runtime_error("Cong nay khong duoc gan khach hang!");
        }

        maDinhDanh = ma;
        tenOLT = ten;
        soCong = soMoi;
        khuVuc = kv;
        maKhachHang = maKH;
        trangThai = tt;
    }

    std::string getTenOLT() const { return tenOLT; }
    int getSoCong() const { return soCong; }
    std::string getKhuVuc() const { return khuVuc; }
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getTrangThai() const { return trangThai; }
};

#endif
