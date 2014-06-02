// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package swapi

import (
	"code.google.com/p/goprotobuf/proto"
	"masa/sword/sword"
)

type messengerHandler func(msg *sword.MessengerToClient_Content) error

func (c *Client) postMessengerMsg(msg SwordMessage, handler messengerHandler) <-chan error {
	quit := make(chan error, 1)
	wrapper := func(msg *SwordMessage, client, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.MessengerToClient == nil ||
			msg.MessengerToClient.GetMessage() == nil {
			return false
		}
		err = handler(msg.MessengerToClient.GetMessage())
		if err == ErrContinue {
			return false
		}
		quit <- err
		return true
	}
	c.Post(msg, wrapper)
	return quit
}

func CreateTacticalLine(name string, geometry *sword.Location, diffusion *sword.Diffusion) *sword.TacticalLine {
	return &sword.TacticalLine{
		Name:      proto.String(name),
		Geometry:  geometry,
		Diffusion: diffusion,
	}
}

func (c *Client) CreateLimit(name string, unitId uint32, points ...Point) (*TacticalLine, error) {
	msg := SwordMessage{
		ClientToMessenger: &sword.ClientToMessenger{
			Message: &sword.ClientToMessenger_Content{
				LimitCreationRequest: &sword.LimitCreationRequest{
					TacticalLine: CreateTacticalLine(name,
						MakeLineLocation(points...),
						&sword.Diffusion{
							Unit: MakeId(unitId),
						},
					),
				},
			},
		},
	}
	var id uint32
	handler := func(msg *sword.MessengerToClient_Content) error {
		reply := msg.GetLimitCreationRequestAck()
		if reply == nil {
			return ErrContinue
		}
		code := reply.GetErrorCode()
		if code != sword.TacticalLineAck_no_error {
			return makeError(reply, int32(code),
				sword.TacticalLineAck_ErrorCode_name)
		}
		id = reply.GetId()
		return nil
	}
	err := <-c.postMessengerMsg(msg, handler)
	if err != nil {
		return nil, err
	}
	var result *TacticalLine
	ok := c.Model.WaitCondition(func(data *ModelData) bool {
		line := data.TacticalLines[id]
		if line == nil {
			return false
		}
		result = &TacticalLine{}
		DeepCopy(result, line)
		return true
	})
	if !ok {
		return nil, ErrNotFound
	}
	return result, err
}

func (c *Client) CreatePhaseLine(name string, unitId uint32, points ...Point) (*TacticalLine, error) {
	msg := SwordMessage{
		ClientToMessenger: &sword.ClientToMessenger{
			Message: &sword.ClientToMessenger_Content{
				PhaseLineCreationRequest: &sword.PhaseLineCreationRequest{
					TacticalLine: CreateTacticalLine(name,
						MakeLineLocation(points...),
						&sword.Diffusion{
							Unit: MakeId(unitId),
						},
					),
				},
			},
		},
	}
	var id uint32
	handler := func(msg *sword.MessengerToClient_Content) error {
		reply := msg.GetPhaseLineCreationRequestAck()
		if reply == nil {
			return ErrContinue
		}
		code := reply.GetErrorCode()
		if code != sword.TacticalLineAck_no_error {
			return makeError(reply, int32(code),
				sword.TacticalLineAck_ErrorCode_name)
		}
		id = reply.GetId()
		return nil
	}
	err := <-c.postMessengerMsg(msg, handler)
	if err != nil {
		return nil, err
	}
	var result *TacticalLine
	ok := c.Model.WaitCondition(func(data *ModelData) bool {
		line := data.TacticalLines[id]
		if line == nil {
			return false
		}
		result = &TacticalLine{}
		DeepCopy(result, line)
		return true
	})
	if !ok {
		return nil, ErrNotFound
	}
	return result, err
}

func (c *Client) UpdateLimit(limit *TacticalLine) error {
	msg := SwordMessage{
		ClientToMessenger: &sword.ClientToMessenger{
			Message: &sword.ClientToMessenger_Content{
				LimitUpdateRequest: &sword.LimitUpdateRequest{
					Id:           MakeId(limit.Id),
					TacticalLine: CreateTacticalLine(limit.Name, &limit.Location, &limit.Diffusion),
				},
			},
		},
	}
	handler := func(msg *sword.MessengerToClient_Content) error {
		reply := msg.GetLimitUpdateRequestAck()
		if reply == nil {
			return ErrContinue
		}
		code := reply.GetErrorCode()
		if code != sword.TacticalLineAck_no_error {
			return makeError(reply, int32(code),
				sword.TacticalLineAck_ErrorCode_name)
		}
		if reply.GetId() != limit.Id {
			return ErrContinue
		}
		return nil
	}
	return <-c.postMessengerMsg(msg, handler)
}

func (c *Client) UpdatePhaseLine(limit *TacticalLine) error {
	msg := SwordMessage{
		ClientToMessenger: &sword.ClientToMessenger{
			Message: &sword.ClientToMessenger_Content{
				PhaseLineUpdateRequest: &sword.PhaseLineUpdateRequest{
					Id:           MakeId(limit.Id),
					TacticalLine: CreateTacticalLine(limit.Name, &limit.Location, &limit.Diffusion),
				},
			},
		},
	}
	handler := func(msg *sword.MessengerToClient_Content) error {
		reply := msg.GetPhaseLineUpdateRequestAck()
		if reply == nil {
			return ErrContinue
		}
		code := reply.GetErrorCode()
		if code != sword.TacticalLineAck_no_error {
			return makeError(reply, int32(code),
				sword.TacticalLineAck_ErrorCode_name)
		}
		if reply.GetId() != limit.Id {
			return ErrContinue
		}
		return nil
	}
	return <-c.postMessengerMsg(msg, handler)
}

func (c *Client) DeleteLimit(limitId uint32) error {
	msg := SwordMessage{
		ClientToMessenger: &sword.ClientToMessenger{
			Message: &sword.ClientToMessenger_Content{
				LimitDestructionRequest: &sword.LimitDestructionRequest{
					Id: MakeId(limitId),
				},
			},
		},
	}
	handler := func(msg *sword.MessengerToClient_Content) error {
		reply := msg.GetLimitDestructionRequestAck()
		if reply == nil {
			return ErrContinue
		}
		code := reply.GetErrorCode()
		if code != sword.TacticalLineAck_no_error {
			return makeError(reply, int32(code),
				sword.TacticalLineAck_ErrorCode_name)
		}
		if reply.GetId() != limitId {
			return ErrContinue
		}
		return nil
	}
	return <-c.postMessengerMsg(msg, handler)
}

func (c *Client) DeletePhaseLine(limitId uint32) error {
	msg := SwordMessage{
		ClientToMessenger: &sword.ClientToMessenger{
			Message: &sword.ClientToMessenger_Content{
				PhaseLineDestructionRequest: &sword.PhaseLineDestructionRequest{
					Id: MakeId(limitId),
				},
			},
		},
	}
	handler := func(msg *sword.MessengerToClient_Content) error {
		reply := msg.GetPhaseLineDestructionRequestAck()
		if reply == nil {
			return ErrContinue
		}
		code := reply.GetErrorCode()
		if code != sword.TacticalLineAck_no_error {
			return makeError(reply, int32(code),
				sword.TacticalLineAck_ErrorCode_name)
		}
		if reply.GetId() != limitId {
			return ErrContinue
		}
		return nil
	}
	return <-c.postMessengerMsg(msg, handler)
}
