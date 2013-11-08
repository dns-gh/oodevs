// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swapi

func (model *Model) query(query func(d *ModelData) bool) bool {
	reply := make(chan bool, 1)
	model.waitCommand(func(m *Model) {
		reply <- query(m.data)
	})
	return <-reply
}

func (d *ModelData) isUnitInProfile(id uint32, profile *Profile) bool {
	unit, ok := d.Units[id]
	if !ok {
		return false
	}
	return d.isAutomatInProfile(unit.AutomatId, profile)
}

func hasId(ro, rw map[uint32]struct{}, id uint32) bool {
	_, ok := ro[id]
	if !ok {
		_, ok = rw[id]
	}
	return ok
}

func (d *ModelData) isAutomatInProfile(id uint32, profile *Profile) bool {
	if hasId(profile.ReadOnlyAutomats, profile.ReadWriteAutomats, id) {
		return true
	}
	automat, ok := d.Automats[id]
	return ok && d.isFormationInProfile(automat.FormationId, profile)
}

func (d *ModelData) isFormationInProfile(id uint32, profile *Profile) bool {
	if hasId(profile.ReadOnlyFormations, profile.ReadWriteFormations, id) {
		return true
	}
	formation, ok := d.Formations[id]
	if !ok {
		return false
	}
	if formation.ParentId != 0 {
		return d.isFormationInProfile(formation.ParentId, profile)
	}
	return d.isPartyInProfile(formation.PartyId, profile)
}

func (d *ModelData) isPopulationInProfile(id uint32, profile *Profile) bool {
	population, ok := d.Populations[id]
	return ok && d.isPartyInProfile(population.PartyId, profile)
}

func (d *ModelData) isCrowdInProfile(id uint32, profile *Profile) bool {
	if hasId(profile.ReadOnlyCrowds, profile.ReadWriteCrowds, id) {
		return true
	}
	crowd, ok := d.Crowds[id]
	return ok && d.isPartyInProfile(crowd.PartyId, profile)
}

func (d *ModelData) isPartyInProfile(id uint32, profile *Profile) bool {
	return hasId(profile.ReadOnlyParties, profile.ReadWriteParties, id)
}

func (model *Model) queryIdInProfile(id uint32, name string, query func(*ModelData, uint32, *Profile) bool) bool {
	return model.query(func(d *ModelData) bool {
		profile, ok := d.Profiles[name]
		return ok && query(d, id, profile)
	})
}

func (model *Model) IsUnitInProfile(id uint32, name string) bool {
	return model.queryIdInProfile(id, name, (*ModelData).isUnitInProfile)
}

func (model *Model) IsAutomatInProfile(id uint32, name string) bool {
	return model.queryIdInProfile(id, name, (*ModelData).isAutomatInProfile)
}

func (model *Model) IsFormationInProfile(id uint32, name string) bool {
	return model.queryIdInProfile(id, name, (*ModelData).isFormationInProfile)
}

func (model *Model) IsPopulationInProfile(id uint32, name string) bool {
	return model.queryIdInProfile(id, name, (*ModelData).isPopulationInProfile)
}

func (model *Model) IsCrowdInProfile(id uint32, name string) bool {
	return model.queryIdInProfile(id, name, (*ModelData).isCrowdInProfile)
}

func (model *Model) IsPartyInProfile(id uint32, name string) bool {
	return model.queryIdInProfile(id, name, (*ModelData).isPartyInProfile)
}
