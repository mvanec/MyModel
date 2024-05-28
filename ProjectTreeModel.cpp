#include "ProjectTreeModel.h"
#include "ProjectItem.h"

ProjectTreeModel::ProjectTreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
    , rootItem(QVariantList{tr("Year-Month"), tr("Day"), tr("Project")})
{
    setupModelData(QStringView{data}.split(u'\n'), &rootItem);
    printTree(&rootItem, QString(""));
}

ProjectTreeModel::~ProjectTreeModel() = default;

QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    const ProjectItem *parentItem = parent.isValid() ? (ProjectItem *) parent.internalPointer()
                                                     : &rootItem;

    if (auto *childItem = parentItem->childItem(row)) {
        return createIndex(row, column, childItem);
    }
    return {};
}

QModelIndex ProjectTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<ProjectItem *>(index.internalPointer());
    ProjectItem *parentItem = childItem->parentItem();

    return parentItem != &rootItem ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

int ProjectTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const ProjectItem *parentItem = parent.isValid()
                                        ? static_cast<const ProjectItem *>(parent.internalPointer())
                                        : &rootItem;

    return parentItem->childCount();
}

int ProjectTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<ProjectItem *>(parent.internalPointer())->columnCount();
    return headers.count();
    //return rootItem.columnCount();
}

QVariant ProjectTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const auto *item = static_cast<const ProjectItem *>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const
{
    return index.isValid() ? QAbstractItemModel::flags(index) : Qt::ItemFlags(Qt::NoItemFlags);
}

QVariant ProjectTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // return orientation == Qt::Horizontal && role == Qt::DisplayRole ? rootItem.data(section)
    //                                                                 : QVariant{};
    return orientation == Qt::Horizontal && role == Qt::DisplayRole ? headers.at(section)
                                                                    : QVariant{};
}

void ProjectTreeModel::setupModelData(const QList<QStringView> &lines, ProjectItem *parent)
{
    struct ParentIndentation
    {
        ProjectItem *parent;
        qsizetype indentation;
    };

    QList<ParentIndentation> state{{parent, 0}};

    for (const auto &line : lines) {
        qsizetype position = 0;
        for (; position < line.length() && line.at(position).isSpace(); ++position) {
        }

        const QStringView lineData = line.sliced(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const auto columnStrings = lineData.split(u'\t', Qt::SkipEmptyParts);
            QVariantList columnData;
            columnData.reserve(columnStrings.count());

            for (const auto &columnString : columnStrings)
                columnData << columnString.toString();

            if (position > state.constLast().indentation) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.
                auto *lastParent = state.constLast().parent;
                if (lastParent->childCount() > 0)
                    state.append({lastParent->childItem(lastParent->childCount() - 1), position});
            } else {
                while (position < state.constLast().indentation && !state.isEmpty())
                    state.removeLast();
            }

            // Append a new item to the current parent's list of children.
            auto *lastParent = state.constLast().parent;
            lastParent->appendChildItem(std::make_unique<ProjectItem>(columnData, lastParent));
        }
    }
}

void ProjectTreeModel::printTree(ProjectItem *item, QString indent)
{
    if (item == nullptr) {
        return;
    }

    qDebug().noquote() << indent << item->toString() << item->row();
    int rows = item->childCount();
    indent.append("    ");
    for (int i = 0; i < rows; i++) {
        printTree(item->childItem(i), indent);
    }
}
