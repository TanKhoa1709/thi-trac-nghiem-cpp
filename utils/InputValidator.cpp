#include "InputValidator.h"
#include <QRegularExpression>
#include <QLineEdit>
#include <QMessageBox>

// Core validation methods
QString InputValidator::filterGeneral(const QString &input) {
    QString result;
    for (QChar c: input) {
        if (c.isLetterOrNumber() || c.isSpace()) {
            result += c;
        }
    }
    return result.replace(QRegularExpression("\\s+"), " ");
}

QString InputValidator::filterCode(const QString &input) {
    QString result;
    for (QChar c: input) {
        if (c.isLetterOrNumber() || c == '-') {
            result += c;
        }
    }
    return result.toUpper();
}

bool InputValidator::isValidGeneral(const QString &input) {
    if (input.isEmpty())
        return true;
    QRegularExpression pattern("^[a-zA-Z0-9\\s]+$");
    return pattern.match(input).hasMatch();
}

bool InputValidator::isValidCode(const QString &input) {
    if (input.isEmpty())
        return true;
    QRegularExpression pattern("^[A-Z0-9\\-]+$");
    return pattern.match(input).hasMatch();
}

QString InputValidator::filterInput(const QString &input, InputType type) {
    switch (type) {
        case NONE:
            return input;
        case GENERAL:
            return filterGeneral(input);
        case CODE:
            return filterCode(input);
        default:
            return input;
    }
}

// UI validation setup
void InputValidator::setupInputValidation(QLineEdit *lineEdit, InputType type) {
    if (!lineEdit)
        return;

    lineEdit->setProperty("validationType", static_cast<int>(type));

    switch (type) {
        case NONE:
            lineEdit->setMaxLength(500);
            if (lineEdit->placeholderText().isEmpty()) {
                lineEdit->setPlaceholderText("Nhập văn bản (không giới hạn)");
            }
            break;
        case GENERAL:
            lineEdit->setMaxLength(200);
            if (lineEdit->placeholderText().isEmpty()) {
                lineEdit->setPlaceholderText("Nhập văn bản (chữ cái, số, khoảng trắng)");
            }
            break;
        case CODE:
            lineEdit->setMaxLength(50);
            if (lineEdit->placeholderText().isEmpty()) {
                lineEdit->setPlaceholderText("Nhập mã (chữ hoa, không khoảng trắng)");
            }
            break;
    }

    QObject::connect(lineEdit, &QLineEdit::textChanged, [lineEdit, type](const QString &text) {
        QString filtered = filterInput(text, type);
        if (filtered != text) {
            lineEdit->blockSignals(true);
            int cursorPos = lineEdit->cursorPosition();
            lineEdit->setText(filtered);
            lineEdit->setCursorPosition(qMin(cursorPos, filtered.length()));
            lineEdit->blockSignals(false);
        }

        // Visual feedback
        bool isValid = true;
        switch (type) {
            case NONE:
                isValid = true;
                break;
            case GENERAL:
                isValid = isValidGeneral(filtered);
                break;
            case CODE:
                isValid = isValidCode(filtered);
                break;
        }

        QString styleSheet = "padding:8px;";
        if (!filtered.isEmpty()) {
            if (isValid) {
                styleSheet = "QLineEdit { border: 2px solid #27ae60; background-color: #f2fdf2; padding:8px; }";
            } else {
                styleSheet = "QLineEdit { border: 2px solid #e74c3c; background-color: #fdf2f2; padding:8px; }";
            }
        }
        lineEdit->setStyleSheet(styleSheet);
    });
}

// Utility methods
QString InputValidator::sanitizeForModel(const QString &input, InputType type) {
    return filterInput(input, type);
}
