#include "ImageView.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPixmap>

ImageView::ImageView(QWidget* parent)
    : QGraphicsView(parent)
    , _imageItem(nullptr)
{
    setScene(new QGraphicsScene(this));
    _imageItem = scene()->addPixmap(QPixmap());
    _imageItem->setTransformationMode(Qt::SmoothTransformation);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ImageView::setImage(const QImage& image)
{
    _imageItem->setPixmap(QPixmap::fromImage(image));
    setSceneRect(_imageItem->boundingRect());
    fitInView(_imageItem, Qt::KeepAspectRatio);
}

void ImageView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(_imageItem, Qt::KeepAspectRatio);
}
