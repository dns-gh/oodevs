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
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
)

const (
	// This was "COMMON - Move" when the test was written
	MissionMoveId          = uint32(44582)
	MissionAutomatAttackId = uint32(44523)
	MissionMoveCrowdId     = uint32(4490)
	InvalidIdentifier      = uint32(129500)
)

// Test we can send a unit mission and get a successful acknowledgement. The
// mission is not really expected to work successfully at this point, unit
// may not move because of the terrain type and so forth.
func (s *TestSuite) TestGenericMission(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}
	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	heading := swapi.MakeHeading(0)
	dest := swapi.MakePointParam(to)
	params := swapi.MakeParameters(heading, nil, nil, nil, dest)

	// Cannot send order with an invalid unit identifier
	_, err = client.SendUnitOrder(InvalidIdentifier, MissionMoveId, params)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot send order to an engaged unit
	_, err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsSwordError, "error_unit_cannot_receive_order")

	// Should work with disengaged unit
	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)
	_, err = client.SendUnitOrder(unit.Id, MissionMoveId, params)
	c.Assert(err, IsNil)

	// Cannot order with an invalid mission identifier
	_, err = client.SendUnitOrder(unit.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_mission")

	checkParams := func(expected string, params ...interface{}) {
		missionParams := swapi.MakeParameters(params...)
		order, err := client.SendUnitOrder(unit.Id, MissionMoveId, missionParams)
		if len(expected) > 0 {
			c.Assert(err, ErrorMatches, expected)
		} else {
			c.Assert(err, IsNil)
			c.Assert(order.Kind, Equals, swapi.UnitOrder)
		}
	}

	// Missing heading
	checkParams(".*missing.*heading expected.*")

	// Invalid heading
	checkParams(".*invalid.*heading expected.*", dest)

	// Missing limit 1
	checkParams(".*missing.*limit expected.*", heading, nil)

	// Invalid limit 1
	checkParams(".*must be a limit.*", heading, nil, dest)

	// Limit1 is empty
	checkParams(".*limit value is invalid.*", heading, nil, swapi.MakeLimit())

	// Missing limit 2
	limit1 := swapi.MakeLimit(
		swapi.Point{X: from.X + 0.001, Y: from.Y},
		swapi.Point{X: to.X + 0.001, Y: to.Y})
	checkParams(".*missing.*limit expected.*", heading, nil, limit1)

	// Invalid limit 2
	checkParams(".*must be a limit.*", heading, nil, limit1, swapi.MakeHeading(0))

	// Limit2 is empty
	checkParams(".*limit value is invalid.*", heading, nil, limit1, swapi.MakeLimit())

	// Limit1 is null but not limit2
	limit2 := swapi.MakeLimit(
		swapi.Point{X: from.X - 0.001, Y: from.Y},
		swapi.Point{X: to.X - 0.001, Y: to.Y})
	checkParams(".*must be both null.*", heading, nil, nil, limit2)

	// Limit2 is empty but not limit1
	checkParams(".*must be both null.*", heading, nil, limit1, nil)

	// Limits are equal
	checkParams(".*or different.*", heading, nil, limit1, limit1)

	// Missing destination
	checkParams(".*got 4 parameters, an additional LocationComposite is expected.*",
		heading, nil, limit1, limit2)

	// Invalid destination type
	checkParams(".*must be a LocationComposite.*", heading, nil, limit1, limit2, heading)

	// With nil limits
	checkParams("", heading, nil, nil, nil, dest)

	// With valid limits
	checkParams("", heading, nil, limit1, limit2, dest)
}

// Test we can send a automat mission and get a successful acknowledgement.
func (s *TestSuite) TestAutomatMission(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	automat := createAutomat(c, client)
	from := swapi.Point{X: -15.9219, Y: 28.3456}

	limit11 := swapi.Point{X: -15.8241, Y: 28.3241}
	limit12 := swapi.Point{X: -15.8092, Y: 28.3458}
	limit21 := swapi.Point{X: -15.8066, Y: 28.3156}
	limit22 := swapi.Point{X: -15.7941, Y: 28.3410}

	unit, err := client.CreateUnit(automat.Id, UnitType, from)
	c.Assert(err, IsNil)

	// Test trailing optional parameters can be left out
	// There is a "lastEchelonNumber" optional parameter for Attack
	params := swapi.MakeParameters(
		swapi.MakeHeading(0),
		nil,
		swapi.MakeLimit(limit11, limit12),
		swapi.MakeLimit(limit21, limit22),
	)

	// Cannot send order with an invalid unit identifier
	_, err = client.SendAutomatOrder(InvalidIdentifier, MissionAutomatAttackId, params)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot order with an invalid mission identifier
	_, err = client.SendAutomatOrder(automat.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_mission")

	err = client.SetAutomatMode(automat.Id, false)
	c.Assert(err, IsNil)

	// Cannot send order to an disengaged automat
	_, err = client.SendAutomatOrder(automat.Id, MissionAutomatAttackId, params)
	c.Assert(err, IsSwordError, "error_unit_cannot_receive_order")

	err = client.SetAutomatMode(automat.Id, true)
	c.Assert(err, IsNil)

	automatOrder, err := client.SendAutomatOrder(automat.Id, MissionAutomatAttackId, params)
	c.Assert(err, IsNil)
	c.Assert(automatOrder.Kind, Equals, swapi.AutomatOrder)

	// The automat sends orders to its units
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		for _, unitOrder := range data.Orders {
			if unitOrder.TaskerId == unit.Id {
				return unitOrder.ParentId == automatOrder.Id
			}
		}
		return false
	})

	// Send again with an invalid last parameter, to check the mission has an
	// optional additional parameter.
	params = swapi.MakeParameters(
		swapi.MakeHeading(0),
		nil,
		swapi.MakeLimit(limit11, limit12),
		swapi.MakeLimit(limit21, limit22),
		swapi.MakeLimit(limit21, limit22),
	)
	_, err = client.SendAutomatOrder(automat.Id, MissionAutomatAttackId, params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")
}

// Test we can send a crowd mission and get a successful acknowledgement.
func (s *TestSuite) TestCrowdMission(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()
	from := swapi.Point{X: -15.9219, Y: 28.3456}
	to := swapi.Point{X: -15.8193, Y: 28.3456}

	party := data.FindPartyByName("party")
	c.Assert(party, NotNil)

	crowd, err := client.CreateCrowd(party.Id, 0, CrowdType, from, 10, 10, 10, "crowd")
	c.Assert(err, IsNil)
	c.Assert(crowd, NotNil)

	params := swapi.MakeParameters(
		swapi.MakePointParam(to))

	// Cannot send order with an invalid unit identifier
	_, err = client.SendCrowdOrder(InvalidIdentifier, MissionMoveCrowdId, params)
	c.Assert(err, IsSwordError, "error_invalid_unit")

	// Cannot order with an invalid mission identifier
	_, err = client.SendCrowdOrder(crowd.Id, InvalidIdentifier, params)
	c.Assert(err, IsSwordError, "error_invalid_mission")

	order, err := client.SendCrowdOrder(crowd.Id, MissionMoveCrowdId, params)
	c.Assert(err, IsNil)
	c.Assert(order.Kind, Equals, swapi.CrowdOrder)
}
