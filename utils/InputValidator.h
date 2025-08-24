#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QString>
#include <QLineEdit>
#include <QWidget>
#include <QRegularExpression>

class InputValidator {
public:
    enum InputType {
        NONE,        // No validation, freely use
        GENERAL,     // No special chars, but allows letters, numbers, spaces
        CODE         // No spaces, no special chars (except "-"), always uppercase
    };

    // Core validation methods
    static QString filterGeneral(const QString &input);
    static QString filterCode(const QString &input);
    static bool isValidGeneral(const QString &input);
    static bool isValidCode(const QString &input);
    static QString filterInput(const QString &input, InputType type);

    // UI validation setup
    static void setupInputValidation(QLineEdit *lineEdit, InputType type);
    
    // Utility methods
    static QString sanitizeForModel(const QString &input, InputType type);
};

#endif // INPUTVALIDATOR_H
