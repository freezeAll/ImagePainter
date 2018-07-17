#include "imagepainter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ImagePainter w;
	w.show();
	return a.exec();
}
