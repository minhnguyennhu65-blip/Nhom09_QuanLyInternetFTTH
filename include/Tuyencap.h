/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 02 - Quản lý Tuyến Cáp
 * ======================================================= */

#ifndef UC02_TUYEN_CAP_H
#define UC02_TUYEN_CAP_H

#include "LopCoSo.h"
#include "nhap_du_lieu.h"
#include "QuanLyLuuTru.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// ============================================================================
// 1. DATA MODEL: TuyenCap (Kế thừa từ LopCoSo)
// ============================================================================
class TuyenCap : public LopCoSo {
private:
    std::string tenTuyen;
    std::string khuVuc;
    std::string diaDiemDau;
    std::string diaDiemCuoi;
    bool trangThai; // true: Hoạt động, false: Bảo trì

public:
    TuyenCap() : LopCoSo(), trangThai(true) {}

    TuyenCap(const std::string& ma, const std::string& ten, const std::string& kv, const std::string& dau, const std::string& cuoi, bool tt)
        : LopCoSo(ma), tenTuyen(ten), khuVuc(kv), diaDiemDau(dau), diaDiemCuoi(cuoi), trangThai(tt) {}

    // Getters
    std::string getTenTuyen() const { return tenTuyen; }
    std::string getKhuVuc() const { return khuVuc; }
    std::string getDiaDiemDau() const { return diaDiemDau; }
    std::string getDiaDiemCuoi() const { return diaDiemCuoi; }
    bool getTrangThai() const { return trangThai; }

    // --- Thực thi các hàm thuần ảo từ LopCoSo ---

    void nhapThongTin() override {
        setMaDinhDanh(NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen cap (VD: TC01): "));
        tenTuyen = NhapDuLieu::nhapChuoi("Nhap ten tuyen cap: ");
        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc quan ly: ");
        diaDiemDau = NhapDuLieu::nhapChuoi("Nhap dia diem dau: ");
        diaDiemCuoi = NhapDuLieu::nhapChuoi("Nhap dia diem cuoi: ");
        trangThai = NhapDuLieu::xacNhan("Tuyen cap dang hoat dong binh thuong?");
    }

    void hienThiThongTin() const override {
        std::cout << std::left 
                  << std::setw(12) << maDinhDanh 
                  << std::setw(20) << tenTuyen 
                  << std::setw(15) << khuVuc 
                  << std::setw(18) << diaDiemDau 
                  << std::setw(18) << diaDiemCuoi 
                  << (trangThai ? "Hoat dong" : "Bao tri") << std::endl;
    }

    void capNhatThongTin() override {
        std::cout << "--- Cap nhat thong tin cho tuyen cap: " << maDinhDanh << " ---\n";
        tenTuyen = NhapDuLieu::nhapChuoi("Nhap ten tuyen moi: ");
        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc moi: ");
        diaDiemDau = NhapDuLieu::nhapChuoi("Nhap dia diem dau moi: ");
        diaDiemCuoi = NhapDuLieu::nhapChuoi("Nhap dia diem cuoi moi: ");
        trangThai = NhapDuLieu::xacNhan("Xac nhan trang thai Hoat dong?");
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenTuyen + "|" + khuVuc + "|" 
               + diaDiemDau + "|" + diaDiemCuoi + "|" + (trangThai ? "1" : "0");
    }

    void docTuChuoi(const std::string& dongDuLieu) override {
        std::stringstream ss(dongDuLieu);
        std::string ma, ten, kv, dau, cuoi, statusStr;

        std::getline(ss, ma, '|');
        std::getline(ss, ten, '|');
        std::getline(ss, kv, '|');
        std::getline(ss, dau, '|');
        std::getline(ss, cuoi, '|');
        std::getline(ss, statusStr, '|');

        if (!ma.empty()) {
            setMaDinhDanh(ma);
            tenTuyen = ten;
            khuVuc = kv;
            diaDiemDau = dau;
            diaDiemCuoi = cuoi;
            trangThai = (statusStr == "1");
        }
    }
};

// ============================================================================
// 2. SERVICE / CONTROLLER: QuanLyTuyenCap
// Cập nhật: Sử dụng QuanLyLuuTru<TuyenCap>
// ============================================================================
class QuanLyTuyenCap {
private:
    QuanLyLuuTru<TuyenCap> luuTru;

public:
    // Khởi tạo và tự động load file qua constructor của QuanLyLuuTru
    QuanLyTuyenCap(const std::string& duongDanFile = "data/tuyen_cap.txt") 
        : luuTru(duongDanFile) {}

    void themMoi() {
        TuyenCap tuyen;
        tuyen.nhapThongTin();
        luuTru.themMoi(tuyen); // QuanLyLuuTru tự kiểm tra trùng mã và tự lưu file
    }

    void hienThiDanhSach() const {
        std::cout << "\n================================= DANH SACH TUYEN CAP =================================\n";
        std::cout << std::left 
                  << std::setw(12) << "Ma Tuyen" 
                  << std::setw(20) << "Ten Tuyen" 
                  << std::setw(15) << "Khu Vuc" 
                  << std::setw(18) << "Diem Dau" 
                  << std::setw(18) << "Diem Cuoi" 
                  << "Trang Thai" << std::endl;
        std::cout << std::string(95, '-') << std::endl;
        
        luuTru.hienThi(); // Gọi hàm hiển thị của QuanLyLuuTru
        
        std::cout << std::string(95, '-') << std::endl;
    }

    void capNhat() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can cap nhat: ");
        luuTru.capNhat(ma); // QuanLyLuuTru tự tìm đối tượng, gọi capNhatThongTin() và lưu file
    }

    void xoa() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can xoa: ");
        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa tuyen cap nay không?")) {
            luuTru.xoaTheoMa(ma); // QuanLyLuuTru tự xóa và lưu file
        }
    }

    void timKiem() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can tim: ");
        TuyenCap* tuyen = luuTru.timTheoMa(ma);
        if (tuyen != nullptr) {
            std::cout << "\n-> THONG TIN TUYEN CAP TIM THAY:\n";
            tuyen->hienThiThongTin();
        } else {
            std::cout << " -> Khong tim thay ma tuyen: " << ma << "\n";
        }
    }
};

#endif
