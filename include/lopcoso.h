/* =======================================================
 * Tên tác giả: Nguyễn Như Minh
 * Mã sinh viên: B24DCVT253
 * Mô tả file: Lớp cơ sở 
 * ======================================================= */

#ifndef LOP_CO_SO_H
#define LOP_CO_SO_H

#include <string>

// Lớp cơ sở chứa thuộc tính dùng chung và định nghĩa các hàm CRUD bắt buộc
class LopCoSo {
protected:
    std::string maDinhDanh;
public:
    // Khởi tạo đối tượng rỗng
    LopCoSo() : maDinhDanh("") {}
    
    // Khởi tạo đối tượng có sẵn mã định danh
    LopCoSo(const std::string& ma) : maDinhDanh(ma) {}
    
    // Hàm hủy ảo để đảm bảo giải phóng bộ nhớ đúng cách cho lớp kế thừa
    virtual ~LopCoSo() {}
    
    // Lấy mã định danh của đối tượng
    std::string getMaDinhDanh() const {
        return maDinhDanh;
    }
    
    // Thiết lập mã định danh mới (chỉ gán khi chuỗi không rỗng)
    void setMaDinhDanh(const std::string& maMoi) {
        if (!maMoi.empty()) {
            maDinhDanh = maMoi;
        }
    }
    
    // Hàm thuần ảo: Yêu cầu lớp con tự triển khai luồng nhập dữ liệu mới
    virtual void nhapThongTin() = 0;
    
    // Hàm thuần ảo: Yêu cầu lớp con tự triển khai luồng in dữ liệu ra màn hình
    virtual void hienThiThongTin() const = 0;
    
    // Hàm thuần ảo: Yêu cầu lớp con tự triển khai luồng cập nhật thông tin phụ (không sửa mã)
    virtual void capNhatThongTin() = 0;                        
    
    // Hàm thuần ảo: Chuyển dữ liệu đối tượng thành chuỗi cách nhau bởi ký tự '|' để lưu file
    virtual std::string chuyenThanhChuoi() const = 0;
    
    // Hàm thuần ảo: Cắt chuỗi đọc từ file để nạp dữ liệu ngược lại vào các thuộc tính của đối tượng
    virtual void docTuChuoi(const std::string& dongDuLieu) = 0;
};

#endif
