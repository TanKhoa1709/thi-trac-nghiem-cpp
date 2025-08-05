#include "quanlymonhoc.h"
#include <fstream>
#include <sstream>
#include <cstring>

// Constructor
QuanLyMonHoc::QuanLyMonHoc()
{
}

// Destructor
QuanLyMonHoc::~QuanLyMonHoc()
{
    saveToFile();
    danhSachMonHoc.clear();
}

// Get all subjects as dynamic array
void QuanLyMonHoc::danhSach(DynamicArray<MonHoc *> &result)
{
    // Clear the result array before adding
    result.clear();

    for (int i = 0; i < danhSachMonHoc.size(); i++)
    {
        // result.add(&danhSachMonHoc.get(i));
        MonHoc *monHoc = &danhSachMonHoc.get(i);
        result.add(monHoc);
    }
}

// Find subject by code
MonHoc *QuanLyMonHoc::tim(const char *maMon)
{
    for (int i = 0; i < danhSachMonHoc.size(); i++)
    {
        if (std::strcmp(danhSachMonHoc.get(i).getMaMon(), maMon) == 0)
        {
            return &danhSachMonHoc.get(i);
        }
    }
    return nullptr;
}

// Add new subject
bool QuanLyMonHoc::them(MonHoc &monHoc)
{
    if (!monHoc.validate())
    {
        return false;
    }

    // Check if subject code already exists
    if (tim(monHoc.getMaMon()) != nullptr)
    {
        return false;
    }

    danhSachMonHoc.add(monHoc);
    return true;
}

// Update existing subject
bool QuanLyMonHoc::sua(MonHoc &monHoc)
{
    if (!monHoc.validate())
    {
        return false;
    }

    // Find existing subject
    MonHoc *existing = tim(monHoc.getMaMon());
    if (!existing)
    {
        return false; // Subject doesn't exist
    }

    // Update the existing subject's data (only name can be updated, not the code)
    existing->setTenMon(monHoc.getTenMon());

    return true;
}

// Remove subject by code
bool QuanLyMonHoc::xoa(const char *maMon)
{
    MonHoc *subject = tim(maMon);
    if (!subject)
    {
        return false;
    }

    danhSachMonHoc.remove(*subject);
    return true;
}

// Save to file
void QuanLyMonHoc::saveToFile()
{
    std::ofstream file("data/monhoc.txt");

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file for writing: data/monhoc.txt");
    }

    file << danhSachMonHoc.size() << std::endl;

    for (size_t i = 0; i < danhSachMonHoc.size(); i++)
    {
        MonHoc &monHoc = danhSachMonHoc.get(i);
        file << monHoc.getMaMon() << "|" << monHoc.getTenMon() << std::endl;
    }

    file.close();
}

// Load from file
void QuanLyMonHoc::loadFromFile()
{
    std::ifstream file("data/monhoc.txt");

    if (!file.is_open())
    {
        // Create empty data file if it doesn't exist
        std::ofstream createFile("data/monhoc.txt");
        if (createFile.is_open())
        {
            createFile << "0" << std::endl; // Empty file with count = 0
            createFile.close();
        }
        return; // No data to load from empty file
    }

    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        std::string line;
        std::getline(file, line);

        std::stringstream ss(line);
        std::string maMon, tenMon;

        if (std::getline(ss, maMon, '|') && std::getline(ss, tenMon))
        {
            MonHoc *monHoc = new MonHoc(maMon.c_str(), tenMon);
            danhSachMonHoc.add(*monHoc);
        }
    }

    file.close();
}