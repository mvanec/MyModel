#include "ProjectItem.h"

ProjectItem::ProjectItem(QVariantList data, ProjectItem *parentItem)
    : itemData(QVariantList(data))
    , parent(parentItem)
{}

void ProjectItem::appendChildItem(std::unique_ptr<ProjectItem> &&child)
{
    children.push_back(std::move(child));
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

    auto it = find_if(parent->children.begin(),
                      parent->children.end(),
                      [this](const std::unique_ptr<ProjectItem> &treeItem) {
                          return treeItem.get() == this;
                      });
    row = (it != parent->children.end()) ? distance(parent->children.begin(), it) : -1;

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
