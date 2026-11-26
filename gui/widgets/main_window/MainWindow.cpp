#include "MainWindow.h"
#include "../brightness_contrast_dialog/BrightnessContrastDialog.h"
#include "../help_popup/HelpPopup.h"
#include "../image_view/ImageView.h"
#include "FileParser.h"

#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>

#include <exception>
#include <utility>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent, Qt::Window)
    , _tabs(new QTabWidget(this))
    , _contextMenu(new QMenu(this))
    , _pixelStatus(new QLabel(this))
{
    resize(1024, 720);
    setCentralWidget(_tabs);

    // Поведение табов
    _tabs->setTabsClosable(true);
    connect(_tabs, &QTabWidget::tabCloseRequested, this, [this](const int index) {
        auto* page = _tabs->widget(index);
        _tabs->removeTab(index);
        page->deleteLater();
    });

    const auto fileMenu = menuBar()->addMenu(tr("&File"));
    const auto openAction = fileMenu->addAction(tr("&Open…"));
    openAction->setShortcut(QKeySequence::Open);

    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    // adjustment action
    auto* adjustmentAction = fileMenu->addAction(tr("&Brightness/contrast…"));
    adjustmentAction->setEnabled(false);
    connect(adjustmentAction, &QAction::triggered, this, &MainWindow::showBrightnessContrast);
    connect(_tabs, &QTabWidget::currentChanged, adjustmentAction, [adjustmentAction](const int index) {
        adjustmentAction->setEnabled(index >= 0);
    });

    // quit action
    auto quitAction = fileMenu->addAction(tr("&Quit"));
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);

    // Кнопки контекстного меню
    _contextMenu->addAction(openAction);
    _contextMenu->addAction(adjustmentAction);

    // Поведение контекстного меню
    _tabs->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tabs, &QWidget::customContextMenuRequested, this, [this](const QPoint& position) {
        _contextMenu->popup(_tabs->mapToGlobal(position));
    });

    // Help меню
    auto* helpMenu = menuBar()->addMenu(tr("&Help"));
    auto* aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);

    // Нижний статус бар с координатами и именем файлаа
    statusBar()->addPermanentWidget(_pixelStatus);
    connect(_tabs, &QTabWidget::currentChanged, this, [this](int index) {
        _pixelStatus->clear();
        statusBar()->showMessage(index < 0 ? tr("Ready") : _tabs->tabToolTip(index));
    });
}

void MainWindow::openFile()
{
    const auto path = QFileDialog::getOpenFileName(this, tr("Open X-ray image"), {},
                                                   tr("RAW images (*.raw);;All files (*)"));
    if (path.isEmpty())
        return;

    try {
        auto image = FileParser::read(std::filesystem::path(path.toStdU16String()));
        auto* view = new ImageView(std::move(image), _tabs);
        connect(view, &ImageView::pixelHovered, this, [this](int x, int y, quint32 intensity) {
            _pixelStatus->setText(tr("X: %1   Y: %2   Intensity: %3").arg(x).arg(y).arg(intensity));
        });

        connect(view, &ImageView::pixelHoverCleared, _pixelStatus, &QLabel::clear);
        view->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(view, &QWidget::customContextMenuRequested, this, [this, view](const QPoint& position) {
            _contextMenu->popup(view->viewport()->mapToGlobal(position));
        });

        const auto index = _tabs->addTab(view, QFileInfo(path).fileName().replace("&", "&&"));
        _tabs->setTabToolTip(index, path);
        _tabs->setCurrentIndex(index);

        statusBar()->showMessage(path);
    } catch (const std::exception& error) {
        QMessageBox::warning(this, tr("Could not open image"),
                             tr("Could not read %1.\n%2")
                                 .arg(QFileInfo(path).fileName(), QString::fromLocal8Bit(error.what())));
    }
}

void MainWindow::showBrightnessContrast()
{
    const auto view = qobject_cast<ImageView*>(_tabs->currentWidget());
    if (!view) {
        return;
    }

    const auto [brightness, contrast] = view->adjustment();
    BrightnessContrastDialog dialog(brightness, contrast, this);
    connect(&dialog, &BrightnessContrastDialog::adjustmentChanged, view, &ImageView::setAdjustment);
    dialog.exec();
}

void MainWindow::showAbout()
{
    HelpPopup popup(this);
    popup.exec();
}
