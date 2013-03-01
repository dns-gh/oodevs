package main

import (
	"log"
	"os"
	"path/filepath"
	"swapi/simu"
	"time"
)

func main() {
	opts := simu.SimOpts{}
	opts.Executable = "d:/sword/trunk-vc100_x64/applications/simulation_app.exe"
	opts.RootDir = "d:/sword/trunk-data"
	opts.DataDir = opts.RootDir
	opts.ExerciseName = "Egypt"
	opts.ConnectTimeout = 20 * time.Second

	exDir := opts.GetExerciseDir()
	session := simu.CreateDefaultSession()
	session.EndTick = 4
	session.Paused = false
	sessionPath, err := simu.WriteNewSessionFile(session, exDir)
	if err != nil {
		log.Printf("failed to write the session\n")
		os.Exit(1)
	}
	opts.SessionName = filepath.Base(filepath.Dir(sessionPath))
	sim, err := simu.StartSim(&opts)
	if err != nil {
		log.Printf("sim failed to start: %v", err)
		os.Exit(1)
	}
	sim.Wait(30 * time.Second)
}
