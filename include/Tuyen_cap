/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 02 - Quản lý Tuyến Cáp
 * ======================================================= */

#ifndef UC02_TUYEN_CAP_H
#define UC02_TUYEN_CAP_H

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
        std::cout << "--- Cap nhat thông tin cho tuyen cap: " << maDinhDanh << " ---\n";
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
// 2. CLASS SERVICE QUẢN LÝ (CRUD & File IO)
// ============================================================================
class QuanLyTuyenCap {
private:
    std::vector<TuyenCap> danhSach;

public:
    int timKiemIndex(const std::string& ma) const {
        for (size_t i = 0; i < danhSach.size(); ++i) {
            if (danhSach[i].getMaDinhDanh() == ma) return i;
        }
        return -1;
    }

    void themMoi() {
        TuyenCap tuyen;
        tuyen.nhapThongTin();
        if (timKiemIndex(tuyen.getMaDinhDanh()) != -1) {
            std::cout << " -> Loi: Ma tuyen " << tuyen.getMaDinhDanh() << " da ton tai!\n";
            return;
        }
        danhSach.push_back(tuyen);
        std::cout << " -> Them tuyen cap thanh cong!\n";
    }

    void capNhat() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can cap nhat: ");
        int idx = timKiemIndex(ma);
        if (idx == -1) {
            std::cout << " -> Loi: Khong tim thay ma tuyen nay!\n";
            return;
        }
        danhSach[idx].capNhatThongTin();
        std::cout << " -> Cap nhat thong tin thanh cong!\n";
    }

    void xoa() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can xoa: ");
        int idx = timKiemIndex(ma);
        if (idx == -1) {
            std::cout << " -> Loi: Khong tim thay ma tuyen nay!\n";
            return;
        }
        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa tuyen cap nay không?")) {
            danhSach.erase(danhSach.begin() + idx);
            std::cout << " -> Da xoa tuyen cap thanh cong!\n";
        }
    }

    void hienThiDanhSach() const {
        if (danhSach.empty()) {
            std::cout << " -> Danh sach tuyen cap hien dang rong!\n";
            return;
        }
        std::cout << "\n================================= DANH SACH TUYEN CAP =================================\n";
        std::cout << std::left 
                  << std::setw(12) << "Ma Tuyen" 
                  << std::setw(20) << "Ten Tuyen" 
                  << std::setw(15) << "Khu Vuc" 
                  << std::setw(18) << "Diem Dau" 
                  << std::setw(18) << "Diem Cuoi" 
                  << "Trang Thai" << std::endl;
        std::cout << std::string(95, '-') << std::endl;
        for (const auto& tuyen : danhSach) {
            tuyen.hienThiThongTin();
        }
        std::cout << std::string(95, '-') << std::endl;
    }

    bool docTuFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) return false;

        danhSach.clear();
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            TuyenCap tuyen;
            tuyen.docTuChuoi(line);
            danhSach.push_back(tuyen);
        }
        file.close();
        return true;
    }

    bool ghiRaFile(const std::string& filePath) const {
        std::ofstream file(filePath);
        if (!file.is_open()) return false;

        for (const auto& tuyen : danhSach) {
            file << tuyen.chuyenThanhChuoi() << "\n";
        }
        file.close();
        return true;
    }
};

#endif
