/* =======================================================
 * Tên tác giả: Hoàng Trọng Khoa
 * Mã sinh viên: B24DCVT204
 * Mô tả file: Lớp quản lý Hợp Đồng
 * ======================================================= */

#ifndef HOP_DONG_H
#define HOP_DONG_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// maDinhDanh chính là maHopDong. Giữ nguyên thứ tự cột vì UC01 đọc cột 2 (maGoi):
//   maHD|maKH|maGoi|maTB|ngayKy|ngayHetHan|giaThieuDung|trangThai
// F_*: đường dẫn CSDL của các module liên quan, dùng để kiểm tra khóa ngoại
class HopDong : public LopCoSo {
private:
    inline static const std::string F_HD = "data/hop_dong.txt", F_KH = "data/khach_hang.txt",
                                    F_GOI = "data/goi_internet.txt", F_TB = "data/thiet_bi.txt";
    std::string maKhachHang, maGoiCuoc, maThietBi, ngayKy, ngayHetHan;
    std::string trangThai = "Hoat dong"; // Mặc định hợp đồng mới
    double giaThieuDung = 0;

    static bool hopLeMa(const std::string& s) { // khớp NhapDuLieu::nhapMaDinhDanh
        return !s.empty() && std::none_of(s.begin(), s.end(), [](unsigned char c) {
            return c == '|' || std::isspace(c) || std::islower(c); });
    }
    static bool hopLeTT(const std::string& t) { return t == "Hoat dong" || t == "Het han" || t == "Huy"; }
    // Số tiền đọc từ file: chỉ chữ số và tối đa một dấu '.' (không dấu, không số mũ như 1e5)
    static bool hopLeSoTien(const std::string& s) {
        return s != "." && !s.empty() && std::count(s.begin(), s.end(), '.') <= 1 &&
               std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c) || c == '.'; });
    }
    static int ngayThanhSo(const std::string& s) { // "DD/MM/YYYY" -> YYYYMMDD để so sánh
        if (s.size() != 10 || s[2] != '/' || s[5] != '/') return -1;
        for (int i = 0; i < 10; ++i)
            if (i != 2 && i != 5 && !std::isdigit(static_cast<unsigned char>(s[i]))) return -1;
        int d = std::stoi(s.substr(0, 2)), m = std::stoi(s.substr(3, 2)), y = std::stoi(s.substr(6, 4));
        return NhapDuLieu::ngayHopLe(d, m, y) ? y * 10000 + m * 100 + d : -1;
    }
    static void yeuCau(bool ok, const std::string& msg) { if (!ok) throw std::invalid_argument(msg); }

    // Cắt khoảng trắng hai đầu + viết hoa (như NhapDuLieu::nhapMaDinhDanh)
    static std::string chuanHoaMa(std::string s) {
        std::size_t d = s.find_first_not_of(" \t\r\n"), c = s.find_last_not_of(" \t\r\n");
        s = d == std::string::npos ? "" : s.substr(d, c - d + 1);
        for (char& ch : s) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        return s;
    }
    // Duyệt file CSDL (bỏ dòng trống và '#'); chỉ đọc, KHÔNG tạo file. Trả về true nếu có dòng mà
    // khóa chính (cột 0) trùng ma (đã chuẩn hóa); ma rỗng nghĩa là chỉ hỏi "file có dữ liệu không".
    static bool coBanGhi(const std::string& path, const std::string& ma = "") {
        std::ifstream file(path);
        for (std::string dong; std::getline(file, dong);) {
            if (chuanHoaMa(dong).empty() || dong[0] == '#') continue;
            if (ma.empty() || chuanHoaMa(dong.substr(0, dong.find('|'))) == ma) return true;
        }
        return false;
    }
    // Khóa ngoại: nhập lại đến khi mã tồn tại trong file của module tương ứng
    static std::string nhapMaThamChieu(const std::string& loiNhac, const std::string& path, const std::string& ten) {
        while (true) {
            std::string ma = NhapDuLieu::nhapMaDinhDanh(loiNhac);
            if (coBanGhi(path, ma)) return ma;
            std::cout << " -> Loi: Ma " << ten << " [" << ma << "] khong ton tai trong " << path << "!\n";
        }
    }
    // Dùng chung cho thêm mới và cập nhật (không đổi mã). Nhập vào biến tạm, xong mới gán một lần
    // để không để lại đối tượng "nửa cũ nửa mới". Trả về false nếu thiếu dữ liệu nền.
    bool nhapNoiDung(bool nhapMa) {
        for (const std::string* f : {&F_KH, &F_GOI, &F_TB}) // thiếu dữ liệu nền thì không thể nhập mã khóa ngoại
            if (!coBanGhi(*f)) {
                std::cout << " -> Loi: Chua co du lieu trong " << *f << ". Hay them truoc khi lap hop dong!\n";
                return false;
            }
        std::string ma = maDinhDanh, het, tt;
        while (nhapMa) { // khóa chính không được trùng
            ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong (maHD): ");
            if (!coBanGhi(F_HD, ma)) break;
            std::cout << " -> Loi: Ma hop dong [" << ma << "] DA TON TAI trong he thong!\n";
        }
        std::string kh = nhapMaThamChieu("Nhap ma khach hang: ", F_KH, "khach hang");
        std::string goi = nhapMaThamChieu("Nhap ma goi cuoc: ", F_GOI, "goi cuoc");
        std::string tb = nhapMaThamChieu("Nhap ma thiet bi: ", F_TB, "thiet bi");
        std::string ky = NhapDuLieu::nhapNgayThang("Nhap ngay ky hop dong (DD/MM/YYYY): ");
        while (true) { // ngày hết hạn không được trước ngày ký
            het = NhapDuLieu::nhapNgayThang("Nhap ngay het han hop dong (DD/MM/YYYY): ");
            if (ngayThanhSo(het) >= ngayThanhSo(ky)) break;
            std::cout << " -> Loi: Ngay het han phai sau hoac bang ngay ky!\n";
        }
        double gia = NhapDuLieu::nhapSoThucDuong("Nhap gia thieu dung hang thang (>= 0): ");
        while (true) {
            tt = NhapDuLieu::nhapChuoi("Nhap trang thai (Hoat dong/Het han/Huy): ");
            if (hopLeTT(tt)) break;
            std::cout << " -> Loi: Trang thai khong hop le!\n";
        }
        maDinhDanh = ma; maKhachHang = kh; maGoiCuoc = goi; maThietBi = tb;
        ngayKy = ky; ngayHetHan = het; giaThieuDung = gia; trangThai = tt;
        return true;
    }

public:
    HopDong() = default;
    std::string getMaHopDong() const { return maDinhDanh; }
    const std::string& getMaKhachHang() const { return maKhachHang; }
    const std::string& getMaGoiCuoc() const { return maGoiCuoc; }
    const std::string& getMaThietBi() const { return maThietBi; }
    const std::string& getNgayKy() const { return ngayKy; }
    const std::string& getNgayHetHan() const { return ngayHetHan; }
    const std::string& getTrangThai() const { return trangThai; }
    double getGiaThieuDung() const { return giaThieuDung; }
    bool hopLe() const { return !maDinhDanh.empty(); } // false nếu nhapThongTin() bị bỏ (thiếu dữ liệu nền)

    void setTrangThai(const std::string& tt) {
        yeuCau(hopLeTT(tt), "Trang thai hop dong khong hop le: " + tt);
        trangThai = tt;
    }
    void nhapThongTin() override { nhapNoiDung(true); }
    void capNhatThongTin() override { nhapNoiDung(false); }
    void hienThiThongTin() const override {
        std::ostringstream gia; // định dạng trên chuỗi tạm, không đổi cờ của std::cout
        gia << std::fixed << std::setprecision(0) << giaThieuDung;
        std::cout << "Ma HD: " << maDinhDanh << " | Ma KH: " << maKhachHang
                  << " | Ma Goi: " << maGoiCuoc << " | Ma TB: " << maThietBi
                  << " | Ngay ky: " << ngayKy << " | Ngay het han: " << ngayHetHan
                  << " | Gia thieu dung: " << gia.str() << " VND | Trang thai: " << trangThai << "\n";
    }

    // Tiền lưu cố định 2 chữ số thập phân (như HoaDonCuoc) để không mất dữ liệu
    std::string chuyenThanhChuoi() const override {
        std::ostringstream os;
        os << maDinhDanh << '|' << maKhachHang << '|' << maGoiCuoc << '|' << maThietBi << '|'
           << ngayKy << '|' << ngayHetHan << '|' << std::fixed << std::setprecision(2) << giaThieuDung
           << '|' << trangThai;
        return os.str();
    }

    // Kiểm tra toàn vẹn rồi mới gán; ném invalid_argument với thông báo cụ thể
    void docTuChuoi(const std::string& dong) override {
        yeuCau(std::count(dong.begin(), dong.end(), '|') == 7, "Dong du lieu HopDong co so cot khong dung.");
        std::string f[8];
        std::stringstream ss(dong);
        for (auto& x : f) std::getline(ss, x, '|');
        if (!f[7].empty() && f[7].back() == '\r') f[7].pop_back(); // file tạo từ Windows

        const char* ten[] = {"hop dong", "khach hang", "goi cuoc", "thiet bi"};
        for (int i = 0; i < 4; ++i)
            yeuCau(hopLeMa(f[i]), std::string("Ma ") + ten[i] + " khong hop le: " + f[i]);
        int soKy = ngayThanhSo(f[4]), soHet = ngayThanhSo(f[5]);
        yeuCau(soKy >= 0 && soHet >= soKy, "Ngay ky/het han khong hop le (het han phai >= ky): " + f[4] + " -> " + f[5]);
        yeuCau(hopLeSoTien(f[6]), "Gia thieu dung khong hop le: " + f[6]);
        yeuCau(hopLeTT(f[7]), "Trang thai hop dong khong hop le: " + f[7]);
        double gia = 0;
        try { gia = std::stod(f[6]); }
        catch (const std::exception&) { yeuCau(false, "Gia thieu dung vuot gioi han: " + f[6]); }

        maDinhDanh = f[0]; maKhachHang = f[1]; maGoiCuoc = f[2]; maThietBi = f[3];
        ngayKy = f[4]; ngayHetHan = f[5]; giaThieuDung = gia; trangThai = f[7];
    }
};

#endif

