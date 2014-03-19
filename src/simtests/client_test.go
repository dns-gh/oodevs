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
	"code.google.com/p/goprotobuf/proto"
	. "launchpad.net/gocheck"
	"swapi"
	"sword"
	"time"
)

func (s *TestSuite) TestPostTimeout(c *C) {
	sim := startSimOnExercise(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSim(c, sim, nil)
	client := connectClient(c, sim, nil)
	defer client.Close()
	err := client.Login("admin", "")
	c.Assert(err, IsNil) // login failed
	client.PostTimeout = 3 * time.Second

	// Could be any message, we just need one to call Post()
	msg := swapi.SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlResume: &sword.ControlResume{
					Tick: proto.Uint32(1),
				},
			},
		},
	}
	// Post some dummy never ending handler
	quit := make(chan error, 1)
	client.Post(msg, func(msg *swapi.SwordMessage, id, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		return false
	})
	select {
	case err := <-quit:
		c.Assert(err, ErrorMatches, "(?i).*timeout.*")
	case <-time.After(2 * client.PostTimeout):
		c.Fatal("handler should have quit on timeout")
	}
}
