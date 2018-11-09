#include "psnip.h"
#include "Mutex_Win32.hpp"
#include <QApplication>


int main(int argc, char *argv[])
{
	if (IsAppRunning())
	{
		return 0;
	}
	QApplication a(argc, argv);
	PSnip snipRuntime;
	
	if (!snipRuntime.Initialize())
	{
		
		return -1;
	}
	if (argc>1)
	{
		snipRuntime.LoadProject(QString::fromLocal8Bit(argv[1]));
	}
	return a.exec();
}
