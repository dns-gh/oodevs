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
	"strings"
	"swapi"
	"swapi/phy"
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

func getSomeKnowledgeGroup(c *C, data *swapi.ModelData) *swapi.KnowledgeGroup {
	for _, k := range data.KnowledgeGroups {
		return k
	}
	c.Fatal("missing knowledge groups")
	return nil
}

func getAnyKnowledgeGroupIdWithPartyIndex(c *C, data *swapi.ModelData, index int) uint32 {
	parties := map[uint32]struct{}{}
	for id, group := range data.KnowledgeGroups {
		_, found := parties[group.PartyId]
		if found {
			continue
		}
		if len(parties) == index {
			return id
		}
		parties[group.PartyId] = struct{}{}
	}
	c.Fatal("unable to find a knowledge group with party index", index)
	return 0
}

func getSomeEquipment(c *C, unit *swapi.Unit) (uint32, *swapi.EquipmentDotation) {
	for k, v := range unit.EquipmentDotations {
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
