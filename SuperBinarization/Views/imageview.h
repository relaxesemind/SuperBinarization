#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H


#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>

enum DrawTool : int
{
    Rect = 0,
    Curve = 1
};

class ImageView : public QGraphicsView
{
    Q_OBJECT

    using pGraphicsItem = std::shared_ptr<QGraphicsPixmapItem>;

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
///Class
    QGraphicsScene scene;
    qreal currentScale;
    pGraphicsItem item;
    QImage image;

///DrawingLogic
    bool isReadyToDraw();
    bool drawFlag;
    QPointF previousPoint;
    DrawTool drawTool;
    QGraphicsRectItem *tempRect;
    QRectF positiveRect;



private: //methods
    QPointF transformCoordinates(QPointF pos) const;
    void fitImageInView();
    QImage createSubImage(const QImage& image, const QRect & rect);
};

#endif // IMAGEVIEW_H
