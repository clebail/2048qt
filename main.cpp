//-----------------------------------------------------------------------------
#include <QApplication>
#include <QTextCodec>
//#include "CMainWindow.h"
#include "CResolutionDialog.h"
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    CResolutionDialog w;
    w.show();

    return a.exec();
}
//-----------------------------------------------------------------------------
