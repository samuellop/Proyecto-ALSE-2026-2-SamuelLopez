#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QLabel label("¡Hola Mundo desde Qt! 🏍️");
    label.setWindowTitle("ALSE - Inventario Motos");
    label.resize(300, 100);
    label.show();
    
    return app.exec();
}
