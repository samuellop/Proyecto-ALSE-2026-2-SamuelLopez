#ifndef MOVIMIENTODIALOG_H
#define MOVIMIENTODIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>

class MovimientoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MovimientoDialog(int idArticulo, int stockActual, QWidget *parent = nullptr);
    bool guardar(int idUsuario);

private:
    int m_idArticulo;
    int m_stockActual;
    QComboBox *m_tipoCombo;
    QSpinBox *m_cantidadSpin;
    QLineEdit *m_motivoEdit;
};

#endif // MOVIMIENTODIALOG_H
