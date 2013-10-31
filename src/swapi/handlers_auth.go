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
	"fmt"
	"sword"
)

func (model *Model) handleProfileCreation(m *sword.AuthenticationToClient_Content) error {
	mm := m.GetProfileCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	profile := NewProfile(mm.GetProfile())
	if !model.data.addProfile(profile) {
		return fmt.Errorf("cannot insert profile: %s", profile.Login)
	}
	return nil
}

func (model *Model) handleProfileUpdate(m *sword.AuthenticationToClient_Content) error {
	mm := m.GetProfileUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	profile := NewProfile(mm.GetProfile())
	if !model.data.updateProfile(mm.GetLogin(), profile) {
		return fmt.Errorf("cannot find profile to update: %s", profile.Login)
	}
	return nil
}

func (model *Model) handleProfileDestruction(m *sword.AuthenticationToClient_Content) error {
	mm := m.GetProfileDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.removeProfile(mm.GetLogin()) {
		return fmt.Errorf("cannot find profile to remove: %s",
			mm.GetLogin())
	}
	return nil
}
