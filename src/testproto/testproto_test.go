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
	. "launchpad.net/gocheck"
	"testing"
)

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
	c.Assert(msg.EnumNewValue1Msg.GetValue(), IsNil)
}
