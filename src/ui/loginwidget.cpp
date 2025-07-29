#include "loginwidget.h"
#include "appmanager.h"

#include <QMessageBox>
#include <QKeyEvent>
#include <QStyle>
#include <QApplication>

LoginWidget::LoginWidget(AppManager* appManager, QWidget *parent)
    : QWidget(parent)
    , appManager(appManager)
    , mainLayout(nullptr)
    , loginGroup(nullptr)
    , loginLayout(nullptr)
{
    setupUI();
    setupConnections();
    applyStyles();
}

LoginWidget::~LoginWidget()
{
    // Cleanup handled by Qt parent-child relationship
}

void LoginWidget::setupUI()
{
    // Main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    
    // Title section
    titleLabel = new QLabel("Exam Management System");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");
    
    subtitleLabel = new QLabel("CTDL&GT - Thi Trắc Nghiệm");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setObjectName("subtitleLabel");
    
    // Logo placeholder
    logoLabel = new QLabel();
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setFixedSize(80, 80);
    logoLabel->setStyleSheet("border: 2px solid #3498db; border-radius: 40px; background-color: #ecf0f1;");
    logoLabel->setText("LOGO");
    
    // Login group box
    loginGroup = new QGroupBox("Login");
    loginGroup->setFixedWidth(400);
    loginGroup->setObjectName("loginGroup");
    
    loginLayout = new QGridLayout(loginGroup);
    loginLayout->setSpacing(15);
    loginLayout->setContentsMargins(30, 30, 30, 30);
    
    // Username field
    usernameLabel = new QLabel("Username:");
    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Enter username");
    usernameEdit->setObjectName("usernameEdit");
    
    // Password field
    passwordLabel = new QLabel("Password:");
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Enter password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setObjectName("passwordEdit");
    
    // Buttons
    loginButton = new QPushButton("Login");
    loginButton->setObjectName("loginButton");
    loginButton->setDefault(true);
    loginButton->setEnabled(false);
    
    clearButton = new QPushButton("Clear");
    clearButton->setObjectName("clearButton");
    
    // Message label
    messageLabel = new QLabel();
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setObjectName("messageLabel");
    messageLabel->setWordWrap(true);
    messageLabel->hide();
    
    // Layout assembly
    loginLayout->addWidget(usernameLabel, 0, 0);
    loginLayout->addWidget(usernameEdit, 0, 1);
    loginLayout->addWidget(passwordLabel, 1, 0);
    loginLayout->addWidget(passwordEdit, 1, 1);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(loginButton);
    loginLayout->addLayout(buttonLayout, 2, 0, 1, 2);
    
    loginLayout->addWidget(messageLabel, 3, 0, 1, 2);
    
    // Add to main layout
    mainLayout->addWidget(logoLabel);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addWidget(loginGroup);
    mainLayout->addStretch();
}

void LoginWidget::setupConnections()
{
    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
    connect(clearButton, &QPushButton::clicked, this, &LoginWidget::clearFields);
    connect(usernameEdit, &QLineEdit::textChanged, this, &LoginWidget::onUsernameChanged);
    connect(passwordEdit, &QLineEdit::textChanged, this, &LoginWidget::onPasswordChanged);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWidget::onEnterPressed);
    connect(usernameEdit, &QLineEdit::returnPressed, this, &LoginWidget::onEnterPressed);
}

void LoginWidget::applyStyles()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f5f5f5;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        
        #titleLabel {
            font-size: 28px;
            font-weight: bold;
            color: #2c3e50;
            margin: 10px;
        }
        
        #subtitleLabel {
            font-size: 16px;
            color: #7f8c8d;
            margin: 5px;
        }
        
        #loginGroup {
            background-color: white;
            border: 1px solid #bdc3c7;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
            color: #2c3e50;
        }
        
        #loginGroup::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        
        QLabel {
            color: #34495e;
            font-size: 14px;
        }
        
        #usernameEdit, #passwordEdit {
            padding: 8px;
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            font-size: 14px;
            background-color: white;
        }
        
        #usernameEdit:focus, #passwordEdit:focus {
            border-color: #3498db;
        }
        
        #loginButton {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 80px;
        }
        
        #loginButton:hover {
            background-color: #2980b9;
        }
        
        #loginButton:pressed {
            background-color: #21618c;
        }
        
        #loginButton:disabled {
            background-color: #bdc3c7;
            color: #7f8c8d;
        }
        
        #clearButton {
            background-color: #95a5a6;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            min-width: 80px;
        }
        
        #clearButton:hover {
            background-color: #7f8c8d;
        }
        
        #clearButton:pressed {
            background-color: #6c7b7d;
        }
        
        #messageLabel {
            font-size: 12px;
            padding: 5px;
            border-radius: 4px;
            margin-top: 10px;
        }
        
        #messageLabel[error="true"] {
            background-color: #e74c3c;
            color: white;
        }
        
        #messageLabel[info="true"] {
            background-color: #3498db;
            color: white;
        }
    )");
}

void LoginWidget::onLoginClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        setErrorMessage("Please enter both username and password");
        return;
    }
    
    // Clear any previous messages
    messageLabel->hide();
    
    // Disable login button during authentication
    loginButton->setEnabled(false);
    loginButton->setText("Logging in...");
    
    // Attempt login
    if (appManager->login(username, password)) {
        setInfoMessage("Login successful!");
        emit loginSuccessful();
    } else {
        setErrorMessage("Invalid username or password");
        loginButton->setEnabled(true);
        loginButton->setText("Login");
    }
}

void LoginWidget::onUsernameChanged()
{
    validateInput();
}

void LoginWidget::onPasswordChanged()
{
    validateInput();
}

void LoginWidget::onEnterPressed()
{
    if (loginButton->isEnabled()) {
        onLoginClicked();
    }
}

void LoginWidget::validateInput()
{
    bool hasUsername = !usernameEdit->text().trimmed().isEmpty();
    bool hasPassword = !passwordEdit->text().isEmpty();
    
    loginButton->setEnabled(hasUsername && hasPassword);
    
    // Hide message when user starts typing
    if (messageLabel->isVisible()) {
        messageLabel->hide();
    }
}

void LoginWidget::clearFields()
{
    usernameEdit->clear();
    passwordEdit->clear();
    messageLabel->hide();
    
    // Reset login button state
    loginButton->setText("Login");
    loginButton->setEnabled(false); // Will be enabled when user types
    
    usernameEdit->setFocus();
}

void LoginWidget::setErrorMessage(const QString& message)
{
    messageLabel->setText(message);
    messageLabel->setProperty("error", true);
    messageLabel->setProperty("info", false);
    QApplication::style()->unpolish(messageLabel);
    QApplication::style()->polish(messageLabel);
    messageLabel->show();
}

void LoginWidget::setInfoMessage(const QString& message)
{
    messageLabel->setText(message);
    messageLabel->setProperty("error", false);
    messageLabel->setProperty("info", true);
    QApplication::style()->unpolish(messageLabel);
    QApplication::style()->polish(messageLabel);
    messageLabel->show();
}