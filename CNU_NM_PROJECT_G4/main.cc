#include "G4Types.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4EmParameters.hh" // DNA 파라미터 접근을 위해

int main(int argc, char** argv)
{
    // UI 세션 선택 (argc==1 이면 GUI, 아니면 Batch)
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // 기본 RunManager 선택 (멀티스레딩 지원)
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    // 시스템의 코어 수만큼 스레드 사용 (필요시 조절)
    runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    // 필수 사용자 클래스 등록
    // 1. Detector Construction
    runManager->SetUserInitialization(new DetectorConstruction());

    // 2. Physics List (GEANT4-DNA 사용)
    runManager->SetUserInitialization(new PhysicsList());
    // --- GEANT4-DNA 관련 설정 추가 가능 ---
    // 예: G4EmParameters::Instance()->SetAugerCascade(true); // 오제 효과 활성화
    // G4EmParameters::Instance()->SetDNAWizard(true); // DNA 관련 옵션 자동 설정 시도

    // 3. User Action Initialization
    runManager->SetUserInitialization(new ActionInitialization());

    // 시각화 매니저 초기화
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // UI 매니저 가져오기
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui) {
        // GUI 모드
        UImanager->ApplyCommand("/control/execute init.mac"); // 초기화 매크로 실행
        ui->SessionStart();
        delete ui;
    } else {
        // Batch 모드
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName); // 입력받은 매크로 파일 실행
    }

    // 정리
    delete visManager;
    delete runManager;

    return 0;
}