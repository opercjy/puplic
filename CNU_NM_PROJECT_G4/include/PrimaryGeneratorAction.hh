#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* anEvent);

    // 입자 타입을 외부에서 설정하기 위한 함수 (예: UI command)
    void SetParticleType(const G4String& type);

private:
    G4ParticleGun* fParticleGun; // Geant4 기본 입자 총
};

#endif