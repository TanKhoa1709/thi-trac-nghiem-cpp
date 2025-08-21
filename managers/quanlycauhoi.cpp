#include "quanlycauhoi.h"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

// Constructor
QuanLyCauHoi::QuanLyCauHoi(const char *ma) {
    // Copy at most 15 characters to leave space for null-terminator
    std::strncpy(this->maMon, ma, sizeof(this->maMon) - 1);
    this->maMon[15] = '\0'; // Ensure null-termination
}

// Destructor
QuanLyCauHoi::~QuanLyCauHoi() {
    saveToFile();
    cayQuanLyCauHoi.clear();
}

// Get all questions as dynamic array
void QuanLyCauHoi::danhSach(DynamicArray<CauHoi *> &result) {
    // Clear the result array before adding
    result.clear();

    // Use in-order traversal to get questions in sorted order
    cayQuanLyCauHoi.inOrderTraversal([&result](CauHoi &cauHoi) {
        // result.add(&cauHoi);
        CauHoi *question = &cauHoi;
        result.add(question);
    });
}

// Find question by ID
CauHoi *QuanLyCauHoi::tim(int maCauHoi) {
    // Create a temporary question with the ID to search for
    CauHoi query;
    query.setMaCauHoi(maCauHoi);
    return cayQuanLyCauHoi.find(query);
}

// Add new question
bool QuanLyCauHoi::them(CauHoi &cauHoi) {
    if (!cauHoi.validate()) {
        return false;
    }

    // Check if question ID already exists
    if (tim(cauHoi.getMaCauHoi()) != nullptr) {
        return false;
    }

    cayQuanLyCauHoi.add(cauHoi);
    return true;
}

// Update existing question
bool QuanLyCauHoi::sua(CauHoi &cauHoi) {
    if (!cauHoi.validate()) {
        return false;
    }

    // Find existing question
    CauHoi *existing = tim(cauHoi.getMaCauHoi());
    if (!existing) {
        return false; // Question doesn't exist
    }

    // Update the existing question's data (ID cannot be changed)
    existing->setNoiDung(cauHoi.getNoiDung());
    existing->setLuaChonA(cauHoi.getLuaChonA());
    existing->setLuaChonB(cauHoi.getLuaChonB());
    existing->setLuaChonC(cauHoi.getLuaChonC());
    existing->setLuaChonD(cauHoi.getLuaChonD());
    existing->setDapAnDung(cauHoi.getDapAnDung());

    return true;
}

// Remove question by ID
bool QuanLyCauHoi::xoa(int maCauHoi) {
    CauHoi *question = tim(maCauHoi);
    if (!question) {
        return false;
    }

    cayQuanLyCauHoi.remove(*question);
    return true;
}

// Generate random unique question ID
int QuanLyCauHoi::taoMaCauHoiNgauNhien() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 999999);

    int newId;
    do {
        newId = dis(gen);
    } while (tim(newId) != nullptr); // Ensure uniqueness

    return newId;
}

int QuanLyCauHoi::taoMaCauHoiMoi(int minId, int maxId) {
    if (minId < 1 || maxId < minId) {
        throw std::invalid_argument("Loi tao ma cau hoi: minId phai >= 1 va maxId phai >= minId");
    }

    // Hàm đệ quy tìm ID trống gần "trung vị" nhất
    std::function<int(int, int)> findId = [&](int low, int high) -> int {
        if (low > high)
            return -1;

        int mid = low + (high - low) / 2; // Tránh tràn số nguyên

        if (this->tim(mid) == nullptr) {
            return mid; // Tìm thấy mã chưa tồn tại
        }

        // Thử tìm bên trái trước
        int leftId = findId(low, mid - 1);
        if (leftId != -1)
            return leftId;

        // Nếu không có bên trái thì tìm bên phải
        return findId(mid + 1, high);
    };

    int newId = findId(minId, maxId);
    if (newId == -1) {
        throw std::runtime_error("Khong con ID trong pham vi cho phep");
    }
    return newId;
}

// Get random questions
void QuanLyCauHoi::layNgauNhien(DynamicArray<CauHoi *> &result, int soLuong) {
    DynamicArray<CauHoi *> allQuestions;

    danhSach(allQuestions);

    if (soLuong >= allQuestions.size()) {
        for (int i = 0; i < allQuestions.size(); i++) {
            result.add(allQuestions.get(i));
        }
        return;
    }

    // Create indices array and shuffle
    DynamicArray<int> indices;
    for (int i = 0; i < allQuestions.size(); i++) {
        indices.add(i);
    }

    // Use proper random seeding with current time
    std::random_device rd;
    std::mt19937 gen(rd());

    // Add additional entropy using current time
    auto now = std::chrono::high_resolution_clock::now();
    auto time_seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    gen.seed(static_cast<unsigned int>(time_seed ^ rd()));

    // Fisher-Yates shuffle algorithm for better randomness
    for (int i = indices.size() - 1; i > 0; i--) {
        std::uniform_int_distribution<> dis(0, i);
        int j = dis(gen);

        // Swap indices[i] and indices[j]
        int temp = indices.get(i);
        indices.set(i, indices.get(j));
        indices.set(j, temp);
    }

    // Take first soLuong questions
    for (int i = 0; i < soLuong; i++) {
        int index = indices.get(i);
        result.add(allQuestions.get(index));
    }

    return;
}

// Save to file
void QuanLyCauHoi::saveToFile() {
    std::string maMonStr(this->maMon);
    std::string filename = "data/cauhoi/cauhoi_" + maMonStr + ".txt";
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    DynamicArray<CauHoi *> allQuestions;
    danhSach(allQuestions);
    file << allQuestions.size() << std::endl;

    for (int i = 0; i < allQuestions.size(); i++) {
        CauHoi *question = allQuestions.get(i);
        file << question->getMaCauHoi() << "|" << question->getNoiDung() << "|" << question->getLuaChonA() << "|"
             << question->getLuaChonB() << "|" << question->getLuaChonC() << "|" << question->getLuaChonD() << "|"
             << question->getDapAnDung() << std::endl;
    }

    file.close();
}

// Load from file
void QuanLyCauHoi::loadFromFile() {
    std::string filename = "data/cauhoi/cauhoi_" + std::string(this->maMon) + ".txt";
    std::ifstream file(filename);

    if (!file.is_open()) {
        // Create empty data file if it doesn't exist
        std::ofstream createFile(filename);
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

        while (std::getline(ss, token, '|')) {
            tokens.add(token);
        }

        if (tokens.size() == 7) {
            int maCauHoi = std::stoi(tokens.get(0));
            std::string noiDung = tokens.get(1);
            std::string luaChonA = tokens.get(2);
            std::string luaChonB = tokens.get(3);
            std::string luaChonC = tokens.get(4);
            std::string luaChonD = tokens.get(5);
            char dapAnDung = tokens.get(6)[0];

            CauHoi *question = new CauHoi(maCauHoi, noiDung, luaChonA, luaChonB, luaChonC, luaChonD, dapAnDung);
            cayQuanLyCauHoi.add(*question);
        }
    }

    file.close();
}

// Check if question is used in exams (placeholder implementation)
bool QuanLyCauHoi::kiemTraCauHoiDaSuDung(int maCauHoi) {
    // TODO: Implement logic to check if question is used in any exam
    // For now, return false (allowing deletion)
    return false;
}
