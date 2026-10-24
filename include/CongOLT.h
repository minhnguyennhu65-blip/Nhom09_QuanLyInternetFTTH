/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp quản lý Cổng OLT (Entity - UC06 - Single Header)
 * ======================================================= */

#ifndef CONG_OLT_H
#define CONG_OLT_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include "QuanLyLuuTru.h"
#include "KhachHang.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cctype>
#include <limits>
#include <functional>
#include <stdexcept>
#include <algorithm>

class CongOLT : public LopCoSo {
private:
    std::string tenOLT;
    int soCong;
    std::string khuVuc;
    std::string maTuyenCap;  // Khóa ngoại tới Tuyến cáp (UC02)
    std::string maKhachHang; // Khóa ngoại tới Khách hàng (rỗng nếu cổng không có khách)
    std::string trangThai;   // "Trong", "Dang su dung", "Hong"

    // Bộ quản lý Khách hàng dùng để kiểm tra khóa ngoại
    inline static QuanLyLuuTru<KhachHang>* dbKhachHang = nullptr;

    // Hàm kiểm tra mã tuyến cáp có tồn tại không (gắn từ main)
    inline static std::function<bool(const std::string&)> kiemTraTuyenCap = nullptr;

    // ---------- Hàm kiểm tra / tiện ích nội bộ ----------

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == "Trong" || tt == "Dang su dung" || tt == "Hong";
    }

    // Chuỗi dữ liệu hợp lệ: không chứa '|' (dấu phân cách file) và ký tự điều khiển
    static void kiemTraChuoi(const std::string& s, const std::string& ten) {
        if (s == "HUY" || s == "huy")
            throw std::invalid_argument("HUY");
        if (s.empty())
            throw std::invalid_argument(ten + " khong duoc rong!");
        if (s.find('|') != std::string::npos)
            throw std::invalid_argument(ten + " khong duoc chua ky tu '|'!");
        for (char c : s) {
            if (std::iscntrl(static_cast<unsigned char>(c)))
                throw std::invalid_argument(ten + " khong duoc chua ky tu dieu khien!");
        }
    }

    // Chuẩn hóa mã (khóa ngoại): chỉ chữ cái không dấu, số, '_' hoặc '-'; viết hoa.
    static std::string chuanHoaMa(const std::string& ma, const std::string& ten) {
        if (ma == "HUY" || ma == "huy")
            throw std::invalid_argument("HUY");
        if (ma.empty())
            throw std::invalid_argument(ten + " khong duoc rong!");
        std::string kq = ma;
        for (char& c : kq) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(std::isalnum(uc) && uc < 128) && c != '_' && c != '-')
                throw std::invalid_argument(
                    ten + " chi gom chu cai khong dau, chu so, '_' hoac '-'!");
            c = static_cast<char>(std::toupper(uc));
        }
        return kq;
    }

    // Tách theo '|', GIỮ lại cả trường rỗng ở giữa và ở cuối
    static std::vector<std::string> tachTruong(const std::string& s) {
        std::vector<std::string> kq;
        std::size_t dau = 0;
        while (true) {
            std::size_t p = s.find('|', dau);
            if (p == std::string::npos) {
                kq.push_back(s.substr(dau));
                break;
            }
            kq.push_back(s.substr(dau, p - dau));
            dau = p + 1;
        }
        return kq;
    }

    // Chuyển chuỗi thành số nguyên; sai định dạng thì ném invalid_argument
    static int chuyenSoCong(const std::string& s) {
        if (s == "HUY" || s == "huy")
            throw std::invalid_argument("HUY");
        try {
            std::size_t vt = 0;
            int v = std::stoi(s, &vt);
            if (vt != s.size()) throw std::invalid_argument("");
            return v;
        }
        catch (const std::exception&) {
            throw std::invalid_argument("So cong khong hop le!");
        }
    }

    // Cắt chuỗi dài để không làm lệch cột khi in bảng
    static std::string cat(const std::string& s, std::size_t n) {
        return s.size() <= n ? s : s.substr(0, n - 2) + "..";
    }

    bool khachHangTonTai(const std::string& ma) const {
        if (dbKhachHang == nullptr) return false;
        return dbKhachHang->timTheoMa(ma) != nullptr;
    }

    // Lặp lại thao tác nhập cho đến khi setter không ném ngoại lệ, trả về false nếu gõ HUY
    template <typename F>
    static bool lapDenKhiHopLe(F thucHien) {
        while (true) {
            try {
                thucHien();
                return true;
            }
            catch (const std::invalid_argument& e) {
                std::string err = e.what();
                if (err == "HUY") {
                    return false;
                }
                std::cout << " -> Loi: " << err << '\n';
            }
        }
    }

    // Dùng cho cập nhật: bỏ trống = giữ nguyên, sai thì nhập lại
    template <typename F>
    static void capNhatTruong(const std::string& nhan, F setter) {
        while (true) {
            std::string s = NhapDuLieu::nhapChuoiHoacGiuNguyen(
                nhan + " (bo trong de giu nguyen, hoac nhap 'HUY'): ", "");
            if (s.empty()) return;
            if (s == "HUY" || s == "huy") return;
            try { setter(s); return; }
            catch (const std::invalid_argument& e) {
                std::string err = e.what();
                if (err == "HUY") return;
                std::cout << " -> Loi: " << err << '\n';
            }
        }
    }

    // Chọn trạng thái. Trả về false nếu người dùng bỏ trống hoặc hủy
    bool nhapTrangThai(bool choPhepBoTrong) {
        std::string thongBaoChon = choPhepBoTrong
            ? "Chon trang thai (bo trong de giu nguyen, hoac nhap 'HUY'): "
            : "Chon trang thai (hoac nhap 'HUY'): ";

        std::cout << "\nTrang thai cong:\n"
            << "1. Trong\n"
            << "2. Dang su dung\n"
            << "3. Hong\n";

        std::string s = NhapDuLieu::nhapChuoiHoacGiuNguyen(thongBaoChon, "");
        if (s.empty()) {
            if (choPhepBoTrong) return false; // bo trong = giu nguyen
            std::cout << " -> Loi: Lua chon khong hop le!\n";
            return nhapTrangThai(choPhepBoTrong);
        }
        if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");

        int chon = 0;
        try { chon = chuyenSoCong(s); }     // parse nghiem ngat, khong nhan "2abc"
        catch (const std::invalid_argument&) { chon = 0; }

        if (chon == 1) { setTrangThai("Trong"); return true; }
        if (chon == 3) { setTrangThai("Hong");  return true; }

        if (chon == 2) {
            bool thanhCong = lapDenKhiHopLe([&] {
                std::string maKH = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang (hoac 'HUY'): ");
                if (maKH == "HUY" || maKH == "huy") throw std::invalid_argument("HUY");
                setTrangThai("Dang su dung", maKH);
                });
            return thanhCong;
        }

        std::cout << " -> Loi: Lua chon khong hop le!\n";
        return nhapTrangThai(choPhepBoTrong);
    }

public:
    CongOLT() : LopCoSo(), soCong(1), trangThai("Trong") {}

    static void setDBKhachHang(QuanLyLuuTru<KhachHang>* db) {
        dbKhachHang = db;
    }

    static void setKiemTraTuyenCap(std::function<bool(const std::string&)> f) {
        kiemTraTuyenCap = std::move(f);
    }

    // Định dạng tiêu đề bảng chuẩn có dấu phân cách ' | '
    static void inTieuDe() {
        std::cout << std::left
            << std::setw(10) << "Ma cong" << " | "
            << std::setw(12) << "Ma tuyen" << " | "
            << std::setw(16) << "Ten OLT" << " | "
            << std::setw(8) << "So cong" << " | "
            << std::setw(14) << "Khu vuc" << " | "
            << std::setw(10) << "Ma KH" << " | "
            << std::setw(14) << "Trang thai" << '\n'
            << std::string(102, '-') << '\n';
    }

    void setTenOLT(const std::string& ten) {
        kiemTraChuoi(ten, "Ten OLT");
        tenOLT = ten;
    }

    void setSoCong(int so) {
        if (so < 1)
            throw std::invalid_argument("So cong phai lon hon 0!");
        soCong = so;
    }

    void setKhuVuc(const std::string& kv) {
        kiemTraChuoi(kv, "Khu vuc");
        khuVuc = kv;
    }

    void setMaTuyenCap(const std::string& ma) {
        if (ma == "HUY" || ma == "huy")
            throw std::invalid_argument("HUY");
        std::string chuan = chuanHoaMa(ma, "Ma tuyen cap");
        if (kiemTraTuyenCap && !kiemTraTuyenCap(chuan))
            throw std::invalid_argument("Tuyen cap khong ton tai!");
        maTuyenCap = chuan;
    }

    void setTrangThai(const std::string& tt, const std::string& maKH = "") {
        if (!hopLeTrangThai(tt))
            throw std::invalid_argument("Trang thai khong hop le!");
        if (tt == "Dang su dung") {
            std::string chuan = chuanHoaMa(maKH, "Ma khach hang");
            if (!khachHangTonTai(chuan))
                throw std::invalid_argument("Khach hang khong ton tai!");
            maKhachHang = chuan;
        }
        else {
            if (!maKH.empty())
                throw std::invalid_argument("Cong nay khong duoc gan khach hang!");
            maKhachHang.clear();
        }
        trangThai = tt;
    }

    void nhapThongTin() override {
        std::string maC = NhapDuLieu::nhapMaDinhDanh("Nhap ma cong (hoac nhap 'HUY' de thoat): ");
        if (maC == "HUY" || maC == "huy") {
            maDinhDanh = "";
            return;
        }
        setMaDinhDanh(maC);

        if (!lapDenKhiHopLe([&] {
            std::string s = NhapDuLieu::nhapChuoi("Nhap ten OLT (nhap 'HUY' de huy): ");
            if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");
            setTenOLT(s);
            })) {
            maDinhDanh = ""; return;
        }

        if (!lapDenKhiHopLe([&] {
            std::string sSo = NhapDuLieu::nhapChuoi("Nhap so cong (>=1, hoac 'HUY'): ");
            if (sSo == "HUY" || sSo == "huy") throw std::invalid_argument("HUY");
            int so = chuyenSoCong(sSo);
            setSoCong(so);
            })) {
            maDinhDanh = ""; return;
        }

        if (!lapDenKhiHopLe([&] {
            std::string s = NhapDuLieu::nhapChuoi("Nhap khu vuc (nhap 'HUY' de huy): ");
            if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");
            setKhuVuc(s);
            })) {
            maDinhDanh = ""; return;
        }

        if (!lapDenKhiHopLe([&] {
            std::string s = NhapDuLieu::nhapMaDinhDanh("Nhap ma tuyen cap (hoac 'HUY'): ");
            if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");
            setMaTuyenCap(s);
            })) {
            maDinhDanh = ""; return;
        }

        try {
            if (!nhapTrangThai(false)) {
                maDinhDanh = "";
                return;
            }
        }
        catch (const std::invalid_argument& e) {
            if (std::string(e.what()) == "HUY") {
                maDinhDanh = "";
                return;
            }
            throw;
        }
    }

    void capNhatThongTin() override {
        capNhatTruong("Nhap ten OLT moi", [&](const std::string& s) { setTenOLT(s); });
        capNhatTruong("Nhap so cong moi", [&](const std::string& s) { setSoCong(chuyenSoCong(s)); });
        capNhatTruong("Nhap khu vuc moi", [&](const std::string& s) { setKhuVuc(s); });
        capNhatTruong("Nhap ma tuyen cap moi", [&](const std::string& s) { setMaTuyenCap(s); });

        try {
            nhapTrangThai(true);
        }
        catch (const std::invalid_argument& e) {
            if (std::string(e.what()) == "HUY") return;
            throw;
        }
    }

    // Hiển thị một dòng dữ liệu dạng bảng khớp với tiêu đề inTieuDe()
    void hienThiThongTin() const override {
        std::cout << std::left
            << std::setw(10) << cat(maDinhDanh, 9) << " | "
            << std::setw(12) << cat(maTuyenCap, 11) << " | "
            << std::setw(16) << cat(tenOLT, 15) << " | "
            << std::setw(8) << soCong << " | "
            << std::setw(14) << cat(khuVuc, 13) << " | "
            << std::setw(10) << cat(maKhachHang.empty() ? "-" : maKhachHang, 9) << " | "
            << std::setw(14) << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenOLT + "|" +
            std::to_string(soCong) + "|" + khuVuc + "|" +
            maTuyenCap + "|" + maKhachHang + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dongDuLieu) override {
        std::vector<std::string> truong = tachTruong(dongDuLieu);
        if (truong.size() != 7)
            throw std::runtime_error("Du lieu khong hop le: sai so truong (can 7)!");

        try {
            CongOLT tam;
            tam.setMaDinhDanh(chuanHoaMa(truong[0], "Ma cong"));
            tam.setTenOLT(truong[1]);
            tam.setSoCong(chuyenSoCong(truong[2]));
            tam.setKhuVuc(truong[3]);
            tam.setMaTuyenCap(truong[4]);
            tam.setTrangThai(truong[6], truong[5]);
            *this = tam;
        }
        catch (const std::invalid_argument& e) {
            throw std::runtime_error(e.what());
        }
    }

    const std::string& getTenOLT() const { return tenOLT; }
    int getSoCong() const { return soCong; }
    const std::string& getKhuVuc() const { return khuVuc; }
    const std::string& getMaTuyenCap() const { return maTuyenCap; }
    const std::string& getMaKhachHang() const { return maKhachHang; }
    const std::string& getTrangThai() const { return trangThai; }
};

#endif
