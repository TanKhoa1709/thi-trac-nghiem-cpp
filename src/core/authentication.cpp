#include "authentication.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>
#include <QDebug>

AuthenticationManager::AuthenticationManager(QObject* parent)
    : QObject(parent)
{
    initializeDefaultUsers();
}

AuthenticationManager::~AuthenticationManager()
{
}

bool AuthenticationManager::login(const QString& username, const QString& password)
{
    if (!userExists(username)) {
        emit loginFailed("User does not exist");
        return false;
    }

    if (!validatePassword(username, password)) {
        emit loginFailed("Invalid password");
        return false;
    }

    UserInfo user = users[username];
    if (!user.isActive) {
        emit loginFailed("User account is disabled");
        return false;
    }

    currentUser = username;
    sessionToken = generateSessionToken();
    
    emit loginSuccessful(username, user.role);
    return true;
}

void AuthenticationManager::logout()
{
    currentUser.clear();
    sessionToken.clear();
    emit loggedOut();
}

bool AuthenticationManager::isLoggedIn() const
{
    return !currentUser.isEmpty();
}

bool AuthenticationManager::addUser(const QString& username, const QString& password, 
                                   const QString& role, const QString& fullName, 
                                   const QString& email)
{
    if (userExists(username)) {
        return false;
    }

    if (!validateUserRole(role)) {
        return false;
    }

    QString hashedPass = hashPassword(password);
    UserInfo newUser(username, hashedPass, role, fullName, email);
    users[username] = newUser;
    
    emit userAdded(username);
    return true;
}

bool AuthenticationManager::removeUser(const QString& username)
{
    if (!userExists(username)) {
        return false;
    }

    // Don't allow removing the currently logged in user
    if (username == currentUser) {
        return false;
    }

    users.remove(username);
    emit userRemoved(username);
    return true;
}

bool AuthenticationManager::changePassword(const QString& username, 
                                         const QString& oldPassword, 
                                         const QString& newPassword)
{
    if (!userExists(username)) {
        return false;
    }

    if (!validatePassword(username, oldPassword)) {
        return false;
    }

    users[username].hashedPassword = hashPassword(newPassword);
    emit passwordChanged(username);
    return true;
}

bool AuthenticationManager::updateUserInfo(const QString& username, const UserInfo& newInfo)
{
    if (!userExists(username)) {
        return false;
    }

    // Keep the original password if not provided
    UserInfo updatedInfo = newInfo;
    if (updatedInfo.hashedPassword.isEmpty()) {
        updatedInfo.hashedPassword = users[username].hashedPassword;
    }

    users[username] = updatedInfo;
    return true;
}

QString AuthenticationManager::getCurrentUserRole() const
{
    if (!isLoggedIn()) {
        return QString();
    }
    return users[currentUser].role;
}

QString AuthenticationManager::getCurrentUserFullName() const
{
    if (!isLoggedIn()) {
        return QString();
    }
    return users[currentUser].fullName;
}

UserInfo AuthenticationManager::getUserInfo(const QString& username) const
{
    if (userExists(username)) {
        return users[username];
    }
    return UserInfo();
}

QStringList AuthenticationManager::getAllUsers() const
{
    return users.keys();
}

bool AuthenticationManager::userExists(const QString& username) const
{
    return users.contains(username);
}

bool AuthenticationManager::validatePassword(const QString& username, const QString& password) const
{
    if (!userExists(username)) {
        return false;
    }
    
    QString hashedInput = hashPassword(password);
    return hashedInput == users[username].hashedPassword;
}

bool AuthenticationManager::isTeacher(const QString& username) const
{
    QString user = username.isEmpty() ? currentUser : username;
    if (!userExists(user)) {
        return false;
    }
    return users[user].role == "teacher";
}

bool AuthenticationManager::isStudent(const QString& username) const
{
    QString user = username.isEmpty() ? currentUser : username;
    if (!userExists(user)) {
        return false;
    }
    return users[user].role == "student";
}

bool AuthenticationManager::loadUsers(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open user file for reading:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isObject()) {
        qWarning() << "Invalid user data format";
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray usersArray = root["users"].toArray();

    users.clear();
    for (const QJsonValue& value : usersArray) {
        QJsonObject userObj = value.toObject();
        
        UserInfo user;
        user.username = userObj["username"].toString();
        user.hashedPassword = userObj["password"].toString();
        user.role = userObj["role"].toString();
        user.fullName = userObj["fullName"].toString();
        user.email = userObj["email"].toString();
        user.isActive = userObj["isActive"].toBool(true);
        
        users[user.username] = user;
    }

    return true;
}

bool AuthenticationManager::saveUsers(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open user file for writing:" << filePath;
        return false;
    }

    QJsonArray usersArray;
    for (auto it = users.begin(); it != users.end(); ++it) {
        const UserInfo& user = it.value();
        
        QJsonObject userObj;
        userObj["username"] = user.username;
        userObj["password"] = user.hashedPassword;
        userObj["role"] = user.role;
        userObj["fullName"] = user.fullName;
        userObj["email"] = user.email;
        userObj["isActive"] = user.isActive;
        
        usersArray.append(userObj);
    }

    QJsonObject root;
    root["users"] = usersArray;
    root["version"] = "1.0";

    QJsonDocument doc(root);
    file.write(doc.toJson());
    
    return true;
}

QString AuthenticationManager::hashPassword(const QString& password) const
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    hash.addData("exam_system_salt"); // Simple salt
    return hash.result().toHex();
}

bool AuthenticationManager::verifyPassword(const QString& password, const QString& hashedPassword) const
{
    return hashPassword(password) == hashedPassword;
}

QString AuthenticationManager::generateSessionToken() const
{
    QByteArray token;
    for (int i = 0; i < 32; ++i) {
        token.append(QRandomGenerator::global()->bounded(256));
    }
    return token.toHex();
}

void AuthenticationManager::initializeDefaultUsers()
{
    // Create default teacher account
    addUser("teacher", "teacher123", "teacher", "Default Teacher", "teacher@example.com");
    
    // Create default student account
    addUser("student", "student123", "student", "Default Student", "student@example.com");
    
    // Create admin account
    addUser("admin", "admin123", "teacher", "System Administrator", "admin@example.com");
}

bool AuthenticationManager::validateUserRole(const QString& role) const
{
    return role == "teacher" || role == "student";
}
