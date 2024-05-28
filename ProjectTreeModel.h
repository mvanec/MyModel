#pragma once

#include <QAbstractItemModel>
#include "ProjectItem.h"

class ProjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(ProjectTreeModel)

    explicit ProjectTreeModel(const QString &data, QObject *parent = nullptr);
    ~ProjectTreeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

private:
    QList<QString> headers = {"Year-Month", "Date", "Project", "Duration", "Pay"};
    static void setupModelData(const QList<QStringView> &lines, ProjectItem *parent);
    void printTree(ProjectItem *item, QString indent);

    ProjectItem rootItem;
};
