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

func (c *Client) LoginWithVersion(username, password, version string) error {
	msg := SwordMessage{
		ClientToAuthentication: &sword.ClientToAuthentication{
			Message: &sword.ClientToAuthentication_Content{
				AuthenticationRequest: &sword.AuthenticationRequest{
					Login:    proto.String(username),
					Password: proto.String(password),
					Version: &sword.ProtocolVersion{
						Value: proto.String(version),
					},
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
		if ok {
			err = errors.New(name)
		}
		quit <- err
		return true
	}
	c.Post(msg, handler)
	err := <-quit
	return err
}

func (c *Client) Login(username, password string) error {
	return c.LoginWithVersion(username, password, "5.0")
}

func GetUnitMagicActionAck(msg *sword.UnitMagicActionAck) (uint32, error) {
	// Wait for the final UnitMagicActionAck
	code := msg.GetErrorCode()
	if code == sword.UnitActionAck_no_error {
		id := msg.GetUnit().GetId()
		return id, nil
	}
	err := errors.New("unknown error")
	name, ok := sword.UnitActionAck_ErrorCode_name[int32(code)]
	if ok {
		err = errors.New(name)
	}
	return 0, err
}

func (c *Client) CreateFormation(partyId uint32, parentId uint32,
	name string, level int, logLevel string) (uint32, error) {
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
	actionType := sword.UnitMagicAction_formation_creation
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker: tasker,
					Type:   &actionType,
					Parameters: MakeParameters(
						MakeFloat(float32(level)),
						MakeString(name),
						MakeString(logLevel),
					),
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
			id, err := GetUnitMagicActionAck(reply)
			receivedTaskerId = id
			if state == 0 {
				if err == nil {
					err = errors.New(fmt.Sprintf("Got unexpected success %v", m))
				}
				quit <- err
			} else if state == 2 {
				if err != nil {
					quit <- errors.New(fmt.Sprintf(
						"Got unexpected failure %v", m))
				} else {
					quit <- nil
				}
			} else {
				quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
			}
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

func (c *Client) DeleteUnit(unitId uint32) error {
	tasker := &sword.Tasker{
		Unit: &sword.UnitId{
			Id: proto.Uint32(unitId),
		},
	}
	actionType := sword.UnitMagicAction_delete_unit
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     tasker,
					Type:       &actionType,
					Parameters: MakeParameters(),
				},
			},
		},
	}
	state := 0
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
		if reply := m.GetUnitDestruction(); reply != nil {
			if state != 0 {
				quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
				return true
			}
			state = 1
			return false
		} else if reply := m.GetUnitMagicActionAck(); reply != nil {
			id, err := GetUnitMagicActionAck(reply)
			if state == 0 {
				if err == nil {
					err = errors.New(fmt.Sprintf("Got unexpected success %v", m))
				}
			} else if state == 1 {
				if err != nil {
					err = errors.New(fmt.Sprintf("Got unexpected failure %v", m))
				} else {
					if id != unitId {
						err = errors.New(fmt.Sprintf(
							"Deleted unit identifier mismatch: %v != %v", unitId, id))
					}
				}
			} else {
				err = errors.New(fmt.Sprintf("Got unexpected %v", m))
			}
			quit <- err
		} else {
			quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
		}
		return true
	}
	c.Post(msg, handler)
	err := <-quit
	return err
}

func getControlAckError(code sword.ControlAck_ErrorCode) error {
	if code == sword.ControlAck_no_error {
		return nil
	}
	err := errors.New("unknown error")
	name, ok := sword.ControlAck_ErrorCode_name[int32(code)]
	if ok {
		err = errors.New(name)
	}
	return err
}

func (c *Client) Pause() error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlPause: &sword.ControlPause{},
			},
		},
	}
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
		if reply := m.GetControlPauseAck(); reply != nil {
			quit <- getControlAckError(reply.GetErrorCode())
		} else {
			quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
		}
		return true
	}
	c.Post(msg, handler)
	err := <-quit
	return err
}

func (c *Client) Resume(nextPause uint32) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlResume: &sword.ControlResume{
					Tick: proto.Uint32(nextPause),
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
		if msg.SimulationToClient == nil || msg.Context != context {
			return false
		}
		m := msg.SimulationToClient.GetMessage()
		if reply := m.GetControlResumeAck(); reply != nil {
			quit <- getControlAckError(reply.GetErrorCode())
		} else {
			quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
		}
		return true
	}
	c.Post(msg, handler)
	err := <-quit
	return err
}

func (c *Client) Stop() error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlStop: &sword.ControlStop{},
			},
		},
	}
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
		if reply := m.GetControlStopAck(); reply != nil {
			quit <- getControlAckError(reply.GetErrorCode())
		} else {
			quit <- errors.New(fmt.Sprintf("Got unexpected %v", m))
		}
		return true
	}
	c.Post(msg, handler)
	err := <-quit
	return err
}
