#include "ProjectItem.h"

ProjectItem::ProjectItem(QVariantList data, ProjectItem *parentItem)
    : itemData(QVariantList(data))
    , parent(parentItem)
{}

void ProjectItem::appendChildItem(std::unique_ptr<ProjectItem> &&child)
//void ProjectItem::appendChildItem(ProjectItem *child)
{
    children.push_back(std::move(child));
    // children.append(child);
}

ProjectItem *ProjectItem::childItem(int row) const
{
    if (row >= 0 && row < childCount()) {
        return children[row].get();
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

    for (int i = 1; i < int(parent->children.size()); i++) {
        if (this == parent->children[i].get()) {
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
    qDebug().noquote() << "Working with item" << this;
    qDebug() << "There are this many items: " << itemData.count();
    for (int i = 0; i < itemData.count(); i++) {
        val.append(itemData.value(i).value<QString>());
        val.append(" -- ");
    }
    return val;
}
