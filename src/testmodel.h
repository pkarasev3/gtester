#ifndef TESTMODEL_H
#define TESTMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QVector>

class ExecutableBase;
class TestItemBase;
class TestItemRoot;
class TestItemExecutable;
class FileFormatBase;

class TestModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TestModel(QObject *parent = 0);
    ~TestModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(TestItemBase *item, int column = 0) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool isClean();
    bool isChanged();
    bool isRunning();
    TestItemRoot *rootItem();

    void save(FileFormatBase *to);
    void read(FileFormatBase *from);

signals:
    void started();
    void finished();
    void terminated(QString msg);
    void executionStateChanged(bool running);
    void progressUpdated(int percent);
    void testStarted(const QString &test);
    void changed();

public slots:
    void execute();
    void terminate();
    void addExecutable(QString fileName);
    void removeExecutable(const QModelIndex index);
    void removeExecutable(TestItemBase *item);
    void removeExecutable(TestItemExecutable *item);
    void refresh(const QModelIndex &index = QModelIndex());
    void refresh(TestItemBase *item);
    void refresh(TestItemExecutable *item);
    void clear();
    bool getShuffle();
    void setShuffle(bool shuffle);

    void selectAll();
    void deselectAll();
    void invertSelection();


private slots:
    void jobExecuted();
    void jobFinished();
    void executeNextJob();
    void change(bool c = true);

private:
    TestItemRoot *_rootItem;
    bool _clean;
    bool _changed;
    bool _running;
    QList<ExecutableBase*> _pendingJobs;

    int _runningTestCount;
    int _completedTestCount;

    void queueJob(ExecutableBase *job);

    void update(const QModelIndex &index, QVector<int> roles = QVector<int>());
    void updateParents(const QModelIndex &index, QVector<int> roles = QVector<int>(), int column = 0);
    void updateChildren(const QModelIndex &index, QVector<int> roles = QVector<int>());
    void updateRunningTest(const QString &test);

    friend class TestItemBase;
    friend class TestItem;
    friend class ExecutableTestRunner;
};

#endif // TESTMODEL_H
