#ifndef hgc_DetectorConstruction_h
#define hgc_DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include <vector>

#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "defs.h"
#include "SensitiveDetector.hh"
#include "common/include/MaterialsManager.h"

using namespace CLHEP;
using namespace simulations::common;

namespace simulations
{
    namespace hgc
    {
        class DetectorConstruction : public G4VUserDetectorConstruction
        {
        	public:
		        //	Constructor and Destructor
        		DetectorConstruction(RunConfiguration, TTree *tree);
        		~DetectorConstruction(); 

        		G4VPhysicalVolume* Construct();
		        G4VPhysicalVolume* BuildGeometry();
        		void BuildShashlik();
		        void BuildHGCal();
        		int ReadConfigFile();
		        void UpdateGeometry();

                MaterialsManager m_mmanager;
        		RunConfiguration m_runConfiguration;
		        HGCConfiguration m_hgcConfiguration;
		        TTree *m_tree;

        		//
		        //	Volumes
        		//

		        //	World
        		//	/
		        G4Box *solidWorld;
        		G4LogicalVolume *logicWorld;
		        G4VPhysicalVolume *physWorld;

        		/*
        		 *	HGCAL Part
		         *
        		 */
		        G4Box *solidHGCal;
        		G4LogicalVolume *logicHGCal;
		        G4VPhysicalVolume *physHGCal;

        		//
		        //	EM Part Whole Module
        		//
		        G4Box *solidHG_EMModule;
        		G4LogicalVolume *logicHG_EMModule;
		        G4VPhysicalVolume *physHG_EMModule;

        		//	EM Part: Layer
		        //
        		G4Box *solidHG_EMLayer[3];
		        G4LogicalVolume *logicHG_EMLayer[3];
        		G4VPhysicalVolume *physHG_EMLayer;

		        //	EM Part: Abs: 3types depending on thickness;
        		//
		        G4Box *solidHG_EMAbs[3];
        		G4LogicalVolume *logicHG_EMAbs[3];
		        G4VPhysicalVolume *physHG_EMAbs;

        		//	EM Part: Si Pad Layer
		        //
        		G4Box *solidHG_EMPadLayer;
		        G4LogicalVolume *logicHG_EMPadLayer;
        		G4VPhysicalVolume *physHG_EMPadLayer;

		        //	EM Part: Si Pad: 2 types as in Slide 16
        		//	-- 0.9cm2 for first 20layers, and 1.8cm2 for the last 10.
		        //	-- the exact dimensions can be varied, layer lay out too....
        		//
		        G4Box *solidHG_EMPad[2];
        		G4LogicalVolume *logicHG_EMPad[2];
		        G4VPhysicalVolume *physHG_EMPad;

        		G4Box *solidHG_EMPadReadout;
		        G4LogicalVolume *logicHG_EMPadReadout;
        		G4VPhysicalVolume *physHG_EMPadReadout;

		        //
        		//	Front Hadronic CAL
		        //
        		G4Box *solidHG_FHModule;
		        G4LogicalVolume *logicHG_FHModule;
        		G4VPhysicalVolume *physHG_FHModule;

		        //	Fron HCAL: Layer
        		//
		        G4Box *solidHG_FHLayer;
        		G4LogicalVolume *logicHG_FHLayer;
		        G4VPhysicalVolume *physHG_FHLayer;

        		//	Font HCAL: Abs
		        //
        		G4Box *solidHG_FHAbs;
		        G4LogicalVolume *logicHG_FHAbs;
        		G4VPhysicalVolume *physHG_FHAbs;

		        //	Front HCAL: Si Pad Layer
        		//
		        G4Box *solidHG_FHPadLayer;
        		G4LogicalVolume *logicHG_FHPadLayer;
		        G4VPhysicalVolume *physHG_FHPadLayer;

        		//	Front HCAL: Si Pad
		        //
        		G4Box *solidHG_FHPad;
		        G4LogicalVolume *logicHG_FHPad;
        		G4VPhysicalVolume *physHG_FHPad;

		        //	Front HCAl Readout
        		//
		        G4Box *solidHG_FHPadReadout;
        		G4LogicalVolume *logicHG_FHPadReadout;
		        G4VPhysicalVolume *physHG_FHPadReadout;

        		//
		        //	Back HCAL
        		//
		        G4Box *solidHG_BHModule;
        		G4LogicalVolume *logicHG_BHModule;
		        G4VPhysicalVolume *physHG_BHModule;

        		//	Back HCAL: Layer
		        //
        		G4Box *solidHG_BHLayer;
		        G4LogicalVolume *logicHG_BHLayer;
        		G4VPhysicalVolume *physHG_BHLayer;

		        //	Back HCAL: Abs
        		//
		        G4Box *solidHG_BHAbs;
        		G4LogicalVolume *logicHG_BHAbs;
		        G4VPhysicalVolume *physHG_BHAbs;

        		//	Back HCAL: Si Pad Layer
		        //
        		G4Box *solidHG_BHPadLayer;
		        G4LogicalVolume *logicHG_BHPadLayer;
        		G4VPhysicalVolume *physHG_BHPadLayer;

		        //	Back HCAL: Si Pad
        		//
		        G4Box *solidHG_BHPad;
        		G4LogicalVolume *logicHG_BHPad;
		        G4VPhysicalVolume *physHG_BHPad;

        		//	Back HCAL: Pad Readout
		        //
        		G4Box *solidHG_BHPadReadout;
		        G4LogicalVolume *logicHG_BHPadReadout;
        		G4VPhysicalVolume *physHG_BHPadReadout;
        };
    }
}
#endif
