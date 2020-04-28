#ifndef PARLIAMENT_H
#define PARLIAMENT_H

#include <QPieSeries>
#include <QtCharts>
#include "slicepartycontroller.h"
#include "slicecontrolwidget.h"
#include "votewidget.h"
#include "dialogvoteresults.h"


class Parliament : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout* layout;
    QPushButton* addPartyButton;
    QPushButton* voteButton;
    QHBoxLayout* chartControlWidget;
    VoteWidget* vw;
    QPieSeries* slices;
    QChart* chart;
    QChartView* chartview;
    SliceControlWidget* scw;
    DialogVoteResults* dvr;

    SlicePartyController* spc;


public:
    Parliament(int numOfPlayers = 7);

    QChartView* getChartView() const{return chartview;}
    SliceControlWidget* getSliceControlWidget() const {return scw;}

    void transportSlice(Slice* sliceToMove, int pos);
    void connectSliceStuffing(Slice* slice);


signals:
    void totalMembersModified();


public Q_SLOTS:
    int placeSliceFromOrientation(Slice* s);
    void insertSlice();
    void removeSlice(Slice* slice);
    void votingProcess();
    void endVotingProcess();
};

#endif // PARLIAMENT_H
