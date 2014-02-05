#ifndef TESTITEM_H
#define TESTITEM_H

#include <QList>
#include <QVariant>

class TestModel;

class TestItem
{
public:
    enum TestStates {
        StateNone = 0,
        StatePass,
        StateFail,
        StatePartialPass
    };

    TestItem(QString name, bool enabled, TestItem *parent);
    TestItem(TestModel *model);
    ~TestItem();

    void appendChild(TestItem *child);
    void removeChild(TestItem *child);

    TestItem *child(int row);
    int childCount() const;
    int columnCount() const;
    bool isCheckbox(int column) const;
    QVariant bgColor(int column) const;
    QVariant checkState(int column) const;
    QVariant data(int column) const;
    int row() const;
    TestItem *parent() const;
    TestModel *model() const;

    Qt::CheckState enabled() const;
    void setEnabled(bool enabled);
    TestStates state() const;

private:
    QList<TestItem*> _childItems;

    QString _name;
    bool _enabled;
    TestStates _state;

    TestItem *_parentItem;
    TestModel *_model;
};

#endif // TESTITEM_H
