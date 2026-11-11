-- Esquema SQLite - Gestor de Inventario para Motocicletas
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS usuarios (
    id            INTEGER PRIMARY KEY AUTOINCREMENT,
    username      TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,
    salt          TEXT NOT NULL,
    rol           TEXT NOT NULL DEFAULT 'operador' CHECK (rol IN ('admin','operador')),
    created_at    TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE TABLE IF NOT EXISTS categorias (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre      TEXT NOT NULL UNIQUE,
    descripcion TEXT
);

CREATE TABLE IF NOT EXISTS marcas (
    id     INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre TEXT NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS articulos (
    id                INTEGER PRIMARY KEY AUTOINCREMENT,
    codigo            TEXT NOT NULL UNIQUE,
    nombre            TEXT NOT NULL,
    descripcion       TEXT,
    id_categoria      INTEGER NOT NULL REFERENCES categorias(id),
    id_marca          INTEGER REFERENCES marcas(id),
    precio_compra     REAL NOT NULL DEFAULT 0 CHECK (precio_compra >= 0),
    precio_venta      REAL NOT NULL DEFAULT 0 CHECK (precio_venta >= 0),
    stock_actual      INTEGER NOT NULL DEFAULT 0 CHECK (stock_actual >= 0),
    stock_minimo      INTEGER NOT NULL DEFAULT 0 CHECK (stock_minimo >= 0),
    ubicacion         TEXT,
    fecha_adquisicion TEXT,
    activo            INTEGER NOT NULL DEFAULT 1 CHECK (activo IN (0,1))
);

CREATE TABLE IF NOT EXISTS movimientos (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    id_articulo INTEGER NOT NULL REFERENCES articulos(id),
    id_usuario  INTEGER NOT NULL REFERENCES usuarios(id),
    tipo        TEXT NOT NULL CHECK (tipo IN ('ENTRADA','SALIDA')),
    cantidad    INTEGER NOT NULL CHECK (cantidad > 0),
    motivo      TEXT,
    fecha_hora  TEXT NOT NULL DEFAULT (datetime('now'))
);

CREATE INDEX IF NOT EXISTS idx_articulos_categoria ON articulos(id_categoria);
CREATE INDEX IF NOT EXISTS idx_articulos_marca ON articulos(id_marca);
CREATE INDEX IF NOT EXISTS idx_movimientos_articulo ON movimientos(id_articulo);
