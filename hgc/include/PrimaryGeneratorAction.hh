#ifndef hgc_PrimaryGeneratorAction_h
#define hgc_PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include "G4Event.hh"
#include "G4VPrimaryGenerator.hh"

#include "defs.h"

namespace simulations
{
    namespace hgc
    {
        class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
        {
        	public:
		        //	Constructor and Destructor
        		PrimaryGeneratorAction(RunConfiguration runConfiguration, TTree *tree);
		        virtual ~PrimaryGeneratorAction();

        		//	Standard func	
		        void GeneratePrimaries(G4Event *anEvent);

        		//	To keep things simplier
		        G4ParticleGun *particleGun;
        		G4String primName;
		        G4double primEnergy;
        		G4ThreeVector primPos;
		        G4ThreeVector primDir;
        		G4int verbosityLevel;

		        RunConfiguration m_runConfiguration;
        		TTree *m_tree;
        };
    }
}

#endif
