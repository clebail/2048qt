//-----------------------------------------------------------------------------
#ifndef __CMAINWINDOW_H__
#define __CMAINWINDOW_H__
//-----------------------------------------------------------------------------
#include <QMainWindow>
#include "ui_CMainWindow.h"
#include "CSolveur.h"
//-----------------------------------------------------------------------------
class CMainWindow : public QMainWindow, private Ui::CMainWindow {
    Q_OBJECT
private:
    CSolveur *solveur;

private slots:
    void on_actQuitter_triggered();
    void on_actNouveauJeu_triggered();
    void on_pbIA_clicked();
    void onCoupChoisi(CDeplacement::EDirection direction);
    void onSolveurFini();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public:
    explicit CMainWindow(QWidget *parent = 0);
};
//-----------------------------------------------------------------------------
#endif // __CMAINWINDOW_H__
//-----------------------------------------------------------------------------
