#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh" // 필요시 무작위 시작 위치 등 설정

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
    G4int n_particle = 1; // 이벤트 당 생성할 입자 수
    fParticleGun = new G4ParticleGun(n_particle);

    // --- 기본 입자 설정 ---
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("gamma"); // 기본값: 감마
    fParticleGun->SetParticleDefinition(particle);

    // --- 기본 에너지 설정 ---
    fParticleGun->SetParticleEnergy(1.0 * MeV);

    // --- 기본 위치 및 방향 설정 ---
    // 예시: 월드 볼륨 가장자리에서 중심으로 발사
    G4double worldSize = 10.0 * um; // DetectorConstruction과 동일한 값 사용 필요
    G4double pos_z = -0.5 * worldSize; // Z축 음의 방향에서 시작
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., pos_z));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.)); // Z축 양의 방향으로 발사
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // 각 이벤트마다 호출됨
    // 필요시 이벤트마다 위치, 방향, 에너지를 무작위로 변경 가능
    // G4double x0 = (G4UniformRand() - 0.5) * 1*um;
    // G4double y0 = (G4UniformRand() - 0.5) * 1*um;
    // fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, -0.5 * 10.0 * um));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

// 외부에서 입자 타입을 변경하는 함수
void PrimaryGeneratorAction::SetParticleType(const G4String& type)
{
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(type);

    if (particle) {
        fParticleGun->SetParticleDefinition(particle);
        G4cout << "Primary particle type set to: " << type << G4endl;
    } else {
        G4cerr << "ERROR: Particle type '" << type << "' not found!" << G4endl;
    }
}