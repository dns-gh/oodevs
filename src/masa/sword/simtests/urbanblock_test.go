// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package simtests

import (
	. "launchpad.net/gocheck"
	"masa/sword/swapi"
	"masa/sword/sword"
)

func (s *TestSuite) TestUrbanStructuralState(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	data := client.Model.GetData()
	blockId := uint32(35)

	// Check initial structural state(100%)
	block := data.Objects[blockId]
	c.Assert(block, Not(IsNil))
	c.Assert(block.Urban.State, Equals, uint32(100))

	// Destroy urban block with 'fire on location'
	for i := 0; i != 6; i++ {
		err := client.CreateFireOnLocation(swapi.Point{X: -15.8217, Y: 28.3477},
			ResourceTypeWithIndirectFire, 1)
		c.Assert(err, IsNil)
	}

	// Minimum urban block state is 1%
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Objects[blockId].Urban.State == 1
	})

	// With 'Change urban state' magic action, the minimum can be 0%.
	params := swapi.MakeList(
		swapi.MakeIdentifier(uint32(sword.ObjectMagicAction_structural_state)),
		swapi.MakeFloat(0),
	)
	err := client.UpdateObject(blockId, params)
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return data.Objects[blockId].Urban.State == 0
	})

	// Check the state stays 0% after an another fire
	err = client.CreateFireOnLocation(swapi.Point{X: -15.8217, Y: 28.3477},
		ResourceTypeWithIndirectFire, 1)
	c.Assert(err, IsNil)

	client.Model.WaitTicks(2)
	c.Assert(client.Model.GetData().Objects[blockId].Urban.State, Equals, uint32(0))
}
