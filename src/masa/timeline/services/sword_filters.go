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
	"encoding/json"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/timeline/sdk"
)

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
			s.Log("unable to decode metadata on event %v: metadata:%v error:%v\n",
				id, src, err)
			metadata = sdk.Metadata{}
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
