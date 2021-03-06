
//
//  STMonitorHit.cc
//  stopping_target_sim
//
//  Created by Sam Cook on 30/01/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef STMonitorHit_hh
#define STMonitorHit_hh 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class STMonitorHit : public G4VHit
{
public:
    
    STMonitorHit();
    ~STMonitorHit();
    STMonitorHit(const STMonitorHit&);
    
    const STMonitorHit& operator=(const STMonitorHit&);
    G4int operator==(const STMonitorHit&) const;
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    void Draw();
    void Print();
    
    // Setters
    inline void SetPos (G4ThreeVector xyz) { pos = xyz; };
    inline void SetMom (G4ThreeVector Pxyz) { mom = Pxyz; };
    inline void SetPid (G4int p) { pid = p; };
    inline void SetTime (G4float t) {time = t; };
    inline void SetEvent (G4int e) {eventNo = e; };
    inline void SetParentID (G4int p) {parentID = p; };
    inline void SetTrackID (G4int t) {trackID = t; };
    // Getters
    inline G4ThreeVector GetPos() { return pos; };
    inline G4ThreeVector GetMom() { return mom; };
    inline G4float GetTime() {return time; };
    inline G4int GetEvent() {return eventNo; };
    inline G4int GetPID() {return pid; };
    inline G4int GetParentID() {return parentID; };
    inline G4int GetTrackID() {return trackID; };
    
private:
    G4ThreeVector pos, mom;
    G4float time;
    G4int eventNo, pid, parentID, trackID;
    
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// make a hits collection (templated)
typedef G4THitsCollection<STMonitorHit> STMonitorHitsCollection;
// allocator defined in .cc
extern G4Allocator<STMonitorHit> STMonitorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* STMonitorHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) STMonitorHitAllocator.MallocSingle();
    return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void STMonitorHit::operator delete(void *aHit)
{
    STMonitorHitAllocator.FreeSingle((STMonitorHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
