#include "MovimientoDialog.h"
#include "data/DatabaseManager.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

MovimientoDialog::MovimientoDialog(int idArticulo, int stockActual, QWidget *parent)
    : QDialog(parent), m_idArticulo(idArticulo), m_stockActual(stockActual)
{
    setWindowTitle("Registrar Movimiento");
    setFixedWidth(300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    QLabel *infoLabel = new QLabel(QString("Stock actual: %1").arg(stockActual), this);
    formLayout->addRow(infoLabel);

    m_tipoCombo = new QComboBox(this);
    m_tipoCombo->addItem("ENTRADA", "ENTRADA");
    m_tipoCombo->addItem("SALIDA", "SALIDA");
    formLayout->addRow("Tipo:", m_tipoCombo);

    m_cantidadSpin = new QSpinBox(this);
    m_cantidadSpin->setMinimum(1);
    m_cantidadSpin->setMaximum(999999);
    formLayout->addRow("Cantidad:", m_cantidadSpin);

    m_motivoEdit = new QLineEdit(this);
    m_motivoEdit->setPlaceholderText("Ej: Compra a proveedor, Venta...");
    formLayout->addRow("Motivo:", m_motivoEdit);

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &MovimientoDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &MovimientoDialog::reject);
    mainLayout->addWidget(buttonBox);
}

bool MovimientoDialog::guardar(int idUsuario)
{
    QString tipo = m_tipoCombo->currentData().toString();
    int cantidad = m_cantidadSpin->value();

    if (tipo == "SALIDA" && cantidad > m_stockActual) {
        QMessageBox::warning(this, "Error", "No hay suficiente stock para esta salida.");
        return false;
    }

    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    db.transaction(); // Iniciar transacción

    QSqlQuery query(db);
    
    // 1. Insertar el movimiento
    query.prepare("INSERT INTO movimientos (id_articulo, id_usuario, tipo, cantidad, motivo) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(m_idArticulo);
    query.addBindValue(idUsuario);
    query.addBindValue(tipo);
    query.addBindValue(cantidad);
    query.addBindValue(m_motivoEdit->text().trimmed());

    if (!query.exec()) {
        db.rollback();
        QMessageBox::critical(this, "Error", "Error al registrar movimiento: " + query.lastError().text());
        return false;
    }

    // 2. Actualizar el stock del artículo
    int nuevoStock = (tipo == "ENTRADA") ? (m_stockActual + cantidad) : (m_stockActual - cantidad);
    
    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE articulos SET stock_actual = ? WHERE id = ?");
    updateQuery.addBindValue(nuevoStock);
    updateQuery.addBindValue(m_idArticulo);

    if (!updateQuery.exec()) {
        db.rollback();
        QMessageBox::critical(this, "Error", "Error al actualizar stock: " + updateQuery.lastError().text());
        return false;
    }

    db.commit(); // Confirmar ambos cambios
    return true;
}
