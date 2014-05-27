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
	"encoding/json"
	"errors"
	"fmt"
	"net/url"
	"sdk"
	"swapi"
	"sword"
	"sync"
	"time"
	"util"
)

var (
	SwordTimeLayout = "20060102T150405"
	ErrAborted      = errors.New("aborted")
	ErrSkipped      = errors.New("skipped")
	ErrMissingLink  = errors.New("missing link")
	ErrTooManyLinks = errors.New("too many links")
	ErrRetry        = errors.New("retry")
)

type None struct{}

type PendingAction struct {
	url     url.URL
	payload []byte
	lock    bool
}

func NewAction(url url.URL, payload []byte, lock bool) PendingAction {
	return PendingAction{
		url:     url,
		payload: payload,
		lock:    lock,
	}
}

// mutable sword data
type SwordData struct {
	input   *swapi.Client                  // input message link
	handler int32                          // registered input handler
	group   *sync.WaitGroup                // input wait group
	orders  map[uint32]None                // known orders
	actions map[uint32]None                // known actions
	events  map[string]*swapi.SwordMessage // decoded messages
	pending map[string]PendingAction       // pending actions
	retry   uint                           // retry count
	stopped bool                           // set when sword is manually stopped, to skip restarts
}

type Sword struct {
	log     util.Logger
	root    Observer   // attached observer
	name    string     // service name
	address string     // sword address
	clock   bool       // whether we use sword clock
	mutex   sync.Mutex // protects SwordData
	d       SwordData
}

func NewSword(log util.Logger, root Observer, clock bool, name, address string) *Sword {
	return &Sword{
		log:     log,
		root:    root,
		name:    name,
		address: address,
		clock:   clock,
		d: SwordData{
			orders:  map[uint32]None{},
			actions: map[uint32]None{},
			events:  map[string]*swapi.SwordMessage{},
			pending: map[string]PendingAction{},
		},
	}
}

func (s *Sword) Proto(name string) *sdk.Service {
	return &sdk.Service{
		Name:  proto.String(name),
		Clock: proto.Bool(s.clock),
		Sword: &sdk.Sword{
			Address: proto.String(s.address),
		},
	}
}

func (s *Sword) HasClock() bool {
	return s.clock
}

func (*Sword) IsLocked() bool {
	return true
}

func (s *Sword) Log(format string, args ...interface{}) {
	msg := fmt.Sprintf(format, args...)
	s.log.Printf("sword[%v] %v\n", s.address, msg)
}

func (s *Sword) tick(last time.Time, value string) time.Time {
	tick, err := time.Parse(SwordTimeLayout, value)
	if err != nil {
		s.Log("invalid time layout: %v", value)
		return last
	}
	s.root.Tick(tick)
	return tick
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

func (s *Sword) readAction(target string, tick time.Time, order *sword.Action) *sdk.Event {
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

func (s *Sword) event(event *sdk.Event) {
	s.root.UpdateEvent(event.GetUuid(), event)
}

type IdGetter interface {
	GetId() uint32
}

func (s *Sword) isUnknown(data map[uint32]None, id IdGetter) bool {
	value := id.GetId()
	if value == 0 {
		return true
	}
	s.mutex.Lock()
	prev := len(data)
	data[value] = None{}
	next := len(data)
	s.mutex.Unlock()
	return prev != next
}

func (s *Sword) isUnknownOrder(id IdGetter) bool {
	return s.isUnknown(s.d.orders, id)
}

func (s *Sword) isUnknownAction(id IdGetter) bool {
	return s.isUnknown(s.d.actions, id)
}

func (s *Sword) saveAction(data map[uint32]None, msg *swapi.SwordMessage, clientId int32, id IdGetter) {
	msgClientId := msg.SimulationToClient.GetClientId()
	if msgClientId == 0 || msgClientId != clientId {
		return
	}
	value := id.GetId()
	if value == 0 {
		return
	}
	s.mutex.Lock()
	data[value] = None{}
	s.mutex.Unlock()
}

func (s *Sword) saveOrder(msg *swapi.SwordMessage, clientId int32, id IdGetter) {
	s.saveAction(s.d.orders, msg, clientId, id)
}

func (s *Sword) saveMagic(msg *swapi.SwordMessage, clientId int32, id IdGetter) {
	s.saveAction(s.d.actions, msg, clientId, id)
}

func (s *Sword) readMessage(msg *swapi.SwordMessage, clientId int32, err error, last time.Time) time.Time {
	if err != nil {
		return last
	}
	if msg.SimulationToClient == nil {
		return last
	}
	content := msg.SimulationToClient.GetMessage()
	if info := content.ControlInformation; info != nil {
		last = s.tick(last, info.GetDateTime().GetData())
	} else if tick := content.ControlBeginTick; tick != nil {
		last = s.tick(last, tick.GetDateTime().GetData())
	} else if order := content.Action; order != nil {
		event := s.readAction(s.name, last, order)
		if event != nil {
			go s.event(event)
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
			event := readReport(last, report)
			go s.event(event)
		}
	}
	return last
}

func (s *Sword) getRetryTimeout() time.Duration {
	s.mutex.Lock()
	retry := s.d.retry
	s.d.retry++
	s.mutex.Unlock()
	if retry == 0 {
		return 0 * time.Second
	}
	sleep := 1 << (retry - 1) * time.Second
	if sleep > time.Minute {
		sleep = time.Minute
	}
	return sleep
}

func (s *Sword) tryRestart() {
	timeout := s.getRetryTimeout()
	go func() {
		time.Sleep(timeout)
		s.restart()
	}()
}

func (s *Sword) runClient(client *swapi.Client) {
	err := client.Run()
	if err == nil {
		return
	}
	skip := s.stop(client)
	if skip {
		return
	}
	s.Log("network error: %v", err)
	s.tryRestart()
}

func (s *Sword) setupClient(client *swapi.Client) (int32, error) {
	client.EnableModel = true
	client.Model.SetErrorHandler(func(d *swapi.ModelData, m *swapi.SwordMessage, err error) error {
		if err != nil {
			s.Log("model error: %v", err)
		}
		return nil
	})
	go s.runClient(client)
	var handler int32
	if s.clock {
		last := time.Time{}
		handler = client.Register(func(msg *swapi.SwordMessage, id, context int32, err error) bool {
			last = s.readMessage(msg, id, err, last)
			return false
		})
	}
	key, err := client.GetAuthenticationKey()
	if err != nil {
		s.Log("invalid key request: %v", err)
		return handler, err
	}
	err = client.LoginWithAuthenticationKey("", "", key)
	if err != nil {
		s.Log("invalid login: %v", err)
		return handler, err
	}
	return handler, nil
}

func (s *Sword) start(restart bool) error {
	// quick check so we don't make a new client for nothing
	s.mutex.Lock()
	skip := s.d.input != nil || s.d.stopped
	s.mutex.Unlock()
	if restart && skip {
		return ErrSkipped
	}
	client, err := swapi.NewClient(s.address)
	if err != nil {
		// if we fail here, nobody will call
		// tryRestart unless we do, runClient
		// will only do it after setupClient
		if restart {
			s.tryRestart()
		}
		return err
	}
	handler, err := s.setupClient(client)
	// make sure we close dangling clients
	defer func() {
		if client == nil {
			return
		}
		client.Unregister(handler)
		client.Close()
	}()
	if err != nil {
		return err
	}
	s.mutex.Lock()
	defer s.mutex.Unlock()
	// we need to validate state again
	// since we were previously unlocked
	if s.d.input != nil {
		return ErrTooManyLinks
	}
	if restart && s.d.stopped {
		return ErrSkipped
	}
	s.d.input = client
	s.d.handler = handler
	s.d.group = &sync.WaitGroup{}
	s.d.retry = 0
	for uuid, it := range s.d.pending {
		s.Log("retrying %s", uuid)
		go s.Apply(uuid, it.url, it.payload)
	}
	if !restart {
		s.d.stopped = false
	}
	client = nil // skip deferred close
	return nil
}

func (s *Sword) logAndStart(restart bool) error {
	prefix := ""
	if restart {
		prefix = "re"
	}
	s.Log("%sconnecting to %s", prefix, s.address)
	err := s.start(restart)
	if err == ErrSkipped {
		s.Log("skipping reconnection to %s", s.address)
	} else if err != nil {
		s.Log("error %sconnecting to %s: %s", prefix, s.address, err)
	} else {
		s.Log("%sconnected to %s", prefix, s.address)
	}
	return err
}

func (s *Sword) Start() error {
	return s.logAndStart(false)
}

func (s *Sword) restart() error {
	return s.logAndStart(true)
}

func (s *Sword) stop(previous *swapi.Client) bool {
	s.mutex.Lock()
	// when restarting, skip stop when saved client is
	// different from the one we're trying to close
	if previous != nil && previous != s.d.input {
		s.mutex.Unlock()
		return true
	}
	// no previous client mean we want to stop
	// and abort any in-flight restarts
	if previous == nil {
		s.d.stopped = true
		for uuid, it := range s.d.pending {
			s.Log("aborting action %v", uuid)
			go s.root.OnApply(uuid, ErrAborted, it.lock)
		}
		s.d.orders = map[uint32]None{}
		s.d.actions = map[uint32]None{}
		s.d.pending = map[string]PendingAction{}
	}
	client := s.d.input
	handler := s.d.handler
	group := s.d.group
	s.d.input = nil
	s.d.handler = 0
	s.d.group = nil
	s.mutex.Unlock()
	if client != nil {
		suffix := "ing"
		if previous != nil {
			suffix = "ed"
		}
		s.Log("disconnect%v from %s", suffix, s.address)
		client.Unregister(handler)
		go func() {
			group.Wait()
			client.Close()
		}()
	}
	return false
}

func (s *Sword) Stop() error {
	s.stop(nil)
	return nil
}

func processAck(code int32, msg, def string) error {
	if code == 0 {
		return nil
	}
	if len(msg) == 0 {
		return util.NewError(int32(code), def)
	}
	return util.NewError(int32(code), msg)
}

func (s *Sword) waitAck(errors chan<- error, msg *swapi.SwordMessage, client, context int32, err error) bool {
	if err != nil {
		errors <- err
		return true
	}
	if msg.SimulationToClient == nil {
		return false
	}
	if msg.ClientId != client || msg.Context != context {
		return false
	}
	m := msg.SimulationToClient.GetMessage()
	if ack := m.OrderAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.FragOrderAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.MagicActionAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.UnitMagicActionAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.ObjectMagicActionAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.KnowledgeGroupMagicActionAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), ack.GetErrorMsg(), code.String())
	} else if ack := m.SetAutomatModeAck; ack != nil {
		code := ack.GetErrorCode()
		err = processAck(int32(code), code.String(), code.String())
	}
	errors <- err
	return true
}

func isOrder(msg *sword.ClientToSim) bool {
	m := msg.GetMessage()
	switch {
	case m.UnitOrder != nil:
		return true
	case m.AutomatOrder != nil:
		return true
	case m.CrowdOrder != nil:
		return true
	case m.FragOrder != nil:
		return true
	}
	return false
}

func (d *SwordData) tryRemove(uuid string) (bool, string) {
	previous := len(d.pending)
	delete(d.pending, uuid)
	removed := previous != len(d.pending)
	if removed {
		return true, ""
	}
	return false, " (obsolete)"
}

func (s *Sword) Apply(uuid string, url url.URL, payload []byte) {
	msg := swapi.SwordMessage{
		ClientToSimulation: &sword.ClientToSim{},
	}
	err := proto.Unmarshal(payload, msg.ClientToSimulation)
	if err != nil {
		s.Log("invalid payload: %v", err)
		go s.root.OnApply(uuid, err, false)
		return
	}
	data, _ := json.Marshal(msg.ClientToSimulation.GetMessage())
	action := NewAction(url, payload, isOrder(msg.ClientToSimulation))
	s.mutex.Lock()
	s.d.pending[uuid] = action
	s.mutex.Unlock()
	// must have at least 1 buffer because we can abort it AND receive
	// an error from swapi handler
	errors := make(chan error, 1)
	s.Log("-> action %v %s", uuid, data)
	go s.postAction(errors, msg)
	err = <-errors
	s.mutex.Lock()
	defer s.mutex.Unlock()
	// ignore updates unless our action has been removed
	// it means it has been applied by someone else already
	if err == nil {
		removed, suffix := s.d.tryRemove(uuid)
		s.Log("<- action %v%v", uuid, suffix)
		if removed {
			go s.root.OnApply(uuid, err, action.lock)
		}
		return
	}
	// ignore any error which does not
	// come from the server directly
	retry := swapi.IsConnectionError(err) ||
		err == swapi.ErrConnectionClosed ||
		err == ErrRetry ||
		err == ErrMissingLink
	retry = retry && !s.d.stopped
	suffix := ""
	removed := false
	if retry {
		suffix = " (will retry)"
	} else {
		removed, suffix = s.d.tryRemove(uuid)
	}
	s.Log("<- action %v error: %v%v", uuid, err, suffix)
	if !retry && removed {
		go s.root.OnApply(uuid, err, action.lock)
		return
	}
}

func (s *Sword) getClient(retry bool) (*swapi.Client, *sync.WaitGroup, error) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	if retry && s.d.retry > 0 {
		return nil, nil, ErrRetry
	}
	if s.d.input == nil {
		return nil, nil, ErrMissingLink
	}
	s.d.group.Add(1)
	return s.d.input, s.d.group, nil
}

func (s *Sword) postAction(errors chan<- error, msg swapi.SwordMessage) {
	client, group, err := s.getClient(true)
	if err != nil {
		errors <- err
		return
	}
	defer group.Done()
	client.PostWithTimeout(msg, func(msg *swapi.SwordMessage, id, context int32, err error) bool {
		return s.waitAck(errors, msg, id, context, err)
	}, 5*time.Minute)
}

func isSwordEvent(event *sdk.Event) bool {
	action := event.GetAction()
	url, err := url.Parse(action.GetTarget())
	if err != nil {
		return false
	}
	if url.Scheme != "sword" {
		return false
	}
	return len(action.GetPayload()) > 0
}

func (s *Sword) Update(events ...*sdk.Event) {
	for _, event := range events {
		if !isSwordEvent(event) {
			continue
		}
		msg := swapi.SwordMessage{}
		payload := event.GetAction().GetPayload()
		err := swapi.DecodeMessage(&msg, swapi.ClientToSimulationTag, payload)
		if err != nil {
			s.Log("unable to decode payload %v: %v", string(payload), err)
			continue
		}
		s.mutex.Lock()
		s.d.events[event.GetUuid()] = &msg
		s.mutex.Unlock()
	}
}

func (s *Sword) Delete(events ...string) {
	s.mutex.Lock()
	for _, uuid := range events {
		delete(s.d.events, uuid)
	}
	s.mutex.Unlock()
}

type Ids map[uint32]struct{}

func isEmptyTasker(tasker *sword.Tasker) bool {
	return tasker.GetUnit().GetId() == 0 &&
		tasker.GetAutomat().GetId() == 0 &&
		tasker.GetFormation().GetId() == 0 &&
		tasker.GetParty().GetId() == 0 &&
		tasker.GetCrowd().GetId() == 0 &&
		tasker.GetPopulation().GetId() == 0
}

func isTaskerInProfile(tasker *sword.Tasker, data *swapi.ModelData, profile *swapi.Profile, units, inhabitants Ids) bool {
	if isEmptyTasker(tasker) {
		return true
	}
	if tasker.Population != nil {
		if _, ok := inhabitants[tasker.Population.GetId()]; ok {
			return true
		}
	} else if tasker.Unit != nil {
		if _, ok := units[tasker.Unit.GetId()]; ok {
			return true
		}
	}
	return data.IsTaskerInProfile(tasker, profile)
}

var (
	clientMagicActions = map[sword.UnitMagicAction_Type]struct{}{
		sword.UnitMagicAction_log_finish_handlings:      struct{}{},
		sword.UnitMagicAction_log_supply_change_quotas:  struct{}{},
		sword.UnitMagicAction_log_supply_pull_flow:      struct{}{},
		sword.UnitMagicAction_log_supply_push_flow:      struct{}{},
		sword.UnitMagicAction_unit_change_superior:      struct{}{},
		sword.UnitMagicAction_change_knowledge_group:    struct{}{},
		sword.UnitMagicAction_change_logistic_links:     struct{}{},
		sword.UnitMagicAction_change_formation_superior: struct{}{},
		sword.UnitMagicAction_change_automat_superior:   struct{}{},
	}
)

func (s *Sword) getEvent(uuid string) *swapi.SwordMessage {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	// works because we never write/update current events
	// only add or overwrite with a new pointer
	return s.d.events[uuid]
}

func (s *Sword) filterProfile(data *swapi.ModelData, profile *swapi.Profile, units, inhabitants Ids, event *sdk.Event) bool {
	decoded := s.getEvent(event.GetUuid())
	if decoded == nil {
		return false
	}
	m := decoded.ClientToSimulation.GetMessage()
	switch {
	case m == nil:
		return false
	case m.UnitOrder != nil:
		id := m.UnitOrder.Tasker.GetId()
		if id == 0 {
			return false
		}
		if _, ok := units[id]; ok {
			return false
		}
		return !data.IsUnitInProfile(id, profile)
	case m.AutomatOrder != nil:
		id := m.AutomatOrder.Tasker.GetId()
		return id != 0 && !data.IsAutomatInProfile(id, profile)
	case m.CrowdOrder != nil:
		id := m.CrowdOrder.Tasker.GetId()
		return id != 0 && !data.IsCrowdInProfile(id, profile)
	case m.FragOrder != nil:
		tasker := m.FragOrder.GetTasker()
		return !isTaskerInProfile(tasker, data, profile, units, inhabitants)
	case m.MagicAction != nil:
		return !profile.Supervisor
	case m.UnitMagicAction != nil:
		tasker := m.UnitMagicAction.GetTasker()
		actionType := m.UnitMagicAction.GetType()
		isInProfile := isTaskerInProfile(tasker, data, profile, units, inhabitants)
		if _, ok := clientMagicActions[actionType]; ok {
			return !isInProfile
		}
		return !profile.Supervisor || !isInProfile
	case m.ObjectMagicAction != nil:
		id := m.ObjectMagicAction.GetObject().GetId()
		return !profile.Supervisor || id != 0 && !data.IsObjectInProfile(id, profile)
	case m.KnowledgeMagicAction != nil:
		id := m.KnowledgeMagicAction.GetKnowledgeGroup().GetId()
		return !profile.Supervisor || id != 0 && !data.IsKnowledgeGroupInProfile(id, profile)
	case m.SetAutomatMode != nil:
		id := m.SetAutomatMode.GetAutomate().GetId()
		return id != 0 && !data.IsAutomatInProfile(id, profile)
	}
	// report is not supported yet
	return true
}

func (s *Sword) filterEngaged(event *sdk.Event) bool {
	decoded := s.getEvent(event.GetUuid())
	if decoded == nil {
		return false
	}
	m := decoded.ClientToSimulation.GetMessage()
	return m != nil && m.UnitOrder != nil && m.UnitOrder.GetParent() != 0
}

func getEngaged(config EventFilterConfig) bool {
	engaged, ok := config["sword_filter_engaged"].(bool)
	if !ok {
		return false
	}
	return engaged
}

func getProfile(config EventFilterConfig) string {
	name, _ := config["sword_profile"].(string)
	return name
}

func getCustomProfile(config EventFilterConfig) (*swapi.Profile, Ids, Ids) {
	filters, ok := config["sword_filter"].(map[string]map[uint32]struct{})
	if !ok {
		return nil, nil, nil
	}
	return &swapi.Profile{
		Login:               "custom",
		Supervisor:          false,
		ReadOnlyFormations:  map[uint32]struct{}{},
		ReadWriteFormations: filters["formations"],
		ReadOnlyAutomats:    map[uint32]struct{}{},
		ReadWriteAutomats:   filters["automats"],
		ReadOnlyParties:     map[uint32]struct{}{},
		ReadWriteParties:    filters["parties"],
		ReadOnlyCrowds:      map[uint32]struct{}{},
		ReadWriteCrowds:     filters["crowds"],
	}, filters["units"], filters["inhabitants"]
}

func (s *Sword) addProfileFilter(dst *[]EventFilter, data *swapi.ModelData, config EventFilterConfig) {
	name := getProfile(config)
	if len(name) == 0 {
		return
	}
	profile, ok := data.Profiles[name]
	if !ok {
		return
	}
	*dst = append(*dst, func(event *sdk.Event) bool {
		return s.filterProfile(data, profile, Ids{}, Ids{}, event)
	})
}

func (s *Sword) addCustomFilter(dst *[]EventFilter, data *swapi.ModelData, config EventFilterConfig) {
	profile, units, inhabitants := getCustomProfile(config)
	if profile == nil {
		return
	}
	*dst = append(*dst, func(event *sdk.Event) bool {
		return s.filterProfile(data, profile, units, inhabitants, event)
	})
}

func (s *Sword) addEngagedFilter(dst *[]EventFilter, config EventFilterConfig) {
	if getEngaged(config) {
		*dst = append(*dst, func(event *sdk.Event) bool {
			return s.filterEngaged(event)
		})
	}
}

func (s *Sword) addFilters(data *swapi.ModelData, config EventFilterConfig) []EventFilter {
	filters := []EventFilter{}
	s.addProfileFilter(&filters, data, config)
	s.addCustomFilter(&filters, data, config)
	s.addEngagedFilter(&filters, config)
	return filters
}

func (s *Sword) GetFilters(config EventFilterConfig) []EventFilter {
	s.mutex.Lock()
	if s.d.input == nil {
		s.mutex.Unlock()
		return []EventFilter{}
	}
	d := s.d.input.Model.GetData()
	s.mutex.Unlock()
	return s.addFilters(d, config)
}

func (s *Sword) Filter(event *sdk.Event, config EventFilterConfig) bool {
	client, group, err := s.getClient(false)
	if err != nil {
		return false
	}
	defer group.Done()
	return client.Model.Query(func(data *swapi.ModelData) bool {
		for _, filter := range s.addFilters(data, config) {
			if filter(event) {
				return true
			}
		}
		return false
	})
}

func (s *Sword) WaitFor(operand func(d *swapi.ModelData) bool) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	if s.d.input != nil {
		s.d.input.Model.WaitCondition(operand)
	}
}
