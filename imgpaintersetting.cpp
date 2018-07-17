#include "imgpaintersetting.h"

ImgPainterSetting::ImgPainterSetting(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);
}



ImgPainterSetting::ImgPainterSetting(QString &path, QString &name, bool b, QWidget * parent) :
	QDialog(parent)
{
	ui.setupUi(this);
	ui.lineEdit->setText(path);
	ui.lineEdit_2->setText(name);
	if (b)
	{
		ui.radioButton->setChecked(true);
	}
	else
	{
		ui.radioButton_2->setChecked(true);
	}
	connect(this, &QDialog::accepted, this, &ImgPainterSetting::p_ok_button);

}

ImgPainterSetting::~ImgPainterSetting()
{
}

void ImgPainterSetting::p_ok_button()
{
	emit return_setting(ui.lineEdit->text(), ui.lineEdit_2->text(), ui.radioButton->isChecked());
}
