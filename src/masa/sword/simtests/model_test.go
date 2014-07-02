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
	"fmt"
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/swtest"
	"sort"
	"strings"
	"time"
)

type prettyPrinter struct {
	// Rewrite unstable properties so random variations do not impact the
	// output.
	HideUnstable bool
	shift        string
	lines        []string
}

func (p *prettyPrinter) Shift() {
	p.shift += "  "
}

func (p *prettyPrinter) Unshift() {
	p.shift = p.shift[:len(p.shift)-2]
}

func (p *prettyPrinter) P(format string, a ...interface{}) {
	s := fmt.Sprintf(format, a...)
	p.lines = append(p.lines, p.shift+s)
}

func (p *prettyPrinter) Unstable(value interface{}) string {
	if p.HideUnstable {
		return "-"
	}
	return fmt.Sprintf("%v", value)
}

func (p *prettyPrinter) GetOutput() string {
	return strings.Join(p.lines, "\n") + "\n"
}

func getAutomatUnits(data *swapi.ModelData, automatId uint32) []int {
	keys := []int{}
	for id, unit := range data.Units {
		if unit.AutomatId == automatId {
			keys = append(keys, int(id))
		}
	}
	sort.Ints(keys)
	return keys
}

func getChildFormations(data *swapi.ModelData, formationId uint32) []int {
	keys := []int{}
	for id, formation := range data.Formations {
		if formation.ParentId == formationId {
			keys = append(keys, int(id))
		}
	}
	sort.Ints(keys)
	return keys
}

func getFormationAutomats(data *swapi.ModelData, formationId uint32) []int {
	keys := []int{}
	for id, automat := range data.Automats {
		if automat.FormationId == formationId {
			keys = append(keys, int(id))
		}
	}
	sort.Ints(keys)
	return keys
}

func getPartyFormations(data *swapi.ModelData, partyId uint32) []int {
	keys := []int{}
	for id, formation := range data.Formations {
		if formation.PartyId == partyId && formation.ParentId == 0 {
			keys = append(keys, int(id))
		}
	}
	sort.Ints(keys)
	return keys
}

func getPartyCrowds(data *swapi.ModelData, partyId uint32) []int {
	keys := []int{}
	for id, crowd := range data.Crowds {
		if crowd.PartyId == partyId {
			keys = append(keys, int(id))
		}
	}
	sort.Ints(keys)
	return keys
}

func getPartyPopulations(data *swapi.ModelData, partyId uint32) []int {
	keys := []int{}
	for id, population := range data.Populations {
		if population.PartyId == partyId {
			keys = append(keys, int(id))
		}
	}
	sort.Ints(keys)
	return keys
}

func getPartyKnowledgeGroups(data *swapi.ModelData, partyId uint32) []int {
	keys := []int{}
	for id, group := range data.KnowledgeGroups {
		if group.PartyId == partyId {
			keys = append(keys, int(id))
		}
	}
	sort.Ints(keys)
	return keys
}

func printParties(p *prettyPrinter, model *swapi.ModelData) *prettyPrinter {
	printKnowledgeGroup := func(p *prettyPrinter, group *swapi.KnowledgeGroup) {
		p.P("Id: %s", p.Unstable(group.Id))
		p.P("PartyId: %s", p.Unstable(group.PartyId))
		p.P("Name: %s", group.Name)
		if group.IsCrowdDefaultGroup {
			p.P("IsCrowdDefaultGroup: true")
		}
	}

	printPopulation := func(p *prettyPrinter, pop *swapi.Population) {
		p.P("Id: %s", p.Unstable(pop.Id))
		p.P("PartyId: %s", p.Unstable(pop.PartyId))
		p.P("Name: %s", pop.Name)
	}

	printCrowd := func(p *prettyPrinter, c *swapi.Crowd) {
		p.P("Id: %s", p.Unstable(c.Id))
		p.P("PartyId: %s", p.Unstable(c.PartyId))
		p.P("Name: %s", c.Name)
	}

	printUnit := func(p *prettyPrinter, u *swapi.Unit) {
		p.P("Id: %s", p.Unstable(u.Id))
		p.P("AutomatId: %s", p.Unstable(u.AutomatId))
		p.P("Name: %s", u.Name)
	}

	var printAutomat func(p *prettyPrinter, a *swapi.Automat)
	printAutomat = func(p *prettyPrinter, a *swapi.Automat) {
		p.P("Id: %s", p.Unstable(a.Id))
		p.P("PartyId: %s", p.Unstable(a.PartyId))
		p.P("Name: %s", a.Name)

		for _, k := range getAutomatUnits(model, a.Id) {
			child := model.Units[uint32(k)]
			p.Shift()
			p.P("Unit[%s]", p.Unstable(k))
			p.Shift()
			printUnit(p, child)
			p.Unshift()
			p.Unshift()
		}
	}

	var printFormation func(p *prettyPrinter, f *swapi.Formation)
	printFormation = func(p *prettyPrinter, f *swapi.Formation) {
		p.P("Id: %s", p.Unstable(f.Id))
		p.P("Name: %s", f.Name)
		p.P("ParentId: %s", p.Unstable(f.ParentId))
		p.P("PartyId: %s", p.Unstable(f.PartyId))
		p.P("Level: %s", f.Level)
		p.P("LogLevel: %s", f.LogLevel)

		for _, k := range getChildFormations(model, f.Id) {
			child := model.Formations[uint32(k)]
			p.Shift()
			p.P("Formation[%s]", p.Unstable(k))
			p.Shift()
			printFormation(p, child)
			p.Unshift()
			p.Unshift()
		}

		for _, k := range getFormationAutomats(model, f.Id) {
			child := model.Automats[uint32(k)]
			p.Shift()
			p.P("Automat[%s]", p.Unstable(k))
			p.Shift()
			printAutomat(p, child)
			p.Unshift()
			p.Unshift()
		}
	}

	printParty := func(p *prettyPrinter, party *swapi.Party) {
		p.P("Name: %s", party.Name)

		for _, k := range getPartyFormations(model, party.Id) {
			child := model.Formations[uint32(k)]
			p.Shift()
			p.P("Formation[%s]", p.Unstable(k))
			p.Shift()
			printFormation(p, child)
			p.Unshift()
			p.Unshift()
		}

		for _, k := range getPartyCrowds(model, party.Id) {
			child := model.Crowds[uint32(k)]
			p.Shift()
			p.P("Crowds[%s]", p.Unstable(k))
			p.Shift()
			printCrowd(p, child)
			p.Unshift()
			p.Unshift()
		}

		for _, k := range getPartyPopulations(model, party.Id) {
			child := model.Populations[uint32(k)]
			p.Shift()
			p.P("Population[%s]", p.Unstable(k))
			p.Shift()
			printPopulation(p, child)
			p.Unshift()
			p.Unshift()
		}

		for _, k := range getPartyKnowledgeGroups(model, party.Id) {
			child := model.KnowledgeGroups[uint32(k)]
			p.Shift()
			p.P("KnowledgeGroup[%s]", p.Unstable(k))
			p.Shift()
			printKnowledgeGroup(p, child)
			p.Unshift()
			p.Unshift()
		}
	}

	keys := []int{}
	for k := range model.Parties {
		keys = append(keys, int(k))
	}
	sort.Ints(keys)

	for _, k := range keys {
		party := model.Parties[uint32(k)]
		p.P("Party[%v]", p.Unstable(k))
		p.Shift()
		printParty(p, party)
		p.Unshift()
	}
	return p
}

func (s *TestSuite) TestModelInitialization(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	model := client.Model

	dump := printParties(&prettyPrinter{HideUnstable: true},
		model.GetData()).GetOutput()
	expected := readFileAsString(c, "testdata/model.txt")
	swtest.DeepEquals(c, dump, expected)
	client.Close()

	// Do not enable the model, it is there but get no updates
	client, err := swapi.NewClient(sim.GetClientAddr())
	c.Assert(err, IsNil)
	client.EnableModel = false
	go client.Run()
	err = client.Login("alluser", "alluser")
	c.Assert(err, IsNil)
	c.Assert(client.Model, NotNil)
	ok := client.Model.WaitReady(2 * time.Second)
	c.Assert(ok, Equals, false)
	client.Close()

	// Terminate the simulation before the client, the model should continue
	// to handle queries, not panic.
	client = connectClient(c, sim, NewAllUserOpts(ExCrossroadSmallOrbat))
	err = sim.Stop()
	c.Assert(err, IsNil)
	data := client.Model.GetData()
	c.Assert(data, NotNil)
}

func (s *TestSuite) TestModelIsolation(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	model := client.Model
	data := model.GetData()
	expected := printParties(&prettyPrinter{}, data).GetOutput()

	// Modify some properties, query the model again and check it did
	// not change.
	delete(data.Parties, 2)
	data.Parties[1].Name = "foobar"
	data.Formations[5].Name = "blaz"
	modified := printParties(&prettyPrinter{}, data).GetOutput()
	c.Assert(modified, Not(Equals), expected)

	updated := printParties(&prettyPrinter{}, model.GetData()).GetOutput()
	c.Assert(updated, Equals, expected)
}

func (s *TestSuite) TestModelHandlers(c *C) {
	model := swapi.NewModel()
	defer model.Close()

	// Test RegisterHandler timeout
	done1 := make(chan error, 1)
	model.RegisterHandlerTimeout(100*time.Millisecond,
		func(model *swapi.ModelData, msg *swapi.SwordMessage, err error) bool {
			if err != nil {
				done1 <- err
				close(done1)
			}
			return err != nil
		})
	err := <-done1
	c.Assert(err, Equals, swapi.ErrTimeout)

	// Test unregistering a model handler
	done2 := make(chan error, 1)
	id := model.RegisterHandlerTimeout(0,
		func(model *swapi.ModelData, msg *swapi.SwordMessage, err error) bool {
			if err != nil {
				done2 <- err
				close(done2)
			}
			return err != nil
		})
	model.UnregisterHandler(id)
	err = <-done2
	c.Assert(err, Equals, swapi.ErrConnectionClosed)
}
