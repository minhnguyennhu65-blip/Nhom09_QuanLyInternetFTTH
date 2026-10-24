/* =======================================================
 * Tên tác giả: Hoàng Trọng Khoa
 * Mã sinh viên: B24DCVT204
 * Mô tả file: Lớp quản lý Hợp Đồng Thue Bao
 * ======================================================= */

#ifndef HOP_DONG_H
#define HOP_DONG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <cctype>
#include <vector>
#include <regex>
#include <cmath>

using namespace std;

class HopDong : public LopCoSo {
private:
    string maKhachHang;
    string maGoiCuoc;
    string maThietBi;
    string ngayKy;
    string ngayHetHan;
    double giaThieuDung;
    string trangThai;
    string loaiKhachHang; // Liên kết với loại khách hàng (Ca nhan / Doanh nghiep)

    // Kiem tra khoa ngoai voi file du lieu tương ứng
    bool kiemTraKhoaNgoai(const string& filePath, const string& ma) const {
        ifstream file(filePath);
        if (!file.is_open()) return false;

        string dong;
        while (getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') continue;

            stringstream ss(dong);
            string maTrongFile;

            if (getline(ss, maTrongFile, '|') && maTrongFile == ma) {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    // Lấy loại khách hàng từ file khach_hang.txt dựa trên mã khách hàng
    string layLoaiKhachHang(const string& maKH) const {
        ifstream file("data/khach_hang.txt");
        if (!file.is_open()) return "";

        string dong;
        while (getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') continue;

            stringstream ss(dong);
            string maTrongFile, ten, sdt, diaChi, email, loaiKH, ngayDK, trangThai;

            if (getline(ss, maTrongFile, '|') && maTrongFile == maKH) {
                getline(ss, ten, '|');
                getline(ss, sdt, '|');
                getline(ss, diaChi, '|');
                getline(ss, email, '|');
                getline(ss, loaiKH, '|');
                getline(ss, ngayDK, '|');
                getline(ss, trangThai, '|');

                file.close();
                return loaiKH;
            }
        }

        file.close();
        return "";
    }

    // Kiem tra trang thai hop dong
    bool kiemTraTrangThai(const string& tt) const {
        return tt == "Hoat dong" || tt == "Het han" || tt == "Huy";
    }

    // Nhap va xac thuc khoa ngoai co ho tro huy (0)
    string nhapMaKhoaNgoai(const string& loiNhac, const string& filePath, const string& tenDoiTuong) const {
        while (true) {
            string ma = NhapDuLieu::nhapChuoi(loiNhac);

            if (ma == "0") return ""; // Tra ve chuoi rong de bao hieu huy

            if (!kiemTraKhoaNgoai(filePath, ma)) {
                cout << " -> Loi: Ma " << tenDoiTuong << " [" << ma << "] khong ton tai trong " << filePath << ". Vui long nhap lai (hoac nhap '0' de huy).\n";
                continue;
            }

            return ma;
        }
    }

public:
    HopDong()
        : LopCoSo(), maKhachHang(""), maGoiCuoc(""), maThietBi(""), ngayKy(""), ngayHetHan(""), giaThieuDung(0.0), trangThai("Hoat dong") {
    }

    // In tiêu đề bảng chuẩn có dấu phân cách ' | '
    static void inTieuDe() {
        std::cout << std::left
            << std::setw(7) << "Ma HD" << " | "
            << std::setw(7) << "Ma KH" << " | "
            << std::setw(8) << "Ma Goi" << " | "
            << std::setw(7) << "Ma TB" << " | "
            << std::setw(12) << "Ngay ky" << " | "
            << std::setw(12) << "Ngay het" << " | "
            << std::setw(15) << "Gia (VND)" << " | "
            << std::setw(15) << "Loai KH" << " | "
            << std::setw(12) << "Trang thai" << "\n"
            << std::string(127, '-') << "\n";
    }

    // GETTERS
    string getMaHopDong() const { return maDinhDanh; }
    string getMaKhachHang() const { return maKhachHang; }
    string getMaGoiCuoc() const { return maGoiCuoc; }
    string getMaThietBi() const { return maThietBi; }
    string getNgayKy() const { return ngayKy; }
    string getNgayHetHan() const { return ngayHetHan; }
    double getGiaThieuDung() const { return giaThieuDung; }
    string getTrangThai() const { return trangThai; }
    string getLoaiKhachHang() const { return loaiKhachHang; }

    // NHAP THONG TIN HOP DONG MOI
    void nhapThongTin() override {
        while (true) {
            string ma = NhapDuLieu::nhapChuoi("Nhap ma hop dong (Nhan '0' de huy): ");

            if (ma == "0") {
                cout << " -> Da huy thao tac them hop dong.\n";
                return;
            }

            if (kiemTraKhoaNgoai("data/hop_dong.txt", ma)) {
                cout << " -> Loi: Ma hop dong [" << ma << "] da ton tai trong data/hop_dong.txt!\n";
                continue;
            }

            setMaDinhDanh(ma);
            break;
        }

        maKhachHang = nhapMaKhoaNgoai("Nhap ma khach hang (Nhan '0' de huy): ", "data/khach_hang.txt", "khach hang");
        if (maKhachHang.empty()) { cout << " -> Da huy thao tac them hop dong.\n"; return; }

        // Tự động lấy loại khách hàng từ file khach_hang.txt
        loaiKhachHang = layLoaiKhachHang(maKhachHang);
        if (loaiKhachHang.empty()) {
            cout << " -> Loi: Khong the lay loai khach hang tu ma [" << maKhachHang << "]!\n";
            return;
        }
        cout << " -> Loai khach hang: " << loaiKhachHang << "\n";

        maGoiCuoc = nhapMaKhoaNgoai("Nhap ma goi cuoc (Nhan '0' de huy): ", "data/goi_internet.txt", "goi cuoc");
        if (maGoiCuoc.empty()) { cout << " -> Da huy thao tac them hop dong.\n"; return; }

        maThietBi = nhapMaKhoaNgoai("Nhap ma thiet bi (Nhan '0' de huy): ", "data/thiet_bi.txt", "thiet bi");
        if (maThietBi.empty()) { cout << " -> Da huy thao tac them hop dong.\n"; return; }

        // Nhập ngày ký và ngày hết hạn
        while (true) {
            ngayKy = NhapDuLieu::nhapNgayThang("Nhap ngay ky (DD/MM/YYYY): ");
            ngayHetHan = NhapDuLieu::nhapNgayThang("Nhap ngay het han (DD/MM/YYYY): ");

            if (NhapDuLieu::soSanhNgay(ngayHetHan, ngayKy) < 0) {
                cout << " -> Loi: Ngay het han phai sau hoac bang ngay ky!\n";
                continue;
            }
            break;
        }

        // Nhập giá thuê dùng
        giaThieuDung = NhapDuLieu::nhapSoThucDuong("Nhap gia thieu dung (>= 0): ");

        // Nhập trạng thái
        while (true) {
            trangThai = NhapDuLieu::nhapChuoi("Trang thai (Hoat dong/Het han/Huy): ");
            if (kiemTraTrangThai(trangThai)) break;
            cout << " -> Loi: Trang thai khong hop le.\n";
        }
    }

    // CAP NHAT THONG TIN HOP DONG
    void capNhatThongTin() override {
        cout << "\n--- CAP NHAT THONG TIN HOP DONG ---\n";

        string input;
        while (true) {
            cout << "Ma khach hang hien tai: " << maKhachHang << "\nNhap ma KH moi (Enter de giu nguyen, '0' de huy): ";
            input = NhapDuLieu::nhapChuoiHoacGiuNguyen("Ma KH moi: ", maKhachHang);
            if (input.empty() || input == "0") break;
            if (!kiemTraKhoaNgoai("data/khach_hang.txt", input)) { cout << " -> Loi: Ma khach hang khong ton tai!\n"; continue; }
            maKhachHang = input;
            // Cập nhật lại loại khách hàng khi thay đổi mã khách hàng
            loaiKhachHang = layLoaiKhachHang(maKhachHang);
            if (!loaiKhachHang.empty()) {
                cout << " -> Loai khach hang duoc cap nhat: " << loaiKhachHang << "\n";
            }
            break;
        }

        while (true) {
            cout << "Ma goi cuoc hien tai: " << maGoiCuoc << "\nNhap ma goi cuoc moi (Enter de giu nguyen, '0' de huy): ";
            input = NhapDuLieu::nhapChuoiHoacGiuNguyen("Ma goi cuoc moi: ", maGoiCuoc);
            if (input.empty() || input == "0") break;
            if (!kiemTraKhoaNgoai("data/goi_internet.txt", input)) { cout << " -> Loi: Ma goi cuoc khong ton tai!\n"; continue; }
            maGoiCuoc = input; break;
        }

        while (true) {
            cout << "Ma thiet bi hien tai: " << maThietBi << "\nNhap ma thiet bi moi (Enter de giu nguyen, '0' de huy): ";
            input = NhapDuLieu::nhapChuoiHoacGiuNguyen("Ma thiet bi moi: ", maThietBi);
            if (input.empty() || input == "0") break;
            if (!kiemTraKhoaNgoai("data/thiet_bi.txt", input)) { cout << " -> Loi: Ma thiet bi khong ton tai!\n"; continue; }
            maThietBi = input; break;
        }

        // Cập nhật ngày
        string nkMoi = NhapDuLieu::nhapNgayThangHoacGiuNguyen("Nhap ngay ky moi (Enter de giu nguyen): ", ngayKy);
        if (!nkMoi.empty()) ngayKy = nkMoi;

        string nhhMoi = NhapDuLieu::nhapNgayThangHoacGiuNguyen("Nhap ngay het han moi (Enter de giu nguyen): ", ngayHetHan);
        if (!nhhMoi.empty()) ngayHetHan = nhhMoi;

        if (NhapDuLieu::soSanhNgay(ngayHetHan, ngayKy) < 0) {
            cout << " -> Loi: Ngay het han phai sau ngay ky!\n";
            return;
        }

        // Cập nhật giá
        giaThieuDung = NhapDuLieu::nhapSoThucHoacGiuNguyen("Nhap gia thieu dung moi (-1 de giu nguyen): ", giaThieuDung, 0.0, DBL_MAX, true);

        // Cập nhật trạng thái
        string ttMoi = NhapDuLieu::nhapChuoiHoacGiuNguyen("Nhap trang thai moi (Enter de giu nguyen): ", trangThai);
        if (!ttMoi.empty()) {
            if (kiemTraTrangThai(ttMoi)) trangThai = ttMoi;
            else cout << " -> Loi: Trang thai khong hop le.\n";
        }

        cout << " -> Cap nhat hop dong thanh cong!\n";
    }

    // HIEN THI THONG TIN (một dòng căn cột)
    void hienThiThongTin() const override {
        std::cout << std::left
            << std::setw(7) << maDinhDanh << " | "
            << std::setw(7) << maKhachHang << " | "
            << std::setw(8) << maGoiCuoc << " | "
            << std::setw(7) << maThietBi << " | "
            << std::setw(12) << ngayKy << " | "
            << std::setw(12) << ngayHetHan << " | "
            << std::setw(15) << std::fixed << std::setprecision(0) << giaThieuDung << " | "
            << std::setw(15) << loaiKhachHang << " | "
            << std::setw(12) << trangThai << '\n';
    }

    // CHUYEN DU LIEU THANH CHUOI DE GHI FILE
    string chuyenThanhChuoi() const override {
        stringstream ss;
        ss << maDinhDanh << '|'
            << maKhachHang << '|'
            << maGoiCuoc << '|'
            << maThietBi << '|'
            << ngayKy << '|'
            << ngayHetHan << '|'
            << setprecision(15) << giaThieuDung << '|'
            << loaiKhachHang << '|'
            << trangThai;
        return ss.str();
    }

    // DOC DU LIEU TU CHUOI GHI FILE
    void docTuChuoi(const string& dong) override {
        stringstream ss(dong);
        string maHD, maKH, maGoi, maTB, ky, hetHan, giaStr, loaiKH, tt, duThua;

        if (!getline(ss, maHD, '|') || !getline(ss, maKH, '|') || !getline(ss, maGoi, '|') ||
            !getline(ss, maTB, '|') || !getline(ss, ky, '|') || !getline(ss, hetHan, '|') ||
            !getline(ss, giaStr, '|') || !getline(ss, loaiKH, '|') || !getline(ss, tt, '|')) {
            throw std::invalid_argument("Dong du lieu HopDong co so cot khong dung.");
        }

        if (!tt.empty() && tt.back() == '\r') tt.pop_back();
        if (getline(ss, duThua, '|')) {
            throw std::invalid_argument("Dong du lieu HopDong co du lieu thua.");
        }

        // Chuyển giá
        double giaDoc = 0.0;
        try {
            size_t pos = 0;
            giaDoc = stod(giaStr, &pos);
            if (pos != giaStr.size() || !isfinite(giaDoc) || giaDoc < 0) {
                throw std::invalid_argument("Gia thieu dung khong hop le.");
            }
        }
        catch (...) {
            throw std::invalid_argument("Gia thieu dung khong hop le.");
        }

        // Kiểm tra ngày
        if (!NhapDuLieu::laNgayThangHopLe(ky) || !NhapDuLieu::laNgayThangHopLe(hetHan)) {
            throw std::invalid_argument("Ngay khong hop le.");
        }

        if (NhapDuLieu::soSanhNgay(hetHan, ky) < 0) {
            throw std::invalid_argument("Ngay het han phai sau hoac bang ngay ky.");
        }

        // Kiểm tra trạng thái
        if (!kiemTraTrangThai(tt)) {
            throw std::invalid_argument("Trang thai hop dong khong hop le: " + tt);
        }

        // Gán giá trị
        setMaDinhDanh(maHD);
        maKhachHang = maKH;
        maGoiCuoc = maGoi;
        maThietBi = maTB;
        ngayKy = ky;
        ngayHetHan = hetHan;
        giaThieuDung = giaDoc;
        loaiKhachHang = loaiKH;
        trangThai = tt;
    }
};

#endif // HOP_DONG_H
