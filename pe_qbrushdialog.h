#ifndef PE_QBRUSHDIALOG_H
#define PE_QBRUSHDIALOG_H

#include <QDialog>
#include <QPainter>
#include "ui_pe_Brush.h"

class pe_qBrushDialog : public QDialog
{
	Q_OBJECT

public:
	pe_qBrushDialog(QWidget *parent);
	~pe_qBrushDialog();
	void paintEvent(QPaintEvent *event)					override;
	QColor GetColor();
	float GetLineWidth();
public slots:
	void BrushUpdate();
private:
	QColor color;
	float w;
	Ui_Form_Brush brushui;
};

#endif // PE_QBRUSHDIALOG_H
