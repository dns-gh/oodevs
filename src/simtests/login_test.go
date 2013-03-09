package simtests

import (
	"swapi"
	"swapi/simu"
	"testing"
)

func startSimOnExercise(t *testing.T, exercise string, endTick int,
	paused bool) *simu.SimProcess {

	opts := MakeOpts()
	opts.ExerciseName = exercise

	session := simu.CreateDefaultSession()
	session.Paused = paused
	WriteSession(t, opts, session)
	sim, err := simu.StartSim(opts)
	if err != nil {
		t.Fatalf("failed to start the simulation: %v", err)
	}
	return sim
}

func ConnectClient(t *testing.T, sim *simu.SimProcess) *swapi.Client {
	c, err := swapi.Connect(sim.DispatcherAddr)
	if err != nil {
		t.Fatalf("failed connect to simulation: %v", err)
	}
	return c
}

func TestLogin(t *testing.T) {
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

	sim := startSimOnExercise(t, "tests/crossroad-small-empty", 1000, false)
	defer sim.Kill()

	// Test invalid login
	client := ConnectClient(t, sim)
	// Also check returned context is valid
	addContextChecker(client)
	err := client.Login("foo", "bar")
	if err == nil {
		t.Fatal("login with invalid credentials should have failed")
	}
	client.Close()

	// Test valid login
	client = ConnectClient(t, sim)
	addContextChecker(client)
	err = client.Login("admin", "")
	if err != nil {
		t.Fatalf("login failed: %v", err)
	}
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

func TestNoDataSentUntilSuccessfulLogin(t *testing.T) {
	sim := startSimOnExercise(t, "tests/crossroad-small-empty", 1000, false)
	defer sim.Kill()

	// Connect and watch incoming messages
	client := ConnectClient(t, sim)
	seen := waitForMessages(client, 5*time.Second)
	if seen {
        /* SWBUG-10026

		   t.Fatal("messages seen before any client action")
		*/
	}
	err := client.Login("foo", "bar")
	if err == nil {
		t.Fatal("login should have failed")
	}
	seen = waitForMessages(client, 5*time.Second)
	if seen {
        /* SWBUG-10026

		   t.Fatal("message seen after invalid login")
		*/
	}
}
