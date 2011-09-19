// 
// 
// A sensitive dector implementation
// this should be accessed whenever
// a particle enters counterOne
// 
// scook 20-06-10
//

#include "STcounterSD.hh"
#include "STanalysis.hh"
#include "G4SDManager.hh"

STcounterSD::STcounterSD(G4String name) : G4VSensitiveDetector(name) 
{
    G4String HCname = name + "CounterCollection";
    collectionName.insert(HCname);
}

STcounterSD::~STcounterSD(){;}

void STcounterSD::Initialize(G4HCofThisEvent *pThisHC)
{
    counterCollection = new STcounterHitsCollection(SensitiveDetectorName, 
                                                    collectionName[0]);
    static G4int HCID = -1; // test if this gets assigned on subsequent calls
    if (HCID < 0) 
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    pThisHC->AddHitsCollection(HCID, counterCollection);
}

void STcounterSD::EndOfEvent(G4HCofThisEvent *pThisHC) 
{
    STanalysis* analysis = STanalysis::getInitdPointer();
    G4int nHits = counterCollection->entries();
    
    for (int i = 0; i<nHits; ++i) 
    {
        G4ThreeVector position = (*counterCollection)[i]->GetPos();
        G4float time = (*counterCollection)[i]->GetTime();
        analysis->addHit(position, time);
    }
    analysis->close();
}

G4bool STcounterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // add checks that first hit etc
    G4StepStatus stepStatus = aStep->GetPreStepPoint()->GetStepStatus();
    
    if (not stepStatus == fGeomBoundary) return false;
    
    STcounterHit *newHit = new STcounterHit();
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    newHit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());
    counterCollection->insert(newHit);
    return true;
}