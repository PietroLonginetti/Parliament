#ifndef PARTY_H
#define PARTY_H

#include <QString>

using namespace std;
class Slice;
enum Orientation {RADICAL_LEFT, LEFT, CENTRE_LEFT, CENTRE, CENTRE_RIGHT, RIGHT, RADICAL_RIGHT};

class Party {

private:
    Slice* slice;

    QString name;
    Orientation orientation;
    int members;
    QString owner;
    int consensus;


public:
    Party(QString name, Orientation o, int members, QString owner = "Unknown", int consensus = 0);

    Slice* getSlice() const;
    void setSlice(Slice* slice);
    const QString &getName() const;
    const QString &getOwner() const;
    int getMembers() const;
    int getConsensus() const;
    Orientation getOrientation() const;
    void setName(const QString &name);
    void setOwner(const QString &owner);
    void setMembers(int members);
    void setConsensus(int consensus);
    void setOrientation(Orientation orientation);

    ~Party();
};

#endif // PARTY_H
