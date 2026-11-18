#include <QApplication>
#include <QMessageBox>
#include "data/DatabaseManager.h"
#include "ui/LoginWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ALSE_InventarioMotos");

    if (!DatabaseManager::instance().init()) {
        QMessageBox::critical(nullptr, "Error", "No se pudo inicializar la base de datos.");
        return -1;
    }

    LoginWindow loginWin;
    
    // Cuando el login sea exitoso, mostramos un mensaje por ahora
    QObject::connect(&loginWin, &LoginWindow::loginExitoso, [&]() {
        QMessageBox::information(nullptr, "Éxito", "¡Bienvenido al sistema! (Aquí irá la ventana principal)");
        app.quit(); // Cerramos la app por ahora
    });

    loginWin.show();
    
    return app.exec();
}
