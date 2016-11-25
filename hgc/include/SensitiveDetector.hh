#ifndef hgc_SensitiveDetector_h
#define hgc_SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include "hgc/include/defs.h"

#include <vector>
#include <string>

#include "TTree.h"
#include "TH1D.h"

using namespace std;

//
//	Define the HGCal SD Class
//
namespace simulations
{
    namespace hgc
    {
        class SensitiveDetector : public G4VSensitiveDetector
        {
        	public:
		        SensitiveDetector(std::string const& name, 
                    RunConfiguration, HGCConfiguration, TTree*);
        		virtual ~SensitiveDetector();

		        void Initialize(G4HCofThisEvent*);
        		G4bool ProcessHits(G4Step*, G4TouchableHistory*);
		        void EndOfEvent(G4HCofThisEvent*);

                RunConfiguration m_runConfiguration;
                HGCConfiguration m_hgcConfiguration;
        		TTree *m_tree;
                std::string m_name;

                //  z, y, x
                double m_response[30][30][30];
		        Double_t _response;
        		Double_t respPerLayer[50];
        };
    }
}

#endif 
