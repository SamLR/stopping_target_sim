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

STcounterSD::STcounterSD(G4String name, G4String filename, G4String treename): 
    G4VSensitiveDetector(name), mEventNumber(0) 
{
    G4String HCname = name + "CounterCollection";
    collectionName.insert(HCname);
    mAnalysis = new STanalysis(filename, treename);
}

STcounterSD::~STcounterSD()
{
    mAnalysis->update();
    delete mAnalysis;
}

void STcounterSD::Initialize(G4HCofThisEvent *pThisHC)
{
    ++mEventNumber;
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
    G4int nHits = counterCollection->entries();
    
    for (int i = 0; i<nHits; ++i) 
    {
        G4ThreeVector position = (*counterCollection)[i]->GetPos();
        G4float time = (*counterCollection)[i]->GetTime();
        mAnalysis->addHit(mEventNumber, 0, 0, 0, position, G4ThreeVector(), time); // MPPC don't care about PID
    }
//    mAnalysis->update(); 
}

G4bool STcounterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // add checks that first hit etc
    G4StepStatus stepStatus = aStep->GetPreStepPoint()->GetStepStatus();
    
    if (not stepStatus == fGeomBoundary) return false;
    
    G4int pid = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
    
    if (pid != 0) return false; // only pay attention to optical photons
    
    STcounterHit *newHit = new STcounterHit();
    newHit->SetEvent(mEventNumber);    
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    newHit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());
    counterCollection->insert(newHit);
    return true;
}