#pragma once
#include "ui_pe_colorHelperDialog.h"
#include <QApplication>
#include <QDialog>
#include <QshowEvent>
#include <QPainter>
#include <QEvent>
#include <QCursor>
#include <QWheelEvent>
#include <QTimer>
#include <QFocusEvent>
#include <QAction>
#include <QClipboard>
#include <QFrame>
#include <QMenu>
#define  SUBCOLOR_HELPERDIALOG_XOFFSET 4
class pe_SubColorHelperDialog :public QWidget
{
public:
	pe_SubColorHelperDialog(QWidget *parent = NULL) :QWidget(parent)
	{
		HelperDialogUI.setupUi(this);
	};
	~pe_SubColorHelperDialog() {};

	void paintEvent(QPaintEvent *e)
	{
		QPainter pt(this);
		QPen pen(QColor(255, 255, 255));
		pt.setPen(pen);
		pt.fillRect(rect(), Qt::white);
		pen.setColor(m_Color);
		pt.fillRect(QRect(SUBCOLOR_HELPERDIALOG_XOFFSET, height() / 2 - 16, 32, 32), m_Color);
	}
	void SetColor(QColor clr)
	{
		m_Color = clr;
		QString RGB, HSV;
		int h, s, v, l;
		RGB.sprintf("R:%d G:%d B:%d", clr.red(), clr.green(), clr.blue());
		HelperDialogUI.label_RGB->setText(RGB);
		clr.getHsv(&h, &s, &v);
		HSV.sprintf("H:%d S:%d V:%d", h, s, v);
		HelperDialogUI.label_HSV->setText(HSV);
		update();
	}
private:
	QColor  m_Color;
	Ui_WidgetColorHelper HelperDialogUI;
};
