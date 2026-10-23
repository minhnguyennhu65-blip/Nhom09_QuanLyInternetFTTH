/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp khuôn mẫu (Template Repository) đóng gói
 * thuật toán xử lý mảng (vector), file text và các hàm CRUD.
 * Yêu cầu: C++17 trở lên
 *
 * Hợp đồng với kiểu T:
 *  - copy được / gán được, có constructor mặc định
 *  - docTuChuoi() PHẢI ném ngoại lệ khi dòng dữ liệu sai định dạng
 *  - chuyenThanhChuoi() không được trả về chuỗi chứa '\n' hoặc '\r'
 *  - capNhatThongTin() không được đổi mã định danh
 * ======================================================= */

#ifndef QUAN_LY_LUU_TRU_H
#define QUAN_LY_LUU_TRU_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include <filesystem>
#include <system_error>
#include <ctime>
#include <utility>
#include <algorithm>

template <typename T>
class QuanLyLuuTru {
private:
    std::string duongDanFile;
    std::vector<T> danhSach;
    std::vector<std::string> dongChuThich; // Các dòng bắt đầu bằng '#', ghi lại khi lưu
    bool choPhepGhi = true;                // false khi nạp file thất bại: cấm ghi đè để bảo vệ dữ liệu cũ

    // Bỏ '\r' / '\n' cuối dòng (file tạo trên Windows, đọc trên Linux/Mac)
    static void catXuongDong(std::string& s) {
        while (!s.empty() && (s.back() == '\r' || s.back() == '\n'))
            s.pop_back();
    }

    // Sao lưu file gốc (tên có mốc thời gian, không đè bản sao lưu cũ).
    // Trả về tên file sao lưu, rỗng nếu thất bại.
    std::string saoLuuFileGoc() const {
        std::string tenBak = duongDanFile + ".bak." + std::to_string(std::time(nullptr));
        std::ifstream in(duongDanFile, std::ios::binary);
        std::ofstream out(tenBak, std::ios::binary | std::ios::trunc);
        if (!in.is_open() || !out.is_open()) return "";
        out << in.rdbuf();
        out.flush();
        return out ? tenBak : "";
    }

public:
    // Constructor: tự động nạp dữ liệu khi khởi tạo
    // LƯU Ý: các ràng buộc khóa ngoại (vd CongOLT::setDBKhachHang) phải được
    // thiết lập TRƯỚC khi tạo repository này.
    explicit QuanLyLuuTru(const std::string& duongDan)
        : duongDanFile(duongDan) {
        docTuFile();
    }

    // Repository gắn với một file, copy sẽ tạo 2 nơi cùng ghi một file
    QuanLyLuuTru(const QuanLyLuuTru&) = delete;
    QuanLyLuuTru& operator=(const QuanLyLuuTru&) = delete;

    std::vector<T>& getDanhSach() {
        return danhSach;
    }

    const std::vector<T>& getDanhSach() const {
        return danhSach;
    }

    // Đọc dữ liệu từ file
    bool docTuFile() {
        namespace fs = std::filesystem;
        std::error_code ec;

        bool tonTai = fs::exists(duongDanFile, ec);
        if (ec) {
            std::cout << " -> Loi: Khong kiem tra duoc file " << duongDanFile << "!\n";
            choPhepGhi = false;
            return false;
        }

        // File chưa có: tạo thư mục cha (nếu cần) và file rỗng
        if (!tonTai) {
            danhSach.clear();
            dongChuThich.clear();

            fs::path cha = fs::path(duongDanFile).parent_path();
            if (!cha.empty()) {
                fs::create_directories(cha, ec);
                if (ec) {
                    std::cout << " -> Loi: Khong the tao thu muc " << cha.string() << "!\n";
                    choPhepGhi = false;
                    return false;
                }
            }
            std::ofstream fileMoi(duongDanFile);
            if (!fileMoi.is_open()) {
                std::cout << " -> Loi: Khong the tao file " << duongDanFile << "!\n";
                choPhepGhi = false;
                return false;
            }
            choPhepGhi = true;
            return true;
        }

        // File đã có nhưng không mở được: TUYỆT ĐỐI không tạo lại (sẽ xóa sạch dữ liệu)
        std::ifstream file(duongDanFile);
        if (!file.is_open()) {
            std::cout << " -> Loi: Khong the doc file " << duongDanFile
                      << "! Chuong trinh se khong ghi de len file nay.\n";
            choPhepGhi = false;
            return false;
        }

        danhSach.clear();
        dongChuThich.clear();

        std::string dong;
        bool dauTien = true;
        bool coDongLoi = false;

        while (std::getline(file, dong)) {
            // Bỏ BOM UTF-8 nếu file được lưu từ Notepad
            if (dauTien) {
                dauTien = false;
                if (dong.compare(0, 3, "\xEF\xBB\xBF") == 0) dong.erase(0, 3);
            }
            catXuongDong(dong);

            if (dong.empty()) continue;
            if (dong[0] == '#') {
                dongChuThich.push_back(dong);
                continue;
            }

            try {
                T doiTuong;
                doiTuong.docTuChuoi(dong);
                if (doiTuong.getMaDinhDanh().empty())
                    throw std::runtime_error("Ma dinh danh rong!");
                if (timTheoMa(doiTuong.getMaDinhDanh()) != nullptr)
                    throw std::runtime_error("Ma dinh danh bi trung!");
                danhSach.push_back(std::move(doiTuong));
            }
            catch (const std::exception& e) {
                coDongLoi = true;
                std::cout << " -> Canh bao: Bo qua dong du lieu loi ("
                          << e.what() << "): " << dong << "\n";
            }
            catch (...) {
                coDongLoi = true;
                std::cout << " -> Canh bao: Bo qua dong du lieu loi: "
                          << dong << "\n";
            }
        }

        // Lỗi đọc giữa chừng (không phải EOF bình thường): dữ liệu có thể thiếu
        if (file.bad()) {
            std::cout << " -> Loi: Doc file " << duongDanFile
                      << " bi gian doan! Se khong ghi de len file nay.\n";
            choPhepGhi = false;
            return false;
        }
        file.close();

        if (coDongLoi) {
            std::string tenBak = saoLuuFileGoc();
            if (tenBak.empty()) {
                std::cout << " -> Canh bao: Khong sao luu duoc file goc!\n";
            } else {
                std::cout << " -> Da sao luu file goc sang " << tenBak << "\n";
            }
        }
        choPhepGhi = true;
        return true;
    }

    // Lưu dữ liệu vào file: ghi ra file tạm rồi đổi tên, nên nếu giữa chừng
    // bị lỗi/mất điện thì file gốc vẫn nguyên vẹn
    bool luuVaoFile() {
        namespace fs = std::filesystem;

        if (!choPhepGhi) {
            std::cout << " -> Loi: File " << duongDanFile
                      << " khong nap duoc truoc do, tu choi ghi de de bao ve du lieu!\n";
            return false;
        }

        // Tuần tự hóa trước và kiểm tra, tránh làm hỏng file giữa chừng
        std::vector<std::string> cacDong;
        cacDong.reserve(danhSach.size());
        for (const auto& doiTuong : danhSach) {
            std::string s = doiTuong.chuyenThanhChuoi();
            if (s.find_first_of("\r\n") != std::string::npos) {
                std::cout << " -> Loi: Du lieu chua ky tu xuong dong, khong the luu!\n";
                return false;
            }
            cacDong.push_back(std::move(s));
        }

        std::string tenTam = duongDanFile + ".tmp";
        {
            std::ofstream file(tenTam, std::ios::trunc);
            if (!file.is_open()) {
                std::cout << " -> Loi: Khong the mo file " << tenTam << " de ghi du lieu!\n";
                return false;
            }
            for (const auto& c : dongChuThich) file << c << "\n";
            for (const auto& s : cacDong) file << s << "\n";
            file.flush();
            if (!file) {
                file.close();
                std::error_code ec;
                fs::remove(tenTam, ec);
                std::cout << " -> Loi: Ghi file " << tenTam << " that bai!\n";
                return false;
            }
        } // file tạm được đóng tại đây

        std::error_code ec;
        fs::rename(tenTam, duongDanFile, ec);
        if (ec) {
            std::error_code ec2;
            fs::remove(tenTam, ec2);
            std::cout << " -> Loi: Khong the thay the file " << duongDanFile
                      << " (" << ec.message() << ")!\n";
            return false;
        }
        return true;
    }

    // Thêm đối tượng; không lưu được file thì hoàn tác
    // LƯU Ý: push_back có thể cấp phát lại vector, làm mất hiệu lực mọi
    // con trỏ/tham chiếu tới phần tử đã lấy trước đó.
    bool themMoi(const T& doiTuongMoi) {
        if (doiTuongMoi.getMaDinhDanh().empty()) {
            std::cout << " -> Loi: Ma dinh danh khong duoc de trong!\n";
            return false;
        }
        if (timTheoMa(doiTuongMoi.getMaDinhDanh()) != nullptr) {
            std::cout << " -> Loi: Ma dinh danh da ton tai!\n";
            return false;
        }
        danhSach.push_back(doiTuongMoi);
        if (!luuVaoFile()) {
            danhSach.pop_back();
            std::cout << " -> Khong luu duoc file, da huy thao tac them!\n";
            return false;
        }
        std::cout << " -> Them thanh cong!\n";
        return true;
    }

    // In danh sách dạng bảng: inTieuDe (tùy chọn) in dòng tiêu đề cột bằng <iomanip>,
    // mỗi bản ghi tự in một dòng căn cột trong hienThiThongTin(); cuối bảng in tổng số.
    static void inBang(const std::vector<T>& ds,
                       const std::function<void()>& inTieuDe = nullptr) {
        if (ds.empty()) {
            std::cout << " -> Danh sach rong!\n";
            return;
        }
        if (inTieuDe) inTieuDe();
        for (const auto& doiTuong : ds) {
            doiTuong.hienThiThongTin();
        }
        std::cout << " Tong so: " << ds.size() << "\n";
    }

    void hienThi(const std::function<void()>& inTieuDe = nullptr) const {
        inBang(danhSach, inTieuDe);
    }

    // Tìm kiếm theo điều kiện bất kỳ (theo tên gần đúng, theo khoảng năm, ...).
    // Trả về BẢN SAO các phần tử thỏa điều kiện (an toàn, không treo con trỏ).
    std::vector<T> timKiem(const std::function<bool(const T&)>& dieuKien) const {
        std::vector<T> ketQua;
        for (const auto& doiTuong : danhSach) {
            if (dieuKien(doiTuong)) ketQua.push_back(doiTuong);
        }
        return ketQua;
    }

    // Sắp xếp: trả về bản sao đã sắp xếp (không đổi thứ tự trong file).
    // sanhTruoc(a, b) = true nếu a đứng trước b. Dùng stable_sort để giữ thứ tự khi bằng nhau.
    std::vector<T> sapXep(const std::function<bool(const T&, const T&)>& sanhTruoc) const {
        std::vector<T> banSao = danhSach;
        std::stable_sort(banSao.begin(), banSao.end(), sanhTruoc);
        return banSao;
    }

    // Tìm theo mã. Con trỏ trả về chỉ dùng tạm: nó mất hiệu lực sau themMoi/xoaTheoMa
    T* timTheoMa(const std::string& ma) {
        for (auto& doiTuong : danhSach) {
            if (doiTuong.getMaDinhDanh() == ma) return &doiTuong;
        }
        return nullptr;
    }

    const T* timTheoMa(const std::string& ma) const {
        for (const auto& doiTuong : danhSach) {
            if (doiTuong.getMaDinhDanh() == ma) return &doiTuong;
        }
        return nullptr;
    }

    // Sửa thông tin; lỗi nhập, đổi mã, hoặc không lưu được file thì khôi phục bản cũ
    bool capNhat(const std::string& ma) {
        T* doiTuong = timTheoMa(ma);
        if (doiTuong == nullptr) {
            std::cout << " -> Khong tim thay ma: " << ma << "\n";
            return false;
        }
        T banSao = *doiTuong;

        try {
            doiTuong->capNhatThongTin();
        } catch (const std::exception& e) {
            *doiTuong = banSao;
            std::cout << " -> Cap nhat bi huy (" << e.what() << "), da khoi phuc du lieu cu!\n";
            return false;
        } catch (...) {
            *doiTuong = banSao;
            std::cout << " -> Cap nhat bi huy do loi khong xac dinh, da khoi phuc du lieu cu!\n";
            return false;
        }

        if (doiTuong->getMaDinhDanh() != ma) {
            *doiTuong = banSao;
            std::cout << " -> Loi: Khong duoc thay doi ma dinh danh, da khoi phuc du lieu cu!\n";
            return false;
        }
        if (!luuVaoFile()) {
            *doiTuong = banSao;
            std::cout << " -> Khong luu duoc file, da khoi phuc du lieu cu!\n";
            return false;
        }
        std::cout << " -> Cap nhat thanh cong!\n";
        return true;
    }

    // Xóa theo mã.
    // kiemTraRangBuoc (tùy chọn): trả về chuỗi rỗng nếu được phép xóa,
    // ngược lại trả về lý do không được xóa (vd "Khach hang dang duoc gan cho cong X").
    bool xoaTheoMa(const std::string& ma,
                   const std::function<std::string(const T&)>& kiemTraRangBuoc = nullptr) {
        for (auto it = danhSach.begin(); it != danhSach.end(); ++it) {
            if (it->getMaDinhDanh() != ma) continue;

            if (kiemTraRangBuoc) {
                std::string lyDo = kiemTraRangBuoc(*it);
                if (!lyDo.empty()) {
                    std::cout << " -> Khong the xoa: " << lyDo << "\n";
                    return false;
                }
            }

            T banSao = *it;
            auto viTri = it - danhSach.begin();
            danhSach.erase(it);

            if (!luuVaoFile()) {
                danhSach.insert(danhSach.begin() + viTri, banSao);
                std::cout << " -> Khong luu duoc file, da huy thao tac xoa!\n";
                return false;
            }
            std::cout << " -> Xoa thanh cong!\n";
            return true;
        }
        std::cout << " -> Khong tim thay ma: " << ma << "\n";
        return false;
    }

    std::string getDuongDanFile() const {
        return duongDanFile;
    }
};

#endif
