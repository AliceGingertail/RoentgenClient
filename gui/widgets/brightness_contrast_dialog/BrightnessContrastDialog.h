#pragma once

#include <QDialog>

class QSlider;

class BrightnessContrastDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrightnessContrastDialog(int brightness, int contrast, QWidget* parent = nullptr);

signals:
    void adjustmentChanged(int brightness, int contrast);

private:
    QSlider* _brightness;
    QSlider* _contrast;
};
