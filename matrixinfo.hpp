#pragma once
#include <QObject>
#include <QPainter>
#include "opencv2\opencv.hpp"

#include <QImage>
#include <QWheelEvent>
#include <QMessageBox>
#include <QMenu>
#include <QTimer>
#include <QDialog>
#include <QDialog>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QMimeData>
#include <QLinkedList>

class MatrixInfo : public QDialog
{
	Q_OBJECT

public:
	explicit MatrixInfo(QWidget *parent = 0);
	explicit MatrixInfo(QImage*, QRect, QWidget *parent = 0);
	~MatrixInfo();
protected:
	void mouseMoveEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
private:
	void resizeEvent(QResizeEvent*);
	void paintEvent(QPaintEvent*);

	QPainter* painter;


	QImage* dis_img;
	QRect dis_rect;
	bool begin_paint;

	QVector<QVector<QColor>> mat_date;

	QSize source_s;

	int start_row;
	int start_col;

	bool selecting;

	QRectF select_rect;

	int mat_rect_size;
};