package simtests

import (
	"code.google.com/p/goprotobuf/proto"
	. "launchpad.net/gocheck"
	"swapi"
	"sword"
	"time"
)

func (s *TestSuite) TestPostTimeout(c *C) {
	sim := startSimOnExercise(c, ExCrossroadSmallEmpty, 1000, false)
	defer sim.Kill()
	client := ConnectClient(c, sim)
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
	quit := make(chan error)
	client.Post(msg, func(msg *swapi.SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		return false
	})
	go func() {
		time.Sleep(2 * client.PostTimeout)
		quit <- nil
	}()
	err = <-quit
	c.Assert(err, ErrorMatches, "(?i).*timeout.*")
}
