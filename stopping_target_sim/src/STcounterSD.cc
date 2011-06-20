// 
// 
// A sensitive dector implementation
// this should be accessed whenever
// a particle enters counterOne
// 
// scook 20-06-10
//

#include "STcounterSD.hh"

G4int STcounterSD::totalCount = 0; // initialise the static variable

STcounterSD::STcounterSD(G4String name) : G4VSensitiveDetector(name) 
{
    G4String HCname;
    collectionName.insert(HCname="counterCollection"); 
    // collectionName is an inherited attribute that must store HC names
    // not sure about the bizare assignment & pass form though...
}

STcounterSD::~STcounterSD(){;}

void STcounterSD::Initialize(G4HCofThisEvent *pThisHC)
{
    // sets up the hits collections etc; Gods alone know what they actually do
    // but they seem to be vectors of values
    counterCollection = new STcounterHitsCollection(SensitiveDetectorName, collectionName[0]);
    
    static G4int HCID = -1;
    if (HCID < 0)
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    pThisHC->AddHitsCollection(HCID, counterCollection);
}

void STcounterSD::EndOfEvent(G4HCofThisEvent *pThisHC)
{
    G4cout << "current numbers of hits:" << totalCount << G4endl;     
}

G4bool STcounterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // we want to record the first hit each event
    // first check that the partice is entering the detector
    // then count it
    G4StepPoint* point = aStep->GetPreStepPoint();
    
     if (point->GetStepStatus() == fGeomBoundary) 
     {
         ++totalCount; 
         STcounterHit* newHit = new STcounterHit();
         newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
         newHit->SetPos      (aStep->GetPostStepPoint()->GetPosition());
         newHit->SetEdep     (5); // this probably won't be used
         counterCollection->insert( newHit );
         return true;
     } 
     return false;
}