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
#include <string>
#include <sstream>
#include <stdexcept>
#include <limits>

class CongOLT : public LopCoSo {
private:
    // Các thuộc tính chính của Cổng OLT
    std::string tenOLT;
    int soCong;
    std::string khuVuc;
    std::string maKhachHang; // Liên kết khóa ngoại với bảng Khách hàng
    std::string trangThai;   // Trạng thái: "Trong", "Dang su dung", "Hong"

    // Con trỏ tĩnh quản lý CSDL Khách hàng để kiểm tra tính toàn vẹn tham chiếu (Khóa ngoại)
    // Dùng inline static để khai báo và khởi tạo trực tiếp ngay trong file .h
    inline static QuanLyLuuTru<KhachHang>* dbKhachHang = nullptr;

    // Kiểm tra xem trạng thái truyền vào có nằm trong danh mục cho phép hay không
    bool hopLeTrangThai(const std::string& tt) const {
        return tt == "Trong" ||
               tt == "Dang su dung" ||
               tt == "Hong";
    }

    // Kiểm tra sự tồn tại của mã khách hàng trong cơ sở dữ liệu
    // Trả về false nếu chưa kết nối DB để ép buộc kiểm tra khóa ngoại nghiêm ngặt
    bool khachHangTonTai(const std::string& ma) const {
        if (dbKhachHang == nullptr)
            return false;

        return dbKhachHang->timTheoMa(ma) != nullptr;
    }

    // Giao diện tương tác để người dùng chọn trạng thái cổng và ràng buộc mã khách hàng tương ứng
    void nhapTrangThai() {
        int chon;
        do {
            std::cout << "\nTrang thai cong:\n";
            std::cout << "1. Trong\n";
            std::cout << "2. Dang su dung\n";
            std::cout << "3. Hong\n";

            chon = NhapDuLieu::nhapSoNguyenDuong("Chon trang thai: ");

            if (chon < 1 || chon > 3)
                std::cout << "Lua chon khong hop le!\n";
        } while (chon < 1 || chon > 3);

        // Xử lý logic gán dữ liệu dựa theo lựa chọn trạng thái
        if (chon == 1) {
            trangThai = "Trong";
            maKhachHang = ""; // Trạng thái trống thì không có khách hàng sử dụng
        } else if (chon == 2) {
            trangThai = "Dang su dung";
            // Bắt buộc nhập mã khách hàng hợp lệ và tồn tại trong hệ thống (Khóa ngoại)
            do {
                maKhachHang = NhapDuLieu::nhapMaDinhDanh("Nhap ma khach hang: ");
                if (!khachHangTonTai(maKhachHang))
                    std::cout << "Khach hang khong ton tai!\n";
            } while (!khachHangTonTai(maKhachHang));
        } else {
            trangThai = "Hong";
            maKhachHang = ""; // Trạng thái hỏng thì thu hồi mã khách hàng
        }
    }

public:
    // Constructor mặc định khởi tạo giá trị ban đầu
    CongOLT() : LopCoSo(), soCong(1), trangThai("Trong") {}

    // Phương thức tĩnh thiết lập liên kết đến bộ quản lý Khách hàng từ chương trình chính (main.cpp)
    static void setDBKhachHang(QuanLyLuuTru<KhachHang>* db) {
        dbKhachHang = db;
    }

    // Nhập thông tin chi tiết cho một Cổng OLT mới (Ghi đè từ LopCoSo)
    void nhapThonTin() override {
        maDinhDanh = NhapDuLieu::nhapMaDinhDanh("Nhap ma cong: ");
        tenOLT = NhapDuLieu::nhapChuoi("Nhap ten OLT: ");

        do {
            soCong = NhapDuLieu::nhapSoNguyenDuong("Nhap so cong: ");
            if (soCong < 1) std::cout << "So cong phai lon hon 0!\n";
        } while (soCong < 1);

        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc: ");
        nhapTrangThai();
    }

    // Cập nhật thông tin Cổng OLT (giữ nguyên mã định danh, sửa các thuộc tính khác)
    void capNhatThongTin() override {
        tenOLT = NhapDuLieu::nhapChuoi("Nhap ten OLT moi: ");

        do {
            soCong = NhapDuLieu::nhapSoNguyenDuong("Nhap so cong moi: ");
            if (soCong < 1) std::cout << "So cong phai lon hon 0!\n";
        } while (soCong < 1);

        khuVuc = NhapDuLieu::nhapChuoi("Nhap khu vuc moi: ");
        nhapTrangThai();
    }

    // Hiển thị thông tin cổng OLT ra màn hình dưới dạng tường minh
    void hienThiThongTin() const override {
        std::cout << "Ma cong: " << maDinhDanh
                  << " | OLT: " << tenOLT
                  << " | So cong: " << soCong
                  << " | Khu vuc: " << khuVuc
                  << " | Ma khach hang: "
                  << (maKhachHang.empty() ? "Khong co" : maKhachHang)
                  << " | Trang thai: " << trangThai << '\n';
    }

    // Chuyển đổi đối tượng thành định dạng chuỗi ngăn cách bằng dấu gạch đứng '|' để ghi file
    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenOLT + "|" +
               std::to_string(soCong) + "|" + khuVuc + "|" +
               maKhachHang + "|" + trangThai;
    }

    // Đọc và phân tích dữ liệu từ một dòng trong file văn bản, kiểm tra lỗi cú pháp và ràng buộc
    void docTuChuoi(const std::string& dongDuLieu) override {
        std::istringstream dong(dongDuLieu);
        std::string ma, ten, so, kv, maKH, tt, thua;

        // Đảm bảo dòng dữ liệu có đúng cấu trúc 6 trường phân tách bởi dấu '|'
        if (!std::getline(dong, ma, '|') ||
            !std::getline(dong, ten, '|') ||
            !std::getline(dong, so, '|') ||
            !std::getline(dong, kv, '|') ||
            !std::getline(dong, maKH, '|') ||
            !std::getline(dong, tt) ||
            std::getline(dong, thua, '|')) {
            throw std::runtime_error("Du lieu khong hop le!");
        }

        // Kiểm tra và ép kiểu số lượng cổng
        std::istringstream kiemTraSo(so);
        int soMoi;

        if (!(kiemTraSo >> soMoi) || soMoi < 1 ||
            (kiemTraSo >> std::ws, !kiemTraSo.eof())) {
            throw std::runtime_error("So cong khong hop le!");
        }

        // Kiểm tra tính hợp lệ của các trường cơ bản và trạng thái
        if (ma.empty() || ten.empty() || kv.empty() || !hopLeTrangThai(tt)) {
            throw std::runtime_error("Thong tin khong hop le!");
        }

        // Kiểm tra ràng buộc khóa ngoại với mã khách hàng theo từng trạng thái thực tế
        if (tt == "Dang su dung") {
            if (maKH.empty() || !khachHangTonTai(maKH))
                throw std::runtime_error("Ma khach hang khong ton tai!");
        } else if (!maKH.empty()) {
            throw std::runtime_error("Cong nay khong duoc gan khach hang!");
        }

        // Gán dữ liệu sau khi đã vượt qua toàn bộ các tầng kiểm tra hợp lệ
        maDinhDanh = ma;
        tenOLT = ten;
        soCong = soMoi;
        khuVuc = kv;
        maKhachHang = maKH;
        trangThai = tt;
    }

    // Các hàm Getters để truy vấn thuộc tính từ bên ngoài
    std::string getTenOLT() const { return tenOLT; }
    int getSoCong() const { return soCong; }
    std::string getKhuVuc() const { return khuVuc; }
    std::string getMaKhachHang() const { return maKhachHang; }
    std::string getTrangThai() const { return trangThai; }
};

#endif
