#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include <QString>
#include "../models/SinhVien.h"
#include "../managers/QuanLyDiem.h"

// Forward declarations
class QuanLyLop;

/**
 * @brief Authentication Controller
 * Handles teacher and student authentication
 */
class AuthController : public QObject {
    Q_OBJECT

public:
    explicit AuthController(QObject *parent = nullptr);

    ~AuthController();

    // Set reference to class manager for student authentication
    void setClassManager(QuanLyLop *manager);

public slots:
    void authenticate(QString username, QString password, QString userType);

signals:
    void loginSuccess(QString userType, SinhVien *student = nullptr);

    void loginFailed(QString errorMessage);

private:
    QuanLyLop *classManager;

    bool authenticateTeacher(const QString &username, const QString &password);

    SinhVien *authenticateStudent(const QString &masv, const QString &password);
};

#endif // AUTHCONTROLLER_H
