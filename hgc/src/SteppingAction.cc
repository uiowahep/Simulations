#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"

#include "hgc/include/SteppingAction.hh"

#include <iostream>

using namespace std;
using namespace CLHEP;

namespace simulations
{
    namespace hgc
    {
		SteppingAction::SteppingAction(RunConfiguration runConfiguration)
			: m_runConfiguration(runConfiguration)
		{;}

		SteppingAction::~SteppingAction()
		{;}

		void SteppingAction::UserSteppingAction(const G4Step* aStep)
		{
			//	To avoid INfinite Internal Reflections
			//
			if (aStep->GetTrack()->GetCurrentStepNumber()>1000)
				aStep->GetTrack()->SetTrackStatus(fStopAndKill);
		/*	G4String pName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
			if (pName == "opticalphoton")
			{
				//	Just to double check
				//
				if (runParams.verbosity>0)
					cout << pName << endl;

				G4double opEne = aStep->GetTrack()->GetTotalEnergy()/eV;
				G4double hPlanck = 4.136*ev;
				G4double cc =
			}
			*/
		}
	}
}
