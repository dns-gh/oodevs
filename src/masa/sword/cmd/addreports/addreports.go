// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package main

import (
	"flag"
	"fmt"
	"log"
	"masa/sword/swapi"
	"math/rand"
	"os"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

func createClient(addr, user, password string) *swapi.Client {
	client, err := swapi.NewClient(addr)
	if err != nil {
		log.Fatalf("error: could not create client: %s", err)
	}
	swapi.ConnectClient(client)
	err = client.Login(user, password)
	if err != nil {
		log.Fatalf("could not login: %s", err)
	}
	return client
}

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, strings.TrimSpace(`
addreports creates a swapi.Client, connects it to a simulation and sends
debug_internal magic actions to create reports. Simulation must be run with the
testCommands flag.

With -unit=0, the reports will be spread over all readable model units.
`)+"\n")
		flag.PrintDefaults()
	}
	clientCount := flag.Int("clients", 1, "connections count")
	jobs := flag.Int("jobs", 1, "concurrent jobs per connection")
	host := flag.String("host", "localhost", "simulation server host name")
	port := flag.Uint("port", 10001, "simulation server port")
	user := flag.String("user", "", "user name")
	password := flag.String("password", "", "user password")

	report := flag.Uint("report", 0, "report identifier, 'NTR' report by default")
	number := flag.Uint("number", 100, "total number of reports to emit, 0 means infinity")
	block := flag.Uint("block", 100, "number of reports per client call")
	unit := flag.Uint("unit", 0, "recipient of reports")
	flag.Parse()

	// Connect clients and start jobs
	addr := fmt.Sprintf("%s:%d", *host, *port)
	log.Printf("connecting to %s\n", addr)
	log.Printf("logging in as %s\n", *user)

	// Create logger client
	logger := createClient(addr, *user, *password)
	ok := logger.Model.WaitReady(10 * time.Second)
	if !ok {
		log.Fatalf("local model took too long to initialize")
	}

	entities := []uint32{}
	if *unit == 0 {
		// List all exercise units, pick randomly when emitting reports
		model := logger.Model.GetData()
		for id := range model.Units {
			entities = append(entities, id)
		}
		fmt.Printf("spreading reports over %d entities\n", len(entities))
	} else {
		entities = []uint32{uint32(*unit)}
	}

	// Get tick information
	tick := int32(0)
	reports := int32(0)
	go func() {
		prevNow := time.Now()
		prevCount := reports
		logger.Model.RegisterHandlerTimeout(0,
			func(data *swapi.ModelData, msg *swapi.SwordMessage, err error) bool {
				if err != nil {
					log.Printf("tick handler error: %s", err)
					return true
				}

				count := atomic.LoadInt32(&reports)
				if tick < data.Tick {
					tick = data.Tick
					now := time.Now()
					dtime := now.Sub(prevNow)
					created := float64(0)
					if dtime > 0 {
						created = float64(count-prevCount) / (float64(dtime) / float64(time.Second))
					}
					log.Printf("Tick %d, %.2f sec, reports %d, %.1f reports/s\n", tick,
						float64(dtime)/float64(time.Second), reports, created)
					prevNow = now
					prevCount = count
				}
				return false
			})
	}()

	// Get report count information
	logger.Model.RegisterHandlerTimeout(0,
		func(data *swapi.ModelData, msg *swapi.SwordMessage, err error) bool {
			if err != nil {
				log.Printf("handler error: %s", err)
				return true
			}
			sim := msg.SimulationToClient
			if sim == nil || sim.Message == nil {
				return false
			}
			report := sim.Message.Report
			if report == nil {
				return false
			}
			atomic.AddInt32(&reports, 1)
			return false
		})

	// Send reports
	blocks := make(chan uint, *jobs)
	wg := sync.WaitGroup{}
	for i := 0; i != *clientCount; i++ {
		client := createClient(addr, *user, *password)
		for j := 0; j != *jobs; j++ {
			wg.Add(1)
			go func() {
				defer wg.Done()
				for n := range blocks {
					id := entities[rand.Intn(len(entities))]
					err := client.CreateReport(uint32(n), uint32(*report), uint32(id))
					if err != nil {
						fmt.Errorf("could not create report: %s", err)
					}
				}
			}()
		}
	}

	remaining := *number
	for *number == 0 || remaining > 0 {
		n := *block
		if *number > 0 && n > remaining {
			n = remaining
		}
		remaining -= n
		blocks <- n
	}
	close(blocks)
	wg.Wait()

	logger.Model.WaitTicks(1)
	logger.Close()
}
