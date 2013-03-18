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
	"swapi"
	"swapi/simu"
	"time"
)

func startSimOnExercise(c *C, exercise string, endTick int,
	paused bool) *simu.SimProcess {

	opts := MakeOpts()
	opts.ExerciseName = exercise
	opts.ConnectTimeout = 20 * time.Second

	session := simu.CreateDefaultSession()
	session.Paused = paused
	WriteSession(c, opts, session)
	sim, err := simu.StartSim(opts)
	c.Assert(err, IsNil) // failed to start the simulation
	return sim
}

func ConnectClient(c *C, sim *simu.SimProcess) *swapi.Client {
	client, err := swapi.Connect(sim.DispatcherAddr)
	client.PostTimeout = 3 * time.Minute
	c.Assert(err, IsNil) // failed to connect to simulation
	return client
}

func (s *TestSuite) TestLogin(c *C) {
	addContextChecker := func(client *swapi.Client) {
		// Capture response contexts and double check them
		client.Register(func(msg *swapi.SwordMessage, ctx int32,
			err error) bool {
			if msg != nil && msg.AuthenticationToClient != nil {
				auth := msg.AuthenticationToClient
				resp := auth.GetMessage().GetAuthenticationResponse()
				if resp != nil {
					if auth.GetContext() != ctx {
						/* SWBUG-10025

						   t.Fatalf("AuthenticationResponse context does not "+
						       "match: %v != %v", auth.GetContext(), ctx)
						*/
					}
					return true
				}
			}
			return false
		})
	}

	sim := startSimOnExercise(c, "crossroad-small-empty", 1000, false)
	defer sim.Stop()

	// Test invalid login
	client := ConnectClient(c, sim)
	// Also check returned context is valid
	addContextChecker(client)
	err := client.Login("foo", "bar")
	c.Assert(err, ErrorMatches, "invalid_login")
	client.Close()

	// Test invalid version
	client = ConnectClient(c, sim)
	err = client.LoginWithVersion("admin", "user", "1.0")
	c.Assert(err, ErrorMatches, "mismatched_protocol_version")
	client.Close()

	// Test valid login
	client = ConnectClient(c, sim)
	addContextChecker(client)
	err = client.Login("admin", "")
	c.Assert(err, IsNil) // login failed

	// Test model readyness
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // model initialization timed out

	ok = client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // second wait for model initialization timed out
	c.Assert(client.Model.IsReady(), Equals, true)
	client.Close()
}

func waitForMessages(client *swapi.Client, timeout time.Duration) bool {
	waitch := make(chan int)
	msgch := make(chan int, 32)

	handler := func(msg *swapi.SwordMessage, ctx int32, err error) bool {
		if msg != nil && msg.AuthenticationToClient != nil {
			auth := msg.AuthenticationToClient
			resp := auth.GetMessage().GetAuthenticationResponse()
			if resp != nil {
				return false
			}
		}
		msgch <- 1
		return false
	}
	client.Register(handler)
	go func() {
		time.Sleep(timeout)
		waitch <- 1
	}()
	select {
	case <-waitch:
		return false
	case <-msgch:
		break
	}
	return true
}

func (s *TestSuite) TestNoDataSentUntilSuccessfulLogin(c *C) {
	sim := startSimOnExercise(c, "crossroad-small-empty", 1000, false)
	defer sim.Stop()

	// Connect and watch incoming messages
	client := ConnectClient(c, sim)
	seen := waitForMessages(client, 5*time.Second)
	if seen {
		/* SWBUG-10026

		   t.Fatal("messages seen before any client action")
		*/
	}
	err := client.Login("foo", "bar")
	c.Assert(err, NotNil) // login with invalid credentials should have failed
	seen = waitForMessages(client, 5*time.Second)
	if seen {
		/* SWBUG-10026

		   t.Fatal("message seen after invalid login")
		*/
	}
}

func (s *TestSuite) TestListConnectedProfiles(c *C) {

	checkProfile := func(p *swapi.Profile, login string, supervisor bool) {
		c.Assert(p.Login, Equals, login)
		c.Assert(p.Password, Equals, "")
		c.Assert(p.Supervisor, Equals, supervisor)
	}

	connect := func(sim *simu.SimProcess, user, password string) *swapi.Client {
		client := ConnectClient(c, sim)
		err := client.Login(user, password)
		c.Assert(err, IsNil)
		ok := client.Model.WaitReady(10 * time.Second)
		c.Assert(ok, Equals, true)
		return client
	}

	sim := startSimOnExercise(c, "crossroad-small-empty", 1000, false)
	defer sim.Stop()

	// The dispatcher sends connected user lists upon authentication, which
	// without proper 'context' can confuse the command callback and make it
	// think it actually did something. Instead, wait for the model to be
	// initialized so all this noise is behind us.
	client := connect(sim, "admin", "")
	defer client.Close() // need to release a connection for graceful Stop()
	client2 := connect(sim, "user1", "user1")

	// Admin can get itself
	profiles, err := client.ListConnectedProfiles()
	c.Assert(err, IsNil)
	c.Assert(len(profiles), Equals, 2)
	checkProfile(profiles[0], "admin", true)
	checkProfile(profiles[1], "user1", false)

	// Regular use can get admin?
	profiles, err = client2.ListConnectedProfiles()
	c.Assert(err, IsNil)
	c.Assert(len(profiles), Equals, 2)
	checkProfile(profiles[0], "admin", true)
	checkProfile(profiles[1], "user1", false)
}
