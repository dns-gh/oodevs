// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 15:04 $
// $Revision: 12 $
// $Workfile: PHY_RoleAction_Objects.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KS_AgentQuerier.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "PHY_RoleAction_Objects_DataComputer.h"
#include "PHY_RoleAction_Objects_CapabilityComputer.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( PHY_RoleAction_Objects, "PHY_RoleAction_Objects" )

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects constructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects::PHY_RoleAction_Objects( MT_RoleContainer& role, MIL_AgentPion& pion )
    : MT_Role_ABC( role )
    , pPion_     ( &pion )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects::PHY_RoleAction_Objects()
    : MT_Role_ABC()
    , pPion_     ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects destructor
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects::~PHY_RoleAction_Objects()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Objects::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this )
         & pPion_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetRealObject
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC* PHY_RoleAction_Objects::GetRealObject( uint nKnowledgeObjectID )
{
    assert( pPion_ );
    
    DEC_Knowledge_Object* pKnowledge = pPion_->GetKSQuerier().GetKnowledgeObjectFromDiaID( nKnowledgeObjectID );
    if( !pKnowledge )
        return 0;

    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    if( pObject )
        return pObject;

    pPion_->GetArmy().GetKSObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ComputeDataWithReinforcement
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Objects::ComputeDataWithReinforcement( PHY_RoleAction_Objects_DataComputer& dataComputer ) const
{
    assert( pPion_ );
    
    dataComputer.SetActivePion( *pPion_ );
    GetRole< PHY_RolePion_Composantes >().Apply( dataComputer );

    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        (**itReinforcement).GetRole< PHY_RoleAction_Objects >().ComputeDataWithReinforcement( dataComputer );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ComputeCapabilityWithReinforcement
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
inline
void PHY_RoleAction_Objects::ComputeCapabilityWithReinforcement( PHY_RoleAction_Objects_CapabilityComputer& capabilityComputer ) const
{
    GetRole< PHY_RolePion_Composantes >().Apply( capabilityComputer );
    if( capabilityComputer.HasCapability() )
        return;

    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
    {
        (**itReinforcement).GetRole< PHY_RoleAction_Objects >().ComputeCapabilityWithReinforcement( capabilityComputer );
        if( capabilityComputer.HasCapability() )
            return;
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Construct
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Construct( MIL_RealObject_ABC& object )
{
    if( !object.CanBeConstructed() )
        return eImpossible;

    if( object.GetConstructionPercentage() == 1. )
        return eFinished;

    PHY_RoleAction_Objects_DataComputer dataComputer( PHY_RoleAction_Objects_DataComputer::eConstruct, object );
    ComputeDataWithReinforcement( dataComputer );
    
    dataComputer.ReserveConsumptions();

    const MT_Float rDeltaPercentage = dataComputer.GetDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
    {
        dataComputer.RollbackConsumptionsReservations();
        return eNoCapacity;
    }

    const uint                  nDotationNeeded   = object.GetDotationNeededForConstruction( rDeltaPercentage );
    const PHY_DotationCategory* pDotationCategory = object.GetType().GetDotationCategoryForConstruction();
    if( pDotationCategory && !dataComputer.HasDotations( nDotationNeeded, *pDotationCategory ) )
    {
        dataComputer.RollbackConsumptionsReservations();
        return eNoMoreDotation;
    }

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( object );

    object.Construct( rDeltaPercentage );
    if( pDotationCategory )
        dataComputer.ConsumeDotations( nDotationNeeded, *pDotationCategory );

    if( object.GetConstructionPercentage() == 1. )
        return eFinished;

    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Construct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Construct( MIL_RealObject_ABC* pObject, DEC_Knowledge_Object*& pKnowledge )
{
    if( !pObject )
    {
        pKnowledge = 0;
        return eImpossible;
    }

    assert( pPion_ );
    pKnowledge = pPion_->GetKSQuerier().GetKnowledgeObject( *pObject );
    return Construct( *pObject );
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Prepare
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Prepare( MIL_RealObject_ABC& object )
{
    if( !object.CanBePrepared() )
        return eImpossible;

    if( object.GetConstructionPercentage() == 1. )
        return eFinished;

    PHY_RoleAction_Objects_DataComputer dataComputer( PHY_RoleAction_Objects_DataComputer::ePrepare, object );
    ComputeDataWithReinforcement( dataComputer );
    
    dataComputer.ReserveConsumptions();

    const MT_Float rDeltaPercentage = dataComputer.GetDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
    {
        dataComputer.RollbackConsumptionsReservations();
        return eNoCapacity;
    }

    const uint                   nDotationNeeded  = object.GetDotationNeededForConstruction( rDeltaPercentage );
    const PHY_DotationCategory* pDotationCategory = object.GetType().GetDotationCategoryForConstruction();
    if( pDotationCategory && !dataComputer.HasDotations( nDotationNeeded, *pDotationCategory ) )
    {
        dataComputer.RollbackConsumptionsReservations();
        return eNoMoreDotation;
    }

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( object );

    object.Prepare( rDeltaPercentage );
    if( pDotationCategory )
        dataComputer.ConsumeDotations( nDotationNeeded, *pDotationCategory );

    if( object.GetConstructionPercentage() == 1. )
        return eFinished;

    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Prepare
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Prepare( MIL_RealObject_ABC* pObject, DEC_Knowledge_Object*& pKnowledge )
{
    if( !pObject )
    {
        pKnowledge = 0;
        return eImpossible;
    }

    assert( pPion_ );
    pKnowledge = pPion_->GetKSQuerier().GetKnowledgeObject( *pObject );
    return Prepare( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Destroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Destroy( uint nKnowledgeObjectID )
{
    assert( pPion_ );
    
    DEC_Knowledge_Object* pKnowledge = pPion_->GetKSQuerier().GetKnowledgeObjectFromDiaID( nKnowledgeObjectID );
    if( !pKnowledge )
        return eImpossible;
    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject )
    {
        pPion_->GetArmy().GetKSObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
        return eImpossible;
    }

    if( !pObject->CanBeDestroyed() )
        return eImpossible;

    if( pObject->IsMined() )
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( PHY_RoleAction_Objects_DataComputer::eDemine, *pObject );
        ComputeDataWithReinforcement( dataComputer );
        dataComputer.ReserveConsumptions();
   
        const MT_Float rDeltaPercentage = dataComputer.GetDeltaPercentage();
        if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        {
            dataComputer.RollbackConsumptionsReservations();
            return eNoCapacity;
        }

        pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
        pObject->Demine( rDeltaPercentage );
        return eRunning;
    }
    else
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( PHY_RoleAction_Objects_DataComputer::eDestroy, *pObject );
        ComputeDataWithReinforcement( dataComputer );
        dataComputer.ReserveConsumptions();
   
        const MT_Float rDeltaPercentage = dataComputer.GetDeltaPercentage();
        if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        {
            dataComputer.RollbackConsumptionsReservations();
            return eNoCapacity;
        }

        const uint                  nDotationRecovered = pObject->GetDotationRecoveredWhenDestroying( rDeltaPercentage );
        const PHY_DotationCategory* pDotationCategory  = pObject->GetType().GetDotationCategoryForConstruction();

        pObject->Destroy( rDeltaPercentage );

        if( pDotationCategory && pPion_->GetArmy() == pObject->GetArmy() )
            dataComputer.RecoverDotations( nDotationRecovered, *pDotationCategory );

        if( pObject->GetConstructionPercentage() == 0. )
        {
            pPion_->GetArmy().GetKSObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
            return eFinished;
        }
        pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
        return eRunning;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Mine( MIL_RealObject_ABC& object )
{
    if( !object.CanBeMined() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( PHY_RoleAction_Objects_DataComputer::eMine, object );
    ComputeDataWithReinforcement( dataComputer );
    dataComputer.ReserveConsumptions();

    const MT_Float rDeltaPercentage = dataComputer.GetDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
    {
        dataComputer.RollbackConsumptionsReservations();
        return eNoCapacity;
    }

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( object );
    object.Mine( rDeltaPercentage );
    if( object.GetMiningPercentage() == 1. )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Mine( uint nKnowledgeObjectID )
{
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    return Mine( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Demine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Demine( uint nKnowledgeObjectID )
{
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    if( !pObject->CanBeMined() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( PHY_RoleAction_Objects_DataComputer::eDemine, *pObject );
    ComputeDataWithReinforcement( dataComputer );
    dataComputer.ReserveConsumptions();

    const MT_Float rDeltaPercentage = dataComputer.GetDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
    {
        dataComputer.RollbackConsumptionsReservations();
        return eNoCapacity;
    }

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
    pObject->Demine( rDeltaPercentage );
    if( pObject->GetMiningPercentage() == 0. )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Bypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Bypass( uint nKnowledgeObjectID )
{
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    if( !pObject->CanBeBypassed() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( PHY_RoleAction_Objects_DataComputer::eBypass, *pObject );
    ComputeDataWithReinforcement( dataComputer );
    dataComputer.ReserveConsumptions();

    const MT_Float rDeltaPercentage = dataComputer.GetDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
    {
        dataComputer.RollbackConsumptionsReservations();
        return eNoCapacity;
    }


    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
    pObject->Bypass( rDeltaPercentage );
    if( pObject->GetBypassPercentage() == 1. )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Activate
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Activate( uint nKnowledgeObjectID )
{
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    if( !pObject->CanBeActivated() )
        return eImpossible;

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
    pObject->Activate();
    return eFinished;
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ResumeWork
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::ResumeWork( uint nKnowledgeObjectID )
{
    assert( pPion_ );
    DEC_Knowledge_Object* pKnowledge = pPion_->GetKSQuerier().GetKnowledgeObjectFromDiaID( nKnowledgeObjectID );
    if( !pKnowledge )
        return eImpossible;
    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject )
    {
        pPion_->GetArmy().GetKSObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
        return eImpossible;
    }

    if( pObject->GetConstructionPercentage() != 1. )
    {
        if( pObject->IsPrepared() )
            return Prepare( *pObject );
        else
            return Construct( *pObject );
    }
    else if( pObject->CanBeMined() && pObject->GetMiningPercentage() != 1. )
        return Mine( *pObject );
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StartAnimateObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StartAnimateObject( uint nKnowledgeObjectID )
{
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return;

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
    pObject->AddAnimator( *pPion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StopAnimateObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StopAnimateObject( uint nKnowledgeObjectID )
{
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return;

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
    pObject->RemoveAnimator( *pPion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StartOccupyingObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StartOccupyingObject( uint nKnowledgeObjectID )
{
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return;

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
    pObject->AddOccupier( *pPion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StopOccupyingObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StopOccupyingObject( uint nKnowledgeObjectID )
{
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return;

    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectInteraction( *pObject );
    pObject->RemoveOccupier( *pPion_ );
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanConstructWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanConstructWithReinforcement( const MIL_RealObjectType& objectType ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( PHY_RoleAction_Objects_CapabilityComputer::eConstruct, objectType );
    ComputeCapabilityWithReinforcement( capabilityComputer );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanBypassWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanBypassWithReinforcement( const MIL_RealObjectType& objectType ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( PHY_RoleAction_Objects_CapabilityComputer::eBypass, objectType );
    ComputeCapabilityWithReinforcement( capabilityComputer );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanDestroyWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanDestroyWithReinforcement( const MIL_RealObjectType& objectType ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( PHY_RoleAction_Objects_CapabilityComputer::eDestroy, objectType );
    ComputeCapabilityWithReinforcement( capabilityComputer );
    return capabilityComputer.HasCapability();
}


