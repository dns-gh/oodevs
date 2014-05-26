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

type PathPoint struct {
	Point    Point
	Waypoint int32
	Reached  bool
}

func NewPathPoint(point Point) PathPoint {
	return PathPoint{
		Point:    point,
		Waypoint: -1,
		Reached:  false,
	}
}

type Segment struct {
	From Point
	To   Point
}

type TacticalLine struct {
	Id        uint32
	Name      string
	Location  sword.Location
	Diffusion sword.Diffusion
}

func NewTacticalLine(id uint32, line *sword.TacticalLine) *TacticalLine {
	return &TacticalLine{
		Id:        id,
		Name:      line.GetName(),
		Location:  *line.GetGeometry(),
		Diffusion: *line.GetDiffusion(),
	}
}

type Profile struct {
	Login               string
	Password            string
	Supervisor          bool
	TimeControl         bool
	ReadOnlyFormations  map[uint32]struct{}
	ReadWriteFormations map[uint32]struct{}
	ReadOnlyAutomats    map[uint32]struct{}
	ReadWriteAutomats   map[uint32]struct{}
	ReadOnlyParties     map[uint32]struct{}
	ReadWriteParties    map[uint32]struct{}
	ReadOnlyCrowds      map[uint32]struct{}
	ReadWriteCrowds     map[uint32]struct{}
}

func mapIds(src *sword.IdList) map[uint32]struct{} {
	dst := map[uint32]struct{}{}
	for _, obj := range src.GetElem() {
		if obj.Id != nil {
			dst[*obj.Id] = struct{}{}
		}
	}
	return dst
}

func NewProfile(profile *sword.Profile) *Profile {
	return &Profile{
		Login:               profile.GetLogin(),
		Password:            profile.GetPassword(),
		Supervisor:          profile.GetSupervisor(),
		TimeControl:         profile.GetTimeControl(),
		ReadOnlyFormations:  mapIds(profile.GetReadOnlyFormations()),
		ReadWriteFormations: mapIds(profile.GetReadWriteFormations()),
		ReadOnlyAutomats:    mapIds(profile.GetReadOnlyAutomates()),
		ReadWriteAutomats:   mapIds(profile.GetReadWriteAutomates()),
		ReadOnlyParties:     mapIds(profile.GetReadOnlyParties()),
		ReadWriteParties:    mapIds(profile.GetReadWriteParties()),
		ReadOnlyCrowds:      mapIds(profile.GetReadOnlyCrowds()),
		ReadWriteCrowds:     mapIds(profile.GetReadWriteCrowds()),
	}
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
	Position Point
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

type ResourceNetwork struct {
	Name        string
	Consumption uint32
	Critical    bool
	Activated   bool
	Production  uint32
	StockMax    uint32
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

type Urban struct {
	ResourceNetworks map[string]*ResourceNetwork
}

type Object struct {
	Id             uint32
	ObjectType     string
	Name           string
	PartyId        uint32
	Activated      bool
	Bypass         int32
	Altitude       int32
	Construction   int32
	Trafficability float32
	Urban          *Urban
}

func NewObject(id, partyId uint32, objectType, name string) *Object {
	return &Object{
		Id:         id,
		ObjectType: objectType,
		Name:       name,
		PartyId:    partyId,
	}
}

func NewUrban(id uint32, name string,
	resourceNetworks map[string]*ResourceNetwork) *Object {
	return &Object{
		Id:         id,
		ObjectType: "urban",
		Name:       name,
		Urban: &Urban{
			ResourceNetworks: resourceNetworks,
		},
	}
}

type Equipment struct {
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

type Human struct {
	Quantity     int32
	Rank         int32
	State        int32
	Injury       int32
	Psyop        bool
	Contaminated bool
}

type Resource struct {
	Quantity      int32
	LowThreshold  float32
	HighThreshold float32
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

type Stock struct {
	Type     uint32
	Quantity int32
}

type Unit struct {
	Id                  uint32
	Type                uint32
	AutomatId           uint32
	Name                string
	Pc                  bool
	Position            Point
	Speed               int32
	PathPoints          uint32
	DebugBrain          bool
	Equipments          map[uint32]*Equipment
	LentEquipments      []*LentEquipment
	BorrowedEquipments  []*BorrowedEquipment
	PartySurrenderedTo  uint32
	Humans              []*Human
	Resources           map[uint32]Resource
	Posture             Posture
	VisionCones         VisionCones
	TransporterId       uint32
	TransportedIds      []uint32
	Adhesions           map[uint32]float32
	HumanFactors        HumanFactors
	RawOperationalState int32
	Neutralized         bool
	Stocks              []*Stock
	Installation        int32
}

type Automat struct {
	Id                   uint32
	PartyId              uint32
	FormationId          uint32
	Name                 string
	Engaged              bool
	DebugBrain           bool
	KnowledgeGroupId     uint32
	LogSuperiors         []uint32
	SuperiorQuotas       map[uint32]int32
	LogMaintenanceManual bool
	LogSupplyManual      bool
}

type Formation struct {
	Id                   uint32
	PartyId              uint32
	ParentId             uint32
	Name                 string
	Level                string
	LogLevel             string
	LogSuperiors         []uint32
	SuperiorQuotas       map[uint32]int32
	LogMaintenanceManual bool
	LogSupplyManual      bool
}

type KnowledgeGroup struct {
	Id                  uint32
	Name                string
	PartyId             uint32
	ParentId            uint32
	Type                string
	IsCrowdDefaultGroup bool
	Enabled             bool
}

type UnitKnowledge struct {
	Id                  uint32
	KnowledgeGroupId    uint32
	UnitId              uint32
	UnitType            uint32
	IdentificationLevel int32
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
	Id          uint32
	Name        string
	Diplomacies map[uint32]sword.EnumDiplomacy
}

func NewParty(id uint32, name string) *Party {
	return &Party{
		Id:          id,
		Name:        name,
		Diplomacies: map[uint32]sword.EnumDiplomacy{},
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

type Pathfind struct {
	Id     uint32
	UnitId uint32
	Points []PathPoint
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
	ParentId    uint32
	Kind        OrderKind
	MissionType uint32
	TaskerId    uint32
}

type ActionKind int

const (
	MagicAction ActionKind = iota
	UnitMagicAction
	ObjectMagicAction
	KnowledgeMagicAction
	SetAutomatMode
)

type Action struct {
	Id      uint32
	Kind    ActionKind
	ErrCode int32
	ErrMsg  string
}

type MaintenanceHandlingProvider struct {
	Id      uint32
	State   sword.LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus
	EndTick int32
}

type MaintenanceHandling struct {
	Id       uint32
	Provider *MaintenanceHandlingProvider
}

type MedicalHandlingProvider struct {
	Id    uint32
	State sword.LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus
}

type MedicalHandling struct {
	Id       uint32
	Provider *MedicalHandlingProvider
}

type FuneralHandlingHandler struct {
	Id    uint32
	State sword.LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus
}

type FuneralHandling struct {
	Id      uint32
	UnitId  uint32
	Handler *FuneralHandlingHandler
}

type SupplyHandlingConvoy struct {
	ConvoyerId uint32
	State      sword.LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus
}

type SupplyHandling struct {
	Id         uint32
	SupplierId uint32
	ProviderId uint32
	Convoy     *SupplyHandlingConvoy
}

type Location struct {
	Type   sword.Location_Geometry
	Points []Point
}

type FireEffect struct {
	Id       uint32
	Type     sword.StartFireEffect_EnumFireEffectType
	Location Location
}

type FireDetection struct {
	Id    uint32
	Units []uint32
	Firer uint32
}

type ReplayInfo struct {
	FirstTick int32
	TickCount int32
}

type SupplyRequest struct {
	ResourceId  uint32
	RequesterId uint32
	SupplierId  uint32
	RecipientId uint32
	Requested   int32
}

type ModelData struct {
	Actions              map[uint32]*Action
	Parties              map[uint32]*Party
	Formations           map[uint32]*Formation
	Automats             map[uint32]*Automat
	Units                map[uint32]*Unit
	Crowds               map[uint32]*Crowd
	Populations          map[uint32]*Population
	KnowledgeGroups      map[uint32]*KnowledgeGroup
	UnitKnowledges       map[uint32]*UnitKnowledge
	ObjectKnowledges     map[uint32]*ObjectKnowledge
	CrowdKnowledges      map[uint32]*CrowdKnowledge
	Profiles             map[string]*Profile
	Orders               map[uint32]*Order
	Objects              map[uint32]*Object
	MaintenanceHandlings map[uint32]*MaintenanceHandling
	MedicalHandlings     map[uint32]*MedicalHandling
	FuneralHandlings     map[uint32]*FuneralHandling
	SupplyHandlings      map[uint32]*SupplyHandling
	FireEffects          map[uint32]*FireEffect
	FireDetections       map[uint32]*FireDetection
	LocalWeathers        map[uint32]*LocalWeather
	Pathfinds            map[uint32]*Pathfind
	SupplyRequests       map[uint32]*SupplyRequest
	TacticalLines        map[uint32]*TacticalLine
	GlobalWeather        Weather
	// Available scores definitions
	KnownScores map[string]struct{}
	Scores      map[string]float32
	// Tick and time of the most recent started tick
	Tick int32
	Time time.Time
	// Latest ending tick
	LastTick     int32
	TickDuration int32
	Replay       *ReplayInfo
	// True once the initial state has been received
	Ready bool
}

func NewModelData() *ModelData {
	return &ModelData{
		Actions:              map[uint32]*Action{},
		Parties:              map[uint32]*Party{},
		Formations:           map[uint32]*Formation{},
		Automats:             map[uint32]*Automat{},
		Units:                map[uint32]*Unit{},
		Crowds:               map[uint32]*Crowd{},
		Populations:          map[uint32]*Population{},
		KnowledgeGroups:      map[uint32]*KnowledgeGroup{},
		UnitKnowledges:       map[uint32]*UnitKnowledge{},
		ObjectKnowledges:     map[uint32]*ObjectKnowledge{},
		CrowdKnowledges:      map[uint32]*CrowdKnowledge{},
		Profiles:             map[string]*Profile{},
		LocalWeathers:        map[uint32]*LocalWeather{},
		Pathfinds:            map[uint32]*Pathfind{},
		Orders:               map[uint32]*Order{},
		Objects:              map[uint32]*Object{},
		MaintenanceHandlings: map[uint32]*MaintenanceHandling{},
		MedicalHandlings:     map[uint32]*MedicalHandling{},
		FuneralHandlings:     map[uint32]*FuneralHandling{},
		SupplyHandlings:      map[uint32]*SupplyHandling{},
		FireEffects:          map[uint32]*FireEffect{},
		FireDetections:       map[uint32]*FireDetection{},
		KnownScores:          map[string]struct{}{},
		Scores:               map[string]float32{},
		SupplyRequests:       map[uint32]*SupplyRequest{},
		TacticalLines:        map[uint32]*TacticalLine{},
	}
}

func (model *ModelData) Reset() {
	newModel := NewModelData()
	// Following fields are not sent as part of simulation state
	newModel.Profiles = model.Profiles
	newModel.KnownScores = model.KnownScores
	newModel.Scores = model.Scores
	newModel.Tick = model.Tick
	newModel.Time = model.Time
	newModel.TickDuration = model.TickDuration
	*model = *newModel
}

func (model *ModelData) FindPartyByName(name string) *Party {
	for _, party := range model.Parties {
		if party.Name == name {
			return party
		}
	}
	return nil
}

func (model *ModelData) addFormation(f *Formation) bool {
	if _, ok := model.Formations[f.ParentId]; !ok {
		if _, ok = model.Parties[f.PartyId]; !ok {
			return false
		}
	}
	size := len(model.Formations)
	model.Formations[f.Id] = f
	return size != len(model.Formations)
}

func (model *ModelData) removeFormation(formationId uint32) bool {
	for _, f := range model.Formations {
		if f.ParentId == formationId {
			return false
		}
	}
	for _, a := range model.Automats {
		if a.FormationId == formationId {
			return false
		}
	}
	size := len(model.Formations)
	delete(model.Formations, formationId)
	return size != len(model.Formations)
}

func (model *ModelData) addAutomat(a *Automat) bool {
	if _, ok := model.Formations[a.FormationId]; !ok {
		return false
	}
	size := len(model.Automats)
	model.Automats[a.Id] = a
	return size != len(model.Automats)
}

func (model *ModelData) removeAutomat(automatId uint32) bool {
	for _, u := range model.Units {
		if u.AutomatId == automatId {
			return false
		}
	}
	size := len(model.Automats)
	delete(model.Automats, automatId)
	return size != len(model.Automats)
}

func (model *ModelData) addCrowdElement(crowdId, elementId uint32, position Point) bool {
	crowd, ok := model.Crowds[crowdId]
	if !ok {
		return false
	}
	size := len(crowd.CrowdElements)
	crowd.CrowdElements[elementId] = &CrowdElement{elementId, 0, position}
	return size != len(crowd.CrowdElements)
}

func (model *ModelData) removeCrowdElement(crowdId, elementId uint32) bool {
	crowd, ok := model.Crowds[crowdId]
	if !ok {
		return false
	}
	size := len(crowd.CrowdElements)
	delete(crowd.CrowdElements, elementId)
	return size != len(crowd.CrowdElements)
}

func (model *ModelData) addUnit(unit *Unit) bool {
	if _, ok := model.Automats[unit.AutomatId]; !ok {
		return false
	}
	size := len(model.Units)
	model.Units[unit.Id] = unit
	return size != len(model.Units)
}

func (model *ModelData) removeUnit(unitId uint32) bool {
	size := len(model.Units)
	delete(model.Units, unitId)
	return size != len(model.Units)
}

func (model *ModelData) changeUnitAutomat(unitId, automatId uint32) error {
	unit, ok := model.Units[unitId]
	if !ok {
		return fmt.Errorf("invalid unit identifier: %v", unitId)
	}
	if _, ok = model.Automats[automatId]; !ok {
		return fmt.Errorf("invalid automat identifier: %v", automatId)
	}
	unit.AutomatId = automatId
	return nil
}

func (model *ModelData) changeAutomatFormation(automatId, formationId uint32) error {
	automat, ok := model.Automats[automatId]
	if !ok {
		return fmt.Errorf("invalid automat identifier: %v", automatId)
	}
	if _, ok = model.Formations[formationId]; !ok {
		return fmt.Errorf("invalid formation identifier: %v", formationId)
	}
	automat.FormationId = formationId
	return nil
}

func (model *ModelData) changeFormationSuperior(formationId, partyId, parentId uint32) error {
	formation, ok := model.Formations[formationId]
	if !ok {
		return fmt.Errorf("invalid formation identifier: %v", formationId)
	}
	if partyId != 0 && parentId != 0 {
		return fmt.Errorf("unable to set both parent & party")
	}
	if partyId != 0 {
		if _, ok = model.Parties[partyId]; !ok {
			return fmt.Errorf("invalid party identifier: %v", partyId)
		}
	}
	if parentId != 0 {
		if _, ok = model.Formations[parentId]; !ok {
			return fmt.Errorf("invalid parent formation identifier: %v", parentId)
		}
	}
	formation.ParentId = parentId
	formation.PartyId = partyId
	return nil
}

func (model *ModelData) addCrowd(crowd *Crowd) bool {
	if _, ok := model.Parties[crowd.PartyId]; !ok {
		return false
	}
	size := len(model.Crowds)
	model.Crowds[crowd.Id] = crowd
	return size != len(model.Crowds)
}

func (model *ModelData) removeCrowd(id uint32) bool {
	size := len(model.Crowds)
	delete(model.Crowds, id)
	return size != len(model.Crowds)
}

func (model *ModelData) addPopulation(population *Population) bool {
	if _, ok := model.Parties[population.PartyId]; !ok {
		return false
	}
	size := len(model.Populations)
	model.Populations[population.Id] = population
	return size != len(model.Populations)
}

func (model *ModelData) addProfile(profile *Profile) bool {
	size := len(model.Profiles)
	model.Profiles[profile.Login] = profile
	return size != len(model.Profiles)
}

func (model *ModelData) updateProfile(login string, profile *Profile) bool {
	size := len(model.Profiles)
	model.Profiles[login] = profile
	return size == len(model.Profiles)
}

func (model *ModelData) removeProfile(login string) bool {
	size := len(model.Profiles)
	delete(model.Profiles, login)
	return size != len(model.Profiles)
}

func (model *ModelData) addTacticalLine(line *TacticalLine) bool {
	size := len(model.TacticalLines)
	model.TacticalLines[line.Id] = line
	return size != len(model.TacticalLines)
}

func (model *ModelData) updateTacticalLine(line *TacticalLine) bool {
	size := len(model.TacticalLines)
	model.TacticalLines[line.Id] = line
	return size == len(model.TacticalLines)
}

func (model *ModelData) removeTacticalLine(id uint32) bool {
	size := len(model.TacticalLines)
	delete(model.TacticalLines, id)
	return size != len(model.TacticalLines)
}

func (model *ModelData) addKnowledgeGroup(group *KnowledgeGroup) bool {
	if _, ok := model.Parties[group.PartyId]; !ok {
		return false
	}
	// FIXME checking conflicts break replayer tests
	model.KnowledgeGroups[group.Id] = group
	return true
}

func (model *ModelData) removeKnowledgeGroup(id uint32) bool {
	for _, u := range model.UnitKnowledges {
		if u.KnowledgeGroupId == id {
			return false
		}
	}
	for _, o := range model.ObjectKnowledges {
		if o.KnowledgeGroupId == id {
			return false
		}
	}
	for _, c := range model.CrowdKnowledges {
		if c.KnowledgeGroupId == id {
			return false
		}
	}
	size := len(model.KnowledgeGroups)
	delete(model.KnowledgeGroups, id)
	return size != len(model.KnowledgeGroups)
}

func (model *ModelData) addUnitKnowledge(knowledge *UnitKnowledge) bool {
	if _, ok := model.KnowledgeGroups[knowledge.KnowledgeGroupId]; !ok {
		return false
	}
	size := len(model.UnitKnowledges)
	model.UnitKnowledges[knowledge.Id] = knowledge
	return size != len(model.UnitKnowledges)
}

func (model *ModelData) removeUnitKnowledge(id uint32) bool {
	size := len(model.UnitKnowledges)
	delete(model.UnitKnowledges, id)
	return size != len(model.UnitKnowledges)
}

func (model *ModelData) addObjectKnowledge(knowledge *ObjectKnowledge) bool {
	if _, ok := model.KnowledgeGroups[knowledge.KnowledgeGroupId]; !ok {
		return false
	}
	size := len(model.ObjectKnowledges)
	model.ObjectKnowledges[knowledge.Id] = knowledge
	return size != len(model.ObjectKnowledges)
}

func (model *ModelData) removeObjectKnowledge(id uint32) bool {
	size := len(model.ObjectKnowledges)
	delete(model.ObjectKnowledges, id)
	return size != len(model.ObjectKnowledges)
}

func (model *ModelData) addCrowdKnowledge(knowledge *CrowdKnowledge) bool {
	if _, ok := model.KnowledgeGroups[knowledge.KnowledgeGroupId]; !ok {
		return false
	}
	size := len(model.CrowdKnowledges)
	model.CrowdKnowledges[knowledge.Id] = knowledge
	return size != len(model.CrowdKnowledges)
}

func (model *ModelData) removeCrowdKnowledge(id uint32) bool {
	size := len(model.CrowdKnowledges)
	delete(model.CrowdKnowledges, id)
	return size != len(model.CrowdKnowledges)
}

func (model *ModelData) removeLocalWeather(id uint32) bool {
	size := len(model.LocalWeathers)
	delete(model.LocalWeathers, id)
	return size != len(model.LocalWeathers)
}

func (model *ModelData) removePathfind(id uint32) bool {
	size := len(model.Pathfinds)
	delete(model.Pathfinds, id)
	return size != len(model.Pathfinds)
}

func (model *ModelData) updateUrban(id uint32, resourceNetworks map[string]*ResourceNetwork) bool {
	if urban, ok := model.Objects[id]; ok && urban.Urban != nil {
		urban.Urban.ResourceNetworks = resourceNetworks
		return true
	}
	return false
}

func (model *ModelData) addOrder(order *Order) bool {
	size := len(model.Orders)
	model.Orders[order.Id] = order
	return size != len(model.Orders)
}

func (model *ModelData) addAction(action *Action) bool {
	size := len(model.Actions)
	model.Actions[action.Id] = action
	return size != len(model.Actions)
}

func (model *ModelData) removeAction(id uint32) bool {
	size := len(model.Actions)
	delete(model.Actions, id)
	return size != len(model.Actions)
}

func (model *ModelData) changeAutomatLogisticsLinks(entityId uint32, superiors []uint32) bool {
	if automat, ok := model.Automats[entityId]; ok {
		automat.LogSuperiors = append([]uint32{}, superiors...)
		return true
	}
	return false
}

func (model *ModelData) changeFormationLogisticsLinks(entityId uint32, superiors []uint32) bool {
	if formation, ok := model.Formations[entityId]; ok {
		formation.LogSuperiors = append([]uint32{}, superiors...)
		return true
	}
	return false
}

func (model *ModelData) changeAutomatSupplyQuotas(suppliedId uint32, quotas map[uint32]int32) bool {
	if automat, ok := model.Automats[suppliedId]; ok {
		automat.SuperiorQuotas = quotas
		return true
	}
	return false
}

func (model *ModelData) changeFormationSupplyQuotas(suppliedId uint32, quotas map[uint32]int32) bool {
	if formation, ok := model.Formations[suppliedId]; ok {
		formation.SuperiorQuotas = quotas
		return true
	}
	return false
}

func (model *ModelData) addObject(object *Object) bool {
	size := len(model.Objects)
	model.Objects[object.Id] = object
	return size != len(model.Objects)
}

func (model *ModelData) removeObject(objectId uint32) bool {
	size := len(model.Objects)
	delete(model.Objects, objectId)
	return size != len(model.Objects)
}

func (model *ModelData) addFireEffect(effect *FireEffect) bool {
	size := len(model.FireEffects)
	model.FireEffects[effect.Id] = effect
	return size != len(model.FireEffects)
}

func (model *ModelData) removeFireEffect(effectId uint32) bool {
	size := len(model.FireEffects)
	delete(model.FireEffects, effectId)
	return size != len(model.FireEffects)
}

func (model *ModelData) updateFireDetection(perception *FireDetection) {
	if p, ok := model.FireDetections[perception.Id]; ok {
		p.Units = append(p.Units, perception.Units...)
	} else {
		model.FireDetections[perception.Id] = perception
	}
}

func (model *ModelData) removeFireDetection(id uint32, units []uint32) bool {
	size := len(model.FireDetections)
	delete(model.FireDetections, id)
	return size != len(model.FireDetections)
}

func (model *ModelData) addSupplyRequest(id uint32) bool {
	size := len(model.SupplyRequests)
	model.SupplyRequests[id] = &SupplyRequest{}
	return size != len(model.SupplyRequests)
}

func (model *ModelData) removeSupplyRequest(id uint32) bool {
	size := len(model.SupplyRequests)
	delete(model.SupplyRequests, id)
	return size != len(model.SupplyRequests)
}

var (
	simToClientHandlers = []func(model *ModelData, m *sword.SimToClient_Content) error{
		(*ModelData).handleAutomatAttributes,
		(*ModelData).handleAutomatChangeKnowledgeGroup,
		(*ModelData).handleAutomatChangeLogisticLinks,
		(*ModelData).handleAutomatChangeSuperior,
		(*ModelData).handleAutomatCreation,
		(*ModelData).handleAutomatDestruction,
		(*ModelData).handleAutomatOrder,
		(*ModelData).handleChangeDiplomacy,
		(*ModelData).handleControlBeginTick,
		(*ModelData).handleControlEndTick,
		(*ModelData).handleControlGlobalWeather,
		(*ModelData).handleControlInformation,
		(*ModelData).handleControlLocalWeatherCreation,
		(*ModelData).handleControlLocalWeatherDestruction,
		(*ModelData).handleControlSendCurrentStateBegin,
		(*ModelData).handleControlSendCurrentStateEnd,
		(*ModelData).handleCrowdConcentrationCreation,
		(*ModelData).handleCrowdConcentrationDestruction,
		(*ModelData).handleCrowdConcentrationUpdate,
		(*ModelData).handleCrowdCreation,
		(*ModelData).handleCrowdDestruction,
		(*ModelData).handleCrowdFlowCreation,
		(*ModelData).handleCrowdFlowDestruction,
		(*ModelData).handleCrowdFlowUpdate,
		(*ModelData).handleCrowdKnowledgeCreation,
		(*ModelData).handleCrowdKnowledgeDestruction,
		(*ModelData).handleCrowdOrder,
		(*ModelData).handleCrowdUpdate,
		(*ModelData).handleFireEffectCreation,
		(*ModelData).handleFireEffectDestruction,
		(*ModelData).handleFireDetectionCreation,
		(*ModelData).handleFireDetectionDestruction,
		(*ModelData).handleFormationChangeSuperior,
		(*ModelData).handleFormationCreation,
		(*ModelData).handleFormationDestruction,
		(*ModelData).handleFormationUpdate,
		(*ModelData).handleFragOrder,
		(*ModelData).handleKnowledgeGroupCreation,
		(*ModelData).handleKnowledgeGroupDestruction,
		(*ModelData).handleKnowledgeGroupUpdate,
		(*ModelData).handleLogFuneralHandlingCreation,
		(*ModelData).handleLogFuneralHandlingDestruction,
		(*ModelData).handleLogFuneralHandlingUpdate,
		(*ModelData).handleLogMaintenanceHandlingCreation,
		(*ModelData).handleLogMaintenanceHandlingDestruction,
		(*ModelData).handleLogMaintenanceHandlingUpdate,
		(*ModelData).handleLogMedicalHandlingCreation,
		(*ModelData).handleLogMedicalHandlingDestruction,
		(*ModelData).handleLogMedicalHandlingUpdate,
		(*ModelData).handleLogSupplyHandlingCreation,
		(*ModelData).handleLogSupplyHandlingDestruction,
		(*ModelData).handleLogSupplyHandlingUpdate,
		(*ModelData).handleLogSupplyQuotas,
		(*ModelData).handleLogSupplyState,
		(*ModelData).handleActionCreation,
		(*ModelData).handleActionDestruction,
		(*ModelData).handleObjectCreation,
		(*ModelData).handleObjectDestruction,
		(*ModelData).handleObjectKnowledgeCreation,
		(*ModelData).handleObjectKnowledgeDestruction,
		(*ModelData).handleObjectUpdate,
		(*ModelData).handlePartyCreation,
		(*ModelData).handlePathfindCreation,
		(*ModelData).handlePopulationCreation,
		(*ModelData).handlePopulationUpdate,
		(*ModelData).handleSupplyRequestCreation,
		(*ModelData).handleSupplyRequestDestruction,
		(*ModelData).handleSupplyRequestUpdate,
		(*ModelData).handleUnitAttributes,
		(*ModelData).handleUnitChangeSuperior,
		(*ModelData).handleUnitCreation,
		(*ModelData).handleUnitDestruction,
		(*ModelData).handleUnitKnowledgeCreation,
		(*ModelData).handleUnitKnowledgeUpdate,
		(*ModelData).handleUnitKnowledgeDestruction,
		(*ModelData).handleUnitOrder,
		(*ModelData).handleUnitPathfind,
		(*ModelData).handleUnitVisionCones,
		(*ModelData).handleUrbanCreation,
		(*ModelData).handleUrbanUpdate,
	}
	authToClientHandlers = []func(model *ModelData, m *sword.AuthenticationToClient_Content) error{
		(*ModelData).handleProfileCreation,
		(*ModelData).handleProfileDestruction,
		(*ModelData).handleProfileUpdate,
	}
	aarToClientHandlers = []func(model *ModelData, m *sword.AarToClient_Content) error{
		(*ModelData).handleAarInformation,
		(*ModelData).handleIndicator,
	}
	replayToClientHandlers = []func(model *ModelData, m *sword.ReplayToClient_Content) error{
		(*ModelData).handleControlReplayInformation,
	}
	messengerToClientHandlers = []func(model *ModelData, m *sword.MessengerToClient_Content) error{
		(*ModelData).handleLimitCreation,
		(*ModelData).handleLimitUpdate,
		(*ModelData).handleLimitDestruction,
		(*ModelData).handlePhaseLineCreation,
		(*ModelData).handlePhaseLineUpdate,
		(*ModelData).handlePhaseLineDestruction,
	}
)

func (model *ModelData) update(msg *SwordMessage) error {
	if msg.SimulationToClient != nil {
		m := msg.SimulationToClient.GetMessage()
		for _, handler := range simToClientHandlers {
			err := handler(model, m)
			if err != ErrSkipHandler {
				return err
			}
		}
	} else if msg.AuthenticationToClient != nil {
		m := msg.AuthenticationToClient.GetMessage()
		for _, handler := range authToClientHandlers {
			err := handler(model, m)
			if err != ErrSkipHandler {
				return err
			}
		}
	} else if msg.AarToClient != nil {
		m := msg.AarToClient.GetMessage()
		for _, handler := range aarToClientHandlers {
			err := handler(model, m)
			if err != ErrSkipHandler {
				return err
			}
		}
	} else if msg.ReplayToClient != nil {
		m := msg.ReplayToClient.GetMessage()
		for _, handler := range replayToClientHandlers {
			err := handler(model, m)
			if err != ErrSkipHandler {
				return err
			}
		}
	} else if msg.MessengerToClient != nil {
		m := msg.MessengerToClient.GetMessage()
		for _, handler := range messengerToClientHandlers {
			err := handler(model, m)
			if err != ErrSkipHandler {
				return err
			}
		}
	}
	return nil
}
