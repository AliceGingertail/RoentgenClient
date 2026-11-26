#include "ImageView.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollBar>
#include <QWheelEvent>

#include <algorithm>
#include <cmath>
#include <utility>

ImageView::ImageView(XrayImage image, QWidget* parent)
    : QGraphicsView(parent)
    , _image(std::move(image))
    , _imageItem(nullptr)
{
    const auto grayscale = _image.toGrayscale8();
    QImage preview(XrayImage::width, XrayImage::height, QImage::Format_Grayscale8);
    for (int row = 0; row < preview.height(); ++row) {
        std::copy_n(grayscale.data() + row * preview.width(), preview.width(), preview.scanLine(row));
    }

    setScene(new QGraphicsScene(this));
    _imageItem = scene()->addPixmap(QPixmap::fromImage(preview));
    _imageItem->setTransformationMode(Qt::SmoothTransformation);
    setSceneRect(_imageItem->boundingRect());
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::ScrollHandDrag);
    viewport()->setMouseTracking(true);
}

bool ImageView::viewportEvent(QEvent* event)
{
    if (event->type() == QEvent::Leave) {
        emit pixelHoverCleared();
    }
    return QGraphicsView::viewportEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
    reportPixel(event->position().toPoint());
}

void ImageView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    if (_imageItem && _zoom == 1.0) {
        fitInView(_imageItem, Qt::KeepAspectRatio);
    }
}

void ImageView::wheelEvent(QWheelEvent* event)
{
    const auto delta = event->angleDelta().y();
    if (delta == 0) {
        event->ignore();
        return;
    }

    const auto position = event->position().toPoint();
    const auto anchor = mapToScene(position);
    const auto zoom = std::clamp(_zoom * std::pow(1.25, delta / 120.0), 1.0, 16.0);
    const auto factor = zoom / _zoom;
    _zoom = zoom;
    if (_zoom == 1.0) {
        fitInView(_imageItem, Qt::KeepAspectRatio);
    } else {
        scale(factor, factor);
        const auto offset = mapFromScene(anchor) - position;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + offset.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() + offset.y());
    }

    reportPixel(position);
    event->accept();
}

void ImageView::reportPixel(const QPoint& position)
{
    // Переводим координаты виджета в координаты исходного снимка.
    const auto point = _imageItem->mapFromScene(mapToScene(position));
    if (!_imageItem->boundingRect().contains(point) ||
        point.x() >= XrayImage::width || point.y() >= XrayImage::height) {
        emit pixelHoverCleared();
        return;
    }

    const auto x = static_cast<int>(point.x());
    const auto y = static_cast<int>(point.y());
    emit pixelHovered(x, y, _image.intensities[y * XrayImage::width + x]);
}
