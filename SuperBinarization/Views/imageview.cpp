#include "imageview.h"
#include <QDebug>
#include "Common/consts.h"
#include "Models/appstorage.h"
#include <QtMath>
#include "Common/magic.h"

ImageView::ImageView(QWidget *widget) : QGraphicsView(widget)
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setAlignment(Qt::AlignCenter);
    currentScale = 1.0;
    drawFlag = false;
    tempRect = nullptr;
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet(QString(
                          "background-color: #89AFD1;"
                          ));
}

QImage ImageView::getImage() const
{
    return image;
}

void ImageView::setImage(const QImage &value)
{
    image = value;

    QPixmap pixmap = QPixmap::fromImage(value);
    item = std::make_shared<QGraphicsPixmapItem>(pixmap);
    scene->addItem(item.get());

    centerOn(item.get());
    scene->setSceneRect(0,0,pixmap.width(),pixmap.height());
    fitInView(item.get(),Qt::KeepAspectRatio);

    this->update();
//    scale(this->rect().width()/scene.sceneRect().width(),this->rect().width()/scene.sceneRect().width());
//    QSize imageSize = image.size();
//    while (imageSize.height() > 800 or imageSize.width() > 800)
//    {
//         imageSize /= Global::zoomMul;

//    }
//    this->setFixedSize(imageSize);
}

void ImageView::clearView()
{
    setImage(QImage());
    scene->clear();
}

bool ImageView::isReadyToDraw()
{
    return (!scene->items().isEmpty());
}

QPointF ImageView::transformCoordinates(QPointF pos) const
{
    QPointF l = mapToScene(pos.x(),pos.y());
    return l;
}

QPen ImageView::currentPen()
{
    return QPen(QBrush(Qt::green),2,Qt::SolidLine,Qt::RoundCap);
}

void ImageView::fitImageInView()
{
        //adapt scene's bounding rect to image
        scene->setSceneRect(QRectF(0, 0, item->pixmap().width(), item->pixmap().height()));

        //fit scene into graphicsview - workaround from
        //http://www.qtforum.org/article/35467/the-2px-border-problem-in-qgraphicsview.html
        QTransform matrix(1,0,0,0,1,0,0,0,1);
        qreal xscale = this->width() / this->sceneRect().width();
        qreal yscale = this->height() / this->sceneRect().height();
        xscale = yscale = qMin(xscale,yscale);
        matrix.scale(xscale,yscale);
        this->setTransform(matrix);

        //compute current scaleFactor and corresponding wheel position
        double width = (double)scene->width() / (double)this->width();
        double height = (double)scene->height() / (double)this->height();
        currentScale = (height > width) ? (1.0 / height) : (1.0 / width);
//        wheelPosition = calcWheelPosition(scaleFactor);


        //        choosePixmapTransform();
}

QImage ImageView::createSubImage(const QImage &image, const QRect &rect)
{
    size_t offset = rect.x() * image.depth() / 8 + rect.y() * image.bytesPerLine();
    return QImage(image.bits() + offset, rect.width(), rect.height(), image.bytesPerLine(), image.format());
}

QImage ImageView::createSubImage(const QImage &input, const QPainterPath &path)
{
    if(!input.isNull() and !path.isEmpty())
    {
        QRect r = path.boundingRect().toRect().intersected(input.rect());
        QImage tmp(input.size(), QImage::Format_ARGB32);
        tmp.fill(Qt::transparent);

        QPainter painter(&tmp);
        painter.setClipPath(path);
        painter.drawImage(QPoint{}, input, input.rect());
        painter.end();

        return tmp.copy(r);
    }
    return QImage();
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if (scene->items().isEmpty())
     {
         return;
     }

     centerOn(item.get());
     QPoint degrees = event->angleDelta() / 8;
     if (degrees.y() > 0)
     {
         scale(Global::zoomMul,Global::zoomMul);
         currentScale *= Global::zoomMul;
     }
     else
     {
         scale(1/Global::zoomMul,1/Global::zoomMul);
         currentScale /= Global::zoomMul;
     }
//     emit imageDidZoomed(currentZoom);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if (!drawFlag or !isReadyToDraw())
    {
        return;
    }

    QPointF point = transformCoordinates(event->pos());

//    if (image.rect().contains(point.toPoint()))
//    {
//        return;
//    }

    switch (AppStorage::shared().drawTool)
    {
        case DrawTool::Rect:
        {
             if (tempRect)
             {
                 scene->removeItem(tempRect);
             }
//             qreal width = std::abs(previousPoint.x() - point.x());
//             qreal height = std::abs(previousPoint.y() - point.y());
//             QPoint tl;
//             qreal x = std::min(previousPoint.x(),point.x());
//             qreal y = std::max(previousPoint.y(),point.y());
//             QRectF rectf(x,y,width,height);

             QRectF nativeRect;
             nativeRect.setBottomLeft(previousPoint);
             nativeRect.setTopRight(point);

//             positiveRect = rectf;

             tempRect = scene->addRect(nativeRect,QPen(QBrush(Qt::red),3,Qt::SolidLine,Qt::RoundCap));
             this->update();
        break;
        }
         case DrawTool::Curve:
        {
            QLineF line(previousPoint, point);
            completeLine << scene->addLine(line,currentPen());
            previousPoint = point;
        break;
        }
    }
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if (!isReadyToDraw())
    {
        return;
    }

    for_magic(it,completeLine)
    {
        scene->removeItem(*it);
    }
    completeLine.clear();

    drawFlag = true;
    previousPoint = transformCoordinates(event->pos());
    startPoint = previousPoint;
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    drawFlag = false;

    DrawTool tool = AppStorage::shared().drawTool;

    if (tool == DrawTool::Rect)
    {
        QRectF rect = tempRect->rect();

        if (!image.rect().contains(rect.toRect()))
        {
            return;
        }

        QRect rectt = rect.toRect();

        qreal width = std::abs(rectt.bottomLeft().x() - rectt.bottomRight().x());
        qreal height = std::abs(rectt.bottomLeft().y() - rectt.topLeft().y());
        QPoint tl;
        tl.rx() = std::min(rectt.topLeft().x(),rectt.topRight().x());
        tl.ry() = std::max(rectt.topLeft().y(),rectt.topRight().y());

        QRect normRect(tl.x(),tl.y(),width,height);

        QImage piece = image.copy(normRect);
    //    setImage(piece);
    }

    if (tool == DrawTool::Curve)
    {
        QPointF point = transformCoordinates(event->pos());
        completeLine << scene->addLine({point,startPoint},currentPen());

//        for_magic(it,completeLine)
//        {
//            scene.removeItem(*it);
//        }
    }
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    QRectF rectf = sceneRect();
    fitInView(rectf,Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(event);
}















