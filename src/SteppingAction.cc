#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh" // 지오메트리 정보 접근 (필요시)
#include "RunAction.hh"           // RunAction 접근

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fRunAction(nullptr) // RunAction은 Run 시작 시 얻어옴
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    // RunAction 포인터 가져오기 (매 스텝마다 할 필요는 없지만, 안전하게)
    // RunManager 를 통해 현재 RunAction 가져오기
    if (!fRunAction) {
      fRunAction = static_cast<RunAction*>(
                     const_cast<G4UserRunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
    }


    // 스텝 정보를 가져옴
    G4double edepStep = aStep->GetTotalEnergyDeposit();

    // 에너지가 침적되지 않았으면 아무것도 안함
    if (edepStep <= 0.) return;

    // 현재 스텝이 일어난 볼륨(PreStepPoint 기준)
    G4LogicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // 현재 볼륨이 DNA 볼륨인지 확인 (예: 이름 비교)
    // DetectorConstruction에서 설정한 DNA 로지컬 볼륨 이름("DNA")과 비교
    if (volume->GetName() == "DNA")
    {
        // DNA 볼륨 내에서 에너지 침적이 발생함
        G4int copyNo = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(); // 어떤 DNA 조각인지 식별
        G4ThreeVector position = aStep->GetPreStepPoint()->GetPosition(); // 상호작용 위치

        // 1. EventAction에 히트 정보 전달 (Ntuple 저장 등 이벤트 단위 처리용)
        if (fEventAction) {
            fEventAction->AddDNAHit(copyNo, edepStep, position);
        }

        // 2. RunAction의 Accumulable에 에너지 침적량 추가 (런 전체 통계용)
        if (fRunAction) {
            fRunAction->AddDnaEdep(edepStep);
        }

        // 추가 분석:
        // - 어떤 프로세스에 의해 에너지가 침적되었는지 확인: aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()
        // - 입자 종류 확인: aStep->GetTrack()->GetDefinition()->GetParticleName()
    }
}