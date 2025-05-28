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
    
    MySteppingAction* steppingAction = new MySteppingAction();
    SetUserAction(steppingAction);
    
    MyEventAction* eventAction = new MyEventAction(steppingAction);
    SetUserAction(eventAction);
    
}
