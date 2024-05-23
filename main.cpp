#include "ProjectTreeModel.h"

#include <QApplication>
#include <QFile>
#include <QScreen>
#include <QTreeView>

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();
    // return a.exec();

    QApplication app(argc, argv);

    constexpr auto fileName = ":/default.txt"_L1;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << fileName << " cannot be opened: "_L1 << file.errorString();
    }

    ProjectTreeModel model(QString::fromUtf8(file.readAll()));
    file.close();

    QTreeView view;
    view.setModel(&model);
    view.setWindowTitle(ProjectTreeModel::tr("Simple Tree Model"));
    for (int c = 0; c < model.columnCount(); ++c)
        view.resizeColumnToContents(c);
    view.expandAll();
    const auto screenSize = view.screen()->availableSize();
    view.resize({screenSize.width() / 2, screenSize.height() * 2 / 3});
    view.show();
    return QCoreApplication::exec();
}
