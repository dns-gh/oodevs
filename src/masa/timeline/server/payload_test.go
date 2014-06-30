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
	"masa/sword/sword"
	"masa/sword/swtest"
	"masa/timeline/sdk"
)

const (
	event = `
{
    "uuid": "3EB69DEF-5B70-4907-ABE0-7169DC96540C",
    "name": "pause sword",
    "info": "this command will pause sword after 30s",
    "begin": "2011-01-01T12:00:00Z",
    "action": {
        "target": "sword://sim",
        "apply": true,
        "payload": {
            "message": {
                "control_pause": {}
            },
            "context": 4450460
        }
    }
}
`
	request = `
{
    "type": "EVENT_CREATE",
    "event": {
        "create": {
            "event": ` + event + `
        }
    }
}
`
)

var (
	message = sword.ClientToSim{
		Context: proto.Int32(4450460),
		Message: &sword.ClientToSim_Content{
			ControlPause: &sword.ControlPause{},
		},
	}
)

func jsoncompare(c *C, a, b []byte) {
	var rawa, rawb interface{}
	err := jsondecode(a, &rawa)
	c.Assert(err, IsNil)
	err = jsondecode(b, &rawb)
	c.Assert(err, IsNil)
	swtest.AssertEqualOrDiff(c, rawa, rawb)
}

func testcycle(c *C, text []byte, dst, ref interface{}) {
	data, err := FixJsonPayloads(text, Encode)
	c.Assert(err, IsNil)
	err = jsondecode(data, dst)
	c.Assert(err, IsNil)
	c.Assert(ref, DeepEquals, dst)
	data, err = json.Marshal(dst)
	c.Assert(err, IsNil)
	data, err = FixJsonPayloads(data, Decode)
	c.Assert(err, IsNil)
	jsoncompare(c, data, text)
}

func makeevent(c *C) *sdk.Event {
	payload, err := proto.Marshal(&message)
	c.Assert(err, IsNil)
	return &sdk.Event{
		Uuid:  proto.String("3EB69DEF-5B70-4907-ABE0-7169DC96540C"),
		Name:  proto.String("pause sword"),
		Info:  proto.String("this command will pause sword after 30s"),
		Begin: proto.String("2011-01-01T12:00:00Z"),
		Action: &sdk.Action{
			Target:  proto.String("sword://sim"),
			Apply:   proto.Bool(true),
			Payload: payload,
		},
	}
}

func (TestSuite) TestFixRequest(c *C) {
	ref := sdk.ClientRequest{
		Type: sdk.Type_EVENT_CREATE.Enum(),
		Event: &sdk.EventRequests{
			Create: &sdk.EventCreate{
				Event: makeevent(c),
			},
		},
	}
	testcycle(c, []byte(request), &sdk.ClientRequest{}, &ref)
}

const (
	reply = `
{
    "type": "EVENT_CREATE",
    "event": {
        "create": {
            "event": ` + event + `
        }
    }
}
`
)

func (TestSuite) TestDecodeEvent(c *C) {
	filter := NewSwordFilter()
	for i := 0; i < 4; i++ {
		raw, err := filter.DecodeEvent(makeevent(c))
		c.Assert(err, IsNil)
		data, err := json.Marshal(raw)
		c.Assert(err, IsNil)
		jsoncompare(c, []byte(event), data)
	}
}

func (TestSuite) TestEncodeEvent(c *C) {
	filter := NewSwordFilter()
	evt := makeevent(c)
	for i := 0; i < 4; i++ {
		raw := map[string]interface{}{}
		err := jsondecode([]byte(event), &raw)
		c.Assert(err, IsNil)
		data, err := filter.EncodeEvent(raw)
		c.Assert(err, IsNil)
		c.Assert(data, DeepEquals, evt)
		events, err := filter.EncodeEvents([]byte("[" + event + "," + event + "," + event + "]"))
		c.Assert(err, IsNil)
		c.Assert(events, DeepEquals, []*sdk.Event{evt, evt, evt})
	}
}
