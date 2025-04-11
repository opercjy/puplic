#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

// Accumulable Manager 포함
#include "G4AccumulableManager.hh"

// Analysis Manager 포함 (파일 출력용)
// #include "g4analysis.hh"

RunAction::RunAction()
 : G4UserRunAction(),
   fTotalDnaEdep(0.0) // Accumulable 초기화
{
    // Accumulable 등록
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->RegisterAccumulable(fTotalDnaEdep);

    // --- Analysis Manager 설정 (예: ROOT 파일 출력) ---
    // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // analysisManager->SetVerboseLevel(1);
    // analysisManager->SetNtupleMerging(true); // MT 모드에서 Ntuple 병합 활성화

    // // 파일 이름 설정
    // analysisManager->SetFileName("NuclearMedicineDNA_Results");

    // // Ntuple 생성 (예: DNA 이벤트별 정보)
    // analysisManager->CreateNtuple("DNAHits", "DNA Hit Information");
    // analysisManager->CreateNtupleIColumn("EventID"); // 이벤트 ID
    // analysisManager->CreateNtupleIColumn("DNACopyNo"); // 맞은 DNA 번호
    // analysisManager->CreateNtupleDColumn("Edep");      // 에너지 침적량 (keV)
    // analysisManager->CreateNtupleDColumn("PosX");      // 위치 X (nm)
    // analysisManager->CreateNtupleDColumn("PosY");      // 위치 Y (nm)
    // analysisManager->CreateNtupleDColumn("PosZ");      // 위치 Z (nm)
    // analysisManager->FinishNtuple();

    // // 히스토그램 생성 (예: DNA당 총 에너지 침적 분포)
    // analysisManager->CreateH1("TotalEdepPerDNA","Total Edep per DNA segment", 100, 0., 10*keV); // 0 ~ 10 keV 범위, 100개 빈
}

RunAction::~RunAction()
{
    // delete G4AnalysisManager::Instance(); // 싱글톤이므로 직접 삭제하지 않음
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    // Accumulable 초기화
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();

    // // Analysis Manager 파일 열기
    // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    // analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    // 모든 스레드의 Accumulable 값 병합 (Master 스레드에서만 실행됨)
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    // 결과 출력 (Master 스레드에서만)
    if (IsMaster()) {
        G4int nofEvents = aRun->GetNumberOfEvent();
        if (nofEvents == 0) return;

        // 병합된 Accumulable 값 가져오기
        G4double totalDnaEdep = fTotalDnaEdep.GetValue();

        G4cout << "--------------------End of Run-----------------------" << G4endl;
        G4cout << " The run consisted of " << nofEvents << " events." << G4endl;
        G4cout << " Total energy deposited in all DNA segments: "
               << G4BestUnit(totalDnaEdep, "Energy") << G4endl;
        G4cout << " Average energy deposited per event in DNA: "
               << G4BestUnit(totalDnaEdep / nofEvents, "Energy") << G4endl;
        G4cout << "-----------------------------------------------------" << G4endl;

        // // Analysis Manager 결과 저장 및 파일 닫기
        // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        // analysisManager->Write();
        // analysisManager->CloseFile();
    }
}

// Accumulable 값을 업데이트하는 함수 (SteppingAction 등에서 호출)
void RunAction::AddDnaEdep(G4double edep)
{
    fTotalDnaEdep += edep;
}