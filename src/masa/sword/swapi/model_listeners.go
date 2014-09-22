// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swapi

type ModelEventTag int

const (
	AutomatCreate ModelEventTag = iota
	AutomatDelete
	AutomatReparent
	AutomatUpdate
	CrowdCreate
	CrowdDelete
	CrowdUpdate
	FormationCreate
	FormationDelete
	FormationReparent
	FormationUpdate
	PartyCreate
	PartyUpdate
	PopulationCreate
	PopulationUpdate
	ProfileCreate
	ProfileDelete
	ProfileUpdate
	UnitCreate
	UnitDelete
	UnitReparent
	UnitUpdate
)

type ModelEvent struct {
	Tag  ModelEventTag
	Id   uint32
	Name string
}

type ModelListener func(ModelEvent)

type ModelListeners struct {
	id        int32
	listeners map[int32]ModelListener
}

func NewModelListeners() *ModelListeners {
	return &ModelListeners{
		listeners: map[int32]ModelListener{},
	}
}

func (m *ModelListeners) Register(listener ModelListener) int32 {
	m.id++
	m.listeners[m.id] = listener
	return m.id
}

func (m *ModelListeners) Unregister(id int32) {
	delete(m.listeners, id)
}

func (m *ModelListeners) Notify(event ModelEvent) {
	if m == nil {
		return
	}
	for _, listener := range m.listeners {
		listener(event)
	}
}

func (m *ModelListeners) NotifyId(tag ModelEventTag, id uint32) {
	m.Notify(ModelEvent{
		Tag: tag,
		Id:  id,
	})
}

func (m *ModelListeners) NotifyName(tag ModelEventTag, name string) {
	m.Notify(ModelEvent{
		Tag:  tag,
		Name: name,
	})
}
