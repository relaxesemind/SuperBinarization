#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "Views/drawtool.h"


#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QLineF>


class ImageView : public QGraphicsView
{
    Q_OBJECT

    using pGraphicsItem = std::shared_ptr<QGraphicsPixmapItem>;
    using pLineItem     = std::shared_ptr<QGraphicsLineItem>;
    using pRectItem     = std::shared_ptr<QGraphicsRectItem>;

public:
    explicit ImageView(QWidget* widget = nullptr);

    QImage getImage() const;

    void setImage(const QImage &value);

protected:
        void wheelEvent(QWheelEvent *event)override;
        void mouseMoveEvent(QMouseEvent *event)override;
        void mousePressEvent(QMouseEvent *event)override;
        void mouseReleaseEvent(QMouseEvent *event)override;
        void resizeEvent(QResizeEvent *event)override;


public slots:
    void clearView();

signals:

private: //property
///Main
    QGraphicsScene *scene;
    qreal currentScale;
    pGraphicsItem item;
    QImage image;

///DrawingLogic
    bool isReadyToDraw();
    bool drawFlag;
    QPointF previousPoint, startPoint;

    QGraphicsRectItem *tempRect;
    QGraphicsLineItem *tempLine;
    QList<QGraphicsLineItem *> completeLine;
    QRectF positiveRect;



private: //methods
    QPointF transformCoordinates(QPointF pos) const;
    QPen currentPen();
    void fitImageInView();
    QImage createSubImage(const QImage& image, const QRect & rect);
    QImage createSubImage(const QImage& image, const QPainterPath & path);
};

#endif // IMAGEVIEW_H
