#include <fstream>
#include <sstream>
#include <vector>
#include "QuanLyDiem.h"
#include "../utils/LinkedList.h"
#include <iostream>

// Constructor
QuanLyDiem::QuanLyDiem(const std::string &maSinhVien)
    : maSinhVien(maSinhVien) {
}

// Destructor
QuanLyDiem::~QuanLyDiem() {
    saveToFile();
    quanLyDiem.clear();
}

// Get all scores as dynamic array
void QuanLyDiem::danhSach(DynamicArray<DiemThi *> &result) {
    // Clear the result array before adding
    result.clear();

    for (int i = 0; i < quanLyDiem.size(); i++) {
        DiemThi *score = &quanLyDiem.get(i);
        result.add(score);
    }
}

// Find score by subject code
DiemThi *QuanLyDiem::tim(const char *maMon) {
    for (int i = 0; i < quanLyDiem.size(); i++) {
        if (std::strcmp(quanLyDiem.get(i).getMaMon(), maMon) == 0) {
            return &quanLyDiem.get(i);
        }
    }
    return nullptr;
}

// Add new score
bool QuanLyDiem::them(DiemThi &diem) {
    if (!diem.validate()) {
        return false;
    }

    // Check if score for this subject already exists
    if (tim(diem.getMaMon()) != nullptr) {
        return false;
    }

    quanLyDiem.add(diem);
    return true;
}

// Update existing score
bool QuanLyDiem::sua(DiemThi &diem) {
    if (!diem.validate()) {
        return false;
    }

    // Find existing score
    DiemThi *existing = tim(diem.getMaMon());
    if (!existing) {
        return false; // Score doesn't exist
    }

    // Update the existing score's data (subject code cannot be changed)
    existing->setDiem(diem.getDiem());
    existing->setDanhSachCauTraLoi(diem.getDanhSachCauTraLoi());
    existing->setDanhSachCauHoi(diem.getDanhSachCauHoi());

    return true;
}

// Remove score by subject code
bool QuanLyDiem::xoa(const char *maMon) {
    DiemThi *score = tim(maMon);
    if (!score) {
        return false;
    }

    quanLyDiem.remove(*score);
    return true;
}

// Calculate average score
double QuanLyDiem::tinhDiemTrungBinh() {
    if (quanLyDiem.isEmpty()) {
        return 0.0;
    }

    double total = 0.0;
    int count = 0;

    for (int i = 0; i < quanLyDiem.size(); i++) {
        DiemThi &score = quanLyDiem.get(i);
        total += score.getDiem();
        count++;
    }

    return count > 0 ? total / count : 0.0;
}

// Count passed subjects (score >= 5.0)
int QuanLyDiem::demSoMonDau() {
    int count = 0;

    for (int i = 0; i < quanLyDiem.size(); i++) {
        DiemThi &score = quanLyDiem.get(i);
        if (score.getDiem() >= 5.0) {
            count++;
        }
    }

    return count;
}

// Count failed subjects (score < 5.0)
int QuanLyDiem::demSoMonRot() {
    int count = 0;

    for (int i = 0; i < quanLyDiem.size(); i++) {
        DiemThi &score = quanLyDiem.get(i);
        if (score.getDiem() < 5.0) {
            count++;
        }
    }

    return count;
}

// Count total subjects taken
int QuanLyDiem::demSoMonDaThi() {
    return quanLyDiem.size();
}

// Save to file
void QuanLyDiem::saveToFile() {
    std::string filename = "data/diemthi/diemthi_" + maSinhVien + ".txt";
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file << quanLyDiem.size() << std::endl;

    for (int i = 0; i < quanLyDiem.size(); i++) {
        DiemThi &score = quanLyDiem.get(i);
        file << score.getMaMon() << "|";
        file << score.getDiem() << "|";

        // Save the answer list as a string of characters
        DynamicArray<char> *answers = score.getDanhSachCauTraLoi();
        for (int j = 0; j < answers->size(); j++) {
            file << answers->get(j);
        }
        file << "|";

        // Save the question list as comma-separated integers
        DynamicArray<int> *questions = score.getDanhSachCauHoi();
        for (int j = 0; j < questions->size(); j++) {
            file << questions->get(j);
            if (j < questions->size() - 1) {
                file << ",";
            }
        }
        file << std::endl;
    }

    file.close();
}

// Load from file
void QuanLyDiem::loadFromFile() {
    std::string filename = "data/diemthi/diemthi_" + maSinhVien + ".txt";
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

        if (tokens.size() >= 3) {
            std::string subjectCode = tokens.get(0);
            double score = std::stod(tokens.get(1));
            std::string answersStr = tokens.get(2);

            // Convert string back to DynamicArray<char>
            DynamicArray<char> answers;
            for (char c: answersStr) {
                answers.add(c);
            }

            DiemThi *examScore = new DiemThi(subjectCode.c_str(), score);
            for (int j = 0; j < answers.size(); j++) {
                examScore->getDanhSachCauTraLoi()->add(answers.get(j));
            }

            // Load question list if available (tokens.size() >= 4)
            if (tokens.size() >= 4) {
                std::string questionsStr = tokens.get(3);
                std::stringstream qss(questionsStr);
                std::string questionToken;
                
                while (std::getline(qss, questionToken, ',')) {
                    if (!questionToken.empty()) {
                        int questionId = std::stoi(questionToken);
                        examScore->getDanhSachCauHoi()->add(questionId);
                    }
                }
            }

            quanLyDiem.add(*examScore);
        }
    }

    std::cout << "Load " << quanLyDiem.size() << " diem thanh cong!\n";

    file.close();
}
