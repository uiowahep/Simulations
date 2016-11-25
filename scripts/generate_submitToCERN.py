#!/usr/bin/python

# define some common paths
cmsswdir = "/afs/cern.ch/work/v/vkhriste/Projects/HiggsAnalysis/CMSSW_8_0_20/src"
dirToLaunchFrom = "/afs/cern.ch/work/v/vkhriste/Projects/Simulations/bin/submissions"

# build up a path for this submission
if not os.path.exists(dirToLaunchFrom):
    os.system("mkdir %s" % dirToLaunchFrom)
import datetime
currentTime = datetime.datetime.now().strftime("%Y%m%d_%H%M")
dirToLaunchFrom = os.path.join(dirToLaunchFrom, currentTime)
if not os.path.exists(dirToLaunchFrom):
    os.system("mkdir %s" % dirToLaunchFrom)

# place a description into that folder
descFile = open(os.path.join(dirToLaunchFrom, "description.desc"))
desc = """
Submitting to generate 20K events per energy.
Need it for:
    1) Validating Resolution - 1K is not enough
    2) Start trainging Machine Learning
"""
desc.write(desc)
desc.close()

def main():
    energies = [1, 2, 4, 8, 16, 32, 50, 60]
    nEvents = 20000
    eventsPerJob = 200

    import time, random
    random.seed(time.time())
    cmdlist = []
    for energy in energies:
        for ievents in range(nEvents/eventsPerJob):
            seed = random.randint(0, time.time())
            cmd = ("{executable} --isInteractive=0 --energy=${energy} --particle=e- --numEvents=${numEvents} --seed=${seed} --verbose=0 --geomConfigFile=${geomConfigFile} --output=${output}").format(executable=executable, energy=energy, 
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
            os.system("chmod 755 %s" % os.path.join(dirToLaunchFrom, launchername))
            joblist.append("bsub -q {queue} -o {logfile} -e {errorfile} {launcherscript}".format(queue="1nh" if energy<16 else "8nh", logfile=os.path.join(dirToLaunchFrom, "log__%d.log" % (
                seed)), errorfile=os.path.join(dirToLaunchFrom, "error__%d.log" % (
                seed)), launcherscript=os.path.join(dirToLaunchFrom, 
                    "launcher__%d__%d.sh" % (energy, seed))))

if __name__=="__main__":
    main()
