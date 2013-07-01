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
)

func mismatch(name string, a, b interface{}) error {
	return fmt.Errorf("%v mismatch %v != %v", name, a, b)
}

func invalid(name string, value interface{}) error {
	return fmt.Errorf("invalid %v: %v", name, value)
}

func makeUnitTasker(unitId uint32) *sword.Tasker {
	return &sword.Tasker{
		Unit: &sword.UnitId{
			Id: proto.Uint32(unitId),
		},
	}
}

func makeCrowdTasker(crowdId uint32) *sword.Tasker {
	return &sword.Tasker{
		Crowd: &sword.CrowdId{
			Id: proto.Uint32(crowdId),
		},
	}
}

func GetUnitMagicActionAck(msg *sword.UnitMagicActionAck) (uint32, error) {
	// Wait for the final UnitMagicActionAck
	code := msg.GetErrorCode()
	if code == sword.UnitActionAck_no_error {
		id := msg.GetUnit().GetId()
		return id, nil
	}
	return 0, nameof(sword.UnitActionAck_ErrorCode_name, int32(code))
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
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker: tasker,
					Type:   sword.UnitMagicAction_formation_creation.Enum(),
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

func (c *Client) createUnit(automatId, unitType uint32, location Point,
	name *string, pc *bool) (*Unit, error) {
	tasker := &sword.Tasker{}
	if automatId != 0 {
		tasker.Automat = &sword.AutomatId{
			Id: proto.Uint32(automatId),
		}
	}
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
					Tasker:     tasker,
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

func orderAckHandler(msg *sword.SimToClient_Content) error {
	reply := msg.GetOrderAck()
	if reply == nil {
		return unexpected(msg)
	}
	code := reply.GetErrorCode()
	if code != sword.OrderAck_no_error {
		return nameof(sword.OrderAck_ErrorCode_name, int32(code))
	}
	return nil
}

func (c *Client) SendUnitOrder(unitId, missionType uint32,
	params *sword.MissionParameters) error {
	msg := SwordMessage{
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
	}
	handler := orderAckHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) SendAutomatOrder(unitId, missionType uint32,
	params *sword.MissionParameters) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				AutomatOrder: &sword.AutomatOrder{
					Tasker: &sword.AutomatId{
						Id: proto.Uint32(unitId),
					},
					Type: &sword.MissionType{
						Id: proto.Uint32(missionType),
					},
					Parameters: params,
				},
			},
		},
	}
	handler := orderAckHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) DeleteUnit(unitId uint32) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     makeUnitTasker(unitId),
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
	tasker := &sword.Tasker{}
	if automatId != 0 {
		tasker.Automat = &sword.AutomatId{
			Id: proto.Uint32(automatId),
		}
	}
	if formationId != 0 {
		tasker.Formation = &sword.FormationId{
			Id: proto.Uint32(formationId),
		}
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker: tasker,
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
	tasker := &sword.Tasker{}
	if partyId != 0 {
		tasker.Party = &sword.PartyId{
			Id: proto.Uint32(partyId),
		}
	}
	if formationId != 0 {
		tasker.Formation = &sword.FormationId{
			Id: proto.Uint32(formationId),
		}
	}

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
					Tasker:     tasker,
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

func (c *Client) SendCrowdOrder(unitId, missionType uint32,
	params *sword.MissionParameters) error {
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				CrowdOrder: &sword.CrowdOrder{
					Tasker: &sword.CrowdId{
						Id: proto.Uint32(unitId),
					},
					Type: &sword.MissionType{
						Id: proto.Uint32(missionType),
					},
					Parameters: params,
				},
			},
		},
	}
	handler := orderAckHandler
	return <-c.postSimRequest(msg, handler)
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
			return nameof(sword.SetAutomatModeAck_ErrorCode_name, int32(code))
		}
		return nil
	}
	return <-c.postSimRequest(msg, handler)
}

func getControlAckError(code sword.ControlAck_ErrorCode) error {
	if code == sword.ControlAck_no_error {
		return nil
	}
	return nameof(sword.ControlAck_ErrorCode_name, int32(code))
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
		return getControlAckError(reply.GetErrorCode())
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
		return getControlAckError(reply.GetErrorCode())
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
		return getControlAckError(reply.GetErrorCode())
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) SendFragOrder(automatId, crowdId, unitId, fragOrderType uint32,
	params *sword.MissionParameters) error {
	tasker := &sword.Tasker{}
	if automatId != 0 {
		tasker.Automat = &sword.AutomatId{
			Id: proto.Uint32(automatId),
		}
	} else if crowdId != 0 {
		tasker.Crowd = &sword.CrowdId{
			Id: proto.Uint32(crowdId),
		}
	} else if unitId != 0 {
		tasker.Unit = &sword.UnitId{
			Id: proto.Uint32(unitId),
		}
	}
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				FragOrder: &sword.FragOrder{
					Tasker: tasker,
					Type: &sword.FragOrderType{
						Id: proto.Uint32(fragOrderType),
					},
					Parameters: params,
				},
			},
		},
	}
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetFragOrderAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.OrderAck_no_error {
			return nameof(sword.OrderAck_ErrorCode_name, int32(code))
		}
		return nil
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) SendAutomatFragOrder(unitId, fragOrderType uint32,
	params *sword.MissionParameters) error {
	return c.SendFragOrder(unitId, 0, 0, fragOrderType, params)
}

func (c *Client) SendCrowdFragOrder(unitId, fragOrderType uint32,
	params *sword.MissionParameters) error {
	return c.SendFragOrder(0, unitId, 0, fragOrderType, params)
}

func (c *Client) SendUnitFragOrder(unitId, fragOrderType uint32,
	params *sword.MissionParameters) error {
	return c.SendFragOrder(0, 0, unitId, fragOrderType, params)
}

func defaultUnitMagicHandler(msg *sword.SimToClient_Content) error {
	reply := msg.GetUnitMagicActionAck()
	if reply == nil {
		return unexpected(msg)
	}
	_, err := GetUnitMagicActionAck(reply)
	if err != nil {
		return err
	}
	return nil
}

func createMagicActionMessage(params *sword.MissionParameters, tasker *sword.Tasker,
	magicAction *sword.UnitMagicAction_Type) SwordMessage {
	return SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				UnitMagicAction: &sword.UnitMagicAction{
					Tasker:     tasker,
					Type:       magicAction,
					Parameters: params,
				},
			},
		},
	}
}

func (c *Client) TeleportUnit(unitId uint32, location Point) error {
	msg := createMagicActionMessage(MakeParameters(MakePointParam(location)), makeUnitTasker(unitId),
		sword.UnitMagicAction_move_to.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) TeleportCrowd(crowdId uint32, location Point) error {
	msg := createMagicActionMessage(MakeParameters(MakePointParam(location)), makeCrowdTasker(crowdId),
		sword.UnitMagicAction_move_to.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) CreateLocalWeather(local *LocalWeather) (*LocalWeather, error) {
	// FIXME lighting is ignored because it's completely broken
	msg := SwordMessage{
		ClientToSimulation: &sword.ClientToSim{
			Message: &sword.ClientToSim_Content{
				MagicAction: &sword.MagicAction{
					Type: sword.MagicAction_local_weather.Enum(),
					Parameters: MakeParameters(
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
					),
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
			return nameof(sword.MagicActionAck_ErrorCode_name, int32(code))
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
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetActionCreateFireOrderAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.ActionCreateFireOrderAck_no_error {
			return nameof(sword.ActionCreateFireOrderAck_ErrorCode_name, int32(code))
		}
		return nil
	}
	return <-c.postSimRequest(msg, handler)
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
	handler := func(msg *sword.SimToClient_Content) error {
		reply := msg.GetMagicActionAck()
		if reply == nil {
			return unexpected(msg)
		}
		code := reply.GetErrorCode()
		if code != sword.MagicActionAck_no_error {
			return nameof(sword.MagicActionAck_ErrorCode_name, int32(code))
		}
		return nil
	}
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) SendTotalDestruction(crowdId uint32) error {
	msg := createMagicActionMessage(MakeParameters(), makeCrowdTasker(crowdId),
		sword.UnitMagicAction_crowd_total_destruction.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) ChangeArmedIndividuals(crowdId uint32, armedIndividuals int32) error {
	params := MakeParameters()
	if armedIndividuals != 0 {
		params = MakeParameters(MakeQuantity(armedIndividuals))
	}
	msg := createMagicActionMessage(params, makeCrowdTasker(crowdId),
		sword.UnitMagicAction_crowd_change_armed_individuals.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) ChangeCriticalIntelligence(crowdId uint32, criticalIntelligence string) error {
	params := MakeParameters()
	if criticalIntelligence != "" {
		params = MakeParameters(MakeString(criticalIntelligence))
	}
	msg := createMagicActionMessage(params, makeCrowdTasker(crowdId),
		sword.UnitMagicAction_change_critical_intelligence.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) ChangeHealthState(crowdId uint32, healthy, wounded, contaminated, dead int32) error {
	params := MakeParameters(MakeQuantity(healthy),
		MakeQuantity(wounded),
		MakeQuantity(contaminated),
		MakeQuantity(dead))
	msg := createMagicActionMessage(params, makeCrowdTasker(crowdId),
		sword.UnitMagicAction_crowd_change_health_state.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) ChangeAdhesions(crowdId uint32, adhesions map[uint32]float32) error {
	params := MakeParameters()
	if len(adhesions) != 0 {
		params = MakeParameters(MakeAdhesions(adhesions))
	}
	msg := createMagicActionMessage(params, makeCrowdTasker(crowdId),
		sword.UnitMagicAction_crowd_change_affinities.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) ReloadBrain(crowdId uint32, model string) error {
	params := MakeParameters()
	if model != "" {
		params = MakeParameters(MakeString(model))
	}
	msg := createMagicActionMessage(params, makeCrowdTasker(crowdId),
		sword.UnitMagicAction_reload_brain.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) ChangeExtensions(crowdId uint32, extensions *map[string]string) error {
	params := MakeParameters()
	if extensions != nil {
		params = MakeParameters(MakeExtensions(extensions))
	}
	msg := createMagicActionMessage(params, makeCrowdTasker(crowdId),
		sword.UnitMagicAction_change_extension.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}

func (c *Client) ChangeAttitude(crowdId uint32, attitude int32) error {
	params := MakeParameters()
	if attitude != 0 {
		params = MakeParameters(MakeEnumeration(attitude))
	}
	msg := createMagicActionMessage(params, makeCrowdTasker(crowdId),
		sword.UnitMagicAction_crowd_change_attitude.Enum())
	handler := defaultUnitMagicHandler
	return <-c.postSimRequest(msg, handler)
}
