#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    // Master 스레드에서 실행될 액션 (주로 RunAction)
    SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
    // Worker 스레드에서 실행될 액션들
    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(new RunAction);
    EventAction* eventAction = new EventAction; // EventAction 인스턴스 생성
    SetUserAction(eventAction);
    SetUserAction(new SteppingAction(eventAction)); // EventAction 포인터를 SteppingAction에 전달
}