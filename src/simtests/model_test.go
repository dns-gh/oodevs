package simtests

import (
	. "launchpad.net/gocheck"
	"swapi"
	"time"
)

func (s *TestSuite) TestModelInitialization(c *C) {
	sim := startSimOnExercise(c, "tests/crossroad-small-empty", 1000, false)
	defer sim.Kill()
	client := ConnectClient(c, sim)
	err := client.Login("admin", "")
	c.Assert(err, IsNil) // login failed
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // model initialization timed out
	model := client.Model

	expectedParties := map[uint32]*swapi.Party{
		1: swapi.NewParty(1, "party1"),
		2: swapi.NewParty(2, "party2"),
	}
	parties := model.GetParties()
	c.Assert(len(parties), Equals, len(expectedParties)) // parties count mismatch
	for k, v := range parties {
		other, ok := expectedParties[k]
		c.Assert(ok, Equals, true, Commentf("unexpected party %v:%v",
			v.GetId(), v.GetName()))
		c.Assert(v.GetName(), Equals, other.GetName()) // party names mismatch
	}
}
