#include "hgc/include/RunAction.hh"

namespace simulations
{
    namespace hgc
    {
        RunAction::RunAction()
        {
        	timer = new G4Timer;
        }

        RunAction::~RunAction()
        {
        	delete timer;
        }

        void RunAction::BeginOfRunAction(const G4Run* aRun)
        {
        	G4cout << "### Run " << aRun->GetRunID() << " start. " << G4endl;
        	timer->Start();
        }

        void RunAction::EndOfRunAction(const G4Run *aRun)
        {
        	timer->Stop();
        	G4cout << "Number of Events: " << aRun->GetNumberOfEvent()
        		<< " took " << *timer << G4endl;
        }
    }
}
