#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include"widget.h"
#include<QString>
#include<QMouseEvent>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include"attach.h"
#include"mainwindow3.h"
#include"desktopbk.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);//消除边框
    setFixedSize(width(),height());//固定大小
    mExitMenu=new QMenu(this);
    mExitAct=new QAction();
    mExitAct->setText("退出");
    mExitAct->setIcon(QIcon("C:\\Users\\LENOVO\\Desktop\\func\\close.png"));
    mExitMenu->addAction(mExitAct);     //右键退出
    connect(mExitAct,&QAction::triggered,this,[=]{
        qApp->exit(0);
    });
    mNetAccessManager=new QNetworkAccessManager(this);
    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&MainWindow::onReplied);

    getWeatherInfo("101010100");//请求天气数据

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    mExitMenu->exec(QCursor::pos());
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    bias=event->globalPos()-this->pos();

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-bias);
}

void MainWindow::getWeatherInfo(QString citycode)
{
    QUrl url("https://api.seniverse.com/v3/weather/now.json?key=S1lY6DHhdSxqo396N&location=beijing&language=zh-Hans&unit=c");
    mNetAccessManager->get(QNetworkRequest(url));
}

void MainWindow::parsejson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(byteArray,&err);
    if(err.error!=QJsonParseError::NoError)return;
    QJsonObject obj=doc.object();
    //mnow.city=root.value("location").toObject().value("name").toString();//value("results").toObject().
    //qDebug()<<mnow.city;
    // 访问results数组
    if (obj.contains("results") && obj["results"].isArray()) {
        QJsonArray resultsArray = obj["results"].toArray();
        for (const QJsonValue &result : resultsArray) {
            if (result.isObject()) {
                QJsonObject resultObj = result.toObject();
                // 访问location对象
                if (resultObj.contains("location") && resultObj["location"].isObject()) {
                    QJsonObject locationObj = resultObj["location"].toObject();
                    QString id = locationObj["id"].toString();
                    QString name = locationObj["name"].toString();
                    mnow.city=name;
                    QString country = locationObj["country"].toString();
                    // ...其他数据
                }
                // 访问now对象
                if (resultObj.contains("now") && resultObj["now"].isObject()) {
                    QJsonObject nowObj = resultObj["now"].toObject();
                    QString text = nowObj["text"].toString();
                    mnow.weatherstate=text;
                    QString code = nowObj["code"].toString();
                    mnow.code=code;//.toInt();
                    QString temperature = nowObj["temperature"].toString();
                    mnow.temperature=temperature;//.toInt();
                    //qDebug()<<temperature;
                    // ...其他数据
                }
                // 访问last_update
                QString lastUpdate = resultObj["last_update"].toString();
                //qDebug()<<lastUpdate;
            }
        }
    }
    //qDebug()<<mnow.city<<" "<<mnow.code<<" "<<mnow.temperature<<" "<<mnow.weatherstate;
    //更新UI
    updateUI();
}

void MainWindow::updateUI()
{

    QFont font1 = ui->label_temperature->font();
    font1.setPointSize(24); // 设置字体大小
    QFont font2 = ui->label_weatherstate->font();
    font2.setPointSize(24); // 设置字体大小


    ui->label_temperature->setFont(font1);
    ui->label_temperature->setText("温度/摄氏度:\n"+mnow.temperature);

    ui->label_city->setText(mnow.city);
    ui->label_weatherstate->setFont(font2);
    ui->label_weatherstate->setText(mnow.weatherstate);


    int code=mnow.code.toInt();
    QPixmap pixmap(QString(":/C:/Users/LENOVO/Desktop/white/%1@1x.png").arg(code));
    ui->label_code->setPixmap(pixmap);
    ui->label_code->show();
}

void MainWindow::onReplied(QNetworkReply *reply)
{
    QByteArray byteArray=reply->readAll();
    //qDebug()<<"read all:"<<byteArray.data();
    parsejson(byteArray);

    reply->deleteLater();

}

void MainWindow::on_pushButton_clicked()
{
    attach* att=new attach;
    att->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    desktopbk n;
    n.setsedesktopimage();
}

