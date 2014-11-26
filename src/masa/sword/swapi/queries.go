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

func hasId(ro, rw map[uint32]struct{}, id uint32) bool {
	_, ok := ro[id]
	if !ok {
		_, ok = rw[id]
	}
	return ok
}

func (d *ModelData) isEntityInProfile(id uint32, profile *Profile, noside bool) bool {
	done := map[uint32]struct{}{}
	for id != 0 {
		size := len(done)
		done[id] = struct{}{}
		// prevent potential cycles
		if size == len(done) {
			return false
		}
		group, ok := d.Hierarchies[id]
		if !ok {
			return false
		}
		if hasId(profile.ReadOnlyAutomats, profile.ReadWriteAutomats, id) ||
			hasId(profile.ReadOnlyFormations, profile.ReadWriteFormations, id) ||
			hasId(profile.ReadOnlyParties, profile.ReadWriteParties, id) ||
			hasId(profile.ReadOnlyCrowds, profile.ReadWriteCrowds, id) {
			return true
		}
		// find closest hierarchy
		last := Hierarchy{}
		for _, hierarchy := range group {
			if last.Id == 0 || int32(hierarchy.Tick) <= d.Tick && hierarchy.Tick >= last.Tick {
				last = hierarchy
			}
			if int32(hierarchy.Tick) > d.Tick {
				break
			}
		}
		// no side entities without parents are always in profile
		if noside && last.Parent == 0 {
			return true
		}
		// try again with its parent
		id = last.Parent
		// disable noside special case on parents
		noside = false
	}
	return false
}

func (d *ModelData) IsUnitInProfile(id uint32, profile *Profile) bool {
	return d.isEntityInProfile(id, profile, false)
}

func (d *ModelData) IsAutomatInProfile(id uint32, profile *Profile) bool {
	return d.isEntityInProfile(id, profile, false)
}

func (d *ModelData) IsFormationInProfile(id uint32, profile *Profile) bool {
	return d.isEntityInProfile(id, profile, false)
}

func (d *ModelData) IsPopulationInProfile(id uint32, profile *Profile) bool {
	return d.isEntityInProfile(id, profile, false)
}

func (d *ModelData) IsCrowdInProfile(id uint32, profile *Profile) bool {
	return d.isEntityInProfile(id, profile, false)
}

func (d *ModelData) IsPartyInProfile(id uint32, profile *Profile) bool {
	return d.isEntityInProfile(id, profile, false)
}

func (d *ModelData) IsTaskerInProfile(tasker *sword.Tasker, profile *Profile) bool {
	switch {
	case tasker.Unit != nil:
		return d.isEntityInProfile(tasker.Unit.GetId(), profile, false)
	case tasker.Automat != nil:
		return d.isEntityInProfile(tasker.Automat.GetId(), profile, false)
	case tasker.Formation != nil:
		return d.isEntityInProfile(tasker.Formation.GetId(), profile, false)
	case tasker.Party != nil:
		return d.isEntityInProfile(tasker.Party.GetId(), profile, false)
	case tasker.Crowd != nil:
		return d.isEntityInProfile(tasker.Crowd.GetId(), profile, false)
	case tasker.Population != nil:
		return d.isEntityInProfile(tasker.Population.GetId(), profile, false)
	}
	return false
}

func (d *ModelData) IsKnowledgeGroupInProfile(id uint32, profile *Profile) bool {
	return d.isEntityInProfile(id, profile, false)
}

func (d *ModelData) IsObjectInProfile(id uint32, profile *Profile) bool {
	return d.isEntityInProfile(id, profile, true)
}
