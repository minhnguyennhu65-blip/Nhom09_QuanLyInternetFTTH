#ifndef HOP_DONG_H
#define HOP_DONG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class HopDong : public LopCoSo {
private:
    std::string maKhachHang;
    std::string maGoiCuoc;
    std::string maThietBi;
    std::string ngayKy;         // Format: DD/MM/YYYY
    std::string ngayHetHan;     // Format: DD/MM/YYYY
    std::string trangThai;      // Hoat dong / Tat / Huy
    double giaThieuDung;        // Gia tien thieu dung hang thang (>= 0)
    std::string ghiChu;

    // Enum trang thai hop le
    static const std::string TRANG_THAI_HOAT_DONG;
    static const std::string TRANG_THAI_TAT;
    static const std::string TRANG_THAI_HUY;

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == TRANG_THAI_HOAT_DONG || 
               tt == TRANG_THAI_TAT || 
               tt == TRANG_THAI_HUY;
    }

    // QUY TAC 1: Ma dinh danh - chi gom chu HOA va chu so
    static bool hopLeMa(const std::string& ma) {
        if (ma.empty()) {
            throw std::invalid_argument("[Loi QT1] Ma dinh danh khong duoc de trong!");
        }
        for (unsigned char ch : ma) {
            if (!(std::isupper(ch) || std::isdigit(ch))) {
                throw std::invalid_argument("[Loi QT1] Ma chi gom chu HOA (A-Z) va chu so (0-9)!");
            }
        }
        return true;
    }

    // QUY TAC 2: Chuoi - khong rong va khong chua '|'
    static bool hopLeChuoi(const std::string& s) {
        if (s.empty()) {
            throw std::invalid_argument("[Loi QT2] Thong tin khong duoc de trong!");
        }
        if (s.find('|') != std::string::npos) {
            throw std::invalid_argument("[Loi QT2] Khong duoc chua ky tu '|'!");
        }
        return true;
    }

    // QUY TAC 3: Gia tri so >= 0
    static bool hopLeSo(double v) {
        if (v < 0) {
            throw std::invalid_argument("[Loi QT3] Gia tri so phai >= 0!");
        }
        return true;
    }

    // QUY TAC 5: Ngay thang - DD/MM/YYYY va hop le trong thuc te
    static bool hopLeNgay(const std::string& ngay) {
        if (ngay.length() != 10) {
            throw std::invalid_argument("[Loi QT5] Ngay phai co dinh dang DD/MM/YYYY (10 ky tu)!");
        }
        if (ngay[2] != '/' || ngay[5] != '/') {
            throw std::invalid_argument("[Loi QT5] Ngay phai co dinh dang DD/MM/YYYY!");
        }

        // Kiem tra cac ky tu la chu so
        for (int i = 0; i < 10; i++) {
            if (i != 2 && i != 5) {
                if (!std::isdigit(static_cast<unsigned char>(ngay[i]))) {
                    throw std::invalid_argument("[Loi QT5] Ngay phai chi chua chu so va dau '/'!");
                }
            }
        }

        // Doc ngay, thang, nam
        int ngay_val = std::stoi(ngay.substr(0, 2));
        int thang_val = std::stoi(ngay.substr(3, 2));
        int nam_val = std::stoi(ngay.substr(6, 4));

        // Kiem tra nam hop le (1900-2100)
        if (nam_val < 1900 || nam_val > 2100) {
            throw std::invalid_argument("[Loi QT5] Nam phai trong khoang 1900-2100!");
        }

        // Kiem tra thang (1-12)
        if (thang_val < 1 || thang_val > 12) {
            throw std::invalid_argument("[Loi QT5] Thang phai trong khoang 01-12!");
        }

        // Kiem tra ngay theo thang
        int ngay_toi_da = 31;
        if (thang_val == 4 || thang_val == 6 || thang_val == 9 || thang_val == 11) {
            ngay_toi_da = 30;
        } else if (thang_val == 2) {
            // Kiem tra nam nhuan
            bool la_nam_nhuan = (nam_val % 4 == 0 && nam_val % 100 != 0) || (nam_val % 400 == 0);
            ngay_toi_da = la_nam_nhuan ? 29 : 28;
        }

        if (ngay_val < 1 || ngay_val > ngay_toi_da) {
            throw std::invalid_argument("[Loi QT5] Ngay phai trong khoang 01-" + std::to_string(ngay_toi_da) 
                + " cho thang " + std::to_string(thang_val) + "!");
        }

        return true;
    }

    // Chuyen so thuc thanh chuoi voi 2 chu so thap phan
    static std::string soThucThanhChuoi(double v) {
        std::ostringstream os;
        os << std::fixed << std::setprecision(2) << v;
        return os.str();
    }

    // Doc so thuc tu chuoi
    static double docSoThuc(const std::string& s) {
        if (s.empty()) {
            throw std::invalid_argument("[Loi doc so] Chuoi so thuc rong!");
        }
        try {
            std::size_t pos = 0;
            double v = std::stod(s, &pos);
            if (pos != s.size()) {
                throw std::invalid_argument("[Loi QT3] Gia tri so khong hop le: " + s);
            }
            hopLeSo(v);
            return v;
        } catch (const std::exception& e) {
            throw std::invalid_argument("[Loi QT3] Gia tri so khong hop le: " + s);
        }
    }

public:
    HopDong() : LopCoSo() {
        maKhachHang = "";
        maGoiCuoc = "";
        maThietBi = "";
        ngayKy = "";
        ngayHetHan = "";
        trangThai = TRANG_THAI_HOAT_DONG;
        giaThieuDung = 0.0;
        ghiChu = "";
    }

    // Getter
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getMaGoiCuoc() const { return maGoiCuoc; }
    std::string getMaThietBi() const { return maThietBi; }
    std::string getNgayKy() const { return ngayKy; }
    std::string getNgayHetHan() const { return ngayHetHan; }
    std::string getTrangThai() const { return trangThai; }
    double getGiaThieuDung() const { return giaThieuDung; }
    std::string getGhiChu() const { return ghiChu; }

    // Setter - Quy tac 1: Khoa cung ma dinh danh khi sua
    void setTrangThai(const std::string& tt) {
        if (!hopLeTrangThai(tt)) {
            throw std::invalid_argument("[Loi QT6] Trang thai hop dong phai la: " 
                + TRANG_THAI_HOAT_DONG + ", " + TRANG_THAI_TAT + " hoac " + TRANG_THAI_HUY);
        }
        trangThai = tt;
    }

    void nhapThongTin() override {
        // QUY TAC 1: Nhap ma hop dong (kiem tra trung sau trong he thong quan ly)
        while (true) {
            try {
                maDinhDanh = NhapDuLieu::nhapChuoi("Nhap ma hop dong (maHD, VIET HOA/chu so): ");
                hopLeMa(maDinhDanh);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 7: Nhap ma khach hang (kiem tra tham chieu sau)
        while (true) {
            try {
                maKhachHang = NhapDuLieu::nhapChuoi("Nhap ma khach hang: ");
                hopLeMa(maKhachHang);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 7: Nhap ma goi cuoc (kiem tra tham chieu sau)
        while (true) {
            try {
                maGoiCuoc = NhapDuLieu::nhapChuoi("Nhap ma goi cuoc: ");
                hopLeMa(maGoiCuoc);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 7: Nhap ma thiet bi (kiem tra tham chieu sau)
        while (true) {
            try {
                maThietBi = NhapDuLieu::nhapChuoi("Nhap ma thiet bi: ");
                hopLeMa(maThietBi);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 5: Nhap ngay ky
        while (true) {
            try {
                ngayKy = NhapDuLieu::nhapChuoi("Nhap ngay ky hop dong (DD/MM/YYYY): ");
                hopLeNgay(ngayKy);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 5: Nhap ngay het han
        while (true) {
            try {
                ngayHetHan = NhapDuLieu::nhapChuoi("Nhap ngay het han hop dong (DD/MM/YYYY): ");
                hopLeNgay(ngayHetHan);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 3: Nhap gia thieu dung (>= 0)
        while (true) {
            try {
                giaThieuDung = NhapDuLieu::nhapSoThuc("Nhap gia thieu dung hang thang (>= 0): ");
                hopLeSo(giaThieuDung);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // QUY TAC 6: Nhap trang thai (danh sach co dinh)
        while (true) {
            try {
                trangThai = NhapDuLieu::nhapChuoi(
                    "Nhap trang thai (" + TRANG_THAI_HOAT_DONG + "/" + TRANG_THAI_TAT + "/" + TRANG_THAI_HUY + "): ");
                hopLeTrangThai(trangThai);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << "\n";
            }
        }

        // Nhap ghi chu (co the rong, nhung neu co thi khong chua '|')
        ghiChu = NhapDuLieu::nhapChuoi("Nhap ghi chu (neu co, bao gom ca chuoi rong): ");
        if (ghiChu.find('|') != std::string::npos) {
            std::cout << "[Canh bao] Ghi chu chua ky tu '|', da xoa het ky tu nay.\n";
            ghiChu.erase(std::remove(ghiChu.begin(), ghiChu.end(), '|'), ghiChu.end());
        }
    }

    void hienThiThongTin() const override {
        std::cout << "Ma HD: " << maDinhDanh
                  << " | Ma KH: " << maKhachHang
                  << " | Ma Goi: " << maGoiCuoc
                  << " | Ma TB: " << maThietBi
                  << " | Ngay ky: " << ngayKy
                  << " | Ngay het han: " << ngayHetHan
                  << " | Gia thieu dung: " << giaThieuDung
                  << " | Trang thai: " << trangThai
                  << " | Ghi chu: " << ghiChu << "\n";
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + maKhachHang + "|" + maGoiCuoc + "|" +
               maThietBi + "|" + ngayKy + "|" + ngayHetHan + "|" +
               soThucThanhChuoi(giaThieuDung) + "|" + trangThai + "|" + ghiChu;
    }

    void docTuChuoi(const std::string& dong) override {
        if (std::count(dong.begin(), dong.end(), '|') != 8) {
            throw std::invalid_argument(
                "[Loi doc file] Dong du lieu khong dung so cot (HopDong). Can 8 dau '|'.");
        }

        std::stringstream ss(dong);
        std::string ma, makv, magoi, matb, nky, nhethan, gia, tt, ghu;
        std::getline(ss, ma, '|');
        std::getline(ss, makv, '|');
        std::getline(ss, magoi, '|');
        std::getline(ss, matb, '|');
        std::getline(ss, nky, '|');
        std::getline(ss, nhethan, '|');
        std::getline(ss, gia, '|');
        std::getline(ss, tt, '|');
        std::getline(ss, ghu, '|');

        // Validate tung truong
        try {
            hopLeMa(ma);
            hopLeMa(makv);
            hopLeMa(magoi);
            hopLeMa(matb);
            hopLeNgay(nky);
            hopLeNgay(nhethan);
            double tempGia = docSoThuc(gia);
            if (!hopLeTrangThai(tt)) {
                throw std::invalid_argument("[Loi QT6] Trang thai khong hop le: " + tt);
            }
            hopLeChuoi(ghu); // Neu ghi chu thi phai hop le
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument(std::string(e.what()) + " (dong: " + dong + ")");
        }

        // Chi gan khi toan bo du lieu hop le
        maDinhDanh = ma;
        maKhachHang = makv;
        maGoiCuoc = magoi;
        maThietBi = matb;
        ngayKy = nky;
        ngayHetHan = nhethan;
        giaThieuDung = docSoThuc(gia);
        trangThai = tt;
        ghiChu = ghu;
    }
};

// Dinh nghia cac hang so trang thai
const std::string HopDong::TRANG_THAI_HOAT_DONG = "Hoat dong";
const std::string HopDong::TRANG_THAI_TAT = "Tat";
const std::string HopDong::TRANG_THAI_HUY = "Huy";

#endif