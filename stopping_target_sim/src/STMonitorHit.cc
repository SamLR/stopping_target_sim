//
//  STMonitorHit.hh
//  stopping_target_sim
//
//  Created by Sam Cook on 30/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include "STMonitorHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<STMonitorHit> STMonitorHitAllocator;

STMonitorHit::STMonitorHit() {;}

STMonitorHit::~STMonitorHit() {;}

STMonitorHit::STMonitorHit(const STMonitorHit& right) 
: G4VHit(), pos(right.pos), mom(right.mom), time(right.time), eventNo(right.eventNo), 
pid(right.pid), parentID(right.parentID), trackID(right.trackID)
{;}

const STMonitorHit& STMonitorHit::operator=(const STMonitorHit& right)
{
    pos = right.pos;
    mom = right.mom;
    time = right.time; 
    eventNo = right.eventNo;
    pid = right.pid;
    parentID = right.parentID;
    trackID = right.trackID;
    return *this;
}

G4int STMonitorHit::operator==(const STMonitorHit& right) const
{
    return (this==&right) ? 1 : 0;
}

void STMonitorHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
    {
        G4Circle circle(pos);
        circle.SetScreenSize(2.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,0.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

void STMonitorHit::Print() {;}

