#ifndef CRESOLUTIONDIALOG_H
#define CRESOLUTIONDIALOG_H

#include <QDialog>
#include "CGenetic.h"
#include "ui_CResolutionDialog.h"

#define NB_COLONNE          4
#define NB_LIGNE            4
#define NB_GAME             (NB_COLONNE*NB_LIGNE)

class CResolutionDialog : public QDialog, private Ui::CResolutionDialog {
    Q_OBJECT
public:
    explicit CResolutionDialog(QWidget *parent = nullptr);
    ~CResolutionDialog(void);
private:
    CWGame *games[NB_GAME];
    CGenetic *genetic;
private slots:
    void on_pbGo_clicked(bool checked=false);
    void on_pbPause_clicked(bool checked=false);
};

#endif // CRESOLUTIONDIALOG_H
