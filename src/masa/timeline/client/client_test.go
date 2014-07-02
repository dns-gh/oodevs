// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package client

import (
	"bytes"
	"code.google.com/p/goprotobuf/proto"
	. "launchpad.net/gocheck"
	"masa/sword/swtest"
	"masa/timeline/sdk"
	"testing"
)

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

const (
	text = `
[
    {
        "type": "SESSION_CREATE",
        "session": {
            "create": {
                "name": "buzz"
            }
        }
    },
    {
        "type": "SESSION_START",
        "session": {
            "start": {
                "uuid": "some_uuid"
            }
        }
    }
]
`
)

func (TestSuite) TestLoad(c *C) {
	ref := Commands{
		sdk.ClientRequest{
			Type: sdk.Type_SESSION_CREATE.Enum(),
			Session: &sdk.SessionRequests{
				Create: &sdk.SessionCreate{
					Name: proto.String("buzz"),
				},
			},
		},
		sdk.ClientRequest{
			Type: sdk.Type_SESSION_START.Enum(),
			Session: &sdk.SessionRequests{
				Start: &sdk.SessionStart{
					Uuid: proto.String("some_uuid"),
				},
			},
		},
	}
	cmds, err := ParseReader(bytes.NewBufferString(text))
	c.Assert(err, IsNil)
	swtest.DeepEquals(c, ref, cmds)
}
