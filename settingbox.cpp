#pragma once
#include <imagepainter.h>
#include <random>
SettingBox::SettingBox(const std::string& n) :
	selected(true),
	data(),
	box_color(),
	pen(),
	painting(true),
	box_brush(),
	name(n)
{
	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(0, 255);

	box_color.setRed(uniform_dist(e1));
	box_color.setBlue(uniform_dist(e1));
	box_color.setGreen(uniform_dist(e1));
	box_color.setAlpha(120);

	box_brush.setStyle(Qt::BrushStyle::SolidPattern);

	box_brush.setColor(QColor(box_color.red(), box_color.green(), box_color.blue(), 120));


	pen.setStyle(Qt::PenStyle::DashLine);
	pen.setBrush(box_color);
}

//SettingBox::SettingBox(QRectF rect) :
//	selected(true),
//	data(rect),
//	box_color(),
//	pen(),
//	painting(true),
//	box_brush()
//{
//	std::random_device r;
//	std::default_random_engine e1(r());
//	std::uniform_int_distribution<int> uniform_dist(0, 255);
//	box_color.setRed(uniform_dist(e1));
//	box_color.setBlue(uniform_dist(e1));
//	box_color.setGreen(uniform_dist(e1));
//	box_color.setAlpha(120);
//
//
//
//	pen.setColor(QColor(255, 255, 255));
//	pen.setStyle(Qt::PenStyle::DashLine);
//	pen.setBrush(box_color);
//}

SettingBox::SettingBox(QColor color,QRectF rect,std::string n) :
	selected(false),
	data(rect),
	box_color(color),
	pen(color),
	painting(true),
	box_brush(color),
	name(n)
{
	color.setAlpha(120);
	box_brush.setColor(color);
	pen.setStyle(Qt::PenStyle::SolidLine);

}

SettingBox::~SettingBox()
{

}

void SettingBox::switch_painting_mode()
{
	if (painting)
		painting = false;
	else painting = true;
}


void SettingBox::switch_seleted_mode()
{
	if (selected)
		selected = false;
	else selected = true;
}



WherePos SettingBox::where_is_pos(const QPoint &p)
{
	if ((p.x() > data.left()) && (p.x() < data.right()) &&
		 ((p.y() > data.top() && (p.y()) < data.bottom())))
		return INSIDE;

	if (p.y() <= data.top() && p.y() >= (data.top() - 3) && p.x() >= data.left() && p.x() <= data.right())
		return ONTOP;

	if ((p.y() >= data.bottom() && p.y() <= (data.bottom() + 3) && p.x() >= data.left() && p.x() <= data.right()))
		return ONBOTTOM;

	if (p.x() <= data.left() && p.x() >= data.left() - 3 && p.y() >= data.top() && p.y() <= data.bottom())
		return ONLEFT;

	if (p.x() >= data.right() && p.x() <= data.right() + 3 && p.y() >= data.top() && p.y() <= data.bottom())
		return ONRIGHT;

	return OUTSIDE;
}
