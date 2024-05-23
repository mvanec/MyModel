#pragma once

#include <QVariant>
#include <QtCore>

class ProjectItem
{
public:
    explicit ProjectItem(QVariantList data, ProjectItem *parentItem = nullptr);

    void appendChildItem(ProjectItem child);

    ProjectItem *childItem(int row) const;
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    ProjectItem *parentItem();
    QString toString() const;

private:
    QList<ProjectItem> children;
    QVariantList itemData;
    ProjectItem *parent;
};
