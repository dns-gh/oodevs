// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package swapi

import (
	"fmt"
	"sword"
)

func (model *ModelData) handleLimitCreation(m *sword.MessengerToClient_Content) error {
	mm := m.GetLimitCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId().GetId()
	limit := NewTacticalLine(id, mm.GetTacticalLine())
	if !model.addTacticalLine(limit) {
		return fmt.Errorf("cannot insert limit: %s", limit.Name)
	}
	return nil
}

func (model *ModelData) handleLimitUpdate(m *sword.MessengerToClient_Content) error {
	mm := m.GetLimitUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId().GetId()
	limit := NewTacticalLine(id, mm.GetTacticalLine())
	if !model.updateTacticalLine(limit) {
		return fmt.Errorf("cannot find limit to update: %s", limit.Name)
	}
	return nil
}

func (model *ModelData) handleLimitDestruction(m *sword.MessengerToClient_Content) error {
	mm := m.GetLimitDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId().GetId()
	if !model.removeTacticalLine(id) {
		return fmt.Errorf("cannot find limit to remove: %d", id)
	}
	return nil
}

func (model *ModelData) handlePhaseLineCreation(m *sword.MessengerToClient_Content) error {
	mm := m.GetPhaseLineCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId().GetId()
	line := NewTacticalLine(id, mm.GetTacticalLine())
	if !model.addTacticalLine(line) {
		return fmt.Errorf("cannot insert phase line: %s", line.Name)
	}
	return nil
}

func (model *ModelData) handlePhaseLineUpdate(m *sword.MessengerToClient_Content) error {
	mm := m.GetPhaseLineUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId().GetId()
	line := NewTacticalLine(id, mm.GetTacticalLine())
	if !model.updateTacticalLine(line) {
		return fmt.Errorf("cannot find phase line to update: %s", line.Name)
	}
	return nil
}

func (model *ModelData) handlePhaseLineDestruction(m *sword.MessengerToClient_Content) error {
	mm := m.GetPhaseLineDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId().GetId()
	if !model.removeTacticalLine(id) {
		return fmt.Errorf("cannot find phase line to remove: %d", id)
	}
	return nil
}
