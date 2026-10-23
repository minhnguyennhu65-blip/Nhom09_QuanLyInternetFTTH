/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp cơ sở (trừu tượng) của mọi đối tượng quản lý.
 *
 * Hợp đồng cho các lớp dẫn xuất:
 *  - docTuChuoi() gán mã bằng setMaDinhDanh() và PHẢI ném ngoại lệ
 *    (std::exception) khi dòng dữ liệu sai định dạng / sai ràng buộc.
 *  - chuyenThanhChuoi() không được trả về chuỗi chứa '\n' hoặc '\r';
 *    các trường ngăn cách bằng '|'.
 *  - capNhatThongTin() không được đổi mã định danh.
 * ======================================================= */

#ifndef LOP_CO_SO_H
#define LOP_CO_SO_H

#include <string>
#include <stdexcept>
#include <cctype>

class LopCoSo {
protected:
    std::string maDinhDanh;

    // Kiểm tra và chuẩn hóa mã: không rỗng, chỉ gồm chữ cái không dấu, chữ số,
    // '_' hoặc '-' (nên không có khoảng trắng, '|' và không bắt đầu bằng '#');
    // trả về mã đã viết hoa. Sai thì ném std::invalid_argument.
    static std::string chuanHoaMa(const std::string& ma) {
        if (ma.empty())
            throw std::invalid_argument("Ma dinh danh khong duoc de trong!");
        std::string ketQua = ma;
        for (char& c : ketQua) {
            unsigned char uc = static_cast<unsigned char>(c);
            if (!(std::isalnum(uc) && uc < 128) && c != '_' && c != '-')
                throw std::invalid_argument(
                    "Ma dinh danh chi gom chu cai khong dau, chu so, '_' hoac '-'!");
            c = static_cast<char>(std::toupper(uc));
        }
        return ketQua;
    }

public:
    // Đối tượng rỗng (mã sẽ được gán sau bằng nhapThongTin / docTuChuoi)
    LopCoSo() = default;

    // Đối tượng có sẵn mã; mã sai thì ném std::invalid_argument
    explicit LopCoSo(const std::string& ma) : maDinhDanh(chuanHoaMa(ma)) {}

    // Hủy ảo: xóa đối tượng dẫn xuất qua con trỏ lớp cơ sở vẫn đúng
    virtual ~LopCoSo() = default;

    // Repository sao chép/gán đối tượng để hoàn tác khi lưu file lỗi
    LopCoSo(const LopCoSo&) = default;
    LopCoSo& operator=(const LopCoSo&) = default;
    LopCoSo(LopCoSo&&) = default;
    LopCoSo& operator=(LopCoSo&&) = default;

    // Lấy mã định danh
    std::string getMaDinhDanh() const {
        return maDinhDanh;
    }

    // Đặt mã định danh; mã sai thì ném std::invalid_argument, mã cũ giữ nguyên.
    // Mã được tự viết hoa.
    void setMaDinhDanh(const std::string& maMoi) {
        maDinhDanh = chuanHoaMa(maMoi);
    }

    // Nhập mới toàn bộ thông tin từ bàn phím
    virtual void nhapThongTin() = 0;

    // In một bản ghi ra màn hình (một dòng căn cột)
    virtual void hienThiThongTin() const = 0;

    // Cập nhật các trường phụ (không được sửa mã)
    virtual void capNhatThongTin() = 0;

    // Chuyển đối tượng thành một dòng, các trường cách nhau bởi '|'
    virtual std::string chuyenThanhChuoi() const = 0;

    // Nạp đối tượng từ một dòng của file; ném ngoại lệ nếu sai
    virtual void docTuChuoi(const std::string& dongDuLieu) = 0;
};

#endif
