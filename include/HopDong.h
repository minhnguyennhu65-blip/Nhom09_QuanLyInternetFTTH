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

    // Chuan hoa ma: viet hoa va bo khoang trang
    string chuanHoaMa(string ma) const {
        string ketQua;
        for (unsigned char c : ma) {
            if (!isspace(c)) {
                ketQua += static_cast<char>(toupper(c));
            }
        }
        return ketQua;
    }

    // Kiem tra chuoi khong rong va khong chua dau '|'
    bool chuoiHopLe(const string& str) const {
        if (str.find('|') != string::npos) {
            return false;
        }
        for (unsigned char c : str) {
            if (!isspace(c)) {
                return true;
            }
        }
        return false;
    }

    // Kiem tra ngay DD/MM/YYYY co ton tai tren lich
    bool kiemTraNgay(const string& ngay) const {
        if (!regex_match(ngay, regex(R"([0-9]{2}/[0-9]{2}/[0-9]{4})"))) {
            return false;
        }

        int d, m, y;
        char c1, c2;
        stringstream ss(ngay);
        ss >> d >> c1 >> m >> c2 >> y;

        if (ss.fail() || c1 != '/' || c2 != '/') {
            return false;
        }

        if (y < 1900 || y > 2100 || m < 1 || m > 12 || d < 1) {
            return false;
        }

        int soNgayTrongThang[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        bool namNhuan = (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
        if (namNhuan) {
            soNgayTrongThang[1] = 29;
        }

        return d <= soNgayTrongThang[m - 1];
    }

    // Chuyen ngay sang so YYYYMMDD de so sanh thu tu thoi gian
    int ngayThanhSo(const string& ngay) const {
        if (!kiemTraNgay(ngay)) return -1;
        int d = stoi(ngay.substr(0, 2));
        int m = stoi(ngay.substr(3, 2));
        int y = stoi(ngay.substr(6, 4));
        return y * 10000 + m * 100 + d;
    }

    // Kiem tra khoa ngoai voi file du lieu tương ứng
    bool kiemTraKhoaNgoai(const string& filePath, const string& ma) const {
        if (!chuoiHopLe(ma)) return false;

        ifstream file(filePath);
        if (!file.is_open()) return false;

        const string maCanTim = chuanHoaMa(ma);
        string dong;

        while (getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') continue;

            stringstream ss(dong);
            string maTrongFile;

            if (getline(ss, maTrongFile, '|') && chuanHoaMa(maTrongFile) == maCanTim) {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    // Kiem tra trang thai hop dong
    bool kiemTraTrangThai(const string& tt) const {
        return tt == "Hoat dong" || tt == "Het han" || tt == "Huy";
    }

    // Nhap va xac thuc khoa ngoai co ho tro huy (0)
    string nhapMaKhoaNgoai(const string& loiNhac, const string& filePath, const string& tenDoiTuong) const {
        string ma;
        while (true) {
            cout << loiNhac;
            getline(cin, ma);

            if (ma == "0") return ""; // Tra ve chuoi rong de bao hieu huy

            ma = chuanHoaMa(ma);

            if (!chuoiHopLe(ma)) {
                cout << "Loi: Ma " << tenDoiTuong << " khong duoc chua ky tu '|' hoac de trong.\n";
                continue;
            }

            if (!kiemTraKhoaNgoai(filePath, ma)) {
                cout << "Loi: Ma " << tenDoiTuong << " [" << ma
                     << "] khong ton tai trong " << filePath << ". Vui long nhap lai (hoac nhap '0' de huy).\n";
                continue;
            }

            return ma;
        }
    }

public:
    HopDong()
        : LopCoSo(),
          maKhachHang(""),
          maGoiCuoc(""),
          maThietBi(""),
          ngayKy(""),
          ngayHetHan(""),
          giaThieuDung(0.0),
          trangThai("Hoat dong") {
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

    // NHAP THONG TIN HOP DONG MOI
    void nhapThongTin() override {
        // 1. Nhap ma hop dong
        while (true) {
            cout << "Nhap ma hop dong (Nhan '0' de huy): ";
            getline(cin >> ws, maDinhDanh);

            if (maDinhDanh == "0") {
                maDinhDanh = ""; // Danh dau huy thao tac
                cout << " -> Da huy thao tac them hop dong.\n";
                return;
            }

            maDinhDanh = chuanHoaMa(maDinhDanh);

            if (!chuoiHopLe(maDinhDanh)) {
                cout << "Loi: Ma hop dong khong hop le.\n";
                continue;
            }

            if (kiemTraKhoaNgoai("data/hop_dong.txt", maDinhDanh)) {
                cout << "Loi: Ma hop dong [" << maDinhDanh << "] da ton tai trong data/hop_dong.txt!\n";
                continue;
            }
            break;
        }

        // 2. Nhap ma khach hang (Khoa ngoai)
        maKhachHang = nhapMaKhoaNgoai("Nhap ma khach hang (Nhan '0' de huy): ", "data/khach_hang.txt", "khach hang");
        if (maKhachHang.empty()) {
            maDinhDanh = "";
            cout << " -> Da huy thao tac them hop dong.\n";
            return;
        }

        // 3. Nhap ma goi cuoc (Khoa ngoai)
        maGoiCuoc = nhapMaKhoaNgoai("Nhap ma goi cuoc (Nhan '0' de huy): ", "data/goi_internet.txt", "goi cuoc");
        if (maGoiCuoc.empty()) {
            maDinhDanh = "";
            cout << " -> Da huy thao tac them hop dong.\n";
            return;
        }

        // 4. Nhap ma thiet bi (Khoa ngoai)
        maThietBi = nhapMaKhoaNgoai("Nhap ma thiet bi (Nhan '0' de huy): ", "data/thiet_bi.txt", "thiet bi");
        if (maThietBi.empty()) {
            maDinhDanh = "";
            cout << " -> Da huy thao tac them hop dong.\n";
            return;
        }

        // 5. Nhap ngay ky va ngay het han
        while (true) {
            cout << "Nhap ngay ky (DD/MM/YYYY): ";
            getline(cin, ngayKy);
            if (!kiemTraNgay(ngayKy)) {
                cout << "Loi: Ngay ky khong hop le!\n";
                continue;
            }

            cout << "Nhap ngay het han (DD/MM/YYYY): ";
            getline(cin, ngayHetHan);
            if (!kiemTraNgay(ngayHetHan)) {
                cout << "Loi: Ngay het han khong hop le!\n";
                continue;
            }

            if (ngayThanhSo(ngayHetHan) < ngayThanhSo(ngayKy)) {
                cout << "Loi: Ngay het han phai sau hoac bang ngay ky!\n";
                continue;
            }
            break;
        }

        // 6. Nhap gia thieu dung
        while (true) {
            cout << "Nhap gia thieu dung (>= 0): ";
            if (cin >> giaThieuDung && isfinite(giaThieuDung) && giaThieuDung >= 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout << "Loi: Gia thieu dung phai la so khong am!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // 7. Nhap trang thai
        while (true) {
            cout << "Trang thai (Hoat dong/Het han/Huy): ";
            getline(cin, trangThai);

            if (kiemTraTrangThai(trangThai)) {
                break;
            }
            cout << "Loi: Trang thai khong hop le.\n";
        }
    }

    // CAP NHAT THONG TIN HOP DONG
    void capNhatThongTin() override {
        string input;
        cout << "\n--- CAP NHAT THONG TIN HOP DONG ---\n";

        // Cap nhat Ma KH
        while (true) {
            cout << "Ma khach hang hien tai: " << maKhachHang
                 << "\nNhap ma KH moi (Enter de giu nguyen, '0' de huy): ";
            getline(cin, input);
            if (input.empty() || input == "0") break;

            input = chuanHoaMa(input);
            if (!kiemTraKhoaNgoai("data/khach_hang.txt", input)) {
                cout << "Loi: Ma khach hang khong ton tai trong data/khach_hang.txt!\n";
                continue;
            }
            maKhachHang = input;
            break;
        }

        // Cap nhat Ma Goi Cuoc
        while (true) {
            cout << "Ma goi cuoc hien tai: " << maGoiCuoc
                 << "\nNhap ma goi cuoc moi (Enter de giu nguyen, '0' de huy): ";
            getline(cin, input);
            if (input.empty() || input == "0") break;

            input = chuanHoaMa(input);
            if (!kiemTraKhoaNgoai("data/goi_internet.txt", input)) {
                cout << "Loi: Ma goi cuoc khong ton tai trong data/goi_internet.txt!\n";
                continue;
            }
            maGoiCuoc = input;
            break;
        }

        // Cap nhat Ma Thiet Bi
        while (true) {
            cout << "Ma thiet bi hien tai: " << maThietBi
                 << "\nNhap ma thiet bi moi (Enter de giu nguyen, '0' de huy): ";
            getline(cin, input);
            if (input.empty() || input == "0") break;

            input = chuanHoaMa(input);
            if (!kiemTraKhoaNgoai("data/thiet_bi.txt", input)) {
                cout << "Loi: Ma thiet bi khong ton tai trong data/thiet_bi.txt!\n";
                continue;
            }
            maThietBi = input;
            break;
        }

        // Cap nhat ngay ky & ngay het han
        while (true) {
            string nkMoi, nhhMoi;
            cout << "Ngay ky hien tai: " << ngayKy
                 << "\nNhap ngay ky moi (Enter de giu nguyen): ";
            getline(cin, nkMoi);
            if (nkMoi.empty()) nkMoi = ngayKy;

            cout << "Ngay het han hien tai: " << ngayHetHan
                 << "\nNhap ngay het han moi (Enter de giu nguyen): ";
            getline(cin, nhhMoi);
            if (nhhMoi.empty()) nhhMoi = ngayHetHan;

            if (!kiemTraNgay(nkMoi) || !kiemTraNgay(nhhMoi)) {
                cout << "Loi: Dinh dang ngay khong hop le!\n";
                continue;
            }

            if (ngayThanhSo(nhhMoi) < ngayThanhSo(nkMoi)) {
                cout << "Loi: Ngay het han phai sau hoac bang ngay ky!\n";
                continue;
            }

            ngayKy = nkMoi;
            ngayHetHan = nhhMoi;
            break;
        }

        // Cap nhat gia thieu dung
        while (true) {
            cout << "Gia thieu dung hien tai: " << giaThieuDung
                 << "\nNhap gia thieu dung moi (-1 de giu nguyen): ";
            double giaMoi;
            if (cin >> giaMoi && isfinite(giaMoi) && (giaMoi == -1 || giaMoi >= 0)) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (giaMoi >= 0) giaThieuDung = giaMoi;
                break;
            }
            cout << "Loi: Gia thieu dung khong hop le.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Cap nhat trang thai
        while (true) {
            cout << "Trang thai hien tai: " << trangThai
                 << "\nNhap trang thai moi (Enter de giu nguyen): ";
            getline(cin, input);
            if (input.empty()) break;

            if (kiemTraTrangThai(input)) {
                trangThai = input;
                break;
            }
            cout << "Loi: Trang thai khong hop le.\n";
        }

        cout << " -> Cap nhat hop dong thanh cong!\n";
    }

    // HIEN THI THONG TIN
    void hienThiThongTin() const override {
        cout << left
             << setw(20) << "Ma hop dong: " << maDinhDanh << '\n'
             << setw(20) << "Ma khach hang: " << maKhachHang << '\n'
             << setw(20) << "Ma goi cuoc: " << maGoiCuoc << '\n'
             << setw(20) << "Ma thiet bi: " << maThietBi << '\n'
             << setw(20) << "Ngay ky: " << ngayKy << '\n'
             << setw(20) << "Ngay het han: " << ngayHetHan << '\n'
             << setw(20) << "Gia thieu dung: " << fixed << setprecision(2) << giaThieuDung << " VND\n"
             << setw(20) << "Trang thai: " << trangThai << '\n';
        cout << "----------------------------------------\n";
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
           << trangThai;
        return ss.str();
    }

    // DOC DU LIEU TU CHUOI GHI FILE
    void docTuChuoi(const string& dongDuLieu) override {
        stringstream ss(dongDuLieu);
        string maHD, maKH, maGoi, maTB, ky, hetHan, giaStr, tt, duThua;

        if (!getline(ss, maHD, '|') ||
            !getline(ss, maKH, '|') ||
            !getline(ss, maGoi, '|') ||
            !getline(ss, maTB, '|') ||
            !getline(ss, ky, '|') ||
            !getline(ss, hetHan, '|') ||
            !getline(ss, giaStr, '|') ||
            !getline(ss, tt)) {
            return;
        }

        if (getline(ss, duThua, '|')) return; // Khong chap nhan thua truong

        double giaDoc = 0.0;
        try {
            size_t pos = 0;
            giaDoc = stod(giaStr, &pos);
            if (pos != giaStr.size() || !isfinite(giaDoc) || giaDoc < 0) return;
        } catch (...) {
            return;
        }

        maHD = chuanHoaMa(maHD);
        maKH = chuanHoaMa(maKH);
        maGoi = chuanHoaMa(maGoi);
        maTB = chuanHoaMa(maTB);

        if (!chuoiHopLe(maHD) || !chuoiHopLe(maKH) || !chuoiHopLe(maGoi) || !chuoiHopLe(maTB) ||
            !kiemTraNgay(ky) || !kiemTraNgay(hetHan) ||
            ngayThanhSo(hetHan) < ngayThanhSo(ky) ||
            !kiemTraTrangThai(tt)) {
            return;
        }

        maDinhDanh = maHD;
        maKhachHang = maKH;
        maGoiCuoc = maGoi;
        maThietBi = maTB;
        ngayKy = ky;
        ngayHetHan = hetHan;
        giaThieuDung = giaDoc;
        trangThai = tt;
    }
};

#endif // HOP_DONG_H
