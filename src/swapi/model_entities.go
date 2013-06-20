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
	"bytes"
	"encoding/gob"
	"sword"
	"time"
)

func DeepCopy(dst, src interface{}) interface{} {
	// not the fastest method, but it works
	buffer := new(bytes.Buffer)
	err := gob.NewEncoder(buffer).Encode(src)
	if err != nil {
		panic(err)
	}
	err = gob.NewDecoder(buffer).Decode(dst)
	if err != nil {
		panic(err)
	}
	return dst
}

type Point struct {
	X float64
	Y float64
}

type Profile struct {
	Login      string
	Password   string
	Supervisor bool
}

type Population struct {
	Id      uint32
	PartyId uint32
	Name    string
}

type CrowdElement struct {
	Id       uint32
	attitude uint32
}

func NewCrowdElement(id uint32) *CrowdElement {
	return &CrowdElement{
		Id: id,
	}
}

type Crowd struct {
	Id                   uint32
	PartyId              uint32
	Name                 string
	Healthy              int32
	Wounded              int32
	Dead                 int32
	Contaminated         int32
	ArmedIndividuals     float32
	CriticalIntelligence string
	Adhesions            map[uint32]float32
	CrowdElements        map[uint32]*CrowdElement
}

func NewCrowd(id, partyId uint32, name string) *Crowd {
	return &Crowd{
		Id:            id,
		PartyId:       partyId,
		Name:          name,
		Adhesions:     map[uint32]float32{},
		CrowdElements: map[uint32]*CrowdElement{},
	}
}

type Unit struct {
	Id         uint32
	AutomatId  uint32
	Name       string
	Pc         bool
	Position   Point
	PathPoints uint32
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

type KnowledgeGroup struct {
	Id       uint32
	Name     string
	PartyId  uint32
	ParentId uint32
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

type Weather struct {
	Temperature   float32
	WindSpeed     float32
	WindDirection int32
	CloudFloor    float32
	CloudCeil     float32
	CloudDensity  float32
	Precipitation sword.WeatherAttributes_EnumPrecipitationType
	Lightning     sword.WeatherAttributes_EnumLightingType
}

type LocalWeather struct {
	Weather
	StartTime   time.Time
	EndTime     time.Time
	TopLeft     Point
	BottomRight Point
	Id          uint32
}

type ModelData struct {
	Parties       map[uint32]*Party
	Profiles      map[string]*Profile
	GlobalWeather Weather
	LocalWeathers map[uint32]*LocalWeather
	// Tick and time of the most recent started tick
	Tick int32
	Time time.Time
}

func NewModelData() *ModelData {
	return &ModelData{
		Parties:       map[uint32]*Party{},
		Profiles:      map[string]*Profile{},
		LocalWeathers: map[uint32]*LocalWeather{},
	}
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

func (model *ModelData) addCrowdElement(crowdId, elementId uint32) bool {
	if crowd := model.FindCrowd(crowdId); crowd != nil {
		if element := crowd.CrowdElements[elementId]; element == nil {
			crowd.CrowdElements[ elementId ] = &CrowdElement{ elementId, 0 }
			return true
		}
	}
	return false
}

func (model *ModelData) removeCrowdElement(crowdId, elementId uint32) bool {
	if crowd := model.FindCrowd(crowdId); crowd != nil {
		if element := crowd.CrowdElements[elementId]; element != nil {
			delete(crowd.CrowdElements, elementId)
			return true
		}
	}
	return false
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

func (model *ModelData) addLocalWeather(weather *LocalWeather) {
	model.LocalWeathers[weather.Id] = weather
}

func (model *ModelData) removeLocalWeather(id uint32) bool {
	_, ok := model.LocalWeathers[id]
	if ok {
		delete(model.LocalWeathers, id)
		return true
	}
	return false
}
