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

func (model *ModelData) handleAarInformation(m *sword.AarToClient_Content) error {
	mm := m.GetAarInformation()
	if mm == nil {
		return ErrSkipHandler
	}
	scores, err := ReadScores([]byte(mm.GetInformation()))
	if err != nil {
		return err
	}
	model.KnownScores = scores
	return nil
}

func (model *ModelData) handleIndicator(m *sword.AarToClient_Content) error {
	mm := m.GetIndicator()
	if mm == nil {
		return ErrSkipHandler
	}
	model.Scores[mm.GetName()] = mm.GetValue()
	return nil
}
