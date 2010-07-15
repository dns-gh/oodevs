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
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Objects/Object.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/AnimatorAttribute.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/ContaminationCapacity.h"
#include "Entities/Objects/DecontaminationCapacity.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/Objects/PopulationAttribute.h"
#include "Entities/Objects/SupplyRouteAttribute.h"
#include "Entities/Objects/StockAttribute.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects_DataComputer.h"

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
    template< typename Capacity >
    Capacity* IsValidObjectCapacity( const boost::shared_ptr< DEC_Knowledge_Object >& pKnowledge )
    {
        if( !pKnowledge || !pKnowledge->IsValid() )
            return 0;

        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if( pObject )
            return pObject->Retrieve< Capacity >();
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
        return int( eQueryValid );
    }
    return int( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeAnimated
// Created: NLD 2004-11-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeAnimated( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( const MIL_Object_ABC* object = pKnowledge->GetObjectKnown() )
            return (*object)().CanBeAnimatedBy( callerAgent );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetAnimationLevel
// Created: MGD 2010-02-15
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetAnimationLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge)
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        if( MIL_Object_ABC* object = pKnowledge->GetObjectKnown() )
        {
            const AnimatorAttribute* capacity = object->RetrieveAttribute< AnimatorAttribute >();
            if( capacity )
                return float(capacity->GetCurrent()) / float(capacity->GetMaxAnimators());
        }
    }
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DecontaminateZone
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::DecontaminateZone( const MIL_Agent_ABC& callerAgent, const TER_Localisation* location )
{
    assert( location );
    MIL_ObjectFilter filter;
    filter.Set( "nbc zone" );
    filter.Set( "nbc cloud" );
    T_KnowledgeObjectVector knownObjects;
    callerAgent.GetArmy().GetKnowledge().GetObjects( knownObjects, filter );
    for( CIT_KnowledgeObjectVector it = knownObjects.begin(); it != knownObjects.end(); ++it )
        if( location->IsIntersecting( (*it)->GetLocalisation() ) )
            if( ContaminationCapacity* pCapacity = IsValidObjectCapacity< ContaminationCapacity >( *it ) )
                pCapacity->DecontaminateZone( *location );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::DamageObject
// Created: SBO 2006-01-23
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::DamageObject( MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float factor, const PHY_DotationCategory* dotation )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            if( (*pObject)().CanBePerceived() )
            {
                (*pObject)().Destroy( factor );
                callerAgent.Get< dotation::PHY_RoleInterface_Dotations >().AddFireReservation( *dotation, 1 );
                return int( eQueryValid );
            }
    return int( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeOccupied
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeOccupied( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( const MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            return (*pObject)().CanBeOccupiedBy( callerAgent );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::CanBeBypassed
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::CanBeBypassed( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( const MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            return (*pObject)().CanBeBypassed();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EquipLogisticRoute
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::EquipLogisticRoute( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            if( SupplyRouteAttribute* pAttribute = pObject->RetrieveAttribute< SupplyRouteAttribute >() )
            {
                pAttribute->Equip();
                return int( eQueryValid );
            }
    return int( eQueryInvalid );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float density )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            if( PopulationAttribute* pAttribute = pObject->RetrieveAttribute< PopulationAttribute >() )
                pAttribute->SetDensity( density );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity
// Created: NLD 2006-03-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown() )
            if( PopulationAttribute* pAttribute = pObject->RetrieveAttribute< PopulationAttribute >() )
                pAttribute->Reset();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsStockSupplied
// Created: JCR 2009-06-03
// Created: RPD 2009-11-06
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsStockSupplied( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge )
    {
        MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
        if( pObject )
        {
            StockAttribute* pAttribute = pObject->RetrieveAttribute< StockAttribute >();
            if( pAttribute )
                return pAttribute->IsFull();
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: static void DEC_KnowledgeObjectFunctions::IsKnowledgeValid
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsKnowledgeValid( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return( pKnowledge && pKnowledge->IsValid() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->IsReservedObstacleActivated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsReservedObstacle
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
bool DEC_KnowledgeObjectFunctions::IsReservedObstacle( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    return pKnowledge && pKnowledge->IsValid() && pKnowledge->IsReservedObstacle();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsBypassed
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsBypassed( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return int( pKnowledge->IsBypassed() ? eTristate_True : eTristate_False );
    return int( eTristate_DontKnow );
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
// Name: DEC_KnowledgeObjectFunctions::GetType
// Created: NLD 2004-04-09
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
std::string DEC_KnowledgeObjectFunctions::GetType( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge->GetType().GetName();
    return "";
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth
// Created: NLD 2004-05-13
// Modified: JVT 2004-12-17
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( const DEC_Knowledge_ObjectAttributeCrossingSite* attribute = pKnowledge->Retrieve< DEC_Knowledge_ObjectAttributeCrossingSite >() )
            return float( attribute->GetWidth() );
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
int DEC_KnowledgeObjectFunctions::IsAnEnemy( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return int( eTristate_DontKnow );
    return int( pKnowledge->IsAnEnemy( callerAgent.GetArmy() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::IsAFriend
// Created: MGD 2010-01-26
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::IsAFriend( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return int( eTristate_DontKnow );
    return int( pKnowledge->IsAFriend( callerAgent.GetArmy() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-01-27
// -----------------------------------------------------------------------------
int DEC_KnowledgeObjectFunctions::GetCurrentPerceptionLevel( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
        return (int)pKnowledge->GetCurrentPerceptionLevel( callerAgent ).GetID();
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetConstructionLevel
// Created: MGD 2010-04-02
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetConstructionLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        if( MIL_Object_ABC* object = pKnowledge->GetObjectKnown() )
        {
            const ConstructionAttribute* capacity = object->RetrieveAttribute< ConstructionAttribute >();
            if( capacity )
                return float(capacity->GetState());
            return 1.f;
        }
    }
    return 0.f;
}
// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::GetValorizationLevel
// Created: MGD 2010-04-02
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::GetValorizationLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        if( MIL_Object_ABC* object = pKnowledge->GetObjectKnown() )
        {
            const MineAttribute* capacity = object->RetrieveAttribute< MineAttribute >();
            if( capacity )
                return float(capacity->GetState());
            return 1.f;
        }
    }
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeObjectFunctions::EstimatedWorkTime
// Created: GGE & PSN 2010-04-09
// -----------------------------------------------------------------------------
float DEC_KnowledgeObjectFunctions::EstimatedWorkTime( MIL_Agent_ABC& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        if( MIL_Object_ABC* object = pKnowledge->GetObjectKnown() )
        {
            PHY_RoleAction_Objects_DataComputer dataComputer( pion, PHY_RoleAction_Objects_DataComputerPionData::eDestroy, *object );
            return float( dataComputer.ComputeWorkTime() );
        }
    return -1.0f;
}
