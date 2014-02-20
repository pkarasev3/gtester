#ifndef TESTITEMROOT_H
#define TESTITEMROOT_H

#include "testitembase.h"

class TestItemRoot : public TestItemBase
{
public:
    TestItemRoot(TestModel *model);
    virtual ~TestItemRoot();

    virtual QVariant getData(int column) const;

    virtual void save(FileFormatBase *to);
    virtual void saveChildren(FileFormatBase *to);

    virtual void read(FileFormatBase *from);

};

#endif // TESTITEMROOT_H
