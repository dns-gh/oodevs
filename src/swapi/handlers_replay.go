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
	"sword"
)

func (model *ModelData) handleControlReplayInformation(
	m *sword.ReplayToClient_Content) error {

	mm := m.GetControlReplayInformation()
	if mm == nil {
		return ErrSkipHandler
	}
	firstTick := int32(1)
	if mm.FirstTick != nil {
		firstTick = *mm.FirstTick
	}
	model.Replay = &ReplayInfo{
		FirstTick: firstTick,
		TickCount: mm.GetTickCount(),
	}
	return nil
}
