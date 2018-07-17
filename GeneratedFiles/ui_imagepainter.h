/********************************************************************************
** Form generated from reading UI file 'imagepainter.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEPAINTER_H
#define UI_IMAGEPAINTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImagePainterClass
{
public:

    void setupUi(QWidget *ImagePainterClass)
    {
        if (ImagePainterClass->objectName().isEmpty())
            ImagePainterClass->setObjectName(QStringLiteral("ImagePainterClass"));
        ImagePainterClass->resize(600, 400);

        retranslateUi(ImagePainterClass);

        QMetaObject::connectSlotsByName(ImagePainterClass);
    } // setupUi

    void retranslateUi(QWidget *ImagePainterClass)
    {
        ImagePainterClass->setWindowTitle(QApplication::translate("ImagePainterClass", "ImagePainter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImagePainterClass: public Ui_ImagePainterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEPAINTER_H
