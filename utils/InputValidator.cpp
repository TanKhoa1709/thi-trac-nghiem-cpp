#include "InputValidator.h"
#include <QRegularExpression>
#include <QStringList>

QString InputValidator::sanitizeUsername(const QString &input) {
    QString result = input.trimmed();
    
    // Remove special characters, keep only alphanumeric
    result = keepOnlyAlphanumeric(result);
    
    // Ensure it's not empty and within reasonable length
    if (result.length() > 50) {
        result = result.left(50);
    }
    
    return result;
}

QString InputValidator::sanitizeStudentId(const QString &input) {
    QString result = input.trimmed().toUpper();
    
    // Remove special characters and spaces, keep only alphanumeric
    result = keepOnlyAlphanumeric(result);
    
    // Limit length for student ID (typical student ID format)
    if (result.length() > 20) {
        result = result.left(20);
    }
    
    return result;
}

QString InputValidator::sanitizeClassCode(const QString &input) {
    QString result = input.trimmed().toUpper();
    
    // Remove special characters and spaces, keep only alphanumeric
    result = keepOnlyAlphanumeric(result);
    
    // Limit length for class code
    if (result.length() > 20) {
        result = result.left(20);
    }
    
    return result;
}

QString InputValidator::sanitizeSubjectCode(const QString &input) {
    QString result = input.trimmed().toUpper();
    
    // Remove special characters and spaces, keep only alphanumeric
    result = keepOnlyAlphanumeric(result);
    
    // Subject code has a limit of 15 characters (as per MonHoc class)
    if (result.length() > 15) {
        result = result.left(15);
    }
    
    return result;
}

QString InputValidator::sanitizeName(const QString &input) {
    QString result = input.trimmed();
    
    // Keep valid name characters (letters, spaces, Vietnamese characters)
    result = keepValidNameChars(result);
    
    // Normalize multiple spaces to single space
    result = result.replace(QRegularExpression("\\s+"), " ");
    
    // Limit length
    if (result.length() > 100) {
        result = result.left(100);
    }
    
    return result;
}

QString InputValidator::removeSpecialCharsAndTrim(const QString &input) {
    QString result = input.trimmed();
    
    // Remove common special characters but keep letters, numbers, and basic punctuation
    QRegularExpression specialChars("[^\\w\\s\\-\\.]");
    result = result.remove(specialChars);
    
    return result;
}

bool InputValidator::isAlphanumeric(const QString &input) {
    if (input.isEmpty()) return false;
    
    QRegularExpression alphanumeric("^[a-zA-Z0-9\\-]+$");
    return alphanumeric.match(input).hasMatch();
}

bool InputValidator::isValidName(const QString &input) {
    if (input.trimmed().isEmpty()) return false;
    
    // Allow letters (including Vietnamese), spaces, hyphens, and dots
    QRegularExpression namePattern("^[a-zA-ZàáạảãâầấậẩẫăằắặẳẵèéẹẻẽêềếệểễìíịỉĩòóọỏõôồốộổỗơờớợởỡùúụủũưừứựửữỳýỵỷỹđĐ\\s\\-\\.]+$");
    return namePattern.match(input.trimmed()).hasMatch();
}

bool InputValidator::isValidLength(const QString &input, int minLength, int maxLength) {
    int length = input.trimmed().length();
    return length >= minLength && length <= maxLength;
}

QString InputValidator::filterInput(const QString &input, InputType type) {
    switch (type) {
        case USERNAME:
            return sanitizeUsername(input);
        case STUDENT_ID:
            return sanitizeStudentId(input);
        case CLASS_CODE:
            return sanitizeClassCode(input);
        case SUBJECT_CODE:
            return sanitizeSubjectCode(input);
        case PERSON_NAME:
            return sanitizeName(input);
        case GENERAL_TEXT:
            return removeSpecialCharsAndTrim(input);
        default:
            return input.trimmed();
    }
}

// Private helper methods
QString InputValidator::removeVietnameseTones(const QString &input) {
    QString result = input;
    
    // Vietnamese character replacement map
    QStringList vietnamese = {"àáạảãâầấậẩẫăằắặẳẵ", "èéẹẻẽêềếệểễ", "ìíịỉĩ", "òóọỏõôồốộổỗơờớợởỡ", "ùúụủũưừứựửữ", "ỳýỵỷỹ", "đ"};
    QStringList english = {"a", "e", "i", "o", "u", "y", "d"};
    
    for (int i = 0; i < vietnamese.size(); i++) {
        QString vnChars = vietnamese[i];
        QString enChar = english[i];
        for (int j = 0; j < vnChars.length(); j++) {
            result = result.replace(vnChars[j], enChar);
            result = result.replace(vnChars[j].toUpper(), enChar.toUpper());
        }
    }
    
    return result;
}

QString InputValidator::keepOnlyAlphanumeric(const QString &input) {
    QString result;
    for (QChar c : input) {
        if (c.isLetterOrNumber() || c == '-') {
            result += c;
        }
    }
    return result;
}

QString InputValidator::keepValidNameChars(const QString &input) {
    QString result;
    for (QChar c : input) {
        // Keep letters (including Vietnamese), spaces, hyphens, and dots
        if (c.isLetter() || c.isSpace() || c == '-' || c == '.') {
            result += c;
        }
    }
    return result;
}
