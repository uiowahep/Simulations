#!/usr/bin/python

def main():
    energies = [1, 2, 4, 8, 16, 32, 50, 60]
    nEvents = 20000
    eventsPerJob = 200

    seed = 1
    cmdlist = []
    for energy in energies:
        for ievents in range(nEvents/eventsPerJob):
            cmd = ("{executable} --isInteractive=0 --energy=${energy} --particle=e- --numEvents=${numEvents} --seed=${seed} --verbose=0 --geomConfigFile=${geomConfigFile} --output=${output}").format(executable=executable, energy=energy, 
                numEvents=eventsPerJob, seed=seed, geomConfigFile=geomConfigFile, 
                output=output)
            launcherName = "laucnher_%d.sh" % seed
            launcher = open(os.path.join(dirToLaunchFrom, launcherName), "w")


            seed+=1

if __name__=="__main__":
    main()
