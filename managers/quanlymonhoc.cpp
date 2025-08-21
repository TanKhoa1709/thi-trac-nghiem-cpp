#include "quanlymonhoc.h"
#include <fstream>
#include <sstream>
#include <cstring>

// Constructor
QuanLyMonHoc::QuanLyMonHoc() {
}

// Destructor
QuanLyMonHoc::~QuanLyMonHoc() {
    saveToFile();
    danhSachMonHoc.clear();
}

// Get all subjects as dynamic array
void QuanLyMonHoc::danhSach(DynamicArray<MonHoc *> &result) {
    // Clear the result array before adding
    result.clear();

    for (int i = 0; i < danhSachMonHoc.size(); i++) {
        // result.add(&danhSachMonHoc.get(i));
        MonHoc *monHoc = &danhSachMonHoc.get(i);
        result.add(monHoc);
    }
}

// Find subject by code
MonHoc *QuanLyMonHoc::tim(const char *maMon) {
    for (int i = 0; i < danhSachMonHoc.size(); i++) {
        if (std::strcmp(danhSachMonHoc.get(i).getMaMon(), maMon) == 0) {
            return &danhSachMonHoc.get(i);
        }
    }
    return nullptr;
}

// Add new subject
bool QuanLyMonHoc::them(MonHoc &monHoc) {
    if (!monHoc.validate()) {
        return false;
    }

    // Check if subject code already exists
    if (tim(monHoc.getMaMon()) != nullptr) {
        return false;
    }

    // Auto-assign subject index if not set
    if (monHoc.getSubjectIndex() < 0) {
        monHoc.setSubjectIndex(danhSachMonHoc.size());
    }

    danhSachMonHoc.add(monHoc);
    return true;
}

// Update existing subject
bool QuanLyMonHoc::sua(MonHoc &monHoc) {
    if (!monHoc.validate()) {
        return false;
    }

    // Find existing subject
    MonHoc *existing = tim(monHoc.getMaMon());
    if (!existing) {
        return false; // Subject doesn't exist
    }

    // Update the existing subject's data (only name can be updated, not the code)
    existing->setTenMon(monHoc.getTenMon());

    return true;
}

// Remove subject by code
bool QuanLyMonHoc::xoa(const char *maMon) {
    MonHoc *subject = tim(maMon);
    if (!subject) {
        return false;
    }

    danhSachMonHoc.remove(*subject);
    return true;
}

// Save to file
void QuanLyMonHoc::saveToFile() {
    std::ofstream file("data/monhoc.txt");

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: data/monhoc.txt");
    }

    file << danhSachMonHoc.size() << std::endl;

    for (size_t i = 0; i < danhSachMonHoc.size(); i++) {
        MonHoc &monHoc = danhSachMonHoc.get(i);
        file << monHoc.getMaMon() << "|" << monHoc.getTenMon() << "|" 
             << monHoc.getSubjectIndex() << std::endl;
    }

    file.close();
}

// Load from file
void QuanLyMonHoc::loadFromFile() {
    std::ifstream file("data/monhoc.txt");

    if (!file.is_open()) {
        // Create empty data file if it doesn't exist
        std::ofstream createFile("data/monhoc.txt");
        if (createFile.is_open()) {
            createFile << "0" << std::endl; // Empty file with count = 0
            createFile.close();
        }
        return; // No data to load from empty file
    }

    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++) {
        std::string line;
        std::getline(file, line);

        std::stringstream ss(line);
        std::string token;
        DynamicArray<std::string> tokens;

        // Split by |
        while (std::getline(ss, token, '|')) {
            tokens.add(token);
        }

        if (tokens.size() >= 3) {
            // Format mới: maMon|tenMon|subjectIndex
            std::string maMon = tokens.get(0);
            std::string tenMon = tokens.get(1);
            int subjectIndex = std::stoi(tokens.get(2));
            MonHoc *monHoc = new MonHoc(maMon.c_str(), tenMon, subjectIndex);
            danhSachMonHoc.add(*monHoc);
        } else if (tokens.size() >= 2) {
            // Backward compatibility: maMon|tenMon (format cũ)
            std::string maMon = tokens.get(0);
            std::string tenMon = tokens.get(1);
            // Tự động assign subjectIndex dựa trên thứ tự hiện tại
            int subjectIndex = danhSachMonHoc.size();
            MonHoc *monHoc = new MonHoc(maMon.c_str(), tenMon, subjectIndex);
            danhSachMonHoc.add(*monHoc);
        }
    }

    file.close();
}
