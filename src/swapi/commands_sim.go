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
	ErrContinue = errors.New("continue")
	ErrNotFound = errors.New("not found")
)

func mismatch(name string, a, b interface{}) error {
	return fmt.Errorf("%v mismatch %v != %v", name, a, b)
}

func invalid(name string, value interface{}) error {
	return fmt.Errorf("invalid %v: %v", name, value)
}

func makeTasker(automat, crowd, formation, party, unit uint32) *sword.Tasker {
	tasker := &sword.Tasker{}
	if automat != 0 {
		tasker.Automat = &sword.AutomatId{
			Id: proto.Uint32(automat),
		}
	}
	if crowd != 0 {
		tasker.Crowd = &sword.CrowdId{
			Id: proto.Uint32(crowd),
		}
	}
	if formation != 0 {
		tasker.Formation = &sword.FormationId{
			Id: proto.Uint32(formation),
		}
	}
	if party != 0 {
		tasker.Party = &sword.PartyId{
			Id: proto.Uint32(party),
		}
	}
	if unit != 0 {
		tasker.Unit = &sword.UnitId{
			Id: proto.Uint32(unit),
		}
	}
	return tasker
}

func GetUnitMagicActionAck(msg *sword.UnitMagicActionAck) (uint32, error) {
	code := msg.GetErrorCode()
	if code == sword.UnitActionAck_no_error {
		id := msg.GetUnit().GetId()
		return id, nil
	}
	return 0, makeError(msg, int32(code), sword.UnitActionAck_ErrorCode_name)
}

func GetKnowledgeGroupMagicActionAck(msg *sword.KnowledgeGroupMagicActionAck) (uint32, error) {
	code := msg.GetErrorCode()
	if code == sword.KnowledgeGroupAck_no_error {
		id := msg.GetKnowledgeGroup().GetId()
		return id, nil
	}
	return 0, makeError(msg, int32(code), sword.KnowledgeGroupAck_ErrorCode_name)
}

type simHandler func(msg *sword.SimToClient_Content) error

func (c *Client) postSimRequest(msg SwordMessage, handler simHandler) <-chan error {
	quit := make(chan error, 1)
	wrapper := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.SimulationToClient == nil ||
			msg.SimulationToClient.GetMessage() == nil ||
			msg.ClientId != c.clientId ||
			msg.Context != context {
			return false
		}
		err = handler(msg.SimulationToClient.GetMessage())
		if err == ErrContinue {
			return false
		}
		quit <- err
		return true
	}
	c.Post(msg, wrapper)
	return quit
}

func (c *Client) CreateFormationTest(partyId uint32, parentId uint32,
	params *sword.MissionParameters) (*Formation, error) {
	taskerId := parentId
	if partyId != 0 {
		taskerId = partyId
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     makeTasker(0, 0, parentId, partyId, 0),
					Type:       sword.UnitMagicAction_formation_creation.Enum(),
					Parameters: params,
				},
			},
		},
	}
	var created *Formation
	receivedTaskerId := uint32(0)
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetMagicActionAck(); reply != nil {
			// Ignore this message, UnitMagicActionAck should be enough
			return ErrContinue
		}
		if reply := msg.GetFormationCreation(); reply != nil {
			// Ignore this message, its context should not be set anyway
			return ErrContinue
		}
		reply := msg.GetUnitMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		// Wait for the final UnitMagicActionAck
		id, err := GetUnitMagicActionAck(reply)
		if err != nil {
			return err
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		receivedTaskerId = id
		created = c.Model.GetFormation(value.GetFormation().GetId())
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	if err != nil {
		return nil, err
	}
	if receivedTaskerId == 0 {
		return nil, invalid("tasker id", receivedTaskerId)
	}
	if taskerId != receivedTaskerId {
		return nil, mismatch("tasker id", taskerId, receivedTaskerId)
	}
	return created, nil
}

func (c *Client) CreateFormation(partyId uint32, parentId uint32,
	name string, level int, logLevel string) (*Formation, error) {
	params := MakeParameters(
		MakeFloat(float32(level)),
		MakeString(name),
		MakeString(logLevel))
	return c.CreateFormationTest(partyId, parentId, params)
}

func (c *Client) createUnit(automatId, unitType uint32, location Point,
	name *string, pc *bool) (*Unit, error) {
	params := []*sword.MissionParameter{
		MakeIdentifier(unitType),
		MakePointParam(location),
	}
	if name != nil {
		params = append(params, MakeString(*name))
	}
	if pc != nil {
		params = append(params, MakeBoolean(*pc))
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     makeTasker(automatId, 0, 0, 0, 0),
					Type:       sword.UnitMagicAction_unit_creation.Enum(),
					Parameters: MakeParameters(params...),
				},
			},
		},
	}
	var created *Unit
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetUnitCreation(); reply != nil {
			// Context should not be set on this
			return ErrContinue
		}
		if reply := msg.GetUnitAttributes(); reply != nil {
			// Context should not be set on this
			return ErrContinue
		}
		if reply := msg.GetUnitEnvironmentType(); reply != nil {
			// Context should not be set on this
			return ErrContinue
		}
		reply := msg.GetUnitMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		_, err := GetUnitMagicActionAck(reply)
		if err != nil {
			return err
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		created = c.Model.GetUnit(value.GetAgent().GetId())
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return created, err
}

func (c *Client) CreateUnit(automatId, unitType uint32, location Point) (*Unit, error) {
	return c.createUnit(automatId, unitType, location, nil, nil)
}

func (c *Client) CreateUnitWithName(automatId, unitType uint32, location Point,
	name string, pc bool) (*Unit, error) {
	return c.createUnit(automatId, unitType, location, &name, &pc)
}

func (c *Client) waitOrder(id uint32) (*Order, error) {
	done := c.Model.WaitCondition(func(data *ModelData) bool {
		_, found := data.Orders[id]
		return found
	})
	if !done {
		return nil, ErrNotFound
	}
	return c.Model.GetOrder(id), nil
}

func (c *Client) sendOrder(msg SwordMessage) (*Order, error) {
	id := uint32(0)
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetOrderAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.OrderAck_no_error {
			return makeError(reply, int32(code), sword.OrderAck_ErrorCode_name)
		}
		id = reply.GetId()
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	if err != nil {
		return nil, err
	}
	return c.waitOrder(id)
}

func (c *Client) SendUnitOrder(unitId, missionType uint32,
	params *sword.MissionParameters) (*Order, error) {
	return c.sendOrder(SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitOrder: &sword.UnitOrder{
					Tasker: &sword.UnitId{
						Id: proto.Uint32(unitId),
					},
					Type: &sword.MissionType{
						Id: proto.Uint32(missionType),
					},
					Parameters: params,
				},
			},
		},
	})
}

func (c *Client) SendAutomatOrder(automatId, missionType uint32,
	params *sword.MissionParameters) (*Order, error) {
	return c.sendOrder(SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				AutomatOrder: &sword.AutomatOrder{
					Tasker: &sword.AutomatId{
						Id: proto.Uint32(automatId),
					},
					Type: &sword.MissionType{
						Id: proto.Uint32(missionType),
					},
					Parameters: params,
				},
			},
		},
	})
}

func (c *Client) DeleteUnit(unitId uint32) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     makeTasker(0, 0, 0, 0, unitId),
					Type:       sword.UnitMagicAction_delete_unit.Enum(),
					Parameters: MakeParameters(),
				},
			},
		},
	}
	destroyed := false
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetUnitDestruction(); reply != nil {
			if destroyed {
				return unexpected(msg)
			}
			destroyed = true
			return ErrContinue
		}
		reply := msg.GetUnitMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		id, err := GetUnitMagicActionAck(reply)
		if err != nil {
			return err
		}
		if !destroyed {
			return unexpected(msg)
		}
		if id != unitId {
			return mismatch("unit id", unitId, id)
		}
		return nil
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) CreateAutomat(formationId, automatId, automatType,
	knowledgeGroupId uint32) (*Automat, error) {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker: makeTasker(automatId, 0, formationId, 0, 0),
					Type:   sword.UnitMagicAction_automat_creation.Enum(),
					Parameters: MakeParameters(
						MakeIdentifier(automatType),
						MakeIdentifier(knowledgeGroupId),
					),
				},
			},
		},
	}
	var created *Automat
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetAutomatCreation(); reply != nil {
			// Ignore this message, its context should not be set anyway
			return ErrContinue
		}
		reply := msg.GetUnitMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		_, err := GetUnitMagicActionAck(reply)
		if err != nil {
			return err
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		created = c.Model.GetAutomat(value.GetAutomat().GetId())
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return created, err
}

func (c *Client) CreateCrowd(partyId, formationId uint32, crowdType string,
	location Point, healthy, wounded, dead int32, name string) (*Crowd, error) {

	params := []*sword.MissionParameter{
		MakeString(crowdType),
		MakePointParam(location),
		MakeQuantity(healthy),
		MakeQuantity(wounded),
		MakeQuantity(dead),
		MakeString(name),
	}

	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     makeTasker(0, 0, formationId, partyId, 0),
					Type:       sword.UnitMagicAction_crowd_creation.Enum(),
					Parameters: MakeParameters(params...),
				},
			},
		},
	}

	var created *Crowd
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetCrowdCreation(); reply != nil {
			// Context should not be set on this
			return ErrContinue
		}
		reply := msg.GetUnitMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		_, err := GetUnitMagicActionAck(reply)
		if err != nil {
			return err
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		created = c.Model.GetCrowd(value.GetCrowd().GetId())
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return created, err
}

func (c *Client) SendCrowdOrder(crowdId, missionType uint32,
	params *sword.MissionParameters) (*Order, error) {
	return c.sendOrder(SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				CrowdOrder: &sword.CrowdOrder{
					Tasker: &sword.CrowdId{
						Id: proto.Uint32(crowdId),
					},
					Type: &sword.MissionType{
						Id: proto.Uint32(missionType),
					},
					Parameters: params,
				},
			},
		},
	})
}

func (c *Client) SetAutomatMode(automatId uint32, engaged bool) error {
	mode := sword.EnumAutomatMode_engaged
	if !engaged {
		mode = sword.EnumAutomatMode_disengaged
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				SetAutomatMode: &sword.SetAutomatMode{
					Automate: &sword.AutomatId{
						Id: proto.Uint32(automatId),
					},
					Mode: &mode,
				},
			},
		},
	}
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetSetAutomatModeAck()
		if reply == nil {
			return unexpected(msg)
		}
		if reply.GetAutomate() == nil || reply.GetAutomate().GetId() != automatId {
			return fmt.Errorf("invalid automat identifier")
		}
		if code := reply.GetErrorCode(); code != sword.SetAutomatModeAck_no_error {
			return makeError(reply, int32(code), sword.SetAutomatModeAck_ErrorCode_name)
		}
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	if err != nil {
		return err
	}
	ok := c.Model.WaitCondition(func(data *ModelData) bool {
		return data.FindAutomat(automatId).Engaged == engaged
	})
	if !ok {
		return fmt.Errorf("automat mode change timed out")
	}
	return nil
}

type ControlAckError interface {
	GetErrorCode() sword.ControlAck_ErrorCode
}

func getControlAckError(reply ControlAckError) error {
	code := reply.GetErrorCode()
	if code == sword.ControlAck_no_error {
		return nil
	}
	return makeError(reply, int32(code), sword.ControlAck_ErrorCode_name)
}

func (c *Client) Pause() error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlPause: &sword.ControlPause{},
			},
		},
	}
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetControlPauseAck()
		if reply == nil {
			return unexpected(msg)
		}
		return getControlAckError(reply)
	}
	return <-c.postSimRequest(msg, handler)
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
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetControlResumeAck()
		if reply == nil {
			return unexpected(msg)
		}
		return getControlAckError(reply)
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) Stop() error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlStop: &sword.ControlStop{},
			},
		},
	}
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetControlStopAck()
		if reply == nil {
			return unexpected(msg)
		}
		return getControlAckError(reply)
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) SendFragOrder(automatId, crowdId, unitId, fragOrderType uint32,
	params *sword.MissionParameters) (*Order, error) {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				FragOrder: &sword.FragOrder{
					Tasker: makeTasker(automatId, crowdId, 0, 0, unitId),
					Type: &sword.FragOrderType{
						Id: proto.Uint32(fragOrderType),
					},
					Parameters: params,
				},
			},
		},
	}
	id := uint32(0)
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetFragOrderAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.OrderAck_no_error {
			return makeError(reply, int32(code), sword.OrderAck_ErrorCode_name)
		}
		id = reply.GetId()
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	if err != nil {
		return nil, err
	}
	return c.waitOrder(id)
}

func (c *Client) SendAutomatFragOrder(automatId, fragOrderType uint32,
	params *sword.MissionParameters) (*Order, error) {
	return c.SendFragOrder(automatId, 0, 0, fragOrderType, params)
}

func (c *Client) SendCrowdFragOrder(crowdId, fragOrderType uint32,
	params *sword.MissionParameters) (*Order, error) {
	return c.SendFragOrder(0, crowdId, 0, fragOrderType, params)
}

func (c *Client) SendUnitFragOrder(unitId, fragOrderType uint32,
	params *sword.MissionParameters) (*Order, error) {
	return c.SendFragOrder(0, 0, unitId, fragOrderType, params)
}

func defaultUnitMagicHandler(msg *sword.SimToClient_Content) error {
	reply := msg.GetUnitMagicActionAck()
	if reply == nil {
		return unexpected(msg)
	}
	_, err := GetUnitMagicActionAck(reply)
	return err
}

func defaultMagicHandler(msg *sword.SimToClient_Content) error {
	reply := msg.GetMagicActionAck()
	if reply == nil {
		return unexpected(msg)
	}
	code := reply.GetErrorCode()
	if code != sword.MagicActionAck_no_error {
		return makeError(reply, int32(code), sword.MagicActionAck_ErrorCode_name)
	}
	return nil
}

func defaultKnowledgeGroupMagicHandler(msg *sword.SimToClient_Content) error {
	reply := msg.GetKnowledgeGroupMagicActionAck()
	if reply == nil {
		return unexpected(msg)
	}
	_, err := GetKnowledgeGroupMagicActionAck(reply)
	return err
}

type UnitMagicEnumer interface {
	Enum() *sword.UnitMagicAction_Type
}

func (c *Client) sendUnitMagicAction(automat, crowd, formation, unit uint32,
	params *sword.MissionParameters, magicAction UnitMagicEnumer) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     makeTasker(automat, crowd, formation, 0, unit),
					Type:       magicAction.Enum(),
					Parameters: params,
				},
			},
		},
	}
	return <-c.postSimRequest(msg, defaultUnitMagicHandler)
}

func createKnowledgeMagicActionMessage(params *sword.MissionParameters, knowledgeGroupId uint32,
	magicAction *sword.KnowledgeMagicAction_Type) *SwordMessage {
	return &SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				KnowledgeMagicAction: &sword.KnowledgeMagicAction{
					KnowledgeGroup: &sword.KnowledgeGroupId{
						Id: &knowledgeGroupId,
					},
					Type:       magicAction,
					Parameters: params,
				},
			},
		},
	}
}

func (c *Client) TeleportUnit(unitId uint32, location Point) error {
	return c.sendUnitMagicAction(0, 0, 0, unitId,
		MakeParameters(MakePointParam(location)),
		sword.UnitMagicAction_move_to)
}

func (c *Client) TeleportCrowd(crowdId uint32, location Point) error {
	return c.sendUnitMagicAction(0, crowdId, 0, 0,
		MakeParameters(MakePointParam(location)),
		sword.UnitMagicAction_move_to)
}

func (c *Client) DestroyLocalWeather(id uint32) error {
	params := MakeParameters()
	if id != 0 {
		params = MakeParameters(
			MakeIdentifier(id))
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type:       sword.MagicAction_local_weather_destruction.Enum(),
					Parameters: params,
				},
			},
		},
	}
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) UpdateGlobalWeather(global *Weather) error {
	params := MakeParameters()
	if global != nil {
		params = MakeParameters(
			MakeFloat(global.Temperature),
			MakeFloat(global.WindSpeed),
			MakeHeading(global.WindDirection),
			MakeFloat(global.CloudFloor),
			MakeFloat(global.CloudCeil),
			MakeFloat(global.CloudDensity),
			MakeEnumeration(int32(global.Precipitation)))
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type:       sword.MagicAction_global_weather.Enum(),
					Parameters: params,
				},
			},
		},
	}
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) CreateLocalWeather(local *LocalWeather) (*LocalWeather, error) {
	params := MakeParameters()
	if local != nil {
		params = MakeParameters(MakeFloat(local.Temperature),
			MakeFloat(local.WindSpeed),
			MakeHeading(local.WindDirection),
			MakeFloat(local.CloudFloor),
			MakeFloat(local.CloudCeil),
			MakeFloat(local.CloudDensity),
			MakeEnumeration(int32(local.Precipitation)),
			MakeTime(local.StartTime),
			MakeTime(local.EndTime),
			MakeRectangleParam(local.TopLeft, local.BottomRight),
			MakeIdentifier(local.Id),
		)
	}
	// FIXME lighting is ignored because it's completely broken
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type:       sword.MagicAction_local_weather.Enum(),
					Parameters: params,
				},
			},
		},
	}
	var id uint32
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.MagicActionAck_no_error {
			return makeError(reply, int32(code), sword.MagicActionAck_ErrorCode_name)
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		id = value.GetIdentifier()
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	if err != nil {
		return nil, err
	}
	ok := c.Model.WaitCondition(func(data *ModelData) bool {
		return data.LocalWeathers[id] != nil
	})
	if !ok {
		return nil, ErrTimeout
	}
	return c.Model.GetLocalWeather(id), nil
}

func (c *Client) ChangeDiplomacyTest(params *sword.MissionParameters) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type:       sword.MagicAction_change_diplomacy.Enum(),
					Parameters: params,
				},
			},
		},
	}
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) ChangeDiplomacy(party1Id uint32, party2Id uint32, diplomacy sword.EnumDiplomacy) error {
	params := MakeParameters(
		MakeIdentifier(party1Id),
		MakeIdentifier(party2Id),
		MakeEnumeration(int32(diplomacy)))
	return c.ChangeDiplomacyTest(params)
}

func (c *Client) CreateFireOnLocationTest(params *sword.MissionParameters) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type:       sword.MagicAction_create_fire_order_on_location.Enum(),
					Parameters: params,
				},
			},
		},
	}
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) CreateFireOnLocation(location Point, ammoType uint32, salvoCount int) error {
	params := MakeParameters(
		MakePointParam(location),
		MakeResourceType(ammoType),
		MakeFloat(float32(salvoCount)))
	return c.CreateFireOnLocationTest(params)
}

func (c *Client) ChangeResourceNetworkTest(params *sword.MissionParameters) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type:       sword.MagicAction_change_resource_network_properties.Enum(),
					Parameters: params,
				},
			},
		},
	}
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) ChangeResourceNetwork(urban *Urban) error {
	params := MakeParameters(MakeNullValue(), MakeNullValue())
	params.Elem[0] = MakeIdentifier(urban.Id)
	resources := []*sword.MissionParameter_Value{}
	for _, r := range urban.ResourceNetworks {
		resources = append(resources,
			MakeList(
				MakeString(r.Name),
				MakeQuantity(int32(r.Consumption)),
				MakeBoolean(r.Critical),
				MakeBoolean(r.Activated),
				MakeQuantity(int32(r.Production)),
				MakeQuantity(int32(r.StockMax))))
	}
	params.Elem[1] = MakeParameter(resources...)
	return c.ChangeResourceNetworkTest(params)
}

func (c *Client) SendTotalDestruction(crowdId uint32) error {
	return c.sendUnitMagicAction(0, crowdId, 0, 0,
		MakeParameters(), sword.UnitMagicAction_crowd_total_destruction)
}

func (c *Client) ChangeArmedIndividuals(crowdId uint32, armedIndividuals int32) error {
	params := MakeParameters()
	if armedIndividuals != 0 {
		params = MakeParameters(MakeQuantity(armedIndividuals))
	}
	return c.sendUnitMagicAction(0, crowdId, 0, 0, params,
		sword.UnitMagicAction_crowd_change_armed_individuals)
}

func (c *Client) ChangeCriticalIntelligence(crowdId uint32, criticalIntelligence string) error {
	params := MakeParameters()
	if criticalIntelligence != "" {
		params = MakeParameters(MakeString(criticalIntelligence))
	}
	return c.sendUnitMagicAction(0, crowdId, 0, 0, params,
		sword.UnitMagicAction_change_critical_intelligence)
}

func (c *Client) ChangeHealthState(crowdId uint32, healthy, wounded, contaminated, dead int32) error {
	params := MakeParameters(MakeQuantity(healthy),
		MakeQuantity(wounded),
		MakeQuantity(contaminated),
		MakeQuantity(dead))
	return c.sendUnitMagicAction(0, crowdId, 0, 0, params,
		sword.UnitMagicAction_crowd_change_health_state)
}

func (c *Client) ChangeCrowdAdhesions(crowdId uint32, adhesions map[uint32]float32) error {
	params := MakeParameters()
	if len(adhesions) != 0 {
		params = MakeParameters(MakeAdhesions(adhesions))
	}
	return c.sendUnitMagicAction(0, crowdId, 0, 0, params,
		sword.UnitMagicAction_crowd_change_affinities)
}

func (c *Client) ChangeUnitAdhesions(unitId uint32, adhesions map[uint32]float32) error {
	params := MakeParameters()
	if len(adhesions) != 0 {
		params = MakeParameters(MakeAdhesions(adhesions))
	}
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_unit_change_affinities)
}

func (c *Client) ReloadBrain(crowdId uint32, model string) error {
	params := MakeParameters()
	if model != "" {
		params = MakeParameters(MakeString(model))
	}
	return c.sendUnitMagicAction(0, crowdId, 0, 0, params,
		sword.UnitMagicAction_reload_brain)
}

func (c *Client) ChangeExtensions(crowdId uint32, extensions *map[string]string) error {
	params := MakeParameters()
	if extensions != nil {
		params = MakeParameters(MakeExtensions(extensions))
	}
	return c.sendUnitMagicAction(0, crowdId, 0, 0, params,
		sword.UnitMagicAction_change_extension)
}

func (c *Client) ChangeAttitude(crowdId uint32, attitude int32) error {
	params := MakeParameters()
	if attitude != 0 {
		params = MakeParameters(MakeEnumeration(attitude))
	}
	return c.sendUnitMagicAction(0, crowdId, 0, 0, params,
		sword.UnitMagicAction_crowd_change_attitude)
}

func (c *Client) LogisticsChangeLinks(automatId uint32, superiors []uint32) error {
	params := []*sword.MissionParameter{}
	for _, s := range superiors {
		params = append(params, MakeIdentifier(s))
	}
	return c.sendUnitMagicAction(automatId, 0, 0, 0,
		MakeParameters(params...),
		sword.UnitMagicAction_change_logistic_links)
}

func (c *Client) LogisticsSupplyChangeQuotas(supplierId uint32, suppliedId uint32, quotas map[uint32]int32) error {
	params := []*sword.MissionParameter{MakeIdentifier(supplierId)}
	values := []*sword.MissionParameter_Value{}
	for dotation, qty := range quotas {
		values = append(values, MakeList(MakeIdentifier(dotation), MakeQuantity(qty)))
	}
	params = append(params, MakeParameter(values...))
	return c.sendUnitMagicAction(suppliedId, 0, 0, 0,
		MakeParameters(params...),
		sword.UnitMagicAction_log_supply_change_quotas)
}

func (c *Client) LogisticsSupplyPushFlowTest(supplierId uint32, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(0, 0, 0, supplierId, params,
		sword.UnitMagicAction_log_supply_push_flow)
}

func (c *Client) LogisticsSupplyPushFlow(supplierId uint32, suppliedId uint32) error {
	recipient := &sword.SupplyFlowRecipient{Receiver: &sword.AutomatId{Id: proto.Uint32(suppliedId)}}
	recipients := []*sword.SupplyFlowRecipient{recipient}
	pushFlowParams := &sword.PushFlowParameters{Recipients: recipients}
	param := MakeParameter(&sword.MissionParameter_Value{PushFlowParameters: pushFlowParams})
	return c.LogisticsSupplyPushFlowTest(supplierId, MakeParameters(param))
}

func (c *Client) LogisticsSupplyPullFlow(supplierId uint32, suppliedId uint32) error {
	supplier := &sword.ParentEntity{Formation: &sword.FormationId{Id: proto.Uint32(supplierId)}}
	pullFlowParams := &sword.PullFlowParameters{Supplier: supplier}
	param := MakeParameter(&sword.MissionParameter_Value{PullFlowParameters: pullFlowParams})
	return c.sendUnitMagicAction(suppliedId, 0, 0, 0, MakeParameters(param),
		sword.UnitMagicAction_log_supply_pull_flow)
}

func (c *Client) CreateKnowledgeGroupTest(params *sword.MissionParameters) (*KnowledgeGroup, error) {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type:       sword.MagicAction_create_knowledge_group.Enum(),
					Parameters: params,
				},
			},
		},
	}
	var id uint32
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.MagicActionAck_no_error {
			return makeError(reply, int32(code), sword.MagicActionAck_ErrorCode_name)
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		id = value.GetIdentifier()
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	if err != nil {
		return nil, err
	}
	var group *KnowledgeGroup
	ok := c.Model.WaitCondition(func(data *ModelData) bool {
		for _, party := range data.Parties {
			for _, g := range party.KnowledgeGroups {
				if g.Id == id {
					group = g
					return true
				}
			}
		}
		return false
	})
	if !ok {
		return nil, ErrTimeout
	}
	return group, nil
}

func (c *Client) CreateKnowledgeGroup(partyId uint32,
	kind string) (*KnowledgeGroup, error) {

	params := MakeParameters(MakeIdentifier(partyId), MakeString(kind))
	return c.CreateKnowledgeGroupTest(params)
}

func (c *Client) ChangeUnitSuperior(unitId, automatId uint32) error {
	params := MakeParameters()
	if automatId != 0 {
		params = MakeParameters(MakeAutomat(automatId))
	}
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_unit_change_superior)
}

func (c *Client) ChangeAutomatSuperior(automatId, formationId uint32) error {
	params := MakeParameters()
	if formationId != 0 {
		params = MakeParameters(MakeFormation(formationId))
	}
	return c.sendUnitMagicAction(automatId, 0, 0, 0, params,
		sword.UnitMagicAction_change_formation_superior)
}

func (c *Client) ChangeFormationSuperior(formationId, parentId uint32, isParty bool) error {
	params := MakeParameters()
	if parentId != 0 {
		if isParty {
			params = MakeParameters(MakeParty(parentId))
		} else {
			params = MakeParameters(MakeFormation(parentId))
		}
	}
	return c.sendUnitMagicAction(0, 0, formationId, 0, params,
		sword.UnitMagicAction_change_formation_superior)
}

func (c *Client) KnowledgeGroupMagicActionTest(actionType sword.KnowledgeMagicAction_Type, params *sword.MissionParameters, knowledgeGroupId uint32) error {
	msg := createKnowledgeMagicActionMessage(params, knowledgeGroupId, &actionType)
	return <-c.postSimRequest(*msg, defaultKnowledgeGroupMagicHandler)
}

func (c *Client) EnableKnowledgeGroup(knowledgeGroupId uint32, enable bool) error {
	return c.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_enable, MakeParameters(MakeBoolean(enable)), knowledgeGroupId)
}

func (c *Client) ChangeKnowledgeGroupSuperiorToArmy(knowledgeGroupId uint32, partyId uint32) error {
	return c.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_party, MakeParameters(MakeParty(partyId)), knowledgeGroupId)
}

func (c *Client) ChangeKnowledgeGroupSuperiorToKnowledgeGroup(knowledgeGroupId uint32, partyId uint32, superiorKnowledgeGroupId uint32) error {
	params := MakeParameters(MakeParty(partyId), MakeKnowledgeGroup(superiorKnowledgeGroupId))
	return c.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_party_parent, params, knowledgeGroupId)
}

func (c *Client) ChangeKnowledgeGroupType(knowledgeGroupId uint32, knowledgeGroupType string) error {
	return c.KnowledgeGroupMagicActionTest(sword.KnowledgeMagicAction_update_type, MakeParameters(MakeString(knowledgeGroupType)), knowledgeGroupId)
}

func (c *Client) AddUnitKnowledgeInKnowledgeGroup(knowledgeGroupId uint32, entityId uint32, perceptionLevel int32) (*UnitKnowledge, error) {
	params := MakeParameters(MakeIdentifier(entityId), MakeEnumeration(perceptionLevel))
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				KnowledgeMagicAction: &sword.KnowledgeMagicAction{
					KnowledgeGroup: &sword.KnowledgeGroupId{
						Id: &knowledgeGroupId,
					},
					Type:       sword.KnowledgeMagicAction_add_knowledge.Enum(),
					Parameters: params,
				},
			},
		},
	}

	var created *UnitKnowledge
	handler := func(msg *sword.SimToClient_Content) error {

		if msg.GetUnitKnowledgeCreation() != nil ||
			msg.GetUnitKnowledgeUpdate() != nil {
			return ErrContinue
		}
		reply := msg.GetKnowledgeGroupMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		_, err := GetKnowledgeGroupMagicActionAck(reply)
		if err != nil {
			return err
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		created = c.Model.GetUnitKnowledge(knowledgeGroupId, value.GetIdentifier())
		if created == nil {
			return fmt.Errorf("created unit knowledge %d/%d is not available "+
				"after ack", knowledgeGroupId, value.GetIdentifier())
		}
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return created, err
}

func (c *Client) ChangeKnowledgeGroupTest(automatId uint32,
	params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(automatId, 0, 0, 0, params,
		sword.UnitMagicAction_change_knowledge_group)
}

func (c *Client) ChangeKnowledgeGroup(automatId, knowledgeGroupId uint32) error {
	params := MakeParameters(MakeKnowledgeGroup(knowledgeGroupId))
	return c.ChangeKnowledgeGroupTest(automatId, params)
}

func (c *Client) DebugBrainTest(id uint32, params *sword.MissionParameters) error {
	// Abusing the super tolerant simulation tasker parser
	return c.sendUnitMagicAction(0, 0, 0, id, params,
		sword.UnitMagicAction_change_brain_debug)
}

func (c *Client) DebugBrain(automatId uint32, enable bool) error {
	params := MakeParameters(MakeBoolean(enable))
	return c.DebugBrainTest(automatId, params)
}

func (c *Client) TransferEquipment(unitId uint32, targetId uint32, equipments []Equipment) error {
	params := MakeParameters(MakeIdentifier(targetId), MakeEquipments(equipments))
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_transfer_equipment)
}

func (c *Client) SurrenderTest(automatId uint32, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(automatId, 0, 0, 0, params,
		sword.UnitMagicAction_surrender_to)
}

func (c *Client) Surrender(automatId, partyId uint32) error {
	return c.SurrenderTest(automatId, MakeParameters(MakeParty(partyId)))
}

func (c *Client) CancelSurrender(automatId uint32) error {
	params := MakeParameters()
	return c.sendUnitMagicAction(automatId, 0, 0, 0, params,
		sword.UnitMagicAction_cancel_surrender)
}

func (c *Client) CreateWounds(unitId uint32, humans map[int32]int32) error {
	params := MakeParameters()
	if len(humans) != 0 {
		params = MakeParameters(MakeHumans(humans))
	}
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_create_wounds)
}

func (c *Client) ChangeHumanState(unitId uint32, humans []*HumanDotation) error {
	params := MakeParameters()
	if len(humans) != 0 {
		params = MakeParameters(MakeHumansDotation(humans))
	}
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_change_human_state)
}

func (c *Client) ChangeDotation(unitId uint32, resources []*ResourceDotation) error {
	params := MakeParameters()
	if len(resources) != 0 {
		params = MakeParameters(MakeResourcesDotation(resources))
	}
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_change_dotation)
}

func (c *Client) ChangeEquipmentState(unitId uint32, equipments map[uint32]*EquipmentDotation) error {
	params := MakeParameters()
	if len(equipments) != 0 {
		params = MakeParameters(MakeEquipmentDotation(equipments))
	}
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_change_equipment_state)
}

func (c *Client) CreateBreakdowns(unitId uint32, equipments map[uint32]*EquipmentDotation) error {
	params := MakeParameters()
	if len(equipments) != 0 {
		params = MakeParameters(MakeBreakdowns(equipments))
	}
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_create_breakdowns)
}

func (c *Client) ChangePostureTest(unitId uint32, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_change_posture)
}

func (c *Client) ChangePosture(unitId uint32, posture sword.UnitAttributes_Posture) error {
	return c.ChangePostureTest(unitId, MakeParameters(MakeEnumeration(int32(posture))))
}

func (c *Client) ChangeHumanFactorsTest(unitId uint32, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(0, 0, 0, unitId, params,
		sword.UnitMagicAction_change_human_factors)
}

func (c *Client) ChangeHumanFactors(unitId uint32, tiredness sword.UnitAttributes_EnumUnitTiredness,
	morale sword.UnitAttributes_EnumUnitMorale, experience sword.UnitAttributes_EnumUnitExperience,
	stress sword.UnitAttributes_EnumUnitStress) error {
	params := MakeParameters(MakeEnumeration(int32(tiredness)), MakeEnumeration(int32(morale)),
		MakeEnumeration(int32(experience)), MakeEnumeration(int32(stress)))
	return c.ChangeHumanFactorsTest(unitId, params)
}

func (c *Client) EnableVisionCones(enabled bool, unitIds ...uint32) error {
	var units []*sword.UnitId = nil
	for _, unitId := range unitIds {
		units = append(units, &sword.UnitId{Id: proto.Uint32(unitId)})
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlToggleVisionCones: &sword.ControlEnableVisionCones{
					VisionCones: proto.Bool(enabled),
					Units:       units,
				},
			},
		},
	}
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetControlEnableVisionConesAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.ControlEnableVisionConesAck_no_error {
			return makeError(reply, int32(code), sword.ControlEnableVisionConesAck_ErrorCode_name)
		}
		return nil
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) ListEnabledVisionCones(start uint32, count uint32) (*sword.ListEnabledVisionConesAck, error) {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ListEnabledVisionCones: &sword.ListEnabledVisionCones{
					Start: &sword.UnitId{Id: proto.Uint32(start)},
					Count: proto.Uint32(count),
				},
			},
		},
	}
	var reply *sword.ListEnabledVisionConesAck
	handler := func(msg *sword.SimToClient_Content) error {
		reply = msg.GetListEnabledVisionConesAck()
		if reply == nil {
			return unexpected(msg)
		}
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return reply, err
}

// Returns the checkpoint name or an error.
func (c *Client) CreateCheckpoint(name string) (string, error) {
	var n *string
	if len(name) > 0 {
		n = proto.String(name)
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlCheckpointSaveNow: &sword.ControlCheckPointSaveNow{
					Name: n,
				},
			},
		},
	}
	checkpoint := ""
	err := <-c.postSimRequest(msg, func(msg *sword.SimToClient_Content) error {
		reply := msg.GetControlCheckpointSaveNowAck()
		if reply == nil {
			return unexpected(msg)
		}
		if e := reply.GetErrorMsg(); len(e) > 0 {
			return fmt.Errorf("%s", e)
		}
		checkpoint = reply.GetName()
		return nil
	})
	return checkpoint, err
}

func (c *Client) RecoverTransporters(unitId uint32) error {
	return c.sendUnitMagicAction(0, 0, 0, unitId, MakeParameters(),
		sword.UnitMagicAction_recover_transporters)
}
