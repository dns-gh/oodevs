// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package services

import (
	"code.google.com/p/goprotobuf/proto"
	"fmt"
	. "launchpad.net/gocheck"
	"net/url"
	"os"
	"sdk"
	"strconv"
	"swapi"
	"swfake"
	"sword"
	"testing"
	"time"
	"util"
)

// gocheck boilerplate
func Test(t *testing.T) { TestingT(t) }

type TestSuite struct {
	port int
}

func GetTestPort() int {
	value := os.Getenv("TEST_PORT")
	port, err := strconv.Atoi(value)
	if len(value) == 0 || err != nil {
		return 60000
	}
	return port
}

var (
	_ = Suite(&TestSuite{GetTestPort()})
)

type Fixture struct {
	server *swfake.SwordServer
	client *Sword
	tick   func(tick time.Time)
}

func (Fixture) Abort(err error) {}

func (f *Fixture) Tick(tick time.Time) {
	if f.tick != nil {
		f.tick(tick)
	}
}

func (Fixture) OnApply(uuid string, err error, lock bool) {}

func (Fixture) UpdateEvent(uuid string, event *sdk.Event) (*sdk.Event, error) {
	return event, nil
}

func (f *Fixture) Close() {
	f.client.Stop()
	f.server.Close()
}

func (t *TestSuite) makeFixtureWith(c *C, f *Fixture) {
	local := fmt.Sprintf("localhost:%v", t.port)
	log := util.MakeGocheckLogger(c)
	server, err := swfake.NewSwordServer(log, local, true, false)
	c.Assert(err, IsNil)
	f.server = server
	f.client = NewSword(log, f, true, "sim", local)
}

func (t *TestSuite) makeFixture(c *C) *Fixture {
	f := &Fixture{}
	t.makeFixtureWith(c, f)
	return f
}

func waitConnectedCount(c *C, server *swfake.SwordServer, expected int) {
	for idx := 0; idx < 10; idx++ {
		actual := server.ConnectedCount()
		if actual == expected {
			return
		}
		time.Sleep(100 * time.Millisecond)
	}
	c.Fatal("timeout")
}

func (t *TestSuite) TestSwordStartStop(c *C) {
	f := t.makeFixture(c)
	defer f.Close()
	err := f.client.Start()
	c.Assert(err, IsNil)
	c.Assert(f.server.ConnectedCount(), Equals, 1)
	err = f.client.Stop()
	c.Assert(err, IsNil)
	waitConnectedCount(c, f.server, 0)
}

func getSomePayload(c *C) []byte {
	msg := sword.ClientToSim{
		Message: &sword.ClientToSim_Content{
			FragOrder: &sword.FragOrder{
				Tasker: swapi.MakeAutomatTasker(57),
				Type:   swapi.MakeId(354),
			},
		},
	}
	payload, err := proto.Marshal(&msg)
	c.Assert(err, IsNil)
	return payload
}

func (t *TestSuite) TestTwoConsecutiveOrders(c *C) {
	f := t.makeFixture(c)
	defer f.Close()
	err := f.client.Start()
	c.Assert(err, IsNil)
	url, err := url.Parse("sword://sim")
	c.Assert(err, IsNil)
	f.client.Apply("1", *url, getSomePayload(c))
	f.client.Apply("2", *url, getSomePayload(c))
	waitConnectedCount(c, f.server, 1)
}

func (t *TestSuite) TestSwordTimestamps(c *C) {
	quit := make(chan struct{})
	first := true
	f := Fixture{
		tick: func(tick time.Time) {
			c.Check(tick.Location(), Equals, time.UTC)
			if first {
				close(quit)
			}
			first = false
		},
	}
	t.makeFixtureWith(c, &f)
	defer f.Close()
	err := f.client.Start()
	c.Assert(err, IsNil)
	select {
	case <-quit: // we've seen one valid tick
	case <-time.After(2 * time.Second):
		c.Fatal("tick timeout")
	}
}
