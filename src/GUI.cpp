// main.cpp
#include <qt6/QtWidgets/QApplication>
#include <qt6/QtWidgets/QPushButton>
#include <qt6/QtWidgets/QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the main window
    QWidget window;
    window.setWindowTitle("Qt6 Simple Window");
    window.setGeometry(100, 100, 300, 200); // x, y, width, height

    // Create a button
    QPushButton button("Click Me", &window);
    button.setGeometry(100, 80, 100, 30); // x, y, width, height

    // Show the window
    window.show();

    // Execute the application
    return app.exec();
}