#ifndef ARTICULODIALOG_H
#define ARTICULODIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>

class ArticuloDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArticuloDialog(QWidget *parent = nullptr, int idArticulo = -1);
    bool guardar();

private:
    void cargarCombos();
    void cargarDatos(int id);

    int m_id;
    QLineEdit *m_codigoEdit;
    QLineEdit *m_nombreEdit;
    QLineEdit *m_ubicacionEdit;
    QComboBox *m_categoriaCombo;
    QComboBox *m_marcaCombo;
    QDoubleSpinBox *m_precioCompraSpin;
    QDoubleSpinBox *m_precioVentaSpin;
    QSpinBox *m_stockSpin;
    QSpinBox *m_stockMinSpin;
};

#endif // ARTICULODIALOG_H
