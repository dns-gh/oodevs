package simtests

import (
	"code.google.com/p/goprotobuf/proto"
	"errors"
	"fmt"
	. "launchpad.net/gocheck"
	"swapi"
	"sword"
)

// For a given tasker type, send a unit magic action which is not
// implemented by target type. It used to return nothing, not even an
// error. This test should be adjusted if actions support more types.
func postInvalidUnitMagicAction(client *swapi.Client, tasker *sword.Tasker) error {
	params := swapi.NewMissionParams()
	actionType := sword.UnitMagicAction_crowd_total_destruction
	if tasker.GetCrowd() != nil {
		actionType = sword.UnitMagicAction_delete_unit
	}

	msg := swapi.SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     tasker,
					Type:       &actionType,
					Parameters: params.Params,
				},
			},
		},
	}
	quit := make(chan error)
	handler := func(msg *swapi.SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.SimulationToClient == nil || msg.Context != context {
			return false
		}
		m := msg.SimulationToClient.GetMessage()
		if reply := m.GetUnitMagicActionAck(); reply != nil {
			_, err = swapi.GetUnitMagicActionAck(reply)
			if err == nil {
				err = errors.New(fmt.Sprintf("Got unexpected success: %v", m))
			}
		} else {
			err = errors.New(fmt.Sprintf("Got unexpected: %v", m))
		}
		quit <- err
		return true

	}
	client.Post(msg, handler)
	err := <-quit
	return err
}

func (s *TestSuite) TestNotImplementedUnitMagicAction(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Kill()
	model := client.Model
	data := model.GetData()

	// Get a formation identifier
	formations := data.ListFormations()
	c.Assert(len(formations) > 0, Equals, true)
	formationId := formations[0].Id

	// Get an automat identifier
	automats := data.ListAutomats()
	c.Assert(len(automats) > 0, Equals, true)
	automatId := automats[0].Id

	// Get a unit identifier
	units := data.ListUnits()
	c.Assert(len(units) > 0, Equals, true)
	unitId := units[0].Id

	// Get a crowd identifier
	crowds := data.ListCrowds()
	c.Assert(len(crowds) > 0, Equals, true)
	crowdId := crowds[0].Id

	// Get a population identifier
	populations := data.ListPopulations()
	c.Assert(len(populations) > 0, Equals, true)
	popId := populations[0].Id

	taskers := []*sword.Tasker{
		&sword.Tasker{
			Formation: &sword.FormationId{
				Id: proto.Uint32(formationId),
			},
		},
		&sword.Tasker{
			Automat: &sword.AutomatId{
				Id: proto.Uint32(automatId),
			},
		},
		&sword.Tasker{
			Unit: &sword.UnitId{
				Id: proto.Uint32(unitId),
			},
		},
		&sword.Tasker{
			Crowd: &sword.CrowdId{
				Id: proto.Uint32(crowdId),
			},
		},
		&sword.Tasker{
			Population: &sword.PopulationId{
				Id: proto.Uint32(popId),
			},
		},
	}
	for _, tasker := range taskers {
		err := postInvalidUnitMagicAction(client, tasker)
		c.Assert(err, ErrorMatches, "error_invalid_unit",
			Commentf("for tasker %v", tasker))
	}
}
