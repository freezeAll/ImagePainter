#pragma once
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

#include "GeneratedFiles/ui_imgpaintersetting.h"
class ImgPainterSetting : public QDialog
{
	Q_OBJECT

public:
	explicit ImgPainterSetting(QWidget *parent = 0);
	explicit ImgPainterSetting(QString&, QString&, bool, QWidget *parent = 0);
	~ImgPainterSetting();
	public slots:

	void p_ok_button();
signals:
	void return_setting(QString, QString, bool);
private:
	Ui::ImgPainterSetting ui;
};