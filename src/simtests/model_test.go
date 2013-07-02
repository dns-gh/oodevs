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
	"sort"
	"strings"
	"swapi"
	"swapi/simu"
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

func printParties(p *prettyPrinter, model *swapi.ModelData) *prettyPrinter {
	printKnowledgeGroup := func(p *prettyPrinter, group *swapi.KnowledgeGroup) {
		p.P("Id: %s", p.Unstable(group.Id))
		p.P("PartyId: %s", p.Unstable(group.PartyId))
		p.P("Name: %s", group.Name)
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
		keys := []int{}
		for k := range a.Automats {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := a.Automats[uint32(k)]
			p.Shift()
			p.P("Automat[%s]", p.Unstable(k))
			p.Shift()
			printAutomat(p, child)
			p.Unshift()
			p.Unshift()
		}

		for k := range a.Units {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := a.Units[uint32(k)]
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
		keys := []int{}
		for k := range f.Formations {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := f.Formations[uint32(k)]
			p.Shift()
			p.P("Formation[%s]", p.Unstable(k))
			p.Shift()
			printFormation(p, child)
			p.Unshift()
			p.Unshift()
		}

		keys = []int{}
		for k := range f.Automats {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := f.Automats[uint32(k)]
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
		keys := []int{}
		for k := range party.Formations {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := party.Formations[uint32(k)]
			p.Shift()
			p.P("Formation[%s]", p.Unstable(k))
			p.Shift()
			printFormation(p, child)
			p.Unshift()
			p.Unshift()
		}

		keys = []int{}
		for k := range party.Crowds {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := party.Crowds[uint32(k)]
			p.Shift()
			p.P("Crowds[%s]", p.Unstable(k))
			p.Shift()
			printCrowd(p, child)
			p.Unshift()
			p.Unshift()
		}

		keys = []int{}
		for k := range party.Populations {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := party.Populations[uint32(k)]
			p.Shift()
			p.P("Population[%s]", p.Unstable(k))
			p.Shift()
			printPopulation(p, child)
			p.Unshift()
			p.Unshift()
		}

		keys = []int{}
		for k := range party.KnowledgeGroups {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := party.KnowledgeGroups[uint32(k)]
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

func loginAndWaitModel(c *C, sim *simu.SimProcess, user, password,
	exercise string) *swapi.Client {

	client := ConnectClient(c, sim)
	err := client.Login(user, password)
	c.Assert(err, IsNil) // login failed
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // model initialization timed out
	return client
}

func connectAndWaitModel(c *C, user, password, exercise string) (
	*simu.SimProcess, *swapi.Client) {

	sim := startSimOnExercise(c, exercise, 1000, false)
	client := loginAndWaitModel(c, sim, user, password, exercise)
	return sim, client
}

func connectAllUserAndWait(c *C, exercise string) (*simu.SimProcess, *swapi.Client) {
	return connectAndWaitModel(c, "alluser", "alluser", exercise)
}

func (s *TestSuite) TestModelInitialization(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model

	dump := printParties(&prettyPrinter{HideUnstable: true},
		model.GetData()).GetOutput()
	expected := "" +
		`Party[-]
  Name: party
    Formation[-]
      Id: -
      Name: formation-1
      ParentId: -
      PartyId: -
      Level: xxxxx
      LogLevel: none
        Formation[-]
          Id: -
          Name: formation-1.1
          ParentId: -
          PartyId: -
          Level: xxxx
          LogLevel: none
        Formation[-]
          Id: -
          Name: formation-1.2
          ParentId: -
          PartyId: -
          Level: xxxx
          LogLevel: none
            Automat[-]
              Id: -
              PartyId: -
              Name: ARMOR.MBT squadron
                Unit[-]
                  Id: -
                  AutomatId: -
                  Name: ARMOR.Armored squadron CP
                Unit[-]
                  Id: -
                  AutomatId: -
                  Name: ARMOR.MBT platoon
                Unit[-]
                  Id: -
                  AutomatId: -
                  Name: ARMOR.MBT platoon
                Unit[-]
                  Id: -
                  AutomatId: -
                  Name: ARMOR.MBT platoon
                Unit[-]
                  Id: -
                  AutomatId: -
                  Name: ARMOR.MBT platoon
            Automat[-]
              Id: -
              PartyId: -
              Name: Logistic combat train [23]
                Unit[-]
                  Id: -
                  AutomatId: -
                  Name: Logistic CT
            Automat[-]
              Id: -
              PartyId: -
              Name: Logistic combat train [31]
                Unit[-]
                  Id: -
                  AutomatId: -
                  Name: Logistic CT
    Formation[-]
      Id: -
      Name: empty
      ParentId: -
      PartyId: -
      Level: xxxxx
      LogLevel: none
    Formation[-]
      Id: -
      Name: BLD
      ParentId: -
      PartyId: -
      Level: x
      LogLevel: logistic_base
        Automat[-]
          Id: -
          PartyId: -
          Name: Supply logistic area  (corps) [26]
            Unit[-]
              Id: -
              AutomatId: -
              Name: Supply CP (division)
    Crowds[-]
      Id: -
      PartyId: -
      Name: Standard Crowd
    Population[-]
      Id: -
      PartyId: -
      Name: population
    KnowledgeGroup[-]
      Id: -
      PartyId: -
      Name: knowledge group [3]
Party[-]
  Name: empty-party
    KnowledgeGroup[-]
      Id: -
      PartyId: -
      Name: knowledge group[4]
`
	c.Assert(dump, Equals, expected)
	client.Close()

	client, err := swapi.NewClient(sim.DispatcherAddr)
	c.Assert(err, IsNil)
	client.EnableModel = false
	go client.Run()
	err = client.Login("alluser", "alluser")
	c.Assert(err, IsNil)
	c.Assert(client.Model, NotNil)
	ok := client.Model.WaitReady(2 * time.Second)
	c.Assert(ok, Equals, false)
}

func (s *TestSuite) TestModelIsolation(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model
	data := model.GetData()
	expected := printParties(&prettyPrinter{}, data).GetOutput()

	// Modify some properties, query the model again and check it did
	// not change.
	delete(data.Parties, 2)
	data.Parties[1].Name = "foobar"
	data.Parties[1].Formations[5].Name = "blaz"
	delete(data.Parties[1].Formations, 6)
	modified := printParties(&prettyPrinter{}, data).GetOutput()
	c.Assert(modified, Not(Equals), expected)

	updated := printParties(&prettyPrinter{}, model.GetData()).GetOutput()
	c.Assert(updated, Equals, expected)
}
