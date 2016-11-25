#ifndef hgc_RunAction_h
#define hgc_RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Run.hh"
#include "G4Timer.hh"

namespace simulations
{
    namespace hgc
    {
        class RunAction : public G4UserRunAction
        {
        	public:
		        RunAction();
        		~RunAction();

		        void BeginOfRunAction(const G4Run* aRun);
        		void EndOfRunAction(const G4Run* aRun);

		        G4Timer *timer;
        };
    }
}

#endif
