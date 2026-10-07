/* =======================================================
 * Tên tác giả: Trần Ngọc Huy
 * Mã sinh viên: B24DCVT183
 * Mô tả file: Use Case 01 - Quản lý Gói cước Internet
 * ======================================================= */

#ifndef UC01_GOI_INTERNET_H
#define UC01_GOI_INTERNET_H

#include "LopCoSo.h"
#include  "NhapDuLieu.h"
#include "QuanLyLuuTru.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

class GoiInternet : public LopCoSo {
private:
    std::string tenGoi;
    double tocDo;      // Mbps
    double giaThang;   // VNĐ
    std::string moTa;
    bool trangThai;    // true: Hoạt động, false: Ngừng

    // Hàm tiện ích hỗ trợ cắt khoảng trắng 2 đầu chuỗi
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, last - first + 1);
    }

public:
    GoiInternet() : LopCoSo(), tocDo(0.0), giaThang(0.0), trangThai(true) {}

    GoiInternet(const std::string& ma, const std::string& ten, double td, double gia, const std::string& mt, bool tt)
        : LopCoSo(ma), tenGoi(ten), tocDo(td), giaThang(gia), moTa(mt), trangThai(tt) {}

    std::string getTenGoi() const { return tenGoi; }
    double getTocDo() const { return tocDo; }
    double getGiaThang() const { return giaThang; }
    std::string getMoTa() const { return moTa; }
    bool getTrangThai() const { return trangThai; }

    void nhapThongTin() override {
        setMaDinhDanh(NhapDuLieu::nhapMaDinhDanh("Nhap ma goi internet (VD: NET100): "));
        tenGoi = NhapDuLieu::nhapChuoi("Nhap ten goi: ");
        tocDo = NhapDuLieu::nhapSoThucDuong("Nhap toc do (Mbps): ");
        giaThang = NhapDuLieu::nhapSoThucDuong("Nhap gia cuoc thang (VND): ");
        moTa = NhapDuLieu::nhapChuoi("Nhap mo ta chi tiet: ");
        trangThai = NhapDuLieu::xacNhan("Kich hoat goi ngay?");
    }

    void hienThiThongTin() const override {
        std::cout << std::left 
                  << std::setw(12) << maDinhDanh 
                  << std::setw(22) << tenGoi 
                  << std::setw(12) << (std::to_string((int)tocDo) + " Mbps")
                  << std::setw(16) << (std::to_string((long long)giaThang) + " VND")
                  << std::setw(16) << (trangThai ? "Hoat dong" : "Ngung hoat dong")
                  << moTa << std::endl;
    }

    void capNhatThongTin() override {
        std::cout << "--- Cap nhat thong tin cho ma goi: " << maDinhDanh << " ---\n";
        tenGoi = NhapDuLieu::nhapChuoi("Nhap ten goi moi: ");
        tocDo = NhapDuLieu::nhapSoThucDuong("Nhap toc do moi (Mbps): ");
        giaThang = NhapDuLieu::nhapSoThucDuong("Nhap gia cuoc moi (VND): ");
        moTa = NhapDuLieu::nhapChuoi("Nhap mo ta moi: ");
        trangThai = NhapDuLieu::xacNhan("Doi trang thai sang Hoat dong?");
    }

    std::string chuyenThanhChuoi() const override {
        return maDinhDanh + "|" + tenGoi + "|" + std::to_string(tocDo) + "|" 
               + std::to_string((long long)giaThang) + "|" + moTa + "|" + (trangThai ? "1" : "0");
    }

    // --- HÀM ĐỌC FILE ĐÃ SỬA ĐẦY ĐỦ CÁC ĐIỀU KIỆN VALIDATE ---
    void docTuChuoi(const std::string& dongDuLieu) override {
        std::stringstream ss(dongDuLieu);
        std::string token;
        std::vector<std::string> tokens;

        // Tách chuỗi theo ký tự '|'
        while (std::getline(ss, token, '|')) {
            tokens.push_back(trim(token));
        }

        // 1. Kiểm tra đủ đúng 6 trường thông tin
        if (tokens.size() != 6) {
            throw std::invalid_argument("Du lieu khong du hoac du thua truong thông tin (Yeu cau 6 truong)");
        }

        std::string ma = tokens[0];
        std::string ten = tokens[1];
        std::string tocDoStr = tokens[2];
        std::string giaStr = tokens[3];
        std::string mt = tokens[4];
        std::string statusStr = tokens[5];

        // 6. Kiểm tra chuỗi dữ liệu (mã và tên không được rỗng)
        if (ma.empty() || ten.empty()) {
            throw std::invalid_argument("Ma goi va Ten goi khong duoc de trong!");
        }

        // 3. Kiểm tra chuyển đổi số (std::stod) chống nổ chương trình
        double td = 0.0;
        double gia = 0.0;
        try {
            size_t idx1 = 0, idx2 = 0;
            td = std::stod(tocDoStr, &idx1);
            gia = std::stod(giaStr, &idx2);

            // Đảm bảo toàn bộ chuỗi là số, không chứa ký tự lạ (VD: "100abc")
            if (idx1 != tocDoStr.length() || idx2 != giaStr.length()) {
                throw std::invalid_argument("Dinh dang so chua ky tu khong hop le");
            }
        } catch (...) {
            throw std::invalid_argument("Loi chuyen doi dinh dang so (Toc do / Gia thang)");
        }

        // 4. Kiểm tra điều kiện giá trị số >= 0
        if (td < 0.0 || gia < 0.0) {
            throw std::invalid_argument("Toc do va Gia thang phai la so >= 0");
        }

        // 5. Kiểm tra nghiêm ngặt trạng thái (Chỉ chấp nhận "0" hoặc "1")
        if (statusStr != "0" && statusStr != "1") {
            throw std::invalid_argument("Trang thai chi duoc phep la '0' hoac '1'");
        }

        // Gán dữ liệu khi tất cả kiểm tra thành công
        setMaDinhDanh(ma);
        tenGoi = ten;
        tocDo = td;
        giaThang = gia;
        moTa = mt;
        trangThai = (statusStr == "1");
    }
};

// ============================================================================
// SERVICE: QuanLyGoiInternet
// ============================================================================
class QuanLyGoiInternet {
private:
    QuanLyLuuTru<GoiInternet> luuTru;

    // 7. Kiểm tra khóa ngoại maGoi khi tích hợp hệ thống
    bool kiemTraKhoaNgoai(const std::string& maGoi) const {
        // Nơi kết nối với các Use Case khác (VD: UC03 HopDong, UC04 PhieuDangKy)
        // Nếu maGoi đang được tham chiếu trong Hợp đồng -> return true
        return false; 
    }

public:
    QuanLyGoiInternet(const std::string& duongDanFile = "data/goi_internet.txt") 
        : luuTru(duongDanFile) {}

    void themMoi() {
        GoiInternet goi;
        goi.nhapThongTin();
        luuTru.themMoi(goi);
    }

    void hienThiDanhSach() const {
        std::cout << "\n============================== DANH SACH GOI INTERNET ==============================\n";
        std::cout << std::left 
                  << std::setw(12) << "Ma Goi" 
                  << std::setw(22) << "Ten Goi" 
                  << std::setw(12) << "Toc Do" 
                  << std::setw(16) << "Gia Thang" 
                  << std::setw(16) << "Trang Thai" 
                  << "Mo Ta" << std::endl;
        std::cout << std::string(90, '-') << std::endl;
        luuTru.hienThi();
        std::cout << std::string(90, '-') << std::endl;
    }

    void capNhat() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can cap nhat: ");
        luuTru.capNhat(ma);
    }

    void xoa() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can xoa: ");
        
        // 7. Thực hiện kiểm tra khóa ngoại trước khi tiến hành xóa
        if (kiemTraKhoaNgoai(ma)) {
            std::cout << " -> Loi: Khong the xoa! Ma goi [" << ma 
                      << "] dang duoc su dung trong Hop Dong / Phieu Dang Ky!\n";
            return;
        }

        if (NhapDuLieu::xacNhan("Ban co chac chan muon xoa goi nay khong?")) {
            luuTru.xoaTheoMa(ma);
        }
    }

    void timKiem() {
        std::string ma = NhapDuLieu::nhapMaDinhDanh("Nhap ma goi can tim: ");
        GoiInternet* goi = luuTru.timTheoMa(ma);
        if (goi != nullptr) {
            std::cout << "\n-> THONG TIN GOI INTERNET TIM THAY:\n";
            goi->hienThiThongTin();
        } else {
            std::cout << " -> Khong tim thay ma goi: " << ma << "\n";
        }
    }
};

#endif
  
              
      

 
    

