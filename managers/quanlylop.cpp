#include "quanlylop.h"
#include <fstream>
#include <sstream>

// Constructor
QuanLyLop::QuanLyLop() {
}

// Destructor
QuanLyLop::~QuanLyLop() {
    saveToFile();
    danhSachLop.clear();
}

// Get all classes as dynamic array
void QuanLyLop::danhSach(DynamicArray<Lop *> &result) {
    // Clear the result array before adding
    result.clear();

    for (int i = 0; i < danhSachLop.size(); i++) {
        // result.add(&danhSachLop.get(i));
        Lop *lop = &danhSachLop.get(i);
        result.add(lop);
    }
}

// Find class by code
Lop *QuanLyLop::tim(const std::string &maLop) {
    for (int i = 0; i < danhSachLop.size(); i++) {
        if (danhSachLop.get(i).getMaLop() == maLop) {
            return &danhSachLop.get(i);
        }
    }
    return nullptr;
}

// Add new class
bool QuanLyLop::them(Lop &lop) {
    if (!lop.validate()) {
        return false;
    }

    // Check if class code already exists
    if (tim(lop.getMaLop()) != nullptr) {
        return false;
    }

    danhSachLop.add(lop);
    return true;
}

// Update existing class
bool QuanLyLop::sua(Lop &lop) {
    if (!lop.validate()) {
        return false;
    }

    // Find existing class
    Lop *existing = tim(lop.getMaLop());
    if (!existing) {
        return false; // Class doesn't exist
    }

    // Update the existing class's data (only name can be updated, not the code)
    existing->setTenLop(lop.getTenLop());

    return true;
}

// Remove class by code
bool QuanLyLop::xoa(const std::string &maLop) {
    Lop *lop = tim(maLop);
    if (!lop) {
        return false;
    }

    danhSachLop.remove(*lop);
    return false;
}

// Save to file
void QuanLyLop::saveToFile() {
    std::ofstream file("data/lop.txt");

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: data/lop.txt");
    }

    file << danhSachLop.size() << std::endl;

    for (size_t i = 0; i < danhSachLop.size(); i++) {
        Lop &lop = danhSachLop.get(i);
        file << lop.getMaLop() << "|" << lop.getTenLop() << std::endl;
    }

    file.close();
}

// Load from file
void QuanLyLop::loadFromFile() {
    std::ifstream file("data/lop.txt");

    if (!file.is_open()) {
        // Create empty data file if it doesn't exist
        std::ofstream createFile("data/lop.txt");
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
        std::string maLop, tenLop;

        if (std::getline(ss, maLop, '|') && std::getline(ss, tenLop)) {
            Lop *lop = new Lop(maLop, tenLop);
            danhSachLop.add(*lop);
        }
    }

    file.close();
}
