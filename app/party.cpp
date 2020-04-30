#include "party.h"
#include "slice.h"

// --- CONSTRUCTORS --- //
Party::Party(){
    this->name = "Unknown";
    this->orientation = CENTRE;
    this->members = 0;
    this->owner = "Unknown";
    this->consensus = 0;
    this->isMixedGroup = false;
}

Party::Party(QString name, Orientation o, int members, QString owner, int consensus, bool isMixedGroup) {
    this->name = name;
    this->orientation = o;
    this->members = members;
    this->owner = owner;
    this->consensus = consensus;
    this->isMixedGroup = isMixedGroup;
}

Party::Party(bool isMixedGroup){
    if(!isMixedGroup)
        Party();
    else {
        this->name = "Mixed Group";
        this->orientation = NONE;
        this->members = 0;
        this->owner = "";
        this->consensus = 0;
        this->isMixedGroup = true;
    }
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
    this->slice->setLabel(name);
}

void Party::setOwner(const QString &owner) {
    Party::owner = owner;
}

void Party::setMembers(int members) {
    Party::members = members;
    if(slice != nullptr)
        this->slice->setValue(members);
}

void Party::setConsensus(int consensus) {
    Party::consensus = consensus;
}

void Party::setOrientation(Orientation orientation) {
    Party::orientation = orientation;
}

void Party::setIsMixedGroup(const bool itIs){
    Party::isMixedGroup = itIs;
}

bool Party::getIsMixedGroup() const {
    return this->isMixedGroup;
}

// --- DESTRUCTOR --- //
Party::~Party(){

}
