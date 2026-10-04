#ifndef NHAP_DU_LIEU_H
#define NHAP_DU_LIEU_H

#include <iostream>
#include <string>
#include <limits>

class NhapDuLieu {
public:
   
    static int nhapSoNguyen(std::string thongBao) {
        int giaTri;
        while (true) {
            std::cout << thongBao;
            if (std::cin >> giaTri) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return giaTri;
            }
            std::cout << " -> Loi: Vui long nhap mot so nguyen hop le!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    static double nhapSoThuc(std::string thongBao) {
        double giaTri;
        while (true) {
            std::cout << thongBao;
            if (std::cin >> giaTri) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return giaTri;
            }
            std::cout << " -> Loi: Vui long nhap mot so thuc hop le!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    static std::string nhapChuoi(std::string thongBao) {
        std::string chuoi;
        while (true) {
            std::cout << thongBao;
            std::getline(std::cin, chuoi);
            if (!chuoi.empty()) return chuoi;
            std::cout << " -> Loi: Thong tin khong duoc de trong!\n";
        }
    }
    static bool xacNhan(std::string thongBao) {
        std::string luaChon;
        while (true) {
            std::cout << thongBao << " (y/n): ";
            std::getline(std::cin, luaChon);
            if (luaChon == "y" || luaChon == "Y") return true;
            if (luaChon == "n" || luaChon == "N") return false;
            std::cout << " -> Vui long chi nhap 'y' (Dong y) hoac 'n' (Huy bo).\n";
        }
    }
};

#endif 
