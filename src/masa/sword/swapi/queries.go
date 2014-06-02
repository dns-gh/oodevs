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
	"masa/sword/sword"
)

func (model *Model) Query(query func(d *ModelData) bool) bool {
	reply := false
	model.waitCommand(func(m *Model) {
		reply = query(m.data)
	})
	return reply
}

func (d *ModelData) IsUnitInProfile(id uint32, profile *Profile) bool {
	unit, ok := d.Units[id]
	if !ok {
		return false
	}
	return d.IsAutomatInProfile(unit.AutomatId, profile)
}

func hasId(ro, rw map[uint32]struct{}, id uint32) bool {
	_, ok := ro[id]
	if !ok {
		_, ok = rw[id]
	}
	return ok
}

func (d *ModelData) IsAutomatInProfile(id uint32, profile *Profile) bool {
	if hasId(profile.ReadOnlyAutomats, profile.ReadWriteAutomats, id) {
		return true
	}
	automat, ok := d.Automats[id]
	return ok && d.IsFormationInProfile(automat.FormationId, profile)
}

func (d *ModelData) IsFormationInProfile(id uint32, profile *Profile) bool {
	if hasId(profile.ReadOnlyFormations, profile.ReadWriteFormations, id) {
		return true
	}
	formation, ok := d.Formations[id]
	if !ok {
		return false
	}
	if formation.ParentId != 0 {
		return d.IsFormationInProfile(formation.ParentId, profile)
	}
	return d.IsPartyInProfile(formation.PartyId, profile)
}

func (d *ModelData) IsPopulationInProfile(id uint32, profile *Profile) bool {
	population, ok := d.Populations[id]
	return ok && d.IsPartyInProfile(population.PartyId, profile)
}

func (d *ModelData) IsCrowdInProfile(id uint32, profile *Profile) bool {
	if hasId(profile.ReadOnlyCrowds, profile.ReadWriteCrowds, id) {
		return true
	}
	crowd, ok := d.Crowds[id]
	return ok && d.IsPartyInProfile(crowd.PartyId, profile)
}

func (d *ModelData) IsPartyInProfile(id uint32, profile *Profile) bool {
	return hasId(profile.ReadOnlyParties, profile.ReadWriteParties, id)
}

func (d *ModelData) IsTaskerInProfile(tasker *sword.Tasker, profile *Profile) bool {
	switch {
	case tasker.Unit != nil:
		return d.IsUnitInProfile(tasker.Unit.GetId(), profile)
	case tasker.Automat != nil:
		return d.IsAutomatInProfile(tasker.Automat.GetId(), profile)
	case tasker.Formation != nil:
		return d.IsFormationInProfile(tasker.Formation.GetId(), profile)
	case tasker.Party != nil:
		return d.IsPartyInProfile(tasker.Party.GetId(), profile)
	case tasker.Crowd != nil:
		return d.IsCrowdInProfile(tasker.Crowd.GetId(), profile)
	case tasker.Population != nil:
		return d.IsPopulationInProfile(tasker.Population.GetId(), profile)
	}
	return false
}

func (d *ModelData) IsKnowledgeGroupInProfile(id uint32, profile *Profile) bool {
	knowledgeGroup, ok := d.KnowledgeGroups[id]
	return ok && d.IsPartyInProfile(knowledgeGroup.PartyId, profile)
}

func (d *ModelData) IsObjectInProfile(id uint32, profile *Profile) bool {
	object, ok := d.Objects[id]
	return ok && (object.PartyId == 0 || d.IsPartyInProfile(object.PartyId, profile))
}
