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
	"crypto/rand"
	"encoding/hex"
	"flag"
	"fmt"
	"log"
	"masa/sword/swapi"
	"os"
	"sync"
	"time"
)

// Returns a random string of size characters.
func makePayload(size int) (string, error) {
	data := make([]byte, (size+1)/2)
	_, err := rand.Read(data)
	if err != nil {
		return "", err
	}
	return hex.EncodeToString(data)[:size], nil
}

// Calls client.Echo endlessly with a fixed payload of payloadSize characters,
// and check returned value.
func stress(payloadSize int, client *swapi.Client) error {
	payload, err := makePayload(payloadSize)
	if err != nil {
		return err
	}
	for {
		result, err := client.Echo(payload)
		if err != nil {
			return err
		}
		if result != payload {
			return fmt.Errorf("input differs from output: %s != %s", payload, result)
		}
	}
}

func fatal(format string, args ...interface{}) {
	fmt.Fprintf(os.Stderr, format, args...)
	os.Exit(1)
}

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, `stress a simulation server with echo messages

stressclient connects --clients independant clients to a running simulation
using authentication keys. Each client spawns --jobs goroutines, each endlessly
sending and checking echo messages of approximatively --payload characters.
Note that echo messages are broadcast to all clients, and roundtrip between
the dispatcher and the simulation. The simulation must be started with
--test-command for the echoes to be accepted.

`)
		flag.PrintDefaults()
	}
	clientCount := flag.Int("clients", 1, "connections count")
	jobs := flag.Int("jobs", 1, "concurrent jobs per connection")
	payloadSize := flag.Int("payload", 256, "message size")
	host := flag.String("host", "localhost", "simulation server host name")
	port := flag.Uint("port", 10001, "simulation server port")
	user := flag.String("user", "", "user name")
	flag.Parse()

	if *clientCount <= 0 {
		fatal("error: --clients must be greater than zero\n")
	}
	if *jobs <= 0 {
		fatal("error: --jobs must be greater than zero\n")
	}
	if *payloadSize <= 0 {
		fatal("error: --payload must be greater than zero\n")
	}

	// Log inputs and outputs
	sentch := make(chan int, 5*(*jobs)*(*clientCount))
	go func() {
		sent := 0
		received := 0
		in := 0
		out := 0
		started := false
		start := time.Now()
		ticker := time.NewTicker(1 * time.Second)
		defer ticker.Stop()
		for {
			select {
			case now := <-ticker.C:
				if !started {
					continue
				}
				duration := now.Sub(start)
				inrate := float64(received) / (1024 * float64(duration) / float64(time.Second))
				outrate := float64(sent) / (1024 * float64(duration) / float64(time.Second))
				log.Printf("in: %d %.1f kb/s, out: %d %.1f kb/s\n",
					in, inrate, out, outrate)
				sent = 0
				received = 0
				in = 0
				out = 0
				start = now
			case size := <-sentch:
				started = true
				if size >= 0 {
					received += size
					in++
				} else {
					sent += -size
					out++
				}
			}
		}
	}()

	// Connect clients and start jobs
	addr := fmt.Sprintf("%s:%d", *host, *port)
	log.Printf("connecting to %s\n", addr)
	log.Printf("logging in as %s\n", *user)
	wg := sync.WaitGroup{}
	for i := 0; i != *clientCount; i++ {
		client, err := swapi.NewClient(addr)
		if err != nil {
			log.Fatalf("error: could not create client: %s", err)
		}
		client.Logger = func(in bool, size int, msg *swapi.SwordMessage) {
			if !in {
				size = -size
			}
			sentch <- size
		}
		swapi.ConnectClient(client)

		key, err := client.GetAuthenticationKey()
		if err != nil {
			log.Fatalf("error: could not login: %s", err)
		}
		err = client.LoginWithAuthenticationKey(*user, "", key)
		if err != nil {
			log.Fatalf("error: could not login: %s", err)
		}

		for j := 0; j != *jobs; j++ {
			wg.Add(1)
			go func() {
				defer wg.Done()
				err := stress(*payloadSize, client)
				if err != nil {
					log.Printf("error: %s\n", err)
				}
			}()
		}
	}
	wg.Wait()
}
