#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H

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
#include "../wintool/config_json_template.h"

#include "imgpaintersetting.h"
#include <QException> 
#include "matrixinfo.hpp"
enum WherePos
{
	ONTOP,
	ONLEFT,
	ONBOTTOM,
	ONRIGHT,
	INSIDE,
	OUTSIDE
};
typedef std::pair<bool, WherePos> SelectBoxStatu;


class SettingBox
{
public:
	SettingBox(const std::string& = "");
	//SettingBox(QRectF);
	SettingBox(QColor, QRectF, std::string);
	~SettingBox();
	void switch_painting_mode();
	void switch_seleted_mode();
	WherePos where_is_pos(const QPoint &);

	std::string name;

	bool selected;
	bool painting;
	QRectF data;
	QPen pen;
	QColor box_color;
	QBrush box_brush;
};

class ImagePainter : public QWidget
{
	Q_OBJECT

public:
	explicit ImagePainter(QWidget *parent = 0);
	~ImagePainter();
	void debug();
	void display_mat(const cv::Mat&);
	void display_qimage(const QImage&);
	void set_is_connected_camera(const bool&);
	enum Statu
	{
		PAINTING_BOX,
		SELECT_BOX,
		UNDIS_BOX,
		NORMAL
	};
signals:
	void timer_run();
public slots:
	void show_matrix();
	void on_timer();
	void off_timer();
	void show_axis();
	void show_setting();
	void change_setting(QString, QString, bool);
	void out_point();
	void paint_new_box(const std::string& = "");
	void switch_select_mode();
	void stop_painting();
	void delete_selected_box();
	bool get_box_from_name(const std::string&, QRectF&);

	


	void save_2_json();
	void init_json();
protected:
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);

private:

	Statu statu;


	QLinkedList<SettingBox> box_list;
	QPointF box_start_point;
	QPointF box_end_point;


	QPainter* painter;
	void resizeEvent(QResizeEvent*);
	void contextMenuEvent(QContextMenuEvent*);



	void paintEvent(QPaintEvent*);
	void wheelEvent(QWheelEvent*);


	QRect img_2_widget(const QRect&);
	QRect widget_2_img(const QRect&);

	QPoint img_2_widget(const QPoint&);
	QPoint widget_2_img(const QPoint&);

	void resize_rects();

	void zoomin(QWheelEvent *);
	void zoomout(QWheelEvent *);

	QImage  mat2qimage(const cv::Mat&);

	//QPixmap mat2qpixmap(const cv::Mat&);
	//QPainter* painter;
	//cv::Mat dismat;
	cv::Mat rgb;
	QImage disimg;
	QPixmap dispm;
	QRectF target;
	QRectF source;
	QPointF target_p;
	QPointF source_p;
	QSizeF target_s;
	QSizeF source_s;

	QVector<QPointF> log_vec;

	SelectBoxStatu box_statu;

	QPointF inside_move_start_point;
	QPointF start_point;
	QPointF box_move_vector;

	float log_zoom;

	bool dis_number;
	QTimer number_timer;

	QFont font;

	QRect rectangle;

	bool axis;
	bool axis_temp;

	QString out_path;
	QString out_name;

	bool is_tab;

	bool is_connected_camera;
public:
	Statu get_statu();
};

#endif // IMAGEPAINTER_H
