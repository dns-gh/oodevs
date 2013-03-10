package simtests

import (
	"fmt"
	. "launchpad.net/gocheck"
	"sort"
	"strings"
	"swapi"
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

func printParties(p *prettyPrinter, parties map[uint32]*swapi.Party) *prettyPrinter {
	var printFormation func(p *prettyPrinter, f *swapi.Formation)
	printFormation = func(p *prettyPrinter, f *swapi.Formation) {
		p.P("Id: %d", f.Id)
		p.P("Name: %s", f.Name)
		p.P("ParentId: %d", f.ParentId)
		p.P("PartyId: %d", f.PartyId)
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
	for k := range parties {
		keys = append(keys, int(k))
	}
	sort.Ints(keys)

	for _, k := range keys {
		party := parties[uint32(k)]
		p.P("Party[%v]", k)
		p.Shift()
		printParty(p, party)
		p.Unshift()
	}
	return p
}

func (s *TestSuite) TestModelInitialization(c *C) {
	sim := startSimOnExercise(c, "crossroad-small-orbat", 1000, false)
	defer sim.Kill()
	client := ConnectClient(c, sim)
	err := client.Login("admin", "")
	c.Assert(err, IsNil) // login failed
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // model initialization timed out
	model := client.Model

	dump := printParties(&prettyPrinter{}, model.GetParties()).GetOutput()
	expected := "" +
`Party[1]
  Name: party
    Formation[5]
      Id: 5
      Name: formation-1
      ParentId: 0
      PartyId: 1
        Formation[7]
          Id: 7
          Name: formation-1.1
          ParentId: 5
          PartyId: 1
        Formation[8]
          Id: 8
          Name: formation-1.2
          ParentId: 5
          PartyId: 1
    Formation[6]
      Id: 6
      Name: empty
      ParentId: 0
      PartyId: 1
Party[2]
  Name: empty-party
`

	c.Assert(dump, Equals, expected)
}
