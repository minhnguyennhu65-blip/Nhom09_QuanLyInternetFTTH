/* =========================================================
- Tên tác giả: Đinh Văn Trường
- Mã sinh viên: B24DCVT362
- Use Case phụ trách: UC08 (Phiếu sửa chữa)
- (Đã được cập nhật để đồng bộ và fix lỗi biên dịch)
- ========================================================= */

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

class PhieuSuaChua : public LopCoSo {
private:
    std::string maHopDong;
    std::string maKhachHang;
    std::string maNhanVien;
    std::string ngayBaoSua;
    std::string moTaLoi;
    std::string trangThai;

    // Chuẩn hóa mã: viết hoa và bỏ khoảng trắng
    std::string chuanHoaMa(std::string ma) const {
        std::string res;
        for (unsigned char c : ma) {
            if (!std::isspace(c)) {
                res += static_cast<char>(std::toupper(c));
            }
        }
        return res;
    }

    // Kiểm tra chuỗi không rỗng, không chứa dấu |
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

    // Kiểm tra ngày DD/MM/YYYY có tồn tại trên lịch
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

        if (y < 1 || m < 1 || m > 12) return false;

        int soNgay[] = { 31, 28, 31, 30, 31, 31, 30, 31, 30, 31, 30, 31 };
        bool namNhuan = (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
        if (namNhuan) soNgay[1] = 29;

        return d >= 1 && d <= soNgay[m - 1];
    }

    // Kiểm tra ngày không lớn hơn ngày hiện tại
    bool vuotQuaHomNay(const std::string& ngay) const {
        if (!ngayHopLe(ngay)) return true;

        int d = std::stoi(ngay.substr(0, 2));
        int m = std::stoi(ngay.substr(3, 2));
        int y = std::stoi(ngay.substr(6, 4));

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

    // Kiểm tra khóa ngoại (Tự động tạo file rỗng nếu chưa có)
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
            if (dong.empty() || dong[0] == '#') continue;
            std::stringstream ss(dong);
            if (std::getline(ss, maTrongFile, '|')) {
                if (chuanHoaMa(maTrongFile) == maCanTim) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }

    // Kiểm tra trùng mã phiếu sửa chữa
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

    std::string nhapMaThamChieu(const std::string& loiNhac, const std::string& filePath, const std::string& tenLoai) {
        while (true) {
            std::string ma = NhapDuLieu::nhapChuoi(loiNhac);
            ma = chuanHoaMa(ma);

            if (!chuoiHopLe(ma)) {
                std::cout << "Loi: Ma khong duoc rong hoac chua ky tu '|'.\n";
                continue;
            }

            if (!kiemTraKhoaNgoai(filePath, ma)) {
                std::cout << "Ma " << tenLoai << " [" << ma << "] khong ton tai trong " << filePath << ". Nhap lai.\n";
                continue;
            }

            return ma;
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
    void setMaHopDong(const std::string& ma) {
        if (chuoiHopLe(ma)) maHopDong = chuanHoaMa(ma);
    }
    void setMaKhachHang(const std::string& ma) {
        if (chuoiHopLe(ma)) maKhachHang = chuanHoaMa(ma);
    }
    void setMaNhanVien(const std::string& ma) {
        if (chuoiHopLe(ma)) maNhanVien = chuanHoaMa(ma);
    }
    void setNgayBaoSua(const std::string& ngay) {
        if (ngayBaoSuaHopLe(ngay)) ngayBaoSua = ngay;
    }
    void setMoTaLoi(const std::string& moTa) {
        if (chuoiHopLe(moTa)) moTaLoi = moTa;
    }
    void setTrangThai(const std::string& tt) {
        if (trangThaiHopLe(tt)) trangThai = tt;
    }

    // NHẬP THÔNG TIN
    void nhapThongTin() override {
        while (true) {
            std::cout << "Nhap ma phieu sua chua: ";
            std::string ma;
            std::getline(std::cin >> std::ws, ma);
            ma = chuanHoaMa(ma);

            if (!chuoiHopLe(ma)) {
                std::cout << "Loi: Ma phieu khong duoc rong hoac chua ky tu '|'.\n";
                continue;
            }
            if (kiemTraTrungMaPhieu("data/phieu_sua_chua.txt", ma)) {
                std::cout << "Loi: Ma phieu sua chua [" << ma << "] DA TON TAI!\n";
                continue;
            }
            maDinhDanh = ma;
            break;
        }

        maHopDong = nhapMaThamChieu("Nhap ma hop dong: ", "data/hop_dong.txt", "hop dong");
        maKhachHang = nhapMaThamChieu("Nhap ma khach hang: ", "data/khach_hang.txt", "khach hang");
        maNhanVien = nhapMaThamChieu("Nhap ma nhan vien: ", "data/nhan_vien.txt", "nhan vien");

        while (true) {
            std::string ngay = NhapDuLieu::nhapChuoi("Nhap ngay bao sua (DD/MM/YYYY): ");
            if (!ngayHopLe(ngay)) {
                std::cout << "Loi: Ngay khong ton tai tren lich.\n";
                continue;
            }
            if (vuotQuaHomNay(ngay)) {
                std::cout << "Loi: Ngay bao sua khong duoc lon hon ngay hien tai!\n";
                continue;
            }
            ngayBaoSua = ngay;
            break;
        }

        while (true) {
            std::string moTa = NhapDuLieu::nhapChuoi("Nhap mo ta loi: ");
            if (!chuoiHopLe(moTa)) {
                std::cout << "Loi: Mo ta khong duoc rong!\n";
                continue;
            }
            moTaLoi = moTa;
            break;
        }

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

    // CẬP NHẬT THÔNG TIN (Override bắt buộc từ LopCoSo)
    void capNhatThongTin() override {
        std::cout << "\n--- CAP NHAT THONG TIN PHIEU SUA CHUA ---\n";
        
        while (true) {
            std::string moTa = NhapDuLieu::nhapChuoi("Nhap mo ta loi moi: ");
            if (chuoiHopLe(moTa)) {
                moTaLoi = moTa;
                break;
            }
            std::cout << "Loi: Mo ta khong hop le!\n";
        }

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
            std::cout << "-> CANH BAO: Bo qua dong du lieu loi dinh dang: " << dong << "\n";
            return;
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
