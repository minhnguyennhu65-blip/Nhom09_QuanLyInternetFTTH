/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp quản lý thiết bị ONT/Modem (Entity - UC05 - Single Header)
 * ======================================================= */

#ifndef THIET_BI_H
#define THIET_BI_H

#include "LopCoSo.h"
#include "NhapDuLieu.h"
#include "HopDong.h"
#include "QuanLyLuuTru.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>

class ThietBi : public LopCoSo {
private:
    std::string soSerial;
    std::string loaiThietBi;   // "ONT" hoặc "Modem"
    std::string hangSanXuat;
    std::string maHopDong;     // Khóa ngoại tới Hợp đồng (rỗng nếu thiết bị ở Kho/Hong)
    std::string trangThai = "Kho"; // "Dang cap phat", "Kho", "Hong"

    // Hàm kiểm tra mã hợp đồng có tồn tại không (gắn từ main)
    inline static std::function<bool(const std::string&)> kiemTraHopDong = nullptr;

    // ---------- Hàm kiểm tra / tiện ích nội bộ ----------

    static bool hopLeTrangThai(const std::string& tt) {
        return tt == "Dang cap phat" || tt == "Kho" || tt == "Hong";
    }

    static bool hopLeLoai(const std::string& loai) {
        return loai == "ONT" || loai == "Modem";
    }

    // Chuỗi dữ liệu hợp lệ: không chứa '|' và ký tự điều khiển, xử lý từ khóa HUY
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

    // Chuẩn hóa mã định danh / mã hợp đồng
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

    // Tách theo '|', giữ lại cả trường rỗng ở giữa và cuối
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

    // Cắt chuỗi dài để không làm lệch cột khi in bảng
    static std::string cat(const std::string& s, std::size_t n) {
        return s.size() <= n ? s : s.substr(0, n - 2) + "..";
    }

    // Lặp lại thao tác nhập cho đến khi hợp lệ hoặc gõ HUY
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

    // Dùng cho cập nhật: bỏ trống = giữ nguyên
    template <typename F>
    static void capNhatTruong(const std::string& nhan, F setter) {
        while (true) {
            std::string s = NhapDuLieu::nhapChuoiHoacGiuNguyen(
                nhan + " (bo trong de giu nguyen, hoac nhap 'HUY'): ", "");
            if (s.empty()) return;
            if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");
            try { setter(s); return; }
            catch (const std::invalid_argument& e) {
                std::string err = e.what();
                if (err == "HUY") throw std::invalid_argument("HUY");
                std::cout << " -> Loi: " << err << '\n';
            }
        }
    }

    // Chọn trạng thái thiết bị
    bool nhapTrangThai(bool choPhepBoTrong) {
        std::string thongBaoChon = choPhepBoTrong
            ? "Chon trang thai (bo trong de giu nguyen, hoac nhap 'HUY'): "
            : "Chon trang thai (hoac nhap 'HUY'): ";

        std::cout << "\nTrang thai thiet bi:\n"
            << "1. Kho\n"
            << "2. Dang cap phat\n"
            << "3. Hong\n";

        std::string s = NhapDuLieu::nhapChuoiHoacGiuNguyen(thongBaoChon, "");
        if (s.empty() && choPhepBoTrong) return false;
        if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");

        int chon = 0;
        try {
            chon = std::stoi(s);
        }
        catch (...) {
            chon = 0;
        }

        if (choPhepBoTrong && chon == 0) return false;
        if (chon == 1) { setTrangThaiVaHD("Kho", ""); return true; }
        if (chon == 3) { setTrangThaiVaHD("Hong", ""); return true; }

        if (chon == 2) {
            bool thanhCong = lapDenKhiHopLe([&] {
                std::string maHD = NhapDuLieu::nhapMaDinhDanh("Nhap ma hop dong lien quan (hoac 'HUY'): ");
                if (maHD == "HUY" || maHD == "huy") throw std::invalid_argument("HUY");
                setTrangThaiVaHD("Dang cap phat", maHD);
                });
            return thanhCong;
        }

        std::cout << " -> Loi: Lua chon khong hop le!\n";
        return nhapTrangThai(choPhepBoTrong);
    }

public:
    ThietBi() : LopCoSo(), trangThai("Kho") {}

    static void setKiemTraHopDong(std::function<bool(const std::string&)> f) {
        kiemTraHopDong = std::move(f);
    }

    // In tiêu đề bảng chuẩn có dấu phân cách ' | '
    static void inTieuDe() {
        std::cout << std::left
            << std::setw(12) << "Ma TB" << " | "
            << std::setw(16) << "Serial" << " | "
            << std::setw(10) << "Loai" << " | "
            << std::setw(16) << "Hang SX" << " | "
            << std::setw(12) << "Ma HD" << " | "
            << std::setw(16) << "Trang thai" << '\n'
            << std::string(90, '-') << '\n';
    }

    const std::string& getSoSerial() const { return soSerial; }
    const std::string& getLoaiThietBi() const { return loaiThietBi; }
    const std::string& getHangSanXuat() const { return hangSanXuat; }
    const std::string& getMaHopDong() const { return maHopDong; }
    const std::string& getTrangThai() const { return trangThai; }

    void setSoSerial(const std::string& serial) {
        kiemTraChuoi(serial, "So serial");
        soSerial = serial;
    }

    void setLoaiThietBi(const std::string& loai) {
        if (loai == "HUY" || loai == "huy") throw std::invalid_argument("HUY");
        if (!hopLeLoai(loai))
            throw std::invalid_argument("Loai thiet bi chi duoc la ONT hoac Modem!");
        loaiThietBi = loai;
    }

    void setHangSanXuat(const std::string& hsx) {
        kiemTraChuoi(hsx, "Hang san xuat");
        hangSanXuat = hsx;
    }

    void setTrangThaiVaHD(const std::string& tt, const std::string& maHD = "") {
        if (!hopLeTrangThai(tt))
            throw std::invalid_argument("Trang thai khong hop le!");
        if (tt == "Dang cap phat") {
            std::string chuanHD = chuanHoaMa(maHD, "Ma hop dong");
            if (kiemTraHopDong && !kiemTraHopDong(chuanHD))
                throw std::invalid_argument("Hop dong khong ton tai!");
            maHopDong = chuanHD;
        }
        else {
            if (!maHD.empty())
                throw std::invalid_argument("Thiet bi Kho/Hong khong duoc gan ma hop dong!");
            maHopDong.clear();
        }
        trangThai = tt;
    }

    void nhapThongTin() override {
        std::string maTB = NhapDuLieu::nhapMaDinhDanh("Nhap ma thiet bi (hoac nhap 'HUY' de thoat): ");
        if (maTB == "HUY" || maTB == "huy") {
            maDinhDanh = "";
            return;
        }
        setMaDinhDanh(maTB);

        if (!lapDenKhiHopLe([&] {
            std::string s = NhapDuLieu::nhapChuoi("Nhap so serial (nhap 'HUY' de huy): ");
            if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");
            setSoSerial(s);
            })) {
            maDinhDanh = ""; return;
        }

        if (!lapDenKhiHopLe([&] {
            std::cout << "Loai thiet bi:\n1. ONT\n2. Modem\n";
            int chonLoai = NhapDuLieu::nhapSoNguyenTrongKhoang("Chon loai (1-2): ", 1, 2);
            setLoaiThietBi(chonLoai == 1 ? "ONT" : "Modem");
            })) {
            maDinhDanh = ""; return;
        }

        if (!lapDenKhiHopLe([&] {
            std::string s = NhapDuLieu::nhapChuoi("Nhap hang san xuat (nhap 'HUY' de huy): ");
            if (s == "HUY" || s == "huy") throw std::invalid_argument("HUY");
            setHangSanXuat(s);
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
        capNhatTruong("Nhap so serial moi", [&](const std::string& s) { setSoSerial(s); });

        // Cập nhật loại thiết bị tùy chọn
        std::string sLoai = NhapDuLieu::nhapChuoiHoacGiuNguyen("Chon loai moi (1. ONT, 2. Modem, bo trong de giu nguyen, hoac 'HUY'): ", "");
        if (sLoai == "HUY" || sLoai == "huy") throw std::invalid_argument("HUY");
        if (sLoai == "1") setLoaiThietBi("ONT");
        else if (sLoai == "2") setLoaiThietBi("Modem");

        capNhatTruong("Nhap hang san xuat moi", [&](const std::string& s) { setHangSanXuat(s); });

        try {
            nhapTrangThai(true);
        }
        catch (const std::invalid_argument& e) {
            if (std::string(e.what()) == "HUY") return;
            throw;
        }
    }

    // Hiển thị một dòng dữ liệu dạng bảng khớp với inTieuDe()
    void hienThiThongTin() const override {
        std::cout << std::left
            << std::setw(12) << cat(maDinhDanh, 11) << " | "
            << std::setw(16) << cat(soSerial, 15) << " | "
            << std::setw(10) << cat(loaiThietBi, 9) << " | "
            << std::setw(16) << cat(hangSanXuat, 15) << " | "
            << std::setw(12) << cat(maHopDong.empty() ? "-" : maHopDong, 11) << " | "
            << std::setw(16) << trangThai << '\n';
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + soSerial + "|" + loaiThietBi + "|" +
            hangSanXuat + "|" + maHopDong + "|" + trangThai;
    }

    void docTuChuoi(const std::string& dong) override {
        std::vector<std::string> truong = tachTruong(dong);
        if (truong.size() != 6)
            throw std::runtime_error("Dong du lieu ThietBi co so cot khong dung (can 6).");

        try {
            ThietBi tam;
            tam.setMaDinhDanh(chuanHoaMa(truong[0], "Ma thiet bi"));
            tam.setSoSerial(truong[1]);
            tam.setLoaiThietBi(truong[2]);
            tam.setHangSanXuat(truong[3]);
            tam.setTrangThaiVaHD(truong[5], truong[4]);
            *this = tam;
        }
        catch (const std::invalid_argument& e) {
            throw std::runtime_error(e.what());
        }
    }
};

#endif
