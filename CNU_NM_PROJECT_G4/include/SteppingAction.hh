#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4Step;
class EventAction; // EventAction에 정보를 전달하기 위해 필요
class RunAction;   // RunAction의 Accumulable에 접근하기 위해 필요 (RunManager 통해)

class SteppingAction : public G4UserSteppingAction
{
public:
    // EventAction 포인터를 받아 멤버 변수에 저장
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* aStep);

private:
    EventAction* fEventAction; // EventAction 인스턴스 포인터
    RunAction* fRunAction;     // RunAction 인스턴스 포인터 (BeginOfRun에서 가져옴)
};

#endif