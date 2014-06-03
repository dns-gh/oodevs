// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package testproto

import (
	"code.google.com/p/goprotobuf/proto"
	"flag"
	. "launchpad.net/gocheck"
	"masa/testproto/after"
	"masa/testproto/before"
	"testing"
)

// Command line options are not used but left here so gosword tests can
// be run with the same arguments from ant. Without these, go test whines input
// options are unsupported instead of ignoring them.
var (
	application  string
	rootdir      string
	rundir       string
	exercisesDir string
	testPort     int
	showlog      bool
)

func init() {
	flag.StringVar(&application, "application", "",
		"path to simulation_app executable")
	flag.StringVar(&rootdir, "root-dir", "",
		"path to simulation root directory")
	flag.StringVar(&rundir, "run-dir", "",
		"path application run directory, default to application directory")
	flag.StringVar(&exercisesDir, "exercises-dir", "",
		"exercises directory, default to root-dir/exercises")
	flag.IntVar(&testPort, "test-port", 35000,
		"base port for spawned simulations")
	flag.BoolVar(&showlog, "show-log", false,
		"show log")
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

// Serialize input, deserialize as Root1 and return any error.
func RoundTrip(c *C, input *after.Root) (*before.Root, error) {
	data, err := proto.Marshal(input)
	c.Assert(err, IsNil)
	output := &before.Root{}
	err = proto.Unmarshal(data, output)
	return output, err
}

func (s *TestSuite) TestNewEnumValue(c *C) {
	// Pass an existing enumeration value
	_, err := RoundTrip(c, &after.Root{
		EnumNewValueMsg: &after.EnumNewValueMsg{
			Value: after.EnumNewValue_value0.Enum(),
		},
	})
	c.Assert(err, IsNil)

	// Pass a missing enumeration value. Strangely, Go successfully deserializes
	// the message.
	msg, err := RoundTrip(c, &after.Root{
		EnumNewValueMsg: &after.EnumNewValueMsg{
			Value: after.EnumNewValue_value1.Enum(),
		},
	})
	c.Assert(err, IsNil)
	c.Assert(msg.EnumNewValueMsg.GetValue(), NotNil)
}

func (s *TestSuite) TestOptionalNewEnumValue(c *C) {
	// Pass an existing enumeration value
	msg, err := RoundTrip(c, &after.Root{
		EnumNewValueMsg: &after.EnumNewValueMsg{
			Value:    after.EnumNewValue_value0.Enum(),
			Optional: after.EnumNewValue_value0.Enum(),
		},
	})
	c.Assert(err, IsNil)
	c.Assert(msg.EnumNewValueMsg.GetValue(), Equals, before.EnumNewValue(0))

	// Pass a missing enumeration value. Strangely, Go successfully deserializes
	// the message.
	msg, err = RoundTrip(c, &after.Root{
		EnumNewValueMsg: &after.EnumNewValueMsg{
			Value:    after.EnumNewValue_value0.Enum(),
			Optional: after.EnumNewValue_value1.Enum(),
		},
	})
	c.Assert(err, IsNil)
	c.Assert(msg.EnumNewValueMsg.GetValue(), Equals, before.EnumNewValue(0))
}
