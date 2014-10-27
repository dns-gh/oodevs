// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************

// Simulation benchmarking tool, see flag usage string for details.
package main

import (
	"bufio"
	"encoding/xml"
	"flag"
	"fmt"
	"masa/sword/swapi"
	"masa/sword/swapi/phy"
	"masa/sword/swapi/simu"
	"masa/sword/swrun"
	"masa/sword/swtest"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"
	"time"
)

var (
	Cfg *swtest.Config
)

const (
	ExSwTerrain2Empty = "sw-terrain2-empty"
)

func NewAdminOpts(exercise string) *swrun.ClientOpts {
	return &swrun.ClientOpts{
		Exercise: exercise,
		User:     "admin",
		Password: "",
	}
}

func connectAndWaitModel(opts *swrun.ClientOpts) (*simu.SimProcess, *swapi.Client, error) {
	return swrun.ConnectAndWaitModel(opts, Cfg)
}

func loadPhysicalData(dataDir, name string) (*phy.PhysicalData, error) {
	phyDir := filepath.Join(dataDir, "data/models/ada/physical")
	path := filepath.Join(phyDir, name)
	return phy.ReadPhysicalData(path)
}

func getVersionAndDate() (string, time.Time, error) {
	date := time.Unix(0, 0)
	// Start and stop the simulation
	sim, client, err := connectAndWaitModel(NewAdminOpts(ExSwTerrain2Empty))
	if err != nil {
		return "", date, err
	}
	swrun.StopSimAndClient(sim, client)

	// Extract date and version from sim.log
	// [2014-10-27 13:59:10] <Simulation> <info> Sword Simulation - Version 6.0.0.0.abab - Release
	reStartup := regexp.MustCompile(`^\[([^\]]+).*Sword Simulation - Version\s+(\S+)`)
	path := sim.Opts.GetSimLogPath()
	fp, err := os.Open(path)
	if err != nil {
		return "", date, fmt.Errorf("cannot open sim.log: %s", err)
	}
	defer fp.Close()
	scanner := bufio.NewScanner(fp)
	for scanner.Scan() {
		m := reStartup.FindStringSubmatch(scanner.Text())
		if m == nil {
			continue
		}
		d, err := time.Parse("2006-01-02 15:04:06", m[1])
		if err != nil {
			return "", date, fmt.Errorf("unexpected date: %s", m[1])
		}
		version := m[2]
		return version, d, nil
	}
	return "", date, fmt.Errorf("could not find date or version in sim.log: %s",
		filepath.ToSlash(path))
}

// Repeatedly computes a single path, sequentially.
func benchmarkPathfind(from, to swapi.Point, quick bool) ([]float64, error) {
	sim, client, err := connectAndWaitModel(NewAdminOpts(ExSwTerrain2Empty))
	if err != nil {
		return nil, err
	}
	defer swrun.StopSimAndClient(sim, client)
	phydb, err := loadPhysicalData(sim.Opts.DataDir, "test")
	if err != nil {
		return nil, err
	}

	// Create a single VW Combi unit
	party := &swrun.Party{
		Name: "party1",
		Formations: []*swrun.Formation{
			&swrun.Formation{
				Name: "formation1",
				Automats: []*swrun.Automat{
					&swrun.Automat{
						Name: "automat",
						Type: "VW Combi Rally",
						Units: []*swrun.Unit{
							&swrun.Unit{
								Name: "unit",
								Type: "VW Combi",
							},
						},
					},
				},
			},
		},
	}
	err = swrun.FindOrCreateEntities(client, phydb, party)
	if err != nil {
		return nil, err
	}

	unit := party.Formations[0].Automats[0].Units[0].Entity

	loops := 10
	if quick {
		loops = 1
	}
	durations := []float64{}
	for i := 0; i < loops; i++ {
		start := time.Now()
		points, err := client.UnitPathfindRequest(unit.Id, from, to)
		if err != nil {
			return nil, err
		}
		if len(points) < 10 {
			return nil, fmt.Errorf("not enough points in pathfind")
		}
		end := time.Now()
		durations = append(durations, float64(end.Sub(start)/time.Millisecond))
	}

	return durations, nil
}

// Short pathfind to measure call overhead.
func BenchmarkPathfindShort(quick bool) ([]float64, error) {
	from := swapi.Point{X: 14.9611, Y: 58.6744}
	to := swapi.Point{X: 14.9585, Y: 58.6753}
	return benchmarkPathfind(from, to, quick)
}

// Pathfind across the map.
func BenchmarkPathfindLong(quick bool) ([]float64, error) {
	from := swapi.Point{X: 14.7215, Y: 58.2794}
	to := swapi.Point{X: 16.4767, Y: 58.7091}
	return benchmarkPathfind(from, to, quick)
}

// Impossible pathfind request, from mainland to an island.
func BenchmarkPathfindImpossible(quick bool) ([]float64, error) {
	from := swapi.Point{X: 14.9611, Y: 58.6744}
	to := swapi.Point{X: 15.0367, Y: 58.6495}
	return benchmarkPathfind(from, to, quick)
}

// A single measure
type BenchmarkResult struct {
	Name     string  `xml:"name,attr"`     // Measure identifier
	Time     int64   `xml:"time,attr"`     // Date as number of seconds since 1970-01-01
	Value    float64 `xml:"value,attr"`    // Measure value
	Version  string  `xml:"version,attr"`  // Sword version as X.Y.*
	Exercise string  `xml:"exercise,attr"` // Source exercise, usually ignored
	Comment  string  `xml:"comment,attr"`
}

type BenchmarkResults struct {
	XMLName xml.Name           `xml:"results"`
	Results []*BenchmarkResult `xml:"result"`
}

// Benchmarking functions return an array of measures or an error.
type BenchFunc func(quick bool) ([]float64, error)

// Runs a single benchmark and synthetizes the results.
func benchmarkOne(fn BenchFunc, quick bool) (float64, error) {
	values, err := fn(quick)
	if err != nil {
		return 0, err
	}
	if len(values) == 0 {
		return 0, fmt.Errorf("no values returned")
	}
	value := values[0]
	for _, v := range values[1:] {
		if v < value {
			value = v
		}
	}
	return value, nil
}

// Writes benchmark measures as XML.
func writeResults(path string, results []*BenchmarkResult) error {
	dir := filepath.Dir(path)
	err := os.MkdirAll(dir, 755)
	if err != nil {
		return err
	}
	fp, err := os.Create(path)
	if err != nil {
		return err
	}
	defer fp.Close()
	x := &BenchmarkResults{
		Results: results,
	}
	return xml.NewEncoder(fp).Encode(x)
}

func benchmark() error {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, strings.TrimSpace(`
simbench starts simulation or related applications process and benchmarks
features using its protobuf API.

By default it runs all benchmarks and prints measured values on standard
output. With -output FILE, results are written to an XML file to be loaded in
the benchmark database for performance changes tracking. With -f PATTERN,
only benchmarks matched by supplied regular expression pattern are executed.

Without additional arguments, simbench expects to be run from sword project
root directory like:

  $ cd /path/to/sword
  $ bin/simbench

Applications path, run directory and other data configurations can be set with
various flags shared with the testing framework.
`))
		flag.PrintDefaults()
	}
	output := flag.String("output", "", "benchmark results XML file")
	filter := flag.String("f", ".*", "regular expression to filter tests")
	quick := flag.Bool("quick", false, "check benchmark execution but do not loop")
	Cfg = swtest.ParseFlags()
	flag.Parse()
	matcher, err := regexp.Compile(*filter)
	if err != nil {
		return fmt.Errorf("invalid -filter value: %s", err)
	}

	// Assume benchmarks are run from sword project root directory
	cwd, err := os.Getwd()
	if err != nil {
		return err
	}
	Cfg.BaseDir = cwd

	failed := 0
	benchmarks := map[string]BenchFunc{
		"sw_pathfind_short":      BenchmarkPathfindShort,
		"sw_pathfind_long":       BenchmarkPathfindLong,
		"sw_pathfind_impossible": BenchmarkPathfindImpossible,
	}
	sorted := []string{}
	for k, _ := range benchmarks {
		if matcher.MatchString(k) {
			sorted = append(sorted, k)
		}
	}
	sort.Strings(sorted)

	version, date, err := getVersionAndDate()
	if err != nil {
		return err
	}
	refDate := time.Unix(0, 0)
	seconds := date.Sub(refDate) / time.Second

	results := []*BenchmarkResult{}
	for _, name := range sorted {
		fn := benchmarks[name]
		start := time.Now()
		value, err := benchmarkOne(fn, *quick)
		end := time.Now()
		duration := float64(end.Sub(start)/time.Millisecond) / 1000.0
		if err != nil {
			fmt.Fprintf(os.Stderr, "FAILED %s: %s in %.1fs\n", name, err, duration)
			failed += 1
			continue
		}
		fmt.Printf("OK: %s: %f in %.1fs\n", name, value, duration)
		results = append(results, &BenchmarkResult{
			Name:     name,
			Value:    value,
			Exercise: "swbench",
			Version:  version,
			Time:     int64(seconds),
		})
	}
	if len(*output) > 0 {
		fmt.Printf("writing %s\n", *output)
		err = writeResults(*output, results)
		if err != nil {
			return err
		}
	}
	if failed > 0 {
		return fmt.Errorf("%d benchmarks failed", failed)
	}
	return nil
}

func main() {
	err := benchmark()
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s\n", err)
		os.Exit(1)
	}
}
