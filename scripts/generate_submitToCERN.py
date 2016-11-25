#!/usr/bin/python

import os, sys

# define some common paths
cmsswdir = "/afs/cern.ch/work/v/vkhriste/Projects/HiggsAnalysis/CMSSW_8_0_20/src"
dirToLaunchFrom = "/Users/vk/software/simulations/build-2/submissions"
executable = "/Users/vk/software/simulations/Simulations/bin/simulate_hgcal"
geomConfigFile = "/Users/vk/software/simulations/Simulations/hgc/config/geomConfigFile.geom"
outputLocation = "/afs/cern.ch/work/v/vkhriste/Projects/data/Simulations/hgcal"
#dirToLaunchFrom = "/afs/cern.ch/work/v/vkhriste/Projects/Simulations/bin/submissions"

# build up a path for this submission
if not os.path.exists(dirToLaunchFrom):
    os.system("mkdir %s" % dirToLaunchFrom)
import datetime
currentTime = datetime.datetime.now().strftime("%Y%m%d_%H%M")
dirToLaunchFrom = os.path.join(dirToLaunchFrom, currentTime)
if not os.path.exists(dirToLaunchFrom):
    os.system("mkdir %s" % dirToLaunchFrom)

# build up a path for output files
outputLocation = os.path.join(outputLocation, currentTime)
if not os.path.exists(outputLocation):
    os.system("mkdir %s" % outputLocation)

# place a description into that folder
descFile = open(os.path.join(dirToLaunchFrom, "description.desc"), "w")
desc = """
Submitting to generate 20K events per energy.
Need it for:
    1) Validating Resolution - 1K is not enough
    2) Start trainging Machine Learning
"""
descFile.write(desc)
descFile.close()

def main():
    energies = [1, 2, 4, 8, 16, 32, 50, 60]
    nEvents = 20000
    eventsPerJob = 200

    import time, random
    random.seed(time.time())
    joblist = []
    for energy in energies:
        for ievents in range(nEvents/eventsPerJob):
            seed = random.randint(0, int(time.time()))
            output = "hgcaldata__%d__%d.root" % (energy, seed)
            output = os.path.join(outputLocation, output)
            cmd = ("{executable} --isInteractive=0 --energy={energy} --particle=e- --numEvents={numEvents} --seed={seed} --verbose=0 --geomConfigFile={geomConfigFile} --output={output}").format(executable=executable, energy=energy, 
                numEvents=eventsPerJob, seed=seed, geomConfigFile=geomConfigFile, 
                output=output)
            launcherName = "laucnher__%d__%d.sh" % (energy, seed)
            launcher = open(os.path.join(dirToLaunchFrom, launcherName), "w")

            # get to cmssw and issue cmsenv
            launcher.write("cd %s\n" % cmsswdir)
            launcher.write("eval `scramv1 runtime -sh`\n")
            launcher.write("source %s\n" % os.path.join(
                os.environ["SIMULATIONSHOME"], "config", "env.sh"))
            launcher.write("%s\n" % cmd)
            launcher.close()
            os.system("chmod 755 %s" % os.path.join(dirToLaunchFrom, launcherName))
            joblist.append("bsub -q {queue} -o {logfile} -e {errorfile} {launcherscript}".format(queue="1nh" if energy<16 else "8nh", logfile=os.path.join(dirToLaunchFrom, "log__%d.log" % (
                seed)), errorfile=os.path.join(dirToLaunchFrom, "error__%d.log" % (
                seed)), launcherscript=os.path.join(dirToLaunchFrom, 
                    "launcher__%d__%d.sh" % (energy, seed))))

    # writting all the launchers into the submitter
    submitter = open(os.path.join(dirToLaunchFrom, "submitter.sh"), "w")
    for cmd in joblist:
        submitter.write("%s\n" % cmd)
    submitter.close()
    os.system("chmod 755 %s" % os.path.join(dirToLaunchFrom, "submitter.sh"))
if __name__=="__main__":
    main()
