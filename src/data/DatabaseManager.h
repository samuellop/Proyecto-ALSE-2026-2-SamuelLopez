#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    bool init();
    QSqlDatabase getDatabase() const;

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool crearTablas();
    bool insertarDatosIniciales();
    const QString nombreConexion = "alse_conexion_inventario";
};

#endif // DATABASEMANAGER_H
