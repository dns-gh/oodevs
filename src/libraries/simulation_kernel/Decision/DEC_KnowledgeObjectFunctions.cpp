// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects_DataComputer.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/AnimatorAttribute.h"
#include "Entities/Objects/CapacityRetriever.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/ContaminationCapacity.h"
#include "Entities/Objects/DecontaminationCapacity.h"
#include "Entities/Objects/IntoxicationCapacity.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/Objects/BurnAttribute.h"
#include "Entities/Objects/FireAttribute.h"
#include "Entities/Objects/ImprovableCapacity.h"
#include "Entities/Objects/PopulationAttribute.h"
#include "Entities/Objects/SupplyRouteAttribute.h"
#include "Entities/Objects/StockAttribute.h"
#include "Entities/Objects/TrafficabilityAttribute.h"
#include "Entities/Objects/UndergroundAttribute.h"
#include "Entities/Objects/UndergroundCapacity.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/CrossingSiteAttribute.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/QueryValidity.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::Recon
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::Recon( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        pKnowledge->Recon( callerAgent );
}

namespace
{
    MIL_Object_ABC* GetObjectKnown( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
    {
        if( pKnowledge && pKnowledge->IsValid() )
            return pKnowledge->GetObjectKnown();
        return 0;
    }

    template< typename Capacity >
    Capacity* IsValidObjectCapacity( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
    {
        if( MIL_Object_ABC* pObject = GetObjectKnown( pKnowledge ) )
            return pObject->Retrieve< Capacity >();
        return 0;
    }

    template< typename Attribute >
    Attribute* IsValidObjectAttribute( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
    {
        if( MIL_Object_ABC* pObject = GetObjectKnown( pKnowledge ) )
            return pObject->RetrieveAttribute< Attribute >();
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::QueueForDecontamination
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::QueueForDecontamination( MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( DecontaminationCapacity* pCapacity = IsValidObjectCapacity< DecontaminationCapacity >( pKnowledge ) )
    {
        pCapacity->QueueForDecontamination( callerAgent );
        return static_cast< int >( eQueryValid );
    }
    return static_cast< int >( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::QueueUnitForDecontamination
// Created: LGY 2013-01-03
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::QueueUnitForDecontamination( DEC_Decision_ABC* agent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( agent )
        return QueueForDecontamination( agent->GetPion(), pKnowledge );
    return static_cast< int >( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::QueueKnowledgeForDecontamination
// Created: LGY 2012-12-20
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::QueueKnowledgeForDecontamination( boost::shared_ptr< DEC_Knowledge_Agent > pAgent, boost::shared_ptr< DEC_Knowledge_Object > pObject )
{
    if( pAgent && pAgent->IsValid() )
        return QueueForDecontamination( pAgent->GetAgentKnown(), pObject );
    return static_cast< int >( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::PopulationQueueForDecontamination
// Created: LGY 2011-11-18
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::PopulationQueueForDecontamination( MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( DecontaminationCapacity* pCapacity = IsValidObjectCapacity< DecontaminationCapacity >( pKnowledge ) )
    {
        pCapacity->QueueForDecontamination( callerPopulation );
        return static_cast< int >( eQueryValid );
    }
    return static_cast< int >( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeAnimated
// Created: NLD 2004-11-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeAnimated( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( const MIL_Object_ABC* object = GetObjectKnown( pKnowledge ) )
        return ( *object )().CanBeAnimatedBy( callerAgent );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetAnimationLevel
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetAnimationLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( const AnimatorAttribute* attr = IsValidObjectAttribute< AnimatorAttribute >( pKnowledge ) )
        return static_cast< float >( attr->GetAnimatorsRatio() );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DecontaminateZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::DecontaminateZone( const MIL_Agent_ABC& callerAgent, const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_ObjectFilter filter;
    filter.Set( "nbc zone" );
    filter.Set( "nbc cloud" );
    T_KnowledgeObjectVector knownObjects;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjects( knownObjects, filter );
    for( auto it = knownObjects.begin(); it != knownObjects.end(); ++it )
        if( *it && location->IsIntersecting( ( *it )->GetLocalisation() ) )
        {
            if( ContaminationCapacity* pContaminationCapacity = IsValidObjectCapacity< ContaminationCapacity >( *it ) )
                pContaminationCapacity->DecontaminateZone( *(*it)->GetObjectKnown(), *location );
            if( IntoxicationCapacity* pIntoxicationCapacity = IsValidObjectCapacity< IntoxicationCapacity >( *it ) )
                pIntoxicationCapacity->DesintoxicateZone( *(*it)->GetObjectKnown(), *location );
        }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DamageObject
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::DamageObject( MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float factor, const PHY_DotationCategory* dotation )
{
    if( MIL_Object_ABC* pObject = GetObjectKnown( pKnowledge ) )
        if( ( *pObject )().CanBePerceived() )
        {
            ( *pObject )().Destroy( factor );
            callerAgent.Get< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( *dotation, 1 );
            return static_cast< int >( eQueryValid );
        }
    return static_cast< int >( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeOccupied
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeOccupied( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( const MIL_Object_ABC* pObject = GetObjectKnown( pKnowledge ) )
        return ( *pObject )().CanBeOccupiedBy( callerAgent );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeBypassed
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeBypassed( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( const MIL_Object_ABC* pObject = GetObjectKnown( pKnowledge ) )
        return ( *pObject )().CanBeBypassed();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EquipLogisticRoute
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::EquipLogisticRoute( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( SupplyRouteAttribute* pAttribute = IsValidObjectAttribute< SupplyRouteAttribute >( pKnowledge ) )
    {
        pAttribute->Equip();
        return static_cast< int >( eQueryValid );
    }
    return static_cast< int >( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float density )
{
    if( PopulationAttribute* pAttribute = IsValidObjectAttribute< PopulationAttribute >( pKnowledge ) )
        pAttribute->SetDensity( density );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( PopulationAttribute* pAttribute = IsValidObjectAttribute< PopulationAttribute >( pKnowledge ) )
        pAttribute->Reset();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsStockSupplied
// Created: JCR 2009-06-03
// Created: RPD 2009-11-06
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsStockSupplied( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( StockAttribute* pAttribute = IsValidObjectAttribute< StockAttribute >( pKnowledge ) )
        return pAttribute->IsFull();
    return false;
}

// -----------------------------------------------------------------------------
// Name: static void DEC_KnowledgeObjectFunctions::IsKnowledgeValid
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsKnowledgeValid( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsObstacleActivated
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsObstacleActivated( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->IsObstacleActivated();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsActivableObstacle
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsActivableObstacle( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->IsActivableObstacle();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetActivationTime
// Created: GGE 2012-09-19
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::GetActivationTime( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->GetActivationTime();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsBypassed
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsBypassed( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->IsBypassed() ? eTristate_True : eTristate_False );
    return static_cast< int >( eTristate_DontKnow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsMined
// Created: DDA 2012-03-16
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsMined( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->IsMined() ? eTristate_True : eTristate_False );
    return static_cast< int >( eTristate_DontKnow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsFullMined
// Created: MMC 2013-06-14
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsFullMined( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->IsFullMined() ? eTristate_True : eTristate_False );
    return static_cast< int >( eTristate_DontKnow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::MustBeMined
// Created: MMC 2013-06-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::MustBeMined( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->MustBeMined();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsConstructed
// Created: MIA 2012-03-13
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsConstructed( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->IsConstructed() ? eTristate_True : eTristate_False );
    return static_cast< int >( eTristate_DontKnow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsBreached
// Created: LDC 2011-08-05
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsBreached( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return false; // $$$$ LDC: TODO depending on how we re-code breach. Current implementation (scipio v1.11) doesn't work very well.
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::GetLocalisation
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const TER_Localisation* DEC_KnowledgeObjectFunctions::GetLocalisation( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return &pKnowledge->GetLocalisation();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::IsInAvoidanceArea
// Created: MGD 2010-09-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsInAvoidanceArea( boost::shared_ptr< MT_Vector2D > point, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->GetAvoidanceLocalisation().IsInside( *point );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_KnowledgeObjectFunctions::IsNearEffectArea
// Created: LMT 2012-01-23
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsNearEffectArea( boost::shared_ptr< MT_Vector2D > point, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, double distance )
{
    if( !pKnowledge )
        return false;
    TER_Localisation location = pKnowledge->GetLocalisation();
    MT_Vector2D result;
    location.ComputeNearestPoint( *point, result );
    return result.SquareDistance( *point ) < distance * distance;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetType
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
std::string DEC_KnowledgeObjectFunctions::GetType( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->GetType().GetName();
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetName
// Created: JCR 2010-07-07
// -----------------------------------------------------------------------------
std::string DEC_KnowledgeObjectFunctions::GetName( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->GetName();
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth
// Created: NLD 2004-05-13
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( const CrossingSiteAttribute* attribute = IsValidObjectAttribute< CrossingSiteAttribute >( pKnowledge ) )
        return static_cast< float >( attribute->GetWidth() );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsRecon( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->IsRecon();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsAnEnemy
// Created: MGD 2010-01-26
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsAnEnemy( const MIL_Entity_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->IsAnEnemy( callerAgent.GetArmy() ) );
    return static_cast< int >( eTristate_DontKnow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsAFriend
// Created: MGD 2010-01-26
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsAFriend( const MIL_Entity_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->IsAFriend( callerAgent.GetArmy() ) );
    return static_cast< int >( eTristate_DontKnow );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-01-27
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::GetCurrentPerceptionLevel( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return static_cast< int >( pKnowledge->GetCurrentPerceptionLevel( callerAgent ).GetID() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetConstructionLevel
// Created: MGD 2010-04-02
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetConstructionLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( MIL_Object_ABC* object = GetObjectKnown( pKnowledge ) )
        if( const ConstructionAttribute* attr = object->RetrieveAttribute< ConstructionAttribute >() )
            return static_cast< float >( attr->GetState() );
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetBurningLevel
// Created: BCI 2011-01-25
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetBurningLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge)
{
    const BurnAttribute* burnAttribute = IsValidObjectAttribute< BurnAttribute >( pKnowledge );
    const FireAttribute* fireAttribute = IsValidObjectAttribute< FireAttribute >( pKnowledge );
    if( burnAttribute && fireAttribute )
        if( int maxHeat = fireAttribute->GetMaxHeat() )
            return static_cast< float >( burnAttribute->GetCurrentHeat() ) / maxHeat;
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetValorizationLevel
// Created: MGD 2010-04-02
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetValorizationLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( MIL_Object_ABC* object = GetObjectKnown( pKnowledge ) )
    {
        const MineAttribute* attr = object->RetrieveAttribute< MineAttribute >();
        if( attr )
            return static_cast< float >( attr->GetState() );
        return 0.f;
    }
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EstimatedWorkTime
// Created: GGE & PSN 2010-04-09
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::EstimatedWorkTime( MIL_Agent_ABC& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( MIL_Object_ABC* object = GetObjectKnown( pKnowledge ) )
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( pion, eDestroy, *object );
        return static_cast< float >( dataComputer.ComputeWorkTime() );
    }
    return -1.0f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeValorized
// Created: LMT 2011-04-05
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeValorized( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return IsValidObjectCapacity< ImprovableCapacity >( pKnowledge ) != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::HasCapacity
// Created: JSR 2012-04-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::HasCapacity( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::string& capacity )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return CapacityRetriever::RetrieveCapacity( pKnowledge->GetType(), capacity ) != 0;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::BuildInstantaneously
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::BuildInstantaneously( const DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return;
    if( MIL_Object_ABC* obj = pKnowledge->GetObjectKnown() )
    {
        callerAgent.GetPion().GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *obj );
        ( *obj )().Construct();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsUndergroundNetworkExit
// Created: JSR 2011-06-06
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsUndergroundNetworkExit( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return IsValidObjectCapacity< UndergroundCapacity >( pKnowledge ) != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::ActivateUndergroundNetworkExit
// Created: JSR 2011-07-19
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::ActivateUndergroundNetworkExit( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    UndergroundAttribute* pAttribute = IsValidObjectAttribute< UndergroundAttribute >( pKnowledge );
    if( pAttribute && !pAttribute->IsActivated() )
    {
        pAttribute->SetActivation( true );
        return static_cast< int >( eQueryValid );
    }
    return static_cast< int >( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DeactivateUndergroundNetworkExit
// Created: JSR 2011-07-19
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::DeactivateUndergroundNetworkExit( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    UndergroundAttribute* pAttribute = IsValidObjectAttribute< UndergroundAttribute >( pKnowledge );
    if( pAttribute && pAttribute->IsActivated() )
    {
        pAttribute->SetActivation( false );
        return static_cast< int >( eQueryValid );
    }
    return static_cast< int >( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EstimatedUndergroundTime
// Created: JSR 2011-06-06
// -----------------------------------------------------------------------------
double DEC_KnowledgeObjectFunctions::EstimatedUndergroundTime( const DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( IsValidObjectCapacity< UndergroundCapacity >( pKnowledge ) )
        return callerAgent.GetPion().Get< PHY_RoleAction_MovingUnderground >().EstimatedUndergroundTime( pKnowledge );
    return -1.0f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::HideInUndergroundNetwork
// Created: JSR 2011-06-06
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::HideInUndergroundNetwork( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    bool ret = callerAgent.GetPion().Get< PHY_RoleAction_MovingUnderground >().HideInUndergroundNetwork( pKnowledge );
    return static_cast< int >( ret ? eQueryValid : eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetOutFromUndergroundNetwork
// Created: JSR 2011-06-06
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::GetOutFromUndergroundNetwork( DEC_Decision_ABC& callerAgent )
{
    PHY_RoleAction_MovingUnderground& role = callerAgent.GetPion().Get< PHY_RoleAction_MovingUnderground >();
    if( !role.CanExitFromCurrentLocation() )
        return static_cast< int >( eQueryInvalid );
    role.GetOutFromUndergroundNetwork();
    return static_cast< int >( eQueryValid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EnterAndExitInSameUndergroundNetwork
// Created: GGE 2012-03-27
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::EnterAndExitInSameUndergroundNetwork( boost::shared_ptr< DEC_Knowledge_Object > pEnter, boost::shared_ptr< DEC_Knowledge_Object > pExit )
{
    UndergroundAttribute* pEnterAttribute = IsValidObjectAttribute< UndergroundAttribute >( pEnter );
    UndergroundAttribute* pExitAttribute = IsValidObjectAttribute< UndergroundAttribute >( pExit );
    if ( pEnterAttribute && pExitAttribute )
    {
        const std::string EnterUnderGroundNetwork = pEnterAttribute->Network();
        const std::string ExitUnderGroundNetwork = pExitAttribute->Network();
        if ( EnterUnderGroundNetwork == ExitUnderGroundNetwork )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsObjectTrafficable
// Created: NMI 2014-01-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsObjectTrafficable( boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge, double weight )
{
    if( const TrafficabilityAttribute* pAttribute = IsValidObjectAttribute< TrafficabilityAttribute >( objectKnowledge ) )
        return pAttribute->GetMaxValue() >= weight;
    return true;
}