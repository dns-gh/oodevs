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
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
	. "launchpad.net/gocheck"
	"sdk"
)

func (TestSuite) TestExpectedJsonSpecs(c *C) {
	text := `
{
    "tag": "update_events",
    "tick": "tick",
    "events": [
        {
            "uuid": "a"
        },
        {
            "uuid": "b"
        }
    ],
    "uuids": [
        "a",
        "b"
    ]
}`
	dst := sdk.Message{}
	err := json.Unmarshal([]byte(text), &dst)
	c.Assert(err, IsNil)
	expected := sdk.Message{
		Tag:  sdk.MessageTag_update_events.Enum(),
		Tick: proto.String("tick"),
		Events: []*sdk.Event{
			&sdk.Event{Uuid: proto.String("a")},
			&sdk.Event{Uuid: proto.String("b")},
		},
		Uuids: []string{"a", "b"},
	}
	c.Assert(dst, DeepEquals, expected)
}
