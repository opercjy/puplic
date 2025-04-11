#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField(); // 필요시 Sensitive Detector 정의

private:
    // DNA 볼륨을 식별하기 위한 포인터 (예시)
    std::vector<G4LogicalVolume*> fDNALogicalVolumes;

    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // 필요시 멤버 변수 추가 (예: DNA 개수, 크기 등)
    G4int fNumberOfDNAs;
};

#endif