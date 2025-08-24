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
    static bool isValidLength(const QString &input, int minLength, int maxLength);
    static QString filterInput(const QString &input, InputType type);

    // UI validation setup
    static void setupInputValidation(QLineEdit *lineEdit, InputType type);
    
    // Data validation methods
    static bool validateStudentData(const QString &studentId, const QString &lastName, 
                                   const QString &firstName, const QString &password);
    static bool validateClassData(const QString &classCode, const QString &className);
    static bool validateSubjectData(const QString &subjectCode, const QString &subjectName);
    
    // Utility methods
    static QString sanitizeForModel(const QString &input, InputType type);
    static void showValidationError(QWidget *parent, const QString &title, const QString &message);
    static InputType getValidationType(QLineEdit *lineEdit);
};

#endif // INPUTVALIDATOR_H
