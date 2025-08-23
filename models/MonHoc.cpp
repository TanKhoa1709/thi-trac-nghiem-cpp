#include "MonHoc.h"
#include "../managers/QuanLyCauHoi.h"
#include <cstring>
#include <iostream>

// Default constructor
MonHoc::MonHoc() : tenMon(""), subjectIndex(0), quanLyCauHoi(nullptr) {
    std::memset(maMon, 0, sizeof(maMon));
}

// Parameterized constructor with subjectIndex
MonHoc::MonHoc(const char *ma, const std::string &ten, int subjectIndex)
    : tenMon(ten), subjectIndex(subjectIndex) {
    setMaMon(ma);
    int minId, maxId;
    calculateBoundaryForIndex(subjectIndex, minId, maxId);
    quanLyCauHoi = new QuanLyCauHoi(ma, minId, maxId);
}

// // Parameterized constructor without subjectIndex (will be auto-assigned)
// MonHoc::MonHoc(const char *ma, const std::string &ten)
//     : tenMon(ten), subjectIndex(-1), minId(1), maxId(10000), quanLyCauHoi(nullptr) {
//     setMaMon(ma);
//     // Note: boundary calculation and QuanLyCauHoi creation will be done 
//     // when subjectIndex is set via setSubjectIndex()
// }

// Destructor
MonHoc::~MonHoc() {
    delete quanLyCauHoi;
    quanLyCauHoi = nullptr;
}

// Set subject code with validation
void MonHoc::setMaMon(const char *ma) {
    if (ma != nullptr) {
        std::strncpy(maMon, ma, 15);
        maMon[15] = '\0'; // Ensure null termination
    } else {
        std::memset(maMon, 0, sizeof(maMon));
    }
}

// Get minId calculated from subjectIndex
int MonHoc::getMinId() const {
    return BOUNDARY_START + subjectIndex * QUESTIONS_PER_SUBJECT;
}

// Get maxId calculated from subjectIndex
int MonHoc::getMaxId() const {
    return getMinId() + QUESTIONS_PER_SUBJECT - 1;
}

// Set subject index and recalculate boundary
void MonHoc::setSubjectIndex(int index) {
    subjectIndex = index;
    
    // Recreate QuanLyCauHoi with new boundary if it doesn't exist
    if (quanLyCauHoi == nullptr) {
        int minId, maxId;
        calculateBoundaryForIndex(subjectIndex, minId, maxId);
        quanLyCauHoi = new QuanLyCauHoi(maMon, minId, maxId);
    }
}

// Calculate boundary for given index (static method)
void MonHoc::calculateBoundaryForIndex(int index, int &minId, int &maxId) {
    minId = BOUNDARY_START + index * QUESTIONS_PER_SUBJECT;
    maxId = minId + QUESTIONS_PER_SUBJECT - 1;
}

// Validate subject data
bool MonHoc::validate() const {
    // Check if subject code is not empty
    if (std::strlen(maMon) == 0) {
        return false;
    }

    // Check if subject name is not empty
    if (tenMon.empty()) {
        return false;
    }

    // Check if subject index is valid
    if (subjectIndex < 0) {
        return false;
    }

    return true;
}
