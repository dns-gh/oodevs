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
	"time"
)

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, ""+
			`
`)
		flag.PrintDefaults()
	}
	host := flag.String("host", "localhost", "simulation server host name")
	port := flag.Uint("port", 10001, "simulation server port")
	user := flag.String("user", "Superviseur", "user name")
	physical := flag.String("physical", "C:/ldc/models-algeria/data/data/models/algerie/physical", "physical model directory")
	outfile := flag.String("out", "Output.csv", "Output file")
	flag.Parse()

	addr := fmt.Sprintf("%s:%d", *host, *port)
	log.Printf("connecting to %s\n", addr)
	log.Printf("logging in as %s\n", *user)
	client, err := swapi.Connect(addr)
	if err != nil {
		log.Fatalf("error: could not create client: %s", err)
	}
	key, err := client.GetAuthenticationKey()
	if err != nil {
		log.Fatalf("error: could not login: %s", err)
	}
	err = client.LoginWithAuthenticationKey(*user, "", key)
	if err != nil {
		log.Fatalf("error: could not login: %s", err)
	}
	client.Model.WaitReady(2 * time.Second)
	data := client.Model.GetData()
	log.Printf("--Data read")
	resources, err := ReadResources(*physical + "/defense-v1/Dotations.xml")
	if err != nil {
		log.Fatalf("Error reading resources: %s", err)
	}
	output, err := os.OpenFile(*outfile, os.O_RDWR|os.O_APPEND, 0660)
	if err != nil {
		log.Fatalf("Error opening Output.csv %s", err)
	}
	defer output.Close()
	//idealement quantite en UF
	fmt.Fprintf(output, "Id Unite;Nom Unite;Dotation;Quantite;Masse unitaire;Volume unitaire;Consommation journalière;Quantite en UF;\n")
	for id, unit := range data.Units {
		for _, stock := range unit.Stocks {
			log.Printf("Stock found")
			amount := stock.Quantity
			resource, err := resources.GetResource(stock.Type)
			packageSize := float64(resource.PackageSize)
			mass := resource.Mass / packageSize
			volume := resource.Volume / packageSize
			if err != nil {
				log.Fatalf("Error %s getting resource %d\n", err, stock.Type)
			}
			fmt.Fprintf(output, "%d;%s;%s;%d;%f;%f\n", id, unit.Name, resource.Name, amount, float64(amount)*mass, float64(amount)*volume)
		}
	}
}
