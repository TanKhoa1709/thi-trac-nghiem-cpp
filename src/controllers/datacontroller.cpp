#include "datacontroller.h"
#include "monhoc.h"
#include "cauhoi.h"
#include "lop.h"
#include "sinhvien.h"
#include "diemthi.h"

DataController::DataController(QObject* parent)
    : QObject(parent)
    , monHocManager(nullptr)
    , cauHoiManager(nullptr)
    , lopManager(nullptr)
    , sinhVienManager(nullptr)
    , diemThiManager(nullptr)
{
}

DataController::~DataController()
{
}

void DataController::setManagers(QuanLyMonHoc* mh, QuanLyCauHoi* ch, QuanLyLop* l, 
                                 QuanLySinhVien* sv, QuanLyDiemThi* dt)
{
    monHocManager = mh;
    cauHoiManager = ch;
    lopManager = l;
    sinhVienManager = sv;
    diemThiManager = dt;
}

// Subject management
bool DataController::addSubject(const QString& id, const QString& name, int credits)
{
    if (!monHocManager) return false;
    
    MonHoc monHoc(id, name, credits);
    bool success = monHocManager->themMonHoc(monHoc);
    
    if (success) {
        // Signal emission removed for simplified build
    }
    
    return success;
}

bool DataController::removeSubject(const QString& id)
{
    if (!monHocManager) return false;
    
    bool success = monHocManager->xoaMonHoc(id);
    
    if (success) {
        // Signal emission removed for simplified build
    }
    
    return success;
}

QVariantMap DataController::getSubject(const QString& id)
{
    QVariantMap result;
    
    if (!monHocManager) return result;
    
    MonHoc* monHoc = monHocManager->timMonHoc(id);
    if (monHoc) {
        result["id"] = monHoc->getMaMon();
        result["name"] = monHoc->getTenMon();
        result["credits"] = monHoc->getSoTinChi();
    }
    
    return result;
}

QVariantList DataController::getAllSubjects()
{
    QVariantList result;
    
    if (!monHocManager) return result;
    
    QList<MonHoc> subjects = monHocManager->layDanhSachMonHoc();
    for (const MonHoc& mh : subjects) {
        QVariantMap subject;
        subject["id"] = mh.getMaMon();
        subject["name"] = mh.getTenMon();
        subject["credits"] = mh.getSoTinChi();
        result.append(subject);
    }
    
    return result;
}

// Question management
bool DataController::addQuestion(const QVariantMap& questionData)
{
    if (!cauHoiManager) return false;
    
    if (!validateQuestionData(questionData)) {
        // Signal emission removed for simplified build
        return false;
    }
    
    int id = questionData["id"].toInt();
    QString content = questionData["content"].toString();
    QStringList answers = questionData["answers"].toStringList();
    QString correctAnswer = questionData["correctAnswer"].toString();
    QString subjectId = questionData["subjectId"].toString();
    int difficulty = questionData["difficulty"].toInt();
    
    CauHoi cauHoi(id, content, answers, correctAnswer.at(0).toLatin1(), subjectId, difficulty);
    bool success = cauHoiManager->themCauHoi(cauHoi);
    
    if (success) {
        // Signal emission removed for simplified build
    }
    
    return success;
}

QVariantList DataController::getQuestionsBySubject(const QString& subjectId)
{
    QVariantList result;
    
    if (!cauHoiManager) return result;
    
    QList<CauHoi> questions = cauHoiManager->layCauHoiTheoMon(subjectId);
    for (const CauHoi& ch : questions) {
        QVariantMap question;
        question["id"] = ch.getId();
        question["content"] = ch.getNoiDung();
        question["answers"] = ch.getDapAn();
        question["correctAnswer"] = QString(ch.getDapAnDung());
        question["subjectId"] = ch.getMaMon();
        question["difficulty"] = ch.getMucDo();
        result.append(question);
    }
    
    return result;
}

// Student management
bool DataController::addStudent(const QVariantMap& studentData)
{
    if (!sinhVienManager) return false;
    
    QString id = studentData["id"].toString();
    QString lastName = studentData["lastName"].toString();
    QString firstName = studentData["firstName"].toString();
    QDate birthDate = studentData["birthDate"].toDate();
    QString gender = studentData["gender"].toString();
    QString phone = studentData["phone"].toString();
    QString email = studentData["email"].toString();
    QString classId = studentData["classId"].toString();
    
    SinhVien sinhVien(id, lastName, firstName, birthDate, gender, phone, email, classId);
    bool success = sinhVienManager->themSinhVien(sinhVien);
    
    if (success) {
        // Signal emission removed for simplified build
    }
    
    return success;
}

QVariantList DataController::getStudentsByClass(const QString& classId)
{
    QVariantList result;
    
    if (!sinhVienManager) return result;
    
    QList<SinhVien> allStudents = sinhVienManager->layTatCaSinhVien();
    for (const SinhVien& sv : allStudents) {
        if (sv.getMaLop() == classId) {
            QVariantMap student;
            student["id"] = sv.getMaSV();
            student["lastName"] = sv.getHo();
            student["firstName"] = sv.getTen();
            student["fullName"] = sv.getHoTen();
            student["birthDate"] = sv.getNgaySinh();
            student["gender"] = sv.getGioiTinh();
            student["phone"] = sv.getSoDienThoai();
            student["email"] = sv.getEmail();
            student["classId"] = sv.getMaLop();
            result.append(student);
        }
    }
    
    return result;
}

// Class management
bool DataController::addClass(const QString& id, const QString& name, int year, const QString& department)
{
    if (!lopManager) return false;
    
    Lop lop(id, name, year, department);
    bool success = lopManager->themLop(lop);
    
    if (success) {
        // Signal emission removed for simplified build
    }
    
    return success;
}

QVariantList DataController::getAllClasses()
{
    QVariantList result;
    
    if (!lopManager) return result;
    
    for (int i = 0; i < lopManager->getSoLuongLop(); ++i) {
        Lop* lop = lopManager->layLop(i);
        if (lop) {
            QVariantMap classInfo;
            classInfo["id"] = lop->getMaLop();
            classInfo["name"] = lop->getTenLop();
            classInfo["year"] = lop->getNienKhoa();
            classInfo["department"] = lop->getKhoa();
            classInfo["studentCount"] = lop->soLuongSinhVien();
            result.append(classInfo);
        }
    }
    
    return result;
}

// Statistics
QVariantMap DataController::getOverallStatistics()
{
    QVariantMap stats;
    
    stats["totalSubjects"] = monHocManager ? monHocManager->soLuongMonHoc() : 0;
    stats["totalQuestions"] = cauHoiManager ? cauHoiManager->tongSoCauHoi() : 0;
    stats["totalClasses"] = lopManager ? lopManager->getSoLuongLop() : 0;
    stats["totalStudents"] = sinhVienManager ? sinhVienManager->tongSoSinhVien() : 0;
    stats["totalExams"] = diemThiManager ? diemThiManager->tongSoBaiThi() : 0;
    
    return stats;
}

// Validation
bool DataController::validateSubjectId(const QString& id)
{
    return monHocManager ? !monHocManager->kiemTraMaMonTonTai(id) : false;
}

bool DataController::validateStudentId(const QString& id)
{
    return sinhVienManager ? !sinhVienManager->kiemTraMaSVTonTai(id) : false;
}

bool DataController::validateQuestionData(const QVariantMap& data)
{
    QStringList required = {"id", "content", "answers", "correctAnswer", "subjectId"};
    return validateRequiredFields(data, required);
}

bool DataController::validateRequiredFields(const QVariantMap& data, const QStringList& requiredFields)
{
    for (const QString& field : requiredFields) {
        if (!data.contains(field) || data[field].toString().isEmpty()) {
            return false;
        }
    }
    return true;
}
