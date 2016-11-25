#ifndef common_MaterialsManager_H
#define common_MaterialsManager_H 1

#include "globals.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"
#include "G4NistManager.hh"

#include <map>

namespace simulations
{
    namespace common
    {
        class MaterialsManager
        {
	        public:
                MaterialsManager();
                virtual ~MaterialsManager() {}

        		//	List of Materials/Elements
        		G4Element *eLu;
        		G4Element *eY;
        		G4Element *eSi;
        		G4Element *eO;
        		G4Element *eH;
		
        		G4Material *mAir;
        		G4Material *mFe;
		        G4Material *mPb;
        		G4Material *mCu;
		        G4Material *mSi;
        		G4Material *mZn;
        		G4Material *mO;
		        G4Material *mC;
        		G4Material *mBr;
        		G4Material *mBrass;
		        G4Material *mVacuum;
        		G4Material *mAl2O3;
		        G4Material *mBeO;
        		G4Material *mMgO;
		        G4Material *mTiO;
        		G4Material *mCs3Sb;
		        G4Material *mGaP;
        		G4Material *mGaAsP;
        		G4Material *mGaPCs;
		        G4Material *mGaInP;
        		G4Material *mW;
        		G4Material *mLYSO;
        		G4Material *mSiO2;
        		G4Material *mG10;
        		G4Material *mClm;
        		G4Material *mGlass;
        		G4Material *mEpoxy;
        		G4Material *mPCB;
        };
    }
}
#endif











