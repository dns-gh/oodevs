// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package server

import (
	. "launchpad.net/gocheck"
	"net/http"
	"os"
	"strconv"
	"testing"
	"util"
)

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
	_        = Suite(&TestSuite{GetTestPort()})
	NilUuid  = ""
	StatusOk = 0
)

func getcode(err error) int {
	if custom, ok := err.(*util.Error); ok {
		return int(custom.Code)
	}
	return 0
}

func addSession(c *Controller, uuid, name string) int {
	_, err := c.CreateSession(uuid, name)
	return getcode(err)
}

func deleteSession(c *Controller, uuid string) int {
	return getcode(c.DeleteSession(uuid))
}

func attachTimer(c *Controller, uuid, base string) int {
	_, err := c.AttachTimerService(uuid, "timer", base)
	return getcode(err)
}

func startSession(c *Controller, uuid string, offset int64) int {
	_, err := c.StartSession(uuid, offset)
	return getcode(err)
}

func stopSession(c *Controller, uuid string) int {
	_, err := c.StopSession(uuid)
	return getcode(err)
}

func (TestSuite) TestCrudSession(c *C) {
	uuid := "3E6DCB9A-222A-4606-A0FB-D5C8A28C9DBB"
	ctl := MakeController(util.MakeGocheckLogger(c))
	c.Assert(ctl.ListSessions(), HasLen, 0)
	err := addSession(ctl, NilUuid, "buzz")
	c.Assert(err, Equals, StatusOk)
	c.Assert(ctl.ListSessions(), HasLen, 1)
	err = addSession(ctl, uuid, "buzz2")
	c.Assert(err, Equals, StatusOk)
	c.Assert(ctl.ListSessions(), HasLen, 2)
	err = addSession(ctl, uuid, "buzz3")
	c.Assert(err, Equals, http.StatusConflict)
	c.Assert(ctl.ListSessions(), HasLen, 2)
	err = addSession(ctl, "invalid", "buzz4")
	c.Assert(err, Equals, http.StatusBadRequest)
	c.Assert(ctl.ListSessions(), HasLen, 2)
	err = deleteSession(ctl, uuid)
	c.Assert(err, Equals, StatusOk)
	c.Assert(ctl.ListSessions(), HasLen, 1)
	err = deleteSession(ctl, uuid)
	c.Assert(err, Equals, http.StatusNotFound)
	c.Assert(ctl.ListSessions(), HasLen, 1)
}

func (TestSuite) TestSessionStart(c *C) {
	uuid := "3E6DCB9A-222A-4606-A0FB-D5C8A28C9DBB"
	ctl := MakeController(util.MakeGocheckLogger(c))
	addSession(ctl, uuid, "buzz")
	err := attachTimer(ctl, uuid, "2006-01-02T15:04:05+07:00")
	c.Assert(err, Equals, StatusOk)
	err = startSession(ctl, "invalid", 0)
	c.Assert(err, Equals, http.StatusNotFound)
	err = startSession(ctl, uuid, 1000)
	c.Assert(err, Equals, StatusOk)
	err = stopSession(ctl, uuid)
	c.Assert(err, Equals, StatusOk)
}
