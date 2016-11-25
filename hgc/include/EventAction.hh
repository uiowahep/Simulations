#ifndef hgc_EventAction_h
#define hgc_EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Event.hh"
#include "G4Timer.hh"

#include "TTree.h"

namespace simulations
{
    namespace hgc
    {
        class EventAction : public G4UserEventAction
        {
        	public:
		        EventAction(TTree*);
        		~EventAction();

		        void BeginOfEventAction(const G4Event*);
        		void EndOfEventAction(const G4Event*);

		        G4Timer *timer;
                TTree *m_tree;
        };
    }
}

#endif
