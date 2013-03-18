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
	sim := startSimOnExercise(c, "crossroad-small-empty", 1000, false)
	defer sim.Stop()

	// Test invalid login
	client := ConnectClient(c, sim)
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

func connectAndWait(c *C, sim *simu.SimProcess, user, password string) *swapi.Client {
	client := ConnectClient(c, sim)
	err := client.Login(user, password)
	c.Assert(err, IsNil)
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true)
	return client
}

func (s *TestSuite) TestListConnectedProfiles(c *C) {
	checkProfile := func(p *swapi.Profile, login string, supervisor bool) {
		c.Assert(p.Login, Equals, login)
		c.Assert(p.Password, Equals, "")
		c.Assert(p.Supervisor, Equals, supervisor)
	}

	sim := startSimOnExercise(c, "crossroad-small-empty", 1000, false)
	defer sim.Stop()

	// The dispatcher sends connected user lists upon authentication, which
	// without proper 'context' can confuse the command callback and make it
	// think it actually did something. Instead, wait for the model to be
	// initialized so all this noise is behind us.
	client := connectAndWait(c, sim, "admin", "")
	defer client.Close() // need to release a connection for graceful Stop()
	client2 := connectAndWait(c, sim, "user1", "user1")

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

func (s *TestSuite) TestProfileEditing(c *C) {
	sim := startSimOnExercise(c, "crossroad-small-empty", 1000, false)
	defer sim.Stop()

	userProfile := &swapi.Profile{
		Login:      "userprofile",
		Password:   "",
		Supervisor: false,
	}
	adminProfile := &swapi.Profile{
		Login:      "adminprofile",
		Password:   "adminpassword",
		Supervisor: true,
	}
	emptyLoginProfile := &swapi.Profile{
		Login:      "",
		Password:   "adminpassword",
		Supervisor: false,
	}

	// A regular user cannot do anything
	user := connectAndWait(c, sim, "user1", "user1")
	_, err := user.CreateProfile(userProfile)
	c.Assert(err, ErrorMatches, "forbidden")

	_, err = user.UpdateProfile("user1", userProfile)
	c.Assert(err, ErrorMatches, "forbidden")

	err = user.DeleteProfile("user2")
	c.Assert(err, ErrorMatches, "forbidden")
	user.Close()

	// An admin can create regular users
	admin := connectAndWait(c, sim, "admin", "")
	profile, err := admin.CreateProfile(userProfile)
	c.Assert(err, IsNil)
	c.Assert(profile, NotNil)
	c.Assert(profile.Login, Equals, userProfile.Login)

	// And supervisor as well
	profile, err = admin.CreateProfile(adminProfile)
	c.Assert(err, IsNil)
	c.Assert(profile, NotNil)
	c.Assert(profile.Login, Equals, adminProfile.Login)

	// Test empty login
	profile, err = admin.CreateProfile(emptyLoginProfile)
	c.Assert(err, ErrorMatches, "invalid_login")

	// Test duplicate login
	profile, err = admin.CreateProfile(adminProfile)
	c.Assert(err, ErrorMatches, "duplicate_login")

	// Regular profile update
	userProfile.Password = "userpassword"
	profile, err = admin.UpdateProfile(userProfile.Login, userProfile)
	c.Assert(err, IsNil)
	c.Assert(profile, NotNil)
	c.Assert(profile.Password, Equals, userProfile.Password)

	// Duplicating through update
	profile, err = admin.UpdateProfile("user1", userProfile)
	c.Assert(err, ErrorMatches, "duplicate_login")

	// Updating with empty login
	profile, err = admin.UpdateProfile(userProfile.Login, emptyLoginProfile)
	c.Assert(err, ErrorMatches, "invalid_login")

	// Updating missing profile
	profile, err = admin.UpdateProfile("missing", userProfile)
	c.Assert(err, ErrorMatches, "invalid_profile")

	// Delete myself
	err = admin.DeleteProfile("admin")
	c.Assert(err, ErrorMatches, "invalid_profile")

	// Delete valid profile
	err = admin.DeleteProfile("user2")
	c.Assert(err, IsNil)
	user2 := ConnectClient(c, sim)
	err = user2.Login("user2", "user2")
	c.Assert(err, ErrorMatches, "invalid_login")
	user2.Close()

	// Delete missing profile
	err = admin.DeleteProfile("missing")
	c.Assert(err, ErrorMatches, "invalid_profile")

	admin.Close()
	// Check the users are usable
	user = connectAndWait(c, sim, userProfile.Login, userProfile.Password)
	user.Close()
	admin = connectAndWait(c, sim, adminProfile.Login, adminProfile.Password)
	// Check user creation/deletion are still applied
	c.Assert(admin.Model.GetProfile("user2"), IsNil)
	c.Assert(admin.Model.GetProfile(userProfile.Login), NotNil)
	admin.Close()
	// And taken in account for removed...
	user2 = ConnectClient(c, sim)
	err = user2.Login("user2", "user2")
	c.Assert(err, ErrorMatches, "invalid_login")
	user2.Close()
	// ... and created profiles
	user = ConnectClient(c, sim)
	err = user.Login(userProfile.Login, userProfile.Password)
	c.Assert(err, IsNil)
	user.Close()
}
