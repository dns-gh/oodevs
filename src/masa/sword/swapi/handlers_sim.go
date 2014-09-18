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
	"masa/sword/sword"
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

func (model *ModelData) handleControlSendCurrentStateBegin(m *sword.SimToClient_Content) error {
	if m.GetControlSendCurrentStateBegin() == nil {
		return ErrSkipHandler
	}
	model.Reset()
	return nil
}

func (model *ModelData) handleControlSendCurrentStateEnd(m *sword.SimToClient_Content) error {
	if m.GetControlSendCurrentStateEnd() == nil {
		return ErrSkipHandler
	}
	model.Ready = true
	return nil
}

func (model *ModelData) handleControlBeginTick(m *sword.SimToClient_Content) error {
	mm := m.GetControlBeginTick()
	if mm == nil {
		return ErrSkipHandler
	}
	t, err := GetTime(mm.GetDateTime().GetData())
	if err != nil {
		return err
	}
	model.Time = t
	model.Tick = mm.GetCurrentTick()
	return nil
}

func (model *ModelData) handleControlChangeTimeFactor(m *sword.SimToClient_Content) error {
	mm := m.GetControlChangeTimeFactorAck()
	if mm == nil {
		return ErrSkipHandler
	}
	model.TimeFactor = mm.GetTimeFactor()
	return nil
}

func (model *ModelData) handleControlEndTick(m *sword.SimToClient_Content) error {
	mm := m.GetControlEndTick()
	if mm == nil {
		return ErrSkipHandler
	}
	model.LastTick = mm.GetCurrentTick()
	return nil
}

func (model *ModelData) handleControlInformation(m *sword.SimToClient_Content) error {
	mm := m.GetControlInformation()
	if mm == nil {
		return ErrSkipHandler
	}
	t, err := GetTime(mm.GetDateTime().GetData())
	if err != nil {
		return err
	}
	model.Time = t
	model.TickDuration = mm.GetTickDuration()
	if !model.Ready {
		// At least in replayer case, the initial state contains a
		// ControlInformation which helps initialize the model. But it
		// indicates the next first tick to be started.
		model.Tick = mm.GetCurrentTick() - 1
	}
	return nil
}

func (model *ModelData) handlePartyCreation(m *sword.SimToClient_Content) error {
	mm := m.GetPartyCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	party := NewParty(
		mm.GetParty().GetId(),
		mm.GetName(),
	)
	if !model.addParty(party) {
		//ignore errors for now
		//return fmt.Errorf("cannot insert party %d", party.Id)
	}
	return nil
}

func (model *ModelData) handlePartyUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetPartyUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	party, ok := model.Parties[mm.GetParty().GetId()]
	if !ok {
		return fmt.Errorf("cannot update party %d",
			mm.GetParty().GetId())
	}
	if mm.Name != nil {
		party.Name = *mm.Name
	}
	return nil
}

func (model *ModelData) handleLogSupplyState(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyState()
	if mm == nil {
		return ErrSkipHandler
	}
	unitId := mm.GetUnit().GetId()
	unit, ok := model.Units[unitId]
	if !ok {
		return fmt.Errorf("cannot apply handle log supply state for unit %d", unitId)
	}
	unit.IsSupplyEnabled = mm.GetChain()
	if stocks := mm.GetStocks(); stocks != nil {
		for _, stock := range stocks.GetElem() {
			unit.Stocks = append(unit.Stocks, &Stock{
				Type:     stock.GetResource().GetId(),
				Quantity: stock.GetQuantity(),
			})
		}
	}
	return nil
}

func (model *ModelData) handleLogMedicalState(m *sword.SimToClient_Content) error {
	mm := m.GetLogMedicalState()
	if mm == nil {
		return ErrSkipHandler
	}
	unitId := mm.GetUnit().GetId()
	unit, ok := model.Units[unitId]
	if !ok {
		return fmt.Errorf("cannot apply handle log medical state for unit %d", unitId)
	}
	unit.IsMedicalEnabled = mm.GetChain()
	// unused fields
	return nil
}

func (model *ModelData) handleLogMaintenanceState(m *sword.SimToClient_Content) error {
	mm := m.GetLogMaintenanceState()
	if mm == nil {
		return ErrSkipHandler
	}
	unitId := mm.GetUnit().GetId()
	unit, ok := model.Units[unitId]
	if !ok {
		return fmt.Errorf("cannot apply handle log maintenance state for unit %d", unitId)
	}
	unit.IsMaintenanceEnabled = mm.GetChain()
	// unused fields
	return nil
}

func (model *ModelData) handleUnitCreation(m *sword.SimToClient_Content) error {
	mm := m.GetUnitCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	unit := &Unit{
		Id:                  mm.GetUnit().GetId(),
		Type:                mm.GetType().GetId(),
		AutomatId:           mm.GetAutomat().GetId(),
		Name:                mm.GetName(),
		Pc:                  mm.GetPc(),
		Equipments:          map[uint32]*Equipment{},
		LentEquipments:      []*LentEquipment{},
		BorrowedEquipments:  []*BorrowedEquipment{},
		Resources:           map[uint32]Resource{},
		RawOperationalState: 100,
	}
	if !model.addUnit(unit) {
		return fmt.Errorf("cannot insert unit %d", unit.Id)
	}
	return nil
}

func (model *ModelData) handleUnitAttributes(m *sword.SimToClient_Content) error {
	mm := m.GetUnitAttributes()
	if mm == nil {
		return ErrSkipHandler
	}
	unit, ok := model.Units[mm.GetUnit().GetId()]
	if !ok {
		return fmt.Errorf("cannot update unit %d",
			mm.GetUnit().GetId())
	}
	if mm.Headquarters != nil {
		unit.Pc = *mm.Headquarters
	}
	if mm.Position != nil {
		unit.Position = ReadPoint(mm.GetPosition())
	}
	if mm.Speed != nil {
		unit.Speed = *mm.Speed
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
			unit.Equipments[dotation.GetType().GetId()] = &Equipment{
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
	if humans := mm.GetHumanDotations(); humans != nil {
		unit.Humans = []*Human{}
		for _, human := range humans.GetElem() {
			injury := int32(0)
			if injuries := human.GetInjuries(); len(injuries) != 0 {
				injury = int32(injuries[0].GetSeriousness())
			}
			unit.Humans = append(unit.Humans, &Human{
				Quantity:     human.GetQuantity(),
				Rank:         int32(human.GetRank()),
				State:        int32(human.GetState()),
				Injury:       injury,
				Psyop:        human.GetMentallyWounded(),
				Contaminated: human.GetContaminated()})
		}
	}
	if resources := mm.GetResourceDotations(); resources != nil {
		for _, dotation := range resources.GetElem() {
			unit.Resources[dotation.GetType().GetId()] = Resource{
				Quantity:      dotation.GetQuantity(),
				LowThreshold:  dotation.GetLowThreshold(),
				HighThreshold: dotation.GetHighThreshold()}
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
		unit.TransportedIds = ReadIds(transported.Elem)
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
	if mm.Installation != nil {
		unit.Installation = *mm.Installation
	}
	if mm.Name != nil {
		unit.Name = *mm.Name
	}
	return nil
}

func (model *ModelData) handleAutomatCreation(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	automat := &Automat{
		Id:                   mm.GetAutomat().GetId(),
		PartyId:              mm.GetParty().GetId(),
		Name:                 mm.GetName(),
		Engaged:              true,
		KnowledgeGroupId:     mm.GetKnowledgeGroup().GetId(),
		LogMaintenanceManual: mm.GetLogMaintenanceManual(),
		LogSupplyManual:      mm.GetLogSupplyManual(),
	}
	if parent := mm.GetParent().GetFormation(); parent != nil {
		automat.FormationId = parent.GetId()
	}
	if !model.addAutomat(automat) {
		return fmt.Errorf("cannot insert automat %d", automat.Id)
	}
	return nil
}

func (model *ModelData) handleAutomatDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetAutomat().GetId()
	if !model.removeAutomat(id) {
		return fmt.Errorf("cannot destroy automat %d", id)
	}
	return nil
}

func (model *ModelData) handleAutomatAttributes(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatAttributes()
	if mm == nil {
		return ErrSkipHandler
	}
	automat, ok := model.Automats[mm.GetAutomat().GetId()]
	if !ok {
		return fmt.Errorf("cannot update automat %d",
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
		automat.DebugBrain = *mm.BrainDebug
	}
	if mm.LogMaintenanceManual != nil {
		automat.LogMaintenanceManual = *mm.LogMaintenanceManual
	}
	if mm.LogSupplyManual != nil {
		automat.LogSupplyManual = *mm.LogSupplyManual
	}
	if mm.Name != nil {
		automat.Name = *mm.Name
	}
	return nil
}

func (model *ModelData) handleFormationCreation(m *sword.SimToClient_Content) error {
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
	formation := &Formation{
		Id:                   mm.GetFormation().GetId(),
		Name:                 mm.GetName(),
		ParentId:             mm.GetParent().GetId(),
		PartyId:              mm.GetParty().GetId(),
		Level:                level,
		LogLevel:             logLevel,
		LogMaintenanceManual: mm.GetLogMaintenanceManual(),
		LogSupplyManual:      mm.GetLogSupplyManual(),
	}
	if !model.addFormation(formation) {
		return fmt.Errorf("cannot create formation %v with unknown"+
			"parent party=%v/parent=%v", formation.Id, formation.PartyId,
			formation.ParentId)
	}
	return nil
}

func (model *ModelData) handleFormationDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetFormationDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.removeFormation(mm.GetFormation().GetId()) {
		return fmt.Errorf("cannot destroy formation %d",
			mm.GetFormation().GetId())
	}
	return nil
}

func (model *ModelData) handleFormationUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetFormationUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	formation, ok := model.Formations[mm.GetFormation().GetId()]
	if !ok {
		return fmt.Errorf("cannot update formation %d",
			mm.GetFormation().GetId())
	}
	if mm.LogMaintenanceManual != nil {
		formation.LogMaintenanceManual = *mm.LogMaintenanceManual
	}
	if mm.LogSupplyManual != nil {
		formation.LogSupplyManual = *mm.LogSupplyManual
	}
	if mm.Name != nil {
		formation.Name = *mm.Name
	}
	return nil
}

func (model *ModelData) handleCrowdCreation(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	crowd := NewCrowd(
		mm.GetCrowd().GetId(),
		mm.GetParty().GetId(),
		mm.GetName(),
	)
	if !model.addCrowd(crowd) {
		return fmt.Errorf("cannot insert crowd %d", crowd.Id)
	}
	return nil
}

func (model *ModelData) handleCrowdDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetCrowd().GetId()
	if !model.removeCrowd(id) {
		return fmt.Errorf("cannot destroy crowd %d", id)
	}
	return nil
}

func (model *ModelData) handleCrowdUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	crowd, ok := model.Crowds[mm.GetCrowd().GetId()]
	if !ok {
		return fmt.Errorf("cannot update crowd %d",
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
	if mm.Name != nil {
		crowd.Name = *mm.Name
	}
	return nil
}

func (model *ModelData) handleCrowdFlowCreation(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdFlowCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.addCrowdElement(mm.GetCrowd().GetId(), mm.GetFlow().GetId(), Point{}) {
		return fmt.Errorf("cannot insert crowd flow %d into crowd %d",
			mm.GetFlow().GetId(), mm.GetCrowd().GetId())
	}
	return nil
}

func (model *ModelData) handleCrowdFlowDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdFlowDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.removeCrowdElement(mm.GetCrowd().GetId(), mm.GetFlow().GetId()) {
		return fmt.Errorf("cannot remove crowd flow %d from crowd %d",
			mm.GetFlow().GetId(), mm.GetCrowd().GetId())
	}
	return nil
}

func (model *ModelData) handleCrowdFlowUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdFlowUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	crowd, ok := model.Crowds[mm.GetCrowd().GetId()]
	if !ok {
		return fmt.Errorf("cannot update crowd %d",
			mm.GetCrowd().GetId())
	}
	element := crowd.CrowdElements[mm.GetFlow().GetId()]
	if element == nil {
		return fmt.Errorf("cannot update crowd flow %d",
			mm.GetFlow().GetId())
	}
	element.Attitude = int32(mm.GetAttitude())
	return nil
}

func (model *ModelData) handleCrowdConcentrationCreation(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdConcentrationCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.addCrowdElement(mm.GetCrowd().GetId(), mm.GetConcentration().GetId(), ReadPoint(mm.GetPosition())) {
		return fmt.Errorf("cannot insert crowd concentration %d into crowd %d",
			mm.GetConcentration().GetId(), mm.GetCrowd().GetId())
	}
	return nil
}

func (model *ModelData) handleCrowdConcentrationDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdConcentrationDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.removeCrowdElement(mm.GetCrowd().GetId(), mm.GetConcentration().GetId()) {
		return fmt.Errorf("cannot remove crowd concentration %d from crowd %d",
			mm.GetConcentration().GetId(), mm.GetCrowd().GetId())
	}
	return nil
}

func (model *ModelData) handleCrowdConcentrationUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdConcentrationUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	crowd, ok := model.Crowds[mm.GetCrowd().GetId()]
	if !ok {
		return fmt.Errorf("cannot update crowd %d",
			mm.GetCrowd().GetId())
	}
	element := crowd.CrowdElements[mm.GetConcentration().GetId()]
	if element == nil {
		return fmt.Errorf("cannot update crowd concentration %d",
			mm.GetConcentration().GetId())
	}
	element.Attitude = int32(mm.GetAttitude())
	return nil
}

func (model *ModelData) handlePopulationCreation(m *sword.SimToClient_Content) error {
	mm := m.GetPopulationCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	population := NewPopulation(
		mm.GetId().GetId(),
		mm.GetParty().GetId(),
		mm.GetName(),
	)
	if !model.addPopulation(population) {
		return fmt.Errorf("cannot insert population %d", population.Id)
	}
	for _, o := range mm.GetObjects() {
		population.LivingArea[o.GetId()] = NewBlock(map[string]int32{})
	}
	return nil
}

func (model *ModelData) handlePopulationUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetPopulationUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	population, ok := model.Populations[mm.GetId().GetId()]
	if !ok {
		return fmt.Errorf("cannot update population %d",
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
	if mm.Name != nil {
		population.Name = *mm.Name
	}
	return nil
}

func (model *ModelData) handleUnitPathfind(m *sword.SimToClient_Content) error {
	mm := m.GetUnitPathfind()
	if mm == nil {
		return ErrSkipHandler
	}
	unit, ok := model.Units[mm.GetUnit().GetId()]
	if !ok {
		return fmt.Errorf("cannot find pathfind source unit %d",
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

func (model *ModelData) handleUnitDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetUnitDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.removeUnit(mm.GetUnit().GetId()) {
		return fmt.Errorf("cannot destroy unit %d",
			mm.GetUnit().GetId())
	}
	return nil
}

func (model *ModelData) handleKnowledgeGroupCreation(m *sword.SimToClient_Content) error {
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
	if !model.addKnowledgeGroup(group) {
		return fmt.Errorf("cannot insert knowledge group %d", group.Id)
	}
	return nil
}

func (model *ModelData) handleKnowledgeGroupDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetKnowledgeGroupDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetKnowledgeGroup().GetId()
	if !model.removeKnowledgeGroup(id) {
		return fmt.Errorf("cannot destroy knowledge group %d", id)
	}
	return nil
}

func (model *ModelData) handleKnowledgeGroupUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetKnowledgeGroupUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	group, ok := model.KnowledgeGroups[mm.GetKnowledgeGroup().GetId()]
	if !ok {
		return fmt.Errorf("cannot update knowledge group %d",
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
	if mm.Name != nil {
		group.Name = *mm.Name
	}
	return nil
}

func (model *ModelData) handleUnitKnowledgeCreation(m *sword.SimToClient_Content) error {
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
	if !model.addUnitKnowledge(knowledge) {
		return fmt.Errorf("cannot insert unit knowledge %d into group %d",
			knowledge.Id, knowledge.KnowledgeGroupId)
	}
	return nil
}

func (model *ModelData) handleUnitKnowledgeUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetUnitKnowledgeUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetKnowledge().GetId()
	k, ok := model.UnitKnowledges[id]
	if !ok {
		return fmt.Errorf("cannot update unit knowledge %d", id)
	}
	k.KnowledgeGroupId = mm.KnowledgeGroup.GetId()
	if mm.IdentificationLevel != nil {
		k.IdentificationLevel = int32(*mm.IdentificationLevel)
	}
	return nil
}

func (model *ModelData) handleUnitKnowledgeDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetUnitKnowledgeDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetKnowledge().GetId()
	if !model.removeUnitKnowledge(id) {
		return fmt.Errorf("cannot destroy unit knowledge %d", id)
	}
	return nil
}

func (model *ModelData) handleObjectKnowledgeCreation(m *sword.SimToClient_Content) error {
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
	if !model.addObjectKnowledge(knowledge) {
		return fmt.Errorf("cannot insert object knowledge %d into group %d",
			knowledge.Id, knowledge.KnowledgeGroupId)
	}
	return nil
}

func (model *ModelData) handleObjectKnowledgeDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetObjectKnowledgeDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetKnowledge().GetId()
	if !model.removeObjectKnowledge(id) {
		return fmt.Errorf("cannot destroy object knowledge %d", id)
	}
	return nil
}

func (model *ModelData) handleCrowdKnowledgeCreation(m *sword.SimToClient_Content) error {
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
	if !model.addCrowdKnowledge(knowledge) {
		return fmt.Errorf("cannot insert crowd knowledge %d into group %d",
			knowledge.Id, knowledge.KnowledgeGroupId)
	}
	return nil
}

func (model *ModelData) handleCrowdKnowledgeDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdKnowledgeDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetKnowledge().GetId()
	if !model.removeCrowdKnowledge(id) {
		return fmt.Errorf("cannot destroy crowd knowledge %d", id)
	}
	return nil
}

func (model *ModelData) handleControlGlobalWeather(m *sword.SimToClient_Content) error {
	mm := m.GetControlGlobalWeather()
	if mm == nil {
		return ErrSkipHandler
	}
	attributes := mm.GetAttributes()
	weather := &model.GlobalWeather
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

func (model *ModelData) handleControlLocalWeatherCreation(m *sword.SimToClient_Content) error {
	mm := m.GetControlLocalWeatherCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	start, err := GetTime(mm.GetStartDate().GetData())
	if err != nil {
		return fmt.Errorf("cannot parse local weather start time: %v",
			mm.GetStartDate())
	}
	end, err := GetTime(mm.GetEndDate().GetData())
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
	model.LocalWeathers[w.Id] = w
	return nil
}

func (model *ModelData) handleControlLocalWeatherDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetControlLocalWeatherDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.removeLocalWeather(mm.GetWeather().GetId()) {
		return fmt.Errorf("cannot destroy local weather %d",
			mm.GetWeather().GetId())
	}
	return nil
}

func (model *ModelData) handleChangeDiplomacy(m *sword.SimToClient_Content) error {
	mm := m.GetChangeDiplomacy()
	if mm == nil {
		return ErrSkipHandler
	}
	party1 := model.Parties[mm.GetParty1().GetId()]
	if party1 == nil {
		return fmt.Errorf("cannot resolve diplomacy change on first party %d",
			mm.GetParty1().GetId())
	}
	party2 := model.Parties[mm.GetParty2().GetId()]
	if party2 == nil {
		return fmt.Errorf("cannot resolve diplomacy change on second party %d",
			mm.GetParty2().GetId())
	}
	party1.Diplomacies[party2.Id] = mm.GetDiplomacy()
	return nil
}

func (model *ModelData) handleUrbanCreation(m *sword.SimToClient_Content) error {
	mm := m.GetUrbanCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	urban := NewUrban(
		mm.GetObject().GetId(),
		mm.GetName(),
		NewResourceNetworks(mm.GetAttributes()),
	)
	if !model.addObject(urban) {
		return fmt.Errorf("cannot insert urban block %d", urban.Id)
	}
	return nil
}

func (model *ModelData) handleUrbanUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetUrbanUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.updateUrban(mm.GetObject().GetId(), NewResourceNetworks(mm.GetAttributes())) {
		return fmt.Errorf("cannot update urban block %d",
			mm.GetObject().GetId())
	}
	return nil
}

func (model *ModelData) handleAutomatChangeLogisticLinks(m *sword.SimToClient_Content) error {
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
		if !model.changeAutomatLogisticsLinks(automat.GetId(), superiors) {
			return fmt.Errorf("cannot update logistic links on automat %d",
				automat.GetId())
		}
	} else if formation := requester.GetFormation(); formation != nil {
		if !model.changeFormationLogisticsLinks(formation.GetId(), superiors) {
			return fmt.Errorf("cannot update logistic links on formation %d",
				formation.GetId())
		}
	}
	return nil
}

func (model *ModelData) handleAutomatChangeKnowledgeGroup(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatChangeKnowledgeGroup()
	if mm == nil {
		return ErrSkipHandler
	}
	d := model
	_, ok := d.Parties[mm.GetParty().GetId()]
	if !ok {
		return fmt.Errorf("unknown party %d", mm.GetParty().GetId())
	}
	automat, ok := d.Automats[mm.GetAutomat().GetId()]
	if !ok {
		return fmt.Errorf("unknown automat %d", mm.GetAutomat().GetId())
	}
	group, ok := d.KnowledgeGroups[mm.GetKnowledgeGroup().GetId()]
	if !ok {
		return fmt.Errorf("unknown knowledge group %d", mm.GetKnowledgeGroup().GetId())
	}
	if group.PartyId != mm.GetParty().GetId() {
		return fmt.Errorf("bad party %d for knowledge group %d", mm.GetParty().GetId(), mm.GetKnowledgeGroup().GetId())
	}
	automat.KnowledgeGroupId = mm.GetKnowledgeGroup().GetId()
	return nil
}

func (model *ModelData) handleLogSupplyQuotas(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyQuotas()
	if mm == nil {
		return ErrSkipHandler
	}
	quotas := map[uint32]int32{}
	for _, quota := range mm.GetQuotas().GetElem() {
		quotas[quota.GetResource().GetId()] = quota.GetQuantity()
	}
	d := model
	if automat := mm.GetSupplied().Automat; automat != nil {
		if !d.changeAutomatSupplyQuotas(automat.GetId(), quotas) {
			return fmt.Errorf("cannot update supply quotas on automat %d",
				automat.GetId())
		}
	} else if formation := mm.GetSupplied().Formation; formation != nil {
		if !d.changeFormationSupplyQuotas(formation.GetId(), quotas) {
			return fmt.Errorf("cannot update supply quotas on formation %d",
				formation.GetId())
		}
	}
	return nil
}

func (model *ModelData) handleUnitChangeSuperior(m *sword.SimToClient_Content) error {
	mm := m.GetUnitChangeSuperior()
	if mm == nil {
		return ErrSkipHandler
	}
	unitId := mm.GetUnit().GetId()
	automatId := mm.GetParent().GetId()
	if err := model.changeUnitAutomat(unitId, automatId); err != nil {
		return fmt.Errorf("cannot change %d unit automat to %d: %s",
			unitId, automatId, err)
	}
	return nil
}

func (model *ModelData) handleAutomatChangeSuperior(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatChangeSuperior()
	if mm == nil {
		return ErrSkipHandler
	}
	d := model
	automatId := mm.GetAutomat().GetId()
	formationId := mm.GetSuperior().GetFormation().GetId()
	if err := d.changeAutomatFormation(automatId, formationId); err != nil {
		return fmt.Errorf("cannot change %d automat formation to %d: %s",
			automatId, formationId, err)
	}
	return nil
}

func (model *ModelData) handleFormationChangeSuperior(m *sword.SimToClient_Content) error {
	mm := m.GetFormationChangeSuperior()
	if mm == nil {
		return ErrSkipHandler
	}
	d := model
	formationId := mm.GetFormation().GetId()
	partyId := mm.GetSuperior().GetParty().GetId()
	parentId := mm.GetSuperior().GetFormation().GetId()
	if err := d.changeFormationSuperior(formationId, partyId, parentId); err != nil {
		return fmt.Errorf("cannot change %d formation superior: %s",
			formationId, err)
	}
	return nil
}

func addOrder(model *ModelData, id, parentId, missionType, tasker uint32,
	kind OrderKind) error {
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
	if !model.addOrder(order) {
		return fmt.Errorf("cannot insert order %d", order.Id)
	}
	return nil
}

func (model *ModelData) handleUnitOrder(m *sword.SimToClient_Content) error {
	mm := m.GetUnitOrder()
	if mm == nil {
		return ErrSkipHandler
	}
	return addOrder(model, mm.GetId(), mm.GetParent(), mm.GetType().GetId(),
		mm.GetTasker().GetId(), UnitOrder)
}

func (model *ModelData) handleAutomatOrder(m *sword.SimToClient_Content) error {
	mm := m.GetAutomatOrder()
	if mm == nil {
		return ErrSkipHandler
	}
	return addOrder(model, mm.GetId(), 0, mm.GetType().GetId(),
		mm.GetTasker().GetId(), AutomatOrder)
}

func (model *ModelData) handleCrowdOrder(m *sword.SimToClient_Content) error {
	mm := m.GetCrowdOrder()
	if mm == nil {
		return ErrSkipHandler
	}
	return addOrder(model, mm.GetId(), 0, mm.GetType().GetId(),
		mm.GetTasker().GetId(), CrowdOrder)
}

func (model *ModelData) handleFragOrder(m *sword.SimToClient_Content) error {
	mm := m.GetFragOrder()
	if mm == nil {
		return ErrSkipHandler
	}
	return addOrder(model, mm.GetId(), 0, mm.GetType().GetId(),
		GetTaskerId(mm.GetTasker()), FragOrder)
}

func (model *ModelData) handleUnitVisionCones(m *sword.SimToClient_Content) error {
	mm := m.GetUnitVisionCones()
	if mm == nil {
		return ErrSkipHandler
	}
	unit, ok := model.Units[mm.GetUnit().GetId()]
	if !ok {
		return fmt.Errorf("cannot update vision cones on unit %d",
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

func (model *ModelData) handleObjectCreation(m *sword.SimToClient_Content) error {
	mm := m.GetObjectCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	object := NewObject(mm.GetObject().GetId(),
		mm.GetParty().GetId(),
		mm.GetType().GetId(),
		mm.GetName(),
	)
	if !model.addObject(object) {
		return fmt.Errorf("cannot insert object %d", object.Id)
	}
	return nil
}

func (model *ModelData) handleObjectUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetObjectUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	object, ok := model.Objects[mm.GetObject().GetId()]
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
		if trafficability := attributes.GetTrafficability(); trafficability != nil {
			object.Trafficability = trafficability.GetValue()
		}
	}
	if mm.Name != nil {
		object.Name = *mm.Name
	}
	return nil
}

func (model *ModelData) handleObjectDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetObjectDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.removeObject(mm.GetObject().GetId()) {
		return fmt.Errorf("cannot destroy object %d",
			mm.GetObject().GetId())
	}
	return nil
}

func (model *ModelData) handleLogMaintenanceHandlingCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogMaintenanceHandlingCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, found := model.MaintenanceHandlings[id]; found {
		return fmt.Errorf("maintenance handling to create already exists %d", id)
	}
	model.MaintenanceHandlings[id] = &MaintenanceHandling{Id: id}
	return nil
}

func (model *ModelData) handleLogMaintenanceHandlingUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogMaintenanceHandlingUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if h, ok := model.MaintenanceHandlings[id]; ok {
		h.Provider = &MaintenanceHandlingProvider{
			Id:      mm.GetProvider().GetId(),
			State:   mm.GetState(),
			EndTick: mm.GetCurrentStateEndTick(),
		}
		return nil
	}
	return fmt.Errorf("cannot update maintenance handling %d", id)
}

func (model *ModelData) handleLogMaintenanceHandlingDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogMaintenanceHandlingDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	size := len(model.MaintenanceHandlings)
	delete(model.MaintenanceHandlings, id)
	if size == len(model.MaintenanceHandlings) {
		return fmt.Errorf("cannot destroy maintenance handling %d", id)
	}
	return nil
}

func (model *ModelData) handleLogMedicalHandlingCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogMedicalHandlingCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, found := model.MedicalHandlings[id]; found {
		return fmt.Errorf("medical handling to create already exists %d", id)
	}
	model.MedicalHandlings[id] = &MedicalHandling{Id: id}
	return nil
}

func (model *ModelData) handleLogMedicalHandlingUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogMedicalHandlingUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if h, ok := model.MedicalHandlings[id]; ok {
		h.Provider = &MedicalHandlingProvider{
			Id:    mm.GetProvider().GetId(),
			State: mm.GetState(),
		}
		return nil
	}
	return fmt.Errorf("cannot update medical handling %d", id)
}

func (model *ModelData) handleLogMedicalHandlingDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogMedicalHandlingDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	size := len(model.MedicalHandlings)
	delete(model.MedicalHandlings, id)
	if size == len(model.MedicalHandlings) {
		return fmt.Errorf("cannot destroy medical handling %d", id)
	}
	return nil
}

func (model *ModelData) handleLogFuneralHandlingCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogFuneralHandlingCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, found := model.FuneralHandlings[id]; found {
		return fmt.Errorf("funeral handling to create already exists %d", id)
	}
	model.FuneralHandlings[id] = &FuneralHandling{
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

func (model *ModelData) handleLogFuneralHandlingUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogFuneralHandlingUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if h, ok := model.FuneralHandlings[id]; ok {
		h.Handler = &FuneralHandlingHandler{
			State: mm.GetState(),
			Id:    GetParentEntityId(mm.GetHandlingUnit()),
		}
		return nil
	}
	return fmt.Errorf("cannot update funeral handling %d", id)
}

func (model *ModelData) handleLogFuneralHandlingDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogFuneralHandlingDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	size := len(model.FuneralHandlings)
	delete(model.FuneralHandlings, id)
	if size == len(model.FuneralHandlings) {
		return fmt.Errorf("cannot destroy funeral handling %d", id)
	}
	return nil
}

func (model *ModelData) handleLogSupplyHandlingCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyHandlingCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if _, found := model.SupplyHandlings[id]; found {
		return fmt.Errorf("supply handling to create already exists %d", id)
	}
	model.SupplyHandlings[id] = &SupplyHandling{
		Id:         mm.GetRequest().GetId(),
		SupplierId: GetParentEntityId(mm.GetSupplier()),
		ProviderId: GetParentEntityId(mm.GetTransportersProvider()),
	}
	return nil
}

func (model *ModelData) handleLogSupplyHandlingUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyHandlingUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if h, ok := model.SupplyHandlings[id]; ok {
		h.Convoy = &SupplyHandlingConvoy{
			ConvoyerId: mm.GetConvoyer().GetId(),
			State:      mm.GetState(),
		}
		return nil
	}
	return fmt.Errorf("cannot update supply handling %d", id)
}

func (model *ModelData) handleLogSupplyHandlingDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyHandlingDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	size := len(model.SupplyHandlings)
	delete(model.SupplyHandlings, id)
	if size == len(model.SupplyHandlings) {
		return fmt.Errorf("cannot destroy supply handling %d", id)
	}
	return nil
}

func (model *ModelData) handleFireEffectCreation(m *sword.SimToClient_Content) error {
	mm := m.GetStartFireEffect()
	if mm == nil {
		return ErrSkipHandler
	}
	points := ReadPoints(mm.GetLocation().GetCoordinates().GetElem())
	effect := &FireEffect{
		Id:   mm.GetFireEffect().GetId(),
		Type: mm.GetType(),
		Location: Location{
			Type:   mm.GetLocation().GetType(),
			Points: points,
		},
	}
	if !model.addFireEffect(effect) {
		return fmt.Errorf("cannot insert fire effect %d", effect.Id)
	}
	return nil
}

func (model *ModelData) handleFireEffectDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetStopFireEffect()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetFireEffect().GetId()
	if !model.removeFireEffect(id) {
		return fmt.Errorf("cannot destroy fire effect %d", id)
	}
	return nil
}

func (model *ModelData) handleActionCreation(m *sword.SimToClient_Content) error {
	mm := m.GetAction()
	if mm == nil {
		return ErrSkipHandler
	}
	action := Action{
		Id:      mm.GetId(),
		ErrCode: mm.GetErrorCode(),
		ErrMsg:  mm.GetErrorMsg(),
	}
	switch {
	case mm.MagicAction != nil:
		action.Kind = MagicAction
	case mm.UnitMagicAction != nil:
		action.Kind = UnitMagicAction
	case mm.ObjectMagicAction != nil:
		action.Kind = ObjectMagicAction
	case mm.KnowledgeMagicAction != nil:
		action.Kind = KnowledgeMagicAction
	case mm.SetAutomatMode != nil:
		action.Kind = SetAutomatMode
	case mm.UnitOrder != nil:
		action.Kind = UnitAction
	case mm.AutomatOrder != nil:
		action.Kind = AutomatAction
	case mm.CrowdOrder != nil:
		action.Kind = CrowdAction
	case mm.FragOrder != nil:
		action.Kind = FragAction
	default:
		return fmt.Errorf("missing action element on action %d", action.Id)
	}
	if !model.addAction(&action) {
		return fmt.Errorf("cannot insert action %d", action.Id)
	}
	return nil
}

func (model *ModelData) handleActionDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetActionDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	if !model.removeAction(mm.GetId()) {
		return fmt.Errorf("cannot destroy action %d", mm.GetId())
	}
	return nil
}

func (model *ModelData) handleFireDetectionCreation(m *sword.SimToClient_Content) error {
	mm := m.GetStartUnitFireDetection()
	if mm == nil {
		return ErrSkipHandler
	}
	units := ReadIds(mm.GetUnits())
	perception := &FireDetection{
		Id:    mm.GetFire().GetId(),
		Units: units,
		Firer: mm.GetFirer().GetId(),
	}
	model.updateFireDetection(perception)
	return nil
}

func (model *ModelData) handleFireDetectionDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetStopUnitFireDetection()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetFire().GetId()
	units := ReadIds(mm.GetUnits())
	if !model.removeFireDetection(id, units) {
		return fmt.Errorf("cannot destroy fire perception %d", id)
	}
	return nil
}

func (model *ModelData) handlePathfindCreation(m *sword.SimToClient_Content) error {
	mm := m.GetPathfindCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId()
	model.Pathfinds[id] = &Pathfind{
		Id:                   id,
		UnitId:               mm.GetRequest().GetUnit().GetId(),
		Positions:            ReadPoints(mm.GetRequest().GetPositions()),
		EquipmentTypes:       ReadIds(mm.GetRequest().GetEquipmentTypes()),
		IgnoreDynamicObjects: mm.GetRequest().GetIgnoreDynamicObjects(),
		Result:               ReadPathPoints(mm.GetResult().GetPoints()),
		Name:                 mm.GetRequest().GetName(),
	}
	return nil
}

func (model *ModelData) handlePathfindDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetPathfindDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId()
	if !model.removePathfind(id) {
		return fmt.Errorf("cannot destroy pathfind %d", id)
	}
	return nil
}

func (model *ModelData) handlePathfindUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetPathfindUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetId()
	if p, ok := model.Pathfinds[id]; ok {
		p.Name = mm.GetRequest().GetName()
		return nil
	}
	return fmt.Errorf("cannot update pathfind %d", id)
}

func (model *ModelData) handleSupplyRequestCreation(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyRequestCreation()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if !model.addSupplyRequest(id) {
		return fmt.Errorf("cannot insert supply request %d", id)
	}
	return nil
}

func (model *ModelData) handleSupplyRequestUpdate(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyRequestUpdate()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if request, ok := model.SupplyRequests[id]; ok {
		request.ResourceId = mm.GetResource().GetId()
		request.RequesterId = mm.GetRequester().GetId()
		request.SupplierId = mm.GetSupplier().GetId()
		request.RecipientId = mm.GetRecipient().GetId()
		request.Requested = mm.GetRequested()
		return nil
	}
	return fmt.Errorf("cannot update supply request %d", id)
}

func (model *ModelData) handleSupplyRequestDestruction(m *sword.SimToClient_Content) error {
	mm := m.GetLogSupplyRequestDestruction()
	if mm == nil {
		return ErrSkipHandler
	}
	id := mm.GetRequest().GetId()
	if !model.removeSupplyRequest(id) {
		return fmt.Errorf("cannot destroy supply request %d", id)
	}
	return nil
}
