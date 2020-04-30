#ifndef SLICEPARTYCONTROLLER_H
#define SLICEPARTYCONTROLLER_H

#include "party.h"
#include "slice.h"
#include "slicecontrolwidget.h"
#include "votewidget.h"
#include <vector>

class SlicePartyController : public QObject {
    Q_OBJECT

private:
    SliceControlWidget* scw;
    VoteWidget* vw;
    std::vector<Party *> parties = {};
    Slice* selectedSlice;
    int votingYes;
    int votingNo;

public:
    SlicePartyController();

    Party* createParty(QString name, Orientation o, int members);
    Party* copyParty(Party* other);
    Party* createMixedGroup(int members);

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
    bool findMixedGroup(vector<Party *>::iterator &it);


    //funzioni di controllo del vettore//
    vector<Party *>::iterator getPartiesBegin();
    void pushBackParty(Party* p);
    unsigned int getPartiesSize();
    Party* getPartyAt(int i);
    void erasePartyAt(int i);

    ~SlicePartyController();


signals:
    void partyCreated(Party* party);


public Q_SLOTS:
    void showBorder(bool show, Slice* slice);
    void changeCursor(bool change);
    void explodeSlice(bool explode, Slice* slice);
    void removePartySelected();
    void updatePercentages();
    void updateMixedGroupMembers(int membersToAdd);
    void handleSliceClicked(Slice* slice);
    void releaseLockedSlice(Slice* slice);
    void handleSliceClickedDuringVote(Slice* slice);
};

#endif // SLICEPARTYCONTROLLER_H
