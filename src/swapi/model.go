package swapi

import (
	"fmt"
	"sword"
	"time"
)

const DatePattern = "20060102T150405"

type modelCmd struct {
	done chan int
	cmd  func(model *Model)
}

type modelCond struct {
	id   int
	done chan int
	cond func(model *Model) bool
}

// The model registers a handler to a Client and updates itself with
// incoming messages in a separate goroutine. It supports two kind of
// actions: commands which are just functions applied once on the Model,
// mostly used to implement queries, and conditions which are fired each
// time a message is processed until they return true.
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
	msgch  chan *SwordMessage
	cmdch  chan *modelCmd
	condch chan *modelCond
}

func NewModel() *Model {
	model := Model{
		ready:       false,
		condId:      0,
		WaitTimeout: 60 * time.Second,
		msgch:       make(chan *SwordMessage, 128),
		cmdch:       make(chan *modelCmd, 4),
		condch:      make(chan *modelCond, 4),
		conds:       []*modelCond{},
		data:        NewModelData(),
	}
	go model.run()
	return &model
}

func (model *Model) run() {
	for {
		select {
		case msg := <-model.msgch:
			model.update(msg)
			for i := 0; i != len(model.conds); {
				if model.conds[i].cond(model) {
					model.conds[i].done <- 1
					model.conds = append(model.conds[:i], model.conds[i+1:]...)
					continue
				}
				i++
			}
		case cmd := <-model.cmdch:
			cmd.cmd(model)
			cmd.done <- 1
		case cond := <-model.condch:
			if cond.cond(model) {
				cond.done <- 1
			} else {
				model.conds = append(model.conds, cond)
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
			t, err := time.Parse(DatePattern, mm.GetDateTime().GetData())
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
			unit := NewUnit(
				mm.GetUnit().GetId(),
				mm.GetAutomat().GetId(),
				mm.GetName())
			if !d.addUnit(unit) {
				// XXX report the error here
			}
		} else if mm := m.GetAutomatCreation(); mm != nil {
			automat := NewAutomat(
				mm.GetAutomat().GetId(),
				mm.GetParty().GetId(),
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
		} else if mm := m.GetPopulationCreation(); mm != nil {
			population := NewPopulation(
				mm.GetId().GetId(),
				mm.GetParty().GetId(),
				mm.GetName())
			if !d.addPopulation(population) {
				// XXX report error here
			}
		} else if mm := m.GetUnitDestruction(); mm != nil {
			d.removeUnit(mm.GetUnit().GetId())
		}
	}
}

// Register a handler on supplied client and start processing messages
func (model *Model) Listen(client *Client) {
	handler := func(msg *SwordMessage, context int32, err error) bool {
		if err != nil {
			return true
		}
		model.msgch <- msg
		return false
	}
	client.Register(handler)
}

func (model *Model) waitCommand(cmd func(model *Model)) {
	c := &modelCmd{
		done: make(chan int, 1),
		cmd:  cmd,
	}
	model.cmdch <- c
	<-c.done
}

// Post and wait for a condition to be validated. Return false on
// timeout. The condition is validated only once.
func (model *Model) waitCond(timeout time.Duration,
	cond func(model *Model) bool) bool {
	model.condId++
	c := &modelCond{
		id:   model.condId,
		done: make(chan int, 1),
		cond: cond,
	}
	model.condch <- c
	timeoutch := make(chan int)
	if timeout.Nanoseconds() > 0 {
		go func() {
			time.Sleep(timeout)
			timeoutch <- 1
		}()
	}
	removed := false
	select {
	case <-c.done:
		break
	case <-timeoutch:
		model.waitCommand(func(model *Model) {
			for i := 0; i != len(model.conds); {
				if model.conds[i].id == c.id {
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
		d = model.data.Copy()
	})
	return d
}

func (model *Model) WaitUntilTick(tick int32) bool {
	return model.waitCond(model.WaitTimeout, func(model *Model) bool {
		return tick <= model.data.Tick
	})
}
