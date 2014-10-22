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
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/sword/swtest"
	"masa/timeline/sdk"
	"masa/timeline/swfake"
	"masa/timeline/util"
	"net/url"
	"os"
	"strconv"
	"testing"
	"time"
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

func WaitConnected(server *swfake.SwordServer) {
	for {
		detached := server.GetLinks()
		done := len(detached) > 0
		detached.Close()
		if done {
			return
		}
	}
}

type Fixture struct {
	server *swfake.SwordServer
	client *Sword
	tick   func(tick time.Time)
	eloop  *util.EventLoop
}

func (Fixture) Abort(err error)                              {}
func (Fixture) CloseEvent(uuid string, err error, lock bool) {}
func (Fixture) UpdateEvent(uuid string, event *sdk.Event)    {}
func (Fixture) InvalidateFilters()                           {}
func (Fixture) UpdateServices()                              {}

func (f *Fixture) Tick(tick time.Time) {
	if f.tick != nil {
		f.Post(func() { f.tick(tick) })
	}
}

func (f *Fixture) Post(operand func()) {
	f.eloop.Post(operand)
}

func (f *Fixture) Close() {
	f.eloop.Post(func() { f.client.Stop() })
	f.server.Close()
	f.eloop.Close()
}

func (t *TestSuite) makeFixtureWith(c *C, f *Fixture) {
	local := fmt.Sprintf("localhost:%v", t.port)
	log := swtest.MakeGocheckLogger(c)
	server, err := swfake.NewSwordServer(log, local, true, false)
	c.Assert(err, IsNil)
	f.server = server
	f.eloop = util.NewEventLoop()
	f.eloop.Start()
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
	f.eloop.Post(func() {
		err := f.client.Start()
		c.Check(err, IsNil)
	})
	WaitConnected(f.server)
	f.eloop.Post(func() {
		err := f.client.Stop()
		c.Check(err, IsNil)
	})
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
	f.eloop.Post(func() {
		err := f.client.Start()
		c.Check(err, IsNil)
		url, err := url.Parse("sword://sim")
		c.Check(err, IsNil)
		f.client.Apply("1", *url, getSomePayload(c))
		f.client.Apply("2", *url, getSomePayload(c))
	})
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
	f.eloop.Post(func() {
		err := f.client.Start()
		c.Check(err, IsNil)
	})
	select {
	case <-quit: // we've seen one valid tick
	case <-time.After(2 * time.Second):
		c.Fatal("tick timeout")
	}
}
