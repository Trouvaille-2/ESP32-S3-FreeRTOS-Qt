#include "dialog.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog loginDlg;
    if(loginDlg.exec()==QDialog::Accepted)
    {
        MainWindow w;
        w.show();

        return a.exec();
    }

    return 0;
}
