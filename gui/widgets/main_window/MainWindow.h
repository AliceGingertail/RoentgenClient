#pragma once

#include <QMainWindow>

class QLabel;
class QMenu;
class QTabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void openFile();
    void showAbout();

    QTabWidget* _tabs;
    QMenu* _contextMenu;
    QLabel* _pixelStatus;
};
