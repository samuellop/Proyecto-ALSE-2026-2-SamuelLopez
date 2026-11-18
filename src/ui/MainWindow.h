#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

class QTableView;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void filtrarTabla(const QString &texto);
    void mostrarSoloStockBajo(bool activo);
    void agregarArticulo();
    void editarArticulo();
    void eliminarArticulo();
    void exportarCSV();

private:
    void configurarUI();
    void configurarModelo();
    void actualizarCategorias();

    QTableView *m_tabla;
    QSqlTableModel *m_modelo;
    QLineEdit *m_buscarEdit;
    QPushButton *m_btnAgregar;
    QPushButton *m_btnEditar;
    QPushButton *m_btnEliminar;
    QPushButton *m_btnExportar;
};

#endif // MAINWINDOW_H
