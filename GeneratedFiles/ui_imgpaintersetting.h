/********************************************************************************
** Form generated from reading UI file 'imgpaintersetting.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMGPAINTERSETTING_H
#define UI_IMGPAINTERSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ImgPainterSetting
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;

    void setupUi(QDialog *ImgPainterSetting)
    {
        if (ImgPainterSetting->objectName().isEmpty())
            ImgPainterSetting->setObjectName(QString::fromUtf8("ImgPainterSetting"));
        ImgPainterSetting->resize(353, 140);
        buttonBox = new QDialogButtonBox(ImgPainterSetting);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(0, 100, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(ImgPainterSetting);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 72, 15));
        label_2 = new QLabel(ImgPainterSetting);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 81, 20));
        lineEdit = new QLineEdit(ImgPainterSetting);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(100, 10, 241, 21));
        lineEdit_2 = new QLineEdit(ImgPainterSetting);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(100, 40, 241, 21));
        radioButton = new QRadioButton(ImgPainterSetting);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(20, 80, 115, 19));
        radioButton->setChecked(true);
        radioButton_2 = new QRadioButton(ImgPainterSetting);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(150, 80, 115, 19));

        retranslateUi(ImgPainterSetting);
        QObject::connect(buttonBox, SIGNAL(rejected()), ImgPainterSetting, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), ImgPainterSetting, SLOT(accept()));

        QMetaObject::connectSlotsByName(ImgPainterSetting);
    } // setupUi

    void retranslateUi(QDialog *ImgPainterSetting)
    {
        ImgPainterSetting->setWindowTitle(QApplication::translate("ImgPainterSetting", "Dialog", nullptr));
        label->setText(QApplication::translate("ImgPainterSetting", "\350\276\223\345\207\272\350\267\257\345\276\204\357\274\232", nullptr));
        label_2->setText(QApplication::translate("ImgPainterSetting", "\350\276\223\345\207\272\346\226\207\344\273\266\345\220\215\357\274\232", nullptr));
        radioButton->setText(QApplication::translate("ImgPainterSetting", "\345\210\266\350\241\250\347\254\246\345\210\206\351\232\224", nullptr));
        radioButton_2->setText(QApplication::translate("ImgPainterSetting", "\351\200\227\345\217\267\345\210\206\351\232\224", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImgPainterSetting: public Ui_ImgPainterSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMGPAINTERSETTING_H
