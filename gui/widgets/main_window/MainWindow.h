#pragma once

#include <QMainWindow>

class ImageView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void openFile();
    void showAbout();

    ImageView* _imageView;
};
