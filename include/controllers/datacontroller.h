#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QVariantMap>

// Forward declarations
class QuanLyMonHoc;
class QuanLyCauHoi;
class QuanLyLop;
class QuanLySinhVien;
class QuanLyDiemThi;

/**
 * @brief Controller cho quản lý dữ liệu
 * Cung cấp interface QML cho các thao tác CRUD
 */
class DataController : public QObject {

private:
    QuanLyMonHoc* monHocManager;
    QuanLyCauHoi* cauHoiManager;
    QuanLyLop* lopManager;
    QuanLySinhVien* sinhVienManager;
    QuanLyDiemThi* diemThiManager;

public:
    explicit DataController(QObject* parent = nullptr);
    ~DataController();

    // Initialization
    void setManagers(QuanLyMonHoc* mh, QuanLyCauHoi* ch, QuanLyLop* l, 
                     QuanLySinhVien* sv, QuanLyDiemThi* dt);

    // Subject management
    bool addSubject(const QString& id, const QString& name, int credits = 3);
    bool removeSubject(const QString& id);
    bool updateSubject(const QString& id, const QString& name, int credits);
    QVariantMap getSubject(const QString& id);
    QVariantList getAllSubjects();

    // Question management
    bool addQuestion(const QVariantMap& questionData);
    bool removeQuestion(int questionId);
    bool updateQuestion(int questionId, const QVariantMap& questionData);
    QVariantMap getQuestion(int questionId);
    QVariantList getQuestionsBySubject(const QString& subjectId);
    QVariantList getAllQuestions();

    // Class management
    bool addClass(const QString& id, const QString& name, int year, const QString& department);
    bool removeClass(const QString& id);
    bool updateClass(const QString& id, const QString& name, int year, const QString& department);
    QVariantMap getClass(const QString& id);
    QVariantList getAllClasses();

    // Student management
    bool addStudent(const QVariantMap& studentData);
    bool removeStudent(const QString& studentId);
    bool updateStudent(const QString& studentId, const QVariantMap& studentData);
    QVariantMap getStudent(const QString& studentId);
    QVariantList getStudentsByClass(const QString& classId);
    QVariantList getAllStudents();

    // Score management
    QVariantList getScoresByStudent(const QString& studentId);
    QVariantList getScoresBySubject(const QString& subjectId);
    QVariantList getScoresByClass(const QString& classId);

    // Search and filter
    QVariantList searchStudents(const QString& keyword);
    QVariantList searchQuestions(const QString& keyword);
    QVariantList filterQuestionsByDifficulty(int difficulty);

    // Statistics
    QVariantMap getSubjectStatistics(const QString& subjectId);
    QVariantMap getClassStatistics(const QString& classId);
    QVariantMap getStudentStatistics(const QString& studentId);
    QVariantMap getOverallStatistics();

    // Undo/Redo operations
    bool undoLastOperation();
    bool redoLastOperation();
    bool canUndo();
    bool canRedo();

    // Validation
    bool validateSubjectId(const QString& id);
    bool validateStudentId(const QString& id);
    bool validateClassId(const QString& id);
    bool validateQuestionData(const QVariantMap& data);

    // File operations
    bool importFromFile(const QString& filePath, const QString& dataType);
    bool exportToFile(const QString& filePath, const QString& dataType);

// Note: Signals removed for simplified build - can be re-added with proper MOC setup

private:
    // Helper methods
    QVariantMap convertSubjectToVariant(const QString& subjectId);
    QVariantMap convertQuestionToVariant(int questionId);
    QVariantMap convertStudentToVariant(const QString& studentId);
    QVariantMap convertClassToVariant(const QString& classId);
    
    bool validateRequiredFields(const QVariantMap& data, const QStringList& requiredFields);
    QString generateUniqueId(const QString& prefix);
};

#endif // DATACONTROLLER_H
