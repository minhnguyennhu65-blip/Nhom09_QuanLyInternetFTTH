
/* =======================================================
 * Tên tác giả: Nguyễn Đức Huy
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp quản lý hóa đơn cước
 * ======================================================= */

#ifndef HOA_DON_CUOC_H
#define HOA_DON_CUOC_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <regex>

#include "LopCoSo.h"

using namespace std;

class HoaDonCuoc : public LopCoSo {
private:
    string maHopDong;
    string thangCuoc;
    double soTien;
    string ngayLap;
    string ngayThanhToan;
    string trangThai;

    // Kiểm tra định dạng tháng cước MM/YYYY.
    bool kiemTraThangCuoc(const string& thang) const {
        if (!regex_match(thang, regex(R"((0[1-9]|1[0-2])/[0-9]{4})"))) {
            return false;
        }
        return true;
    }

    // Kiểm tra ngày theo định dạng DD/MM/YYYY.
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

        if (m < 1 || m > 12 || d < 1) {
            return false;
        }

        int soNgayTrongThang[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        // Kiểm tra năm nhuận.
        bool namNhuan = (y % 400 == 0) ||
                        (y % 4 == 0 && y % 100 != 0);

        if (namNhuan) {
            soNgayTrongThang[1] = 29;
        }

        return d <= soNgayTrongThang[m - 1];
    }

    // Kiểm tra trạng thái hóa đơn.
    bool kiemTraTrangThai(const string& tt) const {
        return tt == "Chua thanh toan" ||
               tt == "Da thanh toan" ||
               tt == "Qua han";
    }

    // Kiểm tra dữ liệu hiện tại của hóa đơn.
    bool kiemTraDuLieu() const {
        if (maHopDong.empty()) {
            return false;
        }

        if (!kiemTraThangCuoc(thangCuoc)) {
            return false;
        }

        if (soTien < 0) {
            return false;
        }

        if (!kiemTraNgay(ngayLap)) {
            return false;
        }

        // Ngày thanh toán có thể để trống nếu chưa thanh toán.
        if (!ngayThanhToan.empty() &&
            !kiemTraNgay(ngayThanhToan)) {
            return false;
        }

        if (!kiemTraTrangThai(trangThai)) {
            return false;
        }

        return true;
    }

public:
    // Hàm khởi tạo mặc định.
    HoaDonCuoc()
        : maHopDong(""),
          thangCuoc(""),
          soTien(0),
          ngayLap(""),
          ngayThanhToan(""),
          trangThai("Chua thanh toan") {
    }

    // Lấy mã hợp đồng liên kết với hóa đơn.
    string getMaHopDong() const {
        return maHopDong;
    }

    // Lấy tháng cước.
    string getThangCuoc() const {
        return thangCuoc;
    }

    // Lấy số tiền cước.
    double getSoTien() const {
        return soTien;
    }

    // Lấy ngày lập hóa đơn.
    string getNgayLap() const {
        return ngayLap;
    }

    // Lấy ngày thanh toán.
    string getNgayThanhToan() const {
        return ngayThanhToan;
    }

    // Lấy trạng thái hóa đơn.
    string getTrangThai() const {
        return trangThai;
    }

    // Cập nhật mã hợp đồng.
    void setMaHopDong(const string& ma) {
        maHopDong = ma;
    }

    // Nhập thông tin hóa đơn cước.
    void nhapThongTin() override {
        LopCoSo::nhapThongTin();

        cout << "Nhap ma hop dong: ";
        getline(cin, maHopDong);

        do {
            cout << "Nhap thang cuoc (MM/YYYY): ";
            getline(cin, thangCuoc);

            if (!kiemTraThangCuoc(thangCuoc)) {
                cout << "Thang cuoc khong hop le. Vui long nhap lai.\n";
            }
        } while (!kiemTraThangCuoc(thangCuoc));

        do {
            cout << "Nhap so tien cuoc: ";
            cin >> soTien;

            if (cin.fail() || soTien < 0) {
                cout << "So tien khong hop le. Vui long nhap lai.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                soTien = -1;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (soTien < 0);

        do {
            cout << "Nhap ngay lap (DD/MM/YYYY): ";
            getline(cin, ngayLap);

            if (!kiemTraNgay(ngayLap)) {
                cout << "Ngay lap khong hop le. Vui long nhap lai.\n";
            }
        } while (!kiemTraNgay(ngayLap));

        cout << "Nhap ngay thanh toan (DD/MM/YYYY, bo trong neu chua thanh toan): ";
        getline(cin, ngayThanhToan);

        while (!ngayThanhToan.empty() &&
               !kiemTraNgay(ngayThanhToan)) {
            cout << "Ngay thanh toan khong hop le. Nhap lai: ";
            getline(cin, ngayThanhToan);
        }

        cout << "Trang thai (Chua thanh toan/Da thanh toan/Qua han): ";
        getline(cin, trangThai);

        while (!kiemTraTrangThai(trangThai)) {
            cout << "Trang thai khong hop le. Nhap lai: ";
            getline(cin, trangThai);
        }
    }

    // Cập nhật thông tin hóa đơn.
    void capNhatThongTin() override {
        cout << "\n--- CAP NHAT HOA DON CUOC ---\n";

        string input;

        cout << "Ma hop dong hien tai: " << maHopDong
             << "\nNhap ma hop dong moi (Enter de giu nguyen): ";
        getline(cin, input);
        if (!input.empty()) {
            maHopDong = input;
        }

        cout << "Thang cuoc hien tai: " << thangCuoc
             << "\nNhap thang cuoc moi (Enter de giu nguyen): ";
        getline(cin, input);
        if (!input.empty()) {
            while (!kiemTraThangCuoc(input)) {
                cout << "Thang cuoc khong hop le. Nhap lai: ";
                getline(cin, input);
            }
            thangCuoc = input;
        }

        cout << "So tien hien tai: " << soTien
             << "\nNhap so tien moi (-1 de giu nguyen): ";

        double tienMoi;
        cin >> tienMoi;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while (cin.fail() || (tienMoi < 0 && tienMoi != -1)) {
            cin.clear();
            cout << "So tien khong hop le. Nhap lai: ";
            cin >> tienMoi;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (tienMoi >= 0) {
            soTien = tienMoi;
        }

        cout << "Ngay lap hien tai: " << ngayLap
             << "\nNhap ngay lap moi (Enter de giu nguyen): ";
        getline(cin, input);

        if (!input.empty()) {
            while (!kiemTraNgay(input)) {
                cout << "Ngay lap khong hop le. Nhap lai: ";
                getline(cin, input);
            }
            ngayLap = input;
        }

        cout << "Ngay thanh toan hien tai: "
             << (ngayThanhToan.empty() ? "Chua thanh toan" : ngayThanhToan)
             << "\nNhap ngay thanh toan moi (Enter de giu nguyen, nhap '-' de xoa): ";
        getline(cin, input);

        if (input == "-") {
            ngayThanhToan = "";
        } else if (!input.empty()) {
            while (!kiemTraNgay(input)) {
                cout << "Ngay thanh toan khong hop le. Nhap lai: ";
                getline(cin, input);
            }
            ngayThanhToan = input;
        }

        cout << "Trang thai hien tai: " << trangThai
             << "\nNhap trang thai moi (Enter de giu nguyen): ";
        getline(cin, input);

        if (!input.empty()) {
            while (!kiemTraTrangThai(input)) {
                cout << "Trang thai khong hop le. Nhap lai: ";
                getline(cin, input);
            }
            trangThai = input;
        }
    }

    // Hiển thị thông tin hóa đơn.
    void hienThiThongTin() const override {
        LopCoSo::hienThiThongTin();

        cout << left
             << setw(22) << "Ma hop dong: " << maHopDong << '\n'
             << setw(22) << "Thang cuoc: " << thangCuoc << '\n'
             << setw(22) << "So tien: " << fixed << setprecision(2)
             << soTien << '\n'
             << setw(22) << "Ngay lap: " << ngayLap << '\n'
             << setw(22) << "Ngay thanh toan: "
             << (ngayThanhToan.empty() ? "Chua thanh toan" : ngayThanhToan)
             << '\n'
             << setw(22) << "Trang thai: " << trangThai << '\n';
    }

    // Chuyển dữ liệu hóa đơn thành một dòng để lưu file.
    // Định dạng gồm 7 trường, phân cách bằng dấu '|'.
    string chuyenThanhDong() const override {
        stringstream ss;

        ss << LopCoSo::chuyenThanhDong() << '|'
           << maHopDong << '|'
           << thangCuoc << '|'
           << soTien << '|'
           << ngayLap << '|'
           << ngayThanhToan << '|'
           << trangThai;

        return ss.str();
    }

    // Đọc thông tin hóa đơn từ một dòng trong file.
    bool docTuDong(const string& dong) override {
        stringstream ss(dong);
        string ma, thang, tien, lap, thanhToan, tt;
        string maCoSo;

        if (!getline(ss, maCoSo, '|') ||
            !getline(ss, ma, '|') ||
            !getline(ss, thang, '|') ||
            !getline(ss, tien, '|') ||
            !getline(ss, lap, '|') ||
            !getline(ss, thanhToan, '|') ||
            !getline(ss, tt)) {
            return false;
        }

        try {
            size_t viTri;
            double tienDoc = stod(tien, &viTri);

            if (viTri != tien.size() || tienDoc < 0) {
                return false;
            }

            // Đọc phần dữ liệu cơ sở theo cấu trúc của lớp cha.
            if (!LopCoSo::docTuDong(maCoSo)) {
                return false;
            }

            maHopDong = ma;
            thangCuoc = thang;
            soTien = tienDoc;
            ngayLap = lap;
            ngayThanhToan = thanhToan;
            trangThai = tt;

            return kiemTraDuLieu();
        } catch (...) {
            return false;
        }
    }
};

#endif // HOA_DON_CUOC_H
