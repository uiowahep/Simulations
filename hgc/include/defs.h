#ifndef hgc_defs_h
#define hgc_defs_h 1

#include "globals.hh"
#include <vector>
#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"
#include "TTree.h"

#include "G4Material.hh"

using namespace std;

namespace simulations
{
    namespace hgc
    {
		/*
		 *	Run Parameters
		 *
		 */
		struct RunConfiguration
		{
			double energy;
            std::string pName;
            std::string geomConfigFileName;
			bool isInteractive;
			bool verbose;
			int seed;
			int numEvents;
            std::string output;
		};

		struct HGCEMConfiguration
		{
			int n1, n2, n3;
			int iAbsMaterial;
			G4Material *mAbs;
            G4double fullAbsZ1, fullAbsZ2, fullAbsZ3;
            G4double fullSiPadZ, fullReadoutPadZ;
            G4Material *mReadoutPad;
		};

		struct HGCFHConfiguration
		{
			int nLayers_Total;
			int iAbsMaterial;
			G4Material *absMat;
		//	G4double fullFHModuleXYZ[3];
		//	G4double fullFHLayerXYZ[3];
			G4double fullFHAbsXYZ[3];
		//	G4double fullFHPadLayerXYZ[3];
			G4double fullFHPadXYZ[3];
			G4double fullFHPadReadoutXYZ[3];
		};

		struct HGCBHConfiguration
		{
			int nLayers_Total;
			int iAbsMaterial;
			G4Material *absMat;
		//	G4double fullBHModuleXYZ[3];
		//	G4double fullBHLayerXYZ[3];
			G4double fullBHAbsXYZ[3];
		//	G4double fullBHPadLayerXYZ[3];
			G4double fullBHPadXYZ[3];
			G4double fullBHPadReadoutXYZ[3];
		};

	    struct HGCConfiguration
		{
		//	G4double fullHGCalXYZ[3];
			HGCEMConfiguration em;
			HGCFHConfiguration fh;
			HGCBHConfiguration bh;
			int emOnOff;
			int fhOnOff;
			int bhOnOff;
		};
	}
}

#endif
