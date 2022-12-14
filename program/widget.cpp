#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QImage>
#include <QLabel>
#include <QMovie>

QImage pic,bottom;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    bottom.load(":/pic/bottom.png");
    QMovie *startUp = new QMovie(":/pic/bottom.gif");
    this->setAttribute(Qt::WA_TranslucentBackground);       //设置背景透明
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);//设置无边框
    this->setGeometry(rectWin);

    QLabel *label = new QLabel(this);
    startUp->setScaledSize(QSize(this->width(),this->height()));
    label->setGeometry(0,0,this->width(),this->height());
    label->setMovie(startUp); // 1. 设置要显示的 GIF 动画图片
    startUp->start();         // 2. 启动动画
    label->show();
    QObject::connect(startUp, &QMovie::frameChanged, [=](int frameNumber) {
        // GIF 动画执行一次就结束
        if (frameNumber == startUp->frameCount() - 1) {
            startUp->stop();
            delete label;   delete startUp;
            pic=bottom;
            update();
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    if (!pic.isNull())
    {
        QPainter* painter = new QPainter(this);
        QRect* rect = new QRect(0,0,this->width(),this->height());
        painter->drawImage(*rect,pic);
        delete painter; delete rect;
    }
}
