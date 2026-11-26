#pragma once

#include "FileParser.h"

#include <QGraphicsView>

class QGraphicsPixmapItem;

class ImageView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageView(XrayImage image, QWidget* parent = nullptr);

signals:
    void pixelHovered(int x, int y, quint32 intensity);
    void pixelHoverCleared();

protected:
    bool viewportEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void reportPixel(const QPoint& position);

    XrayImage _image;
    QGraphicsPixmapItem* _imageItem;
    qreal _zoom = 1.0;
};
