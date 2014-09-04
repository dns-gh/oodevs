// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package services

import (
	gouuid "code.google.com/p/go-uuid/uuid"
	"code.google.com/p/goprotobuf/proto"
	"fmt"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/timeline/sdk"
	"masa/timeline/util"
	"time"
)

// Those functions WILL be called in *some* thread
// so the receiver MUST NOT block on those calls
// and MUST pay attention to synchronisation
type SwordReaderObserver interface {
	Tick(tick time.Time)
	UpdateEvent(uuid string, event *sdk.Event)
	Restart(err error)
	Log(format string, args ...interface{})
}

type SwordReader struct {
	id      int32
	name    string
	poster  SwordReaderObserver
	client  *swapi.Client
	last    time.Time   // last known time
	orders  Ids         // known orders
	actions Ids         // known actions
	async   *SwapiAsync // async swapi events
}

func NewSwordReader(name string, client *swapi.Client) *SwordReader {
	async := NewSwapiAsync()
	id := client.Register(async.Write)
	return &SwordReader{
		id:     id,
		name:   name,
		client: client,
		async:  async,
	}
}

func (s *SwordReader) Attach(poster SwordReaderObserver, orders Ids, actions Ids) {
	s.poster = poster
	s.orders = orders
	s.actions = actions
	s.async.Start(s.readMessage)
}

func (s *SwordReader) Close() {
	s.async.Close()
}

func (s *SwordReader) readMessage(msg *swapi.SwordMessage, clientId, ctx int32, err error) bool {
	if err != nil {
		s.poster.Restart(err)
		return true
	}
	if msg.SimulationToClient == nil {
		return false
	}
	content := msg.SimulationToClient.GetMessage()
	if info := content.ControlInformation; info != nil {
		s.last = s.tick(s.last, info.GetDateTime().GetData())
	} else if tick := content.ControlBeginTick; tick != nil {
		s.last = s.tick(s.last, tick.GetDateTime().GetData())
	} else if order := content.Action; order != nil {
		event := s.readAction(s.name, s.last, order)
		if event != nil {
			s.postEvent(event)
		}
	} else if ack := content.OrderAck; ack != nil {
		s.saveOrder(msg, clientId, ack)
	} else if ack := content.FragOrderAck; ack != nil {
		s.saveOrder(msg, clientId, ack)
	} else if ack := content.MagicActionAck; ack != nil {
		s.saveMagic(msg, clientId, ack)
	} else if ack := content.UnitMagicActionAck; ack != nil {
		s.saveMagic(msg, clientId, ack)
	} else if ack := content.ObjectMagicActionAck; ack != nil {
		s.saveMagic(msg, clientId, ack)
	} else if ack := content.KnowledgeGroupMagicActionAck; ack != nil {
		s.saveMagic(msg, clientId, ack)
	} else if ack := content.SetAutomatModeAck; ack != nil {
		s.saveMagic(msg, clientId, ack)
	} else if report := content.Report; report != nil {
		if false {
			event := readReport(s.last, report)
			s.postEvent(event)
		}
	}
	return false
}

func (s *SwordReader) tick(last time.Time, value string) time.Time {
	tick, err := time.Parse(SwordTimeLayout, value)
	if err != nil {
		s.poster.Log("invalid time layout: %v", value)
		return last
	}
	s.poster.Tick(tick)
	return tick
}

func (s *SwordReader) readAction(target string, tick time.Time, order *sword.Action) *sdk.Event {
	name := ""
	content := sword.ClientToSim_Content{}
	code := order.GetErrorCode()
	codemsg := ""
	magic := true
	predicate := func() bool { return s.isUnknownAction(order) }
	if sub := order.MagicAction; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{MagicAction: sub}
		codemsg = sword.MagicActionAck_ErrorCode(code).String()
	} else if sub := order.UnitMagicAction; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{UnitMagicAction: sub}
		codemsg = sword.UnitActionAck_ErrorCode(code).String()
	} else if sub := order.ObjectMagicAction; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{ObjectMagicAction: sub}
		codemsg = sword.ObjectMagicActionAck_ErrorCode(code).String()
	} else if sub := order.KnowledgeMagicAction; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{KnowledgeMagicAction: sub}
		codemsg = sword.KnowledgeGroupAck_ErrorCode(code).String()
	} else if sub := order.SetAutomatMode; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{SetAutomatMode: sub}
		codemsg = sword.SetAutomatModeAck_ErrorCode(code).String()
	} else if sub := order.AutomatOrder; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{AutomatOrder: sub}
		magic = false
		predicate = func() bool { return s.isUnknownOrder(sub) }
	} else if sub := order.CrowdOrder; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{CrowdOrder: sub}
		magic = false
		predicate = func() bool { return s.isUnknownOrder(sub) }
	} else if sub := order.FragOrder; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{FragOrder: sub}
		magic = false
		predicate = func() bool { return s.isUnknownOrder(sub) }
	} else if sub := order.UnitOrder; sub != nil {
		name = sub.GetName()
		content = sword.ClientToSim_Content{UnitOrder: sub}
		magic = false
		predicate = func() bool { return s.isUnknownOrder(sub) }
	} else {
		return nil
	}
	if !predicate() {
		return nil
	}
	start := &sword.DateTime{Data: proto.String(order.GetStartTime())}
	event := packOrder(name, target, tick, start, &content, true, !magic)
	event.ErrorCode = &code
	errmsg := order.GetErrorMsg()
	event.ErrorText = &errmsg
	if code != 0 && len(errmsg) == 0 {
		// if we don't have any error message,
		// stringify the error code enumeration
		event.ErrorText = &codemsg
	}
	return event
}

func tryParseDate(value *sword.DateTime, tick time.Time) time.Time {
	reply, err := time.Parse(SwordTimeLayout, value.GetData())
	if err != nil {
		return tick
	}
	return reply
}

func encode(msg *sword.ClientToSim) []byte {
	data, err := proto.Marshal(msg)
	if err != nil {
		// fixme log error
		return []byte{}
	}
	return data
}

func packOrder(name, target string, tick time.Time, start *sword.DateTime,
	pkt *sword.ClientToSim_Content, done, readOnly bool) *sdk.Event {
	return &sdk.Event{
		Uuid:     proto.String(gouuid.New()),
		Name:     proto.String(name),
		Begin:    proto.String(util.FormatTime(tryParseDate(start, tick))),
		Done:     proto.Bool(done),
		ReadOnly: proto.Bool(readOnly),
		Action: &sdk.Action{
			Target:  proto.String("sword://" + target),
			Apply:   proto.Bool(true),
			Payload: encode(&sword.ClientToSim{Message: pkt}),
		},
	}
}

func getTaskerId(t *sword.Tasker) uint32 {
	if t.Automat != nil {
		return t.GetAutomat().GetId()
	} else if t.Formation != nil {
		return t.GetFormation().GetId()
	} else if t.Crowd != nil {
		return t.GetCrowd().GetId()
	} else if t.Unit != nil {
		return t.GetUnit().GetId()
	} else if t.Party != nil {
		return t.GetParty().GetId()
	} else if t.Population != nil {
		return t.GetPopulation().GetId()
	}
	return 0
}

func readReport(tick time.Time, report *sword.Report) *sdk.Event {
	return &sdk.Event{
		Uuid: proto.String(gouuid.New()),
		Name: proto.String(fmt.Sprintf("Report %d %s",
			getTaskerId(report.GetSource()),
			report.GetCategory().String(),
		)),
		Begin: proto.String(util.FormatTime(tryParseDate(report.GetTime(), tick))),
		Done:  proto.Bool(true),
	}
}

type IdGetter interface {
	GetId() uint32
}

func (s *SwordReader) isUnknown(ids Ids, id IdGetter) bool {
	value := id.GetId()
	if value == 0 {
		return true
	}
	prev := len(ids)
	ids[value] = struct{}{}
	return prev != len(ids)
}

func (s *SwordReader) isUnknownOrder(id IdGetter) bool {
	return s.isUnknown(s.orders, id)
}

func (s *SwordReader) isUnknownAction(id IdGetter) bool {
	return s.isUnknown(s.actions, id)
}

func (s *SwordReader) saveOrder(msg *swapi.SwordMessage, clientId int32, id IdGetter) {
	s.saveAction(s.orders, msg, clientId, id)
}

func (s *SwordReader) saveMagic(msg *swapi.SwordMessage, clientId int32, id IdGetter) {
	s.saveAction(s.actions, msg, clientId, id)
}

func (s *SwordReader) postEvent(event *sdk.Event) {
	s.poster.UpdateEvent(event.GetUuid(), event)
}

func (s *SwordReader) saveAction(ids Ids, msg *swapi.SwordMessage, clientId int32, id IdGetter) {
	msgClientId := msg.SimulationToClient.GetClientId()
	if msgClientId == 0 || msgClientId != clientId {
		return
	}
	value := id.GetId()
	if value == 0 {
		return
	}
	ids[value] = struct{}{}
}
