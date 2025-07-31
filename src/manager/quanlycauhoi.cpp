#include "manager/quanlycauhoi.h"
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

// Constructor
QuanLyCauHoi::QuanLyCauHoi(const char* maMon) {
    std::strncpy(this->maMon, maMon, 15);
    this->maMon[15] = '\0';
    
    // Set up comparison function for BST (compare by question ID)
    cayQuanLyCauHoi = BinarySearchTree<CauHoi*>();
    
    loadFromFile();
}

// Destructor
QuanLyCauHoi::~QuanLyCauHoi() {
    saveToFile();
    
    // Clean up all question pointers
    auto allQuestions = cayQuanLyCauHoi.inOrder();
    for (CauHoi* question : allQuestions) {
        delete question;
    }
    cayQuanLyCauHoi.clear();
}

// Get all questions as dynamic array
DynamicArray<CauHoi*> QuanLyCauHoi::danhSach() {
    DynamicArray<CauHoi*> result;
    auto questions = cayQuanLyCauHoi.inOrder();
    
    for (CauHoi* question : questions) {
        result.push_back(question);
    }
    
    return result;
}

// Find question by ID
CauHoi* QuanLyCauHoi::tim(int maCauHoi) {
    for (CauHoi* question : cayQuanLyCauHoi.inOrder()) {
        if (question->getMaCauHoi() == maCauHoi) {
            return question;
        }
    }

    return nullptr; // Not found
}

// Add new question
bool QuanLyCauHoi::them(CauHoi* cauHoi) {
    if (!cauHoi || !cauHoi->validate()) {
        return false;
    }
    
    // Check if question ID already exists
    if (tim(cauHoi->getMaCauHoi()) != nullptr) {
        return false;
    }
    
    return cayQuanLyCauHoi.insert(cauHoi);
}

// Update existing question
bool QuanLyCauHoi::sua(CauHoi* cauHoi) {
    if (!cauHoi || !cauHoi->validate()) {
        return false;
    }
    
    // Find existing question
    CauHoi* existing = tim(cauHoi->getMaCauHoi());
    if (!existing) {
        return false; // Question doesn't exist
    }
    
    // Update the existing question's data
    existing->setNoiDung(cauHoi->getNoiDung());
    existing->setLuaChonA(cauHoi->getLuaChonA());
    existing->setLuaChonB(cauHoi->getLuaChonB());
    existing->setLuaChonC(cauHoi->getLuaChonC());
    existing->setLuaChonD(cauHoi->getLuaChonD());
    existing->setDapAnDung(cauHoi->getDapAnDung());
    
    return true;
}

// Remove question by ID
bool QuanLyCauHoi::xoa(int maCauHoi) {
    CauHoi* question = tim(maCauHoi);
    if (!question) {
        return false;
    }
    
    // TODO: Check if question is used in any exam
    if (kiemTraCauHoiDaSuDung(maCauHoi)) {
        return false;
    }
    
    bool removed = cayQuanLyCauHoi.remove(question);
    if (removed) {
        delete question;
    }
    
    return removed;
}

// Generate random unique question ID
int QuanLyCauHoi::taoMaCauHoiNgauNhien() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1000, 999999);
    
    int newId;
    do {
        newId = dis(gen);
    } while (tim(newId) != nullptr);
    
    return newId;
}

// Get random questions
DynamicArray<CauHoi*> QuanLyCauHoi::layNgauNhien(int soLuong) {
    DynamicArray<CauHoi*> result;
    auto allQuestions = cayQuanLyCauHoi.inOrder();
    
    if (soLuong >= static_cast<int>(allQuestions.size())) {
        // Return all questions if requested more than available
        for (CauHoi* question : allQuestions) {
            result.push_back(question);
        }
        return result;
    }
    
    // Shuffle and take first soLuong questions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(allQuestions.begin(), allQuestions.end(), gen);
    
    for (int i = 0; i < soLuong; i++) {
        result.push_back(allQuestions[i]);
    }
    
    return result;
}

// Save to file
void QuanLyCauHoi::saveToFile() {
    std::string filename = "data/cauhoi/cauhoi_" + std::string(maMon) + ".txt";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return;
    }
    
    auto questions = cayQuanLyCauHoi.inOrder();
    file << questions.size() << std::endl;
    
    for (CauHoi* question : questions) {
        file << question->getMaCauHoi() << "|"
             << question->getNoiDung() << "|"
             << question->getLuaChonA() << "|"
             << question->getLuaChonB() << "|"
             << question->getLuaChonC() << "|"
             << question->getLuaChonD() << "|"
             << question->getDapAnDung() << std::endl;
    }
    
    file.close();
}

// Load from file
void QuanLyCauHoi::loadFromFile() {
    std::string filename = "data/cauhoi/cauhoi_" + std::string(maMon) + ".txt";
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        return; // File doesn't exist yet, that's okay
    }
    
    int count;
    file >> count;
    file.ignore(); // Ignore newline after count
    
    for (int i = 0; i < count; i++) {
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 7) {
            int id = std::stoi(tokens[0]);
            char answer = tokens[6].empty() ? 'A' : tokens[6][0];
            
            CauHoi* question = new CauHoi(id, tokens[1], tokens[2], 
                                         tokens[3], tokens[4], tokens[5], answer);
            cayQuanLyCauHoi.insert(question);
        }
    }
    
    file.close();
}

// Check if question is used in exams (placeholder implementation)
bool QuanLyCauHoi::kiemTraCauHoiDaSuDung(int maCauHoi) {
    // TODO: Implement check against exam records
    // For now, return false to allow deletion
    return false;
}