//--------------------------------------------------------------------------------------------------------
//
// ���� � �������� main ��� �������� ����������� ��������
// ��������� ������������ ������ "����" 29.11.2007�.
// � ���� ���������������� ���������� �������� QT4.x
//
//--------------------------------------------------------------------------------------------------------

#include <QApplication>
#include <QTextCodec>
#include <QLabel>
#

#include "UConfigWizard.h"

int main(int argc, char **argv)
{
	QApplication app(argc,argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
	
	UConfigWizard configWizard;
	configWizard.show();
	
	app.connect(&app,SIGNAL(lastWindowClosed()), &app,SLOT(quit()));
	return app.exec();
}
