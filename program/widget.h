#ifndef WIDGET_H
#define WIDGET_H

#include <QPaintEvent>
#include <QWidget>
#include <QRect>

const QSize picSize(449,386);
const float picScale = 0.25;    //图片缩放比例
const QRect rectWin(1345,887,picSize.width()*picScale,picSize.height()*picScale);

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    //Widget(QWidget *parent = 0, Qt::WFlags flags =Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool);//构造函数
    ~Widget();

    void paintEvent(QPaintEvent * event)    Q_DECL_OVERRIDE;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
