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

func connectAndWaitModel(c *C, exercise string) (*simu.SimProcess, *swapi.Client) {
	sim := startSimOnExercise(c, exercise, 1000, false)
	client := ConnectClient(c, sim)
	err := client.Login("alluser", "alluser")
	c.Assert(err, IsNil) // login failed
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // model initialization timed out
	return sim, client
}

func (s *TestSuite) TestModelInitialization(c *C) {
	sim, client := connectAndWaitModel(c, ExCrossroadSmallOrbat)
	defer sim.Kill()
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
Party[2]
  Name: empty-party
`
	c.Assert(dump, Equals, expected)
}

func (s *TestSuite) TestModelIsolation(c *C) {
	sim, client := connectAndWaitModel(c, ExCrossroadSmallOrbat)
	defer sim.Kill()
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

func (s *TestSuite) TestCreateFormation(c *C) {
	sim, client := connectAndWaitModel(c, ExCrossroadSmallEmpty)
	defer sim.Kill()
	model := client.Model

	// Test with invalid tasker
	_, err := client.CreateFormation(0, 0, "invalid-tasker", 1, "")
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Test invalid level
	_, err = client.CreateFormation(1, 0, "invalid-tasker", 42, "")
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Test invalid log level
	_, err = client.CreateFormation(1, 0, "invalid-tasker", 1, "invalid")
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// Add formation to party
	id1, err := client.CreateFormation(1, 0, "newformation", 1, "")

	// Add formation to formation
	_, err = client.CreateFormation(0, id1, "newformation2", 2, "aucun")
	c.Assert(err, IsNil) // failed to create formation
	dump := printParties(&prettyPrinter{}, model.GetData()).GetOutput()
	expected := "" +
		`Party[1]
  Name: party1
    Formation[526]
      Id: 526
      Name: newformation
      ParentId: 0
      PartyId: 1
      Level: b
      LogLevel: none
        Formation[527]
          Id: 527
          Name: newformation2
          ParentId: 526
          PartyId: 1
          Level: o
          LogLevel: none
Party[2]
  Name: party2
`
	c.Assert(dump, Equals, expected)
}

func (s *TestSuite) TestDeleteUnit(c *C) {
	sim, client := connectAndWaitModel(c, ExCrossroadSmallOrbat)
	defer sim.Kill()
	model := client.Model

	// Destroy invalid unit
	err := client.DeleteUnit(1234)
	c.Assert(err, ErrorMatches, "error_invalid_unit")

	// Find some unit
	units := model.GetData().ListUnits()
	c.Assert(len(units) > 0, Equals, true)
	unit := units[0]

	// Blast it
	err = client.DeleteUnit(unit.Id)
	c.Assert(err, IsNil)
	c.Assert(model.GetData().FindUnit(unit.Id), IsNil)
}
