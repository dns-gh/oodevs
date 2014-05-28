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
	"bytes"
	"code.google.com/p/goprotobuf/proto"
	"errors"
	"fmt"
	"sword"
	"text/template"
)

func getUnitMagicActionAck(msg *sword.SimToClient_Content) (*sword.UnitMagicActionAck,
	uint32, error) {
	ack := msg.GetUnitMagicActionAck()
	if ack == nil {
		return nil, 0, unexpected(msg)
	}
	code := ack.GetErrorCode()
	if code == sword.UnitActionAck_no_error {
		id := ack.GetUnit().GetId()
		return ack, id, nil
	}
	return ack, 0, makeError(ack, int32(code), sword.UnitActionAck_ErrorCode_name)
}

func getObjectMagicActionAck(msg *sword.SimToClient_Content) (*sword.ObjectMagicActionAck,
	uint32, error) {
	ack := msg.GetObjectMagicActionAck()
	if ack == nil {
		return nil, 0, unexpected(msg)
	}
	code := ack.GetErrorCode()
	if code == sword.ObjectMagicActionAck_no_error {
		id := ack.GetObject().GetId()
		return ack, id, nil
	}
	return ack, 0, makeError(ack, int32(code), sword.ObjectMagicActionAck_ErrorCode_name)
}

func getKnowledgeGroupMagicActionAck(msg *sword.SimToClient_Content) (
	*sword.KnowledgeGroupMagicActionAck, uint32, error) {
	ack := msg.GetKnowledgeGroupMagicActionAck()
	if ack == nil {
		return nil, 0, unexpected(msg)
	}
	code := ack.GetErrorCode()
	if code == sword.KnowledgeGroupAck_no_error {
		id := ack.GetKnowledgeGroup().GetId()
		return ack, id, nil
	}
	return ack, 0, makeError(msg, int32(code), sword.KnowledgeGroupAck_ErrorCode_name)
}

type simHandler func(msg *sword.SimToClient_Content) error

func (c *Client) postSimRequest(msg SwordMessage, handler simHandler) <-chan error {
	quit := make(chan error, 1)
	wrapper := func(msg *SwordMessage, client, context int32, err error) bool {
		if err != nil {
			quit <- err
			return true
		}
		if msg.SimulationToClient == nil ||
			msg.SimulationToClient.GetMessage() == nil ||
			msg.ClientId != client ||
			msg.Context != context {
			return false
		}
		errMsg := msg.SimulationToClient.GetErrorMsg()
		if len(errMsg) > 0 {
			err = errors.New(errMsg)
		} else {
			err = handler(msg.SimulationToClient.GetMessage())
			if err == ErrContinue {
				return false
			}
		}
		quit <- err
		return true
	}
	c.Post(msg, wrapper)
	return quit
}

func (c *Client) CreateFormationTest(partyId uint32, parentId uint32,
	params *sword.MissionParameters) (*Formation, error) {
	tasker := MakeFormationTasker(parentId)
	taskerId := parentId
	if partyId != 0 {
		tasker = MakePartyTasker(partyId)
		taskerId = partyId
	}
	msg := CreateUnitMagicAction(tasker, params, sword.UnitMagicAction_formation_creation)
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
		// Wait for the final UnitMagicActionAck
		reply, id, err := getUnitMagicActionAck(msg)
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
		MakeString(logLevel),
	)
	return c.CreateFormationTest(partyId, parentId, params)
}

func (c *Client) createUnit(automatId, unitType uint32, location Point,
	name *string, pc *bool) (*Unit, error) {
	params := []interface{}{
		MakeIdentifier(unitType),
		MakePointParam(location),
	}
	if name != nil {
		params = append(params, MakeString(*name))
	}
	if pc != nil {
		params = append(params, MakeBoolean(*pc))
	}
	tasker := MakeAutomatTasker(automatId)
	msg := CreateUnitMagicAction(tasker, MakeParameters(params...),
		sword.UnitMagicAction_unit_creation)
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
		reply, _, err := getUnitMagicActionAck(msg)
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
					Tasker:     MakeId(unitId),
					Type:       MakeId(missionType),
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
					Tasker:     MakeId(automatId),
					Type:       MakeId(missionType),
					Parameters: params,
				},
			},
		},
	})
}

func (c *Client) DestroyUnit(unitId uint32) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), MakeParameters(),
		sword.UnitMagicAction_destroy_all)
}

func (c *Client) DeleteUnit(unitId uint32) error {
	tasker := MakeUnitTasker(unitId)
	msg := CreateUnitMagicAction(tasker, MakeParameters(),
		sword.UnitMagicAction_delete_unit)
	destroyed := false
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetUnitDestruction(); reply != nil {
			if destroyed {
				return unexpected(msg)
			}
			destroyed = true
			return ErrContinue
		}
		_, id, err := getUnitMagicActionAck(msg)
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

func (c *Client) CreateAutomat(formationId, automatType,
	knowledgeGroupId uint32) (*Automat, error) {
	tasker := MakeFormationTasker(formationId)
	msg := CreateUnitMagicAction(tasker, MakeParameters(
		MakeIdentifier(automatType),
		MakeIdentifier(knowledgeGroupId),
	), sword.UnitMagicAction_automat_creation)
	var created *Automat
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetAutomatCreation(); reply != nil {
			// Ignore this message, its context should not be set anyway
			return ErrContinue
		}
		reply, _, err := getUnitMagicActionAck(msg)
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

func (c *Client) CreateAutomatAndUnits(formationId, automatType,
	knowledgeGroupId uint32, location Point) (*Automat, []*Unit, error) {
	tasker := MakeFormationTasker(formationId)
	msg := CreateUnitMagicAction(tasker, MakeParameters(
		MakeIdentifier(automatType),
		MakePointParam(location),
		MakeIdentifier(knowledgeGroupId),
	), sword.UnitMagicAction_automat_and_units_creation)
	var result *sword.MissionParameters
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetAutomatCreation(); reply != nil {
			// Ignore this message, its context should not be set anyway
			return ErrContinue
		}
		reply, _, err := getUnitMagicActionAck(msg)
		if err != nil {
			return err
		}
		result = reply.GetResult()
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	units := []*Unit{}
	if err != nil {
		return nil, units, err
	}
	size := len(result.GetElem())
	ids := make([]uint32, size)
	for i := 0; i < size; i++ {
		value := GetParameterValue(result, i)
		if i == 0 {
			automat := value.GetAutomat()
			if automat == nil {
				return nil, units, invalid("result", result)
			}
			ids[i] = automat.GetId()
		} else {
			agent := value.GetAgent()
			if agent == nil {
				return nil, units, invalid("result", result)
			}
			ids[i] = agent.GetId()
		}
	}
	var automat *Automat
	ok := c.Model.WaitCondition(func(data *ModelData) bool {
		automat = data.Automats[ids[0]]
		if automat == nil {
			return false
		}
		for k, v := range ids {
			if k != 0 {
				unit := data.Units[v]
				if unit == nil {
					units = []*Unit{}
					return false
				}
				units = append(units, unit)
			}
		}
		return true
	})
	if !ok {
		return nil, units, ErrTimeout
	}
	return automat, units, nil
}

func (c *Client) CreateCrowd(partyId, formationId uint32, crowdType string,
	location Point, healthy, wounded, dead int32, name string) (*Crowd, error) {

	tasker := MakePartyTasker(partyId)
	if formationId != 0 {
		tasker = MakeFormationTasker(formationId)
	}
	params := []interface{}{
		MakeString(crowdType),
		MakePointParam(location),
		MakeQuantity(healthy),
		MakeQuantity(wounded),
		MakeQuantity(dead),
		MakeString(name),
	}
	msg := CreateUnitMagicAction(tasker, MakeParameters(params...),
		sword.UnitMagicAction_crowd_creation)

	var created *Crowd
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetCrowdCreation(); reply != nil {
			// Context should not be set on this
			return ErrContinue
		}
		reply, _, err := getUnitMagicActionAck(msg)
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
					Tasker:     MakeId(crowdId),
					Type:       MakeId(missionType),
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
					Automate: MakeId(automatId),
					Mode:     &mode,
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
		automat, ok := data.Automats[automatId]
		return ok && automat.Engaged == engaged
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

func (c *Client) SendFragOrder(tasker *sword.Tasker, fragOrderType uint32,
	params *sword.MissionParameters) (*Order, error) {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				FragOrder: &sword.FragOrder{
					Tasker:     tasker,
					Type:       MakeId(fragOrderType),
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
	tasker := MakeAutomatTasker(automatId)
	return c.SendFragOrder(tasker, fragOrderType, params)
}

func (c *Client) SendCrowdFragOrder(crowdId, fragOrderType uint32,
	params *sword.MissionParameters) (*Order, error) {
	tasker := MakeCrowdTasker(crowdId)
	return c.SendFragOrder(tasker, fragOrderType, params)
}

func (c *Client) SendUnitFragOrder(unitId, fragOrderType uint32,
	params *sword.MissionParameters) (*Order, error) {
	tasker := MakeUnitTasker(unitId)
	return c.SendFragOrder(tasker, fragOrderType, params)
}

func defaultUnitMagicHandler(msg *sword.SimToClient_Content) error {
	_, _, err := getUnitMagicActionAck(msg)
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
	_, _, err := getKnowledgeGroupMagicActionAck(msg)
	return err
}

type UnitMagicEnumerator interface {
	Enum() *sword.UnitMagicAction_Type
}

func CreateUnitMagicAction(tasker *sword.Tasker, params *sword.MissionParameters,
	enumerator UnitMagicEnumerator) SwordMessage {
	return SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     tasker,
					Type:       enumerator.Enum(),
					Parameters: params,
				},
			},
		},
	}
}

func (c *Client) sendUnitMagicAction(tasker *sword.Tasker,
	params *sword.MissionParameters, enumerator UnitMagicEnumerator) error {
	msg := CreateUnitMagicAction(tasker, params, enumerator)
	return <-c.postSimRequest(msg, defaultUnitMagicHandler)
}

type KnowledgeMagicEnumerator interface {
	Enum() *sword.KnowledgeMagicAction_Type
}

func CreateKnowledgeMagicActionMessage(params *sword.MissionParameters, knowledgeGroupId uint32,
	enumerator KnowledgeMagicEnumerator) SwordMessage {
	return SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				KnowledgeMagicAction: &sword.KnowledgeMagicAction{
					KnowledgeGroup: MakeId(knowledgeGroupId),
					Type:           enumerator.Enum(),
					Parameters:     params,
				},
			},
		},
	}
}

func (c *Client) Teleport(tasker *sword.Tasker, location Point) error {
	return c.sendUnitMagicAction(tasker,
		MakeParameters(MakePointParam(location)),
		sword.UnitMagicAction_move_to)
}

type MagicEnumerator interface {
	Enum() *sword.MagicAction_Type
}

func CreateMagicAction(params *sword.MissionParameters,
	enumerator MagicEnumerator) SwordMessage {
	return SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type:       enumerator.Enum(),
					Parameters: params,
				},
			},
		},
	}
}

func (c *Client) DestroyLocalWeather(id uint32) error {
	params := MakeParameters()
	if id != 0 {
		params = MakeParameters(MakeIdentifier(id))
	}
	msg := CreateMagicAction(params, sword.MagicAction_local_weather_destruction)
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
			MakeEnumeration(int32(global.Precipitation)),
		)
	}
	msg := CreateMagicAction(params, sword.MagicAction_global_weather)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) CreateLocalWeather(local *LocalWeather) (*LocalWeather, error) {
	params := MakeParameters()
	if local != nil {
		params = MakeParameters(
			MakeFloat(local.Temperature),
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
	msg := CreateMagicAction(params, sword.MagicAction_local_weather)
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
	msg := CreateMagicAction(params, sword.MagicAction_change_diplomacy)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) ChangeDiplomacy(party1Id uint32, party2Id uint32, diplomacy sword.EnumDiplomacy) error {
	params := MakeParameters(
		MakeIdentifier(party1Id),
		MakeIdentifier(party2Id),
		MakeEnumeration(int32(diplomacy)),
	)
	return c.ChangeDiplomacyTest(params)
}

func (c *Client) CreateFireOnLocationTest(params *sword.MissionParameters) error {
	msg := CreateMagicAction(params, sword.MagicAction_create_fire_order_on_location)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) CreateFireOnLocation(location Point, ammoType uint32, salvoCount int) error {
	params := MakeParameters(
		MakePointParam(location),
		MakeResourceType(ammoType),
		MakeFloat(float32(salvoCount)),
	)
	return c.CreateFireOnLocationTest(params)
}

func (c *Client) ChangeResourceNetworkTest(params *sword.MissionParameters) error {
	msg := CreateMagicAction(params, sword.MagicAction_change_resource_network_properties)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) ChangeResourceNetwork(urban *Object) error {
	params := MakeParameters(nil, nil)
	params.Elem[0] = MakeIdentifier(urban.Id)
	resources := []*sword.MissionParameter_Value{}
	for _, r := range urban.Urban.ResourceNetworks {
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
	return c.sendUnitMagicAction(MakeCrowdTasker(crowdId),
		MakeParameters(), sword.UnitMagicAction_crowd_total_destruction)
}

func (c *Client) ChangeArmedIndividuals(crowdId uint32, armedIndividuals int32) error {
	params := MakeParameters()
	if armedIndividuals != 0 {
		params = MakeParameters(MakeQuantity(armedIndividuals))
	}
	return c.sendUnitMagicAction(MakeCrowdTasker(crowdId), params,
		sword.UnitMagicAction_crowd_change_armed_individuals)
}

func (c *Client) ChangeCriticalIntelligence(crowdId uint32, criticalIntelligence string) error {
	params := MakeParameters()
	if criticalIntelligence != "" {
		params = MakeParameters(MakeString(criticalIntelligence))
	}
	return c.sendUnitMagicAction(MakeCrowdTasker(crowdId), params,
		sword.UnitMagicAction_change_critical_intelligence)
}

func (c *Client) ChangeHealthState(crowdId uint32, healthy, wounded, contaminated, dead int32) error {
	params := MakeParameters(
		MakeQuantity(healthy),
		MakeQuantity(wounded),
		MakeQuantity(contaminated),
		MakeQuantity(dead),
	)
	return c.sendUnitMagicAction(MakeCrowdTasker(crowdId), params,
		sword.UnitMagicAction_crowd_change_health_state)
}

func (c *Client) changeAdhesions(tasker *sword.Tasker, action UnitMagicEnumerator,
	adhesions map[uint32]float32) error {
	params := MakeParameters()
	if len(adhesions) != 0 {
		params = MakeParameters(MakeAdhesions(adhesions))
	}
	return c.sendUnitMagicAction(tasker, params, action)
}

func (c *Client) ChangeCrowdAdhesions(crowdId uint32, adhesions map[uint32]float32) error {
	return c.changeAdhesions(MakeCrowdTasker(crowdId),
		sword.UnitMagicAction_crowd_change_affinities, adhesions)
}

func (c *Client) ChangeUnitAdhesions(unitId uint32, adhesions map[uint32]float32) error {
	return c.changeAdhesions(MakeUnitTasker(unitId),
		sword.UnitMagicAction_unit_change_affinities, adhesions)
}

func (c *Client) ReloadBrainTest(tasker *sword.Tasker, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(tasker, params, sword.UnitMagicAction_reload_brain)
}

func (c *Client) ReloadBrain(tasker *sword.Tasker, model string) error {
	params := MakeParameters()
	if model != "" {
		params = MakeParameters(MakeString(model))
	}
	return c.ReloadBrainTest(tasker, params)
}

func (c *Client) ChangeExtensions(crowdId uint32, extensions *map[string]string) error {
	params := MakeParameters()
	if extensions != nil {
		params = MakeParameters(MakeExtensions(extensions))
	}
	return c.sendUnitMagicAction(MakeCrowdTasker(crowdId), params,
		sword.UnitMagicAction_change_extension)
}

func (c *Client) ChangeAttitude(crowdId uint32, attitude int32) error {
	params := MakeParameters()
	if attitude != 0 {
		params = MakeParameters(MakeEnumeration(attitude))
	}
	return c.sendUnitMagicAction(MakeCrowdTasker(crowdId), params,
		sword.UnitMagicAction_crowd_change_attitude)
}

func (c *Client) LogisticsChangeLinks(tasker *sword.Tasker, superiors []uint32) error {
	params := []interface{}{}
	for _, s := range superiors {
		params = append(params, MakeIdentifier(s))
	}
	return c.sendUnitMagicAction(tasker,
		MakeParameters(params...),
		sword.UnitMagicAction_change_logistic_links)
}

func (c *Client) LogisticsSupplyChangeQuotas(supplierId uint32, supplied *sword.Tasker, quotas map[uint32]int32) error {
	params := []interface{}{MakeIdentifier(supplierId)}
	values := []*sword.MissionParameter_Value{}
	for dotation, qty := range quotas {
		values = append(values, MakeList(MakeIdentifier(dotation), MakeQuantity(qty)))
	}
	params = append(params, MakeParameter(values...))
	return c.sendUnitMagicAction(supplied,
		MakeParameters(params...),
		sword.UnitMagicAction_log_supply_change_quotas)
}

func (c *Client) LogisticsSupplyFlowTest(unitId uint32, params *sword.MissionParameters, action sword.UnitMagicAction_Type) ([]bool, error) {
	msg := CreateUnitMagicAction(MakeAutomatTasker(unitId), params, action)
	var result []bool
	handler := func(msg *sword.SimToClient_Content) error {
		reply, _, err := getUnitMagicActionAck(msg)
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			if err == nil {
				return invalid("result", reply.GetResult())
			}
			return err
		}
		for _, e := range value.GetList() {
			result = append(result, e.GetBooleanValue())
		}
		return err
	}
	err := <-c.postSimRequest(msg, handler)
	return result, err
}

func (c *Client) LogisticsSupplyPushFlowTest(supplierId uint32, params *sword.MissionParameters) ([]bool, error) {
	return c.LogisticsSupplyFlowTest(supplierId, params, sword.UnitMagicAction_log_supply_push_flow)
}

func (c *Client) LogisticsSupplyPushFlow(supplierId, recipientId uint32,
	supplies, equipments map[uint32]uint32) ([]bool, error) {

	recipient := &sword.SupplyFlowRecipient{
		Receiver:  MakeId(recipientId),
		Resources: MakeSupplyFlowResources(supplies),
	}
	recipients := []*sword.SupplyFlowRecipient{recipient}
	pushFlowParams := &sword.PushFlowParameters{
		Recipients:   recipients,
		Transporters: MakeSupplyFlowTransporters(equipments),
	}
	param := MakeParameter(&sword.MissionParameter_Value{PushFlowParameters: pushFlowParams})
	return c.LogisticsSupplyPushFlowTest(supplierId, MakeParameters(param))
}

func (c *Client) LogisticsSupplyPullFlowTest(receiverId uint32, params *sword.MissionParameters) ([]bool, error) {
	return c.LogisticsSupplyFlowTest(receiverId, params, sword.UnitMagicAction_log_supply_pull_flow)
}

func (c *Client) LogisticsSupplyPullFlow(receiverId, supplierId uint32,
	supplies, equipments map[uint32]uint32) ([]bool, error) {

	supplier := &sword.ParentEntity{Formation: MakeId(supplierId)}
	pullFlowParams := &sword.PullFlowParameters{
		Supplier:     supplier,
		Resources:    MakeSupplyFlowResources(supplies),
		Transporters: MakeSupplyFlowTransporters(equipments),
	}
	param := MakeParameter(&sword.MissionParameter_Value{PullFlowParameters: pullFlowParams})
	return c.LogisticsSupplyPullFlowTest(receiverId, MakeParameters(param))
}

func (c *Client) CreateKnowledgeGroupTest(params *sword.MissionParameters) (*KnowledgeGroup, error) {
	msg := CreateMagicAction(params, sword.MagicAction_create_knowledge_group)
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
		g, ok := data.KnowledgeGroups[id]
		if !ok {
			return false
		}
		group = g
		return true
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
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
		sword.UnitMagicAction_unit_change_superior)
}

func (c *Client) ChangeAutomatSuperior(automatId, formationId uint32) error {
	params := MakeParameters()
	if formationId != 0 {
		params = MakeParameters(MakeFormation(formationId))
	}
	return c.sendUnitMagicAction(MakeAutomatTasker(automatId), params,
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
	return c.sendUnitMagicAction(MakeFormationTasker(formationId), params,
		sword.UnitMagicAction_change_formation_superior)
}

func (c *Client) KnowledgeGroupMagicActionTest(actionType sword.KnowledgeMagicAction_Type, params *sword.MissionParameters, knowledgeGroupId uint32) error {
	msg := CreateKnowledgeMagicActionMessage(params, knowledgeGroupId, &actionType)
	return <-c.postSimRequest(msg, defaultKnowledgeGroupMagicHandler)
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

func (c *Client) addKnowledgeInKnowledgeGroup(groupId, entityId uint32,
	perceptionLevel int32) (uint32, error) {

	params := MakeParameters(MakeIdentifier(entityId), MakeEnumeration(perceptionLevel))
	msg := CreateKnowledgeMagicActionMessage(params, groupId,
		sword.KnowledgeMagicAction_add_knowledge)
	id := uint32(0)
	handler := func(msg *sword.SimToClient_Content) error {
		reply, _, err := getKnowledgeGroupMagicActionAck(msg)
		if err != nil {
			return err
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		id = value.GetIdentifier()
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return id, err
}

func (c *Client) CreateUnitKnowledge(groupId, entityId uint32,
	perceptionLevel int32) (*UnitKnowledge, error) {

	id, err := c.addKnowledgeInKnowledgeGroup(groupId, entityId, perceptionLevel)
	if err != nil {
		return nil, err
	}
	created := c.Model.GetUnitKnowledge(id)
	if created == nil {
		return nil, fmt.Errorf("created unit knowledge %d/%d is not available "+
			"after ack", groupId, id)
	}
	return created, nil
}

func (c *Client) CreateObjectKnowledge(groupId, entityId uint32,
	perceptionLevel int32) (*ObjectKnowledge, error) {

	id, err := c.addKnowledgeInKnowledgeGroup(groupId, entityId, perceptionLevel)
	if err != nil {
		return nil, err
	}
	created := c.Model.GetObjectKnowledge(id)
	if created == nil {
		return nil, fmt.Errorf("created object knowledge %d/%d is not available "+
			"after ack", groupId, id)
	}
	return created, nil
}

func (c *Client) CreateCrowdKnowledge(groupId, entityId uint32,
	perceptionLevel int32) (*CrowdKnowledge, error) {

	id, err := c.addKnowledgeInKnowledgeGroup(groupId, entityId, perceptionLevel)
	if err != nil {
		return nil, err
	}
	created := c.Model.GetCrowdKnowledge(id)
	if created == nil {
		return nil, fmt.Errorf("created crowd knowledge %d/%d is not available "+
			"after ack", groupId, id)
	}
	return created, nil
}

func (c *Client) ChangeKnowledgeGroupTest(automatId uint32,
	params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(MakeAutomatTasker(automatId), params,
		sword.UnitMagicAction_change_knowledge_group)
}

func (c *Client) ChangeKnowledgeGroup(automatId, knowledgeGroupId uint32) error {
	params := MakeParameters(MakeKnowledgeGroup(knowledgeGroupId))
	return c.ChangeKnowledgeGroupTest(automatId, params)
}

func (c *Client) DebugBrainTest(id uint32, params *sword.MissionParameters) error {
	// Abusing the super tolerant simulation tasker parser
	return c.sendUnitMagicAction(MakeUnitTasker(id), params,
		sword.UnitMagicAction_change_brain_debug)
}

func (c *Client) DebugBrain(automatId uint32, enable bool) error {
	params := MakeParameters(MakeBoolean(enable))
	return c.DebugBrainTest(automatId, params)
}

func (c *Client) TransferEquipment(unitId uint32, targetId uint32, equipments []Quantity) error {
	params := MakeParameters(MakeIdentifier(targetId), MakeQuantities(equipments))
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
		sword.UnitMagicAction_transfer_equipment)
}

func (c *Client) SurrenderTest(automatId uint32, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(MakeAutomatTasker(automatId), params,
		sword.UnitMagicAction_surrender_to)
}

func (c *Client) Surrender(automatId, partyId uint32) error {
	return c.SurrenderTest(automatId, MakeParameters(MakeParty(partyId)))
}

func (c *Client) CancelSurrender(automatId uint32) error {
	params := MakeParameters()
	return c.sendUnitMagicAction(MakeAutomatTasker(automatId), params,
		sword.UnitMagicAction_cancel_surrender)
}

func (c *Client) CreateWounds(unitId uint32, humans map[int32]int32) error {
	params := MakeParameters()
	if len(humans) != 0 {
		params = MakeParameters(EnumerateHumans(humans))
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
		sword.UnitMagicAction_create_wounds)
}

func (c *Client) ChangeHumanState(unitId uint32, humans []*Human) error {
	params := MakeParameters()
	if len(humans) != 0 {
		params = MakeParameters(MakeHumans(humans))
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
		sword.UnitMagicAction_change_human_state)
}

func (c *Client) RecoverAllHumans(unitId uint32, withLog bool) error {
	action := sword.UnitMagicAction_recover_troops
	if withLog {
		action = sword.UnitMagicAction_recover_troops_except_log
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), MakeParameters(), action)
}

func (c *Client) RecoverHumans(unitId uint32, humans []Quantity) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId),
		MakeParameters(nil, MakeQuantities(humans), nil, nil, nil),
		sword.UnitMagicAction_partial_recovery)
}

func (c *Client) ChangeResource(unitId uint32, resources map[uint32]*Resource) error {
	params := MakeParameters()
	if len(resources) != 0 {
		params = MakeParameters(MakeResources(resources))
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
		sword.UnitMagicAction_change_dotation)
}

func (c *Client) RecoverAllResources(unitId uint32, withLog bool) error {
	action := sword.UnitMagicAction_recover_resources
	if withLog {
		action = sword.UnitMagicAction_recover_resources_except_log
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), MakeParameters(), action)
}

func (c *Client) RecoverResources(unitId uint32, resources []Quantity) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId),
		MakeParameters(nil, nil, MakeQuantities(resources), nil, nil),
		sword.UnitMagicAction_partial_recovery)
}

func (c *Client) ChangeEquipmentState(unitId uint32, equipments map[uint32]*Equipment) error {
	params := MakeParameters()
	if len(equipments) != 0 {
		params = MakeParameters(MakeEquipment(equipments))
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
		sword.UnitMagicAction_change_equipment_state)
}

func (c *Client) RecoverAllEquipments(unitId uint32, withLog bool) error {
	action := sword.UnitMagicAction_recover_equipments
	if withLog {
		action = sword.UnitMagicAction_recover_equipments_except_log
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), MakeParameters(), action)
}

func (c *Client) RecoverEquipments(unitId uint32, equipments []Quantity) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId),
		MakeParameters(MakeQuantities(equipments), nil, nil, nil, nil),
		sword.UnitMagicAction_partial_recovery)
}

func (c *Client) RecoverUnit(unitId uint32, withLog bool) error {
	action := sword.UnitMagicAction_recover_all
	if withLog {
		action = sword.UnitMagicAction_recover_all_except_log
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), MakeParameters(), action)
}

func (c *Client) DestroyRandomEquipment(unitId uint32) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId),
		MakeParameters(), sword.UnitMagicAction_destroy_component)
}

func (c *Client) CreateBreakdowns(unitId uint32, equipments map[uint32]*Equipment) error {
	params := MakeParameters()
	if len(equipments) != 0 {
		params = MakeParameters(MakeBreakdowns(equipments))
	}
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
		sword.UnitMagicAction_create_breakdowns)
}

func (c *Client) ChangePostureTest(unitId uint32, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
		sword.UnitMagicAction_change_posture)
}

func (c *Client) ChangePosture(unitId uint32, posture sword.UnitAttributes_Posture) error {
	return c.ChangePostureTest(unitId, MakeParameters(MakeEnumeration(int32(posture))))
}

func (c *Client) ChangeHumanFactorsTest(unitId uint32, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), params,
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
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlToggleVisionCones: &sword.ControlEnableVisionCones{
					VisionCones: proto.Bool(enabled),
					Units:       MakeIdList(unitIds...).GetElem(),
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
					Start: MakeId(start),
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
func (c *Client) CreateCheckpoint(name string, sendState bool) (string, *ModelData, error) {

	var n *string
	if len(name) > 0 {
		n = proto.String(name)
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ControlCheckpointSaveNow: &sword.ControlCheckPointSaveNow{
					Name:      n,
					SendState: proto.Bool(sendState),
				},
			},
		},
	}

	var snapshot *ModelData
	var snapshotErr error
	if sendState {
		handlerId := c.Register(func(msg *SwordMessage, id, ctx int32, err error) bool {
			if err != nil {
				snapshotErr = err
				return true
			}
			if msg.SimulationToClient == nil {
				return false
			}
			m := msg.SimulationToClient.GetMessage()
			if m.GetControlSendCurrentStateBegin() != nil {
				snapshot = NewModelData()
				return false
			}
			if snapshot != nil {
				err := snapshot.update(msg)
				if err != nil {
					snapshotErr = err
					return true
				}
			}
			if m.GetControlSendCurrentStateEnd() != nil {
				return true
			}
			return false
		})
		defer c.Unregister(handlerId)
	}

	checkpoint := ""
	err := <-c.postSimRequest(msg, func(msg *sword.SimToClient_Content) error {
		reply := msg.GetControlCheckpointSaveNowAck()
		if reply == nil {
			if msg.GetControlSendCurrentStateBegin() != nil ||
				msg.GetControlSendCurrentStateEnd() != nil {
				return ErrContinue
			}
			return unexpected(msg)
		}
		if e := reply.GetErrorMsg(); len(e) > 0 {
			return fmt.Errorf("%s", e)
		}
		checkpoint = reply.GetName()
		return nil
	})
	if snapshotErr != nil {
		err = snapshotErr
	}
	if err == nil && sendState && snapshot == nil {
		err = fmt.Errorf("no snapshot received")
	}
	return checkpoint, snapshot, err
}

func (c *Client) RecoverTransporters(unitId uint32) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), MakeParameters(),
		sword.UnitMagicAction_recover_transporters)
}

func (c *Client) ExecScript(id uint32, function, script string) (string, error) {
	params := MakeParameters(
		MakeString(function),
		MakeString(script),
	)
	msg := CreateUnitMagicAction(MakeUnitTasker(id), params,
		sword.UnitMagicAction_exec_script)
	var result string
	handler := func(msg *sword.SimToClient_Content) error {
		reply, _, err := getUnitMagicActionAck(msg)
		if err != nil {
			return err
		}
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		result = value.GetACharStr()
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return result, err
}

// Substitutes parameters in script Go template and execute the script on
// supplied unit.
func (c *Client) ExecTemplate(id uint32, function, script string,
	params map[string]interface{}) (string, error) {

	w := &bytes.Buffer{}
	t, err := template.New("test").Parse(script)
	if err != nil {
		return "", fmt.Errorf("failed to parse template: %s", err)
	}
	err = t.Execute(w, params)
	if err != nil {
		return "", fmt.Errorf("failed to substitute params: %s", err)
	}
	text := string(w.Bytes())
	return c.ExecScript(id, function, text)
}

func (c *Client) ChangePopulationHealthState(populationId uint32, healthy, wounded,
	dead int32) error {
	params := MakeParameters(MakeQuantity(healthy),
		MakeQuantity(wounded),
		MakeQuantity(dead),
	)
	return c.sendUnitMagicAction(MakeUnitTasker(populationId), params,
		sword.UnitMagicAction_inhabitant_change_health_state)
}

func (c *Client) ChangePopulationAdhesions(populationId uint32, adhesions map[uint32]float32) error {
	params := MakeParameters()
	if len(adhesions) != 0 {
		params = MakeParameters(MakeAdhesions(adhesions))
	}
	return c.sendUnitMagicAction(MakeUnitTasker(populationId), params,
		sword.UnitMagicAction_inhabitant_change_affinities)
}

func (c *Client) CreateFireOrderOnUnit(taskerId uint32, targetKnowledgeId uint32, resourceTypeId uint32, interventions float32) error {
	params := MakeParameters(MakeIdentifier(targetKnowledgeId),
		MakeResourceType(resourceTypeId),
		MakeFloat(interventions))
	return c.sendUnitMagicAction(MakeUnitTasker(taskerId), params,
		sword.UnitMagicAction_create_fire_order)
}

func (c *Client) CreateDirectFireOrderOnUnit(taskerId uint32, targetId uint32) error {
	return c.sendUnitMagicAction(MakeUnitTasker(taskerId),
		MakeParameters(MakeAgent(targetId)),
		sword.UnitMagicAction_create_direct_fire_order)
}

func (c *Client) LoadUnitTest(tasker *sword.Tasker, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(tasker, params, sword.UnitMagicAction_load_unit)
}

func (c *Client) LoadUnit(loader, loadee uint32) error {
	return c.LoadUnitTest(MakeUnitTasker(loader), MakeParameters(MakeAgent(loadee)))
}

func (c *Client) UnloadUnitTest(tasker *sword.Tasker, params *sword.MissionParameters) error {
	return c.sendUnitMagicAction(tasker, params, sword.UnitMagicAction_unload_unit)
}

func (c *Client) UnloadUnit(loader, loadee uint32) error {
	return c.UnloadUnitTest(MakeUnitTasker(loader), MakeParameters(MakeAgent(loadee)))
}

func (c *Client) TriggerError(kind string) error {
	params := MakeParameters(MakeString("trigger_error"), MakeString(kind))
	msg := CreateMagicAction(params, sword.MagicAction_debug_internal)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) Echo(s string) (string, error) {
	params := MakeParameters(MakeString("echo"), MakeString(s))
	msg := CreateMagicAction(params, sword.MagicAction_debug_internal)
	echo := ""
	err := <-c.postSimRequest(msg, func(msg *sword.SimToClient_Content) error {
		err := defaultMagicHandler(msg)
		if err != nil {
			return err
		}
		reply := msg.GetMagicActionAck()
		value := GetParameterValue(reply.GetResult(), 0)
		if value == nil {
			return invalid("result", reply.GetResult())
		}
		echo = value.GetACharStr()
		return nil
	})
	return echo, err
}

type ObjectMagicEnumerator interface {
	Enum() *sword.ObjectMagicAction_Type
}

func CreateObjectMagicAction(objectId uint32, params *sword.MissionParameters,
	enumerator ObjectMagicEnumerator) SwordMessage {
	return SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ObjectMagicAction: &sword.ObjectMagicAction{
					Object:     MakeId(objectId),
					Type:       enumerator.Enum(),
					Parameters: params,
				},
			},
		},
	}
}

func (c *Client) CreateObject(objectType string, partyId uint32,
	location *sword.Location, attributes ...*sword.MissionParameter_Value) (*Object, error) {
	params := []interface{}{
		MakeString(objectType),
		MakeLocation(location),
		"name",
		MakeParty(partyId),
	}
	if len(attributes) != 0 {
		params = append(params, MakeParameter(attributes...))
	}
	msg := CreateObjectMagicAction(0, MakeParameters(params...),
		sword.ObjectMagicAction_create)
	var created *Object
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetObjectCreation(); reply != nil {
			// Context should not be set on this
			return ErrContinue
		}
		_, id, err := getObjectMagicActionAck(msg)
		if err != nil {
			return err
		}
		created = c.Model.GetObject(id)
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return created, err
}

func (c *Client) DeleteObject(objectId uint32) error {
	msg := CreateObjectMagicAction(objectId, MakeParameters(),
		sword.ObjectMagicAction_destroy)

	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetObjectDestruction(); reply != nil {
			return ErrContinue
		}
		_, id, err := getObjectMagicActionAck(msg)
		if err != nil {
			return err
		}
		if id != objectId {
			return mismatch("object id", objectId, id)
		}
		return nil
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) UpdateObject(objectId uint32, attributes ...*sword.MissionParameter_Value) error {
	msg := CreateObjectMagicAction(objectId, MakeParameters(MakeParameter(attributes...)),
		sword.ObjectMagicAction_update)
	handler := func(msg *sword.SimToClient_Content) error {
		if reply := msg.GetObjectUpdate(); reply != nil {
			return ErrContinue
		}
		_, id, err := getObjectMagicActionAck(msg)
		if err != nil {
			return err
		}
		if id != objectId {
			return mismatch("object id", objectId, id)
		}
		return nil
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) LogFinishHandlings(unitId uint32) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), MakeParameters(),
		sword.UnitMagicAction_log_finish_handlings)
}

func (c *Client) GetLogisticHistory(requestId ...uint32) ([]*sword.LogHistoryEntry, error) {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				LogisticHistoryRequest: makeLogisticHistoryRequest(requestId...),
			},
		},
	}
	var entries []*sword.LogHistoryEntry
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetLogisticHistoryAck()
		if reply == nil {
			return ErrContinue
		}
		entries = handleLogisticHistoryAck(reply)
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return entries, err
}

func (c *Client) ListLogisticRequests(currentTick, maxCount int, entityId ...uint32) (
	[]*sword.LogHistoryEntry, error) {

	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ListLogisticRequests: makeListLogisticRequests(currentTick, maxCount,
					entityId...),
			},
		},
	}
	var entries []*sword.LogHistoryEntry
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetListLogisticRequestsAck()
		if reply == nil {
			return ErrContinue
		}
		entries = handleListLogisticRequestsAck(reply)
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return entries, err
}

func (c *Client) SetManualMaintenanceTest(unitId uint32, parameters *sword.MissionParameters) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), parameters,
		sword.UnitMagicAction_log_maintenance_set_manual)
}

func (c *Client) SetManualMaintenance(unitId uint32, mode bool) error {
	return c.SetManualMaintenanceTest(unitId, MakeParameters(MakeBoolean(mode)))
}

func (c *Client) SetManualSupplyTest(unitId uint32, parameters *sword.MissionParameters) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId), parameters,
		sword.UnitMagicAction_log_supply_set_manual)
}

func (c *Client) SetManualSupply(unitId uint32, mode bool) error {
	return c.SetManualSupplyTest(unitId, MakeParameters(MakeBoolean(mode)))
}

func (c *Client) SelectNewLogisticStateTest(params *sword.MissionParameters) error {
	msg := CreateMagicAction(params, sword.MagicAction_select_new_logistic_state)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) SelectNewLogisticState(handlingId uint32) error {
	return c.SelectNewLogisticStateTest(MakeParameters(MakeIdentifier(handlingId)))
}

func (c *Client) TransferToLogisticSuperior(handlingId uint32) error {
	msg := CreateMagicAction(MakeParameters(MakeIdentifier(handlingId)), sword.MagicAction_transfer_to_logistic_superior)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) SelectMaintenanceTransporterTest(params *sword.MissionParameters) error {
	msg := CreateMagicAction(params,
		sword.MagicAction_select_maintenance_transporter)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) SelectMaintenanceTransporter(handlingId, equipmentId uint32) error {
	return c.SelectMaintenanceTransporterTest(MakeParameters(MakeIdentifier(handlingId),
		MakeIdentifier(equipmentId)))
}

func (c *Client) SelectDiagnosisTeamTest(params *sword.MissionParameters) error {
	msg := CreateMagicAction(params, sword.MagicAction_select_diagnosis_team)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) SelectDiagnosisTeam(handlingId, equipmentId uint32) error {
	return c.SelectDiagnosisTeamTest(
		MakeParameters(MakeIdentifier(handlingId), MakeIdentifier(equipmentId)))
}

func (c *Client) SelectRepairTeamTest(params *sword.MissionParameters) error {
	msg := CreateMagicAction(params, sword.MagicAction_select_repair_team)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) SelectRepairTeam(handlingId, equipmentId uint32) error {
	return c.SelectRepairTeamTest(
		MakeParameters(MakeIdentifier(handlingId), MakeIdentifier(equipmentId)))
}

func (c *Client) RecoverStocks(unitId uint32, resources map[uint32]*Resource) error {
	return c.sendUnitMagicAction(MakeUnitTasker(unitId),
		MakeParameters(nil, nil, nil, nil, MakeStocks(resources)),
		sword.UnitMagicAction_partial_recovery)
}

func ReadPathPoints(pathPoints []*sword.PathPoint) []PathPoint {
	points := []PathPoint{}
	for _, v := range pathPoints {
		point := NewPathPoint(ReadPoint(v.GetCoordinate()))
		if waypoint := v.Waypoint; waypoint != nil {
			point.Waypoint = *waypoint
		}
		if reached := v.Reached; reached != nil {
			point.Reached = *reached
		}
		points = append(points, point)
	}
	return points
}

func (c *Client) PathfindRequest(request *sword.PathfindRequest, position ...Point) ([]PathPoint, error) {
	request.Positions = make([]*sword.CoordLatLong, len(position))
	for i, p := range position {
		request.Positions[i] = MakeCoordLatLong(p)
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				ComputePathfind: &sword.ComputePathfind{Request: request},
			},
		},
	}
	var points []PathPoint
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetComputePathfindAck()
		if reply == nil {
			return ErrContinue
		}
		code := reply.GetErrorCode()
		if code != sword.ComputePathfindAck_no_error {
			return makeError(reply, int32(code), sword.ComputePathfindAck_ErrorCode_name)
		}
		points = ReadPathPoints(reply.GetPath().GetPoints())
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return points, err
}

func (c *Client) UnitPathfindRequest(unitId uint32, position ...Point) ([]PathPoint, error) {
	return c.PathfindRequest(
		&sword.PathfindRequest{
			Unit: MakeId(unitId),
		}, position...)
}

func (c *Client) EquipmentListPathfindRequest(equipment uint32, position ...Point) ([]PathPoint, error) {
	var eq []*sword.Id
	eq = append(eq, &sword.Id{Id: proto.Uint32(equipment)})
	return c.PathfindRequest(
		&sword.PathfindRequest{
			Unit:           MakeId(0),
			EquipmentTypes: eq,
		}, position...)
}

func (c *Client) SegmentRequest(position Point, terrains []sword.TerrainType, count uint32) ([]Segment, error) {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				SegmentRequest: &sword.SegmentRequest{
					Position: MakeCoordLatLong(position),
					Terrains: terrains,
					Count:    proto.Uint32(count),
				},
			},
		},
	}
	segments := []Segment{}
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetSegmentRequestAck()
		if reply == nil {
			return ErrContinue
		}
		code := reply.GetErrorCode()
		for _, s := range reply.GetSegments() {
			segments = append(segments,
				Segment{
					From: ReadPoint(s.GetFrom()),
					To:   ReadPoint(s.GetTo()),
				})
		}
		if code != sword.SegmentRequestAck_no_error {
			return makeError(reply, int32(code), sword.SegmentRequestAck_ErrorCode_name)
		}
		return nil
	}
	err := <-c.postSimRequest(msg, handler)
	return segments, err
}

func (c *Client) PathfindTest(params *sword.MissionParameters, action sword.MagicAction_Type) (uint32, error) {
	msg := CreateMagicAction(params, action)
	var id uint32
	handlers := func(msg *sword.SimToClient_Content) error {
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
	err := <-c.postSimRequest(msg, handlers)
	return id, err
}

func (c *Client) CreatePathfindTest(unitId uint32, ignoreDynamicObjects bool, points ...Point) (*Pathfind, error) {
	id, err := c.PathfindTest(MakeParameters(MakePathfindRequest(unitId, ignoreDynamicObjects, points...)),
		sword.MagicAction_pathfind_creation)
	if err != nil {
		return nil, err
	}
	var result *Pathfind
	ok := c.Model.WaitCondition(func(data *ModelData) bool {
		pathfind := data.Pathfinds[id]
		if pathfind == nil {
			return false
		}
		result = pathfind
		return true
	})
	if !ok {
		return nil, ErrNotFound
	}
	return result, err
}

func (c *Client) CreatePathfind(unitId uint32, points ...Point) (*Pathfind, error) {
	return c.CreatePathfindTest(unitId, true, points...)
}

func (c *Client) DestroyPathfind(pathfindId uint32) error {
	msg := CreateMagicAction(MakeParameters(
		MakeIdentifier(pathfindId)),
		sword.MagicAction_pathfind_destruction)
	return <-c.postSimRequest(msg, defaultMagicHandler)
}

func (c *Client) CreateBasicLoadSupplyRequests(taskerId, supplierId, recipentId uint32, resources []Quantity) error {
	return c.sendUnitMagicAction(MakeUnitTasker(taskerId),
		MakeParameters(MakeIdentifier(supplierId), MakeIdentifier(recipentId), MakeQuantities(resources)),
		sword.UnitMagicAction_create_basic_load_supply_request)
}

func (c *Client) CreateStockSupplyRequests(taskerId, supplierId, recipentId uint32, resources []Quantity) error {
	return c.sendUnitMagicAction(MakeUnitTasker(taskerId),
		MakeParameters(MakeIdentifier(supplierId), MakeIdentifier(recipentId), MakeQuantities(resources)),
		sword.UnitMagicAction_create_stock_supply_request)
}
