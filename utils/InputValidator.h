#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <string>
#include <QString>
#include <QRegularExpression>

class InputValidator {
public:
    // Filter input for GENERAL type (no special chars, but allows letters, numbers, spaces)
    static QString filterGeneral(const QString &input);

    // Filter input for CODE type (no spaces, no special chars except "-", uppercase)
    static QString filterCode(const QString &input);

    // Check if string is valid for GENERAL type
    static bool isValidGeneral(const QString &input);

    // Check if string is valid for CODE type
    static bool isValidCode(const QString &input);

    // Validate input length
    static bool isValidLength(const QString &input, int minLength, int maxLength);

    enum InputType {
        NONE,        // No validation, freely use
        GENERAL,     // No special chars, but allows letters, numbers, spaces
        CODE         // No spaces, no special chars (except "-"), always uppercase
    };

    // Real-time input filter for QLineEdit (returns filtered string)
    static QString filterInput(const QString &input, InputType type);

private:
    // Helper methods
    static QString keepValidGeneralChars(const QString &input);
    static QString keepValidCodeChars(const QString &input);
};

#endif // INPUTVALIDATOR_H
