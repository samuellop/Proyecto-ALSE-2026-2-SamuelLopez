#include "MainWindow.h"
#include "data/DatabaseManager.h"
#include "ArticuloDialog.h"
#include "MovimientoDialog.h" // Nuevo include
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCheckBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ALSE - Gestor de Inventario de Motos");
    resize(900, 600);

    configurarUI();
    configurarModelo();
}

void MainWindow::configurarUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Barra de herramientas superior ---
    QHBoxLayout *topLayout = new QHBoxLayout();
    
    m_buscarEdit = new QLineEdit(this);
    m_buscarEdit->setPlaceholderText("Buscar por código o nombre...");
    topLayout->addWidget(new QLabel("Buscar:", this));
    topLayout->addWidget(m_buscarEdit);

    QCheckBox *checkStockBajo = new QCheckBox("Solo alertas de stock bajo", this);
    topLayout->addWidget(checkStockBajo);

    m_btnExportar = new QPushButton("Exportar CSV", this);
    topLayout->addWidget(m_btnExportar);

    mainLayout->addLayout(topLayout);

    // --- Tabla ---
    m_tabla = new QTableView(this);
    m_tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tabla->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(m_tabla);

    // --- Botones inferiores ---
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    m_btnAgregar = new QPushButton("Agregar Repuesto", this);
    m_btnEditar = new QPushButton("Editar", this);
    m_btnMovimiento = new QPushButton("Registrar Entrada/Salida", this); // Nuevo
    m_btnEliminar = new QPushButton("Eliminar", this);
    
    bottomLayout->addWidget(m_btnAgregar);
    bottomLayout->addWidget(m_btnEditar);
    bottomLayout->addWidget(m_btnMovimiento); // Nuevo
    bottomLayout->addWidget(m_btnEliminar);
    bottomLayout->addStretch();

    mainLayout->addLayout(bottomLayout);

    // --- Conexiones ---
    connect(m_buscarEdit, &QLineEdit::textChanged, this, &MainWindow::filtrarTabla);
    connect(checkStockBajo, &QCheckBox::toggled, this, &MainWindow::mostrarSoloStockBajo);
    connect(m_btnExportar, &QPushButton::clicked, this, &MainWindow::exportarCSV);
    connect(m_btnAgregar, &QPushButton::clicked, this, &MainWindow::agregarArticulo);
    connect(m_btnEditar, &QPushButton::clicked, this, &MainWindow::editarArticulo);
    connect(m_btnMovimiento, &QPushButton::clicked, this, &MainWindow::registrarMovimiento); // Nueva conexión
    connect(m_btnEliminar, &QPushButton::clicked, this, &MainWindow::eliminarArticulo);
}

void MainWindow::configurarModelo()
{
    m_modelo = new QSqlTableModel(this, DatabaseManager::instance().getDatabase());
    m_modelo->setTable("articulos");
    m_modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    m_modelo->select();
    m_tabla->setModel(m_modelo);
    
    m_tabla->setColumnHidden(0, true);  // id
    m_tabla->setColumnHidden(3, true);  // descripcion
    m_tabla->setColumnHidden(4, true);  // id_categoria
    m_tabla->setColumnHidden(5, true);  // id_marca
    m_tabla->setColumnHidden(11, true); // fecha_adquisicion
    m_tabla->setColumnHidden(12, true); // activo
    
    m_modelo->setHeaderData(1, Qt::Horizontal, "Código");
    m_modelo->setHeaderData(2, Qt::Horizontal, "Nombre");
    m_modelo->setHeaderData(6, Qt::Horizontal, "P. Compra");
    m_modelo->setHeaderData(7, Qt::Horizontal, "P. Venta");
    m_modelo->setHeaderData(8, Qt::Horizontal, "Stock Actual");
    m_modelo->setHeaderData(9, Qt::Horizontal, "Stock Mín.");
    m_modelo->setHeaderData(10, Qt::Horizontal, "Ubicación");
    
    m_tabla->resizeColumnsToContents();
}

void MainWindow::filtrarTabla(const QString &texto)
{
    if (texto.isEmpty()) {
        m_modelo->setFilter("");
    } else {
        QString filtro = QString("codigo LIKE '%%1%' OR nombre LIKE '%%1%'").arg(texto);
        m_modelo->setFilter(filtro);
    }
    m_modelo->select();
}

void MainWindow::mostrarSoloStockBajo(bool activo)
{
    if (activo) {
        m_modelo->setFilter("stock_actual <= stock_minimo");
    } else {
        m_modelo->setFilter(""); 
    }
    m_modelo->select();
}

void MainWindow::agregarArticulo()
{
    ArticuloDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.guardar()) {
            m_modelo->select();
            QMessageBox::information(this, "Éxito", "Repuesto agregado correctamente.");
        }
    }
}

void MainWindow::editarArticulo()
{
    QModelIndexList selected = m_tabla->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un artículo para editar.");
        return;
    }

    int row = selected.first().row();
    int id = m_modelo->data(m_modelo->index(row, 0)).toInt();

    ArticuloDialog dialog(this, id);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.guardar()) {
            m_modelo->select();
            QMessageBox::information(this, "Éxito", "Repuesto actualizado correctamente.");
        }
    }
}

void MainWindow::registrarMovimiento()
{
    QModelIndexList selected = m_tabla->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un artículo para registrar movimiento.");
        return;
    }

    int row = selected.first().row();
    int id = m_modelo->data(m_modelo->index(row, 0)).toInt();
    int stockActual = m_modelo->data(m_modelo->index(row, 8)).toInt();

    MovimientoDialog dialog(id, stockActual, this);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.guardar(1)) { // ID 1 simula al usuario admin
            m_modelo->select();
            QMessageBox::information(this, "Éxito", "Movimiento registrado correctamente.");
        }
    }
}

void MainWindow::eliminarArticulo()
{
    QModelIndexList selected = m_tabla->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un artículo para eliminar.");
        return;
    }

    int row = selected.first().row();
    int id = m_modelo->data(m_modelo->index(row, 0)).toInt();
    QString nombre = m_modelo->data(m_modelo->index(row, 2)).toString();

    auto reply = QMessageBox::question(this, "Confirmar", 
        QString("¿Está seguro de eliminar '%1'?").arg(nombre),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query(DatabaseManager::instance().getDatabase());
        query.prepare("UPDATE articulos SET activo = 0 WHERE id = ?");
        query.addBindValue(id);
        if (query.exec()) {
            m_modelo->select();
        } else {
            QMessageBox::critical(this, "Error", "No se pudo eliminar: " + query.lastError().text());
        }
    }
}

void MainWindow::exportarCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar Reporte CSV", 
        "inventario_motos.csv", "Archivos CSV (*.csv)");
    
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo crear el archivo.");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    out << "ID,Codigo,Nombre,Categoria,Marca,Precio Compra,Precio Venta,Stock,Stock Minimo\n";

    QSqlQuery query(DatabaseManager::instance().getDatabase());
    query.exec("SELECT a.id, a.codigo, a.nombre, c.nombre as categoria, m.nombre as marca, "
               "a.precio_compra, a.precio_venta, a.stock_actual, a.stock_minimo "
               "FROM articulos a "
               "LEFT JOIN categorias c ON a.id_categoria = c.id "
               "LEFT JOIN marcas m ON a.id_marca = m.id "
               "WHERE a.activo = 1");

    while (query.next()) {
        out << query.value(0).toString() << ","
            << "\"" << query.value(1).toString() << "\","
            << "\"" << query.value(2).toString() << "\","
            << "\"" << query.value(3).toString() << "\","
            << "\"" << query.value(4).toString() << "\","
            << query.value(5).toString() << ","
            << query.value(6).toString() << ","
            << query.value(7).toString() << ","
            << query.value(8).toString() << "\n";
    }

    file.close();
    QMessageBox::information(this, "Éxito", "Reporte exportado correctamente.");
}
