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

    // Kiem tra khoa ngoai voi file du lieu tương ứng (Đã cập nhật sửa lỗi)
    bool kiemTraKhoaNgoai(const string& filePath, const string& ma) const {
        if (!chuoiHopLe(ma)) return false;

        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "\n[Canh bao Hethong] Khong the mo file: " << filePath << ". Vui long kiem tra thu muc 'data' hoac luu file truoc!\n";
            return false;
        }

        const string maCanTim = chuanHoaMa(ma);
        string dong;

        while (getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') continue;

            // Loai bo ky tu an BOM (UTF-8) o dong dau tien neu co
            if (dong.size() >= 3 && dong.substr(0, 3) == "\xEF\xBB\xBF") {
                dong = dong.substr(3);
            }

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
    string nhapMaKhoaNgoai(const string& loiNhac, const string& filePath, const string& tenDoi
