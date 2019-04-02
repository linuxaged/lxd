#include <QtTest>
#include <QCoreApplication>

#include "../src/smallvector.h"

class TestSmallVector : public QObject
{
    Q_OBJECT

public:
    TestSmallVector();
    ~TestSmallVector();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_push_back();

};

TestSmallVector::TestSmallVector()
{

}

TestSmallVector::~TestSmallVector()
{

}

void TestSmallVector::initTestCase()
{

}

void TestSmallVector::cleanupTestCase()
{

}

void TestSmallVector::test_push_back()
{
    lxd::SmallVector<int, 2> vec;
    vec.push_back(100);
    vec.push_back(99);
    QVERIFY(vec[0] == 100);
    QVERIFY(vec[1] == 99);
    QVERIFY(vec.size() == 2);
}

QTEST_MAIN(TestSmallVector)

#include "tst_smallvector.moc"
