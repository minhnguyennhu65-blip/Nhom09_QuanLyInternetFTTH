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
public:
    QuanLyLuuTru(std::string duongDan) : duongDanFile(duongDan) {}
    bool docTuFile(std::vector<T>& danhSach) {
        std::ifstream file(duongDanFile);
        if (!file.is_open()) {
            std::ofstream fileMoi(duongDanFile);
            if (fileMoi.is_open()) {
                fileMoi.close();
            }
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
            } catch (...) {
                continue;
            }
        }
        file.close();
        return true;
    }
    bool luuVaoFile(const std::vector<T>& danhSach) {
        std::ofstream file(duongDanFile, std::ios::trunc); 
        
        if (!file.is_open()) {
            std::cout << " -> Loi: Khong the mo file " << duongDanFile << " de ghi du lieu!\n";
            return false;
        }
        for (const auto& doiTuong : danhSach) {
            file << doiTuong.chuyenThanhChuoi() << "\n";
        }
        file.close();
        return true;
    }
    std::string getDuongDanFile() const {
        return duongDanFile;
    }
};
#endif 
