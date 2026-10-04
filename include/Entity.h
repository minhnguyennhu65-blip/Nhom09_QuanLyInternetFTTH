#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>

class Entity {
protected:
    std::string maDinhDanh; 

public:
    Entity() {}
    Entity(std::string ma) : maDinhDanh(ma) {}
    virtual ~Entity() {}

    std::string getMaDinhDanh() const { return maDinhDanh; }
    void setMaDinhDanh(std::string maMoi) { maDinhDanh = maMoi; }

    
    virtual void nhapThongTin() = 0;
    virtual void hienThiThongTin() const = 0;
    virtual std::string chuyenThanhChuoi() const = 0;          
    virtual void docTuChuoi(const std::string& dongDuLieu) = 0; 
};

#endif 
