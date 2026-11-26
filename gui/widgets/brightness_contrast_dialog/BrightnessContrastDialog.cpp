#include "BrightnessContrastDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPushButton>
#include <QSlider>

BrightnessContrastDialog::BrightnessContrastDialog(int brightness, int contrast, QWidget* parent)
    : QDialog(parent)
    , _brightness(new QSlider(Qt::Horizontal, this))
    , _contrast(new QSlider(Qt::Horizontal, this))
{
    setWindowTitle(tr("Brightness and contrast"));
    setMinimumWidth(320);

    _brightness->setRange(-100, 100);
    _brightness->setValue(brightness);
    _contrast->setRange(-100, 100);
    _contrast->setValue(contrast);

    auto* layout = new QFormLayout(this);
    layout->addRow(tr("&Brightness"), _brightness);
    layout->addRow(tr("&Contrast"), _contrast);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Reset | QDialogButtonBox::Close, this);
    layout->addRow(buttons);

    const auto notify = [this] {
        emit adjustmentChanged(_brightness->value(), _contrast->value());
    };
    connect(_brightness, &QSlider::valueChanged, this, notify);
    connect(_contrast, &QSlider::valueChanged, this, notify);
    connect(buttons->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, [this] {
        _brightness->setValue(0);
        _contrast->setValue(0);
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
