#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class PhysicsList : public G4VModularPhysicsList
{
public:
    PhysicsList();
    virtual ~PhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

private:
    // GEANT4-DNA 물리 프로세스 생성자 옵션 (예: opt6)
    G4String fEmDNAPhysicsList; // 생성자에서 설정
};

#endif