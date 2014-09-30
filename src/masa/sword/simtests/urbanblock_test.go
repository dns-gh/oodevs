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
	"reflect"
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

func (s *TestSuite) TestUrbanResourceNetworkChange(c *C) {
	sim, client := connectAndWaitModel(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)

	// error: invalid parameters count, 2 parameters expected
	params := swapi.MakeParameters()
	err := client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a location or an identifier
	params = swapi.MakeParameters(nil, nil)
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(1000))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// first parameter with a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(21))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsNil)

	// error: second parameter must be a list with valid resource names
	params.Elem[1] = swapi.MakeParameter(
		swapi.MakeList(swapi.MakeString("Running Water")),
		swapi.MakeList(swapi.MakeString("invalid resource name")))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// second parameter with valid resource names
	params.Elem[1] = swapi.MakeParameter(
		swapi.MakeList(swapi.MakeString("Running Water")),
		swapi.MakeList(swapi.MakeString("Natural Gas")))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsNil)

	// update full parameters
	urban := swapi.NewUrban(21, "Bloc urbain [21]", 100,
		map[string]*swapi.ResourceNetwork{
			"Running Water": {Name: "Running Water",
				Consumption: 1, Critical: true, Activated: false, Production: 2,
				StockMax: 3},
			"Natural Gas": {Name: "Natural Gas",
				Consumption: 4, Critical: false, Activated: true, Production: 5,
				StockMax: 6},
		})
	err = client.ChangeResourceNetwork(urban)
	c.Assert(err, IsNil)

	// model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(urban.Urban.ResourceNetworks,
			data.Objects[21].Urban.ResourceNetworks)
	})

	// the magic action changes resources network values totally
	urban = swapi.NewUrban(21, "Bloc urbain [21]", 100,
		map[string]*swapi.ResourceNetwork{
			"Electricity": {Name: "Electricity",
				Consumption: 7, Critical: true, Activated: true, Production: 8,
				StockMax: 9},
		})
	err = client.ChangeResourceNetwork(urban)
	c.Assert(err, IsNil)

	// model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(urban.Urban.ResourceNetworks,
			data.Objects[21].Urban.ResourceNetworks)
	})
}
