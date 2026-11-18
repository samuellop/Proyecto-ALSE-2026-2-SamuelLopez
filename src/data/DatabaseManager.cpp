#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instancia;
    return instancia;
}

bool DatabaseManager::init()
{
    QString rutaAppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(rutaAppData);
    if (!dir.exists()) dir.mkpath(".");

    QString rutaDb = rutaAppData + "/inventario_motos.sqlite";
    qDebug() << "Ruta de la base de datos:" << rutaDb;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", nombreConexion);
    db.setDatabaseName(rutaDb);

    if (!db.open()) {
        qCritical() << "Error al abrir la base de datos:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.exec("PRAGMA foreign_keys = ON;");

    if (!crearTablas()) return false;
    if (!insertarDatosIniciales()) return false;

    return true;
}

QSqlDatabase DatabaseManager::getDatabase() const
{
    return QSqlDatabase::database(nombreConexion);
}

bool DatabaseManager::crearTablas()
{
    QSqlQuery query(getDatabase());
    
    QString sql = R"(
        CREATE TABLE IF NOT EXISTS usuarios (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL,
            salt TEXT NOT NULL,
            rol TEXT NOT NULL DEFAULT 'operador',
            created_at TEXT NOT NULL DEFAULT (datetime('now'))
        );
        CREATE TABLE IF NOT EXISTS categorias (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nombre TEXT NOT NULL UNIQUE,
            descripcion TEXT
        );
        CREATE TABLE IF NOT EXISTS marcas (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nombre TEXT NOT NULL UNIQUE
        );
        CREATE TABLE IF NOT EXISTS articulos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            codigo TEXT NOT NULL UNIQUE,
            nombre TEXT NOT NULL,
            descripcion TEXT,
            id_categoria INTEGER NOT NULL REFERENCES categorias(id),
            id_marca INTEGER REFERENCES marcas(id),
            precio_compra REAL NOT NULL DEFAULT 0,
            precio_venta REAL NOT NULL DEFAULT 0,
            stock_actual INTEGER NOT NULL DEFAULT 0,
            stock_minimo INTEGER NOT NULL DEFAULT 0,
            ubicacion TEXT,
            fecha_adquisicion TEXT,
            activo INTEGER NOT NULL DEFAULT 1
        );
        CREATE TABLE IF NOT EXISTS movimientos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_articulo INTEGER NOT NULL REFERENCES articulos(id),
            id_usuario INTEGER NOT NULL REFERENCES usuarios(id),
            tipo TEXT NOT NULL,
            cantidad INTEGER NOT NULL,
            motivo TEXT,
            fecha_hora TEXT NOT NULL DEFAULT (datetime('now'))
        );
    )";

    QStringList sentencias = sql.split(';', Qt::SkipEmptyParts);
    for (const QString& sentencia : sentencias) {
        if (sentencia.trimmed().isEmpty()) continue;
        if (!query.exec(sentencia)) {
            qCritical() << "Error creando tabla:" << query.lastError().text();
            return false;
        }
    }
    return true;
}

bool DatabaseManager::insertarDatosIniciales()
{
    QSqlQuery query(getDatabase());
    query.exec("INSERT OR IGNORE INTO categorias (nombre) VALUES ('Motor'), ('Frenos'), ('Lubricantes'), ('Llantas'), ('Eléctrico'), ('Transmisión'), ('Accesorios'), ('Carenaje');");
    query.exec("INSERT OR IGNORE INTO marcas (nombre) VALUES ('Yamaha'), ('Honda'), ('Suzuki'), ('Bajaj'), ('AKT'), ('KTM'), ('Kawasaki'), ('TVS');");
    return true;
}
