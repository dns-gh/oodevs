// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swapi

import (
	"code.google.com/p/goprotobuf/proto"
	"sword"
)

type replayHandler func(msg *sword.ReplayToClient_Content) error

func (c *Client) postReplayRequest(msg SwordMessage, handler replayHandler) <-chan error {

	quit := make(chan error, 1)
	wrapper := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.ReplayToClient == nil ||
			msg.ReplayToClient.GetMessage() == nil {
			return false
		}
		err = handler(msg.ReplayToClient.GetMessage())
		if err == ErrContinue {
			return false
		}
		quit <- err
		return true
	}
	c.Post(msg, wrapper)
	return quit
}

func (c *Client) SkipToTick(tick int32) error {
	msg := SwordMessage{
		ClientToReplay: &sword.ClientToReplay{
			Message: &sword.ClientToReplay_Content{
				ControlSkipToTick: &sword.ControlSkipToTick{
					Tick: proto.Int32(tick),
				},
			},
		},
	}
	return <-c.postReplayRequest(msg, func(msg *sword.ReplayToClient_Content) error {
		if reply := msg.ControlSkipToTickAck; reply != nil {
			code := reply.GetErrorCode()
			if code != sword.ControlAck_no_error {
				return makeError(msg, int32(code), sword.ControlAck_ErrorCode_name)
			}
		} else if reply := msg.ControlReplayInformation; reply != nil {
			if *reply.CurrentTick > tick {
				return nil
			}
		}
		return ErrContinue
	})
}
