#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QImage>
#include <QLabel>
#include <QPixmap>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
//界面调整
    this->setAttribute(Qt::WA_TranslucentBackground);       //设置背景透明
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);//设置无边框
    setWindowFlag(Qt::NoDropShadowWindowHint);              //去除阴影边框
//加载资源
    bottom.load(":/pic/bottom.png");
    startUp = new QMovie(":/pic/bottom.gif");
    drag = new QMovie(":/pic/drag.gif");
//图片缩放
    bottom=bottom.scaled(bottom.width()*picScale,bottom.height()*picScale);
    startUp->jumpToFrame(0);    //在播放前调用frameRect需先将当前帧置为有效置(如第0帧)
    startUp->setScaledSize(QSize(startUp->frameRect().width()*picScale,startUp->frameRect().height()*picScale));
    drag->jumpToFrame(0);
    drag->setScaledSize(QSize(drag->frameRect().width()*picScale,drag->frameRect().height()*picScale));
//播放初始动画(阿尼亚探头)
    this->setGeometry(QRect(startP,QSize(bottom.width(),bottom.height())));
    ui->label->setGeometry(this->rect());
    ui->label->setMovie(startUp); // 1. 设置要显示的 GIF 动画图片
    startUp->start();         // 2. 启动动画
    QObject::connect(startUp, &QMovie::frameChanged, [=](int frameNumber) {
        // GIF 动画执行一次就结束
        if (frameNumber == startUp->frameCount() - 1) {
            startUp->stop();
            ui->label->setPixmap(QPixmap::fromImage(bottom));
//            pic=bottom;
//            update();
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
//    if (!pic.isNull())
//    {
//        QPainter* painter = new QPainter(this);
//        QRect* rect = new QRect(0,0,this->width(),this->height());
//        painter->drawImage(*rect,pic);
//        delete painter; delete rect;
    //    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging=true;
    }
    else if (event->button() == Qt::RightButton)
    {

    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        if (!dragGifShown)
        {
            this->setGeometry(QRect(event->globalPosition().toPoint()-QPoint(drag->frameRect().width()/2,drag->frameRect().height()/2), \
                                    QSize(drag->frameRect().width(),drag->frameRect().height())));
            ui->label->setGeometry(this->rect());
            ui->label->setMovie(drag);
            drag->start();
            dragGifShown=true;
        }
        this->setGeometry(QRect(event->globalPosition().toPoint()-QPoint(drag->frameRect().width()/2,drag->frameRect().height()/2),this->size()));
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (dragGifShown)     //刚刚结束拖动
        {
            isDragging=false;
            dragGifShown=false;
            this->setGeometry(QRect(event->globalPosition().toPoint()-QPoint(bottom.width()/2,bottom.height()/2),QSize(bottom.width(),bottom.height())));
            ui->label->setGeometry(this->rect());
            ui->label->clear();
            ui->label->setPixmap(QPixmap::fromImage(bottom));
        }
        else if (isDragging)    //只是左键单击未拖动
        {
            isDragging=false;
        }
    }
}
