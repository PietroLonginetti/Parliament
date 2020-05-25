#include <QtTest>
#include <QApplication>

// add necessary includes here
#include "../app/mainwindow.h"
#include "../app/parliament.h"
#include "../app/party.h"
#include "../app/slice.h"
#include "../app/slicecontrolwidget.h"
#include "../app/slicepartycontroller.h"
#include "../app/votewidget.h"

class testParliament : public QObject
{
    Q_OBJECT

public:
    testParliament();
    ~testParliament();

private slots:
    void testParliamentConstruction_0Players();
    void testParliamentConstruction_1Players();
    void testParliamentConstruction_2Players();
    void testParliamentConstruction_3Players();
    void testParliamentConstruction_4Players();
    void testParliamentConstruction_5Players();
    void testParliamentConstruction_6Players();
    void testParliamentConstruction_7Players();

    void testLegalAssignment();
    void testIllegalAssignment();

    void testAllVotingYes();
    void testAllVotingNo();
    void testParityRestults();
    void testVoteOverwrite();

private:
    QApplication * a;
    MainWindow * w;
};

testParliament::testParliament() {
    int argc = 0;
    a = new QApplication(argc, nullptr);
}
testParliament::~testParliament(){
    delete w;
    delete a;
}

void testParliament::testParliamentConstruction_0Players(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(0);
    w->startGame();
    QCOMPARE(w->getParliament()->getSlices()->slices().at(0)->value(), 100);
}
void testParliament::testParliamentConstruction_1Players(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(1);
    w->startGame();
    QCOMPARE(w->getParliament()->getSlices()->slices().at(0)->value(), 50);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(1)->value(), 50);
}
void testParliament::testParliamentConstruction_2Players(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(2);
    w->startGame();
    QCOMPARE(w->getParliament()->getSlices()->slices().at(0)->value(), 33);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(1)->value(), 34);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(2)->value(), 33);
}
void testParliament::testParliamentConstruction_3Players(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(3);
    w->startGame();
    QCOMPARE(w->getParliament()->getSlices()->slices().at(0)->value(), 25);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(1)->value(), 25);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(2)->value(), 25);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(3)->value(), 25);
}
void testParliament::testParliamentConstruction_4Players(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(4);
    w->startGame();
    QCOMPARE(w->getParliament()->getSlices()->slices().at(0)->value(), 20);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(1)->value(), 20);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(2)->value(), 20);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(3)->value(), 20);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(4)->value(), 20);
}
void testParliament::testParliamentConstruction_5Players(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(5);
    w->startGame();
    QCOMPARE(w->getParliament()->getSlices()->slices().at(0)->value(), 16);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(1)->value(), 16);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(2)->value(), 16);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(3)->value(), 20);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(4)->value(), 16);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(5)->value(), 16);
}
void testParliament::testParliamentConstruction_6Players(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(6);
    w->startGame();
    QCOMPARE(w->getParliament()->getSlices()->slices().at(0)->value(), 14);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(1)->value(), 14);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(2)->value(), 14);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(3)->value(), 16);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(4)->value(), 14);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(5)->value(), 14);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(6)->value(), 14);

}
void testParliament::testParliamentConstruction_7Players(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(7);
    w->startGame();
    QCOMPARE(w->getParliament()->getSlices()->slices().at(0)->value(), 12);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(1)->value(), 12);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(2)->value(), 12);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(3)->value(), 12);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(4)->value(), 16);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(5)->value(), 12);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(6)->value(), 12);
    QCOMPARE(w->getParliament()->getSlices()->slices().at(7)->value(), 12);
}

void testParliament::testLegalAssignment(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(7);
    w->startGame();

    for(int i = 0; i< w->getParliament()->getSlices()->count(); i++){
        if(!static_cast<Slice*>(w->getParliament()->getSlices()->slices().at(i))->getParty()->getIsMixedGroup()){
            QPieSlice* selectedSlice = w->getParliament()->getSlices()->slices().at(i);
            selectedSlice->clicked();

            unsigned int initialMembers = selectedSlice->value();
            w->getParliament()->getSPC()->getControlWidget()->getUi()->membersSpinBox->stepBy(16);
            QCOMPARE(selectedSlice->value(), initialMembers + 16);

            w->getParliament()->getSPC()->getControlWidget()->getUi()->membersSpinBox->stepBy(-16);
            QCOMPARE(selectedSlice->value(), initialMembers);
        }
    }
}
void testParliament::testIllegalAssignment(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(7);
    w->startGame();

    for(int i = 0; i< w->getParliament()->getSlices()->count(); i++){
        if(!static_cast<Slice*>(w->getParliament()->getSlices()->slices().at(i))->getParty()->getIsMixedGroup()){
            QPieSlice* selectedSlice = w->getParliament()->getSlices()->slices().at(i);
            selectedSlice->clicked();

            unsigned int initialMembers = selectedSlice->value();
            w->getParliament()->getSPC()->getControlWidget()->getUi()->membersSpinBox->stepBy(100);  //illegal assigment: spinbox members threshold is "initial members"+ 16
            QCOMPARE(selectedSlice->value(), initialMembers + 16);

            w->getParliament()->getSPC()->getControlWidget()->getUi()->membersSpinBox->stepBy(-16);
            QCOMPARE(selectedSlice->value(), initialMembers);

            w->getParliament()->getSPC()->getControlWidget()->getUi()->membersSpinBox->stepBy(17);  //illegal assigment: spinbox members threshold is "initial members"+ 16
            QCOMPARE(selectedSlice->value(), initialMembers + 16);

            w->getParliament()->getSPC()->getControlWidget()->getUi()->membersSpinBox->stepBy(-16);
            QCOMPARE(selectedSlice->value(), initialMembers);
        }
    }
}

void testParliament::testAllVotingYes(){
    w = new MainWindow;
    w->getUi()->membersSpinBox->setValue(100);
    w->getUi()->playersSpinBox->setValue(4);
    w->startGame();


    w->getParliament()->startVotingProcess();
    w->getParliament()->getVoteWidget()->getUi()->yesButton->setChecked(true);
    for(int i = 0; i < w->getParliament()->getSlices()->count(); i++){
        w->getParliament()->getSlices()->slices().at(i)->clicked();
    }
    QCOMPARE(w->getParliament()->getSPC()->getVotingYes(), 80);
    w->getParliament()->getSPC()->resetVotingParameters();
}
void testParliament::testAllVotingNo(){
    w->getParliament()->startVotingProcess();
    w->getParliament()->getVoteWidget()->getUi()->noButton->setChecked(true);
    for(int i = 0; i < w->getParliament()->getSlices()->count(); i++){
        w->getParliament()->getSlices()->slices().at(i)->clicked();
    }
    QCOMPARE(w->getParliament()->getSPC()->getVotingNo(), 80);
    w->getParliament()->getSPC()->resetVotingParameters();
}
void testParliament::testParityRestults(){
    w->getParliament()->startVotingProcess();
    w->getParliament()->getVoteWidget()->getUi()->yesButton->setChecked(true);
    for(int i = 0; i < (w->getParliament()->getSlices()->count())/2; i++){
        w->getParliament()->getSlices()->slices().at(i)->clicked();
    }
    w->getParliament()->getVoteWidget()->getUi()->noButton->setChecked(true);
    for(int i = (w->getParliament()->getSlices()->count())/2; i < w->getParliament()->getSlices()->count(); i++){
        w->getParliament()->getSlices()->slices().at(i)->clicked();
    }
    QCOMPARE(w->getParliament()->getSPC()->getVotingYes(), 40);
    QCOMPARE(w->getParliament()->getSPC()->getVotingNo(), 40);
    w->getParliament()->getSPC()->resetVotingParameters();
}
void testParliament::testVoteOverwrite(){
    w->getParliament()->startVotingProcess();
    w->getParliament()->getVoteWidget()->getUi()->yesButton->setChecked(true);
    for(int i = 0; i < w->getParliament()->getSlices()->count(); i++){
        w->getParliament()->getSlices()->slices().at(i)->clicked();
    }
    QCOMPARE(w->getParliament()->getSPC()->getVotingYes(), 80);
    w->getParliament()->getVoteWidget()->getUi()->noButton->setChecked(true);
    for(int i = 0; i < w->getParliament()->getSlices()->count(); i++){
        w->getParliament()->getSlices()->slices().at(i)->clicked();
    }
    QCOMPARE(w->getParliament()->getSPC()->getVotingNo(), 80);
    w->getParliament()->getVoteWidget()->getUi()->absteinedButton->setChecked(true);
    for(int i = 0; i < w->getParliament()->getSlices()->count(); i++){
        w->getParliament()->getSlices()->slices().at(i)->clicked();
    }
    QCOMPARE(w->getParliament()->getSPC()->getVotingYes(), 0);
    QCOMPARE(w->getParliament()->getSPC()->getVotingYes(), 0);
    w->getParliament()->getSPC()->resetVotingParameters();
}

QTEST_APPLESS_MAIN(testParliament)

#include "tst_testparliament.moc"
