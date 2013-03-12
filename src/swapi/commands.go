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
	"sword"
)

var (
	ErrInvalidLogin = errors.New("invalid login")
)

func (c *Client) Login(username, password string) error {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				AuthenticationRequest: &sword.AuthenticationRequest{
					Login:    proto.String(username),
					Password: proto.String(password),
					Version:  &sword.ProtocolVersion{Value: proto.String("5.0")},
				},
			},
		},
	}
	quit := make(chan error)
	handler := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.AuthenticationToClient == nil {
			return false
		}
		// we cannot check context as sword always set it to 0
		reply := msg.AuthenticationToClient.GetMessage().GetAuthenticationResponse()
		if reply == nil {
			return false
		}
		code := reply.GetErrorCode()
		if code == sword.AuthenticationResponse_success {
			quit <- nil
			return true
		}
		err = ErrInvalidLogin
		name, ok := sword.AuthenticationResponse_ErrorCode_name[int32(code)]
		if !ok {
			err = errors.New(name)
		}
		quit <- err
		return true
	}
	c.Post(msg, handler)
	return <-quit
}

type MissionParams struct {
	Params *sword.MissionParameters
}

func NewMissionParams() *MissionParams {
	return &MissionParams{
		Params: &sword.MissionParameters{
			Elem: []*sword.MissionParameter{},
		},
	}
}

func (p *MissionParams) addParam(param *sword.MissionParameter_Value) *MissionParams {
	holder := &sword.MissionParameter{
		NullValue: proto.Bool(false),
		Value: []*sword.MissionParameter_Value{
			param,
		},
	}
	p.Params.Elem = append(p.Params.Elem, holder)
	return p
}

func (p *MissionParams) AddAReal(value float32) *MissionParams {
	param := &sword.MissionParameter_Value{}
	param.AReal = proto.Float32(value)
	return p.addParam(param)
}

func (p *MissionParams) AddACharStr(value string) *MissionParams {
	param := &sword.MissionParameter_Value{}
	param.ACharStr = proto.String(value)
	return p.addParam(param)
}

func (c *Client) CreateFormation(partyId uint32, parentId uint32,
	name string, level int) (uint32, error) {
	tasker := &sword.Tasker{}
	taskerId := uint32(0)
	if parentId != 0 {
		tasker.Formation = &sword.FormationId{
			Id: proto.Uint32(parentId),
		}
		taskerId = parentId
	} else if partyId != 0 {
		tasker.Party = &sword.PartyId{
			Id: proto.Uint32(partyId),
		}
		taskerId = partyId
	}
	params := NewMissionParams()
	params.AddAReal(float32(level))
	params.AddACharStr(name)
	params.AddACharStr("")
	actionType := sword.UnitMagicAction_formation_creation
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     tasker,
					Type:       &actionType,
					Parameters: params.Params,
				},
			},
		},
	}
	state := 0
	formationId := uint32(0)
	receivedTaskerId := uint32(0)
	quit := make(chan error)
	handler := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.SimulationToClient == nil || msg.Context != context {
			return false
		}
		m := msg.SimulationToClient.GetMessage()
		if reply := m.GetMagicActionAck(); reply != nil {
			if state != 0 {
				quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
				return true
			}
			code := reply.GetErrorCode()
			if code != sword.MagicActionAck_no_error {
				err = errors.New("unknown error")
				name, ok := sword.MagicActionAck_ErrorCode_name[int32(code)]
				if ok {
					err = errors.New(name)
				}
				quit <- err
				return true
			}
			state = 1
			return false
		} else if reply := m.GetFormationCreation(); reply != nil {
			// FormationCreation feedback
			if state != 1 {
				quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
				return true
			}
			formationId = reply.GetFormation().GetId()
			state = 2
			return false
		} else if reply := m.GetUnitMagicActionAck(); reply != nil {
			// Wait for the final UnitMagicActionAck
			code := reply.GetErrorCode()
			if state == 0 {
				if code == sword.UnitActionAck_no_error {
					quit <- errors.New(fmt.Sprintf(
						"Got unexpected success %v", m))
				} else {
					err = errors.New("unknown error")
					name, ok := sword.UnitActionAck_ErrorCode_name[int32(code)]
					if ok {
						err = errors.New(name)
					}
					quit <- err
				}
			} else if state == 2 {
				if code != sword.UnitActionAck_no_error {
					quit <- errors.New(fmt.Sprintf(
						"Got unexpected failure %v", m))
				} else {
					receivedTaskerId = reply.GetUnit().GetId()
					quit <- nil
				}
			} else {
				quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
			}
			return true
		} else {
			quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
		}
		return true
	}
	c.Post(msg, handler)
	err := <-quit
	if err == nil {
		if formationId == 0 {
			err = errors.New("invalid formation identifier: 0")
		} else if receivedTaskerId == 0 {
			err = errors.New("invalid tasker identifier: 0")
		} else if taskerId != receivedTaskerId {
			err = errors.New(fmt.Sprintf(
				"tasker identifier mismatch: %v != %v", taskerId, receivedTaskerId))
		}
	}
	return formationId, err
}
