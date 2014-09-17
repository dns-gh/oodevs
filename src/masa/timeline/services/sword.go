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
	"bytes"
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
	"errors"
	"fmt"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/timeline/sdk"
	"masa/timeline/util"
	"net/url"
	"time"
)

type SwordStatus int

const (
	SwordStatusDisconnected = iota
	SwordStatusConnecting
	SwordStatusConnected
)

var (
	SwordTimeLayout = "20060102T150405"
	ErrDisconnected = errors.New("disconnected")
	ErrInProgress   = errors.New("in progress")
	ErrSkipped      = errors.New("skipped")
	ErrUnknown      = errors.New("unknown")
)

type Action struct {
	id      string
	url     url.URL
	payload []byte
	msg     swapi.SwordMessage
	lock    bool
}

func NewAction(id string, url url.URL, payload []byte,
	msg swapi.SwordMessage, lock bool) *Action {
	return &Action{
		id:      id,
		url:     url,
		payload: payload,
		msg:     msg,
		lock:    lock,
	}
}

type Ids map[uint32]struct{}

type Sword struct {
	log      util.Logger
	observer Observer // attached observer
	name     string   // service name
	address  string   // sword address
	clock    bool     // whether we use sword clock
	// mutable
	events    map[string]swapi.SwordMessage // decoded messages
	pending   map[string]*Action            // pending actions
	metadata  map[string]*sdk.Metadata      // decoded metadata
	autostart bool                          // auto connect when disconnected
	retry     uint                          // retry count
	link      *SwordLink                    // current sword link
	status    SwordStatus                   // current status
	orders    Ids                           // known order ids
	actions   Ids                           // known action ids
}

func NewSword(log util.Logger, observer Observer, clock bool, name, address string) *Sword {
	return &Sword{
		log:      log,
		observer: observer,
		name:     name,
		address:  address,
		clock:    clock,
		events:   map[string]swapi.SwordMessage{},
		metadata: map[string]*sdk.Metadata{},
		pending:  map[string]*Action{},
		orders:   Ids{},
		actions:  Ids{},
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

// Sword is managed as a state machine with three states as in SwordStatus
// There are four possible transitions, Start, Stop, Attach

type SwordHandler interface {
	PostAttach(link *SwordLink)
	PostLog(link *SwordLink, format string, args ...interface{})
	PostRestart(link *SwordLink, err error)
	PostCloseAction(link *SwordLink, action string, err error)
	PostCloseWriter(link *SwordLink, writer *SwordWriter)
}

func (s *Sword) PostRestart(link *SwordLink, err error) {
	s.observer.Post(func() {
		if err != nil {
			s.Log("network error: %v", err)
		}
		s.restart(link)
	})
}

func (s *Sword) PostAttach(link *SwordLink) {
	s.observer.Post(func() {
		s.attach(link)
	})
}

func (s *Sword) PostLog(link *SwordLink, format string, args ...interface{}) {
	s.observer.Post(func() {
		if link == s.link {
			s.Log(format, args...)
		}
	})
}

func (s *Sword) PostCloseAction(link *SwordLink, action string, err error) {
	s.observer.Post(func() {
		s.closeAction(link, action, err)
	})
}

func (s *Sword) PostCloseWriter(link *SwordLink, writer *SwordWriter) {
	s.observer.Post(func() {
		link.CloseWriter(writer)
	})
}

func (s *Sword) Start() error {
	s.autostart = true
	return s.start()
}

func (s *Sword) setStatus(status SwordStatus, restart bool) {
	s.status = status
	if status == SwordStatusDisconnected {
		if !restart {
			// abort all pending actions
			for _, action := range s.pending {
				s.observer.CloseEvent(action.id, ErrDisconnected, action.lock)
			}
			s.pending = map[string]*Action{}
		}
		if s.autostart {
			s.start()
		}
	} else if status == SwordStatusConnected {
		for _, action := range s.pending {
			s.link.PostAction(action)
		}
	}
}

func (s *Sword) getRetryTimeout() time.Duration {
	s.retry++
	// 1 -> first try, no timeout
	// 2 -> first reconnect, no timeout
	// 3 -> begin at 1 second and double each time
	if s.retry <= 2 {
		return 0 * time.Second
	}
	sleep := 1 << (s.retry - 3) * time.Second
	if sleep > time.Minute {
		s.retry-- // prevent cycles
		sleep = time.Minute
	}
	return sleep
}

func (s *Sword) start() error {
	if s.status == SwordStatusConnecting {
		return ErrInProgress
	} else if s.status == SwordStatusConnected {
		return ErrSkipped
	}
	if !s.autostart {
		return ErrSkipped
	}
	timeout := s.getRetryTimeout()
	prefix := ""
	if s.link != nil {
		prefix = "re"
	}
	suffix := ""
	if d := timeout.Seconds(); d > 0 {
		suffix = fmt.Sprintf(" in %vs", d)
	}
	s.Log("%sconnecting%s", prefix, suffix)
	s.setStatus(SwordStatusConnecting, false)
	go func() {
		time.Sleep(timeout)
		asyncConnect(s, s.address, s.name, s.clock)
	}()
	return nil
}

func logConnect(handler SwordHandler, err error) {
	if err != nil {
		handler.PostLog(nil, "connection failed: %s", err)
	} else {
		handler.PostLog(nil, "connected")
	}
}

func asyncConnect(handler SwordHandler, address, name string, clock bool) {
	link, err := NewSwordLink(handler, address, name, clock)
	if err != nil {
		logConnect(handler, err)
		link = nil
	}
	handler.PostAttach(link)
}

func (s *Sword) attach(link *SwordLink) {
	if s.status != SwordStatusConnecting {
		if link != nil {
			link.Kill()
		}
		return
	}
	if link == nil {
		s.setStatus(SwordStatusDisconnected, true)
		return
	}
	if s.link != nil {
		// now that we have something new
		// we can kill the old model
		s.link.Kill()
	}
	s.link = link
	// Known orders & actions are preserved across connections
	link.Attach(s.observer, s.orders, s.actions)
	s.retry = 0
	s.Log("connected")
	s.setStatus(SwordStatusConnected, false)
}

func (s *Sword) Stop() error {
	s.autostart = false
	if s.status == SwordStatusDisconnected {
		return ErrSkipped
	}
	// SwordStatusConnecting:
	// s.link == nil from start
	// s.link == xxx from restart
	// SwordStatusConnected:
	// s.link = xxx
	if s.link == nil {
		s.setStatus(SwordStatusDisconnected, false)
		return nil
	}
	link := s.link
	s.link = nil
	link.Detach()
	s.Log("disconnected")
	s.setStatus(SwordStatusDisconnected, false)
	return nil
}

func (s *Sword) restart(link *SwordLink) {
	if s.status != SwordStatusConnected &&
		s.link != link {
		// ignore any restart in flowing state
		return
	}
	// lie and tell we are disconnected
	// do not touch the current link, as
	// we need to keep the model alive
	// until we have a new one
	s.setStatus(SwordStatusDisconnected, true)
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

func (s *Sword) Apply(uuid string, url url.URL, payload []byte) error {
	msg, ok := s.events[uuid]
	if !ok {
		return ErrUnknown
	}
	if _, ok := s.pending[uuid]; ok {
		return ErrInProgress
	}
	data, err := json.Marshal(msg.ClientToSimulation.GetMessage())
	if err != nil {
		data = []byte(err.Error())
	}
	s.Log("-> action %v %s", uuid, data)
	action := NewAction(uuid, url, payload, msg, isOrder(msg.ClientToSimulation))
	s.pending[uuid] = action
	if s.status == SwordStatusConnected {
		s.link.PostAction(action)
	}
	return nil
}

func (s *Sword) closeAction(link *SwordLink, id string, err error) {
	// Discard actions when we're not in connected state
	// It should not happen and would not fix potential errors
	// when our order has been applied by the server but the ack
	// has not been received
	if s.status != SwordStatusConnected || s.link != link {
		return
	}
	action, ok := s.pending[id]
	if !ok {
		return
	}
	delete(s.pending, id)
	s.observer.CloseEvent(id, err, action.lock)
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

func (s *Sword) cacheEvent(event *sdk.Event, overwrite bool) *swapi.SwordMessage {
	id := event.GetUuid()
	if !overwrite {
		if msg, ok := s.events[id]; ok {
			return &msg
		}
	}
	if !isSwordEvent(event) {
		return nil
	}
	msg := swapi.SwordMessage{}
	payload := event.GetAction().GetPayload()
	err := swapi.DecodeMessage(&msg, swapi.ClientToSimulationTag, payload)
	if err != nil {
		s.Log("unable to decode payload %v: %v", string(payload), err)
		return nil
	}
	s.events[id] = msg
	return &msg
}

func (s *Sword) Update(events ...*sdk.Event) {
	for _, event := range events {
		s.cacheEvent(event, true)
		s.cacheMetadata(event, true)
	}
}

func (s *Sword) Delete(events ...string) {
	for _, uuid := range events {
		delete(s.events, uuid)
		delete(s.metadata, uuid)
	}
}

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
		sword.UnitMagicAction_log_finish_handlings:      {},
		sword.UnitMagicAction_log_supply_change_quotas:  {},
		sword.UnitMagicAction_log_supply_pull_flow:      {},
		sword.UnitMagicAction_log_supply_push_flow:      {},
		sword.UnitMagicAction_unit_change_superior:      {},
		sword.UnitMagicAction_change_knowledge_group:    {},
		sword.UnitMagicAction_change_logistic_links:     {},
		sword.UnitMagicAction_change_formation_superior: {},
		sword.UnitMagicAction_change_automat_superior:   {},
	}
)

func (s *Sword) filterProfile(data *swapi.ModelData, profile *swapi.Profile, units, inhabitants Ids, event *sdk.Event) bool {
	decoded := s.cacheEvent(event, false)
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
		if _, ok := clientMagicActions[sword.UnitMagicAction_Type(actionType)]; ok {
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
	decoded := s.cacheEvent(event, false)
	if decoded == nil {
		return false
	}
	m := decoded.ClientToSimulation.GetMessage()
	return m != nil && m.UnitOrder != nil && m.UnitOrder.GetParent() != 0
}

func (s *Sword) cacheMetadata(event *sdk.Event, overwrite bool) *sdk.Metadata {
	id := event.GetUuid()
	if !overwrite {
		if metadata, ok := s.metadata[id]; ok {
			return metadata
		}
	}
	metadata := sdk.Metadata{}
	src := event.GetMetadata()
	if len(src) != 0 {
		err := json.NewDecoder(bytes.NewBufferString(src)).Decode(&metadata)
		if err != nil {
			return nil
		}
	}
	s.metadata[id] = &metadata
	return &metadata
}

func (s *Sword) filterMetadata(data *swapi.ModelData, profile *swapi.Profile, event *sdk.Event) bool {
	metadata := s.cacheMetadata(event, false)
	if metadata == nil {
		return false
	}
	id := metadata.GetSwordEntity()
	if id == 0 {
		return false
	}
	return !data.IsUnitInProfile(id, profile) &&
		!data.IsAutomatInProfile(id, profile) &&
		!data.IsFormationInProfile(id, profile) &&
		!data.IsPartyInProfile(id, profile) &&
		!data.IsCrowdInProfile(id, profile) &&
		!data.IsPopulationInProfile(id, profile)
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
		return s.filterProfile(data, profile, Ids{}, Ids{}, event) ||
			s.filterMetadata(data, profile, event)
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
	if s.link == nil {
		return []EventFilter{}
	}
	d := s.link.client.Model.GetData()
	return s.addFilters(d, config)
}

func (s *Sword) Filter(event *sdk.Event, config EventFilterConfig) bool {
	if s.link == nil {
		return false
	}
	return s.link.client.Model.Query(func(data *swapi.ModelData) bool {
		for _, filter := range s.addFilters(data, config) {
			if filter(event) {
				return true
			}
		}
		return false
	})
}

// Custom test functions
func (s *Sword) waitFor(operand func() bool) {
	reply := make(chan bool, 1)
	for {
		s.observer.Post(func() {
			reply <- operand()
		})
		if <-reply {
			return
		}
		time.Sleep(100 * time.Millisecond)
	}
}

func (s *Sword) WaitFor(operand func(d *swapi.ModelData) bool) {
	// a bit convoluted because we want to block, but only this
	// function and not gosword model, and we must run inside
	// the observer event loop to use session data
	s.waitFor(func() bool {
		if s.link == nil {
			return false
		}
		return s.link.client.Model.Query(operand)
	})
}

func (s *Sword) WaitForRetry(retry uint) {
	s.waitFor(func() bool {
		return s.retry >= retry
	})
}

func (s *Sword) WaitForStatus(status SwordStatus) {
	s.waitFor(func() bool {
		return s.status == status
	})
}
