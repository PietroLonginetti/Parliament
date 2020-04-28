#ifndef SLICEPARTYCONTROLLER_H
#define SLICEPARTYCONTROLLER_H

#include "party.h"
#include "slice.h"
#include "slicecontrolwidget.h"
#include "votewidget.h"
#include <vector>

class SlicePartyController : public QObject {

private:
    std::vector<Party *> parties = {};
    SliceControlWidget* scw;
    VoteWidget* vw;
    Slice* selectedSlice = nullptr;

    int votingYes = 0;
    int votingNo = 0;


public:
    Party* createParty(QString name, Orientation o, int members);
    Party* createPartyByCopy(Party* other);

    std::vector<Party *> getParties() {return parties;}
    void setControlWidget(SliceControlWidget* scw) {this->scw = scw;}
    SliceControlWidget* getControlWidget() const {return this->scw;}
    void setVoteWidget(VoteWidget* vw) {this->vw = vw;}
    VoteWidget* getVoteWidget() const {return this->vw;}
    void setSelectedSlice(Slice* slice) {selectedSlice = slice;}
    Slice* getSelectedSlice() const {return selectedSlice;}
    int getVotingYes() const {return votingYes;}
    int getVotingNo() const {return votingNo;}

    bool votePassed();
    void resetVotingParameters();
    void refreshSliceControlWidget(Slice* slice);
    void disconnectControlWidget(Slice* slice);
    void connectControlWidget(Slice* slice);

    ~SlicePartyController();


signals:
    void refresh(Slice* slice);


public Q_SLOTS:
    void showBorder(bool show, Slice* slice);
    void changeCursor(bool change);
    void explodeSlice(bool explode, Slice* slice);
    void removeParty(Slice* slice);
    void updatePercentages();
    void handleSliceClicked(Slice* slice);
    void releaseLockedSlice(Slice* slice);
    void handleSliceClickedDuringVote(Slice* slice);
};

#endif // SLICEPARTYCONTROLLER_H
