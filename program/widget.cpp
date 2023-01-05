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
    top.load(":/pic/top.png");
    right.load(":/pic/right.png");
    left.load(":/pic/left.png");
    startUp = new QMovie(":/pic/bottom.gif");
    drag = new QMovie(":/pic/drag.gif");
    stand = new QMovie(":/pic/stand.gif");
    turn = new QMovie(":/pic/turn.gif");
//图片缩放
    bottom=bottom.scaled(bottom.width()*picScale,bottom.height()*picScale);
    top=top.scaled(top.width()*picScale,top.height()*picScale);
    right=right.scaled(right.width()*picScale,right.height()*picScale);
    left=left.scaled(left.width()*picScale,left.height()*picScale);
    startUp->jumpToFrame(0);    //在播放前调用frameRect需先将当前帧置为有效置(如第0帧)
    startUp->setScaledSize(QSize(startUp->frameRect().width()*picScale,startUp->frameRect().height()*picScale));
    drag->jumpToFrame(0);
    drag->setScaledSize(QSize(drag->frameRect().width()*picScale,drag->frameRect().height()*picScale));
    stand->jumpToFrame(0);
    stand->setScaledSize(QSize(stand->frameRect().width()*picScale,stand->frameRect().height()*picScale));
    turn->jumpToFrame(0);
    turn->setScaledSize(QSize(turn->frameRect().width()*picScale,turn->frameRect().height()*picScale));
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
            ui->label->clear();
            switch (isByEdge())
            {
                case 0:
                    this->setGeometry(QRect(event->globalPosition().toPoint()-QPoint(stand->frameRect().width()/2,stand->frameRect().height()/2),QSize(stand->frameRect().size())));
                    ui->label->setGeometry(this->rect());
                    ui->label->setMovie(stand);
                    stand->start();
                    break;
                case TOP:
                    this->setGeometry(event->globalPosition().toPoint().x(),0,top.width(),top.height());
                    ui->label->setGeometry(this->rect());
                    ui->label->setPixmap(QPixmap::fromImage(top));
                    break;
                case BOTTOM:
                    this->setGeometry(event->globalPosition().toPoint().x(),ScrHeight-bottom.height()+28*picScale,bottom.width(),bottom.height());
                    ui->label->setGeometry(this->rect());
                    ui->label->setPixmap(QPixmap::fromImage(bottom));
                    break;
                case LEFT:
                    this->setGeometry(0,event->globalPosition().toPoint().y(),left.width(),left.height());
                    ui->label->setGeometry(this->rect());
                    ui->label->setPixmap(QPixmap::fromImage(left));
                    break;
                case RIGHT:
                    this->setGeometry(ScrWidth-right.width(),event->globalPosition().toPoint().y(),right.width(),right.height());
                    ui->label->setGeometry(this->rect());
                    ui->label->setPixmap(QPixmap::fromImage(right));
                    break;
            }
        }
        else if (isDragging)    //只是左键单击未拖动
        {
            isDragging=false;
        }
    }
}

int Widget::isByEdge()
{
    qDebug("%d\n",this->width());
    if (this->pos().x()<absorbDis)
        return LEFT;
    else if (ScrWidth-this->pos().x()-drag->frameRect().width()<absorbDis)
        return RIGHT;
    else if (this->pos().y()<absorbDis)
        return TOP;
    else if (ScrHeight-this->pos().y()-drag->frameRect().height()<absorbDis)
        return BOTTOM;
    else
        return 0;
}
