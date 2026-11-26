#include "MainWindow.h"
#include "../help_popup/HelpPopup.h"
#include "../image_view/ImageView.h"
#include "FileParser.h"

#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QImage>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>

#include <algorithm>
#include <exception>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent, Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                              | Qt::WindowCloseButtonHint)
    , _tabs(new QTabWidget(this))
    , _contextMenu(new QMenu(this))
{
    resize(1024, 720);
    setCentralWidget(_tabs);

    _tabs->setTabsClosable(true);
    connect(_tabs, &QTabWidget::tabCloseRequested, this, [this](int index) {
        auto* page = _tabs->widget(index);
        _tabs->removeTab(index);
        page->deleteLater();
    });

    connect(_tabs, &QTabWidget::currentChanged, this, [this](int index) {
        statusBar()->showMessage(index < 0 ? tr("Ready") : _tabs->tabToolTip(index));
    });

    auto* fileMenu = menuBar()->addMenu(tr("&File"));
    auto* openAction = fileMenu->addAction(tr("&Open…"));
    openAction->setShortcut(QKeySequence::Open);

    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    fileMenu->addAction(tr("&Brightness/contrast…"));
    fileMenu->addAction(tr("&Quit"));

    _contextMenu->addAction(openAction);
    _tabs->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_tabs, &QWidget::customContextMenuRequested, this, [this](const QPoint& position) {
        _contextMenu->popup(_tabs->mapToGlobal(position));
    });

    auto* helpMenu = menuBar()->addMenu(tr("&Help"));
    auto* aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);

    addToolBar("Tools");

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::openFile()
{
    const auto path = QFileDialog::getOpenFileName(this, tr("Open X-ray image"), {},
                                                   tr("RAW images (*.raw);;All files (*)"));
    if (path.isEmpty()) {
        return;
    }

    try {
        const auto image = FileParser::read(std::filesystem::path(path.toStdU16String()));
        const auto grayscale = image.toGrayscale8();
        QImage preview(XrayImage::width, XrayImage::height, QImage::Format_Grayscale8);
        for (int row = 0; row < preview.height(); ++row) {
            std::copy_n(grayscale.data() + row * preview.width(), preview.width(), preview.scanLine(row));
        }

        auto* view = new ImageView(_tabs);
        view->setImage(preview);
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

void MainWindow::showAbout()
{
    HelpPopup popup(this);
    popup.exec();
}
