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
    //delete mAnalysis;
}

void STcounterSD::Initialize(G4HCofThisEvent *pThisHC)
{
    if (!mAnalysis) 
    {
        G4String filename = "out.root";
        mAnalysis = STanalysis::getPointer(filename);
    }
}

void STcounterSD::EndOfEvent(G4HCofThisEvent *pThisHC)
{
    if (hitCount > 0) 
    {
        // save/write root files etc
//        printf("\n++++++++++++++++++++\n");
//        printf("name: %s\n", this->SensitiveDetectorName.data());
//        printf("this count: %i\n", hitCount);
//        printf("total count: %i\n", totalCount);
//        printf("++++++++++++++++++++\n");
    }
}

G4bool STcounterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    // we want to record the first hit each event
    // first check that the partice is entering the detector
    // then count it
//    G4cout << "HERE I AM" << G4endl << G4endl << G4endl << G4endl;
    G4StepPoint* point = aStep->GetPreStepPoint();
    const G4ParticleDefinition* particle = 
                            aStep->GetTrack()->GetParticleDefinition();
    G4String particle_name = particle->GetParticleName();
//    G4cout << particle_name << G4endl;
    G4bool optical_photon = (particle_name == "opticalphoton");
    
    if (optical_photon && (point->GetStepStatus() == fGeomBoundary))
    {
        G4float pos [3];
        pos[0] = point->GetPosition().x();
        pos[1] = point->GetPosition().y();
        pos[2] = point->GetPosition().z();
        G4float time = point->GetGlobalTime();
        mAnalysis->addHit(pos, time);
        
        ++hitCount;
        ++totalCount;
        return true;
    } 
    return false;
}