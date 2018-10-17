#include "imagepainter.h"
#include <fstream>
#define MINTHREDSHOLD 3.0
ImagePainter::ImagePainter(QWidget *parent) :
	QWidget(parent),
	source_p(0.0, 0.0),
	target_p(0.0, 0.0),
	source_s(disimg.width(), disimg.height()),
	target_s(this->width(), this->height()),
	log_zoom(1.0),
	dis_number(false),
	number_timer(this),
	axis(false),
	axis_temp(true),
	out_name("out_point"),
	out_path("./"),
	is_tab(true),
	statu(NORMAL),
	box_start_point(),
	is_connected_camera(false),
	done_time(2000),
	dis_done(false)
{

	font.setPixelSize(20);
	font.setFamily("Microsoft YaHei");
	rectangle.setRect(this->width() / 2.0 - 10.0, this->height() / 2.0 - 10.0, 500, 1000);
	this->setAcceptDrops(true);
	connect(this, &ImagePainter::timer_run, this, &ImagePainter::on_timer);
	connect(&number_timer, &QTimer::timeout, this, &ImagePainter::off_timer);

	debug();

	init_json();

	connect(&done_timer, &QTimer::timeout, [this]() {
		dis_done = false;
		done_timer.stop();
	});
}

ImagePainter::~ImagePainter()
{
	save_2_json();
}

void ImagePainter::debug()
{
	cv::Mat dismat = cv::imread("./Image__2018-08-10__11-01-16.bmp");
	display_mat(dismat);
}

void ImagePainter::display_mat(const cv::Mat &m)
{
	disimg = mat2qimage(m);
	log_zoom = 1.0;
	source_p.setX(0.0);
	source_p.setY(0.0);
	source_s = disimg.size();
	this->update();
}

void ImagePainter::clear_all_boxs()
{
	box_list.clear();
}

void ImagePainter::display_qimage(const QImage &i)
{
	disimg = i;
	if (disimg.format() != QImage::Format_RGB888)
	{
		disimg = disimg.convertToFormat(QImage::Format_RGB888);
	}
	log_zoom = 1.0;
	source_p.setX(0.0);
	source_p.setY(0.0);
	source_s = disimg.size();
	this->update();
}

void ImagePainter::set_is_connected_camera(const bool & b)
{
	is_connected_camera = b;
}

void ImagePainter::on_timer()
{
	if (is_connected_camera)	return;
	dis_number = true;
	update();
	number_timer.start(1500);
}

void ImagePainter::off_timer()
{

	if (is_connected_camera)	return;
	dis_number = false;
	number_timer.stop();
	update();
}

void ImagePainter::show_axis()
{
	if (axis)	axis = false;
	else        axis = true;
	update();
}

void ImagePainter::show_setting()
{
	ImgPainterSetting setting(out_path, out_name, is_tab, this);
	connect(&setting, &ImgPainterSetting::return_setting, this, &ImagePainter::change_setting);
	setting.exec();
}

void ImagePainter::change_setting(QString p, QString n, bool b)
{
	out_path = p;
	out_name = n;
	is_tab = b;
}

void ImagePainter::out_point()
{


	std::string file_path;
	if (is_tab)
	{
		file_path = out_path.toStdString() + out_name.toStdString() + ".txt";
	}
	else
	{
		file_path = out_path.toStdString() + out_name.toStdString() + ".csv";

	}
	std::ofstream out(file_path, std::ios::app);

	float outx = source_p.x() + (source_s.width() / 2.0); float outy = source_p.y() + (source_s.height() / 2.0);
	if (is_tab)
	{
		out << outx << "\t" << outy << std::endl;
	}
	else
	{
		out << outx << "," << outy << std::endl;
	}
}

void ImagePainter::paint_new_box(const std::string& n)
{
	setMouseTracking(false);
	statu = PAINTING_BOX;
	this->setCursor(Qt::CrossCursor);
	if (!box_list.isEmpty()) box_list.begin()->selected = false;
	for (auto iter = box_list.begin();iter != box_list.end();iter++)
	{
		if (n == (iter->name))
		{
			iter->selected = true;
			auto temp = *iter;

			box_list.erase(iter);
			box_list.push_front(temp);

			statu = SELECT_BOX;
			setMouseTracking(true);
			update();
			return;
		}
	}

	box_list.push_front(SettingBox(n));

}

void ImagePainter::switch_select_mode()
{
	if (statu == SELECT_BOX)
	{
		statu = NORMAL;
		setMouseTracking(false);
	}
	else if (statu == NORMAL || statu == UNDIS_BOX)
	{
		statu = SELECT_BOX;
		box_list.begin()->selected = true;
		setMouseTracking(true);
	}
	update();
}

void ImagePainter::stop_painting()
{
	statu = NORMAL;
	box_list.erase(box_list.begin());
	this->setCursor(Qt::ArrowCursor);
	update();
}

void ImagePainter::delete_selected_box()
{
	box_list.erase(box_list.begin());
	if (!box_list.empty())
	{
		box_list.begin()->selected = true;
	}
	else
	{
		statu = NORMAL;
	}
	update();


}

 bool ImagePainter::get_box_from_name(const std::string & n, QRectF& rec)
{
	for (auto &a : box_list)
	{
		if (n == a.name)
		{
			rec = widget_2_img(a.data);
			return true;
		}
	}
	return false;
}

 void ImagePainter::display_done_img(const cv::Mat& m)
 {
	 disimg_done = mat2qimage(m,true);
	 log_zoom = 1.0;
	 source_p.setX(0.0);
	 source_p.setY(0.0);
	 source_s = disimg.size();

	 dis_done = true;
	 this->update();
	 done_timer.start(done_time);
 }

 void ImagePainter::save_2_json()
{
	JsonParam<std::vector<json>> box_info_json("box", "box_info");
	std::vector<json> js;
	for (auto &a : box_list)
	{
		json j;
		json color;
		json rec;

		color.emplace("red", a.box_color.red());
		color.emplace("green", a.box_color.green());
		color.emplace("blue", a.box_color.blue());

		rec.emplace("x", a.data.x());
		rec.emplace("y", a.data.y());
		rec.emplace("width", a.data.width());
		rec.emplace("height", a.data.height());


		j.emplace("color",color);
		j.emplace("rect", rec);
		j.emplace("name", a.name);
		js.push_back(j);
	}
	
	box_info_json.set(js);
	box_info_json.save_2_file();
}

void ImagePainter::init_json()
{
	JsonParam<std::vector<json>> box_info_json("box", "box_info");
	std::vector<json> js = box_info_json.get();
	if (js.empty()) return;
	for (auto &a : js)
	{
		QColor c(a["color"]["red"],a["color"]["green"],a["color"]["blue"]);
		QRect r(a["rect"]["x"], a["rect"]["y"], a["rect"]["width"], a["rect"]["height"]);
		std::string s = a["name"];
		box_list.push_back(SettingBox(c,r,s));
	}
	update();
}

void ImagePainter::set_done_time(const int & t)
{
	done_time = t;
}

void ImagePainter::resizeEvent(QResizeEvent *)
{
	target_s.setWidth(this->width());
	target_s.setHeight(this->height());
}

void ImagePainter::contextMenuEvent(QContextMenuEvent *e)
{
	if (is_connected_camera) return;

	QMenu menu(this);
	setMouseTracking(false);
	if (statu == PAINTING_BOX)
	{
		QAction stop_paint(QString::fromLocal8Bit("取消"), this);

		menu.addAction(&stop_paint);
		connect(&stop_paint, &QAction::triggered, this, &ImagePainter::stop_painting);
		menu.exec(e->globalPos());
	}
	else if (statu == SELECT_BOX && box_list.begin()->selected && (box_list.begin()->where_is_pos(e->pos()) == INSIDE))
	{
		QAction delete_selected_box_menu(QString::fromLocal8Bit("删除当前选框"), this);
		menu.addAction(&delete_selected_box_menu);
		connect(&delete_selected_box_menu, &QAction::triggered, this, &ImagePainter::delete_selected_box);
		menu.exec(e->globalPos());
	}
	else
	{
		QAction setting_mode(QString::fromLocal8Bit("编辑选框"), this);
		if (log_zoom != 1.0 || box_list.isEmpty()) setting_mode.setEnabled(false);
		setting_mode.setCheckable(true);
		if (statu == SELECT_BOX) setting_mode.setChecked(true);
		else setting_mode.setChecked(false);
		menu.addAction(&setting_mode);
		connect(&setting_mode, &QAction::triggered, this, &ImagePainter::switch_select_mode);


		QAction new_box(QString::fromLocal8Bit("创建选框"), this);
		if (log_zoom != 1.0) new_box.setEnabled(false);
		menu.addAction(&new_box);
		connect(&new_box, &QAction::triggered, [this]() {paint_new_box(); });

		QAction menu_matrix(QString::fromLocal8Bit("查看矩阵信息"), this);
		menu.addAction(&menu_matrix);
		connect(&menu_matrix, &QAction::triggered, this, &ImagePainter::show_matrix);

		QAction draw_axis(QString::fromLocal8Bit("显示准心"), this);
		draw_axis.setCheckable(true);
		draw_axis.setChecked(axis);
		menu.addAction(&draw_axis);
		connect(&draw_axis, &QAction::triggered, this, &ImagePainter::show_axis);

		QAction menu_out_p(QString::fromLocal8Bit("输出中心点(P)"), this);
		menu.addAction(&menu_out_p);
		connect(&menu_out_p, &QAction::triggered, this, &ImagePainter::out_point);

		QAction menu_out(QString::fromLocal8Bit("输出设置"), this);
		menu.addAction(&menu_out);
		connect(&menu_out, &QAction::triggered, this, &ImagePainter::show_setting);
		menu.exec(e->globalPos());
	}


	if (statu == SELECT_BOX)
	{
		setMouseTracking(true);
	}

}

void ImagePainter::paintEvent(QPaintEvent *)
{
	resize_rects();

	QPainter painter;
	painter.begin(this);
	if (dis_done)
	{
		painter.drawPixmap(target, QPixmap::fromImage(disimg_done), source);
	}
	else
	{
		painter.drawPixmap(target, QPixmap::fromImage(disimg), source);
	}
	painter.setFont(font);
	painter.setPen(QColor(255, 255, 255, 198));
	if (dis_number)
	{
		if (dis_done)
		{
			painter.drawText(rectangle, 0, QString::number((disimg_done.width() / source_s.width()) * 100.0, '.', 0) + "%");
		}
		else 
		{ 
			painter.drawText(rectangle, 0, QString::number((disimg.width() / source_s.width()) * 100.0, '.', 0) + "%"); 
		}
	}
	if (axis)
	{
		if (axis_temp)
		{
			painter.setPen(QColor(255, 0, 0, 125));
			painter.drawLine(QPointF((target_s.width() / 2.0), 0), QPointF(target_s.width() / 2.0, target_s.height()));
			painter.drawLine(QPointF(0, target_s.height() / 2.0), QPointF(target_s.width(), target_s.height() / 2.0));
		}
	}



	if (statu == PAINTING_BOX || statu == SELECT_BOX || statu == NORMAL)
	{
		for (auto &boxes : box_list)
		{

			if (boxes.selected && statu != NORMAL)
			{
				painter.setBrush(boxes.box_brush);
			}
			else
			{
				painter.setBrush(Qt::BrushStyle::NoBrush);
			}
			painter.setPen(boxes.pen);
			painter.drawRect(QRect(boxes.data.x(),boxes.data.y(),boxes.data.width(),boxes.data.height()));
		}
	}
	painter.end();
}

void ImagePainter::wheelEvent(QWheelEvent *e)
{
	if (is_connected_camera)	return;

	switch (statu)
	{
	case ImagePainter::PAINTING_BOX:


		break;
	case ImagePainter::SELECT_BOX:
		break;
	case ImagePainter::UNDIS_BOX:
		if (e->delta() > 0)
		{
			zoomin(e);
		}
		else if (e->delta() < 0)
		{
			zoomout(e);
			if (log_zoom == 1.0) statu = NORMAL;

		}
		break;
	case ImagePainter::NORMAL:
		if (e->delta() > 0)
		{
			zoomin(e);
			statu = UNDIS_BOX;
		}
		else if (e->delta() < 0)
		{
			if (log_zoom <0)
				zoomout(e);

		}
		break;
	default:
		break;
	}

}

QRect ImagePainter::img_2_widget(const QRect & re)
{
	double x_power = double(disimg.size().width()) / double(this->size().width());
	double y_power = double(disimg.size().height()) / double(this->size().height());

	return QRect(double(re.x())/ x_power, double(re.y()) / y_power, double(re.width()) / x_power,double(re.height()) /  y_power);
}

QRect ImagePainter::widget_2_img(const QRect & re)
{
	double x_power = double(disimg.size().width()) / double(this->size().width());
	double y_power = double(disimg.size().height()) / double(this->size().height());

	return QRect(x_power * re.x(), y_power * re.y() , x_power * re.width(), y_power * re.height());
}

QPoint ImagePainter::img_2_widget(const QPoint & p)
{
	double x_power = double(disimg.size().width()) / double(this->size().width());
	double y_power = double(disimg.size().height()) / double(this->size().height());
	return QPoint(double(p.x()) / x_power,double(p.y()) / y_power);
}

QPoint ImagePainter::widget_2_img(const QPoint & p)
{
	double x_power = double(disimg.size().width()) / double(this->size().width());
	double y_power = double(disimg.size().height()) / double(this->size().height());
	return QPoint(x_power * p.x() , y_power * p.y());
}

void ImagePainter::mousePressEvent(QMouseEvent *e)
{
	if (is_connected_camera)	return;

	if (e->button() == Qt::LeftButton)
	{
		QRect save_rect;
		switch (statu)
		{
		case ImagePainter::PAINTING_BOX:

			box_start_point = e->pos();
			box_end_point = e->pos();
			save_rect = QRect(box_start_point.x(), box_start_point.y(), 0, 0);
			box_list.begin()->data.setRect(save_rect.x(),save_rect.y(),save_rect.width(),save_rect.height());
			update();

			break;
		case ImagePainter::SELECT_BOX:
			if (!box_list.empty())
			{
				switch (box_list.begin()->where_is_pos(e->pos()))
				{
				case ONTOP:
					box_statu.first = true;
					box_statu.second = ONTOP;
					break;
				case ONLEFT:
					box_statu.first = true;
					box_statu.second = ONLEFT;
					break;
				case ONBOTTOM:
					box_statu.first = true;
					box_statu.second = ONBOTTOM;
					break;
				case ONRIGHT:
					box_statu.first = true;
					box_statu.second = ONRIGHT;
					break;
				case INSIDE:
					inside_move_start_point = e->pos();
					box_statu.first = true;
					box_statu.second = INSIDE;
					break;
				case OUTSIDE:
					break;
				default:
					break;
				}
			}
			break;
		case ImagePainter::UNDIS_BOX:
			if (e->button() == Qt::LeftButton)
			{
				start_point = e->pos();
				this->setCursor(Qt::ClosedHandCursor);
			}
			break;
		default:
			break;
		}
	}
}

void ImagePainter::mouseMoveEvent(QMouseEvent *e)
{
	if (is_connected_camera)	return;
	QRectF save_rect;
	switch (statu)
	{
	case ImagePainter::PAINTING_BOX:
		box_end_point = e->pos();
		save_rect = QRect(box_start_point.x(), box_start_point.y(), box_end_point.x() - box_start_point.x(), box_end_point.y() - box_start_point.y());
		box_list.begin()->data.setRect(save_rect.x(), save_rect.y(), save_rect.width(), save_rect.height());
		box_list.begin()->data = box_list.begin()->data.normalized();
		update();
		break;
	case ImagePainter::SELECT_BOX:
		if (!box_list.isEmpty())
		{
			switch (box_list.begin()->where_is_pos(e->pos()))
			{
			case ONTOP:
				this->setCursor(Qt::SizeVerCursor);
				break;
			case ONLEFT:
				this->setCursor(Qt::SizeHorCursor);
				break;
			case ONRIGHT:
				this->setCursor(Qt::SizeHorCursor);
				break;
			case ONBOTTOM:
				this->setCursor(Qt::SizeVerCursor);
				break;
			case OUTSIDE:
				this->setCursor(Qt::ArrowCursor);
				break;
			case INSIDE:
				this->setCursor(Qt::SizeAllCursor);
				break;
			default:
				break;
			}
		}
		if (box_statu.first)
		{
			switch (box_statu.second)
			{
			case ONTOP:
				if (box_list.begin()->data.bottom() - (e->pos().y()) >= MINTHREDSHOLD)
				{
					box_list.begin()->data.setTop(e->pos().y());
				}
				else
				{
					box_statu.first = false;
				}
				update();
				break;
			case ONLEFT:
				if (box_list.begin()->data.right() - (e->pos().x()) >= MINTHREDSHOLD)
				{
					box_list.begin()->data.setLeft(e->pos().x());
				}
				else
				{
					box_statu.first = false;
				}
				update();
				break;
			case ONBOTTOM:
				if ((e->pos().y() - box_list.begin()->data.top()) >= MINTHREDSHOLD)
				{
					box_list.begin()->data.setBottom(e->pos().y());
				}
				else
				{
					box_statu.first = false;
				}
				update();
				break;
			case ONRIGHT:
				if ((e->pos().x() - box_list.begin()->data.left()) >= MINTHREDSHOLD)
				{
					box_list.begin()->data.setRight(e->pos().x());
				}
				else
				{
					box_statu.first = false;
				}
				update();
				break;
			case INSIDE:
				box_move_vector = (e->pos() - inside_move_start_point);
				inside_move_start_point = e->pos();
				if ((box_list.begin()->data.x() + box_move_vector.x()) >= 0 && ((box_list.begin()->data.right() + box_move_vector.x()) <= width()))
				{
					box_list.begin()->data.setLeft(box_list.begin()->data.left() + box_move_vector.x());
					box_list.begin()->data.setRight(box_list.begin()->data.right() + box_move_vector.x());
				}
				if ((box_list.begin()->data.y() + box_move_vector.y()) >= 0 && ((box_list.begin()->data.bottom() + box_move_vector.y()) <= height()))
				{
					box_list.begin()->data.setTop(box_list.begin()->data.top() + box_move_vector.y());
					box_list.begin()->data.setBottom(box_list.begin()->data.bottom() + box_move_vector.y());
				}
				update();
				break;
			case OUTSIDE:
				break;
			default:
				break;
			}
		}
		break;
	case ImagePainter::UNDIS_BOX:
		if (e->buttons() == Qt::LeftButton)
		{
			auto m = e->pos();
			auto vec = m - start_point;
			//vec *= log_zoom;
			vec.setX(vec.x() * (source_s.width() / target_s.width()));
			vec.setY(vec.y() * (source_s.height() / target_s.height()));

			if (source_p.x() - vec.x() <= (disimg.width() - source_s.width()) && (source_p.x() - vec.x() >= 0.0))
			{
				source_p.setX(source_p.x() - vec.x());
			}
			if (source_p.y() - vec.y() <= (disimg.height() - source_s.height()) && (source_p.y() - vec.y() >= 0.0))
			{
				source_p.setY(source_p.y() - vec.y());
			}
			start_point = m;
			update();
		}
		break;
	default:
		break;
	}

}

void ImagePainter::mouseReleaseEvent(QMouseEvent *e)
{
	this->setCursor(Qt::ArrowCursor);
	if (e->button() == Qt::LeftButton)
	{
		if (statu == PAINTING_BOX)
		{
			box_list.begin()->selected = true;
			box_list.begin()->pen.setStyle(Qt::PenStyle::SolidLine);
			box_list.begin()->box_brush = QBrush(box_list.begin()->box_color);
			statu = SELECT_BOX;
			setMouseTracking(true);
		}
		else if (statu == SELECT_BOX)
		{
			if (box_statu.first)
			{
				box_statu.first = false;
			}
			else {
				if (e->button() == Qt::MouseButton::LeftButton)
				{
					for (auto iter = box_list.begin(); iter != box_list.end(); iter++)
					{
						if (iter->where_is_pos(e->pos()) == INSIDE)
						{
							auto tmp = *iter;
							box_list.begin()->selected = false;
							tmp.selected = true;
							box_list.erase(iter);
							box_list.push_front(tmp);
							break;
						}
					}
				}
			}
		}
	}
	update();
}

void ImagePainter::keyPressEvent(QKeyEvent *e)
{
	if (is_connected_camera)	return;

	if (axis)
	{
		if (e->key() == Qt::Key_M)
		{
			axis_temp = false;
			update();
		}
	}

	if (e->key() == Qt::Key_P)
	{
		out_point();
	}
}

void ImagePainter::keyReleaseEvent(QKeyEvent *e)
{
	if (is_connected_camera)	return;

	if (axis)
	{
		if (e->key() == Qt::Key_M)
		{
			axis_temp = true;
			update();
		}
	}
}

void ImagePainter::resize_rects()
{
	target.setRect(target_p.x(), target_p.y(), target_s.width(), target_s.height());
	source.setRect(source_p.x(), source_p.y(), source_s.width(), source_s.height());
}

void ImagePainter::zoomin(QWheelEvent *e)
{
	if (is_connected_camera)	return;

	auto m = e->posF();

	float vx = source_s.width() / target_s.width();
	float vy = source_s.height() / target_s.height();

	m.setX(m.x() * vx); m.setY(m.y() * vy);
	m *= 0.05;
	source_p += m;
	source_s *= 0.95;
	log_zoom *= 0.95;
	emit timer_run();
	update();
}

void ImagePainter::zoomout(QWheelEvent *e)
{
	if (is_connected_camera)	return;

	auto m = e->posF();

	float vx = source_s.width() / target_s.width();
	float vy = source_s.height() / target_s.height();

	m.setX(m.x() * vx); m.setY(m.y() * vy);
	m *= 0.05;


	source_p -= m;
	//log_vec.push_back(m);

	if (log_zoom < 1.0)
	{
		source_s /= 0.95;
		log_zoom /= 0.95;
	}
	if (log_zoom >= 1.0)
	{
		source_p.setX(0);
		source_p.setY(0);
	}
	emit timer_run();
	update();
}

QImage ImagePainter::mat2qimage(const cv::Mat &m,bool flag)
{
	if (flag)
	{
		switch (m.channels())
		{
		case 1:
			cv::cvtColor(m, rgb_done, CV_GRAY2RGB);
			break;
		case 3:
			cv::cvtColor(m, rgb_done, CV_BGR2RGB);
			break;
		case 4:
			cv::cvtColor(m, rgb_done, CV_BGRA2RGB);
			break;
		default:
			return QImage();
		}
		return QImage(rgb_done.data, rgb_done.cols, rgb_done.rows, rgb_done.step, QImage::Format::Format_RGB888);
	}
	else{
		switch (m.channels())
		{
		case 1:
			cv::cvtColor(m, rgb, CV_GRAY2RGB);
			break;
		case 3:
			cv::cvtColor(m, rgb, CV_BGR2RGB);
			break;
		case 4:
			cv::cvtColor(m, rgb, CV_BGRA2RGB);
			break;
		default:
			return QImage();
		}
		return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format::Format_RGB888);
	}

}

ImagePainter::Statu ImagePainter::get_statu()
{
	return statu;
}

void ImagePainter::show_matrix()
{
	if (source_s.width() < 20)
	{
		QMessageBox::warning(this, "WARRING", QString::fromLocal8Bit("倍数过大，请调整", 1024));
		return;
	}
	MatrixInfo info(&disimg, QRect(QPoint(source_p.x(), source_p.y()), QSize(source_s.width(), source_s.height())), this);
	info.exec();
}
