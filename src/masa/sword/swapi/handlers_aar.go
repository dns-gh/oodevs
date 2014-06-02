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
	"encoding/xml"
	"fmt"
	"masa/sword/sword"
)

func (model *ModelData) handleAarInformation(m *sword.AarToClient_Content) error {
	mm := m.GetAarInformation()
	if mm == nil {
		return ErrSkipHandler
	}
	data := []byte(mm.GetInformation())
	name := xml.Name{}
	err := xml.Unmarshal(data, &name)
	if err != nil {
		return fmt.Errorf("invalid xml in aar information: %s\n", err)
	}
	switch name.Local {
	case "scores":
		scores, err := ReadScores(data)
		if err != nil {
			return err
		}
		model.KnownScores = scores
	}
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
