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
	"testing"
)

// Command line options are not used but left here so gosword tests can
// be run with the same arguments from ant. Without these, go test whines input
// options are unsupported instead of ignoring them.
var (
	application string
	rootdir     string
	rundir      string
	testPort    int
	showlog     bool
)

func init() {
	flag.StringVar(&application, "application", "",
		"path to simulation_app executable")
	flag.StringVar(&rootdir, "root-dir", "",
		"path to simulation root directory")
	flag.StringVar(&rundir, "run-dir", "",
		"path application run directory, default to application directory")
	flag.IntVar(&testPort, "test-port", 35000,
		"base port for spawned simulations")
	flag.BoolVar(&showlog, "show-log", false,
		"show log")
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

// Serialize input, deserialize as Root1 and return any error.
func RoundTrip(c *C, input *Root2) (*Root1, error) {
	data, err := proto.Marshal(input)
	c.Assert(err, IsNil)
	root1 := &Root1{}
	err = proto.Unmarshal(data, root1)
	return root1, err
}

func (s *TestSuite) TestNewEnumValue(c *C) {
	// Pass an existing enumeration value
	_, err := RoundTrip(c, &Root2{
		EnumNewValue2Msg: &EnumNewValue2Msg{
			Value: EnumNewValue2_value20.Enum(),
		},
	})
	c.Assert(err, IsNil)

	// Pass a missing enumeration value. Strangely, Go successfully deserializes
	// the message.
	msg, err := RoundTrip(c, &Root2{
		EnumNewValue2Msg: &EnumNewValue2Msg{
			Value: EnumNewValue2_value21.Enum(),
		},
	})
	c.Assert(err, IsNil)
	c.Assert(msg.EnumNewValue1Msg.GetValue(), NotNil)
}
