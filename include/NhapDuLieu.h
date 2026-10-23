/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp tiện ích tĩnh (Static Utility Class) xử lý
 * toàn bộ các luồng nhập liệu, kiểm tra định dạng và bắt ngoại lệ.
 *
 * - Hàm nhapXxx: bắt buộc nhập, sai thì báo lỗi và cho nhập lại.
 * - Hàm nhapXxxHoacGiuNguyen: dùng cho chức năng Cập nhật; để trống
 *   (chỉ nhấn Enter) nghĩa là giữ nguyên giá trị cũ.
 * - Khi hết dữ liệu nhập (EOF: Ctrl+D / Ctrl+Z) các hàm nhập ném
 *   std::runtime_error thay vì lặp vô hạn. Hàm main nên bắt ngoại lệ này.
 * ======================================================= */

#ifndef NHAP_DU_LIEU_H
#define NHAP_DU_LIEU_H

#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <cfloat>
#include <ctime>
#include <sstream>

class NhapDuLieu {
public:
    NhapDuLieu() = delete; // Lớp tiện ích tĩnh, không cho tạo đối tượng

    // ================= CHUỖI =================

    // Cắt khoảng trắng 2 đầu, cấm trống, cấm ký tự điều khiển và '|'.
    // toiDa = 0 nghĩa là không giới hạn độ dài.
    static std::string nhapChuoi(const std::string& thongBao, size_t toiDa = 0) {
        return docGiaTri<std::string>(thongBao, false, "",
            [toiDa](const std::string& s, std::string& kq, std::string& loi) {
                return phanTichChuoi(s, toiDa, kq, loi);
            });
    }

    static std::string nhapChuoiHoacGiuNguyen(const std::string& thongBao,
                                              const std::string& giaTriCu, size_t toiDa = 0) {
        return docGiaTri<std::string>(thongBao, true, giaTriCu,
            [toiDa](const std::string& s, std::string& kq, std::string& loi) {
                return phanTichChuoi(s, toiDa, kq, loi);
            });
    }

    // Mã định danh: chỉ gồm chữ cái không dấu, chữ số, '_' hoặc '-'
    // (không khoảng trắng, không bắt đầu bằng '#'), tự viết hoa (" ont01" -> "ONT01")
    static std::string nhapMaDinhDanh(const std::string& thongBao) {
        return docGiaTri<std::string>(thongBao, false, "",
            [](const std::string& s, std::string& kq, std::string& loi) {
                return phanTichMa(s, kq, loi);
            });
    }

    // Mã theo mẫu "tiền tố + N chữ số", ví dụ nhapMaTheoMau("Ma sach: ", "S", 3) -> "S001"
    static std::string nhapMaTheoMau(const std::string& thongBao,
                                     const std::string& tienTo, size_t soChuSo) {
        return docGiaTri<std::string>(thongBao, false, "",
            [tienTo, soChuSo](const std::string& s, std::string& kq, std::string& loi) {
                std::string ma;
                if (!phanTichMa(s, ma, loi)) return false;
                bool dung = ma.size() == tienTo.size() + soChuSo &&
                            ma.compare(0, tienTo.size(), tienTo) == 0;
                for (size_t i = tienTo.size(); dung && i < ma.size(); i++)
                    dung = std::isdigit(static_cast<unsigned char>(ma[i])) != 0;
                if (!dung) {
                    loi = "Ma phai co dang " + tienTo + std::string(soChuSo, 'x') +
                          " (x la chu so)!";
                    return false;
                }
                kq = ma;
                return true;
            });
    }

    // ================= SỐ NGUYÊN =================

    static int nhapSoNguyenTrongKhoang(const std::string& thongBao, int nhoNhat, int lonNhat) {
        return docGiaTri<int>(thongBao, false, 0,
            [nhoNhat, lonNhat](const std::string& s, int& kq, std::string& loi) {
                return phanTichSoNguyen(s, nhoNhat, lonNhat, kq, loi);
            });
    }

    static int nhapSoNguyenHoacGiuNguyen(const std::string& thongBao, int giaTriCu,
                                         int nhoNhat, int lonNhat) {
        return docGiaTri<int>(thongBao, true, giaTriCu,
            [nhoNhat, lonNhat](const std::string& s, int& kq, std::string& loi) {
                return phanTichSoNguyen(s, nhoNhat, lonNhat, kq, loi);
            });
    }

    // Số nguyên không âm (>= 0)
    static int nhapSoNguyenDuong(const std::string& thongBao) {
        return nhapSoNguyenTrongKhoang(thongBao, 0, std::numeric_limits<int>::max());
    }

    // ================= SỐ THỰC =================

    // Số thực trong [nhoNhat, lonNhat]; chanDuoiBaoGom = false nghĩa là phải > nhoNhat
    static double nhapSoThuc(const std::string& thongBao, double nhoNhat, double lonNhat,
                             bool chanDuoiBaoGom = true) {
        return docGiaTri<double>(thongBao, false, 0.0,
            [=](const std::string& s, double& kq, std::string& loi) {
                return phanTichSoThuc(s, nhoNhat, lonNhat, chanDuoiBaoGom, kq, loi);
            });
    }

    static double nhapSoThucHoacGiuNguyen(const std::string& thongBao, double giaTriCu,
                                          double nhoNhat, double lonNhat,
                                          bool chanDuoiBaoGom = true) {
        return docGiaTri<double>(thongBao, true, giaTriCu,
            [=](const std::string& s, double& kq, std::string& loi) {
                return phanTichSoThuc(s, nhoNhat, lonNhat, chanDuoiBaoGom, kq, loi);
            });
    }

    // Số thực >= 0
    static double nhapSoThucDuong(const std::string& thongBao) {
        return nhapSoThuc(thongBao, 0.0, DBL_MAX, true);
    }

    // Số thực > 0 (vd: giá bìa "Lớn hơn 0")
    static double nhapSoThucLonHonKhong(const std::string& thongBao) {
        return nhapSoThuc(thongBao, 0.0, DBL_MAX, false);
    }

    // ================= SỐ ĐIỆN THOẠI =================

    // Đúng 10 chữ số, bắt đầu bằng '0'
    static std::string nhapSoDienThoai(const std::string& thongBao) {
        return docGiaTri<std::string>(thongBao, false, "",
            [](const std::string& s, std::string& kq, std::string& loi) {
                return phanTichSoDienThoai(s, kq, loi);
            });
    }

    static std::string nhapSoDienThoaiHoacGiuNguyen(const std::string& thongBao,
                                                    const std::string& giaTriCu) {
        return docGiaTri<std::string>(thongBao, true, giaTriCu,
            [](const std::string& s, std::string& kq, std::string& loi) {
                return phanTichSoDienThoai(s, kq, loi);
            });
    }

    // ================= NGÀY THÁNG =================

    static bool laNamNhuan(int nam) {
        return (nam % 400 == 0) || (nam % 4 == 0 && nam % 100 != 0);
    }

    // Kiểm tra ngày/tháng/năm hợp lệ (năm 1900..2100, xử lý tháng 2 năm nhuận)
    static bool ngayHopLe(int ngay, int thang, int nam) {
        if (nam < 1900 || nam > 2100) return false;
        if (thang < 1 || thang > 12) return false;

        int soNgayTrongThang[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (thang == 2 && laNamNhuan(nam)) {
            soNgayTrongThang[1] = 29;
        }
        return ngay >= 1 && ngay <= soNgayTrongThang[thang - 1];
    }

    // Kiểm tra chuỗi có đúng dạng DD/MM/YYYY và là ngày thật hay không
    static bool laNgayThangHopLe(const std::string& s) {
        int d, m, y;
        return tachNgay(s, d, m, y);
    }

    // Ngày hiện tại của máy, dạng DD/MM/YYYY
    static std::string ngayHienTai() {
        std::time_t t = std::time(nullptr);
        std::tm lt{};
#ifdef _WIN32
        localtime_s(&lt, &t);
#else
        localtime_r(&t, &lt);
#endif
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%02d/%02d/%04d",
                      lt.tm_mday, lt.tm_mon + 1, lt.tm_year + 1900);
        return buf;
    }

    // So sánh 2 ngày DD/MM/YYYY hợp lệ: <0 nếu a trước b, 0 nếu bằng, >0 nếu a sau b
    static int soSanhNgay(const std::string& a, const std::string& b) {
        int d1 = 0, m1 = 0, y1 = 0, d2 = 0, m2 = 0, y2 = 0;
        if (!tachNgay(a, d1, m1, y1) || !tachNgay(b, d2, m2, y2))
            throw std::invalid_argument("soSanhNgay: ngay khong hop le!");
        long ka = y1 * 10000L + m1 * 100 + d1;
        long kb = y2 * 10000L + m2 * 100 + d2;
        return (ka > kb) - (ka < kb);
    }

    // Nhập ngày DD/MM/YYYY; khongSauHienTai = true thì cấm ngày ở tương lai
    static std::string nhapNgayThang(const std::string& thongBao, bool khongSauHienTai = false) {
        return docGiaTri<std::string>(thongBao, false, "",
            [khongSauHienTai](const std::string& s, std::string& kq, std::string& loi) {
                return phanTichNgay(s, khongSauHienTai, kq, loi);
            });
    }

    static std::string nhapNgayThangHoacGiuNguyen(const std::string& thongBao,
                                                  const std::string& giaTriCu,
                                                  bool khongSauHienTai = false) {
        return docGiaTri<std::string>(thongBao, true, giaTriCu,
            [khongSauHienTai](const std::string& s, std::string& kq, std::string& loi) {
                return phanTichNgay(s, khongSauHienTai, kq, loi);
            });
    }

    // ================= XÁC NHẬN & TIỆN ÍCH =================

    // Xác nhận (y/n) cho thao tác quan trọng như Xóa, Thoát
    static bool xacNhan(const std::string& thongBao) {
        while (true) {
            std::string luaChon = catKhoangTrang(docDong(thongBao + " (y/n): "));
            if (luaChon == "y" || luaChon == "Y") return true;
            if (luaChon == "n" || luaChon == "N") return false;
            std::cout << " -> Vui long chi nhap 'y' (Dong y) hoac 'n' (Huy bo).\n";
        }
    }

    // Chuỗi chữ thường (ASCII) - dùng cho tìm kiếm không phân biệt hoa thường
    static std::string chuyenChuThuong(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return s;
    }

    // Tìm gần đúng: chuỗi "chuaTrong" có chứa "tuKhoa" không (không phân biệt hoa thường)
    static bool chuaChuoi(const std::string& chuaTrong, const std::string& tuKhoa) {
        return chuyenChuThuong(chuaTrong).find(chuyenChuThuong(tuKhoa)) != std::string::npos;
    }

private:
    static const char* dsKhoangTrang() { return " \t\r\n\v\f"; }

    // In thông báo rồi đọc nguyên một dòng; EOF/lỗi luồng -> ném ngoại lệ
    static std::string docDong(const std::string& thongBao) {
        std::cout << thongBao;
        std::string dong;
        if (!std::getline(std::cin, dong)) {
            throw std::runtime_error("Het du lieu nhap (EOF)!");
        }
        return dong;
    }

    static std::string catKhoangTrang(const std::string& s) {
        size_t dau = s.find_first_not_of(dsKhoangTrang());
        if (dau == std::string::npos) return "";
        size_t cuoi = s.find_last_not_of(dsKhoangTrang());
        return s.substr(dau, cuoi - dau + 1);
    }

    // Khung chung: đọc dòng -> (trống: giữ nguyên hoặc báo lỗi) -> phân tích -> lặp đến khi hợp lệ
    template <typename T, typename Parse>
    static T docGiaTri(const std::string& thongBao, bool choPhepTrong,
                       const T& giaTriCu, Parse parse) {
        while (true) {
            std::string s = catKhoangTrang(docDong(thongBao));
            if (s.empty()) {
                if (choPhepTrong) return giaTriCu;
                std::cout << " -> Loi: Thong tin khong duoc de trong!\n";
                continue;
            }
            T kq{};
            std::string loi = "Gia tri khong hop le!";
            if (parse(s, kq, loi)) return kq;
            std::cout << " -> Loi: " << loi << "\n";
        }
    }

    static bool phanTichChuoi(const std::string& s, size_t toiDa,
                              std::string& kq, std::string& loi) {
        for (char c : s) {
            if (std::iscntrl(static_cast<unsigned char>(c))) {
                loi = "Thong tin khong duoc chua ky tu dieu khien (vd: Tab)!";
                return false;
            }
        }
        if (s.find('|') != std::string::npos) {
            loi = "Thong tin khong duoc chua ky tu '|'!";
            return false;
        }
        if (toiDa > 0 && s.size() > toiDa) {
            loi = "Thong tin toi da " + std::to_string(toiDa) + " ky tu!";
            return false;
        }
        kq = s;
        return true;
    }

    static bool phanTichMa(const std::string& s, std::string& kq, std::string& loi) {
        std::string tam;
        if (!phanTichChuoi(s, 0, tam, loi)) return false;
        for (char c : tam) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(std::isalnum(uc) && uc < 128) && c != '_' && c != '-') {
                loi = "Ma chi gom chu cai khong dau, chu so, '_' hoac '-' (khong co khoang trang)!";
                return false;
            }
        }
        std::transform(tam.begin(), tam.end(), tam.begin(),
            [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        kq = tam;
        return true;
    }

    // Toàn bộ chuỗi phải là số nguyên, không rác phía sau, không tràn int
    static bool phanTichSoNguyen(const std::string& s, int nhoNhat, int lonNhat,
                                 int& kq, std::string& loi) {
        char* ketThuc = nullptr;
        errno = 0;
        long long v = std::strtoll(s.c_str(), &ketThuc, 10);
        if (errno == 0 && *ketThuc == '\0' && v >= nhoNhat && v <= lonNhat) {
            kq = static_cast<int>(v);
            return true;
        }
        if (lonNhat == std::numeric_limits<int>::max())
            loi = "Gia tri phai la so nguyen >= " + std::to_string(nhoNhat) + "!";
        else
            loi = "Gia tri phai la so nguyen trong khoang [" + std::to_string(nhoNhat) +
                  ", " + std::to_string(lonNhat) + "]!";
        return false;
    }

    static bool phanTichSoThuc(const std::string& s, double nhoNhat, double lonNhat,
                               bool chanDuoiBaoGom, double& kq, std::string& loi) {
        if (s.find_first_not_of("0123456789.eE+-") == std::string::npos) {
            char* ketThuc = nullptr;
            errno = 0;
            double v = std::strtod(s.c_str(), &ketThuc);
            bool trongKhoang = v <= lonNhat && (chanDuoiBaoGom ? v >= nhoNhat : v > nhoNhat);
            if (errno == 0 && *ketThuc == '\0' && std::isfinite(v) && trongKhoang) {
                kq = (v == 0.0) ? 0.0 : v; // tránh "-0"
                return true;
            }
        }
        loi = std::string("Gia tri phai la so thuc ") + (chanDuoiBaoGom ? ">= " : "> ") +
              soThanhChuoi(nhoNhat);
        if (lonNhat < DBL_MAX) loi += " va <= " + soThanhChuoi(lonNhat);
        loi += "!";
        return false;
    }

    static std::string soThanhChuoi(double v) {
        std::ostringstream os;
        os << v;
        return os.str();
    }

    static bool phanTichSoDienThoai(const std::string& s, std::string& kq, std::string& loi) {
        std::string tam;
        if (!phanTichChuoi(s, 0, tam, loi)) return false;
        bool dung = tam.size() == 10 && tam[0] == '0';
        for (char c : tam) {
            if (!std::isdigit(static_cast<unsigned char>(c))) { dung = false; break; }
        }
        if (!dung) {
            loi = "So dien thoai phai co 10 chu so va bat dau bang '0'!";
            return false;
        }
        kq = tam;
        return true;
    }

    // Tách "DD/MM/YYYY" -> d, m, y; false nếu sai dạng hoặc ngày không có thật
    static bool tachNgay(const std::string& s, int& d, int& m, int& y) {
        if (s.size() != 10 || s[2] != '/' || s[5] != '/') return false;
        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) continue;
            if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
        }
        d = (s[0] - '0') * 10 + (s[1] - '0');
        m = (s[3] - '0') * 10 + (s[4] - '0');
        y = (s[6] - '0') * 1000 + (s[7] - '0') * 100 + (s[8] - '0') * 10 + (s[9] - '0');
        return ngayHopLe(d, m, y);
    }

    static bool phanTichNgay(const std::string& s, bool khongSauHienTai,
                             std::string& kq, std::string& loi) {
        int d, m, y;
        if (!tachNgay(s, d, m, y)) {
            loi = "Ngay khong hop le! Dung dinh dang DD/MM/YYYY (VD: 15/03/2026).";
            return false;
        }
        if (khongSauHienTai && soSanhNgay(s, ngayHienTai()) > 0) {
            loi = "Ngay khong duoc sau ngay hien tai (" + ngayHienTai() + ")!";
            return false;
        }
        kq = s;
        return true;
    }
};

#endif
