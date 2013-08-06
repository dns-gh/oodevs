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
	"os"
	"swapi"
)

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`testclient creates an swapi.Client, connects it to a simulation, logs in
with supplied credentials and waits for connection termination. It is mostly
used to exercise swapi.Model update against real world scenarii.

`)
		flag.PrintDefaults()
	}
	host := flag.String("host", "localhost", "simulation server host name")
	port := flag.Uint("port", 10001, "simulation server port")
	user := flag.String("user", "", "user name")
	resume := flag.Bool("resume", false, "resume the simulation once logged in")
	password := flag.String("password", "", "user password")

	flag.Parse()
	addr := fmt.Sprintf("%s:%d", *host, *port)
	log.Printf("connecting to %s\n", addr)
	client, err := swapi.Connect(addr)
	if err != nil {
		log.Fatalf("error: could not connect client: %s", err)
	}
	termination := make(chan int)
	client.Register(func(msg *swapi.SwordMessage, ctx int32, err error) bool {
		if err != nil {
			close(termination)
			return true
		}
		return false
	})

	tick := int32(0)
	go func() {
		client.Model.WaitConditionTimeout(0, func(data *swapi.ModelData) bool {
			if tick < data.Tick {
				tick = data.Tick
				units := len(data.ListUnits())
				automats := len(data.ListAutomats())
				log.Printf("Tick %d, %d units, %d automats\n", tick, units, automats)
			}
			return false
		})
	}()

	log.Printf("logging in as %s\n", *user)
	err = client.Login(*user, *password)
	if err != nil {
		log.Fatalf("error: could not login: %s", err)
	}
	if *resume {
		err = client.Resume(0)
		if err != nil {
			log.Fatalf("error: could not resume the simulation: %s", err)
		}
	}
	<-termination
}
