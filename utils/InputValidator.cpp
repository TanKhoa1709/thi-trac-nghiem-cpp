#include "InputValidator.h"
#include <QRegularExpression>
#include <QStringList>

QString InputValidator::filterGeneral(const QString &input) {
    QString result = input;
    
    // Keep only letters, numbers, spaces, and some basic punctuation
    result = keepValidGeneralChars(result);
    
    // Normalize multiple spaces to single space
    result = result.replace(QRegularExpression("\\s+"), " ");
    
    return result;
}

QString InputValidator::filterCode(const QString &input) {
    QString result = input.toUpper();
    
    // Keep only letters, numbers, and hyphens (no spaces)
    result = keepValidCodeChars(result);
    
    return result;
}

bool InputValidator::isValidGeneral(const QString &input) {
    if (input.isEmpty()) return true; // Empty is valid for GENERAL
    
    // Allow letters, numbers, spaces, hyphens, dots, and Vietnamese characters
    QRegularExpression generalPattern("^[a-zA-Z0-9àáạảãâầấậẩẫăằắặẳẵèéẹẻẽêềếệểễìíịỉĩòóọỏõôồốộổỗơờớợởỡùúụủũưừứựửữỳýỵỷỹđĐ\\s\\-\\.]+$");
    return generalPattern.match(input).hasMatch();
}

bool InputValidator::isValidCode(const QString &input) {
    if (input.isEmpty()) return true; // Empty is valid for CODE
    
    // Allow only letters, numbers, and hyphens (no spaces, no special chars)
    QRegularExpression codePattern("^[A-Z0-9\\-]+$");
    return codePattern.match(input).hasMatch();
}

bool InputValidator::isValidLength(const QString &input, int minLength, int maxLength) {
    int length = input.trimmed().length();
    return length >= minLength && length <= maxLength;
}

QString InputValidator::filterInput(const QString &input, InputType type) {
    switch (type) {
        case NONE:
            return input; // No filtering, freely use
        case GENERAL:
            return filterGeneral(input);
        case CODE:
            return filterCode(input);
        default:
            return input;
    }
}

// Private helper methods
QString InputValidator::keepValidGeneralChars(const QString &input) {
    QString result;
    for (QChar c : input) {
        // Keep letters (including Vietnamese), numbers, spaces, hyphens, and dots
        if (c.isLetter() || c.isNumber() || c.isSpace() || c == '-' || c == '.') {
            result += c;
        }
    }
    return result;
}

QString InputValidator::keepValidCodeChars(const QString &input) {
    QString result;
    for (QChar c : input) {
        // Keep only letters, numbers, and hyphens (no spaces, no special chars)
        if (c.isLetterOrNumber() || c == '-') {
            result += c;
        }
    }
    return result;
}
