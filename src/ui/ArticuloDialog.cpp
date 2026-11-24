#include "ArticuloDialog.h"
#include "data/DatabaseManager.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVBoxLayout>

ArticuloDialog::ArticuloDialog(QWidget *parent, int idArticulo) : QDialog(parent), m_id(idArticulo)
{
    setWindowTitle(m_id == -1 ? "Agregar Repuesto" : "Editar Repuesto");
    setFixedWidth(350);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    m_codigoEdit = new QLineEdit(this);
    m_nombreEdit = new QLineEdit(this);
    m_ubicacionEdit = new QLineEdit(this);
    
    m_categoriaCombo = new QComboBox(this);
    m_marcaCombo = new QComboBox(this);
    
    m_precioCompraSpin = new QDoubleSpinBox(this);
    m_precioCompraSpin->setMaximum(9999999);
    m_precioCompraSpin->setPrefix("$ ");
    
    m_precioVentaSpin = new QDoubleSpinBox(this);
    m_precioVentaSpin->setMaximum(9999999);
    m_precioVentaSpin->setPrefix("$ ");
    
    m_stockSpin = new QSpinBox(this);
    m_stockSpin->setMaximum(999999);
    
    m_stockMinSpin = new QSpinBox(this);
    m_stockMinSpin->setMaximum(999999);

    formLayout->addRow("Código:", m_codigoEdit);
    formLayout->addRow("Nombre:", m_nombreEdit);
    formLayout->addRow("Categoría:", m_categoriaCombo);
    formLayout->addRow("Marca:", m_marcaCombo);
    formLayout->addRow("P. Compra:", m_precioCompraSpin);
    formLayout->addRow("P. Venta:", m_precioVentaSpin);
    formLayout->addRow("Stock Actual:", m_stockSpin);
    formLayout->addRow("Stock Mínimo:", m_stockMinSpin);
    formLayout->addRow("Ubicación:", m_ubicacionEdit);

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ArticuloDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ArticuloDialog::reject);
    mainLayout->addWidget(buttonBox);

    cargarCombos();
    if (m_id != -1) {
        cargarDatos(m_id);
    }
}

void ArticuloDialog::cargarCombos()
{
    QSqlQuery query(DatabaseManager::instance().getDatabase());
    
    query.exec("SELECT id, nombre FROM categorias ORDER BY nombre");
    while (query.next()) {
        m_categoriaCombo->addItem(query.value(1).toString(), query.value(0).toInt());
    }

    query.exec("SELECT id, nombre FROM marcas ORDER BY nombre");
    while (query.next()) {
        m_marcaCombo->addItem(query.value(1).toString(), query.value(0).toInt());
    }
}

void ArticuloDialog::cargarDatos(int id)
{
    QSqlQuery query(DatabaseManager::instance().getDatabase());
    query.prepare("SELECT codigo, nombre, id_categoria, id_marca, precio_compra, precio_venta, stock_actual, stock_minimo, ubicacion FROM articulos WHERE id = ?");
    query.addBindValue(id);
    
    if (query.exec() && query.next()) {
        m_codigoEdit->setText(query.value(0).toString());
        m_nombreEdit->setText(query.value(1).toString());
        m_categoriaCombo->setCurrentIndex(m_categoriaCombo->findData(query.value(2).toInt()));
        m_marcaCombo->setCurrentIndex(m_marcaCombo->findData(query.value(3).toInt()));
        m_precioCompraSpin->setValue(query.value(4).toDouble());
        m_precioVentaSpin->setValue(query.value(5).toDouble());
        m_stockSpin->setValue(query.value(6).toInt());
        m_stockMinSpin->setValue(query.value(7).toInt());
        m_ubicacionEdit->setText(query.value(8).toString());
    }
}

bool ArticuloDialog::guardar()
{
    if (m_codigoEdit->text().trimmed().isEmpty() || m_nombreEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Aviso", "El código y el nombre son obligatorios.");
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().getDatabase());
    
    if (m_id == -1) {
        // INSERT
        query.prepare("INSERT INTO articulos (codigo, nombre, id_categoria, id_marca, precio_compra, precio_venta, stock_actual, stock_minimo, ubicacion) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    } else {
        // UPDATE
        query.prepare("UPDATE articulos SET codigo=?, nombre=?, id_categoria=?, id_marca=?, precio_compra=?, precio_venta=?, stock_actual=?, stock_minimo=?, ubicacion=? WHERE id=?");
    }

    query.addBindValue(m_codigoEdit->text().trimmed());
    query.addBindValue(m_nombreEdit->text().trimmed());
    query.addBindValue(m_categoriaCombo->currentData().toInt());
    query.addBindValue(m_marcaCombo->currentData().toInt());
    query.addBindValue(m_precioCompraSpin->value());
    query.addBindValue(m_precioVentaSpin->value());
    query.addBindValue(m_stockSpin->value());
    query.addBindValue(m_stockMinSpin->value());
    query.addBindValue(m_ubicacionEdit->text().trimmed());
    
    if (m_id != -1) {
        query.addBindValue(m_id);
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "No se pudo guardar: " + query.lastError().text());
        return false;
    }
    return true;
}
