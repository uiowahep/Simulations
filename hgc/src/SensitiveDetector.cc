#include "hgc/include/SensitiveDetector.hh"
#include "G4UnitsTable.hh"
#include "G4TrackStatus.hh"
#include "G4VProcess.hh"

#include <iostream>
#include <math.h>

using namespace std;
using namespace CLHEP;

namespace simulations
{
    namespace hgc
    {
		//
		//	Constructor
		//
		SensitiveDetector::SensitiveDetector(std::string  const& name, 
            RunConfiguration rcfg, HGCConfiguration gcfg, TTree *tree)
			: G4VSensitiveDetector(name),
			m_runConfiguration(rcfg), m_hgcConfiguration(gcfg),
            m_tree(tree), m_name(name)
		{
            m_tree->Branch((name+"responseImage").c_str(), m_response,
                    (name+"responseImage[30][30][30]/D").c_str());
			m_tree->Branch((name+"response").c_str(), &_response, 
                (name+"response/D").c_str());
			m_tree->Branch((name+"respPerLayer").c_str(), &respPerLayer, 
                (name+"respPerLayer[50]/D").c_str());
		}

		//
		//	Desctructor
		//
		SensitiveDetector::~SensitiveDetector()
		{}

		//
		//	Initialize
		//
		void SensitiveDetector::Initialize(G4HCofThisEvent*)
		{
            for (int i=0; i<30; i++)
                for (int j=0; j<30; j++)
                    for (int k=0; k<30; k++)
                        m_response[i][j][k] = 0;
			_response = 0;
			for (int i=0; i<50; i++)
				respPerLayer[i] = 0;
		}

		//
		//	Process Hits
		//
		G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory*)
		{
			if (aStep->GetTrack()->GetParticleDefinition()->GetPDGCharge()!=0)
			{
				G4ThreeVector dPos = aStep->GetDeltaPosition();
		//		_dx.push_back(dPos.x()/um);
		//		_dy.push_back(dPos.y()/um);
		//		_dz.push_back(dPos.z()/um);
		//		_dr.push_back(dPos.mag()/um);

				//	Here is the MIP-like scaling...
				//
				Double_t resp = (dPos.mag()/um)*80;
		//		_response += resp;
				G4TouchableHandle touchable =
					aStep->GetPreStepPoint()->GetTouchableHandle();
				int ilayer = touchable->GetCopyNumber(1);
				respPerLayer[ilayer] += resp;
				_response += resp;

		//		cout << ilayer/10 + 1 << endl;

				if (m_runConfiguration.verbose)
				{
					G4String preName =
						aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
					G4String postName =
						aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();

					//	Get local/global position for pre Volume
					//
					G4ThreeVector preGlobalPos =
						aStep->GetPreStepPoint()->GetPosition();
					G4TouchableHandle theTouchable =
						aStep->GetPreStepPoint()->GetTouchableHandle();
					G4ThreeVector preLocalPos =
						theTouchable->GetHistory()->GetTopTransform().TransformPoint(
						preGlobalPos);


					G4ThreeVector postGlobalPos =
						aStep->GetPostStepPoint()->GetPosition();
					theTouchable =
						aStep->GetPostStepPoint()->GetTouchableHandle();
					G4ThreeVector postLocalPos =
						theTouchable->GetHistory()->GetTopTransform().TransformPoint(
						postGlobalPos);

					G4cout << "### A hit in Part: " << m_name
						<< G4endl
						<< preName << "  " << postName << " " << dPos.mag()/um
						<< G4endl
						<< preGlobalPos/cm << "  " << preLocalPos/cm
						<< G4endl
						<< postGlobalPos/cm << "  " << postLocalPos/cm
                        << G4endl
						<< G4endl;
				}

                G4double fullX = 30.*cm; G4double fullY = 30.*cm;
                G4ThreeVector preGlobalPos =
                    aStep->GetPreStepPoint()->GetPosition();
                int iy = trunc(preGlobalPos.y()/cm + 
                    fullY/cm/2);
                int ix = trunc(preGlobalPos.x()/cm + 
                    fullX/cm/2);
                m_response[ilayer][iy][ix]+=resp;
			}

			return true;
		}

		//
		//	End of Event
		//
		void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
		{
		}
	}
}
