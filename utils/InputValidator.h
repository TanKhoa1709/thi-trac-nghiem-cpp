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

    // Setup real-time validation for QLineEdit
    static void setupInputValidation(QLineEdit *lineEdit, InputType type);
    
    // Filter input based on type
    static QString filterInput(const QString &input, InputType type);
    
    // Validate input based on type
    static bool isValid(const QString &input, InputType type);
    
    // Validate input length
    static bool isValidLength(const QString &input, int minLength, int maxLength);
    
    // Validation methods for specific data
    static bool validateStudentData(const QString &studentId, const QString &lastName, 
                                   const QString &firstName, const QString &password);
    static bool validateClassData(const QString &classCode, const QString &className);
    static bool validateSubjectData(const QString &subjectCode, const QString &subjectName);
    
    // Show validation error messages
    static void showValidationError(QWidget *parent, const QString &fieldName, const QString &errorMessage);

private:
    // Helper methods for filtering
    static QString filterGeneral(const QString &input);
    static QString filterCode(const QString &input);
    
    // Helper methods for character filtering
    static QString keepValidGeneralChars(const QString &input);
    static QString keepValidCodeChars(const QString &input);
    
    // Get validation type from QLineEdit property
    static InputType getValidationType(QLineEdit *lineEdit);
};

#endif // INPUTVALIDATOR_H