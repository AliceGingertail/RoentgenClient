#include <iostream>
#include <QApplication>

#include "FileParser.h"
#include "widgets/main_window/MainWindow.h"

constexpr std::string roentgen_version = ROENTGEN_VERSION;

QCoreApplication* createApplication(int &argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        if (!qstrcmp(argv[i], "-no-gui"))
            return new QCoreApplication(argc, argv);
    }
    return new QApplication(argc, argv);
}

int main(int argc, char* argv[])
{
    QCoreApplication::setApplicationVersion(QString::fromStdString(roentgen_version));

    const QScopedPointer app(createApplication(argc, argv));


    if (qobject_cast<QApplication *>(app.data())) {
        auto window = new MainWindow();
        window->show();
    } else {
        // start non-GUI version...
    }

    return app->exec();
}
