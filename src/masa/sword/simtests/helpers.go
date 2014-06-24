// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package simtests

import (
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/swapi/phy"
	"strings"
)

func getSomeUnit(c *C, data *swapi.ModelData) *swapi.Unit {
	for _, a := range data.Units {
		return a
	}
	c.Fatal("missing units")
	return nil
}

func getSomeUnitByName(c *C, data *swapi.ModelData, substring string) *swapi.Unit {
	for _, a := range data.Units {
		if strings.Contains(a.Name, substring) {
			return a
		}
	}
	c.Fatal("no unit whose name contains '" + substring + "'")
	return nil
}

func getRandomUnit(c *C, client *swapi.Client) *swapi.Unit {
	data := client.Model.GetData()
	return getSomeUnit(c, data)
}

func getSomeAutomat(c *C, data *swapi.ModelData) *swapi.Automat {
	for _, a := range data.Automats {
		return a
	}
	c.Fatal("missing automats")
	return nil
}

func getSomeAutomatByName(c *C, data *swapi.ModelData, substring string) *swapi.Automat {
	for _, a := range data.Automats {
		if strings.Contains(a.Name, substring) {
			return a
		}
	}
	c.Fatal("no automat whose name contains '" + substring + "'")
	return nil
}

func getSomeFormation(c *C, data *swapi.ModelData) *swapi.Formation {
	for _, f := range data.Formations {
		return f
	}
	c.Fatal("missing formations")
	return nil
}

func getSomeFormationByName(c *C, data *swapi.ModelData, substring string) *swapi.Formation {
	for _, f := range data.Formations {
		if strings.Contains(f.Name, substring) {
			return f
		}
	}
	c.Fatal("no formation whose name contains '" + substring + "'")
	return nil
}

func getSomeCrowd(c *C, data *swapi.ModelData) *swapi.Crowd {
	for _, cr := range data.Crowds {
		return cr
	}
	c.Fatal("missing crowds")
	return nil
}

func getSomePopulation(c *C, data *swapi.ModelData) *swapi.Population {
	for _, p := range data.Populations {
		return p
	}
	c.Fatal("missing populations")
	return nil
}

func getSomeKnowledgeGroup(c *C, data *swapi.ModelData, partyId uint32) *swapi.KnowledgeGroup {
	for _, kg := range data.KnowledgeGroups {
		if kg.PartyId == partyId || partyId == 0 {
			return kg
		}
	}
	c.Fatal("missing knowledge groups")
	return nil
}

func getSomeEquipment(c *C, unit *swapi.Unit) (uint32, *swapi.Equipment) {
	for k, v := range unit.Equipments {
		return k, v
	}
	c.Fatal("unable to find any equipment")
	return 0, nil
}

func getSomeParty(c *C, model *swapi.ModelData) *swapi.Party {
	for _, party := range model.Parties {
		return party
	}
	c.Fatal("no party found")
	return nil
}

func getPartyByName(c *C, model *swapi.ModelData, name string) *swapi.Party {
	for _, party := range model.Parties {
		if party.Name == name {
			return party
		}
	}
	c.Fatal("no party found")
	return nil
}

func getAutomatTypeFromName(c *C, phydb *phy.PhysicalFile, typeName string) uint32 {
	automats, err := phy.ReadAutomats(*phydb)
	c.Assert(err, IsNil)
	automatType := uint32(0)
	for _, automat := range automats.Automats {
		if automat.Name == typeName {
			automatType = automat.Id
			break
		}
	}
	c.Assert(automatType, Greater, uint32(0))
	return automatType
}

func getUnitTypeFromName(c *C, phydb *phy.PhysicalFile, typeName string) uint32 {
	units, err := phy.ReadUnits(*phydb)
	c.Assert(err, IsNil)
	unitType := uint32(0)
	for _, unit := range units.Units {
		if unit.Name == typeName {
			unitType = unit.Id
			break
		}
	}
	c.Assert(unitType, Greater, uint32(0))
	return unitType
}

func getUnitsFromAutomat(automatId uint32, model *swapi.ModelData) []*swapi.Unit {
	units := []*swapi.Unit{}
	for _, unit := range model.Units {
		if unit.AutomatId == automatId {
			units = append(units, unit)
		}
	}
	return units
}

func getResourceTypeFromName(c *C, phydb *phy.PhysicalFile, typeName string) uint32 {
	resources, err := phy.ReadResources(*phydb)
	c.Assert(err, IsNil)
	id, err := resources.GetIdFromName(typeName)
	c.Assert(err, IsNil)
	return id
}
