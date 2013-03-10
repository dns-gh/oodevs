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
	printParty := func(p *prettyPrinter, party *swapi.Party) {
		p.P("Name: %s", party.GetName())
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
	sim := startSimOnExercise(c, "crossroad-small-empty", 1000, false)
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
  Name: party1
Party[2]
  Name: party2
`
	c.Assert(dump, Equals, expected)
}
