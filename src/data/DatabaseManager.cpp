#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QCryptographicHash> // Necesario para el hash del admin

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instancia;
    return instancia;
}

bool DatabaseManager::init()
{
    // Usamos una ruta local en la carpeta del usuario para no ensuciar el repo
    QString rutaAppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(rutaAppData);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString rutaDb = rutaAppData + "/inventario_motos.sqlite";
    qDebug() << "Ruta de la base de datos:" << rutaDb;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", nombreConexion);
    db.setDatabaseName(rutaDb);

    if (!db.open()) {
        qCritical() << "Error al abrir la base de datos:" << db.lastError().text();
        return false;
    }

    // Habilitar llaves foráneas en SQLite
    QSqlQuery query(db);
    query.exec("PRAGMA foreign_keys = ON;");

    if (!crearTablas()) {
        return false;
    }

    if (!insertarDatosIniciales()) {
        return false;
    }

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
            rol TEXT NOT NULL DEFAULT 'operador' CHECK (rol IN ('admin','operador')),
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
            precio_compra REAL NOT NULL DEFAULT 0 CHECK (precio_compra >= 0),
            precio_venta REAL NOT NULL DEFAULT 0 CHECK (precio_venta >= 0),
            stock_actual INTEGER NOT NULL DEFAULT 0 CHECK (stock_actual >= 0),
            stock_minimo INTEGER NOT NULL DEFAULT 0 CHECK (stock_minimo >= 0),
            ubicacion TEXT,
            fecha_adquisicion TEXT,
            activo INTEGER NOT NULL DEFAULT 1 CHECK (activo IN (0,1))
        );

        CREATE TABLE IF NOT EXISTS movimientos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_articulo INTEGER NOT NULL REFERENCES articulos(id),
            id_usuario INTEGER NOT NULL REFERENCES usuarios(id),
            tipo TEXT NOT NULL CHECK (tipo IN ('ENTRADA','SALIDA')),
            cantidad INTEGER NOT NULL CHECK (cantidad > 0),
            motivo TEXT,
            fecha_hora TEXT NOT NULL DEFAULT (datetime('now'))
        );

        CREATE INDEX IF NOT EXISTS idx_articulos_categoria ON articulos(id_categoria);
        CREATE INDEX IF NOT EXISTS idx_articulos_marca ON articulos(id_marca);
        CREATE INDEX IF NOT EXISTS idx_movimientos_articulo ON movimientos(id_articulo);
    )";

    // Ejecutar las sentencias separadas por punto y coma
    QStringList sentencias = sql.split(';', Qt::SkipEmptyParts);
    for (const QString& sentencia : sentencias) {
        if (sentencia.trimmed().isEmpty()) continue;
        if (!query.exec(sentencia)) {
            qCritical() << "Error creando tabla:" << query.lastError().text();
            qCritical() << "Sentencia:" << sentencia;
            return false;
        }
    }

    return true;
}

bool DatabaseManager::insertarDatosIniciales()
{
    QSqlQuery query(getDatabase());
    
    // Insertar categorías
    query.exec("INSERT OR IGNORE INTO categorias (nombre, descripcion) VALUES "
               "('Motor', 'Pistones, cilindros, válvulas, bielas'),"
               "('Frenos', 'Pastillas, discos, líquido de frenos, manetas'),"
               "('Lubricantes', 'Aceites de motor, transmisión, filtros'),"
               "('Llantas', 'Llantas y cámaras'),"
               "('Eléctrico', 'Baterías, bujías, faros, reguladores'),"
               "('Transmisión', 'Cadenas, piñones, coronas, kit de arrastre'),"
               "('Accesorios', 'Maleteros, espejos, alforjas, tapicería'),"
               "('Carenaje', 'Guardafangos, tanques, laterales');");

    // Insertar marcas
    query.exec("INSERT OR IGNORE INTO marcas (nombre) VALUES "
               "('Yamaha'), ('Honda'), ('Suzuki'), ('Bajaj'), "
               "('AKT'), ('KTM'), ('Kawasaki'), ('TVS');");

    // Crear usuario admin por defecto si la tabla está vacía
    query.exec("SELECT COUNT(*) FROM usuarios");
    if (query.next() && query.value(0).toInt() == 0) {
        // Creamos un salt fijo para el admin inicial (en un sistema real, usa QUuid)
        QString salt = "admin_salt_123"; 
        QString passCombinada = "admin123" + salt;
        QByteArray hash = QCryptographicHash::hash(passCombinada.toUtf8(), QCryptographicHash::Sha256);
        
        QSqlQuery insertUser(getDatabase());
        insertUser.prepare("INSERT INTO usuarios (username, password_hash, salt, rol) VALUES ('admin', ?, ?, 'admin')");
        insertUser.addBindValue(QString(hash.toHex()));
        insertUser.addBindValue(salt);
        
        if (insertUser.exec()) {
            qDebug() << "Usuario 'admin' creado con contraseña 'admin123'";
        } else {
            qCritical() << "Error creando admin:" << insertUser.lastError().text();
        }
    }

    return true;
}
