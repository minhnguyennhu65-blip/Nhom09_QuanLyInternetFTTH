/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 01 - Quản lý Gói cước Internet
 * ======================================================= */

#ifndef UC01_GOI_INTERNET_H
#define UC01_GOI_INTERNET_H

#include "lop_co_so.h"
#include "nhap_du_lieu.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>

// ============================================================================
// 1. CLASS DATA MODEL (Kế thừa từ LopCoSo)
// ============================================================================
class GoiInternet : public LopCoSo {
private:
    std::string tenGoi;
    double tocDo;      // Mbps
    double giaThang;   // VNĐ
    std::string moTa;
    bool trangThai;    // true: Hoạt động, false: Ngừng

public:
    GoiInternet() : LopCoSo(), tocDo(0.0), giaThang(0.0), trangThai(true) {}

    GoiInternet(const std::string& ma, const std::string& ten, double td, double gia, const std::string& mt, bool tt)
        : LopCoSo(ma), tenGoi(ten), tocDo(td), giaThang(gia), moTa(mt), trangThai(tt) {}

    // Getters & Setters bổ sung
    std::string getTenGoi() const { return tenGoi; }
    double getTocDo() const { return tocDo; }
    double getGiaThang() const { return giaThang; }
    std::string getMoTa() const { return moTa; }
    bool getTrangThai() const { return trangThai; }

    // --- Thực thi các hàm thuần ảo từ LopCoSo ---

    void nhapThongTin() override {
        setMaDinhDanh(NhapDuLieu::nhapMaDinhDanh("Nhap ma goi internet (VD: NET100): "));
        tenGoi = NhapDuLieu::nhapChuoi("Nhap ten goi: ");
        tocDo = NhapDuLieu::nhapSoThucDuong("Nhap toc do (Mbps): ");
        giaThang = NhapDuLieu::nhapSoThucDuong("Nhap gia cuoc thang (VND): ");
        moTa = NhapDuLieu::nhapChuoi("Nhap mo ta chi tiet: ");
        trangThai = NhapDuLieu::xacNhan("Kich hoat goi ngay?");
    }

    void hienThiThongTin() const override {
        std::cout << std::left 
                  << std::setw(12) << maDinhDanh 
                  << std::setw(22) << tenGoi 
                  << std::setw(12) << (std::to_string((int)tocDo) + " Mbps")
                  << std::setw(16) << (std::to_string((long long)giaThang) + " VND")
                  << std::setw(16) << (trangThai ? "Hoat dong" : "Ngung hoat dong")
                  << moTa << std::endl;
    }

    void capNhatThongTin() override {
        std::cout << "--- Cap nhat thông tin cho ma goi: " << maDinhDanh << " ---\n";
        tenGoi = NhapDuLieu::nhapChuoi("Nhap ten goi moi: ");
        tocDo = NhapDuLieu::nhapSoThucDuong("Nhap toc do moi (Mbps): ");
        giaThang = NhapDuLieu::nhapSoThucDuong("Nhap gia cuoc moi (VND): ");
        moTa = NhapDuLieu::nhapChuoi("Nhap mo ta moi: ");
        trangThai = NhapDuLieu::xacNhan("Doi trang thai sang Hoat dong?");
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenGoi + "|" + std::to_string(tocDo) + "|" 
               + std::to_string((long long)giaThang) + "|" + moTa + "|" + (trangThai ? "1" : "0");
    }

    void docTuChuoi(const std::string& dongDuLieu) override {
        std::stringstream ss(dongDuLieu);
        std::string ma, ten, tocDoStr, giaStr, mt, statusStr;

        std::getline(ss, ma, '|');
        std::getline(ss, ten, '|');
        std::getline(ss, tocDoStr, '|');
        std::getline(ss, giaStr, '|');
        std::getline(ss, mt, '|');
        std::getline(ss, statusStr, '|');

        if (!ma.empty()) {
            setMaDinhDanh(ma);
            tenGoi = ten;
            tocDo = std::stod(tocDoStr);
            giaThang = std::stod(giaStr);
            moTa = mt;
            trangThai = (statusStr == "1");
        }
    }
};

// ============================================================================
// 2. CLASS SERVICE QUẢN LÝ (CRUD & File IO)
// ============================================================================
class QuanLyGoiInternet {
private:
    std::vector<GoiInternet> danhSach;

public:
    int timKiemIndex(const std::string& ma) const {
        for (size_t i = 0; i < danhSach.size(); ++i) {
            if (danhSach[i].getMaDinhDanh() == ma) return i;
        }
        return -1;
    }

    void themMoi() {
        GoiInternet goi;
        goi.nhapThongTin();
        if (timKiemIndex(goi.getMaDinhDanh()) != -1) {
            std::cout << " -> Loi: Ma goi " << goi.getMaDinhDanh() << " da ton tai!\n";
            return;
        }
        danhSach.push_back(goi);
        std::cout << " -> Them goi internet thanh cong!\n";
    }

    void capNhat() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can cap nhat: ");
        int idx = timKiemIndex(ma);
        if (idx == -1) {
            std::cout << " -> Loi: Khong tim thay ma goi nay!\n";
            return;
        }
        danhSach[idx].capNhatThongTin();
        std::cout << " -> Cap nhat thong tin thanh cong!\n";
    }

    void xoa() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can xoa: ");
        int idx = timKiemIndex(ma);
        if (idx == -1) {
            std::cout << " -> Loi: Khong tim thay ma goi nay!\n";
            return;
        }
        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa goi nay không?")) {
            danhSach.erase(danhSach.begin() + idx);
            std::cout << " -> Da xoa goi internet thanh cong!\n";
        }
    }

    void hienThiDanhSach() const {
        if (danhSach.empty()) {
            std::cout << " -> Danh sach goi internet hien dang rong!\n";
            return;
        }
        std::cout << "\n============================== DANH SACH GOI INTERNET ==============================\n";
        std::cout << std::left 
                  << std::setw(12) << "Ma Goi" 
                  << std::setw(22) << "Ten Goi" 
                  << std::setw(12) << "Toc Do" 
                  << std::setw(16) << "Gia Thang" 
                  << std::setw(16) << "Trang Thai" 
                  << "Mo Ta" << std::endl;
        std::cout << std::string(90, '-') << std::endl;
        for (const auto& goi : danhSach) {
            goi.hienThiThongTin();
        }
        std::cout << std::string(90, '-') << std::endl;
    }

    bool docTuFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) return false;

        danhSach.clear();
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            GoiInternet goi;
            goi.docTuChuoi(line);
            danhSach.push_back(goi);
        }
        file.close();
        return true;
    }

    bool ghiRaFile(const std::string& filePath) const {
        std::ofstream file(filePath);
        if (!file.is_open()) return false;

        for (const auto& goi : danhSach) {
            file << goi.chuyenThanhChuoi() << "\n";
        }
        file.close();
        return true;
    }
};

#endif
