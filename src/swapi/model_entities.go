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
	"time"
)

type Point struct {
	X float64
	Y float64
}

func MakePoint(x, y float64) *Point {
	return &Point{X: x, Y: y}
}

type Profile struct {
	Login      string
	Password   string
	Supervisor bool
}

func NewProfile(login, password string, supervisor bool) *Profile {
	return &Profile{
		Login:      login,
		Password:   password,
		Supervisor: supervisor,
	}
}

func (p *Profile) Copy() *Profile {
	return NewProfile(p.Login, p.Password, p.Supervisor)
}

type Population struct {
	Id      uint32
	PartyId uint32
	Name    string
}

func NewPopulation(id, partyId uint32, name string) *Population {
	return &Population{
		Id:      id,
		PartyId: partyId,
		Name:    name,
	}
}

func (p *Population) Copy() *Population {
	return NewPopulation(p.Id, p.PartyId, p.Name)
}

type Crowd struct {
	Id      uint32
	PartyId uint32
	Name    string
}

func NewCrowd(id, partyId uint32, name string) *Crowd {
	return &Crowd{
		Id:      id,
		PartyId: partyId,
		Name:    name,
	}
}

func (c *Crowd) Copy() *Crowd {
	return NewCrowd(c.Id, c.PartyId, c.Name)
}

type Unit struct {
	Id        uint32
	AutomatId uint32
	Name      string
	Pc        bool
	Position  Point
}

func NewUnit(id, automatId uint32, name string, pc bool) *Unit {
	return &Unit{
		Id:        id,
		AutomatId: automatId,
		Name:      name,
		Pc:        pc,
	}
}

func (u *Unit) Copy() *Unit {
	return NewUnit(u.Id, u.AutomatId, u.Name, u.Pc)
}

type Automat struct {
	Id               uint32
	PartyId          uint32
	Name             string
	Automats         map[uint32]*Automat
	Units            map[uint32]*Unit
	Engaged          bool
	KnowledgeGroupId uint32
}

func NewAutomat(id, partyId, knowledgeGroupId uint32, name string) *Automat {
	return &Automat{
		Id:               id,
		PartyId:          partyId,
		Name:             name,
		Automats:         map[uint32]*Automat{},
		Units:            map[uint32]*Unit{},
		Engaged:          true,
		KnowledgeGroupId: knowledgeGroupId,
	}
}

func (a *Automat) Copy() *Automat {
	other := NewAutomat(a.Id, a.PartyId, a.KnowledgeGroupId, a.Name)
	other.Engaged = a.Engaged
	for k, v := range a.Automats {
		other.Automats[k] = v.Copy()
	}
	for k, v := range a.Units {
		other.Units[k] = v.Copy()
	}
	return other
}

type Formation struct {
	Id         uint32
	PartyId    uint32
	ParentId   uint32
	Name       string
	Level      string
	LogLevel   string
	Formations map[uint32]*Formation
	Automats   map[uint32]*Automat
}

func NewFormation(id uint32, name string, parentId uint32,
	partyId uint32, level, logLevel string) *Formation {
	return &Formation{
		Id:         id,
		PartyId:    partyId,
		ParentId:   parentId,
		Name:       name,
		Level:      level,
		LogLevel:   logLevel,
		Formations: map[uint32]*Formation{},
		Automats:   map[uint32]*Automat{},
	}
}

func (formation *Formation) Copy() *Formation {
	f := NewFormation(formation.Id, formation.Name, formation.ParentId,
		formation.PartyId, formation.Level, formation.LogLevel)
	for k, v := range formation.Formations {
		f.Formations[k] = v.Copy()
	}
	for k, v := range formation.Automats {
		f.Automats[k] = v.Copy()
	}
	return f
}

type KnowledgeGroup struct {
	Id       uint32
	Name     string
	PartyId  uint32
	ParentId uint32
}

func NewKnowledgeGroup(id uint32, name string, parentId, partyId uint32) *KnowledgeGroup {
	return &KnowledgeGroup{
		Id:       id,
		PartyId:  partyId,
		ParentId: parentId,
		Name:     name,
	}
}

func (k *KnowledgeGroup) Copy() *KnowledgeGroup {
	return NewKnowledgeGroup(k.Id, k.Name, k.ParentId, k.PartyId)
}

type Party struct {
	Id              uint32
	Name            string
	Formations      map[uint32]*Formation
	Crowds          map[uint32]*Crowd
	Populations     map[uint32]*Population
	KnowledgeGroups map[uint32]*KnowledgeGroup
}

func NewParty(id uint32, name string) *Party {
	return &Party{
		Id:              id,
		Name:            name,
		Formations:      map[uint32]*Formation{},
		Crowds:          map[uint32]*Crowd{},
		Populations:     map[uint32]*Population{},
		KnowledgeGroups: map[uint32]*KnowledgeGroup{},
	}
}

func (party *Party) Copy() *Party {
	p := NewParty(party.Id, party.Name)
	for k, v := range party.Formations {
		p.Formations[k] = v.Copy()
	}
	for k, v := range party.Crowds {
		p.Crowds[k] = v.Copy()
	}
	for k, v := range party.Populations {
		p.Populations[k] = v.Copy()
	}
	for k, v := range party.KnowledgeGroups {
		p.KnowledgeGroups[k] = v.Copy()
	}
	return p
}

type ModelData struct {
	Parties  map[uint32]*Party
	Profiles map[string]*Profile
	// Tick and time of the most recent started tick
	Tick int32
	Time time.Time
}

func NewModelData() *ModelData {
	return &ModelData{
		Parties:  map[uint32]*Party{},
		Profiles: map[string]*Profile{},
	}
}

func (model *ModelData) Copy() *ModelData {
	m := NewModelData()
	m.Tick = model.Tick
	m.Time = model.Time
	for k, v := range model.Parties {
		m.Parties[k] = v.Copy()
	}
	for k, v := range model.Profiles {
		m.Profiles[k] = v.Copy()
	}
	return m
}

func (model *ModelData) FindPartyByName(name string) *Party {
	for _, party := range model.Parties {
		if party.Name == name {
			return party
		}
	}
	return nil
}

func (model *ModelData) ListFormations() []*Formation {
	formations := []*Formation{}
	pendings := []*Formation{}
	for _, p := range model.Parties {
		for _, f := range p.Formations {
			pendings = append(pendings, f)
		}
	}
	for len(pendings) > 0 {
		f := pendings[len(pendings)-1]
		pendings = pendings[:len(pendings)-1]
		for _, v := range f.Formations {
			pendings = append(pendings, v)
		}
		formations = append(formations, f)
	}
	return formations
}

func (model *ModelData) FindFormation(formationId uint32) *Formation {
	for _, f := range model.ListFormations() {
		if f.Id == formationId {
			return f
		}
	}
	return nil
}

func (model *ModelData) addFormation(f *Formation) bool {
	parent := model.FindFormation(f.ParentId)
	if parent != nil {
		parent.Formations[f.Id] = f
		return true
	}
	party, ok := model.Parties[f.PartyId]
	if ok {
		party.Formations[f.Id] = f
		return true
	}
	return false
}

func (model *ModelData) ListAutomats() []*Automat {
	automats := []*Automat{}
	pendings := []*Automat{}
	for _, f := range model.ListFormations() {
		for _, a := range f.Automats {
			pendings = append(pendings, a)
		}
	}
	for len(pendings) > 0 {
		a := pendings[len(pendings)-1]
		pendings = pendings[:len(pendings)-1]
		for _, child := range a.Automats {
			pendings = append(pendings, child)
		}
		automats = append(automats, a)
	}
	return automats
}

func (model *ModelData) FindAutomat(id uint32) *Automat {
	for _, a := range model.ListAutomats() {
		if a.Id == id {
			return a
		}
	}
	return nil
}

func (model *ModelData) addAutomat(automatId, formationId uint32, a *Automat) bool {
	if parent := model.FindAutomat(automatId); parent != nil {
		parent.Automats[a.Id] = a
		return true
	}
	if parent := model.FindFormation(formationId); parent != nil {
		parent.Automats[a.Id] = a
		return true
	}
	return false
}

func (model *ModelData) ListUnits() []*Unit {
	units := []*Unit{}
	for _, a := range model.ListAutomats() {
		for _, u := range a.Units {
			units = append(units, u)
		}
	}
	return units
}

func (model *ModelData) FindUnit(unitId uint32) *Unit {
	for _, u := range model.ListUnits() {
		if u.Id == unitId {
			return u
		}
	}
	return nil
}

func (model *ModelData) FindCrowd(crowdId uint32) *Crowd {
	for _, u := range model.ListCrowds() {
		if u.Id == crowdId {
			return u
		}
	}
	return nil
}

func (model *ModelData) addUnit(unit *Unit) bool {
	f := model.FindAutomat(unit.AutomatId)
	if f != nil {
		f.Units[unit.Id] = unit
		return true
	}
	return false
}

func (model *ModelData) removeUnit(unitId uint32) bool {
	u := model.FindUnit(unitId)
	if u == nil {
		return false
	}
	a := model.FindAutomat(u.AutomatId)
	if a == nil {
		return false
	}
	if _, ok := a.Units[unitId]; ok {
		delete(a.Units, unitId)
		return true
	}
	return false
}

func (model *ModelData) ListCrowds() []*Crowd {
	crowds := []*Crowd{}
	for _, party := range model.Parties {
		for _, crowd := range party.Crowds {
			crowds = append(crowds, crowd)
		}
	}
	return crowds
}

func (model *ModelData) addCrowd(crowd *Crowd) bool {
	party, ok := model.Parties[crowd.PartyId]
	if ok {
		party.Crowds[crowd.Id] = crowd
		return true
	}
	return false
}

func (model *ModelData) ListPopulations() []*Population {
	pops := []*Population{}
	for _, party := range model.Parties {
		for _, pop := range party.Populations {
			pops = append(pops, pop)
		}
	}
	return pops
}

func (model *ModelData) addPopulation(population *Population) bool {
	party, ok := model.Parties[population.PartyId]
	if ok {
		party.Populations[population.Id] = population
		return true
	}
	return false
}

func (model *ModelData) addProfile(profile *Profile) {
	model.Profiles[profile.Login] = profile
}

func (model *ModelData) updateProfile(login string, profile *Profile) bool {
	_, ok := model.Profiles[login]
	if ok {
		model.Profiles[login] = profile
	}
	return ok
}

func (model *ModelData) removeProfile(login string) bool {
	_, ok := model.Profiles[login]
	if ok {
		delete(model.Profiles, login)
	}
	return ok
}

func (model *ModelData) ListKnowledgeGroups() []*KnowledgeGroup {
	groups := []*KnowledgeGroup{}
	for _, party := range model.Parties {
		for _, group := range party.KnowledgeGroups {
			groups = append(groups, group)
		}
	}
	return groups
}

func (model *ModelData) addKnowledgeGroup(group *KnowledgeGroup) bool {
	party, ok := model.Parties[group.PartyId]
	if ok {
		party.KnowledgeGroups[group.Id] = group
		return true
	}
	return false
}
