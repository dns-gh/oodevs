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
	"swapi"
)

func main() {
	log.Println("Masa Sword client - copyright Masa Group 2013")

	host := flag.String("host", "localhost", "server host name")
	port := flag.Int("port", 10001, "server port")
	flag.Parse()
	log.Println("host:", *host)
	log.Println("port:", *port)

	client, err := swapi.Connect(fmt.Sprintf("%v:%v", *host, *port))
	if err != nil {
		log.Fatal(err)
	}
	err = client.Login("Supervisor", "foo")
	if err != nil {
		log.Println("authentication error", err)
	}
	err = client.Login("Supervisor", "")
	if err != nil {
		log.Println("authentication error", err)
	} else {
		log.Println("client connected")
	}
	client.Close()
}
