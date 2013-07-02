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
	"reflect"
	"swapi"
	"sword"
)

const (
	// Resource type without indirect fire
	ResourceTypeWithoutIndirectFire = uint32(1)
	// Resource type with indirect fire
	ResourceTypeWithIndirectFire = uint32(8)
)

func (s *TestSuite) TestChangeDiplomacy(c *C) {
	sim, client := connectAllUserAndWait(c, ExCrossroadSmallOrbat)
	defer sim.Stop()

	//check with no parameters
	params := swapi.MakeParameters()
	err := client.ChangeDiplomacyTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	//check with wrong party
	err = client.ChangeDiplomacy(0, 2, sword.EnumDiplomacy_enemy)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	err = client.ChangeDiplomacy(2, 0, sword.EnumDiplomacy_enemy)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	//check with wrong param number
	params = swapi.MakeParameters(
		swapi.MakeIdentifier(1),
		swapi.MakeIdentifier(2))
	err = client.ChangeDiplomacyTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	//check with all parameters
	model := client.Model.GetData()
	parties := model.Parties
	c.Assert(parties, HasLen, 2)

	party1 := parties[1]
	party2 := parties[2]
	c.Assert(party1, NotNil)
	c.Assert(party2, NotNil)
	diplomacy := sword.EnumDiplomacy_enemy
	err = client.ChangeDiplomacy(party1.Id, party2.Id, diplomacy)
	c.Assert(err, IsNil)

	parties = client.Model.GetData().Parties
	party1 = parties[1]
	party2 = parties[2]
	c.Assert(party1.Diplomacies[party2.Id], Equals, sword.EnumDiplomacy_enemy)
}

func (s *TestSuite) TestFireOrderCreation(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()
	point := swapi.Point{X: -15.8241, Y: 28.3241}

	// error: invalid parameters count, 3 parameters expected
	params := swapi.MakeParameters()
	err := client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_target")

	// error: first parameter must be a location or a point
	params = swapi.MakeParameters(
		swapi.MakeNullValue(),
		swapi.MakeNullValue(),
		swapi.MakeNullValue())
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_target")

	// error: second parameter must be a resource type
	params.Elem[0] = swapi.MakePointParam(point)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	// error: second parameter must be a valid resource type
	params.Elem[1] = swapi.MakeResourceType(uint32(12345))
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	// error: second parameter must be a resource type with a indirect fire
	params.Elem[1] = swapi.MakeResourceType(ResourceTypeWithoutIndirectFire)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_ammunition")

	// error: third parameter must be a real
	params.Elem[1] = swapi.MakeResourceType(ResourceTypeWithIndirectFire)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_iteration")

	// error: third parameter must be a positive real number
	params.Elem[2] = swapi.MakeFloat(float32(-3))
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_iteration")

	// indirect fire with a point
	params.Elem[2] = swapi.MakeFloat(float32(5))
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsNil)

	// indirect fire with a location
	params.Elem[0] = swapi.MakeLocationParam(point)
	err = client.CreateFireOnLocationTest(params)
	c.Assert(err, IsNil)

	err = client.CreateFireOnLocation(point, ResourceTypeWithIndirectFire, 2)
	c.Assert(err, IsNil)
}

func (s *TestSuite) TestResourceNetworkChange(c *C) {
	sim, client := connectAndWaitModel(c, "admin", "", ExCrossroadSmallOrbat)
	defer sim.Stop()

	// error: invalid parameters count, 2 parameters expected
	params := swapi.MakeParameters()
	err := client.ChangeResourceNetworkTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: first parameter must be a location or an identifier
	params = swapi.MakeParameters(swapi.MakeNullValue(), swapi.MakeNullValue())
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// error: first parameter must be a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(1000))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// first parameter with a valid identifier
	params.Elem[0] = swapi.MakeIdentifier(uint32(21))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsNil)

	// error: second parameter must be a list with valid resource names
	params.Elem[1] = swapi.MakeParameter(
		swapi.MakeList(swapi.MakeString("Running Water")),
		swapi.MakeList(swapi.MakeString("invalid resource name")))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, ErrorMatches, "error_invalid_parameter")

	// second parameter with valid resource names
	params.Elem[1] = swapi.MakeParameter(
		swapi.MakeList(swapi.MakeString("Running Water")),
		swapi.MakeList(swapi.MakeString("Natural Gas")))
	err = client.ChangeResourceNetworkTest(params)
	c.Assert(err, IsNil)

	// update full parameters
	urban := swapi.NewUrban(21, "Bloc urbain [21]",
		map[string]*swapi.ResourceNetwork{
			"Running Water": &swapi.ResourceNetwork{Name: "Running Water",
				Consumption: 1, Critical: true, Activated: false, Production: 2,
				StockMax: 3},
			"Natural Gas": &swapi.ResourceNetwork{Name: "Natural Gas",
				Consumption: 4, Critical: false, Activated: true, Production: 5,
				StockMax: 6},
		})
	err = client.ChangeResourceNetwork(urban)
	c.Assert(err, IsNil)

	// model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(urban, data.Urbans[21])
	})

	// the magic action changes resources network values totally
	urban = swapi.NewUrban(21, "Bloc urbain [21]",
		map[string]*swapi.ResourceNetwork{
			"Electricity": &swapi.ResourceNetwork{Name: "Electricity",
				Consumption: 7, Critical: true, Activated: true, Production: 8,
				StockMax: 9},
		})
	err = client.ChangeResourceNetwork(urban)
	c.Assert(err, IsNil)

	// model updated
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(urban, data.Urbans[21])
	})
}
