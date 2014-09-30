// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_Refugee.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Refugee.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Objects/LodgingAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "protocol/ClientSenders.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "NetworkNotificationHandler_ABC.h"
#include "TransportNotificationHandler_ABC.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( refugee::PHY_RolePion_Refugee )

namespace refugee
{

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee constructor
// Created: JSR 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::PHY_RolePion_Refugee()
    : owner_      ( 0 )
    , pCamp_      ( 0 )
    , bManaged_   ( false )
    , bHasChanged_( true )
    , nbrHumansLodgingManaged_  ( 0 )
    , lodgingSatisfaction_      ( 0.0f )
    , securitySatisfaction_     ( 0.5f )
    , healthSatisfaction_       ( 0.0f )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::PHY_RolePion_Refugee( MIL_AgentPion& pion )
    : owner_      ( &pion )
    , pCamp_      ( 0 )
    , bManaged_   ( false )
    , bHasChanged_( true )
    , nbrHumansLodgingManaged_  ( 0 )
    , lodgingSatisfaction_      ( 0.0f )
    , securitySatisfaction_     ( 0.5f )
    , healthSatisfaction_       ( 0.0f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Refugee::~PHY_RolePion_Refugee()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Refugee::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Refugee >( *this );
    file & owner_;
    file & bManaged_;
    file & const_cast< MIL_Object_ABC*& >( pCamp_ );
    file & nbrHumansLodgingManaged_;
    file & lodgingSatisfaction_;
    file & securitySatisfaction_;
    file & healthSatisfaction_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Update( bool /*bIsDead*/ )
{
    if( pCamp_ && pCamp_->IsMarkedForDestruction() )
        UnmanageLodgingCamp();

    UpdateSecuritySatisfaction();

    if( HasChanged() )
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Orientate
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Orientate( MIL_AgentPion& pionManaging )
{
    if( !owner_->GetType().IsRefugee() )
        return;
    bManaged_    = true;
    bHasChanged_ = true;
    UnmanageLodgingCamp();
    owner_->GetAutomate().NotifyRefugeeOriented( pionManaging );
    bool bTransportOnlyLoadable = false;
    pionManaging.Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, *owner_, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Release
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Release( MIL_AgentPion& callerAgent )
{
    if( !owner_->GetType().IsRefugee() || !bManaged_ )
        return;
    bManaged_    = false;
    bHasChanged_ = true;
    owner_->GetAutomate().NotifyRefugeeReleased();
    callerAgent.Apply( &transport::TransportNotificationHandler_ABC::MagicUnloadPion, *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::ReleaseCamp
// Created: NLD 2007-02-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::ReleaseCamp( MIL_AgentPion& callerAgent, const MIL_Object_ABC& camp )
{
    if( pCamp_ && ( pCamp_ != &camp ) )
        UnmanageLodgingCamp();

    if( !owner_->GetType().IsRefugee() || !bManaged_ )
        return;
    pCamp_       = &camp;
    bManaged_    = true;
    bHasChanged_ = true;
    owner_->GetAutomate().NotifyRefugeeReleased( camp );
    callerAgent.Apply( &transport::TransportNotificationHandler_ABC::MagicUnloadPion, *owner_ );
    ManageLodgingCamp( callerAgent );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::IsManaged
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::IsManaged( const MIL_Object_ABC& camp ) const
{
    if( !pCamp_ )
        return false;
    return pCamp_ == &camp; //$$$$
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_refugees_managed( bManaged_ );
    msg().mutable_satisfaction()->set_lodging( lodgingSatisfaction_ );
    msg().mutable_satisfaction()->set_safety( securitySatisfaction_ );
    msg().mutable_satisfaction()->set_access_to_health_care( healthSatisfaction_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::GetNbrHumansCampManaged
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Refugee::GetNbrHumansCampManaged() const
{
    return nbrHumansLodgingManaged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::GetNbrHumansCampUnmanaged
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Refugee::GetNbrHumansCampUnmanaged() const
{
    const PHY_RolePion_Composantes& composantes = owner_->GetRole< PHY_RolePion_Composantes >();
    unsigned int nbrUsableHumans = composantes.GetNbrLivingHumans();
    if( nbrUsableHumans == 0 )
        return 0;
    if( nbrHumansLodgingManaged_ > nbrUsableHumans )
        return nbrUsableHumans;

    return (nbrUsableHumans - nbrHumansLodgingManaged_);
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::UpdateLodging
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::UpdateLodging( unsigned int nbrHumansCampManaged )
{
    float prevSatisf = lodgingSatisfaction_;
    nbrHumansLodgingManaged_ = nbrHumansCampManaged;

    PHY_RolePion_Composantes& composantes = owner_->GetRole< PHY_RolePion_Composantes >();
    unsigned int nbrUsableHumans = composantes.GetNbrLivingHumans();

    if( nbrUsableHumans > 0 )
        lodgingSatisfaction_ = std::min( 1.0f, float( nbrHumansCampManaged ) / float( nbrUsableHumans) );

    if( prevSatisf != lodgingSatisfaction_ )
        bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::HasChanged() const
{
    return bHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::IsManaged
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool PHY_RolePion_Refugee::IsManaged() const
{
    return bManaged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::ManagedLodgingCamp
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::ManageLodgingCamp( MIL_AgentPion& transporter )
{
    if( !pCamp_ )
        UpdateLodging( 0 );

    LodgingAttribute* pLodgingAttribute = const_cast< MIL_Object_ABC* >( pCamp_ )->RetrieveAttribute< LodgingAttribute >();
    if( pLodgingAttribute )
        pLodgingAttribute->ManageResident( *owner_, transporter );

    UpdateHealthSatisfaction();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::UnmanagedLodgingCamp
// Created: MMC 2011-05-09
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::UnmanageLodgingCamp()
{
    if( !pCamp_ )
        return;

    LodgingAttribute* pLodgingAttribute = const_cast< MIL_Object_ABC* >( pCamp_ )->RetrieveAttribute< LodgingAttribute >();
    if( pLodgingAttribute )
        pLodgingAttribute->UnmanageResident( *owner_ );

    pCamp_ = 0;
    UpdateHealthSatisfaction();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::UpdateSecuritySatisfaction
// Created: MMC 2011-05-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::UpdateSecuritySatisfaction()
{
    float prevSatisf = securitySatisfaction_;
    securitySatisfaction_ = 0.5f;
    nearbyUnitsAffinity.resetAffinitySum( owner_->GetRole< PHY_RoleInterface_Perceiver >().GetMaxTheoreticalAgentPerceptionDistance(),
                                          owner_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    if( nearbyUnitsAffinity.maxSqrDistance > 0.01 /*epsilon*/ )
    {
        auto bbKg = owner_->GetKnowledgeGroup()->GetKnowledge();
        if( bbKg )
        {
            auto methodAffinityAgent = boost::bind( &PHY_RolePion_Refugee::AddAffinityNearUnit, this, _1 );
            bbKg->ApplyOnKnowledgesAgent( methodAffinityAgent );
        }

        if( nearbyUnitsAffinity.absAffinitySum_ > 0.0f )
            securitySatisfaction_ = 0.5f * ( nearbyUnitsAffinity.affinitySum_ / nearbyUnitsAffinity.absAffinitySum_ + 1.0f );
    }

    if( prevSatisf != securitySatisfaction_ )
        bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::UpdateHealthSatisfaction
// Created: MMC 2011-05-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::UpdateHealthSatisfaction()
{
    float prevSatisf = healthSatisfaction_;
    healthSatisfaction_ = owner_->GetAutomate().GetLogisticHierarchy().HasSuperior() ? 1.0f : 0.0f;
    if( prevSatisf != healthSatisfaction_ )
        bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Refugee::AddAffinityNearUnit
// Created: MMC 2011-05-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Refugee::AddAffinityNearUnit( DEC_Knowledge_Agent& agent )
{
    const MIL_Army_ABC* army = agent.GetArmy();
    if( army && ( agent.GetPosition().SquareDistance( nearbyUnitsAffinity.position ) < nearbyUnitsAffinity.maxSqrDistance ) )
    {
        float agentAffinity = owner_->GetAffinity( army->GetID() );
        nearbyUnitsAffinity.affinitySum_ += agentAffinity;
        nearbyUnitsAffinity.absAffinitySum_ += fabs( agentAffinity );
    }
}

} // namespace refugee

