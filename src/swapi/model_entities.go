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
	"fmt"
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

type Block struct {
	Residents map[string]int32
	Alerted   bool
	Confined  bool
}

func NewBlock(residents map[string]int32) *Block {
	return &Block{
		Residents: residents,
	}
}

type Population struct {
	Id         uint32
	PartyId    uint32
	Name       string
	Healthy    int32
	Wounded    int32
	Dead       int32
	LivingArea map[uint32]*Block
	Adhesions  map[uint32]float32
}

func NewPopulation(id, partyId uint32, name string) *Population {
	return &Population{
		Id:         id,
		PartyId:    partyId,
		Name:       name,
		LivingArea: map[uint32]*Block{},
		Adhesions:  map[uint32]float32{},
	}
}

type CrowdElement struct {
	Id       uint32
	Attitude int32
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
	Extensions           map[string]string
}

func NewCrowd(id, partyId uint32, name string) *Crowd {
	return &Crowd{
		Id:            id,
		PartyId:       partyId,
		Name:          name,
		Adhesions:     map[uint32]float32{},
		CrowdElements: map[uint32]*CrowdElement{},
		Extensions:    map[string]string{},
	}
}

type Object struct {
	Id         uint32
	ObjectType string
	Name       string
	PartyId    uint32
	Activated  bool
}

func NewObject(id, partyId uint32, objectType, name string) *Object {
	return &Object{
		Id:         id,
		ObjectType: objectType,
		Name:       name,
		PartyId:    partyId,
		Activated:  false,
	}
}

type EquipmentDotation struct {
	Available     int32
	Unavailable   int32
	Repairable    int32
	OnSiteFixable int32
	Repairing     int32
	Captured      int32
	Breakdowns    []int32
	Away          int32
}

type LentEquipment struct {
	Borrower uint32
	TypeId   uint32
	Quantity int32
}

type BorrowedEquipment struct {
	Owner    uint32
	TypeId   uint32
	Quantity int32
}

type HumanDotation struct {
	Quantity     int32
	Rank         int32
	State        int32
	Injury       int32
	Psyop        bool
	Contaminated bool
}

type ResourceDotation struct {
	Type      uint32
	Quantity  int32
	Threshold float32
}

type Posture struct {
	Old        sword.UnitAttributes_Posture
	New        sword.UnitAttributes_Posture
	Transition int32
}

type VisionCone struct {
	Origin   Point
	Height   int32
	Sensor   string
	Headings []int32
}

type VisionCones struct {
	Cones      []*VisionCone
	Elongation float32
}

type HumanFactors struct {
	Tiredness  sword.UnitAttributes_EnumUnitTiredness
	Morale     sword.UnitAttributes_EnumUnitMorale
	Experience sword.UnitAttributes_EnumUnitExperience
	Stress     sword.UnitAttributes_EnumUnitStress
}

type Unit struct {
	Id                  uint32
	AutomatId           uint32
	Name                string
	Pc                  bool
	Position            Point
	PathPoints          uint32
	DebugBrain          bool
	EquipmentDotations  map[uint32]*EquipmentDotation
	LentEquipments      []*LentEquipment
	BorrowedEquipments  []*BorrowedEquipment
	PartySurrenderedTo  uint32
	HumanDotations      []*HumanDotation
	ResourceDotations   []*ResourceDotation
	Posture             Posture
	VisionCones         VisionCones
	TransporterId       uint32
	TransportedIds      []uint32
	Adhesions           map[uint32]float32
	HumanFactors        HumanFactors
	RawOperationalState int32
	Neutralized         bool
}

type Automat struct {
	Id               uint32
	PartyId          uint32
	FormationId      uint32
	Name             string
	Automats         map[uint32]*Automat
	Units            map[uint32]*Unit
	Engaged          bool
	DebugBrain       bool
	KnowledgeGroupId uint32
	LogSuperiors     []uint32
	SuperiorQuotas   map[uint32]int32
}

func NewAutomat(id, partyId, formationId, knowledgeGroupId uint32, name string) *Automat {
	return &Automat{
		Id:               id,
		PartyId:          partyId,
		FormationId:      formationId,
		Name:             name,
		Automats:         map[uint32]*Automat{},
		Units:            map[uint32]*Unit{},
		Engaged:          true,
		DebugBrain:       false,
		KnowledgeGroupId: knowledgeGroupId,
	}
}

type Formation struct {
	Id             uint32
	PartyId        uint32
	ParentId       uint32
	Name           string
	Level          string
	LogLevel       string
	Formations     map[uint32]*Formation
	Automats       map[uint32]*Automat
	LogSuperiors   []uint32
	SuperiorQuotas map[uint32]int32
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
	Id                  uint32
	Name                string
	PartyId             uint32
	ParentId            uint32
	Type                string
	IsCrowdDefaultGroup bool
	Enabled             bool
	UnitKnowledges      map[uint32]*UnitKnowledge
	ObjectKnowledges    map[uint32]*ObjectKnowledge
	CrowdKnowledges     map[uint32]*CrowdKnowledge
}

type UnitKnowledge struct {
	Id               uint32
	KnowledgeGroupId uint32
	UnitId           uint32
	UnitType         uint32
}

type ObjectKnowledge struct {
	Id               uint32
	PartyId          uint32
	ObjectId         uint32
	ObjectType       string
	KnowledgeGroupId uint32
	ObjectPartyId    uint32
	ObjectName       string
	// todo attributes
}

type CrowdKnowledge struct {
	Id               uint32
	KnowledgeGroupId uint32
	CrowdId          uint32
	PartyId          uint32
}

type Party struct {
	Id              uint32
	Name            string
	Formations      map[uint32]*Formation
	Crowds          map[uint32]*Crowd
	Populations     map[uint32]*Population
	KnowledgeGroups map[uint32]*KnowledgeGroup
	Diplomacies     map[uint32]sword.EnumDiplomacy
	Objects         map[uint32]*Object
}

func NewParty(id uint32, name string) *Party {
	return &Party{
		Id:              id,
		Name:            name,
		Formations:      map[uint32]*Formation{},
		Crowds:          map[uint32]*Crowd{},
		Populations:     map[uint32]*Population{},
		KnowledgeGroups: map[uint32]*KnowledgeGroup{},
		Diplomacies:     map[uint32]sword.EnumDiplomacy{},
		Objects:         map[uint32]*Object{},
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

type ResourceNetwork struct {
	Name        string
	Consumption uint32
	Critical    bool
	Activated   bool
	Production  uint32
	StockMax    uint32
}

type Urban struct {
	Id               uint32
	Name             string
	ResourceNetworks map[string]*ResourceNetwork
}

func NewResourceNetworks(attributes *sword.UrbanAttributes) map[string]*ResourceNetwork {
	resources := make(map[string]*ResourceNetwork)
	if attributes != nil {
		infrastructructures := attributes.GetInfrastructures()
		if infrastructructures != nil {
			resourcesNetworks := infrastructructures.GetResourceNetwork()
			for _, r := range resourcesNetworks {
				resources[r.GetResource().GetName()] =
					&ResourceNetwork{
						Name:        r.GetResource().GetName(),
						Consumption: r.GetConsumption(),
						Critical:    r.GetCritical(),
						Activated:   r.GetEnabled(),
						Production:  r.GetProduction(),
						StockMax:    r.GetMaxStock(),
					}
			}
		}
	}
	return resources
}

func NewUrban(id uint32, name string,
	resourceNetworks map[string]*ResourceNetwork) *Urban {
	return &Urban{
		Id:               id,
		Name:             name,
		ResourceNetworks: resourceNetworks,
	}
}

type OrderKind int

const (
	UnitOrder OrderKind = iota
	AutomatOrder
	CrowdOrder
	FragOrder
)

type Order struct {
	Id          uint32
	Kind        OrderKind
	MissionType uint32
	TaskerId    uint32
}

type ModelData struct {
	Parties       map[uint32]*Party
	Profiles      map[string]*Profile
	GlobalWeather Weather
	LocalWeathers map[uint32]*LocalWeather
	Urbans        map[uint32]*Urban
	Orders        map[uint32]*Order
	// Available scores definitions
	KnownScores map[string]struct{}
	Scores      map[string]float32
	// Tick and time of the most recent started tick
	Tick         int32
	Time         time.Time
	TickDuration int32
}

func NewModelData() *ModelData {
	return &ModelData{
		Parties:       map[uint32]*Party{},
		Profiles:      map[string]*Profile{},
		LocalWeathers: map[uint32]*LocalWeather{},
		Urbans:        map[uint32]*Urban{},
		Orders:        map[uint32]*Order{},
		KnownScores:   map[string]struct{}{},
		Scores:        map[string]float32{},
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

func (model *ModelData) removeFormation(formationId uint32) bool {
	f := model.FindFormation(formationId)
	if f == nil {
		return false
	}
	if f.ParentId != 0 {
		supf := model.FindFormation(f.ParentId)
		if supf != nil {
			size := len(supf.Formations)
			delete(supf.Formations, formationId)
			if size != len(supf.Formations) {
				return true
			}
		}
	} else {
		p := model.Parties[f.PartyId]
		if p != nil {
			size := len(p.Formations)
			delete(p.Formations, formationId)
			if size != len(p.Formations) {
				return true
			}
		}
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

func (model *ModelData) removeAutomat(automatId uint32) bool {
	a := model.FindAutomat(automatId)
	if a == nil {
		return false
	}
	f := model.FindFormation(a.FormationId)
	if f == nil {
		return false
	}
	size := len(f.Automats)
	delete(f.Automats, automatId)
	return size != len(f.Automats)
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
			crowd.CrowdElements[elementId] = &CrowdElement{elementId, 0}
			return true
		}
	}
	return false
}

func (model *ModelData) removeCrowdElement(crowdId, elementId uint32) bool {
	crowd := model.FindCrowd(crowdId)
	if crowd == nil {
		return false
	}
	size := len(crowd.CrowdElements)
	delete(crowd.CrowdElements, elementId)
	return size != len(crowd.CrowdElements)
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
	size := len(a.Units)
	delete(a.Units, unitId)
	return size != len(a.Units)
}

func (model *ModelData) changeUnitSuperior(unit *Unit, newSuperior *Automat) error {
	oldAutomat := model.FindAutomat(unit.AutomatId)
	if oldAutomat == nil {
		return fmt.Errorf("invalid automat identifier: %v", unit.AutomatId)
	}
	if !model.removeUnit(unit.Id) {
		return fmt.Errorf("impossible to remove the automat: %v", unit.Id)
	}
	unit.AutomatId = newSuperior.Id
	if !model.addUnit(unit) {
		return fmt.Errorf("impossible to add the automat: %v", unit.Id)
	}
	return nil
}

func (model *ModelData) changeAutomatSuperior(automat *Automat, newSuperior *Formation) error {
	if !model.removeAutomat(automat.Id) {
		return fmt.Errorf("impossible to remove the unit: %v", automat.Id)
	}
	automat.FormationId = newSuperior.Id
	if !model.addAutomat(0, automat.FormationId, automat) {
		return fmt.Errorf("impossible to add the automat: %v", automat.Id)
	}
	return nil
}

func (model *ModelData) changeFormationSuperior(formation, parent *Formation, party *Party) error {
	if !model.removeFormation(formation.Id) {
		return fmt.Errorf("impossible to remove the formation: %v", formation.Id)
	}
	if parent != nil {
		formation.ParentId = parent.Id
	} else {
		formation.ParentId = 0
	}
	if !model.addFormation(formation) {
		return fmt.Errorf("impossible to add the formation: %v", formation.Id)
	}
	return nil
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

func (model *ModelData) FindPopulation(populationId uint32) *Population {
	for _, u := range model.ListPopulations() {
		if u.Id == populationId {
			return u
		}
	}
	return nil
}

func (model *ModelData) addProfile(profile *Profile) bool {
	if _, ok := model.Profiles[profile.Login]; !ok {
		model.Profiles[profile.Login] = profile
		return true
	}
	return false
}

func (model *ModelData) updateProfile(login string, profile *Profile) bool {
	_, ok := model.Profiles[login]
	if ok {
		model.Profiles[login] = profile
	}
	return ok
}

func (model *ModelData) removeProfile(login string) bool {
	size := len(model.Profiles)
	delete(model.Profiles, login)
	return size != len(model.Profiles)
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

func (model *ModelData) FindKnowledgeGroup(knowledgeGroupId uint32) *KnowledgeGroup {
	for _, u := range model.ListKnowledgeGroups() {
		if u.Id == knowledgeGroupId {
			return u
		}
	}
	return nil
}

func (model *ModelData) FindUnitKnowledge(knowledgeGroupId uint32, unitId uint32) *UnitKnowledge {
	knowledgeGroup := model.FindKnowledgeGroup(knowledgeGroupId)
	if knowledgeGroup == nil {
		return nil
	}
	for _, u := range knowledgeGroup.UnitKnowledges {
		if u.UnitId == unitId {
			return u
		}
	}
	return nil
}

func (model *ModelData) addUnitKnowledge(knowledge *UnitKnowledge) bool {
	knowledgeGroup := model.FindKnowledgeGroup(knowledge.KnowledgeGroupId)
	if knowledgeGroup != nil {
		knowledgeGroup.UnitKnowledges[knowledge.Id] = knowledge
		return true
	}
	return false
}

func (model *ModelData) addObjectKnowledge(knowledge *ObjectKnowledge) bool {
	knowledgeGroup := model.FindKnowledgeGroup(knowledge.KnowledgeGroupId)
	if knowledgeGroup != nil {
		knowledgeGroup.ObjectKnowledges[knowledge.Id] = knowledge
		return true
	}
	return false
}

func (model *ModelData) addCrowdKnowledge(knowledge *CrowdKnowledge) bool {
	knowledgeGroup := model.FindKnowledgeGroup(knowledge.KnowledgeGroupId)
	if knowledgeGroup != nil {
		knowledgeGroup.CrowdKnowledges[knowledge.Id] = knowledge
		return true
	}
	return false
}

func (model *ModelData) addLocalWeather(weather *LocalWeather) {
	model.LocalWeathers[weather.Id] = weather
}

func (model *ModelData) removeLocalWeather(id uint32) bool {
	size := len(model.LocalWeathers)
	delete(model.LocalWeathers, id)
	return size != len(model.LocalWeathers)
}

func (model *ModelData) addUrban(urban *Urban) bool {
	if _, ok := model.Urbans[urban.Id]; !ok {
		model.Urbans[urban.Id] = urban
		return true
	}
	return false
}

func (model *ModelData) updateUrban(id uint32, resourceNetworks map[string]*ResourceNetwork) bool {
	if urban, ok := model.Urbans[id]; ok {
		urban.ResourceNetworks = resourceNetworks
		return true
	}
	return false
}

func (model *ModelData) addOrder(order *Order) bool {
	if _, ok := model.Orders[order.Id]; !ok {
		model.Orders[order.Id] = order
		return true
	}
	return false
}

func (model *ModelData) changeLogisticsLinks(entityId uint32, superiors []uint32) bool {
	automat := model.FindAutomat(entityId)
	if automat != nil {
		automat.LogSuperiors = make([]uint32, len(superiors))
		copy(automat.LogSuperiors, superiors)
		return true
	} else {
		formation := model.FindFormation(entityId)
		if formation != nil {
			formation.LogSuperiors = make([]uint32, len(superiors))
			copy(formation.LogSuperiors, superiors)
			return true
		}
	}
	return false
}

func (model *ModelData) changeSupplyQuotas(suppliedId uint32, quotas map[uint32]int32) bool {
	automat := model.FindAutomat(suppliedId)
	if automat != nil {
		automat.SuperiorQuotas = quotas
		return true
	} else {
		formation := model.FindFormation(suppliedId)
		if formation != nil {
			formation.SuperiorQuotas = quotas
			return true
		}
	}
	return false
}

func (model *ModelData) addObject(object *Object) bool {
	party, ok := model.Parties[object.PartyId]
	if ok {
		party.Objects[object.Id] = object
		return true
	}
	return false
}

func (model *ModelData) ListObjects() []*Object {
	objects := []*Object{}
	for _, party := range model.Parties {
		for _, object := range party.Objects {
			objects = append(objects, object)
		}
	}
	return objects
}

func (model *ModelData) FindObject(objectId uint32) *Object {
	for _, o := range model.ListObjects() {
		if o.Id == objectId {
			return o
		}
	}
	return nil
}

func (model *ModelData) removeObject(objectId uint32) bool {
	o := model.FindObject(objectId)
	if o == nil {
		return false
	}
	p := model.Parties[o.PartyId]
	if p != nil {
		size := len(p.Objects)
		delete(p.Objects, objectId)
		if size != len(p.Objects) {
			return true
		}
	}
	return false
}
