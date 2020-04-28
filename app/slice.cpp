#include "slice.h"

// --- CONSTRUCTORS --- //
Slice::Slice(Party* party){
    this->party = party;
    this->setLabel(party->getName());
    this->setValue(party->getMembers());

    connect(this, &Slice::hovered, this, &Slice::onHovered);
    connect(this, &Slice::clicked, this, &Slice::onClicked);

    this->setBorderWidth(2);
    this->setBorderColor(QColor("white"));

    QFont f;
    f.setBold(true);
    f.setPointSizeF(10);
    this->setLabelFont(f);
}

// --- FUNCTIONS --- //
void Slice::setColorFromOrientation(Orientation o){
    switch(o){
    case RADICAL_LEFT:
        this->setColor(QColor(114,0,30));
        break;
    case LEFT:
        this->setColor(QColor(227,66,52));
        break;
    case CENTRE_LEFT:
        this->setColor(QColor(252,192,197));
        break;
    case CENTRE:
        this->setColor(QColor(242,241,232));
        break;
    case CENTRE_RIGHT:
        this->setColor(QColor(13,105,139));
        break;
    case RIGHT:
        this->setColor(QColor(5,5,51));
        break;
    case RADICAL_RIGHT:
        this->setColor(QColor("black"));
    }
}


// --- SLOTS --- //
void Slice::onHovered(bool isHovered){
    emit this->sliceHovered(isHovered,this);
}

void Slice::onClicked(){
    if(this->label() != "Unknown")
        this->setLabelVisible();
    emit this->sliceClicked(this);
}

void Slice::updateName(const QString& text){
    this->setLabelVisible();
    party->setName(text);
    this->setLabel(text);
}

void Slice::updateOwner(const QString& text){
    party->setOwner(text);
}

void Slice::updateOrientation(int index){
    party->setOrientation((Orientation) index);
    setColorFromOrientation((Orientation) index);
    emit orientationChanged(this);
}

void Slice::updateMembers(int members){
    party->setMembers(members);
    this->setValue(members);
}

void Slice::updateConsensus(int points){
    party->setConsensus(points);
}

void Slice::removeYourself(){
    emit sliceRemoving(this);
}
