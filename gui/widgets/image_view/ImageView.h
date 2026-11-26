#pragma once

#include <QGraphicsView>

class QGraphicsPixmapItem;
class QImage;

class ImageView : public QGraphicsView
{
public:
    explicit ImageView(QWidget* parent = nullptr);
    void setImage(const QImage& image);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QGraphicsPixmapItem* _imageItem;
};
