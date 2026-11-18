#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QString>

class AuthService
{
public:
    // Intenta iniciar sesión. Devuelve true si es exitoso.
    static bool login(const QString& username, const QString& password);
    
    // Crea un usuario nuevo (útil para el admin inicial)
    static bool crearUsuario(const QString& username, const QString& password, const QString& rol);

private:
    static QString generarSalt();
    static QString hashPassword(const QString& password, const QString& salt);
};

#endif // AUTHSERVICE_H
