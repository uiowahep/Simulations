/*
 * HGCAL Standalone simulation
 */

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"

#include "QBBC.hh"
//#include "QGSP.hh"

#include "hgc/include/DetectorConstruction.hh"
#include "hgc/include/PrimaryGeneratorAction.hh"
#include "hgc/include/EventAction.hh"
#include "hgc/include/RunAction.hh"
#include "hgc/include/defs.h"
#include "hgc/include/SteppingAction.hh"

#include <time.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"

#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"

#undef times
#include "boost/program_options.hpp"

using namespace std;
using namespace CLHEP;
using namespace simulations::hgc;
namespace po = boost::program_options;

/*
 * main function
 */
int main(int argc, char** argv)
{
    //  defaults
    double energyd = 5.0;
    std::string pnamed = "e-";
    std::string geomConfigFile = "nothing.hgc";
    int neventsd = 1000;
    int seedd = 1;
    bool verbose = false;
    std::string outputd;

    // input program options
    po::options_description desc("Allowed Program Options");
    desc.add_options()
        ("help", "produce help messages")
        ("isInteractive", po::value<bool>(), "true for interactive mode")
        ("energy", po::value<double>(&energyd)->default_value(energyd), "energy of the gun")
        ("particle", po::value<std::string>(&pnamed)->default_value(pnamed), "particle name for the gun")
        ("numEvents", po::value<int>(&neventsd)->default_value(neventsd), "number of events to generate")
        ("seed", po::value<int>(&seedd)->default_value(seedd), "seed")
        ("verbose", po::value<bool>(&verbose)->default_value(verbose), "run in verbose mode if true")
        ("geomConfigFile", po::value<std::string>(&geomConfigFile)->default_value(geomConfigFile))
        ("output", po::value<std::string>(&outputd)->default_value(outputd), "output ROOT file pathname")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help") || argc<2)
    {
        std::cout << desc << std::endl;
        return 1;
    }

    RunConfiguration runConfiguration;
    runConfiguration.isInteractive = vm["isInteractive"].as<bool>();
    runConfiguration.energy = vm["energy"].as<double>();
    runConfiguration.pName = vm["particle"].as<std::string>();
    runConfiguration.numEvents = vm["numEvents"].as<int>();
    runConfiguration.seed = vm["seed"].as<int>();
    runConfiguration.verbose = vm["verbose"].as<bool>();
    runConfiguration.geomConfigFileName = vm["geomConfigFile"].as<std::string>();
    runConfiguration.output = vm["output"].as<std::string>();

	// initialize Random Number Generator
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
//	CLHEP::HepRandom::setTheSeed(time(NULL));
	CLHEP::HepRandom::setTheSeed(runConfiguration.seed);
	CLHEP::HepRandom::showEngineStatus();

    //
	//	Initialize ROOT Output/ EM, FH, BH parts are separate...
	//	As of V5.
	//
    TFile *out = new TFile(runConfiguration.output.c_str(), "RECREATE");
    TTree *tree = new TTree("Events", "Events");

    //
	//	Initialize G4 Classes
	//
	G4RunManager *runManager = new G4RunManager;
	DetectorConstruction *detector = 
		new DetectorConstruction(runConfiguration, tree);
	runManager->SetUserInitialization(detector);
	
	G4PhysListFactory factory;
	G4VModularPhysicsList *phys = factory.GetReferencePhysList("FTFP_BERT");
	runManager->SetUserInitialization(phys);

    // generator
	PrimaryGeneratorAction *primAction = 
		new PrimaryGeneratorAction(runConfiguration, tree);
	runManager->SetUserAction(primAction);

    //  actions
	EventAction *eventAction = new EventAction(tree);
	RunAction *runAction = new RunAction();
	SteppingAction *steppingAction = new SteppingAction(runConfiguration);
	runManager->SetUserAction(runAction);
	runManager->SetUserAction(eventAction);
	runManager->SetUserAction(steppingAction);

	// Init G4 Kernel
	//
	runManager->Initialize();
	G4UImanager *UI = G4UImanager::GetUIpointer();

	if (runConfiguration.isInteractive)
	{
		G4VisManager *visManager = 0;
		#ifdef G4VIS_USE
			//	Visualization
			//
			visManager = new G4VisExecutive;
			visManager->Initialize();
		#endif

		#ifdef G4UI_USE
			G4UIExecutive *ui = new G4UIExecutive(argc, argv);
			ui->SessionStart();
			delete ui;
		#endif
	}
	else
	{
		char cmd[200];
		sprintf(cmd, "/run/beamOn %d", runConfiguration.numEvents);
		UI->ApplyCommand(cmd);
	}

	// Finalize
	delete runManager;
	out->Print();
	out->Write();
	out->Close();

	return 0;
} 
