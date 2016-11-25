#ifndef hgc_SteppingAction_h
#define hgc_SteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "defs.h"

namespace simulations
{
    namespace hgc
    {
        class SteppingAction : public G4UserSteppingAction
        {
          public:
            SteppingAction(RunConfiguration);
            virtual ~SteppingAction();

            virtual void UserSteppingAction(const G4Step*);

        	RunConfiguration m_runConfiguration;
        };
    }
}

#endif

