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

STcounterSD::~STcounterSD()
{
}

void STcounterSD::Initialize(G4HCofThisEvent *pThisHC)
{
}

void STcounterSD::EndOfEvent(G4HCofThisEvent *pThisHC)
{
}

G4bool STcounterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
}