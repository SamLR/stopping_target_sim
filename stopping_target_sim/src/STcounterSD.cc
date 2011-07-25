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
}

void STcounterSD::EndOfEvent(G4HCofThisEvent *pThisHC)
{
    if (hitCount > 0) 
    {
        // save/write root files etc
        printf("\n++++++++++++++++++++\n");
        printf("name: %s\n", this->SensitiveDetectorName.data());
        printf("this count: %i\n", hitCount);
        printf("total count: %i\n", totalCount);
        printf("++++++++++++++++++++\n");        
    }
}

G4bool STcounterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // we want to record the first hit each event
    // first check that the partice is entering the detector
    // then count it
    G4cout << "HERE I AM" << G4endl << G4endl << G4endl << G4endl;
    G4StepPoint* point = aStep->GetPreStepPoint();
    const G4ParticleDefinition* particle = 
                            aStep->GetTrack()->GetParticleDefinition();
    G4String particle_name = particle->GetParticleName();
    G4cout << particle_name << G4endl;
//    G4bool optical_photon = (particle_name == "opticalphoton");
//    
//    if (optical_photon && (point->GetStepStatus() == fGeomBoundary))
//    {
//        ++hitCount;
//        ++totalCount;
//        return true;
//    } 
    return false;
}