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
	"masa/sword/swapi"
	"masa/sword/swapi/phy"
	"os"
	"strconv"
	"strings"
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

func atoi(s string) uint32 {
	result, err := strconv.Atoi(s)
	if err != nil {
		log.Fatalf("Error %s is not a number %s", s, err)
	}
	return uint32(result)
}

func OutputResources(resources phy.Resources, out string) {
	outResourcesFile := out + "-resources.csv"
	outResources, err := os.Create(outResourcesFile)
	if err != nil {
		log.Fatalf("Error creating %s %s", outResourcesFile, err)
	}
	defer outResources.Close()
	writer := csv.NewWriter(outResources)
	writer.Comma = ';'
	record := []string{"Ressource", "Masse", "Volume", "Categorie"} //, "Quantité dans une UF"
	writer.Write(record)
	for _, resource := range resources.Content {
		packageSize := float64(resource.PackageSize)
		mass := resource.Mass / packageSize
		volume := resource.Volume / packageSize
		record = []string{resource.Name, FloatToString(mass), FloatToString(volume), resource.Category}
		writer.Write(record)
	}
	writer.Flush()
}

func OutputConsumptions(resources phy.Resources, physical phy.PhysicalFile, out string) *phy.Units {
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
	record := []string{"Type Unité", "Id", "Ressource", "Capacité", "Consommation normalisée"}
	writer.Write(record)
	for unit, consumption := range consumptions.Consumptions {
		name, err := units.GetName(unit)
		if err != nil {
			log.Fatalf("Error getting unit %d", unit)
		}
		for resourceId, amount := range consumption.Consumption {
			resource := resources.GetResource(resourceId)
			if resource == nil {
				log.Fatalf("Error getting resource %d", resourceId)
			}
			if consumptions.Capacities[unit] == nil {
				log.Fatalf("No capacity for unit %d", unit)
			}
			capacity := consumptions.Capacities[unit].Consumption[resourceId]
			record = []string{name, UintToString(unit), resource.Name, UintToString(capacity), UintToString(amount)}
			writer.Write(record)
		}
	}
	writer.Flush()
	return units
}

func ComputePhysicalData(xmlDir string, out string) (*phy.Resources, *phy.Units, error) {
	log.Printf("--Data read")
	physicalFile, err := phy.ReadPhysical(xmlDir)
	if err != nil {
		log.Fatalf("Error reading physical file: %s", err)
	}
	resources, err := phy.ReadResources(*physicalFile)
	if err != nil {
		log.Fatalf("Error reading resources: %s", err)
	}
	OutputResources(*resources, out)
	units := OutputConsumptions(*resources, *physicalFile, out)
	return resources, units, nil
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
	rootList := flag.String("root", "0", "root logistic units id, separated by commas - if '0' or unset, all units are dumped")
	flag.Parse()

	resources, units, err := ComputePhysicalData(*physical, *out)
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

	root := ParseRoot(*rootList)
	WriteOrbat(*out, data, *resources, root)
	WriteNumberOfUnits(*out, data, units, root)
}

func ParseRoot(rootList string) []uint32 {
	var result []uint32
	for _, value := range strings.Split(rootList, ",") {
		result = append(result, atoi(value))
	}
	return result
}

type Pair struct {
	Stock    int32
	Dotation int32
}

func WriteOrbat(out string, data *swapi.ModelData, resources phy.Resources, root []uint32) {
	outName := out + "-orbat.csv"
	outFile, err := os.Create(outName)
	if err != nil {
		log.Fatalf("Error creating %s %s", outName, err)
	}
	defer outFile.Close()
	writer := csv.NewWriter(outFile)
	writer.Comma = ';'
	//idealement quantite en UF
	record := []string{"Id Unité", "Nom Unité", "Id Type Unité", "Ressource", "Stock", "Dotation"}
	writer.Write(record)
	for id, unit := range data.Units {
		if isLogisticSubordinateList(data, unit, root) {
			stocks := map[uint32]*Pair{}
			for _, stock := range unit.Stocks {
				amount := stock.Quantity
				if stocks[stock.Type] == nil {
					stocks[stock.Type] = &Pair{}
				}
				stocks[stock.Type].Stock = amount
			}
			for idResource, dotation := range unit.Resources {
				if stocks[idResource] == nil {
					stocks[idResource] = &Pair{}
				}
				stocks[idResource].Dotation = dotation.Quantity
			}
			for idResource, pair := range stocks {
				resource := resources.GetResource(idResource)
				if resource == nil {
					log.Fatalf("Error getting resource: %d", idResource)
				}
				record = []string{UintToString(id), unit.Name, UintToString(unit.Type), resource.Name, IntToString(pair.Stock), IntToString(pair.Dotation)}
				writer.Write(record)
			}
		}
	}
	writer.Flush()
}

func isLogisticSubordinateList(data *swapi.ModelData, unit *swapi.Unit, root []uint32) bool {
	for _, value := range root {
		if isLogisticSubordinate(data, unit, value) {
			return true
		}
	}
	return false
}

func isLogisticSubordinate(data *swapi.ModelData, unit *swapi.Unit, root uint32) bool {
	if root == 0 {
		return true
	}
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
		if isAutomatLogisticSubordinate(data, superior, root) || isFormationLogisticSubordinate(data, superior, root) {
			return true
		}
	}
	return isFormationLogisticSubordinate(data, automat.FormationId, root)
}

func isAutomatLogisticSubordinate(data *swapi.ModelData, id uint32, root uint32) bool {
	if root == 0 {
		return true
	}
	automat := data.Automats[id]
	if automat == nil {
		return false
	}
	if automat.Id == root {
		return true
	}
	for _, superior := range automat.LogSuperiors {
		if superior == root {
			return true
		}
		if isAutomatLogisticSubordinate(data, superior, root) || isFormationLogisticSubordinate(data, superior, root) {
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

func WriteNumberOfUnits(out string, data *swapi.ModelData, units *phy.Units, root []uint32) {
	amounts := map[uint32]uint32{}
	for _, unit := range data.Units {
		if isLogisticSubordinateList(data, unit, root) {
			amounts[unit.Type] = amounts[unit.Type] + 1
		}
	}
	outName := out + "-numbers.csv"
	outFile, err := os.Create(outName)
	if err != nil {
		log.Fatalf("Error creating %s %s", outName, err)
	}
	defer outFile.Close()
	writer := csv.NewWriter(outFile)
	writer.Comma = ';'
	record := []string{"Id Type Unité", "Nombre", "Type"}
	writer.Write(record)
	for id, amount := range amounts {
		name, err := units.GetName(id)
		if err != nil {
			log.Fatalf("Error getting name of unit %d %s", id, err)
		}
		record = []string{UintToString(id), UintToString(amount), name}
		writer.Write(record)
	}
	writer.Flush()
}
