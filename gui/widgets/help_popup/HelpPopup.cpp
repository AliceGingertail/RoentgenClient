//
// Created by samsapiol on 11/25/26.
//

#include "HelpPopup.h"

#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

HelpPopup::HelpPopup(QWidget* parent)
    : QDialog(parent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                          | Qt::WindowCloseButtonHint)
{
    setWindowTitle(tr("About RoentgenClient"));

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(32, 28, 32, 20);
    layout->setSpacing(8);

    auto* title = new QLabel(QStringLiteral("RoentgenClient"), this);
    auto titleFont = title->font();
    titleFont.setPixelSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    layout->addWidget(title, 0, Qt::AlignHCenter);

    const auto version = QCoreApplication::applicationVersion();
    if (!version.isEmpty()) {
        auto* versionLabel = new QLabel(tr("Version %1").arg(version), this);
        versionLabel->setTextFormat(Qt::PlainText);
        layout->addWidget(versionLabel, 0, Qt::AlignHCenter);
    }

    auto* copyrightLabel = new QLabel(tr("Copyright © 2026 Roentgen. All rights reserved."), this);
    copyrightLabel->setTextFormat(Qt::PlainText);
    layout->addWidget(copyrightLabel, 0, Qt::AlignHCenter);

    layout->addSpacing(12);
    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttons->setCenterButtons(true);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    setFixedSize(QDialog::sizeHint().expandedTo(QSize(420, 0)));
}
