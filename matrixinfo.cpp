#include "matrixinfo.hpp"

#include <fstream>
#define SMALL_W 200
#define INT(a) QString::number((a),10)
#define COUNT_P ((this->width() - SMALL_W) / mat_rect_size)
#define SELECT_W (COUNT_P * SMALL_W / dis_rect.width())
#define SELECT_H (COUNT_P * (SMALL_W * dis_rect.height() / dis_rect.width()) / dis_rect.width())
#define SMALL_H (SMALL_W * dis_rect.height() / dis_rect.width())
MatrixInfo::MatrixInfo(QWidget *parent) :
	QDialog(parent),
	begin_paint(false)
{
	resize(1000, 600);
}

MatrixInfo::MatrixInfo(QImage *i, QRect r, QWidget * parent) :
	QDialog(parent),
	dis_img(i),
	dis_rect(r),
	begin_paint(false),
	source_s(1.0, 1.0),
	selecting(false),
	start_col(0),
	start_row(0),
	mat_rect_size(40)

{
	resize(1000, 600);
	painter = new QPainter(this);
	this->setMaximumWidth(dis_rect.width() * 40 + SMALL_W);
	mat_date.resize(dis_rect.height());
	auto date_ptr = dis_img->bits();
	int first = dis_img->width() * dis_rect.y() - dis_img->width() + dis_rect.x();
	first *= 3;
	if (first < 0) first = 0;
	int jump = dis_img->width() - dis_rect.width();
	jump *= 3;






	int row = 0, col = 0, o = first;
	while (true)
	{
		if (col == (dis_rect.width() * 3))
		{
			row++;
			if (row == dis_rect.height()) break;
			o += jump;
			col = 0;
		}
		mat_date[row].push_back(QColor(int(*(date_ptr + o)), int(*(date_ptr + o + 1)), int(*(date_ptr + o + 2))));

		o += 3;
		col += 3;
	}



	//std::ofstream log("mat.log", std::ios::app);
	//for (auto a : mat_date)
	//{
	//	for (auto b : a)
	//	{
	//		log << b.red() << " " << b.green() << " " << b.blue() << std::endl;
	//	}
	//}

	select_rect.setRect((this->width() - SMALL_W), 0, SELECT_W, SELECT_H);


	begin_paint = true;

	update();
}

MatrixInfo::~MatrixInfo()
{
	delete painter;
}

void MatrixInfo::mouseMoveEvent(QMouseEvent *e)
{
	QPoint moving_point = e->pos();

	if (selecting)
	{
		if (moving_point.x() >= (this->width() - SMALL_W))
		{
			float last_y = select_rect.y();
			if (moving_point.x() <= (this->width() - SELECT_W))
			{
				select_rect.setRect(moving_point.x(), last_y, SELECT_W, SELECT_H);
			}
			else
			{
				select_rect.setRect(this->width() - SELECT_W, last_y, SELECT_W, SELECT_H);

			}
		}
		else
		{
			float last_y = select_rect.y();
			select_rect.setRect(this->width() - SMALL_W, last_y, SELECT_W, SELECT_H);

		}
		if (moving_point.y() <= SMALL_W)
		{
			float last_x = select_rect.x();
			if (moving_point.y() <= SMALL_H - SELECT_H)
			{
				select_rect.setRect(last_x, moving_point.y(), SELECT_W, SELECT_H);
			}
			else
			{
				select_rect.setRect(last_x, SMALL_H - SELECT_H, SELECT_W, SELECT_H);

			}
		}
		if (moving_point.y() < 0)
		{
			float last_x = select_rect.x();
			select_rect.setRect(last_x, 0, SELECT_W, SELECT_H);
		}

		update();

	}
}

void MatrixInfo::mousePressEvent(QMouseEvent *e)
{
	QPoint start_point = e->pos();
	if (start_point.x() > (this->width() - SMALL_W) && start_point.y() < SMALL_H)
	{
		select_rect.setRect(start_point.x(), start_point.y(), SELECT_W, SELECT_H);


		update();
		selecting = true;
	}
}

void MatrixInfo::mouseReleaseEvent(QMouseEvent *)
{
	if (select_rect.x() > (this->width() - SELECT_W))
	{
		select_rect.setRect((this->width() - SELECT_W), select_rect.y(), SELECT_W, SELECT_H);
	}
	if (select_rect.y() > (SMALL_H - SELECT_H))
	{
		select_rect.setRect(select_rect.x(), (SMALL_H - SELECT_H), SELECT_W, SELECT_H);
	}
	start_row = dis_rect.height() * (select_rect.y() / SMALL_H);
	start_col = dis_rect.width() * ((select_rect.x() - (this->width() - SMALL_W)) / SMALL_W);


	selecting = false;
	update();
}

void MatrixInfo::resizeEvent(QResizeEvent *)
{

	start_row = 0;
	start_col = 0;
	select_rect.setRect((this->width() - SMALL_W), 0, SELECT_W, SELECT_H);

}

void MatrixInfo::paintEvent(QPaintEvent *)
{
	//std::ofstream log("mat.log", std::ios::app);
	//log << mat_date.size() << std::endl;
	if (begin_paint)
	{

		painter->begin(this);

		source_s.setWidth(mat_rect_size);
		source_s.setHeight(mat_rect_size);

		painter->drawImage(QRect(this->width() - SMALL_W, 0, SMALL_W, SMALL_H), *dis_img, dis_rect);
		painter->setPen(QColor(255, 255, 255));


		if (!mat_date.isEmpty())
		{
			int col = start_col, row = start_row, c = 0, r = 0;
			while (row < (start_row + (COUNT_P * dis_rect.height() / dis_rect.width())))
			{

				painter->setBrush(Qt::BrushStyle::SolidPattern);
				painter->setBrush(mat_date[row][col]);
				painter->setPen(mat_date[row][col]);
				painter->drawRect(QRect(c * mat_rect_size, r * mat_rect_size, source_s.width(), source_s.height()));


				QFont font = painter->font();
				font.setPixelSize(12);
				painter->setFont(font);
				painter->setPen(QColor(255 - mat_date[row][col].red(), 255 - mat_date[row][col].green(), 255 - mat_date[row][col].blue()));
				painter->drawText(QPoint(c * mat_rect_size, r * mat_rect_size + 12), INT(mat_date[row][col].red()));
				painter->drawText(QPoint(c * mat_rect_size, r * mat_rect_size + 24), INT(mat_date[row][col].green()));
				painter->drawText(QPoint(c * mat_rect_size, r * mat_rect_size + 36), INT(mat_date[row][col].blue()));

				if (col == (start_col + COUNT_P - 1))
				{
					c = -1;
					col = start_col - 1;

					r++;
					row++;
				}
				c++;
				col++;


			}
			painter->setBrush(Qt::BrushStyle::NoBrush);
			painter->setPen(QColor(255, 255, 255));
			painter->drawRect(select_rect);

			//for (int c = 0, r = 0; r < dis_rect.height(); c++)
			//{
			//	painter->setPen(mat_date[r][c]);
			//	painter->drawRect(QRect(c * 20, r * 20, source_s.width(), source_s.height()));
			//	if (c == (dis_rect.width() - 1))
			//	{
			//		c = 0;
			//		r++;
			//	}

			//}
		}







		painter->end();
	}
}
