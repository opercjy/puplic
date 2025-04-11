#include "EventAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

// Analysis Manager 포함 (파일 출력용)
// #include "g4analysis.hh"

EventAction::EventAction()
: G4UserEventAction()
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* /*anEvent*/)
{
    // 이벤트 시작 시 호출
    // DNA 히트 컬렉션 초기화
    fDNAHitsCollection.clear();
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    // 이벤트 종료 시 호출

    // 현재 이벤트에서 수집된 DNA 히트 정보를 처리
    // 예: Analysis Manager를 이용하여 Ntuple에 저장

    // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // G4int eventID = anEvent->GetEventID();

    // for (const auto& hit : fDNAHitsCollection) {
    //     analysisManager->FillNtupleIColumn(0, eventID);        // Event ID
    //     analysisManager->FillNtupleIColumn(1, hit.dnaCopyNo);  // DNA Copy No
    //     analysisManager->FillNtupleDColumn(2, hit.edep / keV); // Edep (keV)
    //     analysisManager->FillNtupleDColumn(3, hit.posX / nm);  // Pos X (nm)
    //     analysisManager->FillNtupleDColumn(4, hit.posY / nm);  // Pos Y (nm)
    //     analysisManager->FillNtupleDColumn(5, hit.posZ / nm);  // Pos Z (nm)
    //     analysisManager->AddNtupleRow(); // Ntuple 행 추가
    // }

    // 필요시 이벤트별 결과 출력
    // G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    // if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    //     G4cout << "---> End of event: " << eventID << G4endl;
    //     G4cout << "     Number of DNA hits in this event: " << fDNAHitsCollection.size() << G4endl;
    // }
}

// SteppingAction에서 호출되어 히트 정보 추가
void EventAction::AddDNAHit(G4int copyNo, G4double edep, G4ThreeVector pos)
{
    DNAHit hit;
    hit.dnaCopyNo = copyNo;
    hit.edep = edep;
    hit.posX = pos.x();
    hit.posY = pos.y();
    hit.posZ = pos.z();
    fDNAHitsCollection.push_back(hit);
}