#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QPixmap>
#include <QMovie>

class AppManager;

/**
 * @brief Login widget for user authentication
 */
class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(AppManager* appManager, QWidget *parent = nullptr);
    ~LoginWidget();

public slots:
    void clearFields();
    void setErrorMessage(const QString& message);
    void setInfoMessage(const QString& message);

private slots:
    void onLoginClicked();
    void onUsernameChanged();
    void onPasswordChanged();
    void onEnterPressed();

signals:
    void loginRequested(const QString& username, const QString& password);
    void loginSuccessful();

private:
    void setupUI();
    void setupConnections();
    void validateInput();

    AppManager* appManager;

    // UI components
    QVBoxLayout* mainLayout;
    QGroupBox* loginGroup;
    QGridLayout* loginLayout;
    
    QLabel* titleLabel;
    QLabel* subtitleLabel;
    QLabel* logoLabel;
    QLabel* usernameLabel;
    QLabel* passwordLabel;
    QLabel* messageLabel;
    
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    QPushButton* clearButton;
    
    // Styling
    void applyStyles();
};

#endif // LOGINWIDGET_H