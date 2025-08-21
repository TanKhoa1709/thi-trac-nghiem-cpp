#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <string>
#include <QString>
#include <QRegularExpression>

class InputValidator {
public:
    // Validate and sanitize username (remove special chars and spaces)
    static QString sanitizeUsername(const QString &input);

    // Validate and sanitize student ID (alphanumeric only, no spaces)
    static QString sanitizeStudentId(const QString &input);

    // Validate and sanitize class code (alphanumeric only, no spaces)
    static QString sanitizeClassCode(const QString &input);

    // Validate and sanitize subject code (alphanumeric only, no spaces, max 15 chars)
    static QString sanitizeSubjectCode(const QString &input);

    // Validate and sanitize names (allow letters, spaces, Vietnamese characters)
    static QString sanitizeName(const QString &input);

    // Remove special characters and leading/trailing spaces
    static QString removeSpecialCharsAndTrim(const QString &input);

    // Check if string contains only alphanumeric characters
    static bool isAlphanumeric(const QString &input);

    // Check if string contains only valid name characters (letters, spaces, Vietnamese)
    static bool isValidName(const QString &input);

    // Validate input length
    static bool isValidLength(const QString &input, int minLength, int maxLength);

    enum InputType {
        USERNAME,
        STUDENT_ID,
        CLASS_CODE,
        SUBJECT_CODE,
        PERSON_NAME,
        GENERAL_TEXT
    };

    // Real-time input filter for QLineEdit (returns filtered string)
    static QString filterInput(const QString &input, InputType type);

private:
    // Helper methods
    static QString removeVietnameseTones(const QString &input);

    static QString keepOnlyAlphanumeric(const QString &input);

    static QString keepValidNameChars(const QString &input);
};

#endif // INPUTVALIDATOR_H
