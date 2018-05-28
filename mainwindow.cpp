#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include <iostream>
#include <QtCharts>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMapPNG(){
    current_match.makeHeatMap(mapversion);
    QImage smitemap;
    smitemap.load("images/smitemap1.png");
    smitemap = smitemap.scaledToWidth(ui->label_2->width(), Qt::SmoothTransformation);
    ui->label_2->setPixmap(QPixmap::fromImage(smitemap));
}

void MainWindow::updateGraph(string map_type){
QSplineSeries *series = new QSplineSeries();
    vector<pair<int,int>> match_data = current_match.returnGraphData(map_type);
    int count=0;
    for(pair<int,int> mdPair : match_data){
        count++;
        if(count%40==0)
        series->append(mdPair.first, mdPair.second);
    }
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    if(map_type == "DmgDlt")
        chart->setTitle("Damage Dealt (Gods)");
    else if(map_type == "DmgTkn")
        chart->setTitle("Damage Taken");
    else if(map_type == "Heal")
        chart->setTitle("Healing Recieved");
    else if(map_type == "GoldExp")
        chart->setTitle("Gold Recieved");
    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::on_pushButton_clicked() // INITIATE STATAROCK
{
     ui->label_mstyle->setText("Everything");
     mapversion="Everything";
     updateMapPNG();
     vector<string> items = current_match.getItemsBuilt();
     for(string currItem : items){
        ui->textEdit->append( QString::fromStdString(currItem));
     }
     ui->textEdit->setReadOnly(TRUE);
}

void MainWindow::on_pushButton_2_clicked() // RESET
{
    smitemap.load("images/smitemap.png");
    smitemap = smitemap.scaledToWidth(ui->label_2->width(), Qt::SmoothTransformation);
    ui->label_2->setPixmap(QPixmap::fromImage(smitemap));
}

void MainWindow::on_pushButton_3_clicked() // SELECT MATCH
{
    QString username = getenv("USERNAME");
    QString dir = QFileDialog::getOpenFileName(this,tr("Choose"),"C:\\Users\\"+username+"\\Documents\\My Games\\Smite\\BattleGame\\Logs",tr(""));
    if(QString::compare(dir, QString())!=0){
        std::string dir_str = dir.toUtf8().constData();
        current_match.loadSmiteLog(dir_str);
        ui->label_cplayer->setText(QString::fromStdString(current_match.getCurrentPlayer()));
    }
}

void MainWindow::on_btDmgDlt_clicked()
{
    ui->label_mstyle->setText("Damage Dealt");
    mapversion = "DmgDlt";
    updateGraph(mapversion);
    updateMapPNG();
}

void MainWindow::on_btDmgTkn_clicked()
{
    ui->label_mstyle->setText("Damage Taken");
    mapversion = "DmgTkn";
    updateGraph(mapversion);
    updateMapPNG();
}

void MainWindow::on_btHeal_clicked()
{
    ui->label_mstyle->setText("Healing and Mana");
    mapversion = "Heal";
    updateGraph(mapversion);
    updateMapPNG();
}

void MainWindow::on_btGoldExp_clicked()
{
    ui->label_mstyle->setText("Gold and Expirence");
    mapversion = "GoldExp";
    updateGraph(mapversion);
    updateMapPNG();
}
