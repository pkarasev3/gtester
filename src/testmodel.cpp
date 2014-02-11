#include "testmodel.h"

#include <QtGui>

#include "testitemroot.h"
#include "testitemexecutable.h"
#include "executablebase.h"
#include "executabletester.h"

TestModel::TestModel(QObject *parent)
    : QAbstractItemModel(parent), _rootItem(new TestItemRoot(this))
{
}

TestModel::~TestModel()
{
    delete _rootItem;
}

int TestModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TestItemBase*>(parent.internalPointer())->columnCount();
    else
        return _rootItem->columnCount();
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TestItemBase *item = static_cast<TestItemBase*>(index.internalPointer());

    if (role == Qt::ForegroundRole)
        return QColor(Qt::black);

    if (role == Qt::BackgroundColorRole)
        return item->getBackgroundColor(index.column());

    if (role == Qt::CheckStateRole && item->hasCheckbox(index.column()))
        return item->getCheckState(index.column());

    if (role != Qt::DisplayRole)
        return QVariant();

    return item->getData(index.column());
}

bool TestModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    TestItemBase *item = static_cast<TestItemBase*>(index.internalPointer());

    if (role == Qt::CheckStateRole) {
        item->setCheckState(index.column(), value);
        QVector<int> roles;
        roles << role;
        emit dataChanged(index, index, roles);
        updateParents(index, roles);
        updateChildren(index, roles);
        return true;
    }

    return false;
}

Qt::ItemFlags TestModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    TestItemBase *item = static_cast<TestItemBase*>(index.internalPointer());

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (item->hasCheckbox(index.column()))
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->getData(section);

    return QVariant();
}

QModelIndex TestModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TestItemBase *parentItem;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<TestItemBase*>(parent.internalPointer());

    TestItemBase *childItem = parentItem->getChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TestModel::index(TestItemBase *item) const
{
    if (item == _rootItem)
        return QModelIndex();

    return createIndex(item->getRow(), 0, item);
}

QModelIndex TestModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TestItemBase *childItem = static_cast<TestItemBase*>(index.internalPointer());
    TestItemBase *parentItem = childItem->getParent();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->getRow(), 0, parentItem);
}

int TestModel::rowCount(const QModelIndex &parent) const
{
    TestItemBase *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<TestItemBase*>(parent.internalPointer());

    return parentItem->childCount();
}

TestItemRoot * TestModel::rootItem()
{
    return _rootItem;
}

void TestModel::execute()
{

}

void TestModel::terminate()
{

}

void TestModel::addExecutable(QString fileName)
{
    refresh(index( new TestItemExecutable(QFileInfo(fileName), rootItem()) ));
}

void TestModel::removeExecutable(const QModelIndex index)
{
    removeExecutable( static_cast<TestItemBase*>(index.internalPointer()) );
}

void TestModel::removeExecutable(TestItemBase *item)
{
    // Check if the given item is the root item
    TestItemRoot *root = dynamic_cast<TestItemRoot*>(item);
    if (root != 0) {
        // If we got the root item, refresh each executable
        for (int i = 0; i < root->childCount(); i++) {
            TestItemExecutable *exec = dynamic_cast<TestItemExecutable*>(root->getChild(i));
            if (exec == 0) continue;

            removeExecutable(exec);
        }

        return;
    }

    // If we got anything else, let's go trough the items up to the executable
    TestItemExecutable *exec = 0;
    while (exec == 0 && item != 0) {
        exec = dynamic_cast<TestItemExecutable*>(item);
        if (exec != 0) {
            removeExecutable(exec);
            return;
        }

        item = item->getParent();
    }
}

void TestModel::removeExecutable(TestItemExecutable *item)
{
    beginRemoveRows(index(item->getParent()), item->getRow(), item->getRow());
    delete item;
    endRemoveRows();
}

void TestModel::refresh(const QModelIndex &index)
{
    refresh( static_cast<TestItemBase*>(index.internalPointer()) );
}

void TestModel::refresh(TestItemBase *item)
{
    // Check if the given item is the root item
    TestItemRoot *root = dynamic_cast<TestItemRoot*>(item);
    if (root != 0) {
        // If we got the root item, refresh each executable
        for (int i = 0; i < root->childCount(); i++) {
            TestItemExecutable *exec = dynamic_cast<TestItemExecutable*>(root->getChild(i));
            if (exec == 0) continue;

            refresh(exec);
        }

        return;
    }

    // If we got anything else, let's go trough the items up to the executable
    TestItemExecutable *exec = 0;
    while (exec == 0 && item != 0) {
        exec = dynamic_cast<TestItemExecutable*>(item);
        if (exec != 0) {
            refresh(exec);
            return;
        }

        item = item->getParent();
    }
}

void TestModel::refresh(TestItemExecutable *item)
{
    beginRemoveRows(index(item), 0, item->childCount());
    item->deleteChildren();
    endRemoveRows();

    queueJob(new ExecutableTester(item, this));
}

void TestModel::jobExecuted()
{
    emit execute();
    emit executionStateChanged(true);
}

void TestModel::jobFinished()
{
    emit executionStateChanged(false);
    emit finished();

    delete _pendingJobs.first();
    _pendingJobs.pop_front();
    if (_pendingJobs.count() > 0)
        _pendingJobs[0]->execute();
}

void TestModel::jobTerminated()
{
    emit executionStateChanged(false);
    emit terminated("Unknown error");
}

void TestModel::queueJob(ExecutableBase *job)
{
    connect(job, SIGNAL(started()), this, SLOT(jobExecuted()));
    connect(job, SIGNAL(finished()), this, SLOT(jobFinished()));
    connect(job, SIGNAL(terminated()), this, SLOT(jobTerminated()));

    _pendingJobs.append(job);

    if (_pendingJobs.count() == 1)
        job->execute();

}

void TestModel::updateParents(const QModelIndex &index, QVector<int> roles)
{
    for (QModelIndex parent = index.parent(); parent.isValid(); parent = parent.parent())
        emit dataChanged(parent, parent, roles);
}

void TestModel::updateChildren(const QModelIndex &index, QVector<int> roles)
{
    TestItemBase *item = static_cast<TestItemBase*>(index.internalPointer());

    for (int i = 0; i < item->childCount(); i++) {
        QModelIndex child = index.child(i, index.column());
        emit dataChanged(child, child, roles);
        updateChildren(child, roles);
    }
}
