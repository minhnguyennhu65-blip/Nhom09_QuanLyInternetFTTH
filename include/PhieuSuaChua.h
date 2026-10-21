/* =========================================================
 * Tên tác giả: Đinh Văn Trường
 * Mã sinh viên: B24DCVT362
 * Use Case phụ trách: UC08 (Phiếu sửa chữa)
 * Mô tả file: Lớp quản lý đối tượng Phiếu sửa chữa, kế thừa từ LopCoSo.
 * ========================================================= */

#ifndef PHIEU_SUA_CHUA_H
#define PHIEU_SUA_CHUA_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>
#include <ctime>
#include <stdexcept>

class PhieuSuaChua : public LopCoSo {
private:
    std::string maHopDong;
    std::string maKhachHang;
    std::string maNhanVien;
    std::string ngayBaoSua;
    std::string moTaLoi;
    std::string trangThai;

    // Chuan hoa ma: viet hoa va bo khoang trang
    std::string chuanHoaMa(std::string ma) const {
        std::string res;
        for (unsigned char c : ma) {
            if (!std::isspace(c)) {
                res += static_cast<char>(std::toupper(c));
            }
        }
        return res;
    }

    // Kiem tra chuoi khong rong, khong chi co khoang trang va khong chua dau |
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

    // Kiem tra ngay DD/MM/YYYY co ton tai tren lich
    bool ngayHopLe(const std::string& ngay) const {
        if (ngay.length() != 10 ||
            ngay[2] != '/' || ngay[5] != '/') {
            return false;
        }

        for (int i = 0; i < 10; ++i) {
            if (i == 2 || i == 5) continue;

            if (!std::isdigit(
                static_cast<unsigned char>(ngay[i]))) {
                return false;
            }
        }

        int d = (ngay[0] - '0') * 10 + (ngay[1] - '0');
        int m = (ngay[3] - '0') * 10 + (ngay[4] - '0');
        int y = 0;

        for (int i = 6; i < 10; ++i) {
            y = y * 10 + (ngay[i] - '0');
        }

        if (y < 1 || m < 1 || m > 12) return false;

        int soNgay[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        bool namNhuan =
            (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);

        if (namNhuan) soNgay[1] = 29;

        return d >= 1 && d <= soNgay[m - 1];
    }

    // Kiem tra ngay co lon hon ngay hien tai khong
    bool vuotQuaHomNay(const std::string& ngay) const {
        int d = (ngay[0] - '0') * 10 + (ngay[1] - '0');
        int m = (ngay[3] - '0') * 10 + (ngay[4] - '0');
        int y = 0;
        for (int i = 6; i < 10; ++i) {
            y = y * 10 + (ngay[i] - '0');
        }

        std::time_t now = std::time(nullptr);
        std::tm* today = std::localtime(&now);
        if (today == nullptr) return false;

        int todayD = today->tm_mday;
        int todayM = today->tm_mon + 1;
        int todayY = today->tm_year + 1900;

        if (y != todayY) return y > todayY;
        if (m != todayM) return m > todayM;
        return d > todayD;
    }

    bool ngayBaoSuaHopLe(const std::string& ngay) const {
        return ngayHopLe(ngay) && !vuotQuaHomNay(ngay);
    }

    bool trangThaiHopLe(const std::string& tt) const {
        return tt == "Tiep nhan" || tt == "Dang xu ly" || tt == "Hoan thanh";
    }

    // Kiem tra khoa ngoai
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

    // Kiem tra trung ma phieu khóa chính
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

    // Hàm nhập mã tham chiếu cho phép bấm 0 để hủy
    std::string nhapMaThamChieu(const std::string& loiNhac, const std::string& filePath, const std::string& tenLoai, bool& daHuy) {
        while (true) {
            std::string ma = NhapDuLieu::nhapChuoi(loiNhac);
            std::string maChuanHoa = chuanHoaMa(ma);

            if (maChuanHoa == "0") {
                daHuy = true;
                return "";
            }

            if (!chuoiHopLe(maChuanHoa)) {
                std::cout << "Loi: Ma khong duoc rong hoac chua ky tu '|'.\n";
                continue;
            }

            if (!kiemTraKhoaNgoai(filePath, maChuanHoa)) {
                std::cout << "Ma " << tenLoai << " [" << maChuanHoa << "] khong ton tai trong " << filePath << ". Nhap lai hoac nhap 0 de huy.\n";
                continue;
            }

            daHuy = false;
            return maChuanHoa;
        }
    }

public:
    PhieuSuaChua() : LopCoSo() {
        maHopDong = "";
        maKhachHang = "";
        maNhanVien = "";
        ngayBaoSua = "";
        moTaLoi = "";
        trangThai = "Tiep nhan";
    }

    // GETTER
    std::string getMaPhieuSuaChua() const { return maDinhDanh; }
    std::string getMaHopDong() const { return maHopDong; }
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getMaNhanVien() const { return maNhanVien; }
    std::string getNgayBaoSua() const { return ngayBaoSua; }
    std::string getMoTaLoi() const { return moTaLoi; }
    std::string getTrangThai() const { return trangThai; }

    // SETTER
    bool setMaHopDong(const std::string& ma) {
        if (!chuoiHopLe(ma) || !kiemTraKhoaNgoai("data/hop_dong.txt", ma)) return false;
        maHopDong = chuanHoaMa(ma);
        return true;
    }

    bool setMaKhachHang(const std::string& ma) {
        if (!chuoiHopLe(ma) || !kiemTraKhoaNgoai("data/khach_hang.txt", ma)) return false;
        maKhachHang = chuanHoaMa(ma);
        return true;
    }

    bool setMaNhanVien(const std::string& ma) {
        if (!chuoiHopLe(ma) || !kiemTraKhoaNgoai("data/nhan_vien.txt", ma)) return false;
        maNhanVien = chuanHoaMa(ma);
        return true;
    }

    bool setNgayBaoSua(const std::string& ngay) {
        if (!ngayBaoSuaHopLe(ngay)) return false;
        ngayBaoSua = ngay;
        return true;
    }

    bool setMoTaLoi(const std::string& moTa) {
        if (!chuoiHopLe(moTa)) return false;
        moTaLoi = moTa;
        return true;
    }

    bool setTrangThai(const std::string& tt) {
        if (!trangThaiHopLe(tt)) return false;
        trangThai = tt;
        return true;
    }

    // NHẬP THÔNG TIN CÓ HỖ TRỢ HỦY BẰNG PHÍM 0
    void nhapThongTin() override {
        // 1. Nhập mã phiếu sửa chữa
        while (true) {
            std::cout << "Nhap ma phieu sua chua (nhap 0 de huy): ";
            std::string ma;
            std::getline(std::cin >> std::ws, ma);
            ma = chuanHoaMa(ma);

            if (ma == "0") {
                std::cout << " -> Da huy thao tac them phieu sua chua.\n";
                maDinhDanh = ""; // Xóa mã định danh để báo hiệu cho main.cpp bỏ qua
                return;
            }

            if (!chuoiHopLe(ma)) {
                std::cout << "Loi: Ma phieu khong duoc rong hoac chua ky tu '|'.\n";
                continue;
            }
            if (kiemTraTrungMaPhieu("data/phieu_sua_chua.txt", ma)) {
                std::cout << "Loi: Ma phieu sua chua [" << ma << "] DA TON TAI! Vui long nhap ma khac.\n";
                continue;
            }
            maDinhDanh = ma;
            break;
        }

        // 2, 3, 4. Nhập các mã tham chiếu khóa ngoại
        bool daHuy = false;

        maHopDong = nhapMaThamChieu("Nhap ma hop dong (nhap 0 de huy): ", "data/hop_dong.txt", "hop dong", daHuy);
        if (daHuy) {
            std::cout << " -> Da huy thao tac them phieu sua chua.\n";
            maDinhDanh = "";
            return;
        }

        maKhachHang = nhapMaThamChieu("Nhap ma khach hang (nhap 0 de huy): ", "data/khach_hang.txt", "khach hang", daHuy);
        if (daHuy) {
            std::cout << " -> Da huy thao tac them phieu sua chua.\n";
            maDinhDanh = "";
            return;
        }

        maNhanVien = nhapMaThamChieu("Nhap ma nhan vien (nhap 0 de huy): ", "data/nhan_vien.txt", "nhan vien", daHuy);
        if (daHuy) {
            std::cout << " -> Da huy thao tac them phieu sua chua.\n";
            maDinhDanh = "";
            return;
        }

        // 5. Ngày báo sửa
        while (true) {
            std::string ngay = NhapDuLieu::nhapChuoi("Nhap ngay bao sua (DD/MM/YYYY): ");
            if (!ngayHopLe(ngay)) {
                std::cout << "Loi: Ngay khong ton tai hoac sai dinh dang.\n";
                continue;
            }
            if (vuotQuaHomNay(ngay)) {
                std::cout << "Loi: Ngay bao sua khong duoc lon hon ngay hien tai!\n";
                continue;
            }
            ngayBaoSua = ngay;
            break;
        }

        // 6. Mô tả lỗi
        while (true) {
            std::string moTa = NhapDuLieu::nhapChuoi("Nhap mo ta loi: ");
            if (!chuoiHopLe(moTa)) {
                std::cout << "Loi: Mo ta khong duoc rong hoac chua ky tu '|'.\n";
                continue;
            }
            moTaLoi = moTa;
            break;
        }

        // 7. Trạng thái
        int chon;
        do {
            std::cout << "\nChon trang thai:\n 1. Tiep nhan\n 2. Dang xu ly\n 3. Hoan thanh\n";
            chon = NhapDuLieu::nhapSoNguyenDuong("Chon (1-3): ");
            if (chon < 1 || chon > 3) {
                std::cout << "Loi: Chi duoc chon tu 1 den 3!\n";
            }
        } while (chon < 1 || chon > 3);

        if (chon == 1) trangThai = "Tiep nhan";
        else if (chon == 2) trangThai = "Dang xu ly";
        else trangThai = "Hoan thanh";
    }

    void capNhatThongTin() override {
        std::cout << "\n--- CAP NHAT THONG TIN PHIEU SUA CHUA ---\n";

        // Cập nhật mô tả lỗi
        while (true) {
            std::string moTa = NhapDuLieu::nhapChuoi("Nhap mo ta loi moi: ");
            if (chuoiHopLe(moTa)) {
                moTaLoi = moTa;
                break;
            }
            std::cout << "Loi: Mo ta khong duoc rong va khong duoc chua ky tu '|'.\n";
        }

        // Cập nhật trạng thái
        int chon;
        do {
            std::cout << "Chon trang thai moi:\n 1. Tiep nhan\n 2. Dang xu ly\n 3. Hoan thanh\n";
            chon = NhapDuLieu::nhapSoNguyenDuong("Chon (1-3): ");
            if (chon < 1 || chon > 3) {
                std::cout << "Loi: Chi duoc chon tu 1 den 3!\n";
            }
        } while (chon < 1 || chon > 3);

        if (chon == 1) trangThai = "Tiep nhan";
        else if (chon == 2) trangThai = "Dang xu ly";
        else trangThai = "Hoan thanh";

        std::cout << "-> Cap nhat phieu sua chua thanh cong!\n";
    }

    void hienThiThongTin() const override {
        std::cout << "Ma phieu SC: " << maDinhDanh
            << " | Ma HD: " << maHopDong
            << " | Ma KH: " << maKhachHang
            << " | Ma NV: " << maNhanVien
            << " | Ngay bao: " << ngayBaoSua
            << " | Mo ta loi: " << moTaLoi
            << " | Trang thai: " << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + maHopDong + "|" + maKhachHang + "|" + maNhanVien + "|" + ngayBaoSua + "|" + moTaLoi + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        std::vector<std::string> f = tachBanGhi(dong);

        if (f.size() != 7 || !chuoiHopLe(f[0]) || !chuoiHopLe(f[1]) || !chuoiHopLe(f[2]) ||
            !chuoiHopLe(f[3]) || !ngayBaoSuaHopLe(f[4]) || !chuoiHopLe(f[5]) || !trangThaiHopLe(f[6])) {
            return; // Bỏ qua dòng lỗi âm thầm
        }

        maDinhDanh = chuanHoaMa(f[0]);
        maHopDong = chuanHoaMa(f[1]);
        maKhachHang = chuanHoaMa(f[2]);
        maNhanVien = chuanHoaMa(f[3]);
        ngayBaoSua = f[4];
        moTaLoi = f[5];
        trangThai = f[6];
    }
};

#endif // PHIEU_SUA_CHUA_H
