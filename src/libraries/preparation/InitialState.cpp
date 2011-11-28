// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InitialState.h"
#include "InitialStateCrew.h"
#include "InitialStateEquipment.h"
#include "InitialStateResource.h"
#include "StaticModel.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/BreakdownOriginType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/tools.h"
#include "tools/Iterator.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: InitialState constructor
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
InitialState::InitialState( const StaticModel& staticModel, unsigned int typeId )
    : staticModel_     ( staticModel )
    , typeId_          ( typeId )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: InitialState constructor
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
InitialState::InitialState( xml::xistream& xis, const StaticModel& staticModel, unsigned int typeId )
    : staticModel_     ( staticModel )
    , typeId_          ( typeId )
{
    Initialize();
    if( xis.has_child( "humans" ) )
    {
        xis >> xml::start( "humans" )
                >> xml::list( "human", *this, &InitialState::ReadCrew )
            >> xml::end;
    }
    if( xis.has_child( "equipments" ) )
    {
        equipments_.clear();
        xis >> xml::start( "equipments" )
                >> xml::list( "equipment", *this, &InitialState::ReadEquipment )
            >> xml::end;
    }
    if( xis.has_child( "resources" ) )
    {
        resources_.clear();
        xis >> xml::start( "resources" )
                >> xml::list( "resource", *this, &InitialState::ReadResource )
            >> xml::end;
        UpdateResourceMaximum();
    }
}

// -----------------------------------------------------------------------------
// Name: InitialState destructor
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
InitialState::~InitialState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialState::ReadEquipment
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
void InitialState::ReadEquipment( xml::xistream& xis )
{
    InitialStateEquipment equipment = InitialStateEquipment( xis );
    for( CIT_Equipments it = originalEquipments_.begin(); it != originalEquipments_.end(); ++it )
        if( it->name_ == equipment.name_ )
        {
            equipment.breakdowns_ = it->breakdowns_;
            break;
        }
    if( equipment.state_ == eEquipmentState_RepairableWithEvacuation )
    {
        const std::string breakdown = xis.attribute( "breakdown", "" );
        equipment.currentBreakdown_ = std::max( 0, equipment.breakdowns_.findIndex( breakdown.c_str() ) );
    }
    equipments_.push_back( equipment );
}

// -----------------------------------------------------------------------------
// Name: InitialState::ReadCrew
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
void InitialState::ReadCrew( xml::xistream& xis )
{
    InitialStateCrew crew = InitialStateCrew( xis );
    crews_[ crew.rank_ ].number_ -= crew.number_;
    crews_.push_back( crew );
}

// -----------------------------------------------------------------------------
// Name: InitialState::ReadResource
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
void InitialState::ReadResource( xml::xistream& xis )
{
    InitialStateResource resource = InitialStateResource( xis );
    resource.category_ = RetrieveResourceCategory( resource.name_ );
    resources_.push_back( resource );
}

// -----------------------------------------------------------------------------
// Name: InitialState::SerializeAttributes
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
void InitialState::SerializeAttributes( xml::xostream& xos ) const
{
    if( IsEquipmentsSaveNeeded() )
    {
        xos.start( "equipments" );
        for( CIT_Equipments it = equipments_.begin(); it != equipments_.end(); ++it )
            it->Serialize( xos );
        xos.end();
    }
    if( IsCrewsSaveNeeded() )
    {
        assert( crews_.size() > 3 );
        xos.start( "humans" );
        for( CIT_Crews it = crews_.begin() + 3; it != crews_.end(); ++it )
            it->Serialize( xos );
        xos.end();
    }
    if( IsResourcesSaveNeeded() )
    {
        xos.start( "resources" );
        for( CIT_Resources it = resources_.begin(); it != resources_.end(); ++it )
            it->Serialize( xos );
        xos.end();
    }
}

// -----------------------------------------------------------------------------
// Name: InitialState::IsEquipmentsSaveNeeded
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialState::IsEquipmentsSaveNeeded() const
{
    for( unsigned int i = 0; i < equipments_.size(); ++i )
        if( originalEquipments_[ i ] != equipments_[ i ] )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: InitialState::IsCrewsSaveNeeded
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialState::IsCrewsSaveNeeded() const
{
    return crews_.size() > 3;
}

// -----------------------------------------------------------------------------
// Name: InitialState::IsResourcesSaveNeeded
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialState::IsResourcesSaveNeeded() const
{
    if( originalResources_.size() != resources_.size() )
        return true;
    for( unsigned int i = 0; i < resources_.size(); ++i )
        if( originalResources_[ i ] != resources_[ i ] )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: InitialState::Reset
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialState::Reset()
{
    equipments_.clear();
    crews_.clear();
    resources_.clear();
    equipments_ = originalEquipments_;
    crews_      = originalCrews_;
    resources_  = originalResources_;
}

// -----------------------------------------------------------------------------
// Name: InitialState::Initialize
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialState::Initialize()
{
    kernel::AgentType& agent = staticModel_.types_.tools::Resolver< kernel::AgentType >::Get( typeId_ );
    FillResources( agent.CreateResourcesIterator() );
    tools::Iterator< const kernel::AgentComposition& > agentCompositionIterator = agent.CreateIterator();
    int nbrTotalOfficers = 0;
    while( agentCompositionIterator.HasMoreElements() )
    {
        const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
        const std::string& agentName = agentComposition.GetType().GetName();
        const kernel::EquipmentType& equipmentType = staticModel_.objectTypes_.tools::Resolver< kernel::EquipmentType >::Get( agentComposition.GetType().GetId() );
        tools::Iterator< const kernel::BreakdownOriginType& > breakdownIterator = equipmentType.CreateBreakdownsIterator();
        QStringList breakdowns;
        while( breakdownIterator.HasMoreElements() )
        {
            const kernel::BreakdownOriginType& breakdown = breakdownIterator.NextElement();
            if( breakdowns.find( breakdown.GetName().c_str() ) == breakdowns.end() )
                breakdowns << breakdown.GetName().c_str();
        }
        FillResources( equipmentType.CreateResourcesIterator(), agentComposition.GetCount() );
        for( unsigned int i = 0; i < agentComposition.GetCount(); ++i )
            originalEquipments_.push_back( InitialStateEquipment( agentName.c_str(), eEquipmentState_Available, breakdowns ) );
        nbrTotalOfficers += agentComposition.GetCount() * agentComposition.GetCrew();
    }
    originalCrews_.push_back( InitialStateCrew( eHumanRank_Officier,    eHumanState_Healthy, eInjuriesSeriousness_U1, false, false, agent.GetNbrOfficers() ) );
    originalCrews_.push_back( InitialStateCrew( eHumanRank_SousOfficer, eHumanState_Healthy, eInjuriesSeriousness_U1, false, false, agent.GetNbrWarrantOfficers() ) );
    originalCrews_.push_back( InitialStateCrew( eHumanRank_Mdr,         eHumanState_Healthy, eInjuriesSeriousness_U1, false, false, nbrTotalOfficers - agent.GetNbrOfficers() - agent.GetNbrWarrantOfficers() ) );
    Reset();
}

// -----------------------------------------------------------------------------
// Name: InitialState::FillResources
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
void InitialState::FillResources( tools::Iterator< const kernel::DotationCapacityType& > resourcesIterator, unsigned factor /*= 1*/ )
{
    while( resourcesIterator.HasMoreElements() )
    {
        const kernel::DotationCapacityType& dotation = resourcesIterator.NextElement();
        unsigned int i = 0;
        for( i = 0; i < originalResources_.size(); ++i )
            if( originalResources_[ i ].name_ == dotation.GetName().c_str() )
            {
                originalResources_[ i ].number_ += dotation.GetCapacity() * factor;
                originalResources_[ i ].maximum_ += dotation.GetCapacity() * factor;
                originalResources_[ i ].consumption_ += dotation.GetNormalizedConsumption() * factor;
                break;
            }
        if( i == originalResources_.size() )
            originalResources_.push_back( InitialStateResource( dotation.GetName().c_str(), RetrieveResourceCategory( dotation.GetName().c_str() ), dotation.GetCapacity() * factor, dotation.GetCapacity() * factor, dotation.GetLogisticThreshold(), dotation.GetNormalizedConsumption() * factor ) );
    }
}

// -----------------------------------------------------------------------------
// Name: InitialState::RetrieveResourceCategory
// Created: ABR 2011-03-04
// -----------------------------------------------------------------------------
const QString InitialState::RetrieveResourceCategory( const QString& resourceName ) const
{
    const kernel::DotationType& category = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Get( resourceName.ascii() );
    return category.GetCategoryName().c_str();
}

// -----------------------------------------------------------------------------
// Name: InitialState::UpdateResourceMaximum
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void InitialState::UpdateResourceMaximum()
{
    for( unsigned int i = 0; i < resources_.size(); ++i )
        for( unsigned int j = 0; j < originalResources_.size(); ++j )
            if( resources_[ i ].name_ == originalResources_[ j ].name_ )
            {
                resources_[ i ].maximum_ = originalResources_[ j ].maximum_;
                break;
            }
}
