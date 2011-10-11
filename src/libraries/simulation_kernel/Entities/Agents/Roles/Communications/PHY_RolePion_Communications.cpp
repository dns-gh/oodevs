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
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "MIL_Singletons.h"
#include "MIL_Time_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectCollision.h"
#include "Knowledge/DEC_Knowledge_ObjectPerception.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "NetworkNotificationHandler_ABC.h"
#include "SpeedComputer_ABC.h"
#include "WeaponReloadingComputer_ABC.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_FormatString.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

double PHY_RolePion_Communications::rCoefSpeedModificator_         = 0.;
double PHY_RolePion_Communications::rCoefReloadingTimeModificator_ = 0.;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Communications )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Communications* role, const unsigned int /*version*/ )
{
    MIL_Entity_ABC* const entity = &role->entity_;
    archive << entity
            << role->bIsAutonomous_;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Communications* role, const unsigned int /*version*/ )
{
    MIL_Agent_ABC* entity;
  bool isAutonomous;
    archive >> entity
          >> isAutonomous;
    ::new( role )PHY_RolePion_Communications( *entity, isAutonomous );
}


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
        xis.error( "communication-breakdown: speed-modifier < 0" );
    if( rCoefReloadingTimeModificator_ <= 0 )
        xis.error( "communication-breakdown: reloading-time-modifier <= 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::PHY_RolePion_Communications( MIL_Agent_ABC& entity, const bool bIsAutonomous )
    : entity_                    ( entity )
    , bHasChanged_               ( true )
    , bBlackoutReceivedActivated_( false )
    , bBlackoutEmmittedActivated_( false )
    , bIsAutonomous_             ( bIsAutonomous )
    , pJammingKnowledgeGroup_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Communications::~PHY_RolePion_Communications()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        void save( Archive& file, const PHY_RolePion_Communications::T_JammerSet& set, const unsigned int )
        {
            std::size_t size = set.size();
            file << size;
            for ( PHY_RolePion_Communications::CIT_JammerSet it = set.begin(); it != set.end(); ++it )
                file << *it;
        }

        template< typename Archive >
        void load( Archive& file, PHY_RolePion_Communications::T_JammerSet& set, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Object_ABC* pObject;
                file >> pObject;
                set.insert( pObject );
            }
        }

        template< typename Archive >
        inline void serialize( Archive& file, PHY_RolePion_Communications::T_JammerSet& set, const unsigned int nVersion )
        {
            split_free( file, set, nVersion );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void PHY_RolePion_Communications::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Communications >( *this )
         & jammers_
         & bBlackoutReceivedActivated_
         & bBlackoutEmmittedActivated_
         & bHasChanged_
         & pJammingKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::Jam
// Created: NLD 2004-11-04
// Modified: FDS 2010-03-17
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::Jam( const MIL_Object_ABC& jammer )
{
    // UAC ...
    if( bIsAutonomous_ )
        return;

    // $$$$ >>>> MODIF FDS 2010-03-17
    // Copie of Knowledge group for jamming use
    if( CanEmit() )
        CopyKnowledgeGroup();
    // $$$$ <<<< MODIF FDS 2010-03-17

    bHasChanged_ = jammers_.insert( &jammer ).second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CopyKnowledgeGroup
// Created: LDC 2010-04-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::CopyKnowledgeGroup()
{
    if( !pJammingKnowledgeGroup_ )
        pJammingKnowledgeGroup_ = new MIL_KnowledgeGroup( entity_.GetKnowledgeGroup(), entity_, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CopyKnowledgeGroupPartial
// Created: HBD 2010-06-22
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::CopyKnowledgeGroupPartial()
{
    if( !pJammingKnowledgeGroup_ )
    {
        MIL_KnowledgeGroup& parent = entity_.GetKnowledgeGroup();
        pJammingKnowledgeGroup_ = new MIL_KnowledgeGroup( parent, entity_, &parent );
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
    if( pJammingKnowledgeGroup_ && CanEmit() )
    {
        pJammingKnowledgeGroup_->Destroy();
        delete pJammingKnowledgeGroup_;
        pJammingKnowledgeGroup_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::SendFullState( client::UnitAttributes& msg ) const
{
    msg().mutable_communications()->set_jammed( !jammers_.empty() );

    if( !jammers_.empty() || bBlackoutEmmittedActivated_ )
        msg().mutable_communications()->mutable_knowledge_group()->set_id( GetKnowledgeGroup().GetId() );
    else
        msg().mutable_communications()->mutable_knowledge_group()->set_id( 0 );

    msg().set_radio_emitter_disabled( bBlackoutEmmittedActivated_ );
    msg().set_radio_receiver_disabled( bBlackoutReceivedActivated_ );
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
        pJammingKnowledgeGroup_->UpdateKnowledges( MIL_Singletons::GetTime().GetCurrentTick() );
    if( bHasChanged_ )
        entity_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
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
    bBlackoutEmmittedActivated_ = true;
    bBlackoutReceivedActivated_ = true;
    bHasChanged_ = true;
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::ActivatePartialBlackout
// Created: HBD 2010-06-16
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::ActivatePartialBlackout()
{
   if( bBlackoutEmmittedActivated_ && !bBlackoutReceivedActivated_ )
       return;
    CopyKnowledgeGroupPartial();
    bBlackoutEmmittedActivated_ = true;
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
    bBlackoutReceivedActivated_ = false;
    if( pJammingKnowledgeGroup_ && jammers_.empty() )
    {
        pJammingKnowledgeGroup_->Destroy();
        delete pJammingKnowledgeGroup_;
        pJammingKnowledgeGroup_ = 0;
    }
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::GetKnowledgeGroup
// Returns the jamming knowledge group if it is defined, the caller must check with
// CanCommunicate to check if the jamming knowledge group is defined.
// Throws MT_ScipioException if the jamming knowledge group is undefined
// Created: FDS 2010-03-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& PHY_RolePion_Communications::GetKnowledgeGroup() const
{
    if( pJammingKnowledgeGroup_ == 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Jamming knowledge group undefined for agent %d ", entity_.GetID() ) );
    return *pJammingKnowledgeGroup_;
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
    if( object.IsMarkedForDestruction() )
        return;
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = pJammingKnowledgeGroup_->GetKnowledge().ResolveKnowledgeObject( object );

    if( !pKnowledge || !pKnowledge->IsValid() )
        pKnowledge = pJammingKnowledgeGroup_->CreateKnowledgeObject( entity_.GetArmy(), perception.GetObjectPerceived() );

    if( pKnowledge ) // $$$$ LDC: idem fix SLG rev 10556 : objects for urban knowledges don't have knowledges...
        pKnowledge->Update( perception );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::UpdateKnowledgesFromObjectCollision
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Communications::UpdateKnowledgesFromObjectCollision( const DEC_Knowledge_ObjectCollision& collision )
{
    boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = pJammingKnowledgeGroup_->GetKnowledge().ResolveKnowledgeObject( collision.GetObject() );

    if( !pKnowledge || !pKnowledge->IsValid() )
        pKnowledge = pJammingKnowledgeGroup_->CreateKnowledgeObject( entity_.GetArmy(), collision.GetObject() );

    pKnowledge->Update( collision );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CanReceive
// Created: HBD 2010-06-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::CanReceive() const
{
    return jammers_.empty() && !bBlackoutReceivedActivated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Communications::CanEmit
// Created: HBD 2010-06-18
// -----------------------------------------------------------------------------
bool PHY_RolePion_Communications::CanEmit() const
{
    return jammers_.empty() && !bBlackoutEmmittedActivated_;
}
