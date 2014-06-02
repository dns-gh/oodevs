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
	"reflect"
)

func CheckHealthState(c *C, client *swapi.Client,
	id uint32, healthy, wounded, dead int32) {
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Populations[id]
		return unit.Healthy == healthy &&
			unit.Wounded == wounded &&
			unit.Dead == dead
	})
}

func CheckResidentCount(c *C, client *swapi.Client,
	id uint32, healthy, wounded, dead int32) {
	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		unit := data.Populations[id]
		residents := int32(0)
		for _, o := range unit.LivingArea {
			for _, r := range o.Residents {
				residents += r
			}
		}
		return residents == healthy+wounded+dead
	})
}

func (s *TestSuite) TestChangeHealthState(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	model := client.Model
	data := model.GetData()

	// Get a population
	population := getSomePopulation(c, data)

	// Check initial health state
	healthy := int32(100)
	wounded := int32(0)
	dead := int32(0)
	CheckHealthState(c, client, population.Id, healthy, wounded, dead)

	// Check living area
	CheckResidentCount(c, client, population.Id, healthy, wounded, dead)

	// Error: Quantity must be positive
	err := client.ChangePopulationHealthState(population.Id, -1, 0, 0)
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[0\] must be a positive number`)
	err = client.ChangePopulationHealthState(population.Id, 0, -1, 0)
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[1\] must be a positive number`)
	err = client.ChangePopulationHealthState(population.Id, 0, 0, -1)
	c.Assert(err, ErrorMatches, `error_invalid_parameter: parameters\[2\] must be a positive number`)

	// Change health state
	healthy = int32(10)
	wounded = int32(500)
	dead = int32(600)
	err = client.ChangePopulationHealthState(population.Id, 10, 500, 600)
	c.Assert(err, IsNil)

	// Check health state
	CheckHealthState(c, client, population.Id, healthy, wounded, dead)

	// Check living area
	CheckResidentCount(c, client, population.Id, healthy, wounded, dead)
}

func (s *TestSuite) TestPopulationChangeAdhesions(c *C) {
	sim, client := connectAndWaitModel(c, NewAllUserOpts(ExCrossroadSmallOrbat))
	defer stopSimAndClient(c, sim, client)
	model := client.Model
	data := model.GetData()

	// Get a population
	c.Assert(len(data.Populations), Greater, 0)
	population := getSomePopulation(c, data)
	adhesions := map[uint32]float32{1: 0, 2: 0, 33: 0}

	population = data.Populations[population.Id]
	// Check initial state
	c.Assert(population.Adhesions, DeepEquals, adhesions)

	// Error : missing parameter
	err := client.ChangePopulationAdhesions(population.Id, map[uint32]float32{})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Error : adhesion must be between -1 and 1
	adhesions = map[uint32]float32{1: 1.1, 2: 5.2, 33: 10}
	err = client.ChangePopulationAdhesions(population.Id, adhesions)
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	// Change population adhesions
	adhesions = map[uint32]float32{1: 0.7, 2: -0.5, 33: 1}
	err = client.ChangePopulationAdhesions(population.Id, adhesions)
	c.Assert(err, IsNil)

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Populations[population.Id].Adhesions, adhesions)
	})

	// No change adhesions if new adhesions are invalid
	err = client.ChangePopulationAdhesions(population.Id,
		map[uint32]float32{1: -1.1, 2: -5.2, 33: 10})
	c.Assert(err, IsSwordError, "error_invalid_parameter")

	waitCondition(c, client.Model, func(data *swapi.ModelData) bool {
		return reflect.DeepEqual(data.Populations[population.Id].Adhesions, adhesions)
	})
}
