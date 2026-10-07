/* =========================================================
 * Tên tác giả: Đinh Văn Trường
 * Mã sinh viên: B24DCVT362
 * Use Case phụ trách: UC08 (Phiếu sửa chữa / bảo trì)
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
#include <algorithm>
#include <cctype>

// UC08 - Phiếu sửa chữa / bảo trì 
class PhieuSuaChua : public LopCoSo {
private:
    std::string maHopDong;
    std::string maKhachHang;
    std::string maNhanVien;
    std::string ngayBaoSua;
    std::string moTaLoi;
    std::string trangThai; // Tiep nhan / Dang xu ly / Hoan thanh

    // --- HÀM BỔ SUNG NỘI BỘ (PRIVATE) ---
    // Kiểm tra khóa ngoại có trong file dữ liệu gốc không 
    bool kiemTraKhoaNgoai(const std::string& filePath, const std::string& ma) const {
        std::ifstream file(filePath);
        if (!file.is_open()) return true; // File gốc chưa có thì tạm cho qua
        
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

    // Chuyển mã thành viết HOA & xóa khoảng trắng 
    std::string chuanHoaMa(std::string ma) const {
        std::string res = "";
        for (char c : ma) {
            if (!isspace(c)) res += toupper(c);
        }
        return res;
    }

    // Kiểm tra chuỗi không rỗng và không chứa '|' 
    bool chuoiHopLe(const std::string& str) const {
        return !str.empty() && str.find('|') == std::string::npos;
    }

public:
    // Constructor mặc định
    PhieuSuaChua() : LopCoSo() {
        maHopDong = "";
        maKhachHang = "";
        maNhanVien = "";
        ngayBaoSua = "";
        moTaLoi = "";
        trangThai = "Tiep nhan";
    }

    // --- GETTER ---
    std::string getMaPhieuSuaChua() const { return maDinhDanh; }
    std::string getMaHopDong() const { return maHopDong; }
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getMaNhanVien() const { return maNhanVien; }
    std::string getNgayBaoSua() const { return ngayBaoSua; }
    std::string getMoTaLoi() const { return moTaLoi; }
    std::string getTrangThai() const { return trangThai; }

    // --- SETTER ---
    void setMaHopDong(const std::string& maHD) { maHopDong = maHD; }
    void setMaKhachHang(const std::string& maKH) { maKhachHang = maKH; }
    void setMaNhanVien(const std::string& maNV) { maNhanVien = maNV; }
    void setNgayBaoSua(const std::string& ngay) { ngayBaoSua = ngay; }
    void setMoTaLoi(const std::string& moTa) { moTaLoi = moTa; }
    void setTrangThai(const std::string& tt) { trangThai = tt; }

    // Ghi đè hàm nhập thông tin từ bàn phím
    void nhapThongTin() override {
        // 1. Nhập Mã phiếu sửa chữa 
        do {
            std::cout << "Nhap ma phieu sua chua: ";
            std::string input;
            std::getline(std::cin >> std::ws, input);
            maDinhDanh = chuanHoaMa(input);

            if (!chuoiHopLe(maDinhDanh)) {
                std::cout << "-> LOI: Ma phieu khong duoc rong va khong chua ky tu '|'!\n";
            } else break;
        } while (true);

        // 2. Nhập Mã hợp đồng (Check hop_dong.txt)
        do {
            maHopDong = NhapDuLieu::nhapChuoi("Nhap ma hop dong: ");
            if (!chuoiHopLe(maHopDong)) {
                std::cout << "-> LOI: Ma hop dong khong duoc rong va khong chua ky tu '|'!\n";
            } else if (!kiemTraKhoaNgoai("hop_dong.txt", maHopDong)) {
                std::cout << "-> CANH BAO: Ma hop dong [" << maHopDong << "] chua co trong hop_dong.txt! Vui long nhap lai.\n";
            } else break;
        } while (true);

        // 3. Nhập Mã khách hàng (Check khach_hang.txt)
        do {
            maKhachHang = NhapDuLieu::nhapChuoi("Nhap ma khach hang: ");
            if (!chuoiHopLe(maKhachHang)) {
                std::cout << "-> LOI: Ma khach hang khong duoc rong va khong chua ky tu '|'!\n";
            } else if (!kiemTraKhoaNgoai("khach_hang.txt", maKhachHang)) {
                std::cout << "-> CANH BAO: Ma khach hang [" << maKhachHang << "] chua co trong khach_hang.txt! Vui long nhap lai.\n";
            } else break;
        } while (true);

        // 4. Nhập Mã nhân viên (Check nhan_vien.txt)
        do {
            maNhanVien = NhapDuLieu::nhapChuoi("Nhap ma nhan vien: ");
            if (!chuoiHopLe(maNhanVien)) {
                std::cout << "-> LOI: Ma nhan vien khong duoc rong va khong chua ky tu '|'!\n";
            } else if (!kiemTraKhoaNgoai("nhan_vien.txt", maNhanVien)) {
                std::cout << "-> CANH BAO: Ma nhan vien [" << maNhanVien << "] chua co trong nhan_vien.txt! Vui long nhap lai.\n";
            } else break;
        } while (true);

        // 5. Nhập Ngày báo sửa (DD/MM/YYYY)
        do {
            ngayBaoSua = NhapDuLieu::nhapChuoi("Nhap ngay bao sua (DD/MM/YYYY): ");
            if (ngayBaoSua.length() == 10 && ngayBaoSua[2] == '/' && ngayBaoSua[5] == '/') {
                break;
            } else {
                std::cout << "-> LOI: Ngay phai dung dinh dang DD/MM/YYYY (Vi du: 15/03/2026)!\n";
            }
        } while (true);

        // 6. Nhập Mô tả lỗi (Không chứa '|')
        do {
            moTaLoi = NhapDuLieu::nhapChuoi("Nhap mo ta loi su co: ");
            if (!chuoiHopLe(moTaLoi)) {
                std::cout << "-> LOI: Mo ta loi khong duoc rong va khong chua ky tu '|'!\n";
            } else break;
        } while (true);

        // 7. Nhập Trạng thái (Menu chọn)
        std::cout << "Chon trang thai phieu sua chua:\n 1. Tiep nhan\n 2. Dang xu ly\n 3. Hoan thanh\n";
        int chon = NhapDuLieu::nhapSoNguyen("Chon (1-3): ");
        if (chon == 2) trangThai = "Dang xu ly";
        else if (chon == 3) trangThai = "Hoan thanh";
        else trangThai = "Tiep nhan";
    }

    // Ghi đè hàm hiển thị thông tin ra màn hình
    void hienThiThongTin() const override {
        std::cout << "Ma Phieu SC: " << maDinhDanh 
                  << " | Ma HD: " << maHopDong 
                  << " | Ma KH: " << maKhachHang 
                  << " | Ma NV: " << maNhanVien 
                  << " | Ngay Bao: " << ngayBaoSua 
                  << " | Mo Ta Loi: " << moTaLoi 
                  << " | Trang Thai: " << trangThai << "\n";
    }

    // Ghi đè hàm chuyển đối tượng thành chuỗi để ghi file phieu_sua_chua.txt
    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + maHopDong + "|" + maKhachHang + "|" + maNhanVien + "|" + ngayBaoSua + "|" + moTaLoi + "|" + trangThai;
    }

    // Ghi đè hàm đọc dữ liệu từ một dòng trong file text phieu_sua_chua.txt
    void docTuChuoi(const std::string& dong) override {
        std::stringstream ss(dong);
        std::getline(ss, maDinhDanh, '|');
        std::getline(ss, maHopDong, '|');
        std::getline(ss, maKhachHang, '|');
        std::getline(ss, maNhanVien, '|');
        std::getline(ss, ngayBaoSua, '|');
        std::getline(ss, moTaLoi, '|');
        std::getline(ss, trangThai, '|');
    }
};

#endif // PHIEU_SUA_CHUA_H
