package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
	"sync"
)

func copyDir(srcDir, dstDir string) error {
	// Copy old physical database
	err := os.RemoveAll(dstDir)
	if err != nil {
		return err
	}
	err = os.MkdirAll(dstDir, 0700)
	if err != nil {
		return err
	}
	err = filepath.Walk(srcDir, func(path string, fi os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		relPath, err := filepath.Rel(srcDir, path)
		if err != nil {
			return err
		}
		destPath := filepath.Join(dstDir, relPath)
		if fi.IsDir() {
			err = os.Mkdir(destPath, 0700)
			if err != nil && os.IsExist(err) {
				err = nil
			}
		} else {
			data, err := ioutil.ReadFile(path)
			if err != nil {
				return err
			}
			err = ioutil.WriteFile(destPath, data, 0644)
		}
		return err
	})
	return err
}

func runAdn(exePath, runDir, srcDir, dstDir string) error {
	err := os.RemoveAll(dstDir)
	if err != nil {
		return err
	}
	cmd := exec.Command(exePath,
		"-i", filepath.Join(srcDir, "physical.xml"),
		"-o", filepath.Join(dstDir, "physical.xml"),
	)
	cmd.Dir = runDir
	output, err := cmd.Output()
	if err != nil {
		fmt.Fprintf(os.Stderr, "adaptation failed:\n%s\n", output)
	}
	return err
}

func runSimulation(exePath, runDir, dataDir, exercise string, basePort int) error {
	cmd := exec.Command(exePath,
		"--root-dir="+dataDir,
		"--exercise="+exercise,
		"--session=default",
		fmt.Sprintf("--simulation-address=%d", basePort),
		fmt.Sprintf("--dispatcher-address=%d", basePort+1),
		"--test",
	)
	cmd.Dir = runDir
	output, err := cmd.Output()
	if err != nil {
		fmt.Fprintf(os.Stderr, "simulation failed:\n%s\n", string(output))
	}
	return err
}

func migrate(rootDir, dataDir, outDir, runDir, exercise, oldVersion string,
	basePort int) error {

	// Copy old physical database
	phyName := fmt.Sprintf("migrations-%d", basePort)
	tempPhyDir := filepath.Join(dataDir, "data/models/ada/physical", phyName)
	phyDir := filepath.Join(dataDir,
		"tests/test_data_migration/"+oldVersion+"/physical")
	err := copyDir(phyDir, tempPhyDir)
	if err != nil {
		return err
	}
	defer os.RemoveAll(tempPhyDir)

	// Test the physical database can be migrated with ADN
	adnExe := filepath.Join(outDir, "release/adaptation_app.exe")
	updatedDir := filepath.Join(outDir, phyName)
	err = runAdn(adnExe, runDir, tempPhyDir, updatedDir)
	if err != nil {
		return err
	}

	// Copy exercise
	exerciseDir := filepath.Join(dataDir, "tests/test_data_migration",
		oldVersion, "exercises", exercise)
	tempExerciseDir := filepath.Join(dataDir, "exercises", phyName)
	err = copyDir(exerciseDir, tempExerciseDir)
	if err != nil {
		return err
	}
	defer os.RemoveAll(tempExerciseDir)
	exerciseFile := filepath.Join(tempExerciseDir, "exercise.xml")
	data, err := ioutil.ReadFile(exerciseFile)
	if err != nil {
		return err
	}
	data = bytes.Replace(data, []byte("@DATASET@"), []byte("ada"), -1)
	data = bytes.Replace(data, []byte("@PHYSICAL@"), []byte(phyName), -1)
	err = ioutil.WriteFile(exerciseFile, data, 0644)
	if err != nil {
		return err
	}

	// Check the simulation starts and ticks
	simExe := filepath.Join(outDir, "release/simulation_app.exe")
	return runSimulation(simExe, runDir, dataDir, phyName, basePort)
}

func migrateAll() error {
	args := os.Args[1:]
	if len(args) < 7 {
		return fmt.Errorf(`not enough arguments.
ROOTDIR DATADIR OUTDIR RUNDIR BASEPORT JOBS [EXERCISES...] expected`)
	}
	rootDir := args[0]
	dataDir := args[1]
	outDir := args[2]
	runDir := args[3]
	port, err := strconv.ParseInt(args[4], 10, 32)
	if err != nil {
		return fmt.Errorf("invalid port: %s", args[4])
	}
	basePort := int(port)
	jobs, err := strconv.ParseInt(args[5], 10, 32)
	if err != nil {
		return fmt.Errorf("invalid job count: %s", args[5])
	}
	runtime.GOMAXPROCS(int(jobs))

	// Exercises arguments are passed like "EXERCISENAME,VERSION"
	tests := args[6:]
	exercises := make(chan [2]string, len(tests))
	for _, arg := range tests {
		parts := strings.Split(arg, ",")
		if len(parts) != 2 {
			return fmt.Errorf("invalid argument")
		}
		exercise := parts[0]
		oldVersion := parts[1]
		exercises <- [2]string{exercise, oldVersion}
	}
	close(exercises)

	// Setting GOMAXPROC is not enough to control concurrency because of system
	// calls. Use goroutine workers instead.
	running := &sync.WaitGroup{}
	results := make(chan error, len(tests))
	for i := 0; i != int(jobs); i++ {
		port := basePort + 5*i
		running.Add(1)
		go func() {
			defer running.Done()
			for {
				select {
				case ex, ok := <-exercises:
					if !ok {
						return
					}
					err := migrate(rootDir, dataDir, outDir, runDir, ex[0], ex[1], port)
					if err != nil {
						err = fmt.Errorf("%s/%s migration failed: %s", ex[0], ex[1], err)
					}
					results <- err
				}
			}
		}()
	}
	running.Wait()
	close(results)

	var result error
	for err := range results {
		if err != nil {
			fmt.Fprintf(os.Stderr, "%s", err)
			result = fmt.Errorf("migration tests failed")
		}
	}
	return result
}

func main() {
	err := migrateAll()
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s\n", err)
		os.Exit(1)
	}
}
