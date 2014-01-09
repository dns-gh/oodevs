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
	"swadn"
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

func OutputResources(resources swadn.Resources, out string) {
	outResourcesFile := out + "-resources.csv"
	outResources, err := os.Create(outResourcesFile)
	if err != nil {
		log.Fatalf("Error creating %s %s", outResourcesFile, err)
	}
	defer outResources.Close()
	writer := csv.NewWriter(outResources)
	writer.Comma = ';'
	record := []string{"Ressource", "Masse", "Volume"} //, "Quantité dans une UF"
	writer.Write(record)
	for _, resource := range resources.Content {
		packageSize := float64(resource.PackageSize)
		mass := resource.Mass / packageSize
		volume := resource.Volume / packageSize
		record = []string{resource.Name, FloatToString(mass), FloatToString(volume)}
		writer.Write(record)
	}
	writer.Flush()
}

func OutputConsumptions(resources swadn.Resources, physical swadn.PhysicalFile, out string) {
	consumptions, units, err := resources.ReadNormalizedConsumptions(physical)
	if err != nil {
		log.Fatalf("Error reading consumptions %s", err)
	}
	outName := out + "-consumptions.csv"
	outFile, err := os.Create(outName)
	if err != nil {
		log.Fatalf("Error creating %s %s", outName, err)
	}
	defer outFile.Close()
	writer := csv.NewWriter(outFile)
	writer.Comma = ';'
	record := []string{"Type Unité", "Id", "Ressource", "Consommation normalisée"}
	writer.Write(record)
	for unit, consumption := range consumptions.Consumptions {
		for resourceId, amount := range consumption.Consumption {
			resource := resources.GetResource(resourceId)
			if resource == nil {
				log.Fatalf("Error getting resource %d", resourceId)
			}
			name, err := units.GetName(unit)
			if err != nil {
				log.Fatalf("Error getting unit %d", unit)
			}
			record = []string{name, UintToString(unit), resource.Name, UintToString(amount)}
			writer.Write(record)
		}
	}
	writer.Flush()
}

func ComputePhysicalData(xmlDir string, out string) (*swadn.Resources, error) {
	log.Printf("--Data read")
	physicalFile, err := swadn.ReadPhysical(xmlDir)
	if err != nil {
		log.Fatalf("Error reading physical file: %s", err)
	}
	resources, err := swadn.ReadResources(*physicalFile)
	if err != nil {
		log.Fatalf("Error reading resources: %s", err)
	}
	OutputResources(*resources, out)
	OutputConsumptions(*resources, *physicalFile, out)
	return resources, nil
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
	physical := flag.String("physical", "../data/data/models/ada/physical/worldwide", "resources file directory")
	out := flag.String("out", "Output", "Output prefix")
	root := flag.Uint("root", 0, "root logistic unit id - if 0 or unset, all units are dumped")
	flag.Parse()

	resources, err := ComputePhysicalData(*physical, *out)
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

	WriteOrbat(*out, data, *resources, uint32(*root))
}

func WriteOrbat(out string, data *swapi.ModelData, resources swadn.Resources, root uint32) {
	outName := out + "-orbat.csv"
	outFile, err := os.Create(outName)
	if err != nil {
		log.Fatalf("Error creating %s %s", outName, err)
	}
	defer outFile.Close()
	writer := csv.NewWriter(outFile)
	writer.Comma = ';'
	//idealement quantite en UF
	record := []string{"Id Unité", "Nom Unité", "Id Type Unité", "Ressource", "Quantité"}
	writer.Write(record)
	for id, unit := range data.Units {
		if isLogisticSubordinate(data, unit, root) {
			for _, stock := range unit.Stocks {
				amount := stock.Quantity
				resource := resources.GetResource(stock.Type)
				if resource == nil {
					log.Fatalf("Error getting resource: %d", stock.Type)
				}
				record = []string{UintToString(id), unit.Name, UintToString(unit.Type), resource.Name, IntToString(amount)}
				writer.Write(record)
			}
		}
	}
	writer.Flush()
}

func isLogisticSubordinate(data *swapi.ModelData, unit *swapi.Unit, root uint32) bool {
	if unit.AutomatId == root {
		return true
	}
	automat := data.Automats[unit.AutomatId]
	if automat == nil {
		log.Fatalf("Error No automat %d superior of unit %d", unit.AutomatId, unit.Id)
	}
	for _, superior := range automat.LogSuperiors {
		if superior == root {
			return true
		}
	}
	return isFormationLogisticSubordinate(data, automat.FormationId, root)
}

func isFormationLogisticSubordinate(data *swapi.ModelData, id uint32, root uint32) bool {
	if id == root {
		return true
	}
	formation := data.Formations[id]
	if formation == nil {
		return false // we reached the party (or the formation does not exist but this should not happen)
	}
	for _, superior := range formation.LogSuperiors {
		if superior == root {
			return true
		}
	}
	return isFormationLogisticSubordinate(data, formation.ParentId, root)
}
