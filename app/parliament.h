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
    unsigned int parliamentMembers;
    unsigned int numOfPlayers;

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
    Parliament(unsigned int parliamentMembers = 100, unsigned int numOfPlayers = 1);

    QChartView* getChartView() const{return chartview;}
    SliceControlWidget* getSliceControlWidget() const {return scw;}

    void transportSlice(Slice* sliceToMove, int pos);
    void connectStuffing(Slice* slice);
    int placeSliceFromOrientation(Slice* slice);


signals:
    void totalMembersModified();


public Q_SLOTS:
    void addPartyButtonClicked();
    int replaceSliceFromOrientation(Slice* s);
    void insertSlice(Party * party);
    void removeSliceWhileSelected(Slice* slice);
    void removeSliceWhileNotSelected(Slice* slice);
    void startVotingProcess();
    void showResultEndExitVotingProcess();
    void exitVotingProcess();
};

#endif // PARLIAMENT_H
