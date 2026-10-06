/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp tiện ích tĩnh (Static Utility Class) xử lý 
 * toàn bộ các luồng nhập liệu, kiểm tra định dạng và bắt ngoại lệ.
 * ======================================================= */

#ifndef NHAP_DU_LIEU_H
#define NHAP_DU_LIEU_H

#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>

class NhapDuLieu {
public:

    // Nhập chuỗi cơ bản: Cắt khoảng trắng 2 đầu, cấm để trống, cấm nhập ký tự '|'
    static std::string nhapChuoi(const std::string& thongBao) {
        std::string chuoi;
        while (true) {
            std::cout << thongBao;
            std::getline(std::cin, chuoi);

            size_t dau = chuoi.find_first_not_of(" \t");
            if (dau == std::string::npos) {
                std::cout << " -> Loi: Thong tin khong duoc de trong!\n";
                continue;
            }

            size_t cuoi = chuoi.find_last_not_of(" \t");
            chuoi = chuoi.substr(dau, cuoi - dau + 1);

            if (chuoi.find('|') != std::string::npos) {
                std::cout << " -> Loi: Thong tin khong duoc chua ky tu '|'!\n";
                continue;
            }
            return chuoi;
        }
    }

    // Nhập mã định danh: Cấm khoảng trắng ở giữa, tự động viết hoa toàn bộ (VD: " ont01" -> "ONT01")
    static std::string nhapMaDinhDanh(const std::string& thongBao) {
        while (true) {
            std::string ma = nhapChuoi(thongBao);
            bool hopLe = true;

            for (char c : ma) {
                if (std::isspace(static_cast<unsigned char>(c))) {
                    hopLe = false;
                    break;
                }
            }

            if (hopLe) {
                std::transform(
                    ma.begin(),
                    ma.end(),
                    ma.begin(),
                    [](unsigned char c) {
                        return std::toupper(c);
                    }
                );
                return ma;
            }
            std::cout << " -> Loi: Ma dinh danh khong duoc chua khoang trang!\n";
        }
    }

    // Nhập số nguyên dương: Chống lỗi trôi lệnh khi người dùng cố tình nhập chữ
    static int nhapSoNguyenDuong(const std::string& thongBao) {
        int giaTri;
        while (true) {
            std::cout << thongBao;
            if (std::cin >> giaTri && giaTri >= 0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return giaTri;
            }
            std::cout << " -> Loi: Gia tri phai la so nguyen >= 0!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // Nhập số thực dương: Chống lỗi trôi lệnh tương tự như số nguyên
    static double nhapSoThucDuong(const std::string& thongBao) {
        double giaTri;
        while (true) {
            std::cout << thongBao;
            if (std::cin >> giaTri && giaTri >= 0.0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return giaTri;
            }
            std::cout << " -> Loi: Gia tri phai la so thuc >= 0!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // Nhập số điện thoại: Bắt buộc độ dài 10 ký tự, bắt đầu bằng số '0' và toàn là số
    static std::string nhapSoDienThoai(const std::string& thongBao) {
        while (true) {
            std::string sdt = nhapChuoi(thongBao);
            if (sdt.length() == 10 && sdt[0] == '0') {
                bool toanSo = true;
                for (char c : sdt) {
                    if (!std::isdigit(static_cast<unsigned char>(c))) {
                        toanSo = false;
                        break;
                    }
                }
                if (toanSo) return sdt;
            }
            std::cout << " -> Loi: So dien thoai phai co 10 chu so va bat dau bang '0'!\n";
        }
    }

    // Kiểm tra năm nhuận phục vụ cho thuật toán ngày tháng
    static bool laNamNhuan(int nam) {
        return (nam % 400 == 0) || (nam % 4 == 0 && nam % 100 != 0);
    }

    // Kiểm tra tính hợp lệ của ngày tháng năm (xử lý cả tháng 2 năm nhuận)
    static bool ngayHopLe(int ngay, int thang, int nam) {
        if (nam < 1900 || nam > 2100) return false;
        if (thang < 1 || thang > 12) return false;

        int soNgayTrongThang[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (thang == 2 && laNamNhuan(nam)) {
            soNgayTrongThang[1] = 29;
        }
        return ngay >= 1 && ngay <= soNgayTrongThang[thang - 1];
    }

    // Nhập và xác thực chuỗi ngày tháng theo đúng định dạng DD/MM/YYYY
    static std::string nhapNgayThang(const std::string& thongBao) {
        while (true) {
            std::string ngay = nhapChuoi(thongBao);
            if (ngay.length() == 10 && ngay[2] == '/' && ngay[5] == '/') {
                try {
                    bool hopLe = true;
                    for (int i = 0; i < 10; i++) {
                        if (i == 2 || i == 5) continue;
                        if (!std::isdigit(static_cast<unsigned char>(ngay[i]))) {
                            hopLe = false;
                            break;
                        }
                    }
                    if (!hopLe) throw std::exception();

                    int d = std::stoi(ngay.substr(0, 2));
                    int m = std::stoi(ngay.substr(3, 2));
                    int y = std::stoi(ngay.substr(6, 4));

                    if (ngayHopLe(d, m, y)) return ngay;
                } catch (...) {
                    // Bỏ qua ngoại lệ để xuống dòng dưới in thông báo lỗi
                }
            }
            std::cout << " -> Loi: Ngay khong hop le! Dung dinh dang DD/MM/YYYY (VD: 15/03/2026).\n";
        }
    }

    // Nhập xác nhận (Y/N) cho các thao tác quan trọng như Xóa, Thoát
    static bool xacNhan(const std::string& thongBao) {
        std::string luaChon;
        while (true) {
            std::cout << thongBao << " (y/n): ";
            std::getline(std::cin, luaChon);
            if (luaChon == "y" || luaChon == "Y") return true;
            if (luaChon == "n" || luaChon == "N") return false;
            std::cout << " -> Vui long chi nhap 'y' (Dong y) hoac 'n' (Huy bo).\n";
        }
    }
};

#endif
