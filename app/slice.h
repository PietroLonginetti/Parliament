#ifndef SLICE_H
#define SLICE_H

#include <QtCharts/QPieSlice>
#include "party.h"

QT_CHARTS_USE_NAMESPACE

class Slice : public QPieSlice {
    Q_OBJECT

private:
    Party* party;


public:
    Slice(Party* party);

    Party* getParty() const {return party;}

    void setColorFromOrientation(Orientation o);


signals:
    void sliceHovered(bool isHovered, Slice* slice);
    void sliceClicked(Slice* slice);
    void orientationChanged(Slice* slice);
    void sliceRemoving(Slice * slice);


public slots:
    void onHovered(bool isHovered);
    void onClicked();
    void updateName(const QString& text);
    void updateOwner(const QString& text);
    void updateOrientation(int index);
    void updateMembers(int members);
    void updateConsensus(int points);
    void removeYourself();
};

#endif // SLICE_H
