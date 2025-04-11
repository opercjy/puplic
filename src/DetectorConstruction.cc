#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh" // 세포 모델링용
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SDManager.hh" // Sensitive Detector 용
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(), fNumberOfDNAs(4) // 예시: DNA 4개
{ }

DetectorConstruction::~DetectorConstruction()
{ }

void DetectorConstruction::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();

    // 월드 재질 (진공 또는 공기)
    nist->FindOrBuildMaterial("G4_Galactic");

    // 세포/조직 재질 (물로 근사 - GEANT4-DNA 표준)
    nist->FindOrBuildMaterial("G4_WATER");

    // 필요시 다른 재질 정의
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
    // 재질 가져오기
    G4Material* vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    G4Material* water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");

    // --- 월드 볼륨 ---
    G4double worldSize = 10.0 * um; // 시뮬레이션 영역 크기 (조절 필요)
    G4Box* solidWorld = new G4Box("World", 0.5*worldSize, 0.5*worldSize, 0.5*worldSize);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, vacuum, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

    // --- 종양 세포 클러스터 모델링 (예: 구) ---
    // --- 20 Gb 수준 인체/ 세포 Voxel, Mesh profile, GDML 등으로 대체 가능 ---
    G4double cellRadius = 5.0 * um; // 세포 크기 (조절 필요)
    G4Sphere* solidCellCluster = new G4Sphere("CellCluster", 0., cellRadius, 0., twopi, 0., pi);
    G4LogicalVolume* logicCellCluster = new G4LogicalVolume(solidCellCluster, water, "CellCluster"); // 물로 채움
    new G4PVPlacement(0, G4ThreeVector(), logicCellCluster, "CellCluster", logicWorld, false, 0, true);

    // 시각화 속성 (세포)
    G4VisAttributes* cellVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.3)); // 반투명 청록색
    cellVisAtt->SetForceWireframe(false); // 솔리드 형태로 보기
    logicCellCluster->SetVisAttributes(cellVisAtt);

    // --- DNA 모델링 (예: 작은 실린더 또는 구) ---
    G4double dnaRadius = 1.0 * nm; // DNA 폭 (매우 단순화된 값)
    G4double dnaLength = 3.4 * nm; // DNA 한 턴 길이 (매우 단순화된 값)

    // 예시: 작은 구로 DNA 모델링
    G4Sphere* solidDNA = new G4Sphere("DNA_Sphere", 0., dnaRadius, 0., twopi, 0., pi);
    // 예시: 작은 실린더로 DNA 모델링
    // #include "G4Tubs.hh"
    // G4Tubs* solidDNA = new G4Tubs("DNA_Tube", 0., dnaRadius, 0.5*dnaLength, 0., twopi);

    G4LogicalVolume* logicDNA = new G4LogicalVolume(solidDNA, water, "DNA"); // DNA도 물로 근사

    // 시각화 속성 (DNA)
    G4VisAttributes* dnaVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0)); // 불투명 빨간색
    dnaVisAtt->SetForceWireframe(true);
    logicDNA->SetVisAttributes(dnaVisAtt);

    // DNA 배치 (예시: 세포 중심 근처에 무작위 배치)
    // 더 정교한 배치가 필요함
    fDNALogicalVolumes.clear(); // 벡터 초기화
    for (G4int i = 0; i < fNumberOfDNAs; ++i) {
        // 단순 예시: 중심 근처에 적당히 배치 (겹치지 않도록 주의)
        G4double x = (G4UniformRand() - 0.5) * 10.0 * nm;
        G4double y = (G4UniformRand() - 0.5) * 10.0 * nm;
        G4double z = (G4UniformRand() - 0.5) * 10.0 * nm;

        // DNA 볼륨 이름을 다르게 하여 식별 가능하게 함 (예: SteppingAction에서)
        std::string dnaPVName = "DNA_" + std::to_string(i);
        new G4PVPlacement(0, G4ThreeVector(x, y, z), logicDNA, dnaPVName, logicCellCluster, false, i, true); // copy number 'i' 사용
        fDNALogicalVolumes.push_back(logicDNA); // 로지컬 볼륨 저장 (필요시 사용)
    }

    // 월드 반환
    return physWorld;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    DefineMaterials();
    return DefineVolumes();
}

void DetectorConstruction::ConstructSDandField()
{
    // Sensitive Detector (SD) 설정 부분
    // DNA 볼륨에 에너지 침적 등을 기록하기 위해 필요

    // 1. SD 매니저 가져오기
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->SetVerboseLevel(1); // 필요시 로그 레벨 조절

    // 2. MultiFunctionalDetector 생성 (다양한 정보를 기록하기 용이)
    G4MultiFunctionalDetector* dnaDetector = new G4MultiFunctionalDetector("DNADetector");
    sdManager->AddNewDetector(dnaDetector);

    // 3. Primitive Scorer 생성 및 등록 (예: 에너지 침적)
    G4VPrimitiveScorer* energyDeposited = new G4PSEnergyDeposit("EnergyDeposit");
    dnaDetector->RegisterPrimitive(energyDeposited);

    // 4. SD 를 Logical Volume 에 할당
    // DetectorConstruction 에서 생성된 모든 DNA Logical Volume에 할당
    // 주의: 만약 DNA 모양이 여러 개라면 각기 다른 로지컬 볼륨에 할당해야 함
    // 이 예제에서는 하나의 logicDNA 를 복제하여 사용했으므로 이것에 할당
    if (!fDNALogicalVolumes.empty()) {
         SetSensitiveDetector(fDNALogicalVolumes[0], dnaDetector);
         G4cout << ">>> Sensitive detector assigned to DNA logical volume: "
                << fDNALogicalVolumes[0]->GetName() << G4endl;
    } else {
        G4cerr << "ERROR: No DNA logical volumes found to assign SD!" << G4endl;
    }

    // 만약 여러 종류의 DNA 로지컬 볼륨이 있다면 각각 SetSensitiveDetector 호출
    // for (auto lv : fDNALogicalVolumes) {
    //     SetSensitiveDetector(lv, dnaDetector);
    // }
}