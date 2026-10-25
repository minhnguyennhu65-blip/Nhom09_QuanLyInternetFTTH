/* =======================================================
 * Tên tác giả: Nguyễn Đức Huy
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp quản lý hóa đơn cước
 * ======================================================= */

#ifndef HOA_DON_CUOC_H
#define HOA_DON_CUOC_H

#include "LopCoSo.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <regex>
#include <fstream>
#include <cmath>
#include <cctype>
#include <stdexcept>

using namespace std;

class HoaDonCuoc : public LopCoSo {
private:
    string maHopDong;
    string thangCuoc;
    double soTien;
    string ngayLap;
    string ngayThanhToan;
    string trangThai;

    // Chuan hoa ma: viet hoa va bo khoang trang.
    string chuanHoaMa(string ma) const {
        string ketQua;
        for (unsigned char c : ma) {
            if (!isspace(c)) {
                ketQua += static_cast<char>(toupper(c));
            }
        }
        return ketQua;
    }

    // Kiem tra chuoi khong rong va khong chua dau phan cach du lieu '|'.
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

    // Kiem tra dinh dang ma hoa don.
    bool maHoaDonHopLe(const string& ma) const {
        return chuoiHopLe(ma);
    }

    // Kiem tra dinh dang thang cuoc MM/YYYY.
    bool kiemTraThangCuoc(const string& thang) const {
        return regex_match(
            thang,
            regex(R"((0[1-9]|1[0-2])/[0-9]{4})")
        );
    }

    // Kiem tra ngay DD/MM/YYYY co ton tai tren lich.
    bool kiemTraNgay(const string& ngay) const {
        if (!regex_match(
            ngay,
            regex(R"([0-9]{2}/[0-9]{2}/[0-9]{4})"))) {
            return false;
        }

        int d, m, y;
        char c1, c2;

        stringstream ss(ngay);
        ss >> d >> c1 >> m >> c2 >> y;

        if (ss.fail() || c1 != '/' || c2 != '/') {
            return false;
        }

        if (y < 1 || m < 1 || m > 12 || d < 1) {
            return false;
        }

        int soNgayTrongThang[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        bool namNhuan =
            (y % 400 == 0) ||
            (y % 4 == 0 && y % 100 != 0);

        if (namNhuan) {
            soNgayTrongThang[1] = 29;
        }

        return d <= soNgayTrongThang[m - 1];
    }

    // Kiem tra trang thai hoa don.
    bool kiemTraTrangThai(const string& tt) const {
        return tt == "Chua thanh toan" ||
            tt == "Da thanh toan" ||
            tt == "Qua han";
    }

    // Kiem tra ma hop dong co ton tai trong file hop dong khong.
    bool kiemTraHopDongTonTai(const string& ma) const {
        if (!chuoiHopLe(ma)) {
            return false;
        }

        ifstream file("data/hop_dong.txt");

        if (!file.is_open()) {
            return false;
        }

        const string maCanTim = chuanHoaMa(ma);
        string dong;

        while (getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') {
                continue;
            }

            stringstream ss(dong);
            string maTrongFile;

            if (getline(ss, maTrongFile, '|') &&
                chuanHoaMa(maTrongFile) == maCanTim) {
                return true;
            }
        }

        return false;
    }

    // Hàm cắt chuỗi dài để không làm lệch cột khi in bảng
    static string cat(const string& s, size_t n) {
        return s.size() <= n ? s : s.substr(0, n - 2) + "..";
    }

    // Nhap ma hop dong va kiem tra khoa ngoai.
    string nhapMaHopDong() const {
        string ma;

        while (true) {
            cout << "Nhap ma hop dong: ";
            getline(cin, ma);

            ma = chuanHoaMa(ma);

            if (!chuoiHopLe(ma)) {
                cout << "Loi: Ma hop dong khong duoc rong "
                    << "hoac chua ky tu '|'.\n";
                continue;
            }

            if (!kiemTraHopDongTonTai(ma)) {
                cout << "Loi: Ma hop dong [" << ma
                    << "] khong ton tai trong data/hop_dong.txt. "
                    << "Vui long nhap lai.\n";
                continue;
            }

            return ma;
        }
    }

    // Nhap mot ngay hop le.
    string nhapNgay(const string& loiNhac) const {
        string ngay;

        while (true) {
            cout << loiNhac;
            getline(cin, ngay);

            if (kiemTraNgay(ngay)) {
                return ngay;
            }

            cout << "Loi: Ngay khong hop le. "
                << "Vui long nhap theo DD/MM/YYYY.\n";
        }
    }

    // Nhap so tien khong am.
    double nhapSoTien() const {
        double tien;

        while (true) {
            cout << "Nhap so tien cuoc: ";

            if (cin >> tien &&
                isfinite(tien) &&
                tien >= 0) {
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );
                return tien;
            }

            cout << "Loi: So tien phai la mot so khong am.\n";

            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
        }
    }

public:
    // Khoi tao mac dinh.
    HoaDonCuoc()
        : LopCoSo(),
        maHopDong(""),
        thangCuoc(""),
        soTien(0),
        ngayLap(""),
        ngayThanhToan(""),
        trangThai("Chua thanh toan") {
    }

    // GETTER
    string getMaHopDong() const {
        return maHopDong;
    }

    string getThangCuoc() const {
        return thangCuoc;
    }

    double getSoTien() const {
        return soTien;
    }

    string getNgayLap() const {
        return ngayLap;
    }

    string getNgayThanhToan() const {
        return ngayThanhToan;
    }

    string getTrangThai() const {
        return trangThai;
    }

    // SETTER ma hop dong co kiem tra ton tai.
    bool setMaHopDong(const string& ma) {
        string maChuanHoa = chuanHoaMa(ma);

        if (!chuoiHopLe(maChuanHoa) ||
            !kiemTraHopDongTonTai(maChuanHoa)) {
            return false;
        }

        maHopDong = maChuanHoa;
        return true;
    }

    // IN TIÊU ĐỀ BẢNG CHUẨN
    static void inTieuDe() {
        cout << left
            << setw(12) << "Ma HD" << " | "
            << setw(12) << "Ma hop dong" << " | "
            << setw(12) << "Thang cuoc" << " | "
            << setw(14) << "So tien" << " | "
            << setw(12) << "Ngay lap" << " | "
            << setw(15) << "Ngay thanh toan" << " | "
            << setw(16) << "Trang thai" << '\n'
            << string(105, '-') << '\n';
    }

    // NHAP THONG TIN HOA DON.
    void nhapThongTin() override {
        while (true) {
            cout << "Nhap ma hoa don: ";
            getline(cin >> ws, maDinhDanh);

            maDinhDanh = chuanHoaMa(maDinhDanh);

            if (!maHoaDonHopLe(maDinhDanh)) {
                cout << "Loi: Ma hoa don khong duoc rong "
                    << "hoac chua ky tu '|'.\n";
                continue;
            }

            break;
        }

        maHopDong = nhapMaHopDong();

        while (true) {
            cout << "Nhap thang cuoc (MM/YYYY): ";
            getline(cin, thangCuoc);

            if (kiemTraThangCuoc(thangCuoc)) {
                break;
            }

            cout << "Loi: Thang cuoc khong hop le.\n";
        }

        soTien = nhapSoTien();

        ngayLap = nhapNgay("Nhap ngay lap (DD/MM/YYYY): ");

        while (true) {
            cout << "Nhap ngay thanh toan "
                << "(DD/MM/YYYY, bo trong neu chua thanh toan): ";
            getline(cin, ngayThanhToan);

            if (ngayThanhToan.empty() ||
                kiemTraNgay(ngayThanhToan)) {
                break;
            }

            cout << "Loi: Ngay thanh toan khong hop le.\n";
        }

        while (true) {
            cout << "Trang thai "
                << "(Chua thanh toan/Da thanh toan/Qua han): ";
            getline(cin, trangThai);

            if (kiemTraTrangThai(trangThai)) {
                break;
            }

            cout << "Loi: Trang thai khong hop le.\n";
        }
    }

    // CAP NHAT THONG TIN HOA DON.
    void capNhatThongTin() override {
        string input;

        cout << "\n--- CAP NHAT HOA DON CUOC ---\n";

        while (true) {
            cout << "Ma hop dong hien tai: " << maHopDong
                << "\nNhap ma hop dong moi "
                << "(Enter de giu nguyen): ";
            getline(cin, input);

            if (input.empty()) {
                break;
            }

            input = chuanHoaMa(input);

            if (!chuoiHopLe(input)) {
                cout << "Loi: Ma hop dong khong hop le.\n";
                continue;
            }

            if (!kiemTraHopDongTonTai(input)) {
                cout << "Loi: Ma hop dong [" << input
                    << "] khong ton tai trong data/hop_dong.txt.\n";
                continue;
            }

            maHopDong = input;
            break;
        }

        while (true) {
            cout << "Thang cuoc hien tai: " << thangCuoc
                << "\nNhap thang cuoc moi "
                << "(Enter de giu nguyen): ";
            getline(cin, input);

            if (input.empty()) {
                break;
            }

            if (kiemTraThangCuoc(input)) {
                thangCuoc = input;
                break;
            }

            cout << "Loi: Thang cuoc khong hop le.\n";
        }

        while (true) {
            cout << "So tien hien tai: " << soTien
                << "\nNhap so tien moi "
                << "(-1 de giu nguyen): ";

            double tienMoi;

            if (cin >> tienMoi && isfinite(tienMoi) &&
                (tienMoi == -1 || tienMoi >= 0)) {
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n'
                );

                if (tienMoi >= 0) {
                    soTien = tienMoi;
                }

                break;
            }

            cout << "Loi: So tien khong hop le.\n";
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
        }

        while (true) {
            cout << "Ngay lap hien tai: " << ngayLap
                << "\nNhap ngay lap moi "
                << "(Enter de giu nguyen): ";
            getline(cin, input);

            if (input.empty()) {
                break;
            }

            if (kiemTraNgay(input)) {
                ngayLap = input;
                break;
            }

            cout << "Loi: Ngay lap khong hop le.\n";
        }

        while (true) {
            cout << "Ngay thanh toan hien tai: "
                << (ngayThanhToan.empty()
                    ? "Chua thanh toan" : ngayThanhToan)
                << "\nNhap ngay thanh toan moi "
                << "(Enter de giu nguyen, '-' de xoa): ";
            getline(cin, input);

            if (input.empty()) {
                break;
            }

            if (input == "-") {
                ngayThanhToan = "";
                break;
            }

            if (kiemTraNgay(input)) {
                ngayThanhToan = input;
                break;
            }

            cout << "Loi: Ngay thanh toan khong hop le.\n";
        }

        while (true) {
            cout << "Trang thai hien tai: " << trangThai
                << "\nNhap trang thai moi "
                << "(Enter de giu nguyen): ";
            getline(cin, input);

            if (input.empty()) {
                break;
            }

            if (kiemTraTrangThai(input)) {
                trangThai = input;
                break;
            }

            cout << "Loi: Trang thai khong hop le.\n";
        }

        cout << " -> Da cap nhat thong tin hoa don.\n";
    }

    // HIỂN THỊ THÔNG TIN 1 DÒNG DẠNG BẢNG KHỚP VỚI inTieuDe()
    void hienThiThongTin() const override {
        ostringstream ssTien;
        ssTien << fixed << setprecision(0) << soTien;

        cout << left
            << setw(12) << cat(maDinhDanh, 11) << " | "
            << setw(12) << cat(maHopDong, 11) << " | "
            << setw(12) << cat(thangCuoc, 11) << " | "
            << setw(14) << cat(ssTien.str(), 13) << " | "
            << setw(12) << cat(ngayLap, 11) << " | "
            << setw(15) << cat(ngayThanhToan.empty() ? "Chua TT" : ngayThanhToan, 14) << " | "
            << setw(16) << cat(trangThai, 15) << '\n';
    }

    // CHUYEN DU LIEU THANH CHUOI DE LUU FILE.
    string chuyenThanhChuoi() const override {
        stringstream ss;

        ss << maDinhDanh << '|'
            << maHopDong << '|'
            << thangCuoc << '|'
            << setprecision(15) << soTien << '|'
            << ngayLap << '|'
            << ngayThanhToan << '|'
            << trangThai;

        return ss.str();
    }

    // DOC DU LIEU TU CHUOI (Đã sửa lại dùng throw thay vì return ngầm)
    void docTuChuoi(const string& dongDuLieu) override {
        stringstream ss(dongDuLieu);

        string maHoaDon;
        string maHD;
        string thang;
        string tien;
        string lap;
        string thanhToan;
        string tt;
        string duThua;

        if (!getline(ss, maHoaDon, '|') ||
            !getline(ss, maHD, '|') ||
            !getline(ss, thang, '|') ||
            !getline(ss, tien, '|') ||
            !getline(ss, lap, '|') ||
            !getline(ss, thanhToan, '|') ||
            !getline(ss, tt)) {
            throw runtime_error("Dong du lieu HoaDonCuoc thieu cot (can 7 cot).");
        }

        if (getline(ss, duThua, '|')) {
            throw runtime_error("Dong du lieu HoaDonCuoc thua cot.");
        }

        double tienDoc;
        try {
            size_t viTri = 0;
            tienDoc = stod(tien, &viTri);

            if (viTri != tien.size() ||
                !isfinite(tienDoc) ||
                tienDoc < 0) {
                throw runtime_error("So tien trong HoaDonCuoc khong hop le.");
            }
        }
        catch (...) {
            throw runtime_error("Loi phan tich so tien trong HoaDonCuoc.");
        }

        maHoaDon = chuanHoaMa(maHoaDon);
        maHD = chuanHoaMa(maHD);

        if (!maHoaDonHopLe(maHoaDon)) {
            throw runtime_error("Ma hoa don khong hop le: " + maHoaDon);
        }
        if (!chuoiHopLe(maHD)) {
            throw runtime_error("Ma hop dong trong hoa don khong hop le.");
        }
        if (!kiemTraThangCuoc(thang)) {
            throw runtime_error("Thang cuoc khong hop le: " + thang);
        }
        if (!kiemTraNgay(lap)) {
            throw runtime_error("Ngay lap hoa don khong hop le: " + lap);
        }
        if (!thanhToan.empty() && !kiemTraNgay(thanhToan)) {
            throw runtime_error("Ngay thanh toan khong hop le: " + thanhToan);
        }
        if (!kiemTraTrangThai(tt)) {
            throw runtime_error("Trang thai hoa don khong hop le: " + tt);
        }

        maDinhDanh = maHoaDon;
        maHopDong = maHD;
        thangCuoc = thang;
        soTien = tienDoc;
        ngayLap = lap;
        ngayThanhToan = thanhToan;
        trangThai = tt;
    }
};

#endif // HOA_DON_CUOC_H
