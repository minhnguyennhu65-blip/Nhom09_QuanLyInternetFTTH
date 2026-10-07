/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 01 - Quản lý Gói cước Internet
 * ======================================================= */

#ifndef UC01_GOI_INTERNET_H
#define UC01_GOI_INTERNET_H

#include "LopCoSo.h"
#include  "NhapDuLieu.h"
#include "QuanLyLuuTru.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// ============================================================================
// 1. DATA MODEL: GoiInternet (Kế thừa từ LopCoSo)
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

    // Getters
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
        std::cout << "--- Cap nhat thong tin cho ma goi: " << maDinhDanh << " ---\n";
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
// 2. SERVICE / CONTROLLER: QuanLyGoiInternet
// Cap nhut: Su dung QuanLyLuuTru<GoiInternet>
// ============================================================================
class QuanLyGoiInternet {
private:
    QuanLyLuuTru<GoiInternet> luuTru;

public:
    // Khởi tạo và tự động load file qua constructor của QuanLyLuuTru
    QuanLyGoiInternet(const std::string& duongDanFile = "data/goi_internet.txt") 
        : luuTru(duongDanFile) {}

    void themMoi() {
        GoiInternet goi;
        goi.nhapThongTin();
        luuTru.themMoi(goi); // QuanLyLuuTru tự kiểm tra trùng mã và tự lưu file
    }

    void hienThiDanhSach() const {
        std::cout << "\n============================== DANH SACH GOI INTERNET ==============================\n";
        std::cout << std::left 
                  << std::setw(12) << "Ma Goi" 
                  << std::setw(22) << "Ten Goi" 
                  << std::setw(12) << "Toc Do" 
                  << std::setw(16) << "Gia Thang" 
                  << std::setw(16) << "Trang Thai" 
                  << "Mo Ta" << std::endl;
        std::cout << std::string(90, '-') << std::endl;
        
        luuTru.hienThi(); // Gọi hàm hiển thị của QuanLyLuuTru
        
        std::cout << std::string(90, '-') << std::endl;
    }

    void capNhat() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can cap nhat: ");
        luuTru.capNhat(ma); // QuanLyLuuTru tự tìm đối tượng, gọi capNhatThongTin() và lưu file
    }

    void xoa() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can xoa: ");
        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa goi nay không?")) {
            luuTru.xoaTheoMa(ma); // QuanLyLuuTru tự xóa và lưu file
        }
    }

    void timKiem() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can tim: ");
        GoiInternet* goi = luuTru.timTheoMa(ma);
        if (goi != nullptr) {
            std::cout << "\n-> THONG TIN GOI INTERNET TIM THAY:\n";
            goi->hienThiThongTin();
        } else {
            std::cout << " -> Khong tim thay ma goi: " << ma << "\n";
        }
    }
};

#endif


  
              
      

 
    

