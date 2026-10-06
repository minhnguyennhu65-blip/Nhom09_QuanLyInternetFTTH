#ifndef QUAN_LY_LUU_TRU_H
#define QUAN_LY_LUU_TRU_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

template <typename T>
class QuanLyLuuTru {
private:
    std::string duongDanFile;
    std::vector<T> danhSach;
public:
    // Constructor: Tự động nạp dữ liệu khi khởi tạo
    QuanLyLuuTru(const std::string& duongDan)
        : duongDanFile(duongDan) {
        docTuFile();
    }
    // Lấy danh sách (nếu tầng giao diện cần thao tác trực tiếp)
    std::vector<T>& getDanhSach() {
        return danhSach;
    }
    // Đọc dữ liệu từ file
    bool docTuFile() {
        std::ifstream file(duongDanFile);
        if (!file.is_open()) {
            std::ofstream fileMoi(duongDanFile);
            if (!fileMoi.is_open()) {
                std::cout << " -> Loi: Khong the tao file "
                          << duongDanFile << "!\n";
                return false;
            }
            fileMoi.close();
            return true;
        }
        danhSach.clear();
        std::string dong;
        while (std::getline(file, dong)) {
            if (dong.empty() || dong[0] == '#') {
                continue;
            }
            try {
                T doiTuong;
                doiTuong.docTuChuoi(dong);
                danhSach.push_back(doiTuong);
            }
            catch (...) {
                std::cout << " -> Canh bao: Bo qua dong du lieu loi: "
                          << dong << "\n";
            }
        }
        file.close();
        return true;
    }
    // Lưu dữ liệu vào file
    bool luuVaoFile() {
        std::ofstream file(
            duongDanFile,
            std::ios::trunc
        );
        if (!file.is_open()) {
            std::cout << " -> Loi: Khong the mo file "
                      << duongDanFile
                      << " de ghi du lieu!\n";
            return false;
        }
        for (const auto& doiTuong : danhSach) {
            file << doiTuong.chuyenThanhChuoi() << "\n";
        }
        file.close();
        return true;
    }
    // Thêm đối tượng (Nhận dữ liệu từ Menu)
    bool themMoi(const T& doiTuongMoi) {
        // Kiểm tra trùng mã
        if (timTheoMa(doiTuongMoi.getMaDinhDanh()) != nullptr) {
            std::cout << " -> Loi: Ma dinh danh da ton tai!\n";
            return false;
        }
        danhSach.push_back(doiTuongMoi);
        luuVaoFile();
        std::cout << " -> Them thanh cong!\n";
        return true;
    }
    // Hiển thị toàn bộ danh sách
    void hienThi() const {
        if (danhSach.empty()) {
            std::cout << " -> Danh sach rong!\n";
            return;
        }
        for (const auto& doiTuong : danhSach) {
            doiTuong.hienThiThongTin();
        }
    }
    // Tìm theo mã
    T* timTheoMa(const std::string& ma) {
        for (auto& doiTuong : danhSach) {
            if (doiTuong.getMaDinhDanh() == ma) {
                return &doiTuong;
            }
        }
        return nullptr;
    }
    // Sửa thông tin (Nhận mã từ Menu truyền vào)
    bool capNhat(const std::string& ma) {
        T* doiTuong = timTheoMa(ma);
        if (doiTuong == nullptr) {
            std::cout << " -> Khong tim thay ma: " << ma << "\n";
            return false;
        }
        // Chỉ cập nhật thông tin phụ (lớp Entity tự hỏi người dùng)
        doiTuong->capNhatThongTin();
        luuVaoFile();
        std::cout << " -> Cap nhat thanh cong!\n";
        return true;
    }
    // Xóa theo mã (Nhận mã từ Menu truyền vào)
    bool xoaTheoMa(const std::string& ma) {
        for (auto it = danhSach.begin(); it != danhSach.end(); ++it) {
            if (it->getMaDinhDanh() == ma) {
                danhSach.erase(it);
                luuVaoFile();
                std::cout << " -> Xoa thanh cong!\n";
                return true;
            }
        }
        std::cout << " -> Khong tim thay ma: " << ma << "\n";
        return false;
    }
    // Lấy đường dẫn file
    std::string getDuongDanFile() const {
        return duongDanFile;
    }
};

#endif
