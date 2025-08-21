#ifndef VALIDATIONHELPER_H
#define VALIDATIONHELPER_H

#include <QString>
#include <QLineEdit>
#include <QWidget>
#include "InputValidator.h"

/**
 * @brief Helper class để apply validation vào UI components mà không thay đổi models
 */
class ValidationHelper {
public:
    // Apply real-time validation to QLineEdit
    static void setupInputValidation(QLineEdit *lineEdit, InputValidator::InputType type);
    
    // Validate before creating/updating entities
    static bool validateStudentData(const QString &studentId, const QString &lastName, 
                                   const QString &firstName, const QString &password);
    static bool validateClassData(const QString &classCode, const QString &className);
    static bool validateSubjectData(const QString &subjectCode, const QString &subjectName);
    
    // Sanitize input before passing to models
    static QString sanitizeForModel(const QString &input, InputValidator::InputType type);
    
    // Show validation error messages
    static void showValidationError(QWidget *parent, const QString &fieldName, const QString &errorMessage);
    
private:
    // Helper to get validation type from QLineEdit property
    static InputValidator::InputType getValidationType(QLineEdit *lineEdit);
};

#endif // VALIDATIONHELPER_H
