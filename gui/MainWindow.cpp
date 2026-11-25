#include "MainWindow.h"
#include "widgets/help_popup/HelpPopup.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent, Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                              | Qt::WindowCloseButtonHint)
{
    resize(1024, 720);

    auto* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Open"));
    fileMenu->addAction(tr("&Brightness/contrast…"));
    fileMenu->addAction(tr("&Quit"));

    auto* helpMenu = menuBar()->addMenu(tr("&Help"));
    auto* aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);

    addToolBar("Tools");

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::showAbout()
{
    HelpPopup popup(this);
    popup.exec();
}
