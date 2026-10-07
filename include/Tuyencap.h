/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 02 - Quản lý Tuyến Cáp
 * ======================================================= */

#ifndef UC02_TUYEN_CAP_H
#define UC02_TUYEN_CAP_H

#include "LopCoSo.h"
#include  "NhapDuLieu.h"
#include "QuanLyLuuTru.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

class TuyenCap : public LopCoSo {
private:
    std::string tenTuyen;
    std::string khuVuc;
    std::string diaDiemDau;
    std::string diaDiemCuoi;
    bool trangThai;

    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, last - first + 1);
    }

public:
    TuyenCap() : LopCoSo(), trangThai(true) {}

    TuyenCap(const std::string& ma, const std::string& ten, const std::string& kv, const std::string& dau, const std::string& cuoi, bool tt)
        : LopCoSo(ma), tenTuyen(ten), khuVuc(kv), diaDiemDau(dau), diaDiemCuoi(cuoi), trangThai(tt) {}

    std::string getTenTuyen() const { return tenTuyen; }
    std::string getKhuVuc() const { return khuVuc; }
    std::string getDiaDiemDau() const { return diaDiemDau; }
    std::string getDiaDiemCuoi() const { return diaDiemCuoi; }
    bool getTrangThai() const { return trangThai; }

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
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, '|')) {
            tokens.push_back(trim(token));
        }

        // 1. Kiểm tra đủ đúng 6 trường thông tin
        if (tokens.size() != 6) {
            throw std::invalid_argument("Du lieu tuyen cap khong du 6 truong thông tin");
        }

        std::string ma = tokens[0];
        std::string ten = tokens[1];
        std::string kv = tokens[2];
        std::string dau = tokens[3];
        std::string cuoi = tokens[4];
        std::string statusStr = tokens[5];

        // 6. Kiểm tra dữ liệu chuỗi bắt buộc
        if (ma.empty() || ten.empty()) {
            throw std::invalid_argument("Ma tuyen va Ten tuyen khong duoc de trong!");
        }

        // 5. Kiểm tra nghiêm ngặt trạng thái "0" / "1"
        if (statusStr != "0" && statusStr != "1") {
            throw std::invalid_argument("Trang thai phai la '0' hoac '1'");
        }

        setMaDinhDanh(ma);
        tenGoi = ten; // tenTuyen
        khuVuc = kv;
        diaDiemDau = dau;
        diaDiemCuoi = cuoi;
        trangThai = (statusStr == "1");
    }
};

class QuanLyTuyenCap {
private:
    QuanLyLuuTru<TuyenCap> luuTru;

    // 7. Kiểm tra khóa ngoại maTuyen
    bool kiemTraKhoaNgoai(const std::string& maTuyen) const {
        return false;
    }

public:
    QuanLyTuyenCap(const std::string& duongDanFile = "data/tuyen_cap.txt") 
        : luuTru(duongDanFile) {}

    void themMoi() {
        TuyenCap tuyen;
        tuyen.nhapThongTin();
        luuTru.themMoi(tuyen);
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
        luuTru.hienThi();
        std::cout << std::string(95, '-') << std::endl;
    }

    void capNhat() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can cap nhat: ");
        luuTru.capNhat(ma);
    }

    void xoa() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen can xoa: ");
        
        if (kiemTraKhoaNgoai(ma)) {
            std::cout << " -> Loi: Khong the xoa! Ma tuyen [" << ma 
                      << "] dang liên ket voi cac ha tang khac!\n";
            return;
        }

        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa tuyen cap nay khong?")) {
            luuTru.xoaTheoMa(ma);
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
