#include "ValidationHelper.h"
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

void ValidationHelper::setupInputValidation(QLineEdit *lineEdit, InputValidator::InputType type) {
    if (!lineEdit) return;
    
    // Store validation type as property
    lineEdit->setProperty("validationType", static_cast<int>(type));
    
    // Set input mask and validation based on type
    switch (type) {
        case InputValidator::NONE:
            lineEdit->setMaxLength(500);
            lineEdit->setPlaceholderText("Enter text (no restrictions)");
            break;
            
        case InputValidator::GENERAL:
            lineEdit->setMaxLength(200);
            lineEdit->setPlaceholderText("Enter text (letters, numbers, spaces allowed)");
            break;
            
        case InputValidator::CODE:
            lineEdit->setMaxLength(50);
            lineEdit->setPlaceholderText("Enter code (uppercase, no spaces)");
            break;
    }
    
    // Connect text changed signal for real-time validation
    QObject::connect(lineEdit, &QLineEdit::textChanged, [lineEdit, type](const QString &text) {
        QString filtered = InputValidator::filterInput(text, type);
        if (filtered != text) {
            // Temporarily disconnect to avoid recursive calls
            lineEdit->blockSignals(true);
            int cursorPos = lineEdit->cursorPosition();
            lineEdit->setText(filtered);
            lineEdit->setCursorPosition(qMin(cursorPos, filtered.length()));
            lineEdit->blockSignals(false);
        }
        
        // Visual feedback for validation
        bool isValid = true;
        QString styleSheet = "";
        
        switch (type) {
            case InputValidator::NONE:
                isValid = true; // Always valid for NONE type
                break;
            case InputValidator::GENERAL:
                isValid = InputValidator::isValidGeneral(filtered);
                break;
            case InputValidator::CODE:
                isValid = InputValidator::isValidCode(filtered);
                break;
        }
        
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

bool ValidationHelper::validateStudentData(const QString &studentId, const QString &lastName, 
                                          const QString &firstName, const QString &password) {
    if (!InputValidator::isValidLength(studentId, 1, 20) || !InputValidator::isValidCode(studentId)) {
        return false;
    }
    
    if (!InputValidator::isValidLength(lastName, 1, 50) || !InputValidator::isValidGeneral(lastName)) {
        return false;
    }
    
    if (!InputValidator::isValidLength(firstName, 1, 50) || !InputValidator::isValidGeneral(firstName)) {
        return false;
    }
    
    if (!InputValidator::isValidLength(password, 1, 50)) {
        return false;
    }
    
    return true;
}

bool ValidationHelper::validateClassData(const QString &classCode, const QString &className) {
    if (!InputValidator::isValidLength(classCode, 1, 20) || !InputValidator::isValidCode(classCode)) {
        return false;
    }
    
    if (!InputValidator::isValidLength(className, 1, 100) || !InputValidator::isValidGeneral(className)) {
        return false;
    }
    
    return true;
}

bool ValidationHelper::validateSubjectData(const QString &subjectCode, const QString &subjectName) {
    if (!InputValidator::isValidLength(subjectCode, 1, 15) || !InputValidator::isValidCode(subjectCode)) {
        return false;
    }
    
    if (!InputValidator::isValidLength(subjectName, 1, 100) || !InputValidator::isValidGeneral(subjectName)) {
        return false;
    }
    
    return true;
}

QString ValidationHelper::sanitizeForModel(const QString &input, InputValidator::InputType type) {
    return InputValidator::filterInput(input, type);
}

void ValidationHelper::showValidationError(QWidget *parent, const QString &fieldName, const QString &errorMessage) {
    QString message = QString("Invalid %1: %2").arg(fieldName, errorMessage);
    QMessageBox::warning(parent, "Validation Error", message);
}

InputValidator::InputType ValidationHelper::getValidationType(QLineEdit *lineEdit) {
    if (!lineEdit) return InputValidator::GENERAL;
    
    bool ok;
    int typeInt = lineEdit->property("validationType").toInt(&ok);
    if (ok) {
        return static_cast<InputValidator::InputType>(typeInt);
    }
    
    return InputValidator::GENERAL;
}
