#ifndef LOP_CO_SO_H
#define LOP_CO_SO_H

#include <string>
#include <iostream>

class LopCoSo {
protected:
    std::string maDinhDanh; 

public:
    LopCoSo() {}
    LopCoSo(std::string ma) : maDinhDanh(ma) {}
    virtual ~LopCoSo() {}

    std::string getMaDinhDanh() const { return maDinhDanh; }
    void setMaDinhDanh(std::string maMoi) { maDinhDanh = maMoi; }

    
    virtual void nhapThongTin() = 0;
    virtual void hienThiThongTin() const = 0;
    virtual std::string chuyenThanhChuoi() const = 0;          
    virtual void docTuChuoi(const std::string& dongDuLieu) = 0; 
};

#endif 
