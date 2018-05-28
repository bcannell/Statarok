#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "./cprog/SmiteLog.h"
#include <windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateMapPNG();
    void updateGraph(string map_type);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_btDmgDlt_clicked();

    void on_btDmgTkn_clicked();

    void on_btHeal_clicked();

    void on_btGoldExp_clicked();

private:
    Ui::MainWindow *ui;
     QImage smitemap;
     SmiteLogInst current_match;
     string mapversion;
};

#endif // MAINWINDOW_H
