#include <QtTest>

// add necessary includes here

class parliament : public QObject
{
    Q_OBJECT

public:
    parliament();
    ~parliament();

private slots:

};

parliament::parliament()
{

}

parliament::~parliament()
{

}


QTEST_APPLESS_MAIN(parliament)

#include "tst_parliament.moc"
