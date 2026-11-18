#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include "data/DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ALSE_InventarioMotos");

    if (!DatabaseManager::instance().init()) {
        QMessageBox::critical(nullptr, "Error", "No se pudo inicializar la base de datos.");
        return -1;
    }

    QLabel label("¡Base de datos conectada! 🏍️");
    label.setWindowTitle("ALSE - Inventario Motos");
    label.resize(400, 100);
    label.setAlignment(Qt::AlignCenter);
    label.show();
    
    return app.exec();
}
