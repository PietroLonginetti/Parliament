#include <QtTest>

// add necessary includes here

class testParliament : public QObject
{
    Q_OBJECT

public:
    testParliament();
    ~testParliament();

private slots:

};

testParliament::testParliament()
{

}

testParliament::~testParliament()
{

}


QTEST_APPLESS_MAIN(testParliament)

#include "tst_testparliament.moc"
