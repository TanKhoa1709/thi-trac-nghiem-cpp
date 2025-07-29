#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QCryptographicHash>

/**
 * @brief Thông tin người dùng
 */
struct UserInfo {
    QString username;
    QString hashedPassword;
    QString role;           // "teacher" or "student"
    QString fullName;
    QString email;
    bool isActive;
    
    UserInfo() : isActive(true) {}
    UserInfo(const QString& user, const QString& pass, const QString& r, 
             const QString& name = "", const QString& mail = "")
        : username(user), hashedPassword(pass), role(r), fullName(name), email(mail), isActive(true) {}
};

/**
 * @brief Quản lý xác thực người dùng
 */
class AuthenticationManager : public QObject {
    Q_OBJECT

private:
    QMap<QString, UserInfo> users;
    QString currentUser;
    QString sessionToken;

public:
    explicit AuthenticationManager(QObject* parent = nullptr);
    ~AuthenticationManager();

    // Authentication
    bool login(const QString& username, const QString& password);
    void logout();
    bool isLoggedIn() const;
    
    // User management
    bool addUser(const QString& username, const QString& password, 
                 const QString& role, const QString& fullName = "", 
                 const QString& email = "");
    bool removeUser(const QString& username);
    bool changePassword(const QString& username, const QString& oldPassword, 
                       const QString& newPassword);
    bool updateUserInfo(const QString& username, const UserInfo& newInfo);
    
    // User info
    QString getCurrentUser() const { return currentUser; }
    QString getCurrentUserRole() const;
    QString getCurrentUserFullName() const;
    UserInfo getUserInfo(const QString& username) const;
    QStringList getAllUsers() const;
    
    // Validation
    bool userExists(const QString& username) const;
    bool validatePassword(const QString& username, const QString& password) const;
    bool isTeacher(const QString& username = "") const;
    bool isStudent(const QString& username = "") const;
    
    // File operations
    bool loadUsers(const QString& filePath);
    bool saveUsers(const QString& filePath) const;
    
    // Security
    QString hashPassword(const QString& password) const;
    bool verifyPassword(const QString& password, const QString& hashedPassword) const;
    QString generateSessionToken() const;

signals:
    void loginSuccessful(const QString& username, const QString& role);
    void loginFailed(const QString& reason);
    void loggedOut();
    void userAdded(const QString& username);
    void userRemoved(const QString& username);
    void passwordChanged(const QString& username);

private:
    void initializeDefaultUsers();
    bool validateUserRole(const QString& role) const;
};

#endif // AUTHENTICATION_H
