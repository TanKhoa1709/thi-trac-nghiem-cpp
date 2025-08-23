#include "InputValidator.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QObject>

void InputValidator::setupInputValidation(QLineEdit *lineEdit, InputType type) {
    if (!lineEdit)
        return;

    // Store validation type as property
    lineEdit->setProperty("validationType", static_cast<int>(type));

    // Set input mask and validation based on type
    switch (type) {
        case NONE:
            lineEdit->setMaxLength(500);
            lineEdit->setPlaceholderText("Enter text (no restrictions)");
            break;

        case GENERAL:
            lineEdit->setMaxLength(200);
            lineEdit->setPlaceholderText("Enter text (letters, numbers, spaces allowed)");
            break;

        case CODE:
            lineEdit->setMaxLength(50);
            lineEdit->setPlaceholderText("Enter code (uppercase, no spaces)");
            break;
    }

    // Connect text changed signal for real-time validation
    QObject::connect(lineEdit, &QLineEdit::textChanged, [lineEdit, type](const QString &text) {
        QString filtered = filterInput(text, type);
        if (filtered != text) {
            // Temporarily disconnect to avoid recursive calls
            lineEdit->blockSignals(true);
            int cursorPos = lineEdit->cursorPosition();
            lineEdit->setText(filtered);
            lineEdit->setCursorPosition(qMin(cursorPos, filtered.length()));
            lineEdit->blockSignals(false);
        }

        // Visual feedback for validation
        bool isValid = InputValidator::isValid(filtered, type);
        QString styleSheet = "";

        if (!filtered.isEmpty() && !isValid) {
            styleSheet = "QLineEdit { border: 2px solid #e74c3c; background-color: #fdf2f2; }";
        } else if (!filtered.isEmpty() && isValid) {
            styleSheet = "QLineEdit { border: 2px solid #27ae60; background-color: #f2fdf2; }";
        } else {
            styleSheet = ""; // Default style
        }

        lineEdit->setStyleSheet(styleSheet);
    });
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

bool InputValidator::isValid(const QString &input, InputType type) {
    switch (type) {
        case NONE:
            return true; // Always valid for NONE type
        case GENERAL: {
            if (input.isEmpty())
                return true;
            // Allow letters, numbers, spaces, hyphens, dots, and Vietnamese characters
            QRegularExpression generalPattern(
                    "^[a-zA-Z0-9àáạảãâầấậẩẫăằắặẳẵèéẹẻẽêềếệểễìíịỉĩòóọỏõôồốộổỗơờớợởỡùúụủũưừứựửữỳýỵỷỹđĐ\\s\\-\\.]+$");
            return generalPattern.match(input).hasMatch();
        }
        case CODE: {
            if (input.isEmpty())
                return true;
            // Allow only letters, numbers, and hyphens (no spaces, no special chars)
            QRegularExpression codePattern("^[A-Z0-9\\-]+$");
            return codePattern.match(input).hasMatch();
        }
        default:
            return true;
    }
}

bool InputValidator::isValidLength(const QString &input, int minLength, int maxLength) {
    int length = input.trimmed().length();
    return length >= minLength && length <= maxLength;
}

bool InputValidator::validateStudentData(const QString &studentId, const QString &lastName,
                                         const QString &firstName, const QString &password) {
    if (!isValidLength(studentId, 1, 20) || !isValid(studentId, CODE)) {
        return false;
    }

    if (!isValidLength(lastName, 1, 50) || !isValid(lastName, GENERAL)) {
        return false;
    }

    if (!isValidLength(firstName, 1, 50) || !isValid(firstName, GENERAL)) {
        return false;
    }

    if (!isValidLength(password, 1, 50)) {
        return false;
    }

    return true;
}

bool InputValidator::validateClassData(const QString &classCode, const QString &className) {
    if (!isValidLength(classCode, 1, 20) || !isValid(classCode, CODE)) {
        return false;
    }

    if (!isValidLength(className, 1, 100) || !isValid(className, GENERAL)) {
        return false;
    }

    return true;
}

bool InputValidator::validateSubjectData(const QString &subjectCode, const QString &subjectName) {
    if (!isValidLength(subjectCode, 1, 15) || !isValid(subjectCode, CODE)) {
        return false;
    }

    if (!isValidLength(subjectName, 1, 100) || !isValid(subjectName, GENERAL)) {
        return false;
    }

    return true;
}

void InputValidator::showValidationError(QWidget *parent, const QString &fieldName, const QString &errorMessage) {
    QString message = QString("Invalid %1: %2").arg(fieldName, errorMessage);
    QMessageBox::warning(parent, "Validation Error", message);
}

// Private helper methods
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

QString InputValidator::keepValidGeneralChars(const QString &input) {
    QString result;
    for (QChar c: input) {
        // Keep letters (including Vietnamese), numbers, spaces, hyphens, and dots
        if (c.isLetter() || c.isNumber() || c.isSpace() || c == '-' || c == '.') {
            result += c;
        }
    }
    return result;
}

QString InputValidator::keepValidCodeChars(const QString &input) {
    QString result;
    for (QChar c: input) {
        // Keep only letters, numbers, and hyphens (no spaces, no special chars)
        if (c.isLetterOrNumber() || c == '-') {
            result += c;
        }
    }
    return result;
}

InputValidator::InputType InputValidator::getValidationType(QLineEdit *lineEdit) {
    if (!lineEdit)
        return GENERAL;

    bool ok;
    int typeInt = lineEdit->property("validationType").toInt(&ok);
    if (ok) {
        return static_cast<InputType>(typeInt);
    }

    return GENERAL;
}
