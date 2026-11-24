# Gestor de Inventario para Motocicletas 🏍️

Proyecto final del curso **Algoritmos en Sistemas Electrónicos** (ALSE 2026-2), Escuela Colombiana de Ingeniería Julio Garavito.

- **Estudiante:** Samuel López
- **Docente:** Ing. Daniel Palomino Suarez MSc.
- **Video de funcionamiento:** [ver video](https://youtu.be/qtIfLGqzIV0?si=rNshCaKJys5mX26O)

## Descripción
Aplicación de escritorio en C++ con Qt 6 para llevar el inventario de repuestos y accesorios de un taller de motos. Permite registrar y editar repuestos, buscarlos, ver alertas de stock bajo, registrar entradas y salidas de mercancía y exportar el inventario a CSV.

## Funcionalidades
- Inicio de sesión con contraseñas guardadas como hash SHA-256 + salt.
- Registro, edición y baja lógica de repuestos.
- Búsqueda por código o nombre mientras se escribe.
- Filtro de alertas de stock bajo (`stock_actual <= stock_minimo`).
- Movimientos de entrada/salida con actualización del stock en una transacción.
- Exportación del inventario a CSV.

## Tecnologías
- **Lenguaje:** C++17
- **Framework GUI:** Qt 6 (Widgets, Sql)
- **Base de datos:** SQLite
- **Construcción:** CMake
- **Documentación:** Doxygen

## Estructura
```
src/
├── core/   AuthService (autenticación)
├── data/   DatabaseManager (conexión y esquema SQLite)
└── ui/     LoginWindow, MainWindow, ArticuloDialog, MovimientoDialog
db/         schema.sql, seed.sql
docs/       propuesta.md
```

## Compilar y ejecutar (Ubuntu 22.04)
```bash
sudo apt install qt6-base-dev libqt6sql6-sqlite libqt6charts6-dev libgl1-mesa-dev cmake g++
git clone https://github.com/samuellop/Proyecto-ALSE-2026-2-SamuelLopez.git
cd Proyecto-ALSE-2026-2-SamuelLopez
mkdir build && cd build
cmake .. && make
./ALSE_InventarioMotos
```

**Usuario por defecto:** `admin` / `admin123`

La base de datos se crea sola en `~/.local/share/ALSE_InventarioMotos/`.

## Documentación
```bash
doxygen Doxyfile
```
Genera la documentación en `html/` y `latex/`.

## Estado
✅ Versión 1.0 — entrega final.
