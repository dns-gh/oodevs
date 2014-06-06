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
	"masa/sword/swapi/simu"
	"masa/sword/sword"
	"time"
)

func (s *TestSuite) TestLogin(c *C) {
	sim := startSimOnExercise(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSim(c, sim, nil)

	// Test invalid login
	client := connectClient(c, sim, nil)
	c.Assert(client.GetClientId(), Equals, int32(0))
	err := client.Login("foo", "bar")
	c.Assert(err, IsSwordError, "invalid_login")
	c.Assert(client.GetClientId(), Equals, int32(0))
	client.Close()

	// Test invalid version
	client = connectClient(c, sim, nil)
	err = client.LoginWithVersion("admin", "user", "1.0", "")
	c.Assert(err, IsSwordError, "mismatched_protocol_version")
	client.Close()

	// Test valid login
	client = connectClient(c, sim, nil)
	err = client.Login("admin", "")
	c.Assert(err, IsNil) // login failed
	c.Assert(client.GetClientId(), Not(Equals), int32(0))

	// Test model readyness
	ok := client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // model initialization timed out

	ok = client.Model.WaitReady(10 * time.Second)
	c.Assert(ok, Equals, true) // second wait for model initialization timed out
	c.Assert(client.Model.IsReady(), Equals, true)
	client.Close()

	// Test retrieve authentication key
	client = connectClient(c, sim, nil)
	key, err := client.GetAuthenticationKey()
	c.Assert(key, Not(IsNil))
	c.Assert(err, IsNil)

	// For a non-connected client, the simulation send a same key
	key2, err := client.GetAuthenticationKey()
	c.Assert(key2, Not(IsNil))
	c.Assert(err, IsNil)
	c.Assert(key, Equals, key2)

	// Test invalid authentication key
	err = client.LoginWithAuthenticationKey("admin", "", "invalid")
	c.Assert(err, IsSwordError, "invalid_authentication_key")

	// Test valid login
	err = client.LoginWithAuthenticationKey("admin", "", key)
	c.Assert(err, IsNil)
	client.Close()

	// Test supervisor login
	client = connectClient(c, sim, nil)
	key, err = client.GetAuthenticationKey()
	c.Assert(key, Not(IsNil))
	c.Assert(err, IsNil)
	err = client.LoginWithAuthenticationKey("", "", key)
	c.Assert(err, IsNil)
	client.Close()
}

func (s *TestSuite) TestMaxConnections(c *C) {
	sim := startSimOnExercise(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSim(c, sim, nil)

	// Authenticate a maximum of clients
	clients := []*swapi.Client{}
	defer func() {
		for _, client := range clients {
			client.Close()
		}
	}()
	for {
		client := connectClient(c, sim, nil)
		if client.Login("admin", "") != nil {
			client.Close()
			break
		}
		clients = append(clients, client)
		c.Assert(len(clients), Lesser, 100)
	}
	c.Assert(len(clients), Greater, 0)

	// Authenticate client with an authentication key
	client := connectClient(c, sim, nil)
	key, err := client.GetAuthenticationKey()
	c.Assert(key, Not(IsNil))
	c.Assert(err, IsNil)
	err = client.LoginWithAuthenticationKey("", "", key)
	c.Assert(err, IsNil)
	client.Close()

	// Disconnect one client
	last := len(clients) - 1
	client = clients[last]
	client.Close()
	clients = clients[:last]

	// Reconnect one client
	client = connectClient(c, sim, nil)
	err = client.Login("admin", "")
	c.Assert(err, IsNil)
	clients = append(clients, client)
}

func (s *TestSuite) TestNoDataSentUntilSuccessfulLogin(c *C) {
	c.Skip("unreliable, see http://jira.masagroup.net/browse/SWBUG-11396")
	waitForMessages := func(timeout time.Duration, seen chan *swapi.SwordMessage) {
		select {
		case <-time.After(timeout):
			break
		case msg := <-seen:
			if msg != nil {
				c.Fatalf("messages seen before any client action: %v", msg)
			}
		}
	}

	sim := startSimOnExercise(c, NewAdminOpts(ExCrossroadSmallOrbat))
	defer stopSim(c, sim, nil)

	// Connect and watch incoming messages
	client := connectClient(c, sim, nil)
	defer client.Close()

	msgch := make(chan *swapi.SwordMessage, 1)
	handler := func(msg *swapi.SwordMessage, id, ctx int32, err error) bool {
		if err != nil {
			if len(msgch) < cap(msgch) {
				msgch <- nil
			}
			return true
		}
		if msg == nil {
			return false
		}
		if msg.AuthenticationToClient != nil {
			auth := msg.AuthenticationToClient
			if auth.GetMessage().GetAuthenticationResponse() != nil ||
				// ConnectedProfileList is sent after a failed authentication
				auth.GetMessage().GetConnectedProfileList() != nil {
				return false
			}
		}
		if msg.DispatcherToClient != nil &&
			msg.DispatcherToClient.GetMessage() != nil &&
			msg.DispatcherToClient.GetMessage().ServicesDescription != nil {
			return false
		}
		if msg.SimulationToClient != nil &&
			msg.SimulationToClient.GetMessage() != nil {
			// Ignore "forbidden" messages
			m := msg.SimulationToClient.GetMessage()
			if m.ControlPauseAck != nil {
				if m.ControlPauseAck.GetErrorCode() == sword.ControlAck_error_forbidden {
					return false
				}
			} else if m.ControlResumeAck != nil {
				if m.ControlResumeAck.GetErrorCode() == sword.ControlAck_error_forbidden {
					return false
				}
			}
		}

		if len(msgch) < cap(msgch) {
			received := &swapi.SwordMessage{}
			swapi.DeepCopy(received, msg)
			msgch <- received
		}
		return true
	}
	client.Register(handler)

	// Trigger simulation_client messages
	other := loginAndWaitModel(c, sim, NewAllUserOpts(""))
	createAutomat(c, other)

	// Try invalid login followed by Pause/Resume (SWBUG-11716)
	client.Login("foo", "bar")
	client.Pause()
	client.Resume(0)
	waitForMessages(2*time.Second, msgch)

	// Trigger destruction events
	sim.Stop()
	waitForMessages(5*time.Second, msgch)
}

func connectAndWait(c *C, sim *simu.SimProcess, user, password string) *swapi.Client {
	client := connectClient(c, sim, nil)
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

	sim := startSimOnExercise(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSim(c, sim, nil)

	// The dispatcher sends connected user lists upon authentication, which
	// without proper 'context' can confuse the command callback and make it
	// think it actually did something. Instead, wait for the model to be
	// initialized so all this noise is behind us.
	client := connectAndWait(c, sim, "admin", "")
	defer client.Close() // need to release a connection for graceful Stop()
	client2 := connectAndWait(c, sim, "user1", "user1")
	defer client2.Close()

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
	sim := startSimOnExercise(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSim(c, sim, nil)

	userProfile := &swapi.Profile{
		Login:      "userprofile",
		Password:   "",
		Supervisor: false,
	}
	adminProfile := &swapi.Profile{
		Login:       "adminprofile",
		Password:    "adminpassword",
		Supervisor:  true,
		TimeControl: false,
	}
	emptyLoginProfile := &swapi.Profile{
		Login:      "",
		Password:   "adminpassword",
		Supervisor: false,
	}

	// A regular user cannot do anything
	user := connectAndWait(c, sim, "user1", "user1")
	_, err := user.CreateProfile(userProfile)
	c.Assert(err, IsSwordError, "forbidden")

	_, err = user.UpdateProfile("user1", userProfile)
	c.Assert(err, IsSwordError, "forbidden")

	err = user.DeleteProfile("user2")
	c.Assert(err, IsSwordError, "forbidden")
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
	c.Assert(err, IsSwordError, "invalid_login")

	// Test duplicate login
	profile, err = admin.CreateProfile(adminProfile)
	c.Assert(err, IsSwordError, "duplicate_login")

	// Regular profile update
	userProfile.Password = "userpassword"
	profile, err = admin.UpdateProfile(userProfile.Login, userProfile)
	c.Assert(err, IsNil)
	c.Assert(profile, NotNil)
	c.Assert(profile.Password, Equals, userProfile.Password)

	// Supervisor without time control cannot add time control to itself
	supervisor := connectAndWait(c, sim, "adminprofile", "adminpassword")
	adminProfile.TimeControl = true
	profile, err = supervisor.UpdateProfile(adminProfile.Login, adminProfile)
	c.Assert(err, IsSwordError, "forbidden")

	// But admin can
	profile, err = admin.UpdateProfile(adminProfile.Login, adminProfile)
	c.Assert(err, IsNil)
	c.Assert(profile, NotNil)
	c.Assert(profile.TimeControl, Equals, adminProfile.TimeControl)

	// Duplicating through update
	profile, err = admin.UpdateProfile("user1", userProfile)
	c.Assert(err, IsSwordError, "duplicate_login")

	// Updating with empty login
	profile, err = admin.UpdateProfile(userProfile.Login, emptyLoginProfile)
	c.Assert(err, IsSwordError, "invalid_login")

	// Updating missing profile
	profile, err = admin.UpdateProfile("missing", userProfile)
	c.Assert(err, IsSwordError, "invalid_profile")

	// Delete myself
	err = admin.DeleteProfile("admin")
	c.Assert(err, IsSwordError, "invalid_profile")

	// Delete valid profile
	err = admin.DeleteProfile("user2")
	c.Assert(err, IsNil)
	user2 := connectClient(c, sim, nil)
	err = user2.Login("user2", "user2")
	c.Assert(err, IsSwordError, "invalid_login")
	user2.Close()

	// Delete missing profile
	err = admin.DeleteProfile("missing")
	c.Assert(err, IsSwordError, "invalid_profile")

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
	user2 = connectClient(c, sim, nil)
	err = user2.Login("user2", "user2")
	c.Assert(err, IsSwordError, "invalid_login")
	user2.Close()
	// ... and created profiles
	user = connectClient(c, sim, nil)
	err = user.Login(userProfile.Login, userProfile.Password)
	c.Assert(err, IsNil)
	user.Close()
}

func (s *TestSuite) TestProfileTimeControl(c *C) {
	sim := startSimOnExercise(c, NewAdminOpts(ExCrossroadSmallEmpty))
	defer stopSim(c, sim, nil)

	client := connectAndWait(c, sim, "admin", "")
	without, err := client.CreateProfile(&swapi.Profile{
		Login:       "without",
		Password:    "",
		Supervisor:  true,
		TimeControl: false,
	})
	c.Assert(err, IsNil)
	with, err := client.CreateProfile(&swapi.Profile{
		Login:       "with",
		Password:    "",
		Supervisor:  true,
		TimeControl: true,
	})
	c.Assert(err, IsNil)
	client.Close()

	client = connectAndWait(c, sim, without.Login, without.Password)
	_, err = client.Pause()
	c.Assert(err, NotNil)
	_, _, err = client.Resume(0)
	c.Assert(err, NotNil)
	err = client.Stop()
	c.Assert(err, NotNil)
	client.Close()

	client = connectAndWait(c, sim, with.Login, with.Password)
	_, err = client.Pause()
	c.Assert(err, IsNil)
	_, _, err = client.Resume(0)
	c.Assert(err, IsNil)
	err = client.Stop()
	c.Assert(err, IsNil)
	client.Close()
}
