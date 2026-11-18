#include <QApplication>
#include <QMessageBox>
#include "data/DatabaseManager.h"
#include "ui/LoginWindow.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ALSE_InventarioMotos");

    if (!DatabaseManager::instance().init()) {
        QMessageBox::critical(nullptr, "Error", "No se pudo inicializar la base de datos.");
        return -1;
    }

    // 1. Creamos ambas ventanas
    LoginWindow loginWin;
    MainWindow mainWin;

    // 2. Conectamos la señal de login exitoso
    QObject::connect(&loginWin, &LoginWindow::loginExitoso, [&]() {
        mainWin.show(); // Mostramos la principal
        loginWin.hide(); // Ocultamos el login (no lo cerramos para que no muera)
    });

    // 3. Mostramos el login al arrancar
    loginWin.show();
    
    return app.exec();
}
