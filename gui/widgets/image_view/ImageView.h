#pragma once

#include "FileParser.h"

#include <QFutureWatcher>
#include <QGraphicsView>
#include <QImage>

#include <memory>
#include <utility>

class QGraphicsPixmapItem;

class ImageView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ImageView(XrayImage image, QWidget* parent = nullptr);
    std::pair<int, int> adjustment() const { return _adjustment; }
    void setAdjustment(int brightness, int contrast);

signals:
    void pixelHovered(int x, int y, quint32 intensity);
    void pixelHoverCleared();

protected:
    bool viewportEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void renderPreview();
    void reportPixel(const QPoint& position);

    std::shared_ptr<const XrayImage> _image;
    QGraphicsPixmapItem* _imageItem;
    qreal _zoom = 1.0;
    std::pair<int, int> _adjustment{0, 0};
    std::pair<int, int> _renderingAdjustment{0, 0};
    bool _rendering = false;
    QFutureWatcher<QImage> _previewWatcher;
};
