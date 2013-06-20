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
	ready bool
	conds []*modelCond
	data  *ModelData

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
	go model.run()
	return &model
}

func (model *Model) run() {
	for rq := range model.requests {
		if rq.Message != nil {
			model.update(rq.Message)
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
}

func (model *Model) update(msg *SwordMessage) {
	d := model.data
	if msg.SimulationToClient != nil {
		m := msg.SimulationToClient.GetMessage()
		if m.GetControlSendCurrentStateEnd() != nil {
			model.ready = true
		} else if mm := m.GetControlBeginTick(); mm != nil {
			t, err := GetTime(mm.GetDateTime())
			if err != nil {
				// XXX: report parsing error here
				return
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
				mm.GetUnit().GetId(),
				mm.GetAutomat().GetId(),
				mm.GetName(),
				mm.GetPc(),
				Point{},
				0}
			if !d.addUnit(unit) {
				// XXX report the error here
			}
		} else if mm := m.GetUnitAttributes(); mm != nil {
			unit := d.FindUnit(mm.GetUnit().GetId())
			if unit == nil {
				// XXX report there error here
				return
			}
			if mm.Headquarters != nil {
				unit.Pc = *mm.Headquarters
			}
			if mm.Position != nil {
				unit.Position = ReadPoint(mm.GetPosition())
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
			if d.addAutomat(automatId, formationId, automat) {
				return
			}
			// XXX: report error here
		} else if mm := m.GetAutomatAttributes(); mm != nil {
			automat := d.FindAutomat(mm.GetAutomat().GetId())
			if automat == nil {
				// XXX report error here
				return
			}
			if mm.Mode != nil {
				mode := mm.GetMode()
				if mode == sword.EnumAutomatMode_engaged {
					automat.Engaged = true
				} else if mode == sword.EnumAutomatMode_disengaged {
					automat.Engaged = false
				}
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
				panic(fmt.Sprintf("cannot create formation %v with unknown"+
					"parent party=%v/parent=%v", formation.Id, formation.PartyId,
					formation.ParentId))
			}
		} else if mm := m.GetCrowdCreation(); mm != nil {
			crowd := NewCrowd(
				mm.GetCrowd().GetId(),
				mm.GetParty().GetId(),
				mm.GetName())
			if !d.addCrowd(crowd) {
				// XXX report error here
			}
		} else if mm := m.GetCrowdUpdate(); mm != nil {
			crowd := d.FindCrowd(mm.GetCrowd().GetId())
			if crowd == nil {
				// XXX report error here
				return
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
			if d.addCrowdElement(mm.GetCrowd().GetId(), mm.GetFlow().GetId()) {
				// XXX report error here
				return
			}
		} else if mm := m.GetCrowdFlowDestruction(); mm != nil {
			if d.removeCrowdElement(mm.GetCrowd().GetId(), mm.GetFlow().GetId()) {
				return
			}
		} else if mm := m.GetCrowdConcentrationCreation(); mm != nil {
			if d.addCrowdElement(mm.GetCrowd().GetId(), mm.GetConcentration().GetId()) {
				// XXX report error here
				return
			}
		} else if mm := m.GetCrowdConcentrationDestruction(); mm != nil {
			if d.removeCrowdElement(mm.GetCrowd().GetId(), mm.GetConcentration().GetId()) {
				return
			}
		} else if mm := m.GetPopulationCreation(); mm != nil {
			population := &Population{
				mm.GetId().GetId(),
				mm.GetParty().GetId(),
				mm.GetName()}
			if !d.addPopulation(population) {
				// XXX report error here
			}
		} else if mm := m.GetUnitPathfind(); mm != nil {
			unit := d.FindUnit(mm.GetUnit().GetId())
			if unit == nil {
				// XXX report error here
				return
			}
			if mm.Path != nil && mm.Path.Location != nil {
				unit.PathPoints = uint32(0)
				if mm.Path.Location.Coordinates != nil {
					unit.PathPoints = uint32(len(mm.Path.Location.Coordinates.Elem))
				}
			}
		} else if mm := m.GetUnitDestruction(); mm != nil {
			d.removeUnit(mm.GetUnit().GetId())
		} else if mm := m.GetKnowledgeGroupCreation(); mm != nil {
			group := &KnowledgeGroup{
				mm.GetKnowledgeGroup().GetId(),
				mm.GetName(),
				mm.GetParty().GetId(),
				mm.GetParent().GetId()}
			if !d.addKnowledgeGroup(group) {
				// XXX report error here
			}
		} else if mm := m.GetControlLocalWeatherCreation(); mm != nil {
			start, err := GetTime(mm.GetStartDate())
			if err != nil {
				// XXX report error here
				return
			}
			end, err := GetTime(mm.GetEndDate())
			if err != nil {
				// XXX report error here
				return
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
			d.removeLocalWeather(mm.GetWeather().GetId())
		}
	} else if msg.AuthenticationToClient != nil {
		m := msg.AuthenticationToClient.GetMessage()
		if mm := m.GetProfileCreation(); mm != nil {
			profile := &Profile{
				mm.GetProfile().GetLogin(),
				mm.GetProfile().GetPassword(),
				mm.GetProfile().GetSupervisor()}
			d.addProfile(profile)
		} else if mm := m.GetProfileUpdate(); mm != nil {
			profile := &Profile{
				mm.GetProfile().GetLogin(),
				mm.GetProfile().GetPassword(),
				mm.GetProfile().GetSupervisor()}
			d.updateProfile(mm.GetLogin(), profile)
			// XXX report error here
		} else if mm := m.GetProfileDestruction(); mm != nil {
			d.removeProfile(mm.GetLogin())
			// XXX report error here
		}
	}
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
