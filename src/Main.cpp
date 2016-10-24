#include <QApplication>

#include "includes/MainWindow.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow *window{ new MainWindow(nullptr) };
	window->show();

	return app.exec();
}