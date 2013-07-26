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
	"errors"
	"fmt"
	"sword"
	"time"
)

var (
	ErrInvalidTime = errors.New("invalid time")
)

type modelCmd struct {
	done chan int
	cmd  func(model *Model)
}

type modelCond struct {
	id   int
	done chan int
	cond func(model *Model) bool
}

type ModelRequest struct {
	Message *SwordMessage
	Command *modelCmd
	Cond    *modelCond
}

type ModelErrorHandler func(*ModelData, *SwordMessage, error) error

// The model registers a handler to a Client and updates itself with
// incoming messages in a separate goroutine. It supports two kind of
// actions: commands which are just functions applied once on the Model,
// mostly used to implement queries, and conditions which are fired each
// time a message is processed until they return true. Messages, commands and
// conditions are deliberately serialized so we know that commands issued from
// a message handler will have a model update with all previous messages.
//
// Note that accessor will return either immutable entities or copies of
// such entities (mostly collections) so clients can manipulate the
// result freely.
type Model struct {
	// State only touched by the run() goroutine
	// True once the initial state has been received
	ready        bool
	conds        []*modelCond
	data         *ModelData
	errorHandler ModelErrorHandler

	// State only touched by caller routines
	condId      int
	WaitTimeout time.Duration

	// Shared state
	requests chan *ModelRequest
}

func NewModel() *Model {
	model := Model{
		ready:       false,
		condId:      0,
		WaitTimeout: 60 * time.Second,
		requests:    make(chan *ModelRequest, 128),
		conds:       []*modelCond{},
		data:        NewModelData(),
	}
	model.setErrorHandler(nil)

	go model.run()
	return &model
}

func (model *Model) setErrorHandler(errorHandler ModelErrorHandler) {
	if errorHandler == nil {
		errorHandler = func(data *ModelData, msg *SwordMessage, err error) error {
			panic(err)
		}
	}
	model.errorHandler = errorHandler
}

// Reconfigure the model error handler. A nil argument restore the default
// panicking handler. The handler is called upon update errors with the current
// ModelData, the input message and the error instance. Return nil to tell the
// error was handled and the model correctly updated, an error otherwise. Upon
// error, all the conditions will be terminated on error and model goroutine
// terminated.
func (model *Model) SetErrorHandler(errorHandler ModelErrorHandler) {
	model.waitCommand(func(m *Model) {
		model.setErrorHandler(errorHandler)
	})
}

func (model *Model) run() error {
	for rq := range model.requests {
		if rq.Message != nil {
			if err := model.update(rq.Message); err != nil {
				err = model.errorHandler(model.data, rq.Message, err)
				if err != nil {
					for _, cond := range model.conds {
						cond.done <- 0
					}
					return err
				}
			}
			for i := 0; i != len(model.conds); {
				if model.conds[i].cond(model) {
					model.conds[i].done <- 1
					model.conds = append(model.conds[:i], model.conds[i+1:]...)
					continue
				}
				i++
			}
		} else if rq.Command != nil {
			rq.Command.cmd(model)
			rq.Command.done <- 1
		} else if rq.Cond != nil {
			if rq.Cond.cond(model) {
				rq.Cond.done <- 1
			} else {
				model.conds = append(model.conds, rq.Cond)
			}
		}
	}
	return nil
}

func (model *Model) update(msg *SwordMessage) error {
	d := model.data
	if msg.SimulationToClient != nil {
		m := msg.SimulationToClient.GetMessage()
		if m.GetControlSendCurrentStateEnd() != nil {
			model.ready = true
		} else if mm := m.GetControlBeginTick(); mm != nil {
			t, err := GetTime(mm.GetDateTime())
			if err != nil {
				return err
			}
			d.Time = t
			d.Tick = mm.GetCurrentTick()
		} else if mm := m.GetPartyCreation(); mm != nil {
			party := NewParty(
				mm.GetParty().GetId(),
				mm.GetName())
			d.Parties[party.Id] = party
		} else if mm := m.GetUnitCreation(); mm != nil {
			unit := &Unit{
				Id:                 mm.GetUnit().GetId(),
				AutomatId:          mm.GetAutomat().GetId(),
				Name:               mm.GetName(),
				Pc:                 mm.GetPc(),
				Position:           Point{},
				PathPoints:         0,
				DebugBrain:         false,
				EquipmentDotations: map[uint32]*EquipmentDotation{},
				LentEquipments:     []*LentEquipment{},
				BorrowedEquipments: []*BorrowedEquipment{}}

			if !d.addUnit(unit) {
				return fmt.Errorf("cannot insert created unit: %d", unit.Id)
			}
		} else if mm := m.GetUnitAttributes(); mm != nil {
			unit := d.FindUnit(mm.GetUnit().GetId())
			if unit == nil {
				return fmt.Errorf("cannot find unit to update: %d",
					mm.GetUnit().GetId())
			}
			if mm.Headquarters != nil {
				unit.Pc = *mm.Headquarters
			}
			if mm.Position != nil {
				unit.Position = ReadPoint(mm.GetPosition())
			}
			if mm.BrainDebug != nil {
				unit.DebugBrain = *mm.BrainDebug
			}
			if dotations := mm.GetEquipmentDotations(); dotations != nil {
				for _, dotation := range dotations.GetElem() {
					unit.EquipmentDotations[dotation.GetType().GetId()] = &EquipmentDotation{dotation.GetAvailable()}
				}
			}
			if lentEquipments := mm.GetLentEquipments(); lentEquipments != nil {
				unit.LentEquipments = []*LentEquipment{}
				for _, equipment := range lentEquipments.GetElem() {
					unit.LentEquipments = append(unit.LentEquipments, &LentEquipment{equipment.GetBorrower().GetId(),
						equipment.GetType().GetId(), equipment.GetQuantity()})
				}
			}
			if borrowedEquipments := mm.GetBorrowedEquipments(); borrowedEquipments != nil {
				unit.BorrowedEquipments = []*BorrowedEquipment{}
				for _, equipment := range borrowedEquipments.GetElem() {
					unit.BorrowedEquipments = append(unit.BorrowedEquipments, &BorrowedEquipment{equipment.GetOwner().GetId(),
						equipment.GetType().GetId(), equipment.GetQuantity()})
				}
			}
		} else if mm := m.GetAutomatCreation(); mm != nil {
			automat := NewAutomat(
				mm.GetAutomat().GetId(),
				mm.GetParty().GetId(),
				mm.GetKnowledgeGroup().GetId(),
				mm.GetName())
			automatId, formationId := uint32(0), uint32(0)
			if parent := mm.GetParent().GetAutomat(); parent != nil {
				automatId = parent.GetId()
			} else if parent := mm.GetParent().GetFormation(); parent != nil {
				formationId = parent.GetId()
			}
			if !d.addAutomat(automatId, formationId, automat) {
				return fmt.Errorf("cannot insert created automat: %d", automat.Id)
			}
		} else if mm := m.GetAutomatAttributes(); mm != nil {
			automat := d.FindAutomat(mm.GetAutomat().GetId())
			if automat == nil {
				return fmt.Errorf("cannot find automat to update: %d",
					mm.GetAutomat().GetId())
			}
			if mm.Mode != nil {
				mode := mm.GetMode()
				if mode == sword.EnumAutomatMode_engaged {
					automat.Engaged = true
				} else if mode == sword.EnumAutomatMode_disengaged {
					automat.Engaged = false
				}
			}
			if mm.BrainDebug != nil {
				automat.DebugBrain = mm.GetBrainDebug()
			}
		} else if mm := m.GetFormationCreation(); mm != nil {
			level, ok := sword.EnumNatureLevel_name[int32(mm.GetLevel())]
			if !ok {
				level = "unknown"
			}
			logLevel, ok := sword.EnumLogisticLevel_name[int32(mm.GetLogisticLevel())]
			if !ok {
				logLevel = "unknown"
			}
			formation := NewFormation(
				mm.GetFormation().GetId(),
				mm.GetName(),
				mm.GetParent().GetId(),
				mm.GetParty().GetId(),
				level, logLevel)
			if !d.addFormation(formation) {
				return fmt.Errorf("cannot create formation %v with unknown"+
					"parent party=%v/parent=%v", formation.Id, formation.PartyId,
					formation.ParentId)
			}
		} else if mm := m.GetCrowdCreation(); mm != nil {
			crowd := NewCrowd(
				mm.GetCrowd().GetId(),
				mm.GetParty().GetId(),
				mm.GetName())
			if !d.addCrowd(crowd) {
				return fmt.Errorf("cannot insert created crowd: %d", crowd.Id)
			}
		} else if mm := m.GetCrowdUpdate(); mm != nil {
			crowd := d.FindCrowd(mm.GetCrowd().GetId())
			if crowd == nil {
				return fmt.Errorf("cannot find crowd to update: %d",
					mm.GetCrowd().GetId())
			}
			if mm.CriticalIntelligence != nil {
				crowd.CriticalIntelligence = *mm.CriticalIntelligence
			}
			if mm.Healthy != nil {
				crowd.Healthy = *mm.Healthy
			}
			if mm.Wounded != nil {
				crowd.Wounded = *mm.Wounded
			}
			if mm.Dead != nil {
				crowd.Dead = *mm.Dead
			}
			if mm.Contaminated != nil {
				crowd.Contaminated = *mm.Contaminated
			}
			if mm.ArmedIndividuals != nil {
				crowd.ArmedIndividuals = *mm.ArmedIndividuals
			}
			if mm.Adhesions != nil {
				crowd.Adhesions = map[uint32]float32{}
				for _, value := range mm.Adhesions.Adhesion {
					crowd.Adhesions[value.GetParty().GetId()] = value.GetValue()
				}
			}
			if mm.Extension != nil {
				for i := 0; i < len(mm.Extension.Entries); i++ {
					entry := mm.Extension.Entries[i]
					crowd.Extensions[entry.GetName()] = entry.GetValue()
				}
			}
		} else if mm := m.GetCrowdFlowCreation(); mm != nil {
			if !d.addCrowdElement(mm.GetCrowd().GetId(), mm.GetFlow().GetId()) {
				return fmt.Errorf("cannot insert crowd flow %d into crowd %d",
					mm.GetFlow().GetId(), mm.GetCrowd().GetId())
			}
		} else if mm := m.GetCrowdFlowDestruction(); mm != nil {
			if !d.removeCrowdElement(mm.GetCrowd().GetId(), mm.GetFlow().GetId()) {
				return fmt.Errorf("cannot remove crowd flow %d from crowd %d",
					mm.GetFlow().GetId(), mm.GetCrowd().GetId())
			}
		} else if mm := m.GetCrowdFlowUpdate(); mm != nil {
			crowd := d.FindCrowd(mm.GetCrowd().GetId())
			if crowd == nil {
				return fmt.Errorf("cannot find crowd to update: %d",
					mm.GetCrowd().GetId())
			}
			element := crowd.CrowdElements[mm.GetFlow().GetId()]
			if element == nil {
				return fmt.Errorf("cannot find crowd flow to update: %d",
					mm.GetFlow().GetId())
			}
			element.Attitude = int32(mm.GetAttitude())
		} else if mm := m.GetCrowdConcentrationCreation(); mm != nil {
			if !d.addCrowdElement(mm.GetCrowd().GetId(), mm.GetConcentration().GetId()) {
				return fmt.Errorf("cannot insert crowd concentration %d into crowd %d",
					mm.GetConcentration().GetId(), mm.GetCrowd().GetId())
			}
		} else if mm := m.GetCrowdConcentrationDestruction(); mm != nil {
			if !d.removeCrowdElement(mm.GetCrowd().GetId(), mm.GetConcentration().GetId()) {
				return fmt.Errorf("cannot remove crowd concentration %d from crowd %d",
					mm.GetConcentration().GetId(), mm.GetCrowd().GetId())
			}
		} else if mm := m.GetCrowdConcentrationUpdate(); mm != nil {
			crowd := d.FindCrowd(mm.GetCrowd().GetId())
			if crowd == nil {
				return fmt.Errorf("cannot find crowd to update: %d",
					mm.GetCrowd().GetId())
			}
			element := crowd.CrowdElements[mm.GetConcentration().GetId()]
			if element == nil {
				return fmt.Errorf("cannot find crowd concentration to update: %d",
					mm.GetConcentration().GetId())
			}
			element.Attitude = int32(mm.GetAttitude())
		} else if mm := m.GetPopulationCreation(); mm != nil {
			population := &Population{
				mm.GetId().GetId(),
				mm.GetParty().GetId(),
				mm.GetName()}
			if !d.addPopulation(population) {
				return fmt.Errorf("cannot insert population: %d", population.Id)
			}
		} else if mm := m.GetUnitPathfind(); mm != nil {
			unit := d.FindUnit(mm.GetUnit().GetId())
			if unit == nil {
				return fmt.Errorf("cannot find pathfind source unit: %d",
					mm.GetUnit().GetId())
			}
			if mm.Path != nil && mm.Path.Location != nil {
				unit.PathPoints = uint32(0)
				if mm.Path.Location.Coordinates != nil {
					unit.PathPoints = uint32(len(mm.Path.Location.Coordinates.Elem))
				}
			}
		} else if mm := m.GetUnitDestruction(); mm != nil {
			if !d.removeUnit(mm.GetUnit().GetId()) {
				return fmt.Errorf("cannot find unit to destroy: %d",
					mm.GetUnit().GetId())
			}
		} else if mm := m.GetKnowledgeGroupCreation(); mm != nil {
			group := &KnowledgeGroup{
				Id:                  mm.GetKnowledgeGroup().GetId(),
				Name:                mm.GetName(),
				PartyId:             mm.GetParty().GetId(),
				ParentId:            mm.GetParent().GetId(),
				Type:                mm.GetType(),
				IsCrowdDefaultGroup: mm.GetCrowd(),
				Enabled:             true,
				UnitKnowledges:      map[uint32]*UnitKnowledge{},
				ObjectKnowledges:    map[uint32]*ObjectKnowledge{},
				CrowdKnowledges:     map[uint32]*CrowdKnowledge{},
			}
			if !d.addKnowledgeGroup(group) {
				return fmt.Errorf("cannot insert knowledge group: %d", group.Id)
			}
		} else if mm := m.GetUnitKnowledgeCreation(); mm != nil {
			knowledge := &UnitKnowledge{
				Id:               mm.GetKnowledge().GetId(),
				KnowledgeGroupId: mm.GetKnowledgeGroup().GetId(),
				UnitId:           mm.GetUnit().GetId(),
				UnitType:         mm.GetType().GetId(),
			}
			if !d.addUnitKnowledge(knowledge) {
				return fmt.Errorf("cannot insert unit knowledge %d into group %d",
					knowledge.Id, knowledge.KnowledgeGroupId)
			}
		} else if mm := m.GetObjectKnowledgeCreation(); mm != nil {
			knowledge := &ObjectKnowledge{
				Id:               mm.GetKnowledge().GetId(),
				PartyId:          mm.GetParty().GetId(),
				ObjectId:         mm.GetObject().GetId(),
				ObjectType:       mm.GetType().GetId(),
				KnowledgeGroupId: mm.GetKnowledgeGroup().GetId(),
				ObjectPartyId:    mm.GetObjectParty().GetId(),
				ObjectName:       mm.GetObjectName(),
			}
			if !d.addObjectKnowledge(knowledge) {
				return fmt.Errorf("cannot insert object knowledge %d into group %d",
					knowledge.Id, knowledge.KnowledgeGroupId)
			}
		} else if mm := m.GetCrowdKnowledgeCreation(); mm != nil {
			knowledge := &CrowdKnowledge{
				Id:               mm.GetKnowledge().GetId(),
				KnowledgeGroupId: mm.GetKnowledgeGroup().GetId(),
				CrowdId:          mm.GetCrowd().GetId(),
				PartyId:          mm.GetParty().GetId(),
			}
			if !d.addCrowdKnowledge(knowledge) {
				return fmt.Errorf("cannot insert crowd knowledge %d into group %d",
					knowledge.Id, knowledge.KnowledgeGroupId)
			}
		} else if mm := m.GetKnowledgeGroupUpdate(); mm != nil {
			knowledgeGroup := d.FindKnowledgeGroup(mm.GetKnowledgeGroup().GetId())
			if knowledgeGroup == nil {
				return fmt.Errorf("cannot find knowledge group to update: %d",
					mm.GetKnowledgeGroup().GetId())
			}
			if mm.Party != nil {
				knowledgeGroup.PartyId = mm.GetParty().GetId()
			}
			if mm.Parent != nil {
				knowledgeGroup.ParentId = mm.GetParent().GetId()
			}
			if mm.Enabled != nil {
				knowledgeGroup.Enabled = *mm.Enabled
			}
			if mm.Type != nil {
				knowledgeGroup.Type = *mm.Type
			}
		} else if mm := m.GetControlGlobalWeather(); mm != nil {
			attributes := mm.GetAttributes()
			weather := &d.GlobalWeather
			weather.Temperature = float32(attributes.GetTemperature())
			weather.WindSpeed = float32(attributes.GetWindSpeed())
			weather.WindDirection = attributes.GetWindDirection().GetHeading()
			weather.CloudFloor = float32(attributes.GetCloudFloor())
			weather.CloudCeil = float32(attributes.GetCloudCeiling())
			weather.CloudDensity = float32(attributes.GetCloudDensity())
			weather.Precipitation = attributes.GetPrecipitation()
			weather.Lightning = attributes.GetLighting()
		} else if mm := m.GetControlLocalWeatherCreation(); mm != nil {
			start, err := GetTime(mm.GetStartDate())
			if err != nil {
				return fmt.Errorf("cannot parse local weather start time: %v",
					mm.GetStartDate())
			}
			end, err := GetTime(mm.GetEndDate())
			if err != nil {
				return fmt.Errorf("cannot parse local weather end time: %v",
					mm.GetEndDate())
			}
			attr := mm.GetAttributes()
			// most attributes are truncated here...
			w := &LocalWeather{
				Weather: Weather{
					Temperature:   float32(attr.GetTemperature()),
					WindSpeed:     float32(attr.GetWindSpeed()),
					WindDirection: attr.GetWindDirection().GetHeading(),
					CloudFloor:    float32(attr.GetCloudFloor()),
					CloudCeil:     float32(attr.GetCloudCeiling()),
					CloudDensity:  float32(attr.GetCloudDensity()),
					Precipitation: attr.GetPrecipitation(),
					//Lighting:      attr.GetLighting(),
				},
				StartTime:   start,
				EndTime:     end,
				TopLeft:     ReadPoint(mm.GetTopLeft()),
				BottomRight: ReadPoint(mm.GetBottomRight()),
				Id:          mm.GetWeather().GetId(),
			}
			d.addLocalWeather(w)
		} else if mm := m.GetControlLocalWeatherDestruction(); mm != nil {
			if !d.removeLocalWeather(mm.GetWeather().GetId()) {
				return fmt.Errorf("cannot find local weather to destroy: %d",
					mm.GetWeather().GetId())
			}
		} else if mm := m.GetChangeDiplomacy(); mm != nil {
			party1 := d.Parties[mm.GetParty1().GetId()]
			if party1 == nil {
				return fmt.Errorf("cannot resolve diplomacy change first party: %d",
					mm.GetParty1().GetId())
			}
			party2 := d.Parties[mm.GetParty2().GetId()]
			if party2 == nil {
				return fmt.Errorf("cannot resolve diplomacy change second party: %d",
					mm.GetParty2().GetId())
			}
			party1.Diplomacies[party2.Id] = mm.GetDiplomacy()
		} else if mm := m.GetUrbanCreation(); mm != nil {
			urban := NewUrban(
				mm.GetObject().GetId(),
				mm.GetName(),
				NewResourceNetworks(mm.GetAttributes()))
			if !d.addUrban(urban) {
				return fmt.Errorf("cannot insert urban block: %d", urban.Id)
			}
		} else if mm := m.GetUrbanUpdate(); mm != nil {
			if !d.updateUrban(mm.GetObject().GetId(), NewResourceNetworks(mm.GetAttributes())) {
				return fmt.Errorf("cannot update urban block: %d",
					mm.GetObject().GetId())
			}
		} else if mm := m.GetAutomatChangeLogisticLinks(); mm != nil {
			if mm.GetRequester() != nil {
				requester := mm.GetRequester()
				superiors := []uint32{}
				for _, s := range mm.GetSuperior() {
					if s.GetAutomat() != nil {
						superiors = append(superiors, s.GetAutomat().GetId())
					}
					if s.GetFormation() != nil {
						superiors = append(superiors, s.GetFormation().GetId())
					}
				}
				if requester.GetAutomat() != nil {
					if !d.changeLogisticsLinks(requester.GetAutomat().GetId(), superiors) {
						return fmt.Errorf("cannot update logistic links on automat: %d",
							requester.GetAutomat().GetId())
					}
				} else if requester.GetFormation() != nil {
					if !d.changeLogisticsLinks(requester.GetFormation().GetId(), superiors) {
						return fmt.Errorf("cannot update logistic links on formation: %d",
							requester.GetFormation().GetId())
					}
				}
			}
		} else if mm := m.GetLogSupplyQuotas(); mm != nil {
			quotas := map[uint32]int32{}
			if mm.GetQuotas() != nil {
				for _, quota := range mm.GetQuotas().GetElem() {
					quotas[quota.GetResource().GetId()] = quota.GetQuantity()
				}
			}
			if mm.GetSupplied() != nil {
				if mm.GetSupplied().GetAutomat() != nil {
					if !d.changeSupplyQuotas(mm.GetSupplied().GetAutomat().GetId(), quotas) {
						return fmt.Errorf("cannot update supply quotas on automat: %d",
							mm.GetSupplied().GetAutomat().GetId())
					}
				} else if mm.GetSupplied().GetFormation() != nil {
					if !d.changeSupplyQuotas(mm.GetSupplied().GetFormation().GetId(), quotas) {
						return fmt.Errorf("cannot update supply quotas on formation: %d",
							mm.GetSupplied().GetFormation().GetId())
					}
				}
			}
		} else if mm := m.GetUnitChangeSuperior(); mm != nil {
			unit := d.FindUnit(mm.GetUnit().GetId())
			if unit == nil {
				return fmt.Errorf("cannot find unit which superior must be updated: %d",
					mm.GetUnit().GetId())
			}
			newAutomat := d.FindAutomat(mm.GetParent().GetId())
			if newAutomat == nil {
				return fmt.Errorf("cannot find unit new parent automat: %d",
					mm.GetParent().GetId())
			}
			if err := d.changeSuperior(unit, newAutomat); err != nil {
				return fmt.Errorf("cannot change %d unit superior to %d: %s",
					unit.Id, newAutomat.Id, err)
			}
		}
	} else if msg.AuthenticationToClient != nil {
		m := msg.AuthenticationToClient.GetMessage()
		if mm := m.GetProfileCreation(); mm != nil {
			profile := &Profile{
				mm.GetProfile().GetLogin(),
				mm.GetProfile().GetPassword(),
				mm.GetProfile().GetSupervisor()}
			if !d.addProfile(profile) {
				return fmt.Errorf("cannot insert profile: %s", profile.Login)
			}
		} else if mm := m.GetProfileUpdate(); mm != nil {
			profile := &Profile{
				mm.GetProfile().GetLogin(),
				mm.GetProfile().GetPassword(),
				mm.GetProfile().GetSupervisor()}
			if !d.updateProfile(mm.GetLogin(), profile) {
				return fmt.Errorf("cannot find profile to update: %s", profile.Login)
			}
		} else if mm := m.GetProfileDestruction(); mm != nil {
			if !d.removeProfile(mm.GetLogin()) {
				return fmt.Errorf("cannot find profile to remove: %s",
					mm.GetLogin())
			}
		}
	}
	return nil
}

// Register a handler on supplied client and start processing messages
func (model *Model) Listen(client *Client) {
	handler := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			return true
		}
		model.requests <- &ModelRequest{Message: msg}
		return false
	}
	client.Register(handler)
}

func (model *Model) waitCommand(cmd func(model *Model)) {
	c := &ModelRequest{
		Command: &modelCmd{
			done: make(chan int, 1),
			cmd:  cmd,
		},
	}
	model.requests <- c
	<-c.Command.done
}

// Post and wait for a condition to be validated. Return false on
// timeout. The condition is validated only once.
func (model *Model) waitCond(timeout time.Duration,
	cond func(model *Model) bool) bool {
	model.condId++
	c := &ModelRequest{
		Cond: &modelCond{
			id:   model.condId,
			done: make(chan int, 1),
			cond: cond,
		},
	}
	model.requests <- c
	timeoutch := make(chan int)
	if timeout.Nanoseconds() > 0 {
		go func() {
			time.Sleep(timeout)
			timeoutch <- 1
		}()
	}
	removed := false
	select {
	case <-c.Cond.done:
		break
	case <-timeoutch:
		model.waitCommand(func(model *Model) {
			for i := 0; i != len(model.conds); {
				if model.conds[i].id == c.Cond.id {
					model.conds[i].done <- 0
					model.conds = append(model.conds[:i], model.conds[i+1:]...)
					removed = true
					break
				}
				i++
			}
		})
	}
	return !removed
}

// Return true if the model is ready.
func (model *Model) IsReady() bool {
	ready := false
	model.waitCommand(func(model *Model) {
		ready = model.ready
	})
	return ready
}

// Wait for the model to become ready, return false otherwise.
func (model *Model) WaitReady(timeout time.Duration) bool {
	return model.waitCond(timeout, func(model *Model) bool {
		return model.ready
	})
}

func (model *Model) GetData() *ModelData {
	var d *ModelData
	model.waitCommand(func(model *Model) {
		d = &ModelData{}
		DeepCopy(d, model.data)
	})
	return d
}

func (model *Model) GetProfile(login string) *Profile {
	var p *Profile
	model.waitCommand(func(model *Model) {
		profile, ok := model.data.Profiles[login]
		if ok {
			p = &Profile{}
			DeepCopy(p, profile)
		}
	})
	return p
}

func (model *Model) GetFormation(formationId uint32) *Formation {
	var f *Formation
	model.waitCommand(func(model *Model) {
		formation := model.data.FindFormation(formationId)
		if formation != nil {
			f = &Formation{}
			DeepCopy(f, formation)
		}
	})
	return f
}

func (model *Model) GetAutomat(automatId uint32) *Automat {
	var a *Automat
	model.waitCommand(func(model *Model) {
		automat := model.data.FindAutomat(automatId)
		if automat != nil {
			a = &Automat{}
			DeepCopy(a, automat)
		}
	})
	return a
}

func (model *Model) GetUnit(unitId uint32) *Unit {
	var u *Unit
	model.waitCommand(func(model *Model) {
		unit := model.data.FindUnit(unitId)
		if unit != nil {
			u = &Unit{}
			DeepCopy(u, unit)
		}
	})
	return u
}

func (model *Model) GetCrowd(crowdId uint32) *Crowd {
	var c *Crowd
	model.waitCommand(func(model *Model) {
		crowd := model.data.FindCrowd(crowdId)
		if crowd != nil {
			c = &Crowd{}
			DeepCopy(c, crowd)
		}
	})
	return c
}

func (model *Model) GetLocalWeather(id uint32) *LocalWeather {
	var w *LocalWeather
	model.waitCommand(func(model *Model) {
		weather, ok := model.data.LocalWeathers[id]
		if ok {
			w = &LocalWeather{}
			DeepCopy(w, weather)
		}
	})
	return w
}

func (model *Model) GetGlobalWeather() *Weather {
	var w *Weather
	model.waitCommand(func(model *Model) {
		weather := model.data.GlobalWeather
		w = &Weather{}
		DeepCopy(w, weather)
	})
	return w
}

func (model *Model) GetUrban(id uint32) *Urban {
	var u *Urban
	model.waitCommand(func(model *Model) {
		urban, ok := model.data.Urbans[id]
		if ok {
			u = &Urban{}
			DeepCopy(u, urban)
		}
	})
	return u
}

func (model *Model) GetUnitKnowledge(groupId, id uint32) *UnitKnowledge {
	var k *UnitKnowledge
	model.waitCommand(func(model *Model) {
		group := model.data.FindKnowledgeGroup(groupId)
		if group != nil {
			k = group.UnitKnowledges[id]
		}
	})
	return k
}

func (model *Model) GetTick() int32 {
	tick := int32(0)
	model.waitCommand(func(model *Model) {
		tick = model.data.Tick
	})
	return tick
}

func (model *Model) WaitUntilTick(tick int32) bool {
	return model.waitCond(model.WaitTimeout, func(model *Model) bool {
		return tick <= model.data.Tick
	})
}

func (model *Model) WaitTicks(ticks int32) bool {
	endTick := model.GetTick() + ticks
	return model.WaitUntilTick(endTick)
}

// Wait for condition to return true. Return false if the timeout kicks in
// before the condition.
func (model *Model) WaitConditionTimeout(timeout time.Duration,
	condition func(data *ModelData) bool) bool {
	wrapper := func(model *Model) bool {
		return condition(model.data)
	}
	return model.waitCond(timeout, wrapper)
}

func (model *Model) WaitCondition(condition func(data *ModelData) bool) bool {
	wrapper := func(model *Model) bool {
		return condition(model.data)
	}
	return model.waitCond(model.WaitTimeout, wrapper)
}
