package simtests

import (
	"swapi"
	"testing"
	"time"
)

func TestModelInitialization(t *testing.T) {
	sim := startSimOnExercise(t, "tests/crossroad-small-empty", 1000, false)
	defer sim.Kill()
	client := ConnectClient(t, sim)
	err := client.Login("admin", "")
	if err != nil {
		t.Fatalf("login failed: %v", err)
	}
	if !client.Model.WaitReady(10 * time.Second) {
		t.Fatalf("model initialization timed out")
	}
	model := client.Model

	expectedParties := map[uint32]*swapi.Party{
		1: swapi.NewParty(1, "party1"),
		2: swapi.NewParty(2, "party2"),
	}
	parties := model.GetParties()
	if len(parties) != len(expectedParties) {
		t.Fatalf("got %v parties, %v expected", len(parties), len(expectedParties))
	}
	for k, v := range parties {
		other, ok := expectedParties[k]
		if !ok {
			t.Fatalf("unexpected party %v:%v", v.GetId(), v.GetName())
		}
		if other.GetName() != v.GetName() {
			t.Fatalf("got a party named %v, %v expected", v.GetName(),
				other.GetName())
		}
	}
}
