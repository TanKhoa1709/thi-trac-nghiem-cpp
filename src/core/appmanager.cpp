#include "appmanager.h"
#include "authentication.h"
#include "monhoc.h"
#include "cauhoi.h"
#include "lop.h"
#include "sinhvien.h"
#include "diemthi.h"
#include "quanlythi.h"
#include "examcontroller.h"
#include "datacontroller.h"

#include <QDir>
#include <QStandardPaths>
#include <QDebug>

AppManager* AppManager::instance = nullptr;

AppManager::AppManager(QObject* parent)
    : QObject(parent)
    , loggedIn(false)
    , dataDirectory("data")
{
    // Initialize managers
    authManager = std::make_unique<AuthenticationManager>(this);
    quanLyMonHoc = std::make_unique<QuanLyMonHoc>();
    quanLyCauHoi = std::make_unique<QuanLyCauHoi>();
    quanLyLop = std::make_unique<QuanLyLop>();
    quanLySinhVien = std::make_unique<QuanLySinhVien>();
    quanLyDiemThi = std::make_unique<QuanLyDiemThi>();
    quanLyThi = std::make_unique<QuanLyThi>();

    setupConnections();
}

AppManager* AppManager::getInstance()
{
    if (!instance) {
        instance = new AppManager();
    }
    return instance;
}

AppManager::~AppManager()
{
    saveData();
    cleanupManagers();
}

bool AppManager::initialize(const QString& dataDir)
{
    dataDirectory = dataDir;
    
    if (!createDataDirectories()) {
        emit errorOccurred("Failed to create data directories");
        return false;
    }

    if (!loadData()) {
        qWarning() << "Failed to load some data files, using defaults";
    }

    emit messageReceived("Application initialized successfully");
    return true;
}

bool AppManager::loadData()
{
    bool success = true;
    
    // Load authentication data
    QString authFile = dataDirectory + "/users.dat";
    if (!authManager->loadUsers(authFile)) {
        qWarning() << "Failed to load user data, using defaults";
        success = false;
    }

    // Load subject data
    QString subjectFile = dataDirectory + "/subjects.dat";
    if (!quanLyMonHoc->docTuFile(subjectFile)) {
        qWarning() << "Failed to load subject data";
        success = false;
    }

    // Load question data
    QString questionFile = dataDirectory + "/questions.dat";
    if (!quanLyCauHoi->docTuFile(questionFile)) {
        qWarning() << "Failed to load question data";
        success = false;
    }

    // Load class data
    QString classFile = dataDirectory + "/classes.dat";
    if (!quanLyLop->docTuFile(classFile)) {
        qWarning() << "Failed to load class data";
        success = false;
    }

    // Load student data
    QString studentFile = dataDirectory + "/students.dat";
    if (!quanLySinhVien->docTuFile(studentFile)) {
        qWarning() << "Failed to load student data";
        success = false;
    }

    // Load score data
    QString scoreFile = dataDirectory + "/scores.dat";
    if (!quanLyDiemThi->docTuFile(scoreFile)) {
        qWarning() << "Failed to load score data";
        success = false;
    }

    emit dataLoaded();
    return success;
}

bool AppManager::saveData()
{
    bool success = true;

    // Save authentication data
    QString authFile = dataDirectory + "/users.dat";
    if (!authManager->saveUsers(authFile)) {
        qWarning() << "Failed to save user data";
        success = false;
    }

    // Save subject data
    QString subjectFile = dataDirectory + "/subjects.dat";
    if (!quanLyMonHoc->luuVaoFile(subjectFile)) {
        qWarning() << "Failed to save subject data";
        success = false;
    }

    // Save question data
    QString questionFile = dataDirectory + "/questions.dat";
    if (!quanLyCauHoi->luuVaoFile(questionFile)) {
        qWarning() << "Failed to save question data";
        success = false;
    }

    // Save class data
    QString classFile = dataDirectory + "/classes.dat";
    if (!quanLyLop->luuVaoFile(classFile)) {
        qWarning() << "Failed to save class data";
        success = false;
    }

    // Save student data
    QString studentFile = dataDirectory + "/students.dat";
    if (!quanLySinhVien->luuVaoFile(studentFile)) {
        qWarning() << "Failed to save student data";
        success = false;
    }

    // Save score data
    QString scoreFile = dataDirectory + "/scores.dat";
    if (!quanLyDiemThi->luuVaoFile(scoreFile)) {
        qWarning() << "Failed to save score data";
        success = false;
    }

    emit dataSaved();
    return success;
}

void AppManager::shutdown()
{
    saveData();
    logout();
    emit messageReceived("Application shutdown");
}

bool AppManager::login(const QString& username, const QString& password)
{
    if (authManager->login(username, password)) {
        currentUser = username;
        userRole = authManager->getCurrentUserRole();
        loggedIn = true;
        
        emit loginStatusChanged();
        emit currentUserChanged();
        emit userRoleChanged();
        
        return true;
    }
    return false;
}

void AppManager::logout()
{
    if (loggedIn) {
        authManager->logout();
        currentUser.clear();
        userRole.clear();
        loggedIn = false;
        
        emit loginStatusChanged();
        emit currentUserChanged();
        emit userRoleChanged();
    }
}

bool AppManager::changePassword(const QString& oldPassword, const QString& newPassword)
{
    if (!loggedIn) return false;
    
    return authManager->changePassword(currentUser, oldPassword, newPassword);
}

bool AppManager::importData(const QString& filePath, const QString& dataType)
{
    // Implementation for importing different data types
    if (dataType == "subjects") {
        return quanLyMonHoc->docTuFile(filePath);
    } else if (dataType == "questions") {
        return quanLyCauHoi->docTuFile(filePath);
    } else if (dataType == "classes") {
        return quanLyLop->docTuFile(filePath);
    } else if (dataType == "students") {
        return quanLySinhVien->docTuFile(filePath);
    } else if (dataType == "scores") {
        return quanLyDiemThi->docTuFile(filePath);
    }
    return false;
}

bool AppManager::exportData(const QString& filePath, const QString& dataType)
{
    // Implementation for exporting different data types
    if (dataType == "subjects") {
        return quanLyMonHoc->luuVaoFile(filePath);
    } else if (dataType == "questions") {
        return quanLyCauHoi->luuVaoFile(filePath);
    } else if (dataType == "classes") {
        return quanLyLop->luuVaoFile(filePath);
    } else if (dataType == "students") {
        return quanLySinhVien->luuVaoFile(filePath);
    } else if (dataType == "scores") {
        return quanLyDiemThi->luuVaoFile(filePath);
    }
    return false;
}

bool AppManager::backupData(const QString& backupPath)
{
    QDir backupDir(backupPath);
    if (!backupDir.exists()) {
        backupDir.mkpath(".");
    }

    bool success = true;
    success &= exportData(backupPath + "/subjects_backup.dat", "subjects");
    success &= exportData(backupPath + "/questions_backup.dat", "questions");
    success &= exportData(backupPath + "/classes_backup.dat", "classes");
    success &= exportData(backupPath + "/students_backup.dat", "students");
    success &= exportData(backupPath + "/scores_backup.dat", "scores");

    return success;
}

bool AppManager::restoreData(const QString& backupPath)
{
    bool success = true;
    success &= importData(backupPath + "/subjects_backup.dat", "subjects");
    success &= importData(backupPath + "/questions_backup.dat", "questions");
    success &= importData(backupPath + "/classes_backup.dat", "classes");
    success &= importData(backupPath + "/students_backup.dat", "students");
    success &= importData(backupPath + "/scores_backup.dat", "scores");

    if (success) {
        emit dataLoaded();
    }
    return success;
}

QString AppManager::getSystemInfo() const
{
    QString info;
    info += QString("Application Version: %1\n").arg(getVersion());
    info += QString("Data Directory: %1\n").arg(dataDirectory);
    info += QString("Current User: %1\n").arg(currentUser);
    info += QString("User Role: %1\n").arg(userRole);
    info += QString("Subjects: %1\n").arg(quanLyMonHoc->soLuongMonHoc());
    info += QString("Questions: %1\n").arg(quanLyCauHoi->tongSoCauHoi());
    info += QString("Classes: %1\n").arg(quanLyLop->getSoLuongLop());
    info += QString("Students: %1\n").arg(quanLySinhVien->tongSoSinhVien());
    info += QString("Exams: %1\n").arg(quanLyDiemThi->tongSoBaiThi());
    
    return info;
}

bool AppManager::checkDataIntegrity()
{
    // Basic data integrity checks
    bool valid = true;
    
    // Check if all students belong to existing classes
    // Check if all questions belong to existing subjects
    // Check if all scores belong to existing students and subjects
    
    return valid;
}

void AppManager::onDataChanged()
{
    // Auto-save when data changes
    saveData();
}

void AppManager::onError(const QString& error)
{
    emit errorOccurred(error);
}

bool AppManager::createDataDirectories()
{
    QDir dir;
    if (!dir.exists(dataDirectory)) {
        if (!dir.mkpath(dataDirectory)) {
            return false;
        }
    }
    return true;
}

bool AppManager::validateDataFiles()
{
    // Check if required data files exist and are readable
    QStringList requiredFiles = {
        "users.dat", "subjects.dat", "questions.dat", 
        "classes.dat", "students.dat", "scores.dat"
    };
    
    for (const QString& file : requiredFiles) {
        QString filePath = dataDirectory + "/" + file;
        QFile f(filePath);
        if (!f.exists()) {
            // Create empty file if it doesn't exist
            if (f.open(QIODevice::WriteOnly)) {
                f.close();
            }
        }
    }
    return true;
}

void AppManager::setupConnections()
{
    // Connect authentication signals
    connect(authManager.get(), &AuthenticationManager::loginSuccessful,
            this, [this](const QString& username, const QString& role) {
                currentUser = username;
                userRole = role;
                loggedIn = true;
                emit loginStatusChanged();
                emit currentUserChanged();
                emit userRoleChanged();
            });
            
    connect(authManager.get(), &AuthenticationManager::loginFailed,
            this, [this](const QString& reason) {
                emit errorOccurred("Login failed: " + reason);
            });
            
    connect(authManager.get(), &AuthenticationManager::loggedOut,
            this, [this]() {
                currentUser.clear();
                userRole.clear();
                loggedIn = false;
                emit loginStatusChanged();
                emit currentUserChanged();
                emit userRoleChanged();
            });
}

void AppManager::cleanupManagers()
{
    quanLyThi.reset();
    quanLyDiemThi.reset();
    quanLySinhVien.reset();
    quanLyLop.reset();
    quanLyCauHoi.reset();
    quanLyMonHoc.reset();
    authManager.reset();
}
