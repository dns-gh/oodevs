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
	shift string
	lines []string
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

func (p *prettyPrinter) GetOutput() string {
	return strings.Join(p.lines, "\n") + "\n"
}

func printParties(p *prettyPrinter, model *swapi.ModelData) *prettyPrinter {
	printPopulation := func(p *prettyPrinter, pop *swapi.Population) {
		p.P("Id: %d", pop.Id)
		p.P("PartyId: %d", pop.PartyId)
		p.P("Name: %s", pop.Name)
	}

	printCrowd := func(p *prettyPrinter, c *swapi.Crowd) {
		p.P("Id: %d", c.Id)
		p.P("PartyId: %d", c.PartyId)
		p.P("Name: %s", c.Name)
	}

	printUnit := func(p *prettyPrinter, u *swapi.Unit) {
		p.P("Id: %d", u.Id)
		p.P("AutomatId: %d", u.AutomatId)
		p.P("Name: %s", u.Name)
	}

	var printAutomat func(p *prettyPrinter, a *swapi.Automat)
	printAutomat = func(p *prettyPrinter, a *swapi.Automat) {
		p.P("Id: %d", a.Id)
		p.P("PartyId: %d", a.PartyId)
		p.P("Name: %s", a.Name)
		keys := []int{}
		for k := range a.Automats {
			keys = append(keys, int(k))
		}
		sort.Ints(keys)
		for _, k := range keys {
			child := a.Automats[uint32(k)]
			p.Shift()
			p.P("Automat[%d]", k)
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
			p.P("Unit[%d]", k)
			p.Shift()
			printUnit(p, child)
			p.Unshift()
			p.Unshift()
		}
	}

	var printFormation func(p *prettyPrinter, f *swapi.Formation)
	printFormation = func(p *prettyPrinter, f *swapi.Formation) {
		p.P("Id: %d", f.Id)
		p.P("Name: %s", f.Name)
		p.P("ParentId: %d", f.ParentId)
		p.P("PartyId: %d", f.PartyId)
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
			p.P("Formation[%d]", k)
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
			p.P("Automat[%d]", k)
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
			p.P("Formation[%d]", k)
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
			p.P("Crowds[%d]", k)
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
			p.P("Population[%d]", k)
			p.Shift()
			printPopulation(p, child)
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
		p.P("Party[%v]", k)
		p.Shift()
		printParty(p, party)
		p.Unshift()
	}
	return p
}

func connectAndWaitModel(c *C, user, password, exercise string) (
	*simu.SimProcess, *swapi.Client) {

	sim := startSimOnExercise(c, exercise, 1000, false)
	client := ConnectClient(c, sim)
	err := client.Login(user, password)
	c.Assert(err, IsNil) // login failed
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // model initialization timed out
	return sim, client
}

func connectAllUserAndWait(c *C, exercise string) (*simu.SimProcess, *swapi.Client) {
	return connectAndWaitModel(c, "alluser", "alluser", exercise)
}

func (s *TestSuite) TestModelInitialization(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()
	model := client.Model

	dump := printParties(&prettyPrinter{}, model.GetData()).GetOutput()
	expected := "" +
		`Party[1]
  Name: party
    Formation[5]
      Id: 5
      Name: formation-1
      ParentId: 0
      PartyId: 1
      Level: xxxxx
      LogLevel: none
        Formation[7]
          Id: 7
          Name: formation-1.1
          ParentId: 5
          PartyId: 1
          Level: xxxx
          LogLevel: none
        Formation[8]
          Id: 8
          Name: formation-1.2
          ParentId: 5
          PartyId: 1
          Level: xxxx
          LogLevel: none
            Automat[9]
              Id: 9
              PartyId: 1
              Name: ARMOR.MBT squadron
                Unit[10]
                  Id: 10
                  AutomatId: 9
                  Name: ARMOR.Armored squadron CP
                Unit[11]
                  Id: 11
                  AutomatId: 9
                  Name: ARMOR.MBT platoon
                Unit[12]
                  Id: 12
                  AutomatId: 9
                  Name: ARMOR.MBT platoon
                Unit[13]
                  Id: 13
                  AutomatId: 9
                  Name: ARMOR.MBT platoon
                Unit[14]
                  Id: 14
                  AutomatId: 9
                  Name: ARMOR.MBT platoon
    Formation[6]
      Id: 6
      Name: empty
      ParentId: 0
      PartyId: 1
      Level: xxxxx
      LogLevel: none
    Crowds[17]
      Id: 17
      PartyId: 1
      Name: Standard Crowd
    Population[22]
      Id: 22
      PartyId: 1
      Name: population
Party[2]
  Name: empty-party
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
