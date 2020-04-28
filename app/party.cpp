#include "party.h"
#include "slice.h"

// --- CONSTRUCTORS --- //
Party::Party(QString name, Orientation o, int members, QString owner, int consensus) {
    this->name = name;
    this->orientation = o;
    this->members = members;
    this->owner = owner;
    this->consensus = consensus;
}

// --- GETTER & SETTERS --- //
Slice* Party::getSlice() const {
    return this->slice;
}

void Party::setSlice(Slice *slice){
    this->slice = slice;
}

const QString &Party::getName() const {
    return name;
}

const QString &Party::getOwner() const {
    return owner;
}

int Party::getMembers() const {
    return members;
}

int Party::getConsensus() const {
    return consensus;
}

Orientation Party::getOrientation() const {
    return orientation;
}

void Party::setName(const QString &name) {
    Party::name = name;
}

void Party::setOwner(const QString &owner) {
    Party::owner = owner;
}

void Party::setMembers(int members) {
    Party::members = members;
}

void Party::setConsensus(int consensus) {
    Party::consensus = consensus;
}

void Party::setOrientation(Orientation orientation) {
    Party::orientation = orientation;
}

// --- DESTRUCTOR --- //
Party::~Party(){

}
