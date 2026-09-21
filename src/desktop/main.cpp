#include <SmartHomeDashboard.hpp>

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile("styles.qss");
    if(styleFile.open(QIODevice::ReadOnly))
    {
        QTextStream textStream(&styleFile);
        QString styleSheet = textStream.readAll();
        styleFile.close();
        app.setStyleSheet(styleSheet);
    }

    SmartHomeDashboard dashboard;
    dashboard.show();

    return app.exec();
}