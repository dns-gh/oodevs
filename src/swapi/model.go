package swapi

import (
	"fmt"
	"sword"
	"time"
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
	ready   bool
	conds   []*modelCond
	parties map[uint32]*Party

	// State only touched by caller routines
	condId int

	// Shared state
	msgch  chan *SwordMessage
	cmdch  chan *modelCmd
	condch chan *modelCond
}

func NewModel() *Model {
	model := Model{
		ready:   false,
		condId:  0,
		msgch:   make(chan *SwordMessage, 128),
		cmdch:   make(chan *modelCmd, 4),
		condch:  make(chan *modelCond, 4),
		conds:   []*modelCond{},
		parties: map[uint32]*Party{},
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
	if msg.SimulationToClient != nil {
		m := msg.SimulationToClient.GetMessage()
		if m.GetControlSendCurrentStateEnd() != nil {
			model.ready = true
		} else if mm := m.GetPartyCreation(); mm != nil {
			party := NewParty(
				mm.GetParty().GetId(),
				mm.GetName())
			model.parties[party.Id] = party
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
			party, parent := model.findFormation(formation.PartyId,
				formation.ParentId)
			if party == nil {
				panic(fmt.Sprintf("cannot create formation %v with unknown"+
					"parent party=%v/parent=%v", formation.Id, formation.PartyId,
					formation.ParentId))
			}
			if parent == nil {
				party.Formations[formation.Id] = formation
				return
			}
			parent.Formations[formation.Id] = formation
		}
	}
}

func (model *Model) findFormation(partyId uint32,
	parentId uint32) (*Party, *Formation) {
	party, ok := model.parties[partyId]
	if !ok {
		return nil, nil
	}
	if parentId == 0 {
		return party, nil
	}
	pendings := []*Formation{}
	for _, v := range party.Formations {
		pendings = append(pendings, v)
	}
	for len(pendings) > 0 {
		f := pendings[len(pendings)-1]
		pendings = pendings[:len(pendings)-1]
		if f.Id == parentId {
			return party, f
		}
		for _, v := range f.Formations {
			pendings = append(pendings, v)
		}
	}
	return nil, nil
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

func (model *Model) GetParties() map[uint32]*Party {
	parties := map[uint32]*Party{}
	model.waitCommand(func(model *Model) {
		for k, v := range model.parties {
			parties[k] = v.Copy()
		}
	})
	return parties
}
