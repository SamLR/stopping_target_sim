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

STcounterSD::STcounterSD(G4String name) : G4VSensitiveDetector(name) {;}

STcounterSD::~STcounterSD(){;}

void STcounterSD::Initialize(G4HCofThisEvent *pThisHC)
{
    // set up root here (when used)
    hitCount = 0;
}

void STcounterSD::EndOfEvent(G4HCofThisEvent *pThisHC)
{
    // save/write root files etc
    printf("++++++++++++++++++++");
    printf("name: %s", this->SensitiveDetectorName.data());
    printf("this count: %i", hitCount);
    printf("total count: %i", totalCount);
    printf("++++++++++++++++++++");
}

G4bool STcounterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // we want to record the first hit each event
    // first check that the partice is entering the detector
    // then count it
    G4StepPoint* point = aStep->GetPreStepPoint();
    
     if (point->GetStepStatus() == fGeomBoundary) 
     {
         ++hitCount;
         ++totalCount;
         return true;
     } 
     return false;
}