// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package swtest

import (
	"fmt"
	"masa/sword/swapi"
	"masa/sword/swapi/phy"
)

type Unit struct {
	Name    string
	Type    string
	Pos     swapi.Point
	Entity  *swapi.Unit
	Created bool
}

type Automat struct {
	Name    string
	Type    string
	Units   []*Unit
	Entity  *swapi.Automat
	Created bool
}

type Formation struct {
	Name       string
	Formations []*Formation
	Automats   []*Automat
	Entity     *swapi.Formation
	Created    bool
}

type Party struct {
	Name       string
	Formations []*Formation
	Entity     *swapi.Party
}

func findOrCreateUnit(client *swapi.Client, phydb *phy.PhysicalData,
	model *swapi.ModelData, automat *swapi.Automat, unit *Unit) error {

	var found *swapi.Unit
	for _, u := range model.Units {
		if u.Name == unit.Name && u.AutomatId == automat.Id {
			found = u
			break
		}
	}
	unit.Created = found == nil
	if found == nil {
		unitType, err := phydb.Units.GetType(unit.Type)
		if err != nil {
			return err
		}

		u, err := client.CreateUnitWithName(automat.Id, unitType, unit.Pos, unit.Name, false)
		if err != nil {
			return err
		}
		found = u
	}
	unit.Entity = found
	return nil
}

func findOrCreateAutomat(client *swapi.Client, phydb *phy.PhysicalData,
	model *swapi.ModelData, party *swapi.Party, formation *swapi.Formation,
	automat *Automat) error {

	var found *swapi.Automat
	for _, a := range model.Automats {
		if a.Name == automat.Name && a.FormationId == formation.Id {
			found = a
			break
		}
	}

	automat.Created = found == nil
	if found == nil {
		automatType, err := phydb.Automats.GetType(automat.Type)
		if err != nil {
			return err
		}

		// Get the party knowledge group by default
		var group *swapi.KnowledgeGroup
		for _, kg := range model.KnowledgeGroups {
			if kg.ParentId == 0 && kg.PartyId == party.Id {
				group = kg
				break
			}
		}
		if group == nil {
			return fmt.Errorf("cannot find knowledge group for party %d", party.Id)
		}
		a, err := client.CreateAutomat(formation.Id, automatType, group.Id)
		if err != nil {
			return err
		}
		// Created automat has its default name, change it to specified one.
		err = client.Rename(a.Id, automat.Name)
		if err != nil {
			return err
		}
		// This is a bit of a lie but simplifies code by not having to wait
		// for the model to be synchronized.
		a.Name = automat.Name
		found = a
	}
	for _, u := range automat.Units {
		err := findOrCreateUnit(client, phydb, model, found, u)
		if err != nil {
			return err
		}
	}
	automat.Entity = found
	return nil
}

func findOrCreateFormation(client *swapi.Client, phydb *phy.PhysicalData,
	model *swapi.ModelData, party *swapi.Party, parent *swapi.Formation,
	formation *Formation) error {

	var found *swapi.Formation
	for _, f := range model.Formations {
		// A formation can belong to a party or another formation
		if f.PartyId == party.Id && f.Name == formation.Name &&
			(parent == nil || parent.Id == f.ParentId) {
			found = f
			break
		}
	}
	formation.Created = found == nil
	if found == nil {
		partyId := party.Id
		parentId := uint32(0)
		if parent != nil {
			parentId = parent.Id
			partyId = 0
		}
		f, err := client.CreateFormation(partyId, parentId, formation.Name, 1, "")
		if err != nil {
			return err
		}
		found = f
	}
	for _, child := range formation.Formations {
		err := findOrCreateFormation(client, phydb, model, party, found, child)
		if err != nil {
			return err
		}
	}
	for _, automat := range formation.Automats {
		err := findOrCreateAutomat(client, phydb, model, party, found, automat)
		if err != nil {
			return err
		}
	}
	formation.Entity = found
	return nil
}

func findParty(client *swapi.Client, phydb *phy.PhysicalData, model *swapi.ModelData,
	party *Party) error {

	for _, p := range model.Parties {
		if p.Name == party.Name {
			for _, formation := range party.Formations {
				err := findOrCreateFormation(client, phydb, model, p, nil, formation)
				if err != nil {
					return err
				}
			}
			party.Entity = p
			return nil
		}
	}
	// Parties cannot be created yet in gaming.
	return fmt.Errorf("could not find party %s", party.Name)
}

// Walks each Party entity tree and looks for defined entities by name. If the
// entity is found, the caller structure "Entity" field points to a copy of
// the entity. Otherwise an entity matching supplied properties is created then
// referenced by "Entities". The "Created" field is set depending on the entity
// being found or created.
func FindOrCreateEntities(client *swapi.Client, phydb *phy.PhysicalData,
	parties ...*Party) error {

	model := client.Model.GetData()
	for _, party := range parties {
		err := findParty(client, phydb, model, party)
		if err != nil {
			return err
		}
	}
	return nil
}
