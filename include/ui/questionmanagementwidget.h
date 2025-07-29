#ifndef QUESTIONMANAGEMENTWIDGET_H
#define QUESTIONMANAGEMENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGroupBox>
#include <QSplitter>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QSpinBox>

class AppManager;
class CauHoi;

/**
 * @brief Widget for managing exam questions
 * Implements full CRUD operations for questions with subject integration
 */
class QuestionManagementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionManagementWidget(AppManager* appManager, QWidget *parent = nullptr);
    ~QuestionManagementWidget();

public slots:
    void refreshData();
    void onSubjectChanged();

signals:
    void questionAdded(int questionId, const QString& subjectCode);
    void questionUpdated(int questionId);
    void questionDeleted(int questionId);

private slots:
    void onAddQuestion();
    void onEditQuestion();
    void onDeleteQuestion();
    void onQuestionSelectionChanged();
    void onSearchTextChanged();
    void onClearForm();
    void onSaveQuestion();
    void onCancelEdit();

private:
    void setupUI();
    void setupQuestionList();
    void setupQuestionForm();
    void setupConnections();
    void applyStyles();
    
    void loadSubjects();
    void loadQuestions();
    void filterQuestions();
    void populateQuestionTable();
    void clearQuestionForm();
    void loadQuestionToForm(const CauHoi& question);
    bool validateQuestionForm();
    void saveQuestionFromForm();
    void updateQuestionInForm();
    bool canDeleteQuestion(int questionId);
    
    AppManager* appManager;
    
    // UI Components
    QVBoxLayout* mainLayout;
    QSplitter* mainSplitter;
    
    // Left panel - Question list
    QWidget* listPanel;
    QVBoxLayout* listLayout;
    QGroupBox* filterGroup;
    QHBoxLayout* filterLayout;
    QComboBox* subjectFilterCombo;
    QLineEdit* searchEdit;
    QPushButton* refreshButton;
    
    QGroupBox* questionListGroup;
    QVBoxLayout* questionListLayout;
    QTableWidget* questionTable;
    
    QHBoxLayout* listButtonLayout;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    
    // Right panel - Question form
    QWidget* formPanel;
    QVBoxLayout* formLayout;
    QGroupBox* questionFormGroup;
    QGridLayout* formGridLayout;
    
    QLabel* idLabel;
    QLineEdit* idEdit;
    QLabel* subjectLabel;
    QComboBox* subjectCombo;
    QLabel* questionLabel;
    QTextEdit* questionEdit;
    
    QLabel* answerALabel;
    QLineEdit* answerAEdit;
    QLabel* answerBLabel;
    QLineEdit* answerBEdit;
    QLabel* answerCLabel;
    QLineEdit* answerCEdit;
    QLabel* answerDLabel;
    QLineEdit* answerDEdit;
    
    QLabel* correctAnswerLabel;
    QComboBox* correctAnswerCombo;
    
    QHBoxLayout* formButtonLayout;
    QPushButton* saveButton;
    QPushButton* cancelButton;
    QPushButton* clearButton;
    
    // State
    bool isEditing;
    int currentQuestionId;
    QString currentSubjectFilter;
};

#endif // QUESTIONMANAGEMENTWIDGET_H