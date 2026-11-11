# Propuesta de Proyecto Final — ALSE 2026-2

**Curso:** Algoritmos en Sistemas Electrónicos
**Docente:** Ing. Daniel Palomino Suarez.
**Estudiante:** Samuel López
**Repositorio:** PROYECTO-ALSE-2026-2-SAMUEL-LÓPEZ
**Fecha:** 11/11/2026

## Propuesta seleccionada

**3.3 Gestor de Inventario**, con variante de dominio aplicada al sector de
**motocicletas** (taller / concesionario / almacén de repuestos).

## Justificación

1. Cumple los elementos fundamentales exigidos en la sección 2 del documento:
   GUI en Qt, persistencia en SQLite y diseño orientado a objetos.
2. Los requerimientos funcionales del numeral 3.3 (registro de artículos,
   búsqueda y filtrado dinámico, alertas de stock bajo, generación de reportes)
   se alinean directamente con el dominio de repuestos y accesorios de moto.
3. El dominio permite un modelado de datos realista y normalizado
   (categorías, marcas, modelos de moto, compatibilidades repuesto–modelo) e
   incorpora el historial de movimientos de inventario como valor agregado.
4. Es posible evaluar adicionales del documento (exportación a PDF, dashboard
   con QtCharts, lectura de códigos de barras/QR) sujeto al avance del cronograma.

## Alcance preliminar

- Autenticación de usuarios con hash + salt.
- Registro, edición, consulta y baja lógica de artículos (repuestos, insumos,
  accesorios).
- Búsqueda y filtrado dinámico por nombre, categoría, marca y estado de stock.
- Alertas de stock bajo según umbral configurable por artículo.
- Registro de movimientos de entrada/salida y consulta de historial.
- Exportación de reportes en CSV.
- Documentación con Doxygen, diagramas UML y modelo entidad-relación.
- Repositorio GitHub organizado con control de versiones.

## Modelo entidad-relación preliminar

Tablas mínimas: `usuarios`, `categorias`, `marcas`, `articulos`, `movimientos`.

Tablas extendidas: `modelos_moto`, `compatibilidades` (N:M entre
artículos y modelos de moto).

## Estructura propuesta del repositorio
PROYECTO-ALSE-2026-2-SAMUEL-LÓPEZ/
├── CMakeLists.txt
├── README.md
├── docs/ # Doxygen, UML, MER
├── db/ # schema.sql + seed
├── src/
│ ├── core/ # entidades y lógica (sin Qt Widgets)
│ ├── data/ # repositorios + DatabaseManager
│ └── ui/ # ventanas y widgets Qt
└── tests/ # pruebas 