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
	name string, level int, logLevel string) (*Formation, error) {
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
	var created *Formation
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
			// Ignore this message, UnitMagicActionAck should be enough
			return false
		} else if reply := m.GetFormationCreation(); reply != nil {
			// Ignore this message, its context should not be set anyway
			return false
		} else if reply := m.GetUnitMagicActionAck(); reply != nil {
			// Wait for the final UnitMagicActionAck
			id, ret := GetUnitMagicActionAck(reply)
			err = ret
			if err == nil {
				value := GetParameterValue(reply.GetResult(), 0)
				if value != nil {
					receivedTaskerId = id
					created = c.Model.GetFormation(value.GetFormation().GetId())
				} else {
					err = errors.New(fmt.Sprintf("Invalid result: %v",
						reply.GetResult()))
				}
			}
		} else {
			err = errors.New(fmt.Sprintf("Got unexpected %v", m))
		}
		quit <- err
		return true
	}
	c.Post(msg, handler)
	err := <-quit
	if err == nil {
		if receivedTaskerId == 0 {
			err = errors.New("invalid tasker identifier: 0")
		} else if taskerId != receivedTaskerId {
			err = errors.New(fmt.Sprintf(
				"tasker identifier mismatch: %v != %v", taskerId, receivedTaskerId))
		}
	}
	return created, err
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
