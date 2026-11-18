#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginExitoso();

private slots:
    void onLoginClicked();

private:
    QLineEdit *m_userEdit;
    QLineEdit *m_passEdit;
    QPushButton *m_loginButton;
};

#endif // LOGINWINDOW_H
