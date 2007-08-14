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

#include "simulation_kernel_pch.h"

#include "PHY_RoleAction_Objects.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "PHY_RoleAction_Objects_DataComputer.h"
#include "PHY_RoleAction_Objects_CapabilityComputer.h"

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
    
    DEC_Knowledge_Object* pKnowledge = pPion_->GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nKnowledgeObjectID );
    if( !pKnowledge )
        return 0;

    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    if( pObject )
        return pObject;

    pPion_->GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
    return 0;
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
    assert( pPion_ );
    if( !object.CanBeConstructed() )
        return eImpossible;

    if( object.GetConstructionPercentage() == 1. )
        return eFinished;

    PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eConstruct, object );

    const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        return eNoCapacity;

    const uint                  nDotationNeeded   = object.GetDotationNeededForConstruction( rDeltaPercentage );
    const PHY_DotationCategory* pDotationCategory = object.GetType().GetDotationCategoryForConstruction();
    if( pDotationCategory && !dataComputer.HasDotations( *pDotationCategory, nDotationNeeded ) )
        return eNoMoreDotation;

    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

    object.Construct( rDeltaPercentage );
    if( pDotationCategory )
        dataComputer.ConsumeDotations( *pDotationCategory, nDotationNeeded );

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
    pKnowledge = pPion_->GetArmy().GetKnowledge().GetKnowledgeObject( *pObject );
    return Construct( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Destroy
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Destroy( uint nKnowledgeObjectID )
{
    assert( pPion_ );
    
    DEC_Knowledge_Object* pKnowledge = pPion_->GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nKnowledgeObjectID );
    if( !pKnowledge )
        return eImpossible;
    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject )
    {
        pPion_->GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
        return eImpossible;
    }

    if( !pObject->CanBeDestroyed() )
        return eImpossible;

    if( pObject->IsMined() )
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eDemine, *pObject );        
   
        const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
        if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
            return eNoCapacity;

        pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
        pObject->Demine( rDeltaPercentage );
        return eRunning;
    }
    else
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eDestroy, *pObject );
   
        const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
        if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
            return eNoCapacity;

        const uint                  nDotationRecovered = pObject->GetDotationRecoveredWhenDestroying( rDeltaPercentage );
        const PHY_DotationCategory* pDotationCategory  = pObject->GetType().GetDotationCategoryForConstruction();

        pObject->Destroy( rDeltaPercentage );

        if( pDotationCategory && pPion_->GetArmy() == pObject->GetArmy() )
            dataComputer.RecoverDotations( *pDotationCategory, nDotationRecovered );

        if( pObject->GetConstructionPercentage() == 0. )
        {
            pPion_->GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
            return eFinished;
        }
        pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
        return eRunning;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Mine( MIL_RealObject_ABC& object )
{
    assert( pPion_ );
    if( !object.CanBeMined() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eMine, object );

    const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        return eNoCapacity;

    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
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
    assert( pPion_ );
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    if( !pObject->CanBeMined() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eDemine, *pObject );

    const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        return eNoCapacity;

    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
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
    assert( pPion_ );
    MIL_RealObject_ABC* pObject = GetRealObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    if( !pObject->CanBeBypassed() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eBypass, *pObject );

    const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        return eNoCapacity;

    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
    pObject->Bypass( rDeltaPercentage );
    if( pObject->GetBypassPercentage() == 1. )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ResumeWork
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::ResumeWork( uint nKnowledgeObjectID )
{
    assert( pPion_ );
    DEC_Knowledge_Object* pKnowledge = pPion_->GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nKnowledgeObjectID );
    if( !pKnowledge )
        return eImpossible;
    MIL_RealObject_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject )
    {
        pPion_->GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
        return eImpossible;
    }

    if( pObject->GetConstructionPercentage() != 1. )
        return Construct( *pObject );
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
    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
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
    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
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
    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
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
    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
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
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *pPion_, PHY_RoleAction_Objects_CapabilityComputer::eConstruct, objectType );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanBypassWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanBypassWithReinforcement( const MIL_RealObjectType& objectType ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *pPion_, PHY_RoleAction_Objects_CapabilityComputer::eBypass, objectType );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanDestroyWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanDestroyWithReinforcement( const MIL_RealObjectType& objectType ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *pPion_, PHY_RoleAction_Objects_CapabilityComputer::eDestroy, objectType );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanMineWithReinforcement
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanMineWithReinforcement( const MIL_RealObjectType& objectType ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *pPion_, PHY_RoleAction_Objects_CapabilityComputer::eMine, objectType );
    return capabilityComputer.HasCapability();
}

