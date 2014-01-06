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
	"encoding/csv"
	"flag"
	"fmt"
	"log"
	"os"
	"swapi"
	"time"
)

func UintToString(d uint32) string {
	return fmt.Sprintf("%d", d)
}

func IntToString(d int32) string {
	return fmt.Sprintf("%d", d)
}

func FloatToString(f float64) string {
	return fmt.Sprintf("%f", f)
}

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
	physical := flag.String("physical", "C:/ldc/models-algeria/data/data/models/algerie/physical/defense-v1/Dotations.xml", "resources file directory")
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
	resources, err := ReadResources(*physical)
	if err != nil {
		log.Fatalf("Error reading resources: %s", err)
	}
	output, err := os.Create(*outfile)
	if err != nil {
		log.Fatalf("Error creating %s %s", *outfile, err)
	}
	defer output.Close()
	writer := csv.NewWriter(output)
	//idealement quantite en UF
	record := []string{"Id Unité", "Nom Unité", "Dotation", "Quantité", "Masse unitaire", "Volume unitaire", "Consommation journalière", "Quantité en UF"}
	writer.Write(record)
	for id, unit := range data.Units {
		for _, stock := range unit.Stocks {
			amount := stock.Quantity
			resource, err := resources.GetResource(stock.Type)
			packageSize := float64(resource.PackageSize)
			mass := resource.Mass / packageSize
			volume := resource.Volume / packageSize
			if err != nil {
				log.Fatalf("Error %s getting resource %d\n", err, stock.Type)
			}
			record = []string{UintToString(id), unit.Name, resource.Name, IntToString(amount), FloatToString(mass), FloatToString(volume)}
			writer.Write(record)
		}
	}
	writer.Flush()
}
