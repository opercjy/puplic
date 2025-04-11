#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

// --- Physics Constructors ---
// Particles
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

// EM Physics (Standard for higher energies outside DNA focus, if needed)
#include "G4EmStandardPhysics.hh"

// *** GEANT4-DNA Physics ***
#include "G4EmDNAPhysics.hh"
// 다른 옵션들도 사용 가능 (opt0, opt2, opt3, opt4, opt6 등)
// #include "G4EmDNAPhysics_opt0.hh"
// #include "G4EmDNAPhysics_opt2.hh"
// #include "G4EmDNAPhysics_opt4.hh"
// #include "G4EmDNAPhysics_opt6.hh"

PhysicsList::PhysicsList()
 : G4VModularPhysicsList()
{
    SetVerboseLevel(1);

    // GEANT4-DNA 물리 프로세스 생성자 선택
    // 옵션 (opt0, opt2, opt4, opt6 등) 에 따라 다른 모델 사용
    // 예시: G4EmDNAPhysics_opt6 사용
    fEmDNAPhysicsList = "G4EmDNAPhysics_opt6"; // 이 이름으로 생성자를 찾음

    // 필요시 표준 EM 물리 추가 (고에너지 영역 커버)
    // RegisterPhysics(new G4EmStandardPhysics());

    // GEANT4-DNA 물리 등록
    RegisterPhysics(new G4EmDNAPhysics(1, fEmDNAPhysicsList)); // 첫 번째 인자는 verbosity level

    G4cout << "Using GEANT4-DNA Physics: " << fEmDNAPhysicsList << G4endl;
}

PhysicsList::~PhysicsList()
{ }

void PhysicsList::ConstructParticle()
{
    // 필요한 입자들을 생성
    G4BosonConstructor pBosonConstructor;
    pBosonConstructor.ConstructParticle();

    G4LeptonConstructor pLeptonConstructor;
    pLeptonConstructor.ConstructParticle();

    G4MesonConstructor pMesonConstructor;
    pMesonConstructor.ConstructParticle();

    G4BaryonConstructor pBaryonConstructor;
    pBaryonConstructor.ConstructParticle();

    G4IonConstructor pIonConstructor;
    pIonConstructor.ConstructParticle();

    G4ShortLivedConstructor pShortLivedConstructor;
    pShortLivedConstructor.ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
    // Base class G4VModularPhysicsList의 ConstructProcess 호출 필수
    AddTransportation();

    // 등록된 물리 생성자들의 ConstructProcess 호출
    // (예: G4EmDNAPhysics 의 ConstructProcess 가 여기서 호출됨)
    G4VModularPhysicsList::ConstructProcess();

    G4cout << "PhysicsList::ConstructProcess() completed." << G4endl;
}