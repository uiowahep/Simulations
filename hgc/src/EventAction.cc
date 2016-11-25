#include "hgc/include/EventAction.hh"

#include <iostream>

using namespace std;

namespace simulations
{
    namespace hgc
    {
        EventAction::EventAction(TTree *tree): m_tree(tree)
        {
        	timer = new G4Timer();
        }

        EventAction::~EventAction()
        {
        	delete timer;
        }

        void EventAction::BeginOfEventAction(const G4Event* anEvent)
        {
        	G4cout << "### Starting Event: " << anEvent->GetEventID() << endl;
        	timer->Start();
        }

        void EventAction::EndOfEventAction(const G4Event*)
        {
            m_tree->Fill();
        	timer->Stop();
        	G4cout << "### Event Duration: " << *timer << G4endl;
        }
    }
}
