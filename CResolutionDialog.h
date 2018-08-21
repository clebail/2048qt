#ifndef CRESOLUTIONDIALOG_H
#define CRESOLUTIONDIALOG_H

#include <QDialog>
#include "CGenetic.h"
#include "ui_CResolutionDialog.h"

#define NB_GAME             16

class CResolutionDialog : public QDialog, private Ui::CResolutionDialog {
    Q_OBJECT
public:
    explicit CResolutionDialog(QWidget *parent = 0);
private:
    CWGame *games[NB_GAME];
    CGenetic *genetic;
private slots:
    void on_pbGo_clicked(bool checked=false);
};

#endif // CRESOLUTIONDIALOG_H
