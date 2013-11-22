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
	"errors"
	"fmt"
	"sword"
)

var (
	ErrSkipHandler = errors.New("skip handler")
)

func GetTaskerId(tasker *sword.Tasker) uint32 {
	if id := tasker.GetAutomat(); id != nil {
		return id.GetId()
	} else if id := tasker.GetFormation(); id != nil {
		return id.GetId()
	} else if id := tasker.GetCrowd(); id != nil {
		return id.GetId()
	} else if id := tasker.GetUnit(); id != nil {
		return id.GetId()
	} else if id := tasker.GetParty(); id != nil {
		return id.GetId()
	} else if id := tasker.GetPopulation(); id != nil {
		return id.GetId()
	}
	return 0
}

func (model *Model) handleControlSendCurrentStateEnd(m *sword.SimToClient_Content) error {
	if m.GetControlSendCurrentStateEnd() == nil {
		return ErrSkipHandler
	}
	model.ready = true
	return nil
}

func (model *Model) handleControlBeginTick(m *sword.SimToClient_Content) error {
	mm := m.GetControlBeginTick()
	if mm == nil {
		return ErrSkipHandler
	}
	t, err := GetTime(mm.GetDateTime())
	if err != nil {
		return err
	}
	model.data.Time = t
	model.data.Tick = mm.GetCurrentTick()
	return nil
}

func (model *Model) handleControlInformation(m *sword.SimToClient_Content) error {
	mm := m.GetControlInformation()
	if mm == nil {
		return ErrSkipHandler
	}
	model.data.TickDuration = mm.GetTickDuration()
	return nil
}

func (model *Model) handlePartyCreation(m *sword.SimToClient_Content) error {
	mm := m.GetPartyCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	party := NewParty(
		mm.GetParty().GetId(),
		mm.GetName())
	model.data.Parties[party.Id] = party
	return nil
}

func (model *Model) handleUnitCreation(m *sword.SimToClient_Content) error {
	mm := m.GetUnitCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	unit := &Unit{
		Id:                  mm.GetUnit().GetId(),
		AutomatId:           mm.GetAutomat().GetId(),
		Name:                mm.GetName(),
		Pc:                  mm.GetPc(),
		EquipmentDotations:  map[uint32]*EquipmentDotation{},
		LentEquipments:      []*LentEquipment{},
		BorrowedEquipments:  []*BorrowedEquipment{},
		RawOperationalState: 100,
	}
	if !model.data.addUnit(unit) {
		return fmt.Errorf("cannot insert created unit: %d", unit.Id)
	}
	return nil
}

func (model *Model) handleUnitAttributes(m *sword.SimToClient_Content) error {
	mm := m.GetUnitAttributes()
	if mm == nil {
		return ErrSkipHandler
	}
	unit, ok := model.data.Units[mm.GetUnit().GetId()]
	if !ok {
		return fmt.Errorf("cannot find unit to update: %d",
			mm.GetUnit().GetId())
	}
	if mm.Headquarters != nil {
		unit.Pc = *mm.Headquarters
	}
	if mm.Position != nil {
		unit.Position = ReadPoint(mm.GetPosition())
	}
	if mm.BrainDebug != nil {
		unit.DebugBrain = *mm.BrainDebug
	}
	if mm.RawOperationalState != nil {
		unit.RawOperationalState = *mm.RawOperationalState
	}
	if mm.Neutralized != nil {
		unit.Neutralized = *mm.Neutralized
	}
	if dotations := mm.GetEquipmentDotations(); dotations != nil {
		for _, dotation := range dotations.GetElem() {
			unit.EquipmentDotations[dotation.GetType().GetId()] = &EquipmentDotation{
				Available:     dotation.GetAvailable(),
				Unavailable:   dotation.GetUnavailable(),
				Repairable:    dotation.GetRepairable(),
				OnSiteFixable: dotation.GetOnSiteFixable(),
				Repairing:     dotation.GetRepairing(),
				Captured:      dotation.GetCaptured(),
				Breakdowns:    dotation.GetBreakdowns(),
				Away:          dotation.GetAway()}
		}
	}
	if lentEquipments := mm.GetLentEquipments(); lentEquipments != nil {
		unit.LentEquipments = []*LentEquipment{}
		for _, equipment := range lentEquipments.GetElem() {
			unit.LentEquipments = append(unit.LentEquipments, &LentEquipment{
				Borrower: equipment.GetBorrower().GetId(),
				TypeId:   equipment.GetType().GetId(),
				Quantity: equipment.GetQuantity()})
		}
	}
	if borrowedEquipments := mm.GetBorrowedEquipments(); borrowedEquipments != nil {
		unit.BorrowedEquipments = []*BorrowedEquipment{}
		for _, equipment := range borrowedEquipments.GetElem() {
			unit.BorrowedEquipments = append(unit.BorrowedEquipments, &BorrowedEquipment{
				Owner:    equipment.GetOwner().GetId(),
				TypeId:   equipment.GetType().GetId(),
				Quantity: equipment.GetQuantity()})
		}
	}
	if humanDotations := mm.GetHumanDotations(); humanDotations != nil {
		unit.HumanDotations = []*HumanDotation{}
		for _, human := range humanDotations.GetElem() {
			injury := int32(0)
			if injuries := human.GetInjuries(); len(injuries) != 0 {
				injury = int32(injuries[0].GetSeriousness())
			}
			unit.HumanDotations = append(unit.HumanDotations, &HumanDotation{
				Quantity:     human.GetQuantity(),
				Rank:         int32(human.GetRank()),
				State:        int32(human.GetState()),
				Injury:       injury,
				Psyop:        human.GetMentallyWounded(),
				Contaminated: human.GetContaminated()})
		}
	}
	if resourceDotations := mm.GetResourceDotations(); resourceDotations != nil {
		unit.ResourceDotations = []*ResourceDotation{}
		for _, dotation := range resourceDotations.GetElem() {
			unit.ResourceDotations = append(unit.ResourceDotations, &ResourceDotation{
				Type:      dotation.GetType().GetId(),
				Quantity:  dotation.GetQuantity(),
				Threshold: dotation.GetThreshold()})
		}
	}
	if surrenderedTo := mm.GetSurrenderedUnit(); surrenderedTo != nil {
		unit.PartySurrenderedTo = *surrenderedTo.Id
	}
	if mm.OldPosture != nil {
		unit.Posture.Old = *mm.OldPosture
	}
	if mm.NewPosture != nil {
		unit.Posture.New = *mm.NewPosture
	}
	if mm.PostureTransition != nil {
		unit.Posture.Transition = *mm.PostureTransition
	}
	if transportingUnit := mm.GetTransportingUnit(); transportingUnit != nil {
		unit.TransporterId = transportingUnit.GetId()
	}
	if transported := mm.GetTransportedUnits(); transported != nil {
		unit.TransportedIds = make([]uint32, 0, len(transported.Elem))
		for _, it := range transported.Elem {
			unit.TransportedIds = append(unit.TransportedIds, it.GetId())
		}
	}
	if mm.Adhesions != nil {
		unit.Adhesions = map[uint32]float32{}
		for _, value := range mm.Adhesions.Adhesion {
			unit.Adhesions[value.GetParty().GetId()] = value.GetValue()
		}
	}
	if mm.Tiredness != nil {
		unit.HumanFactors.Tiredness = *mm.Tiredness
	}
	if mm.Morale != nil {
		unit.HumanFactors.Morale = *mm.Morale
	}
	if mm.Experience != nil {
		unit.HumanFactors.Experience = *mm.Experience
	}
	if mm.Stress != nil {
		unit.HumanFactors.Stress = *mm.Stress
	}
	return nil
}

func (model *Model) handleAutomatCreation(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	automat := &Automat{
		Id:               mm.GetAutomat().GetId(),
		PartyId:          mm.GetParty().GetId(),
		Name:             mm.GetName(),
		Engaged:          true,
		KnowledgeGroupId: mm.GetKnowledgeGroup().GetId(),
	}
	if parent := mm.GetParent().GetFormation(); parent != nil {
		automat.FormationId = parent.GetId()
	}
	if !model.data.addAutomat(automat) {
		return fmt.Errorf("cannot insert created automat: %d", automat.Id)
	}
	return nil
}

func (model *Model) handleAutomatAttributes(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatAttributes()
	if mm == nil {
		return ErrSkipHandler
	}
	automat, ok := model.data.Automats[mm.GetAutomat().GetId()]
	if !ok {
		return fmt.Errorf("cannot find automat to update: %d",
			mm.GetAutomat().GetId())
	}
	if mm.Mode != nil {
		mode := mm.GetMode()
		if mode == sword.EnumAutomatMode_engaged {
			automat.Engaged = true
		} else if mode == sword.EnumAutomatMode_disengaged {
			automat.Engaged = false
		}
	}
	if mm.BrainDebug != nil {
		automat.DebugBrain = mm.GetBrainDebug()
	}
	return nil
}

func (model *Model) handleFormationCreation(m *sword.SimToClient_Content) error {
	mm := m.GetFormationCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	level, ok := sword.EnumNatureLevel_name[int32(mm.GetLevel())]
	if !ok {
		level = "unknown"
	}
	logLevel, ok := sword.EnumLogisticLevel_name[int32(mm.GetLogisticLevel())]
	if !ok {
		logLevel = "unknown"
	}
	formation := NewFormation(
		mm.GetFormation().GetId(),
		mm.GetName(),
		mm.GetParent().GetId(),
		mm.GetParty().GetId(),
		level,
		logLevel,
	)
	if !model.data.addFormation(formation) {
		return fmt.Errorf("cannot create formation %v with unknown"+
			"parent party=%v/parent=%v", formation.Id, formation.PartyId,
			formation.ParentId)
	}
	return nil
}

func (model *Model) handleCrowdCreation(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	crowd := NewCrowd(
		mm.GetCrowd().GetId(),
		mm.GetParty().GetId(),
		mm.GetName(),
	)
	if !model.data.addCrowd(crowd) {
		return fmt.Errorf("cannot insert created crowd: %d", crowd.Id)
	}
	return nil
}

func (model *Model) handleCrowdUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	crowd, ok := model.data.Crowds[mm.GetCrowd().GetId()]
	if !ok {
		return fmt.Errorf("cannot find crowd to update: %d",
			mm.GetCrowd().GetId())
	}
	if mm.CriticalIntelligence != nil {
		crowd.CriticalIntelligence = *mm.CriticalIntelligence
	}
	if mm.Healthy != nil {
		crowd.Healthy = *mm.Healthy
	}
	if mm.Wounded != nil {
		crowd.Wounded = *mm.Wounded
	}
	if mm.Dead != nil {
		crowd.Dead = *mm.Dead
	}
	if mm.Contaminated != nil {
		crowd.Contaminated = *mm.Contaminated
	}
	if mm.ArmedIndividuals != nil {
		crowd.ArmedIndividuals = *mm.ArmedIndividuals
	}
	if mm.Adhesions != nil {
		crowd.Adhesions = map[uint32]float32{}
		for _, value := range mm.Adhesions.Adhesion {
			crowd.Adhesions[value.GetParty().GetId()] = value.GetValue()
		}
	}
	if mm.Extension != nil {
		for i := 0; i < len(mm.Extension.Entries); i++ {
			entry := mm.Extension.Entries[i]
			crowd.Extensions[entry.GetName()] = entry.GetValue()
		}
	}
	return nil
}

func (model *Model) handleCrowdFlowCreation(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdFlowCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.addCrowdElement(mm.GetCrowd().GetId(), mm.GetFlow().GetId()) {
		return fmt.Errorf("cannot insert crowd flow %d into crowd %d",
			mm.GetFlow().GetId(), mm.GetCrowd().GetId())
	}
	return nil
}

func (model *Model) handleCrowdFlowDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdFlowDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.removeCrowdElement(mm.GetCrowd().GetId(), mm.GetFlow().GetId()) {
		return fmt.Errorf("cannot remove crowd flow %d from crowd %d",
			mm.GetFlow().GetId(), mm.GetCrowd().GetId())
	}
	return nil
}

func (model *Model) handleCrowdFlowUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdFlowUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	crowd, ok := model.data.Crowds[mm.GetCrowd().GetId()]
	if !ok {
		return fmt.Errorf("cannot find crowd to update: %d",
			mm.GetCrowd().GetId())
	}
	element := crowd.CrowdElements[mm.GetFlow().GetId()]
	if element == nil {
		return fmt.Errorf("cannot find crowd flow to update: %d",
			mm.GetFlow().GetId())
	}
	element.Attitude = int32(mm.GetAttitude())
	return nil
}

func (model *Model) handleCrowdConcentrationCreation(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdConcentrationCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.addCrowdElement(mm.GetCrowd().GetId(), mm.GetConcentration().GetId()) {
		return fmt.Errorf("cannot insert crowd concentration %d into crowd %d",
			mm.GetConcentration().GetId(), mm.GetCrowd().GetId())
	}
	return nil
}

func (model *Model) handleCrowdConcentrationDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdConcentrationDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.removeCrowdElement(mm.GetCrowd().GetId(), mm.GetConcentration().GetId()) {
		return fmt.Errorf("cannot remove crowd concentration %d from crowd %d",
			mm.GetConcentration().GetId(), mm.GetCrowd().GetId())
	}
	return nil
}

func (model *Model) handleCrowdConcentrationUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdConcentrationUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	crowd, ok := model.data.Crowds[mm.GetCrowd().GetId()]
	if !ok {
		return fmt.Errorf("cannot find crowd to update: %d",
			mm.GetCrowd().GetId())
	}
	element := crowd.CrowdElements[mm.GetConcentration().GetId()]
	if element == nil {
		return fmt.Errorf("cannot find crowd concentration to update: %d",
			mm.GetConcentration().GetId())
	}
	element.Attitude = int32(mm.GetAttitude())
	return nil
}

func (model *Model) handlePopulationCreation(m *sword.SimToClient_Content) error {
	mm := m.GetPopulationCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	population := NewPopulation(
		mm.GetId().GetId(),
		mm.GetParty().GetId(),
		mm.GetName(),
	)
	if !model.data.addPopulation(population) {
		return fmt.Errorf("cannot insert population: %d", population.Id)
	}
	for _, o := range mm.GetObjects() {
		population.LivingArea[o.GetId()] = NewBlock(map[string]int32{})
	}
	return nil
}

func (model *Model) handlePopulationUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetPopulationUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	population, ok := model.data.Populations[mm.GetId().GetId()]
	if !ok {
		return fmt.Errorf("cannot find population to update: %d",
			mm.GetId().GetId())
	}
	if mm.Healthy != nil {
		population.Healthy = *mm.Healthy
	}
	if mm.Wounded != nil {
		population.Wounded = *mm.Wounded
	}
	if mm.Dead != nil {
		population.Dead = *mm.Dead
	}
	for _, o := range mm.GetOccupations() {
		residents := map[string]int32{}
		for _, p := range o.GetPersons() {
			residents[*p.Usage] = *p.Number
		}
		population.LivingArea[o.GetObject().GetId()] = NewBlock(residents)
	}
	if mm.Adhesions != nil {
		population.Adhesions = map[uint32]float32{}
		for _, value := range mm.Adhesions.Adhesion {
			population.Adhesions[value.GetParty().GetId()] = value.GetValue()
		}
	}
	return nil
}

func (model *Model) handleUnitPathfind(m *sword.SimToClient_Content) error {
	mm := m.GetUnitPathfind()
	if mm == nil {
		return ErrSkipHandler
	}
	unit, ok := model.data.Units[mm.GetUnit().GetId()]
	if !ok {
		return fmt.Errorf("cannot find pathfind source unit: %d",
			mm.GetUnit().GetId())
	}
	if mm.Path != nil && mm.Path.Location != nil {
		unit.PathPoints = uint32(0)
		if mm.Path.Location.Coordinates != nil {
			unit.PathPoints = uint32(len(mm.Path.Location.Coordinates.Elem))
		}
	}
	return nil
}

func (model *Model) handleUnitDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetUnitDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.removeUnit(mm.GetUnit().GetId()) {
		return fmt.Errorf("cannot find unit to destroy: %d",
			mm.GetUnit().GetId())
	}
	return nil
}

func (model *Model) handleKnowledgeGroupCreation(m *sword.SimToClient_Content) error {
	mm := m.GetKnowledgeGroupCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	group := &KnowledgeGroup{
		Id:                  mm.GetKnowledgeGroup().GetId(),
		Name:                mm.GetName(),
		PartyId:             mm.GetParty().GetId(),
		ParentId:            mm.GetParent().GetId(),
		Type:                mm.GetType(),
		IsCrowdDefaultGroup: mm.GetCrowd(),
		Enabled:             true,
	}
	if !model.data.addKnowledgeGroup(group) {
		return fmt.Errorf("cannot insert knowledge group: %d", group.Id)
	}
	return nil
}

func (model *Model) handleUnitKnowledgeCreation(m *sword.SimToClient_Content) error {
	mm := m.GetUnitKnowledgeCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	knowledge := &UnitKnowledge{
		Id:               mm.GetKnowledge().GetId(),
		KnowledgeGroupId: mm.GetKnowledgeGroup().GetId(),
		UnitId:           mm.GetUnit().GetId(),
		UnitType:         mm.GetType().GetId(),
	}
	if !model.data.addUnitKnowledge(knowledge) {
		return fmt.Errorf("cannot insert unit knowledge %d into group %d",
			knowledge.Id, knowledge.KnowledgeGroupId)
	}
	return nil
}

func (model *Model) handleObjectKnowledgeCreation(m *sword.SimToClient_Content) error {
	mm := m.GetObjectKnowledgeCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	knowledge := &ObjectKnowledge{
		Id:               mm.GetKnowledge().GetId(),
		PartyId:          mm.GetParty().GetId(),
		ObjectId:         mm.GetObject().GetId(),
		ObjectType:       mm.GetType().GetId(),
		KnowledgeGroupId: mm.GetKnowledgeGroup().GetId(),
		ObjectPartyId:    mm.GetObjectParty().GetId(),
		ObjectName:       mm.GetObjectName(),
	}
	if !model.data.addObjectKnowledge(knowledge) {
		return fmt.Errorf("cannot insert object knowledge %d into group %d",
			knowledge.Id, knowledge.KnowledgeGroupId)
	}
	return nil
}

func (model *Model) handleCrowdKnowledgeCreation(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdKnowledgeCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	knowledge := &CrowdKnowledge{
		Id:               mm.GetKnowledge().GetId(),
		KnowledgeGroupId: mm.GetKnowledgeGroup().GetId(),
		CrowdId:          mm.GetCrowd().GetId(),
		PartyId:          mm.GetParty().GetId(),
	}
	if !model.data.addCrowdKnowledge(knowledge) {
		return fmt.Errorf("cannot insert crowd knowledge %d into group %d",
			knowledge.Id, knowledge.KnowledgeGroupId)
	}
	return nil
}

func (model *Model) handleKnowledgeGroupUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetKnowledgeGroupUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	group, ok := model.data.KnowledgeGroups[mm.GetKnowledgeGroup().GetId()]
	if !ok {
		return fmt.Errorf("cannot find knowledge group to update: %d",
			mm.GetKnowledgeGroup().GetId())
	}
	if mm.Party != nil {
		group.PartyId = mm.GetParty().GetId()
	}
	if mm.Parent != nil {
		group.ParentId = mm.GetParent().GetId()
	}
	if mm.Enabled != nil {
		group.Enabled = *mm.Enabled
	}
	if mm.Type != nil {
		group.Type = *mm.Type
	}
	return nil
}

func (model *Model) handleControlGlobalWeather(m *sword.SimToClient_Content) error {
	mm := m.GetControlGlobalWeather()
	if mm == nil {
		return ErrSkipHandler
	}
	attributes := mm.GetAttributes()
	weather := &model.data.GlobalWeather
	weather.Temperature = float32(attributes.GetTemperature())
	weather.WindSpeed = float32(attributes.GetWindSpeed())
	weather.WindDirection = attributes.GetWindDirection().GetHeading()
	weather.CloudFloor = float32(attributes.GetCloudFloor())
	weather.CloudCeil = float32(attributes.GetCloudCeiling())
	weather.CloudDensity = float32(attributes.GetCloudDensity())
	weather.Precipitation = attributes.GetPrecipitation()
	weather.Lightning = attributes.GetLighting()
	return nil
}

func (model *Model) handleControlLocalWeatherCreation(m *sword.SimToClient_Content) error {
	mm := m.GetControlLocalWeatherCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	start, err := GetTime(mm.GetStartDate())
	if err != nil {
		return fmt.Errorf("cannot parse local weather start time: %v",
			mm.GetStartDate())
	}
	end, err := GetTime(mm.GetEndDate())
	if err != nil {
		return fmt.Errorf("cannot parse local weather end time: %v",
			mm.GetEndDate())
	}
	attr := mm.GetAttributes()
	// most attributes are truncated here...
	w := &LocalWeather{
		Weather: Weather{
			Temperature:   float32(attr.GetTemperature()),
			WindSpeed:     float32(attr.GetWindSpeed()),
			WindDirection: attr.GetWindDirection().GetHeading(),
			CloudFloor:    float32(attr.GetCloudFloor()),
			CloudCeil:     float32(attr.GetCloudCeiling()),
			CloudDensity:  float32(attr.GetCloudDensity()),
			Precipitation: attr.GetPrecipitation(),
			//Lighting:      attr.GetLighting(),
		},
		StartTime:   start,
		EndTime:     end,
		TopLeft:     ReadPoint(mm.GetTopLeft()),
		BottomRight: ReadPoint(mm.GetBottomRight()),
		Id:          mm.GetWeather().GetId(),
	}
	model.data.addLocalWeather(w)
	return nil
}

func (model *Model) handleControlLocalWeatherDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetControlLocalWeatherDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.removeLocalWeather(mm.GetWeather().GetId()) {
		return fmt.Errorf("cannot find local weather to destroy: %d",
			mm.GetWeather().GetId())
	}
	return nil
}

func (model *Model) handleChangeDiplomacy(m *sword.SimToClient_Content) error {
	mm := m.GetChangeDiplomacy()
	if mm == nil {
		return ErrSkipHandler
	}
	party1 := model.data.Parties[mm.GetParty1().GetId()]
	if party1 == nil {
		return fmt.Errorf("cannot resolve diplomacy change first party: %d",
			mm.GetParty1().GetId())
	}
	party2 := model.data.Parties[mm.GetParty2().GetId()]
	if party2 == nil {
		return fmt.Errorf("cannot resolve diplomacy change second party: %d",
			mm.GetParty2().GetId())
	}
	party1.Diplomacies[party2.Id] = mm.GetDiplomacy()
	return nil
}

func (model *Model) handleUrbanCreation(m *sword.SimToClient_Content) error {
	mm := m.GetUrbanCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	urban := NewUrban(
		mm.GetObject().GetId(),
		mm.GetName(),
		NewResourceNetworks(mm.GetAttributes()),
	)
	if !model.data.addUrban(urban) {
		return fmt.Errorf("cannot insert urban block: %d", urban.Id)
	}
	return nil
}

func (model *Model) handleUrbanUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetUrbanUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.updateUrban(mm.GetObject().GetId(), NewResourceNetworks(mm.GetAttributes())) {
		return fmt.Errorf("cannot update urban block: %d",
			mm.GetObject().GetId())
	}
	return nil
}

func (model *Model) handleAutomatChangeLogisticLinks(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatChangeLogisticLinks()
	if mm == nil {
		return ErrSkipHandler
	}
	requester := mm.GetRequester()
	superiors := []uint32{}
	for _, s := range mm.GetSuperior() {
		if s.Automat != nil {
			superiors = append(superiors, s.Automat.GetId())
		}
		if s.Formation != nil {
			superiors = append(superiors, s.Formation.GetId())
		}
	}
	if automat := requester.GetAutomat(); automat != nil {
		if !model.data.changeAutomatLogisticsLinks(automat.GetId(), superiors) {
			return fmt.Errorf("cannot update logistic links on automat: %d",
				automat.GetId())
		}
	} else if formation := requester.GetFormation(); formation != nil {
		if !model.data.changeFormationLogisticsLinks(formation.GetId(), superiors) {
			return fmt.Errorf("cannot update logistic links on formation: %d",
				formation.GetId())
		}
	}
	return nil
}

func (model *Model) handleAutomatChangeKnowledgeGroup(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatChangeKnowledgeGroup()
	if mm == nil {
		return ErrSkipHandler
	}
	d := model.data
	_, ok := d.Parties[mm.GetParty().GetId()]
	if !ok {
		return fmt.Errorf("unknown party: %d", mm.GetParty().GetId())
	}
	automat, ok := d.Automats[mm.GetAutomat().GetId()]
	if !ok {
		return fmt.Errorf("unknown automat: %d", mm.GetAutomat().GetId())
	}
	group, ok := d.KnowledgeGroups[mm.GetKnowledgeGroup().GetId()]
	if !ok {
		return fmt.Errorf("unknown knowledge group: %d", mm.GetKnowledgeGroup().GetId())
	}
	if group.PartyId != mm.GetParty().GetId() {
		return fmt.Errorf("bad party %d for knowledge group %d", mm.GetParty().GetId(), mm.GetKnowledgeGroup().GetId())
	}
	automat.KnowledgeGroupId = mm.GetKnowledgeGroup().GetId()
	return nil
}

func (model *Model) handleLogSupplyQuotas(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyQuotas()
	if mm == nil {
		return ErrSkipHandler
	}
	quotas := map[uint32]int32{}
	for _, quota := range mm.GetQuotas().GetElem() {
		quotas[quota.GetResource().GetId()] = quota.GetQuantity()
	}
	d := model.data
	if automat := mm.GetSupplied().Automat; automat != nil {
		if !d.changeAutomatSupplyQuotas(automat.GetId(), quotas) {
			return fmt.Errorf("cannot update supply quotas on automat: %d",
				automat.GetId())
		}
	} else if formation := mm.GetSupplied().Formation; formation != nil {
		if !d.changeFormationSupplyQuotas(formation.GetId(), quotas) {
			return fmt.Errorf("cannot update supply quotas on formation: %d",
				formation.GetId())
		}
	}
	return nil
}

func (model *Model) handleUnitChangeSuperior(m *sword.SimToClient_Content) error {
	mm := m.GetUnitChangeSuperior()
	if mm == nil {
		return ErrSkipHandler
	}
	unitId := mm.GetUnit().GetId()
	automatId := mm.GetParent().GetId()
	if err := model.data.changeUnitAutomat(unitId, automatId); err != nil {
		return fmt.Errorf("cannot change %d unit automat to %d: %s",
			unitId, automatId, err)
	}
	return nil
}

func (model *Model) handleAutomatChangeSuperior(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatChangeSuperior()
	if mm == nil {
		return ErrSkipHandler
	}
	d := model.data
	automatId := mm.GetAutomat().GetId()
	formationId := mm.GetSuperior().GetFormation().GetId()
	if err := d.changeAutomatFormation(automatId, formationId); err != nil {
		return fmt.Errorf("cannot change %d automat formation to %d: %s",
			automatId, formationId, err)
	}
	return nil
}

func (model *Model) handleFormationChangeSuperior(m *sword.SimToClient_Content) error {
	mm := m.GetFormationChangeSuperior()
	if mm == nil {
		return ErrSkipHandler
	}
	d := model.data
	formationId := mm.GetFormation().GetId()
	partyId := mm.GetSuperior().GetParty().GetId()
	parentId := mm.GetSuperior().GetFormation().GetId()
	if err := d.changeFormationSuperior(formationId, partyId, parentId); err != nil {
		return fmt.Errorf("cannot change %d formation superior: %s",
			formationId, err)
	}
	return nil
}

func (model *Model) addOrder(id, parentId, missionType, tasker uint32, kind OrderKind) error {
	if missionType == 0 {
		return nil
	}
	order := &Order{
		Id:          id,
		ParentId:    parentId,
		MissionType: missionType,
		TaskerId:    tasker,
		Kind:        kind,
	}
	if !model.data.addOrder(order) {
		return fmt.Errorf("cannot insert order %d", order.Id)
	}
	return nil
}

func (model *Model) handleUnitOrder(m *sword.SimToClient_Content) error {
	mm := m.GetUnitOrder()
	if mm == nil {
		return ErrSkipHandler
	}
	return model.addOrder(mm.GetId(), mm.GetParent(), mm.GetType().GetId(), mm.GetTasker().GetId(), UnitOrder)
}

func (model *Model) handleAutomatOrder(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatOrder()
	if mm == nil {
		return ErrSkipHandler
	}
	return model.addOrder(mm.GetId(), 0, mm.GetType().GetId(), mm.GetTasker().GetId(), AutomatOrder)
}

func (model *Model) handleCrowdOrder(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdOrder()
	if mm == nil {
		return ErrSkipHandler
	}
	return model.addOrder(mm.GetId(), 0, mm.GetType().GetId(), mm.GetTasker().GetId(), CrowdOrder)
}

func (model *Model) handleFragOrder(m *sword.SimToClient_Content) error {
	mm := m.GetFragOrder()
	if mm == nil {
		return ErrSkipHandler
	}
	return model.addOrder(mm.GetId(), 0, mm.GetType().GetId(),
		GetTaskerId(mm.GetTasker()), FragOrder)
}

func (model *Model) handleUnitVisionCones(m *sword.SimToClient_Content) error {
	mm := m.GetUnitVisionCones()
	if mm == nil {
		return ErrSkipHandler
	}
	unit, ok := model.data.Units[mm.GetUnit().GetId()]
	if !ok {
		return fmt.Errorf("cannot find unit for which vision cones must be updated: %d",
			mm.GetUnit().GetId())
	}
	unit.VisionCones.Elongation = mm.GetElongation()
	unit.VisionCones.Cones = nil
	for _, cone := range mm.GetCones().GetElem() {
		headings := []int32{}
		for _, heading := range cone.GetDirections().GetElem() {
			headings = append(headings, heading.GetHeading())
		}
		unit.VisionCones.Cones = append(unit.VisionCones.Cones,
			&VisionCone{
				Origin: Point{
					X: cone.GetOrigin().GetLatitude(),
					Y: cone.GetOrigin().GetLongitude(),
				},
				Height:   cone.GetHeight(),
				Sensor:   cone.GetSensor(),
				Headings: headings,
			})
	}
	return nil
}

func (model *Model) handleObjectCreation(m *sword.SimToClient_Content) error {
	mm := m.GetObjectCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	object := NewObject(mm.GetObject().GetId(),
		mm.GetParty().GetId(),
		mm.GetType().GetId(),
		mm.GetName(),
	)
	if !model.data.addObject(object) {
		return fmt.Errorf("cannot insert created object: %d", object.Id)
	}
	return nil
}

func (model *Model) handleObjectUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetObjectUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	object, ok := model.data.Objects[mm.GetObject().GetId()]
	if !ok {
		return ErrSkipHandler // can be an urban bloc
	}
	if attributes := mm.GetAttributes(); attributes != nil {
		if obstacle := attributes.GetObstacle(); obstacle != nil {
			object.Activated = obstacle.GetActivated()
		}
		if bypass := attributes.GetBypass(); bypass != nil {
			object.Bypass = bypass.GetPercentage()
		}
		if altitude := attributes.GetAltitudeModifier(); altitude != nil {
			object.Altitude = altitude.GetHeight()
		}
		if construction := attributes.GetConstruction(); construction != nil {
			object.Construction = construction.GetPercentage()
		}
	}
	return nil
}

func (model *Model) handleObjectDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetObjectDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.data.removeObject(mm.GetObject().GetId()) {
		return fmt.Errorf("cannot find object to destroy: %d",
			mm.GetObject().GetId())
	}
	return nil
}

func (model *Model) handleLogMaintenanceHandlingCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogMaintenanceHandlingCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, found := model.data.MaintenanceHandlings[id]; found {
		return fmt.Errorf("maintenance handling to create already exists: %d", id)
	}
	model.data.MaintenanceHandlings[id] = &MaintenanceHandling{Id: id}
	return nil
}

func (model *Model) handleLogMaintenanceHandlingUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogMaintenanceHandlingUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if h, ok := model.data.MaintenanceHandlings[id]; ok {
		h.Provider = &MaintenanceHandlingProvider{
			Id:    mm.GetProvider().GetId(),
			State: mm.GetState(),
		}
		return nil
	}
	return fmt.Errorf("cannot find maintenance handling to update: %d", id)
}

func (model *Model) handleLogMaintenanceHandlingDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogMaintenanceHandlingDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, ok := model.data.MaintenanceHandlings[id]; ok {
		delete(model.data.MaintenanceHandlings, id)
		return nil
	}
	return fmt.Errorf("cannot find maintenance handling to destroy: %d", id)
}

func (model *Model) handleLogMedicalHandlingCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogMedicalHandlingCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, found := model.data.MedicalHandlings[id]; found {
		return fmt.Errorf("medical handling to create already exists: %d", id)
	}
	model.data.MedicalHandlings[id] = &MedicalHandling{Id: id}
	return nil
}

func (model *Model) handleLogMedicalHandlingUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogMedicalHandlingUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if h, ok := model.data.MedicalHandlings[id]; ok {
		h.Provider = &MedicalHandlingProvider{
			Id:    mm.GetProvider().GetId(),
			State: mm.GetState(),
		}
		return nil
	}
	return fmt.Errorf("cannot find medical handling to update: %d", id)
}

func (model *Model) handleLogMedicalHandlingDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogMedicalHandlingDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, ok := model.data.MedicalHandlings[id]; ok {
		delete(model.data.MedicalHandlings, id)
		return nil
	}
	return fmt.Errorf("cannot find medical handling to destroy: %d", id)
}

func (model *Model) handleLogFuneralHandlingCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogFuneralHandlingCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, found := model.data.FuneralHandlings[id]; found {
		return fmt.Errorf("funeral handling to create already exists: %d", id)
	}
	model.data.FuneralHandlings[id] = &FuneralHandling{
		Id:     mm.GetRequest().GetId(),
		UnitId: mm.GetUnit().GetId(),
	}
	return nil
}

func GetParentEntityId(parentEntity *sword.ParentEntity) uint32 {
	if parentEntity.GetAutomat() != nil {
		return parentEntity.GetAutomat().GetId()
	}
	return parentEntity.GetFormation().GetId()
}

func (model *Model) handleLogFuneralHandlingUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogFuneralHandlingUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if h, ok := model.data.FuneralHandlings[id]; ok {
		h.Handler = &FuneralHandlingHandler{
			State: mm.GetState(),
			Id:    GetParentEntityId(mm.GetHandlingUnit()),
		}
		return nil
	}
	return fmt.Errorf("cannot find funeral handling to update: %d", id)
}

func (model *Model) handleLogFuneralHandlingDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogFuneralHandlingDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, ok := model.data.FuneralHandlings[id]; ok {
		delete(model.data.FuneralHandlings, id)
		return nil
	}
	return fmt.Errorf("cannot find funeral handling to destroy: %d", id)
}

func (model *Model) handleLogSupplyHandlingCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyHandlingCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, found := model.data.SupplyHandlings[id]; found {
		return fmt.Errorf("supply handling to create already exists: %d", id)
	}
	model.data.SupplyHandlings[id] = &SupplyHandling{
		Id:         mm.GetRequest().GetId(),
		SupplierId: GetParentEntityId(mm.GetSupplier()),
		ProviderId: GetParentEntityId(mm.GetTransportersProvider()),
	}
	return nil
}

func (model *Model) handleLogSupplyHandlingUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyHandlingUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if h, ok := model.data.SupplyHandlings[id]; ok {
		h.Convoy = &SupplyHandlingConvoy{
			ConvoyerId: mm.GetConvoyer().GetId(),
			State:      mm.GetState(),
		}
		return nil
	}
	return fmt.Errorf("cannot find supply handling to update: %d", id)
}

func (model *Model) handleLogSupplyHandlingDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyHandlingDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, ok := model.data.SupplyHandlings[id]; ok {
		delete(model.data.SupplyHandlings, id)
		return nil
	}
	return fmt.Errorf("cannot find supply handling to destroy: %d", id)
}
