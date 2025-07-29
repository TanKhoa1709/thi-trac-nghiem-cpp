#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QString>
#include <memory>

// Forward declarations
class QuanLyMonHoc;
class QuanLyCauHoi;
class QuanLyLop;
class QuanLySinhVien;
class QuanLyDiemThi;
class QuanLyThi;
class AuthenticationManager;

/**
 * @brief Quản lý ứng dụng chính - Main application manager
 * Singleton pattern để quản lý toàn bộ hệ thống
 */
class AppManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY loginStatusChanged)
    Q_PROPERTY(QString currentUser READ getCurrentUser NOTIFY currentUserChanged)
    Q_PROPERTY(QString userRole READ getUserRole NOTIFY userRoleChanged)
    Q_PROPERTY(bool isTeacher READ isTeacher NOTIFY userRoleChanged)
    Q_PROPERTY(bool isStudent READ isStudent NOTIFY userRoleChanged)

private:
    static AppManager* instance;
    
    // Core managers
    std::unique_ptr<QuanLyMonHoc> quanLyMonHoc;
    std::unique_ptr<QuanLyCauHoi> quanLyCauHoi;
    std::unique_ptr<QuanLyLop> quanLyLop;
    std::unique_ptr<QuanLySinhVien> quanLySinhVien;
    std::unique_ptr<QuanLyDiemThi> quanLyDiemThi;
    std::unique_ptr<QuanLyThi> quanLyThi;
    std::unique_ptr<AuthenticationManager> authManager;

    // Application state
    QString currentUser;
    QString userRole;  // "teacher" or "student"
    bool loggedIn;
    QString dataDirectory;

    AppManager(QObject* parent = nullptr);

public:
    static AppManager* getInstance();
    ~AppManager();

    // Initialization
    Q_INVOKABLE bool initialize(const QString& dataDir = "data");
    Q_INVOKABLE bool loadData();
    Q_INVOKABLE bool saveData();
    Q_INVOKABLE void shutdown();

    // Authentication
    Q_INVOKABLE bool login(const QString& username, const QString& password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE bool changePassword(const QString& oldPassword, const QString& newPassword);

    // Getters for QML
    bool isLoggedIn() const { return loggedIn; }
    QString getCurrentUser() const { return currentUser; }
    QString getUserRole() const { return userRole; }
    bool isTeacher() const { return userRole == "teacher"; }
    bool isStudent() const { return userRole == "student"; }

    // Manager access
    QuanLyMonHoc* getMonHocManager() const { return quanLyMonHoc.get(); }
    QuanLyCauHoi* getCauHoiManager() const { return quanLyCauHoi.get(); }
    QuanLyLop* getLopManager() const { return quanLyLop.get(); }
    QuanLySinhVien* getSinhVienManager() const { return quanLySinhVien.get(); }
    QuanLyDiemThi* getDiemThiManager() const { return quanLyDiemThi.get(); }
    QuanLyThi* getThiManager() const { return quanLyThi.get(); }

    // File operations
    Q_INVOKABLE bool importData(const QString& filePath, const QString& dataType);
    Q_INVOKABLE bool exportData(const QString& filePath, const QString& dataType);
    Q_INVOKABLE bool backupData(const QString& backupPath);
    Q_INVOKABLE bool restoreData(const QString& backupPath);

    // System operations
    Q_INVOKABLE QString getVersion() const { return "1.0.0"; }
    Q_INVOKABLE QString getSystemInfo() const;
    Q_INVOKABLE bool checkDataIntegrity();

signals:
    void loginStatusChanged();
    void currentUserChanged();
    void userRoleChanged();
    void dataLoaded();
    void dataSaved();
    void errorOccurred(const QString& error);
    void messageReceived(const QString& message);

private slots:
    void onDataChanged();
    void onError(const QString& error);

private:
    // Helper methods
    bool createDataDirectories();
    bool validateDataFiles();
    void setupConnections();
    void cleanupManagers();
};

#endif // APPMANAGER_H
