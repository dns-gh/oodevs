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
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/ConstructionAttribute.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/Objects/BuildableCapacity.h"
#include "Entities/Objects/WorkableCapacity.h"
#include "Entities/Objects/OccupantAttribute.h"
#include "Entities/Objects/FireAttribute.h"
#include "Entities/Objects/MIL_FireFunctor.h"
#include "Entities/Objects/ExtinguishableCapacity.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
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
// Name: PHY_RoleAction_Objects::GetObject
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
inline
MIL_Object_ABC* PHY_RoleAction_Objects::GetObject( uint nKnowledgeObjectID )
{
    assert( pPion_ );
    
    DEC_Knowledge_Object* pKnowledge = pPion_->GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nKnowledgeObjectID );
    if( !pKnowledge )
        return 0;

    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
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
int PHY_RoleAction_Objects::Construct( MIL_Object_ABC& object )
{    
    if( !object().CanBeConstructed() )
        return eImpossible;

    if( object().IsBuilt() )
        return eFinished;

    PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eConstruct, object );

    const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        return eNoCapacity;

    // $$$$ TODO: refactor to handle more than a single resource
    const ConstructionAttribute& attribute = object.GetAttribute< ConstructionAttribute >();
	const uint                  nDotationNeeded   = attribute.GetDotationNeededForConstruction( rDeltaPercentage );
    const PHY_DotationCategory* pDotationCategory = object.Get< BuildableCapacity >().GetDotationCategory();
    if( pDotationCategory && !dataComputer.HasDotations( *pDotationCategory, nDotationNeeded ) )
        return eNoMoreDotation;

    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

    object().Construct( rDeltaPercentage );
    if( pDotationCategory )
        dataComputer.ConsumeDotations( *pDotationCategory, nDotationNeeded );

    if( object().IsBuilt() )
        return eFinished;

    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Construct
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Construct( MIL_Object_ABC* pObject, DEC_Knowledge_Object*& pKnowledge )
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
        return 0;

    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject ) 
    {
        pPion_->GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
        return 0;
    }
        
    MIL_Object_ABC& object = *pObject; 
    if( !object().CanBeDestroyed() )
        return eImpossible;

    if( object().IsMined() )
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eDemine, object );        
   
        const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
        if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
            return eNoCapacity;

        pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        object().Demine( rDeltaPercentage );
        return eRunning;
    }
    else
    {
        PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eDestroy, object );
   
        const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
        if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
            return eNoCapacity;

        // $$$$ TODO: refactor to handle more than a single resource
        const ConstructionAttribute& attribute = object.GetAttribute< ConstructionAttribute >();
        const uint                  nDotationRecovered = attribute.GetDotationRecoveredWhenDestroying( rDeltaPercentage );
        const PHY_DotationCategory* pDotationCategory  = object.Get< BuildableCapacity >().GetDotationCategory();

        object().Destroy( rDeltaPercentage );

        if( pDotationCategory && pPion_->GetArmy() == pObject->GetArmy() )
            dataComputer.RecoverDotations( *pDotationCategory, nDotationRecovered );

        if( attribute.GetState() == 0. )
        {
            pPion_->GetArmy().GetKnowledge().GetKsObjectKnowledgeSynthetizer().AddObjectKnowledgeToForget( *pKnowledge );
            return eFinished;
        }
        pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        return eRunning;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Mine( MIL_Object_ABC& object )
{
    assert( pPion_ );
    if( !object().CanBeMined() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eMine, object );

    const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        return eNoCapacity;

    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Mine( rDeltaPercentage );
    if( object().IsFullyMined() )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Mine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Mine( uint nKnowledgeObjectID )
{
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    return Mine( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Demine
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Demine( uint nKnowledgeObjectID )
{
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    return Demine( *pObject );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Demine
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Demine( MIL_Object_ABC& object )
{
    if( !object().CanBeMined() )
        return eImpossible;

    PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eDemine, object );

    const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        return eNoCapacity;

    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Demine( rDeltaPercentage );
    if( object.GetAttribute< MineAttribute >().GetState() == 0. )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Bypass
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Bypass( uint nKnowledgeObjectID )
{    
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;

    MIL_Object_ABC& object = *pObject;
    if( !object().CanBeBypassed() )
        return eImpossible;

    if( object().IsBypassed() )
        return eFinished;

    PHY_RoleAction_Objects_DataComputer dataComputer( *pPion_, PHY_RoleAction_Objects_DataComputerPionData::eBypass, object );
    const MT_Float rDeltaPercentage = dataComputer.ComputeDeltaPercentage();
    if( rDeltaPercentage == std::numeric_limits< MT_Float >::max() )
        return eNoCapacity;
    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
    object().Bypass( rDeltaPercentage );
    if( object().IsBypassed() )
        return eFinished;
    return eRunning;
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: ExtinguishableCapacity::GetBestExtinguisherAgent
    // Created: RFT 2004-09-16
    // -----------------------------------------------------------------------------
    int GetBestExtinguisher( const MIL_AgentPion* pPion, MIL_FireFunctor& functor )
    {
        pPion->GetRole< PHY_RolePion_Composantes >().Apply( functor );

        const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = pPion->GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
        for( PHY_RolePion_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
            (*itReinforcement)->GetRole< PHY_RolePion_Composantes >().Apply( functor );
    
        return functor.GetNumberOfTheExtinguisherAgent();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Extinguish
// Created: RFT 28/05/2008
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::Extinguish( uint nKnowledgeObjectID )
{
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject || pObject->IsMarkedForDestruction() )
        return eImpossible;

    MIL_Object_ABC& object = *pObject;
    ExtinguishableCapacity* capacity = object.Retrieve< ExtinguishableCapacity >(); 
    if ( !capacity )    
        return eImpossible;

    pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );

    //selection numberOFireHoses, bestExtinguisherAgent
    MIL_FireFunctor functor( object.GetAttribute< FireAttribute >().GetClass() );
    int bestExtinguisherAgent = GetBestExtinguisher( pPion_, functor );
    object().Extinguish( bestExtinguisherAgent, pPion_->GetNumberOfFireHoses( bestExtinguisherAgent ) );
    
    if( object.GetAttribute< FireAttribute >().GetHeat() <= 0 )
        return eFinished;
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ResumeWork
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::ResumeWork( uint nKnowledgeObjectID )
{    
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject )
        return eImpossible;
    MIL_Object_ABC& object = *pObject;
    if( ! object().IsBuilt() )
        return Construct( object );
    else if( object().CanBeMined() && !object().IsFullyMined() )
        return Mine( object );
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StartAnimateObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StartAnimateObject( uint nKnowledgeObjectID )
{
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject )
        return;

    MIL_Object_ABC& object = *pObject;
    assert( pPion_ );
    WorkableCapacity* capacity = object.Retrieve< WorkableCapacity >();
    if ( capacity )
    {
        pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        object().AddAnimator( *pPion_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StopAnimateObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StopAnimateObject( uint nKnowledgeObjectID )
{
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject )
        return;

    MIL_Object_ABC& object = *pObject;
    assert( pPion_ );
    WorkableCapacity* capacity = object.Retrieve< WorkableCapacity >();
    if( capacity )
    {
        pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( object );
        object().ReleaseAnimator( *pPion_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StartOccupyingObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StartOccupyingObject( uint nKnowledgeObjectID )
{
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject )
        return;

    assert( pPion_ );
    OccupantAttribute* pAttribute = pObject->RetrieveAttribute< OccupantAttribute >();
    if ( pAttribute )
    {
        pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
        pAttribute->AddOccupant( *pPion_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::StopOccupyingObject
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::StopOccupyingObject( uint nKnowledgeObjectID )
{
    MIL_Object_ABC* pObject = GetObject( nKnowledgeObjectID );
    if( !pObject )
        return;

    assert( pPion_ );
    OccupantAttribute* pAttribute = pObject->RetrieveAttribute< OccupantAttribute >();
    if ( pAttribute )
    {
        pPion_->GetKnowledge().GetKsObjectInteraction().NotifyObjectInteraction( *pObject );
        pAttribute->ReleaseOccupant( *pPion_ );
    }
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanConstructWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanConstructWithReinforcement( const std::string& strType ) const
{
    const MIL_ObjectType_ABC& type = MIL_AgentServer::GetWorkspace().GetEntityManager().FindObjectType( strType );
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *pPion_, PHY_RoleAction_Objects_CapabilityComputer::eConstruct, type );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanBypassWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanBypassWithReinforcement( const MIL_ObjectType_ABC& object ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *pPion_, PHY_RoleAction_Objects_CapabilityComputer::eBypass, object );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanDestroyWithReinforcement
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanDestroyWithReinforcement( const MIL_ObjectType_ABC& object ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *pPion_, PHY_RoleAction_Objects_CapabilityComputer::eDestroy, object );
    return capabilityComputer.HasCapability();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::CanMineWithReinforcement
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects::CanMineWithReinforcement( const MIL_ObjectType_ABC& object ) const
{
    PHY_RoleAction_Objects_CapabilityComputer capabilityComputer( *pPion_, PHY_RoleAction_Objects_CapabilityComputer::eMine, object );
    return capabilityComputer.HasCapability();
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetInitialReturnCode
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::GetInitialReturnCode() const
{
    return eRunning;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::GetFinalReturnCode
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
int PHY_RoleAction_Objects::GetFinalReturnCode() const
{
    return eFinished;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::Clean
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::Clean()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ResumeWorkSuspended
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::ResumeWorkSuspended()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ConstructSuspended
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::ConstructSuspended()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::DestroySuspended
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::DestroySuspended()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::MineSuspended
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::MineSuspended()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::DemineSuspended
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::DemineSuspended()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::BypassSuspended
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::BypassSuspended()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects::ExtinguishSuspended
// Created: RFT 28/05/2008
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects::ExtinguishSuspended()
{
}
