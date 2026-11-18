#include "AuthService.h"
#include "data/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>
#include <QUuid>

QString AuthService::generarSalt()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString AuthService::hashPassword(const QString& password, const QString& salt)
{
    QString combinado = password + salt;
    QByteArray hash = QCryptographicHash::hash(combinado.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool AuthService::crearUsuario(const QString& username, const QString& password, const QString& rol)
{
    QString salt = generarSalt();
    QString hash = hashPassword(password, salt);
    
    QSqlQuery query(DatabaseManager::instance().getDatabase());
    query.prepare("INSERT INTO usuarios (username, password_hash, salt, rol) VALUES (?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(hash);
    query.addBindValue(salt);
    query.addBindValue(rol);
    
    if (!query.exec()) {
        qCritical() << "Error al crear usuario:" << query.lastError().text();
        return false;
    }
    return true;
}

bool AuthService::login(const QString& username, const QString& password)
{
    QSqlQuery query(DatabaseManager::instance().getDatabase());
    query.prepare("SELECT password_hash, salt FROM usuarios WHERE username = ?");
    query.addBindValue(username);
    
    if (!query.exec() || !query.next()) {
        return false; // Usuario no encontrado
    }
    
    QString hashGuardado = query.value(0).toString();
    QString saltGuardado = query.value(1).toString();
    
    QString hashIngresado = hashPassword(password, saltGuardado);
    return hashGuardado == hashIngresado;
}
