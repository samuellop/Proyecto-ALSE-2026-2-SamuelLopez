#include "LoginWindow.h"
#include "core/AuthService.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("ALSE - Iniciar Sesión");
    setFixedSize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Gestor de Inventario", this);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(14);
    font.setBold(true);
    title->setFont(font);
    layout->addWidget(title);

    m_userEdit = new QLineEdit(this);
    m_userEdit->setPlaceholderText("Usuario");
    layout->addWidget(m_userEdit);

    m_passEdit = new QLineEdit(this);
    m_passEdit->setPlaceholderText("Contraseña");
    m_passEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_passEdit);

    m_loginButton = new QPushButton("Ingresar", this);
    layout->addWidget(m_loginButton);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    QString user = m_userEdit->text().trimmed();
    QString pass = m_passEdit->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Por favor, llene todos los campos.");
        return;
    }

    if (AuthService::login(user, pass)) {
        emit loginExitoso();
        this->close();
    } else {
        QMessageBox::critical(this, "Error", "Usuario o contraseña incorrectos.");
        m_passEdit->clear();
    }
}
