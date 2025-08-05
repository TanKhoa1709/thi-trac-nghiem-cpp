#include "AuthController.h"
#include "../managers/quanlylop.h"
#include "../utils/DynamicArray.h"
#include "../managers/quanlysinhvien.h"

AuthController::AuthController(QObject *parent)
    : QObject(parent), classManager(nullptr)
{
}

AuthController::~AuthController()
{
    // Don't delete classManager - it's managed elsewhere
}

void AuthController::setClassManager(QuanLyLop *manager)
{
    classManager = manager;
}

void AuthController::authenticate(QString username, QString password, QString userType)
{
    try
    {
        if (userType == "Teacher")
        {
            if (authenticateTeacher(username, password))
            {
                emit loginSuccess("Teacher");
            }
            else
            {
                emit loginFailed("Invalid teacher credentials");
            }
        }
        else if (userType == "Student")
        {
            SinhVien *student = authenticateStudent(username, password);
            if (student)
            {
                emit loginSuccess("Student", student);
            }
            else
            {
                emit loginFailed("Invalid student credentials");
            }
        }
        else
        {
            emit loginFailed("Invalid user type");
        }
    }
    catch (const std::exception &e)
    {
        emit loginFailed(QString("Authentication error: %1").arg(e.what()));
    }
}

bool AuthController::authenticateTeacher(const QString &username, const QString &password)
{
    return (username == "GV" && password == "GV");
}

SinhVien *AuthController::authenticateStudent(const QString &masv, const QString &password)
{
    if (!classManager)
    {
        return nullptr;
    }

    // Search for student in all classes
    DynamicArray<Lop *> danhSachLop;
    classManager->danhSach(danhSachLop);

    for (int i = 0; i < danhSachLop.size(); i++)
    {
        Lop *lop = danhSachLop.get(i);
        if (lop && lop->getQuanLySinhVien())
        {
            SinhVien *sv = lop->getQuanLySinhVien()->tim(masv.toStdString());
            if (sv && sv->getPassword() == password.toStdString())
            {
                return sv;
            }
        }
    }

    return nullptr;
}
