#include "G4NistManager.hh"
#include "G4UnitsTable.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4UserLimits.hh"

#include "hgc/include/DetectorConstruction.hh"

namespace simulations
{
    namespace hgc
    {
        /*
         *	Volume's dimensions
         */
        //	World
        //
        G4double fullWorldX = 5.*m;
        G4double fullWorldY = 5.*m;
        G4double fullWorldZ = 5.*m;

        /*
         *	Member Functions
         */
        DetectorConstruction::~DetectorConstruction()
        {

        }

        //
        //	Constructor
        //
        DetectorConstruction::DetectorConstruction(RunConfiguration cfg,
        	TTree *tree) :
            m_runConfiguration(cfg), m_tree(tree)
        {}

        //	G4 Method
        //
        G4VPhysicalVolume* DetectorConstruction::Construct()
        {
        	return this->BuildGeometry();
        }


        //	Build all the Geometry
        //	Include The EM Field,
        //
        G4VPhysicalVolume* DetectorConstruction::BuildGeometry()
        {
        	//	Define World Dimensions
        	//
        //	G4double fullWorldZ = 1*m;
        //	G4double fullWorldX = 1*m;
        //	G4double fullWorldY = 1*m;

        	//	Create the World iteself first
        	//
        	solidWorld = new G4Box("solidWorld", fullWorldX/2.0, fullWorldY/2.0,
        			fullWorldZ/2.0);
        	logicWorld = new G4LogicalVolume(solidWorld, m_mmanager.mVacuum, "logicWorld",
        			0,0,0);
        	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld",
        			0, 0, 0, false);

        	//	Double checking...
        	//
        	G4cout << "### World: " << fullWorldX/cm << "  " << fullWorldY/cm
        		<< "  " << fullWorldZ/cm
        		<< G4endl;

            //
        	//	Build HGCal
        	//
        	BuildHGCal();

            //
        	//	Return pointer to the WOrld
        	//
        	return physWorld;
        }


        /*
         *	Build HGCAL
         */
        void DetectorConstruction::BuildHGCal()
        {
            //  read in the geometry configuration
            this->ReadConfigFile();

            //  
        	//	SD - we need only EM part to be read out only
        	//
        	G4SDManager *SDManager = G4SDManager::GetSDMpointer();
        	SensitiveDetector *EMSD = new SensitiveDetector("em",
                m_runConfiguration, m_hgcConfiguration, m_tree);
//            SensitiveDetector *FHSD = NULL;
//            SensitiveDetector *BHSD = NULL;
//        	SensitiveDetector *FHSD = new SensitiveDetector("fh", 
//                m_runConfiguration, m_hgcConfiguration, m_tree);
//        	SensitiveDetector *BHSD = new SensitiveDetector("bh", 
//                m_runConfiguration, m_hgcConfiguration, m_tree);
        	SDManager->AddNewDetector(EMSD);
//        	SDManager->AddNewDetector(FHSD);
//        	SDManager->AddNewDetector(BHSD);

        	//	Limit the step size inside the SDs
        	//i
        //	G4UserLimits *sdLimits = new G4UserLimits(
        //			m_hgcConfiguration.em.fullEMPadXYZ[2]);
            
            // define the number of layers and total - hardcoded for now
            m_hgcConfiguration.em.n1 = 10;
            m_hgcConfiguration.em.n2 = 10;
            m_hgcConfiguration.em.n3 = 10;
            int n = m_hgcConfiguration.em.n1+
                m_hgcConfiguration.em.n2+m_hgcConfiguration.em.n3;

            /*
             * Define X/Y/Z full dimensions for all 3 layers of EM part
             */
            G4double fullX = 30.*cm;
            G4double fullY = 30.*cm;
        	G4double fullEMLayerX_1 = fullX;
        	G4double fullEMLayerY_1 = fullY;
        	G4double fullEMLayerZ_1 = m_hgcConfiguration.em.fullAbsZ1 +
        		m_hgcConfiguration.em.fullSiPadZ +
        		m_hgcConfiguration.em.fullReadoutPadZ;

        	G4double fullEMLayerX_2 = fullX;
        	G4double fullEMLayerY_2 = fullY;
        	G4double fullEMLayerZ_2 = m_hgcConfiguration.em.fullAbsZ2 +
        		m_hgcConfiguration.em.fullSiPadZ +
        		m_hgcConfiguration.em.fullReadoutPadZ;

        	G4double fullEMLayerX_3 = fullX;
        	G4double fullEMLayerY_3 = fullY;
        	G4double fullEMLayerZ_3 = m_hgcConfiguration.em.fullAbsZ3 +
        		m_hgcConfiguration.em.fullSiPadZ +
        		m_hgcConfiguration.em.fullReadoutPadZ;

        	G4double fullEMPadLayerX = fullEMLayerX_1;
        	G4double fullEMPadLayerY = fullEMLayerY_1;
        	G4double fullEMPadLayerZ = m_hgcConfiguration.em.fullSiPadZ;

        	G4double fullEMModuleX = fullEMLayerX_1;
        	G4double fullEMModuleY = fullEMLayerY_1;
        	G4double fullEMModuleZ = m_hgcConfiguration.em.n1*fullEMLayerZ_1 +
        		m_hgcConfiguration.em.n2*fullEMLayerZ_2 +
        		m_hgcConfiguration.em.n3*fullEMLayerZ_3;


        	G4double fullHGCalX = fullEMModuleX;
        	G4double fullHGCalY = fullEMModuleY;
        	G4double fullHGCalZ = fullEMModuleZ;

        	//
        	//	Section for Debug Purposes: Print all the Dimensions
        	//
        	G4cout << "### HGCAl: " << fullHGCalX/cm << "  " << fullHGCalY/cm
        		<< "  " << fullHGCalZ/cm
        		<< G4endl
        		<< "### Parameters: " << n
        		<< "  " << m_hgcConfiguration.em.n1 << "  "
        		<< m_hgcConfiguration.em.n2 << "  "
        		<< m_hgcConfiguration.em.n3 << "  "
        		<< m_hgcConfiguration.em.iAbsMaterial
        		<< G4endl
        		<< "### EM: " << fullEMModuleX/cm << "  " << fullEMModuleY/cm
        		<< "  " << fullEMModuleZ/cm
        		<< G4endl
        		<< "### EM: Layer_1: " << fullEMLayerX_1/cm << "  "
        		<< fullEMLayerY_1/cm << "  " << fullEMLayerZ_1/cm
        		<< G4endl
        		<< "  " << m_hgcConfiguration.em.fullAbsZ1/cm
                << G4endl
        		<< "  Material: " << m_hgcConfiguration.em.mAbs
        		<< G4endl
        		<< "### EM: Layer_1: PadLayer: " << fullEMPadLayerX/cm
        		<< "  " << fullEMPadLayerY/cm << "  " << fullEMPadLayerZ/cm
        		<< G4endl
        		<< "### EM: Layer_1: PadReadout: "
        		<< m_hgcConfiguration.em.fullReadoutPadZ/cm
        		<< m_hgcConfiguration.em.mReadoutPad
        		<< G4endl
        		<< "### EM: Layer_2: " << fullEMLayerX_2/cm << "  "
        		<< fullEMLayerY_2/cm << "  " << fullEMLayerZ_2/cm
        		<< G4endl
        		<< "### EM: Layer_3: " << fullEMLayerX_3/cm << "  "
        		<< fullEMLayerY_3/cm << "  " << fullEMLayerZ_3/cm
        		<< G4endl;

        	cout << "### HERE: " << fullHGCalX/cm << "  " << fullHGCalY/cm << "  "
        		<< fullHGCalZ/cm << endl;
        	cout << "### HERE: " << fullWorldX/cm << "  " << fullWorldY/cm << "  "
        		<< fullWorldZ/cm << endl;

            //
            // place the whole hgcal now
        	//
        	solidHGCal = new G4Box("solidHGCal", fullHGCalX/2., fullHGCalY/2.,
        			fullHGCalZ/2.);
        	logicHGCal = new G4LogicalVolume(solidHGCal, m_mmanager.mVacuum, "logicHGCal");
        	physHGCal = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicHGCal,
        			"physHGCal", logicWorld, 0, 0, true);


            //  
        	// Build the EM Part
        	//
        	solidHG_EMModule = new G4Box("solidHG_EMModule", fullEMModuleX/2.,
        			fullEMModuleY/2., fullEMModuleZ/2.);
        	logicHG_EMModule = new G4LogicalVolume(solidHG_EMModule, m_mmanager.mVacuum,
        			"logicHG_EMModule");
        	G4double zpos = -fullHGCalZ/2. + fullEMModuleZ/2;
        	physHG_EMModule = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        		logicHG_EMModule, "physHG_EMModule", logicHGCal, 0, 0, true);

        	//
        	//	There are 3 sections for EM part
        	//	See pdf for specifics
        	//

            //
        	// first group of layers
        	//
        	solidHG_EMLayer[0] = new G4Box("solidHG_EMLayer_1",
        			fullEMLayerX_1/2., fullEMLayerY_1/2., fullEMLayerZ_1/2.);
        	logicHG_EMLayer[0] = new G4LogicalVolume(solidHG_EMLayer[0], 
                    m_mmanager.mVacuum,
        			"logicHG_EMLayer_1");

            //  build the absorber
        	solidHG_EMAbs[0] = new G4Box("solidHG_EMAbs_1",
        			fullEMLayerX_1/2.,
                    fullEMLayerY_1/2.,
        			m_hgcConfiguration.em.fullAbsZ1/2.);
        	logicHG_EMAbs[0] = new G4LogicalVolume(solidHG_EMAbs[0],
        			m_hgcConfiguration.em.mAbs, "logicHG_EMAbs_1");
        	zpos = -fullEMLayerZ_1/2. + m_hgcConfiguration.em.fullAbsZ1/2.;
        	physHG_EMAbs = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMAbs[0], "phys_HF_EMAbs", logicHG_EMLayer[0], 0, 0, true);

            // build silicon pad 
        	solidHG_EMPadLayer = new G4Box("solidHG_EMPadLayer",
        			fullEMPadLayerX/2., fullEMPadLayerY/2., fullEMPadLayerZ/2.);
        	logicHG_EMPadLayer = new G4LogicalVolume(solidHG_EMPadLayer,
        			m_mmanager.mSi, "logicHG_EMPadLayer", 0, EMSD, 0);
        //	logicHG_EMPadLayer->SetUserLimits(sdLimits);
        	zpos = -fullEMLayerZ_1/2. + m_hgcConfiguration.em.fullAbsZ1 +
        		fullEMPadLayerZ/2.;
        	physHG_EMPadLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMPadLayer, "physHG_EMPadLayer", logicHG_EMLayer[0], 0, 0,
        			true);

            //  build readout pad
        	solidHG_EMPadReadout = new G4Box("solidHG_EMPadReadout",
        			fullEMLayerX_1/2.,
        			fullEMLayerY_1/2.,
        			m_hgcConfiguration.em.fullReadoutPadZ/2.);
        	logicHG_EMPadReadout = new G4LogicalVolume(solidHG_EMPadReadout,
        			m_hgcConfiguration.em.mReadoutPad, "logicHG_EMPadReadout");
        	zpos = -fullEMLayerZ_1/2. + m_hgcConfiguration.em.fullAbsZ1 +
        		fullEMPadLayerZ + m_hgcConfiguration.em.fullReadoutPadZ/2.;
        	physHG_EMPadReadout = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMPadReadout, "physHG_EMPadReadout",
        			logicHG_EMLayer[0], 0, 0, true);

        	for (int iLayer=0; iLayer<m_hgcConfiguration.em.n1; iLayer++)
        	{
        		zpos = -fullEMModuleZ/2. + (iLayer + 0.5)*fullEMLayerZ_1;
        		physHG_EMLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        				logicHG_EMLayer[0], "physHG_EMLayer", logicHG_EMModule,
        				0, iLayer, true);
        	}

            //
            // second group of layers
        	//
        	solidHG_EMLayer[1] = new G4Box("solidHG_EMLayer_2",
        			fullEMLayerX_2/2., fullEMLayerY_2/2., fullEMLayerZ_2/2.);
        	logicHG_EMLayer[1] = new G4LogicalVolume(solidHG_EMLayer[1], 
                    m_mmanager.mVacuum,
        			"logicHG_EMLayer_2");

            // build the absorber
        	solidHG_EMAbs[1] = new G4Box("solidHG_EMAbs_2",
        			fullEMLayerX_1/2.,
        			fullEMLayerY_1/2.,
        			m_hgcConfiguration.em.fullAbsZ2/2.);
        	logicHG_EMAbs[1] = new G4LogicalVolume(solidHG_EMAbs[1],
        			m_hgcConfiguration.em.mAbs, "logicHG_EMAbs_2");
        	zpos = -fullEMLayerZ_2/2. + m_hgcConfiguration.em.fullAbsZ2/2.;
        	physHG_EMAbs = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMAbs[1], "phys_HF_EMAbs", logicHG_EMLayer[1], 0, 0, true);

            // build silicon pad
        	solidHG_EMPadLayer = new G4Box("solidHG_EMPadLayer",
        			fullEMPadLayerX/2., fullEMPadLayerY/2., fullEMPadLayerZ/2.);
        	logicHG_EMPadLayer = new G4LogicalVolume(solidHG_EMPadLayer,
        			m_mmanager.mSi, "logicHG_EMPadLayer", 0, EMSD, 0);
        //	logicHG_EMPadLayer->SetUserLimits(sdLimits);
        	zpos = -fullEMLayerZ_2/2. + m_hgcConfiguration.em.fullAbsZ2 +
        		fullEMPadLayerZ/2.;
        	physHG_EMPadLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMPadLayer, "physHG_EMPadLayer", logicHG_EMLayer[1], 0, 0,
        			true);

            // build the readout pad
        	solidHG_EMPadReadout = new G4Box("solidHG_EMPadReadout",
        			fullEMLayerX_1/2.,
        			fullEMLayerY_1/2.,
        			m_hgcConfiguration.em.fullReadoutPadZ/2.);
        	logicHG_EMPadReadout = new G4LogicalVolume(solidHG_EMPadReadout,
        			m_hgcConfiguration.em.mReadoutPad, "logicHG_EMPadReadout");
        	zpos = -fullEMLayerZ_2/2. + m_hgcConfiguration.em.fullAbsZ2 +
        		fullEMPadLayerZ + m_hgcConfiguration.em.fullReadoutPadZ/2.;
        	physHG_EMPadReadout = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMPadReadout, "physHG_EMPadReadout",
        			logicHG_EMLayer[1], 0, 0, true);

        	for (int iLayer=0; iLayer<m_hgcConfiguration.em.n2; iLayer++)
        	{
        		zpos = -fullEMModuleZ/2. + m_hgcConfiguration.em.n1*fullEMLayerZ_1 +
        			(iLayer + 0.5)*fullEMLayerZ_2;
        		physHG_EMLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        				logicHG_EMLayer[1], "physHG_EMLayer", logicHG_EMModule,
        				0, iLayer+m_hgcConfiguration.em.n1, true);
        	}

            //
            // build third group of layers
        	//
        	solidHG_EMLayer[2] = new G4Box("solidHG_EMLayer_3",
        			fullEMLayerX_3/2., fullEMLayerY_3/2., fullEMLayerZ_3/2.);
        	logicHG_EMLayer[2] = new G4LogicalVolume(solidHG_EMLayer[2], 
                    m_mmanager.mVacuum,
        			"logicHG_EMLayer_3");

            // build absorber
        	solidHG_EMAbs[2] = new G4Box("solidHG_EMAbs_3",
        			fullEMLayerX_1/2.,
        			fullEMLayerY_1/2.,
        			m_hgcConfiguration.em.fullAbsZ3/2.);
        	logicHG_EMAbs[2] = new G4LogicalVolume(solidHG_EMAbs[2],
        			m_hgcConfiguration.em.mAbs, "logicHG_EMAbs_3");
        	zpos = -fullEMLayerZ_3/2. + m_hgcConfiguration.em.fullAbsZ3/2.;
        	physHG_EMAbs = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMAbs[2], "phys_HF_EMAbs", logicHG_EMLayer[2], 0, 0, true);

            // build silicon pad
        	solidHG_EMPadLayer = new G4Box("solidHG_EMPadLayer",
        			fullEMPadLayerX/2., fullEMPadLayerY/2., fullEMPadLayerZ/2.);
        	logicHG_EMPadLayer = new G4LogicalVolume(solidHG_EMPadLayer,
        			m_mmanager.mSi, "logicHG_EMPadLayer", 0, EMSD, 0);
        //	logicHG_EMPadLayer->SetUserLimits(sdLimits);
        	zpos = -fullEMLayerZ_3/2. + m_hgcConfiguration.em.fullAbsZ3 +
        		fullEMPadLayerZ/2.;
        	physHG_EMPadLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMPadLayer, "physHG_EMPadLayer", logicHG_EMLayer[2], 0, 0,
        			true);

            // build readout pad
        	solidHG_EMPadReadout = new G4Box("solidHG_EMPadReadout",
        			fullEMLayerX_1/2.,
        			fullEMLayerY_1/2.,
        			m_hgcConfiguration.em.fullReadoutPadZ/2.);
        	logicHG_EMPadReadout = new G4LogicalVolume(solidHG_EMPadReadout,
        			m_hgcConfiguration.em.mReadoutPad, "logicHG_EMPadReadout");
        	zpos = -fullEMLayerZ_3/2. + m_hgcConfiguration.em.fullAbsZ3 +
        		fullEMPadLayerZ + m_hgcConfiguration.em.fullReadoutPadZ/2.;
        	physHG_EMPadReadout = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        			logicHG_EMPadReadout, "physHG_EMPadReadout",
        			logicHG_EMLayer[2], 0, 0, true);

        	for (int iLayer=0; iLayer<m_hgcConfiguration.em.n3; iLayer++)
        	{
        		zpos = -fullEMModuleZ/2. + m_hgcConfiguration.em.n1*fullEMLayerZ_1 +
        			m_hgcConfiguration.em.n2*fullEMLayerZ_2 +
        			(iLayer + 0.5)*fullEMLayerZ_3;
        		physHG_EMLayer = new G4PVPlacement(0, G4ThreeVector(0, 0, zpos),
        				logicHG_EMLayer[2], "physHG_EMLayer", logicHG_EMModule,
        				0, iLayer + m_hgcConfiguration.em.n1 +
        				m_hgcConfiguration.em.n2, true);
        	}

        	return;
        }//	end of BuildHGCal

        /*
         *	Read in HGCAL configuration data
         *	NOTE:
         *	--	All the input sizes are in mm
         *	--	Also, the order of dimensios is exactly as in SHDefs.hh
         */
        int DetectorConstruction::ReadConfigFile()
        {
        	cout << "### Reading in HGCAL Configuration File..." << endl;

        	//	Init/Open/Check file
        	//
        	ifstream hgConfigFile(m_runConfiguration.geomConfigFileName);
        	if (!hgConfigFile)
        	{
        		cout << "### ERROR: File " << m_runConfiguration.geomConfigFileName
        			<< "  hasn't been found!!!" << endl;
        		return -1;
        	}

            // dummy variables
        	double x,y,z;
        	int n, n1, n2, n3;
        	int iMat;
        	int emOnOff, fhOnOff, bhOnOff;

            //  read in the type of the absorber 
            hgConfigFile >> iMat;
            m_hgcConfiguration.em.iAbsMaterial = iMat;
        	if (iMat == 1)
        		m_hgcConfiguration.em.mAbs = m_mmanager.mPb;
        	else if (iMat == 2)
        		m_hgcConfiguration.em.mAbs = m_mmanager.mCu;
        	else if (iMat == 3)
        		m_hgcConfiguration.em.mAbs = m_mmanager.mW;

            //  read in the thicknesses of each group of absorbers(1-10 11-20 21-30)
            hgConfigFile >> x >> y >> z;
            m_hgcConfiguration.em.fullAbsZ1 = x*mm;
            m_hgcConfiguration.em.fullAbsZ2 = y*mm;
            m_hgcConfiguration.em.fullAbsZ3 = z*mm;

            // read in the thickness of silicon pads
            hgConfigFile >> x;
            m_hgcConfiguration.em.fullSiPadZ = x*mm;
            // read in the thickness of PCB - read out plate
            hgConfigFile >> x;
            m_hgcConfiguration.em.fullReadoutPadZ = x*mm;
            // set PCB as the material of readout pad
            m_hgcConfiguration.em.mReadoutPad = m_mmanager.mPCB;

            /*

        	//	Input the HGCal On/Off Settings
        	//
        	hgConfigFile >> emOnOff >> fhOnOff >> bhOnOff;
        	m_hgcConfiguration.emOnOff =emOnOff;
        	m_hgcConfiguration.fhOnOff = fhOnOff;
        	m_hgcConfiguration.bhOnOff = bhOnOff;

        	//	EM Input Part
        	//
        	hgConfigFile >> n >> n1 >> n2 >> n3 >> iMat;
        	m_hgcConfiguration.em.nLayers_Total = n;
        	m_hgcConfiguration.em.nLayers_1 = n1;
        	m_hgcConfiguration.em.nLayers_2 = n2;
        	m_hgcConfiguration.em.nLayers_3 = n3;
        	m_hgcConfiguration.em.iAbsMaterial = iMat;
        	if (iMat == 1)
        		m_hgcConfiguration.em.absMat = m_mmanager.mPb;
        	else if (iMat == 2)
        		m_hgcConfiguration.em.absMat = m_mmanager.mCu;
        	else if (iMat == 3)
        		m_hgcConfiguration.em.absMat = m_mmanager.mW;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.em.fullEMAbsXYZ[0] = x*mm;
        	m_hgcConfiguration.em.fullEMAbsXYZ[1] = y*mm;
        	m_hgcConfiguration.em.fullEMAbsXYZ[2] = z*mm;

        	hgConfigFile >> z;
        	m_hgcConfiguration.em.fullEMAbsXYZ[3] = z*mm;
        	hgConfigFile >> z;
        	m_hgcConfiguration.em.fullEMAbsXYZ[4] = z*mm;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.em.fullEMPadXYZ[0] = x*mm;
        	m_hgcConfiguration.em.fullEMPadXYZ[1] = y*mm;
        	m_hgcConfiguration.em.fullEMPadXYZ[2] = z*mm;

        	hgConfigFile >> x >> y;
        	m_hgcConfiguration.em.fullEMPadXYZ[3] = x*mm;
        	m_hgcConfiguration.em.fullEMPadXYZ[4] = y*mm;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.em.fullEMPadReadoutXYZ[0] = x*mm;
        	m_hgcConfiguration.em.fullEMPadReadoutXYZ[1] = y*mm;
        	m_hgcConfiguration.em.fullEMPadReadoutXYZ[2] = z*mm;

        	//	FH Input Part
        	//
        	hgConfigFile >> n >> iMat;
        	m_hgcConfiguration.fh.nLayers_Total = n;
        	m_hgcConfiguration.fh.iAbsMaterial = iMat;
        	if (iMat == 1)
        		m_hgcConfiguration.fh.absMat = m_mmanager.mBrass;
        	else
        		m_hgcConfiguration.fh.absMat = m_mmanager.mCu;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.fh.fullFHAbsXYZ[0] = x*mm;
        	m_hgcConfiguration.fh.fullFHAbsXYZ[1] = y*mm;
        	m_hgcConfiguration.fh.fullFHAbsXYZ[2] = z*mm;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.fh.fullFHPadXYZ[0] = x*mm;
        	m_hgcConfiguration.fh.fullFHPadXYZ[1] = y*mm;
        	m_hgcConfiguration.fh.fullFHPadXYZ[2] = z*mm;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.fh.fullFHPadReadoutXYZ[0] = x*mm;
        	m_hgcConfiguration.fh.fullFHPadReadoutXYZ[1] = y*mm;
        	m_hgcConfiguration.fh.fullFHPadReadoutXYZ[2] = z*mm;

        	//	BH Input Part
        	//
        	hgConfigFile >> n >> iMat;
        	m_hgcConfiguration.bh.nLayers_Total = n;
        	m_hgcConfiguration.bh.iAbsMaterial = iMat;
        	if (iMat == 1)
        		m_hgcConfiguration.bh.absMat = m_mmanager.mBrass;
        	else
        		m_hgcConfiguration.bh.absMat = m_mmanager.mCu;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.bh.fullBHAbsXYZ[0] = x*mm;
        	m_hgcConfiguration.bh.fullBHAbsXYZ[1] = y*mm;
        	m_hgcConfiguration.bh.fullBHAbsXYZ[2] = z*mm;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.bh.fullBHPadXYZ[0] = x*mm;
        	m_hgcConfiguration.bh.fullBHPadXYZ[1] = y*mm;
        	m_hgcConfiguration.bh.fullBHPadXYZ[2] = z*mm;

        	hgConfigFile >> x >> y >> z;
        	m_hgcConfiguration.bh.fullBHPadReadoutXYZ[0] = x*mm;
        	m_hgcConfiguration.bh.fullBHPadReadoutXYZ[1] = y*mm;
        	m_hgcConfiguration.bh.fullBHPadReadoutXYZ[2] = z*mm;

            hgConfigFile >> iMat;
            if (iMat==0)
                m_hgcConfiguration.pads.mElectronicsMat = m_mmanager.mPCB;
            else 
                m_hgcConfiguration.pads.mElectronicsMat = m_mmanager.mG10;
            */

        	return 1;
        }//	end of Read HGCAL Config Data
    }
}
