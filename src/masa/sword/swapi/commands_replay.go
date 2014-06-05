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
	"errors"
	"fmt"
	"masa/sword/sword"
)

type replayHandler func(msg *sword.ReplayToClient_Content) error

func (c *Client) postReplayWithClientId(msg *sword.ClientToReplay_Content,
	checkClientId bool, handler replayHandler) <-chan error {

	quit := make(chan error, 1)
	wrapper := func(msg *SwordMessage, client, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.ReplayToClient == nil ||
			msg.ReplayToClient.GetMessage() == nil ||
			(checkClientId && (msg.ClientId != client || msg.ClientId == 0)) ||
			(checkClientId && msg.Context != context) {
			return false
		}
		errMsg := msg.ReplayToClient.GetErrorMsg()
		if len(errMsg) > 0 {
			err = errors.New(errMsg)
		} else {
			err = handler(msg.ReplayToClient.GetMessage())
			if err == ErrContinue {
				return false
			}
		}
		quit <- err
		return true
	}
	m := SwordMessage{
		ClientToReplay: &sword.ClientToReplay{
			Message: msg,
		},
	}
	c.Post(m, wrapper)
	return quit
}

func (c *Client) postReplay(msg *sword.ClientToReplay_Content, handler replayHandler) <-chan error {
	return c.postReplayWithClientId(msg, true, handler)
}

func (c *Client) SkipToTick(tick int32) error {
	msg := &sword.ClientToReplay_Content{
		ControlSkipToTick: &sword.ControlSkipToTick{
			Tick: proto.Int32(tick),
		},
	}
	return <-c.postReplayWithClientId(msg, false,
		func(msg *sword.ReplayToClient_Content) error {
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

func (c *Client) GetTimetable(beginTick, endTick int32, broadcast bool) (
	[]sword.TimeTable_TimeMapping, error) {

	msg := &sword.ClientToReplay_Content{
		TimeTableRequest: &sword.TimeTableRequest{
			TickRange: &sword.TimeTableRequest_TimeRange{
				BeginTick: proto.Int32(beginTick),
				EndTick:   proto.Int32(endTick),
			},
			Broadcast: proto.Bool(broadcast),
		},
	}
	table := []sword.TimeTable_TimeMapping{}
	err := <-c.postReplay(msg, func(msg *sword.ReplayToClient_Content) error {
		ack := msg.TimeTableRequestAck
		if ack == nil {
			return unexpected(msg)
		}
		code := ack.GetErrorCode()
		if code != sword.TimeTableRequestAck_no_error {
			return makeError(ack, int32(code), sword.TimeTableRequestAck_ErrorCode_name)
		}
		if ack.TimeTable == nil {
			return fmt.Errorf("received nil time table in TimeTableRequestAck")
		}
		DeepCopy(&table, ack.TimeTable.TimeTableItem)
		return nil
	})
	return table, err
}

func (c *Client) ReplayGetLogisticHistory(requestIds ...uint32) ([]*sword.LogHistoryEntry, error) {
	msg := &sword.ClientToReplay_Content{
		LogisticHistoryRequest: makeLogisticHistoryRequest(requestIds...),
	}
	var entries []*sword.LogHistoryEntry
	handler := func(msg *sword.ReplayToClient_Content) error {
		reply := msg.GetLogisticHistoryAck()
		if reply == nil {
			return ErrContinue
		}
		entries = handleLogisticHistoryAck(reply)
		return nil
	}
	err := <-c.postReplay(msg, handler)
	return entries, err
}

func (c *Client) ReplayListLogisticRequests(currentTick, maxCount int, entities ...uint32) (
	[]*sword.LogHistoryEntry, error) {

	msg := &sword.ClientToReplay_Content{
		ListLogisticRequests: makeListLogisticRequests(currentTick, maxCount,
			entities...),
	}
	var entries []*sword.LogHistoryEntry
	handler := func(msg *sword.ReplayToClient_Content) error {
		reply := msg.GetListLogisticRequestsAck()
		if reply == nil {
			return ErrContinue
		}
		entries = handleListLogisticRequestsAck(reply)
		return nil
	}
	err := <-c.postReplay(msg, handler)
	return entries, err
}

func (c *Client) ReplayStop() error {
	msg := &sword.ClientToReplay_Content{
		ControlStop: &sword.ControlStop{},
	}
	handler := func(msg *sword.ReplayToClient_Content) error {
		reply := msg.GetControlStopAck()
		if reply == nil {
			return unexpected(msg)
		}
		return getControlAckError(reply)
	}
	return <-c.postReplay(msg, handler)
}
