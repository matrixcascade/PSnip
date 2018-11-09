#ifndef MODIFYBUTTON_H
#define MODIFYBUTTON_H

#include <QWidget.h>
#include <QPushButton.h>
#include <QPlainTextEdit.h>
#include <QTextEdit>
#include <QLabel>
#define  MODIFYBUTTON_STATUS_DISPLAY          1
#define  MODIFYBUTTON_STATUS_MODIFY           2
class QLabelButton :public QLabel
{
	Q_OBJECT
public:
	QLabelButton(QWidget *parent = 0)
		:QLabel(parent)
	{}
	~QLabelButton(){}
	signals:
	void SIGNAL_LButtonDown();
public:
	void mousePressEvent(QMouseEvent * event) override;
};

class QModifySubEdit: public QTextEdit
{
	Q_OBJECT
public:
	QModifySubEdit(QWidget *parent=0)
		:QTextEdit(parent)
	{

	}
	~QModifySubEdit(){}

signals:
	void OnConfirm();
	void OnCancel();
protected:
	void keyPressEvent(QKeyEvent *e)
	{
		switch(e->key())
		{
		case Qt::Key_Return:
		case Qt::Key_Enter:
			emit OnConfirm();
			break;
		case Qt::Key_Escape:
			emit OnCancel();
			break;
		default:
			QTextEdit::keyPressEvent(e);
		}
	}
	void focusOutEvent(QFocusEvent *e)
	{
		emit OnConfirm();
	}
// 	void mousePressEvent(QMouseEvent *e)
// 	{
// 		emit OnConfirm();
// 		QTextEdit::mousePressEvent(e);
// 	}


};

class QModifyButton : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(quint32 m_Status READ GetStatus WRITE SetStatus)
	Q_PROPERTY(quint32 m_ID READ GetID WRITE SetID)
	Q_PROPERTY(bool    m_Enabled   READ GetEnabled   WRITE SetEnabled  )
	Q_PROPERTY(bool    m_EnabledModify READ GetEnabledModify WRITE SetEnabledModify)
	Q_PROPERTY(QString m_Text READ GetText WRITE SetText)

public:
	explicit QModifyButton(
		QWidget *parent=0,
		quint32 ID=0,
		QString ObjectName="ModifyButton",
		QString Text="Snapshot"
		);
	~QModifyButton();
	
	quint32          GetStatus() const          {return m_Status;}
	bool             GetEnabled()const          {return m_Enabled;}
	bool             GetEnabledModify()  const  {return m_EnabledModify;}
	QString          GetText()   const          {return m_Text;}
	quint32          GetID()     const          {return m_ID;}

	void	         SetID(quint32 ID)
	{
		m_ID=ID;
	}
	void SetStatus(quint32 status)
	{
		if (status==MODIFYBUTTON_STATUS_DISPLAY)
		{
			ExitModify();
		}
		if (status==MODIFYBUTTON_STATUS_MODIFY)
		{
			EnterModify();
		}
	}

	void  SetEnabled(bool val)       
	{
		m_Enabled=val;
		m_LabelButton->setEnabled(m_Enabled);
		m_TextEdit->setEnabled(m_Enabled);
	}
	
	void  SetEnabledModify(bool val) 
	{
		m_EnabledModify=val;
		if (m_EnabledModify==false)
		{
			ExitModify();
		}

	}

	void SetText(QString Text)
	{
		if(m_Status!=MODIFYBUTTON_STATUS_MODIFY)
		{
			if(Text!=m_Text)
		{
			m_Text=Text;
			m_LabelButton->setText(Text);
			m_TextEdit->setText(Text);
			emit OnValueChange(m_ID,m_Text);
		}
		}
	}

	public slots:
		void EnableModify(){SetEnabledModify(true);}
		void DisableModify(){SetEnabledModify(false);}
		void ValueChange(QString ch){SetText(ch);}
		void ButtonClicked();
		void EnterModify();
		void ExitModify();
		void CancelModify();
	signals:
		void OnValueChange(quint32,QString);
		void OnClicked(quint32);
		void OnModify(quint32,QString);
protected:
	void focusOutEvent(QFocusEvent *);
	void keyPressEvent(QKeyEvent *);
	void resizeEvent(QResizeEvent *);
	void mousePressEvent(QMouseEvent *);
private:
	QString          m_Text;
	QLabelButton	*m_LabelButton;
	QModifySubEdit	*m_TextEdit;
	quint32          m_Status;
	quint32          m_ID;
	bool             m_Enabled;
	bool             m_EnabledModify;
	

};

#endif // MODIFYBUTTON_H
