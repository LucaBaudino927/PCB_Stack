#include "MyActionInitialization.hh"

MyActionInitialization::MyActionInitialization() {}

MyActionInitialization::~MyActionInitialization(){}

//For Multithread...
void MyActionInitialization::BuildForMaster() const
{
    SetUserAction(new MyRunAction());
}

void MyActionInitialization::Build() const{ 
    MyPrimaryGenerator* generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    MyRunAction* runAction = new MyRunAction();
    SetUserAction(runAction);
    
    MyEventAction* eventAction = new MyEventAction();
    SetUserAction(eventAction);
    
    MySteppingAction* steppingAction = new MySteppingAction(eventAction);
    SetUserAction(steppingAction);
}
