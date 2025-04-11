#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector> // DNA 히트 정보를 저장하기 위해

class G4Event;

// DNA 히트 정보를 담을 구조체 (예시)
struct DNAHit {
    G4int    dnaCopyNo;
    G4double edep;
    G4double posX, posY, posZ;
};

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* anEvent);
    virtual void EndOfEventAction(const G4Event* anEvent);

    // SteppingAction에서 호출하여 히트 정보 추가
    void AddDNAHit(G4int copyNo, G4double edep, G4ThreeVector pos);

private:
    // 현재 이벤트에서 발생한 DNA 히트 정보 저장 벡터
    std::vector<DNAHit> fDNAHitsCollection;
};

#endif