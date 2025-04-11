#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh" // G4Accumulable 포함

// Accumulable 예시 (DNA 에너지 침적 합계)
#include "G4Accumulable.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);

    // Accumulable 추가 (예: DNA 총 에너지 침적)
    void AddDnaEdep(G4double edep);

private:
    // Accumulable 멤버 변수 선언
    G4Accumulable<G4double> fTotalDnaEdep;

    // 파일 출력 등을 위한 멤버 변수 (예: AnalysisManager)
    // #include "g4analysis.hh" // 필요시 추가
    // G4AnalysisManager* fAnalysisManager;
};

#endif