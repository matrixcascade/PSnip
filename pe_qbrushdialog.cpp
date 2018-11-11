#include "pe_qbrushdialog.h"

pe_qBrushDialog::pe_qBrushDialog(QWidget *parent)
	: QDialog(parent)
{
	brushui.setupUi(this);
	connect(brushui.spinBox_R,SIGNAL(valueChanged(int)),this,SLOT(BrushUpdate()));
	connect(brushui.spinBox_G,SIGNAL(valueChanged(int)),this,SLOT(BrushUpdate()));
	connect(brushui.spinBox_B,SIGNAL(valueChanged(int)),this,SLOT(BrushUpdate()));
	connect(brushui.horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(BrushUpdate()));
}

pe_qBrushDialog::~pe_qBrushDialog()
{

}

void pe_qBrushDialog::paintEvent(QPaintEvent *event)
{
	QPainter pt(this);
	color.setRgb(brushui.spinBox_R->value(),brushui.spinBox_G->value(),brushui.spinBox_B->value());
	QPen bPen(color);
	bPen.setWidth(w=(brushui.horizontalSlider->value()/10.0));

	pt.setPen(bPen);

	pt.drawLine(5,90,202,90);
}

QColor pe_qBrushDialog::GetColor()
{
	return color;
}

float pe_qBrushDialog::GetLineWidth()
{
	return w;
}

void pe_qBrushDialog::BrushUpdate()
{
	this->update();
}
