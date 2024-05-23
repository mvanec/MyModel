#include "ProjectItem.h"

ProjectItem::ProjectItem(QVariantList data, ProjectItem *parentItem)
    : itemData(QVariantList(data))
    , parent(parentItem)
{}

void ProjectItem::appendChildItem(ProjectItem child)
{
    children.append(child);
}

ProjectItem *ProjectItem::childItem(int row) const
{
    if (row >= 0 && row < childCount()) {
        return const_cast<ProjectItem *>(&children[row]);
    }
    return nullptr;
}

int ProjectItem::childCount() const
{
    return int(children.size());
}

int ProjectItem::columnCount() const
{
    int size = int(itemData.count());
    return size;
}

QVariant ProjectItem::data(int column) const
{
    QVariant datum = itemData.value(column);
    return datum;
}

int ProjectItem::row() const
{
    int row = 0;
    if (parent == nullptr)
        return row;

    QList<ProjectItem> chillun = parent->children;
    for (int i = 1; i < chillun.length(); i++) {
        if (this == &chillun[i]) {
            row = i;
            break;
        }
    }
    return row;
}

ProjectItem *ProjectItem::parentItem()
{
    return parent;
}

QString ProjectItem::toString() const
{
    QString val("");
    for (int i = 0; i < itemData.count(); i++) {
        val.append(itemData.value(i).value<QString>());
        val.append(" -- ");
    }
    return val;
}
