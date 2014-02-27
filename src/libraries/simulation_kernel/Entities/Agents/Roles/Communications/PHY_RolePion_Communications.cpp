// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RolePion_Communications.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:12 $
// $Revision: 4 $
// $Workfile: PHY_RolePion_Communications.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Communications.h"
#include "MIL_Time_ABC.h"
#include "SpeedComputer_ABC.h"
#include "WeaponReloadingComputer_ABC.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectCollision.h"
#include "Knowledge/DEC_Knowledge_ObjectPerception.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MT_Tools/MT_FormatString.h"
#include "NetworkNotificationHandler_ABC.h"
#include "protocol/ClientSenders.h"

double PHY_RolePion_Communications::rCoefSpeedModificator_         = 0.;
double PHY_RolePion_Communications::rCoefReloadingTimeModificator_ = 0.;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Communications )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Initialize
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "communications" )
            >> xml::start( "communication-breakdown" )
                >> xml::attribute( "speed-modifier", rCoefSpeedModificator_ )
                >> xml::attribute( "reloading-time-modifier", rCoefReloadingTimeModificator_ )
            >> xml::end
        >> xml::end;

    if( rCoefSpeedModificator_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "communication-breakdown: speed-modifier < 0" );
    if( rCoefReloadingTimeModificator_ <= 0 )
        throw MASA_EXCEPTION( xis.context() + "communication-breakdown: reloading-time-modifier <= 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::PHY_RolePion_Communications()
    : owner_                     ( 0 )
    , bHasChanged_               ( true )
    , bBlackoutReceivedActivated_( false )
    , bBlackoutEmmittedReport_   ( false )
    , bBlackoutEmmittedActivated_( false )
    , bSilentBeforeCapture_      ( false )
    , bIsAutonomous_             ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::PHY_RolePion_Communications( MIL_Agent_ABC& entity, const bool bIsAutonomous )
    : owner_                     ( &entity )
    , bHasChanged_               ( true )
    , bBlackoutReceivedActivated_( false )
    , bBlackoutEmmittedActivated_( false )
    , bBlackoutEmmittedReport_   ( false )
    , bSilentBeforeCapture_      ( false )
    , bIsAutonomous_             ( bIsAutonomous )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::~PHY_RolePion_Communications()
{
    if( pJammingKnowledgeGroup_ )
    {
        pJammingKnowledgeGroup_->Destroy();
        pJammingKnowledgeGroup_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Communications::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Communications >( *this );
    file & owner_;
    file & bIsAutonomous_;
    file & jammers_;
    file & bBlackoutReceivedActivated_;
    file & bBlackoutEmmittedActivated_;
    file & bHasChanged_;
    file & bSilentBeforeCapture_;
    file & pJammingKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Jam
// Created: NLD 2004-11-04
// Modified: FDS 2010-03-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Jam( const MIL_Object_ABC& jammer )
{
    // UAC ...
    if( bIsAutonomous_ || owner_->IsDead() )
        return;

    if( CanEmit() )
        CopyKnowledgeGroup();

    bHasChanged_ = jammers_.insert( &jammer ).second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CopyKnowledgeGroup
// Created: LDC 2010-04-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::CopyKnowledgeGroup()
{
    if( !pJammingKnowledgeGroup_ )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > group = owner_->GetKnowledgeGroup();
        pJammingKnowledgeGroup_.reset( new MIL_KnowledgeGroup( *group, *owner_, 0 ) );
        group->GetArmy().RegisterKnowledgeGroup( pJammingKnowledgeGroup_ );
        pJammingKnowledgeGroup_->Clone( *group );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CopyKnowledgeGroupPartial
// Created: HBD 2010-06-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::CopyKnowledgeGroupPartial()
{
    if( !pJammingKnowledgeGroup_ )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > parent = owner_->GetKnowledgeGroup();
        pJammingKnowledgeGroup_.reset( new MIL_KnowledgeGroup( *parent, *owner_, parent.get() ) );
        parent->RegisterKnowledgeGroup( pJammingKnowledgeGroup_ );
        pJammingKnowledgeGroup_->Clone( *parent );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Unjam
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Unjam( const MIL_Object_ABC& jammer )
{
    bHasChanged_ = ( jammers_.erase( &jammer ) == 1 );

    // delete copy of knowledge group used in jamming
    if( pJammingKnowledgeGroup_ && ( CanEmit() || owner_->IsMarkedForDestruction() ) )
    {
        if( !owner_->IsMarkedForDestruction() )
            owner_->GetKnowledgeGroup()->Merge( *pJammingKnowledgeGroup_ );
        pJammingKnowledgeGroup_->Destroy();
        pJammingKnowledgeGroup_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::SendFullState( client::UnitAttributes& msg ) const
{
    msg().mutable_communications()->set_jammed( !jammers_.empty() );

    unsigned int jammedKgId = 0;
    if( !jammers_.empty() || bBlackoutEmmittedActivated_ )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > kg = GetJammedKnowledgeGroup();
        if( kg )
            jammedKgId = kg->GetId();
    }
    msg().mutable_communications()->mutable_knowledge_group()->set_id( jammedKgId );

    msg().set_radio_emitter_disabled( bBlackoutEmmittedActivated_ && bBlackoutEmmittedReport_ );
    msg().set_radio_receiver_disabled( bBlackoutReceivedActivated_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CanReport
// Created: LGY 2013-07-03
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::CanReport() const
{
    return !bBlackoutEmmittedReport_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Update( bool /*bIsDead*/ )
{
    if( bHasChanged_ && pJammingKnowledgeGroup_ )
        pJammingKnowledgeGroup_->UpdateKnowledges( MIL_Time_ABC::GetTime().GetCurrentTimeStep() );
    if( bHasChanged_ )
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ActivateBlackout
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::ActivateBlackout()
{
    if( bBlackoutEmmittedActivated_ && bBlackoutReceivedActivated_ )
        return;
    CopyKnowledgeGroup();
    bBlackoutEmmittedReport_ = true;
    bBlackoutEmmittedActivated_ = true;
    bBlackoutReceivedActivated_ = true;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ActivatePartialBlackout
// Created: HBD 2010-06-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::ActivatePartialBlackout( bool report /*= true*/)
{
   if( bBlackoutEmmittedActivated_ && !bBlackoutReceivedActivated_ )
       return;
    CopyKnowledgeGroupPartial();
    bBlackoutEmmittedActivated_ = true;
    bBlackoutEmmittedReport_ = report;
    bBlackoutReceivedActivated_ = false;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::DeactivateBlackout
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::DeactivateBlackout()
{
    if( !bBlackoutEmmittedActivated_ && !bBlackoutReceivedActivated_ )
        return;
    bBlackoutEmmittedActivated_ = false;
    bBlackoutEmmittedReport_ = false;
    bBlackoutReceivedActivated_ = false;
    if( pJammingKnowledgeGroup_ && jammers_.empty() )
    {
        owner_->GetKnowledgeGroup()->Merge( *pJammingKnowledgeGroup_ );
        pJammingKnowledgeGroup_->Destroy();
        pJammingKnowledgeGroup_.reset();
    }
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::GetJammedKnowledgeGroup
// Returns the jamming knowledge group if it is defined, the caller must check with
// CanCommunicate to check if the jamming knowledge group is defined.
// Created: FDS 2010-03-15
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > PHY_RolePion_Communications::GetJammedKnowledgeGroup() const
{
    if( pJammingKnowledgeGroup_ ) // pion is jammed
        return pJammingKnowledgeGroup_;
    else if( owner_->IsDead() ) // if pion is dead, it cannot emit, but it is not jammed
        return owner_->GetAutomate().GetKnowledgeGroup();
    throw boost::shared_ptr< MIL_KnowledgeGroup >();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::IsJammed
// Created: MMC 2012-07-02
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::IsJammed() const
{
    return !jammers_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::IsInEmissionBlackout
// Created: MMC 2012-07-02
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications:: IsInEmissionBlackout() const
{
    return bBlackoutEmmittedActivated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::IsInReceptionBlackout
// Created: MMC 2012-07-02
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::IsInReceptionBlackout() const
{
    return bBlackoutReceivedActivated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ModifySpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Execute(moving::SpeedComputer_ABC& algorithm) const
{
    if( !jammers_.empty() )
        algorithm.AddModifier(rCoefSpeedModificator_, false);
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Execute
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const
{
    if( !jammers_.empty() )
        algorithm.AddModifier( rCoefReloadingTimeModificator_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::UpdateKnowledgesFromObjectPerception
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& perception )
{
    MIL_Object_ABC& object = perception.GetObjectPerceived();
    if( object.IsMarkedForDestruction() || !pJammingKnowledgeGroup_ )
        return;
    auto bbKg = pJammingKnowledgeGroup_->GetKnowledge();
    if( !bbKg )
        return;
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = bbKg->ResolveKnowledgeObject( object );

    if( !pKnowledge || !pKnowledge->IsValid() )
        pKnowledge = pJammingKnowledgeGroup_->CreateKnowledgeObject( perception.GetObjectPerceived() );

    if( pKnowledge ) // $$$$ LDC: idem fix SLG rev 10556 : objects for urban knowledges don't have knowledges...
        pKnowledge->Update( perception );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::UpdateKnowledgesFromObjectCollision
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::UpdateKnowledgesFromObjectCollision( const DEC_Knowledge_ObjectCollision& collision )
{
    if( !pJammingKnowledgeGroup_ )
        return;
    auto bbKg = pJammingKnowledgeGroup_->GetKnowledge();
    if( !bbKg )
        return;
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = bbKg->ResolveKnowledgeObject( collision.GetObject() );

    if( collision.GetObject().IsMarkedForDestruction() )
        return;
    if( !pKnowledge || !pKnowledge->IsValid() )
        pKnowledge = pJammingKnowledgeGroup_->CreateKnowledgeObject( collision.GetObject() );

    pKnowledge->Update( collision );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CanReceive
// Created: HBD 2010-06-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::CanReceive() const
{
    return jammers_.empty() && !bBlackoutReceivedActivated_ && !owner_->IsDead();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CanEmit
// Created: HBD 2010-06-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::CanEmit() const
{
    return jammers_.empty() && !bBlackoutEmmittedActivated_ && !owner_->IsDead();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::NotifyCaptured
// Created: JSR 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::NotifyCaptured()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::NotifyReleased
// Created: JSR 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::NotifyReleased()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::NotifySurrendered
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::NotifySurrendered()
{
    bSilentBeforeCapture_ = bBlackoutEmmittedActivated_;
    if( !bBlackoutEmmittedActivated_ )
        ActivateBlackout();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::NotifySurrenderCanceled
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::NotifySurrenderCanceled()
{
    if( !bSilentBeforeCapture_ )
        DeactivateBlackout();
}
