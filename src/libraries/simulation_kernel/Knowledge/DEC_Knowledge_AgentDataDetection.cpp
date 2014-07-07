// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataDetection.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 19:03 $
// $Revision: 10 $
// $Workfile: DEC_Knowledge_AgentDataDetection.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentDataDetection.h"
#include "DEC_Knowledge_AgentPerceptionDataDetection.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/MIL_Army_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_AgentDataDetection )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection constructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataDetection::DEC_Knowledge_AgentDataDetection()
    : nTimeLastUpdate_             ( 0 )
    , vPosition_                   ( new MT_Vector2D() )
    , rSpeed_                      ( std::numeric_limits< double >::max() )
    , rAltitude_                   ( std::numeric_limits< double >::max() )
    , rPopulationDensity_          ( 0. )
    , pArmySurrenderedTo_          ( 0 )
    , pLastPosture_                ( 0 )
    , pCurrentPosture_             ( 0 )
    , rPostureCompletionPercentage_( 1. )
    , pArmy_                       ( 0 )
    , rOperationalState_           ( std::numeric_limits< double >::max() )
    , bIsPC_                       ( false )
    , bPrisoner_                   ( false )
    , bRefugeeManaged_             ( false )
    , bDead_                       ( false )
    , bWounded_                    ( false )
    , bDirectionUpdated_           ( true )
    , bSpeedUpdated_               ( true )
    , bPositionUpdated_            ( true )
    , bPrisonerUpdated_            ( true )
    , bSurrenderedUpdated_         ( true )
    , bRefugeeManagedUpdated_      ( true )
    , bDeadUpdated_                ( true )
    , bOperationalStateUpdated_    ( false )
    , bArmyAndPcUpdated_           ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataDetection::~DEC_Knowledge_AgentDataDetection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> nTimeLastUpdate_;
    file >> vPosition_;
    file >> vDirection_;
    file >> rSpeed_;
    file >> rAltitude_;
    file >> rPopulationDensity_;
    file >> const_cast< MIL_Army_ABC*& >( pArmySurrenderedTo_ );
    std::size_t nNbr;
    unsigned int nID;
    file >> nNbr;
    while( nNbr-- )
    {
        file >> nID;
        const PHY_Volume* volume = PHY_Volume::FindVolume( nID );
        if( volume )
            visionVolumes_.push_back( volume );
    }
    file >> nID;
    pLastPosture_ = PHY_Posture::FindPosture( nID );
    file >> nID;
    pCurrentPosture_ = PHY_Posture::FindPosture( nID );
    file >> rPostureCompletionPercentage_;
    file >> const_cast< MIL_Army_ABC*& >( pArmy_ );
    file >> rOperationalState_;
    file >> bIsPC_;
    file >> bDead_;
    file >> bWounded_;
    file >> bPrisoner_;
    file >> bRefugeeManaged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << nTimeLastUpdate_;
    file << vPosition_;
    file << vDirection_;
    file << rSpeed_;
    file << rAltitude_;
    file << rPopulationDensity_;
    file << pArmySurrenderedTo_;
    std::size_t size = visionVolumes_.size();
    for( auto it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
        if( !(*it) )
            --size;
    file << size;
    for( auto it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
    {
        if( *it )
        {
            unsigned int id = (*it)->GetID();
            file << id;
        }
    }
    unsigned int last = ( pLastPosture_ ? pLastPosture_->GetID() : static_cast< unsigned int >( -1 ) );
    unsigned int current = ( pCurrentPosture_ ? pCurrentPosture_->GetID() : static_cast< unsigned int >( -1 ) );
    file << last;
    file << current;
    file << rPostureCompletionPercentage_;
    file << bIsPC_;
    file << bDead_;
    file << bWounded_;
    file << bPrisoner_;
    file << bRefugeeManaged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::WriteKnowledges
// Created: NPT 2012-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::WriteKnowledges( xml::xostream& xos ) const
{
    xos << xml::start( "data-detection" )
            << xml::attribute( "last-time-update", nTimeLastUpdate_ )
            << xml::start( "position" )
                << xml::attribute( "x", vPosition_->rX_ )
                << xml::attribute( "y", vPosition_->rY_ )
            << xml::end
            << xml::start( "direction" )
                << xml::attribute( "x", vDirection_.rX_ )
                << xml::attribute( "y", vDirection_.rY_ )
            << xml::end
            << xml::attribute( "speed", rSpeed_ )
            << xml::attribute( "altitude", rAltitude_ )
            << xml::attribute( "population-density", rPopulationDensity_ );
    if( pArmySurrenderedTo_ )
        xos << xml::attribute( "surrendered-to", pArmySurrenderedTo_->GetID() );
    xos     << xml::start( "vision-volumes" );
    for( auto it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
    {
        xos     << xml::start( "volume" );
        xos     << xml::attribute( "id", ( *it )->GetID() );
        xos     << xml::end;
    }
    xos     << xml::end;
    if( pCurrentPosture_ )
        xos << xml::attribute( "posture", pCurrentPosture_->GetID() );
    xos     << xml::attribute( "posture-completion", rPostureCompletionPercentage_ );
    if( bDead_ )
        xos << xml::attribute( "dead", bDead_ );
    if( bPrisoner_ )
        xos << xml::attribute( "prisoner", bPrisoner_ );
    if( bRefugeeManaged_ )
        xos << xml::attribute( "refugee-managed", bRefugeeManaged_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Prepare
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::Prepare()
{
    bDirectionUpdated_ = false;
    bSpeedUpdated_ = false;
    bPositionUpdated_ = false;
    bPrisonerUpdated_ = false;
    bSurrenderedUpdated_ = false;
    bRefugeeManagedUpdated_ = false;
    bDeadUpdated_ = false;
    bOperationalStateUpdated_ = false;
    bArmyAndPcUpdated_ = false;
    rPopulationDensity_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::DoUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Knowledge_AgentDataDetection::DoUpdate( const T& data )
{
    if( data.GetTimeLastUpdate() <= nTimeLastUpdate_ )
        return;
    const MT_Vector2D& vNewPosition = data.GetPosition();
    if( *vPosition_ != vNewPosition )
    {
        vPosition_.reset( new MT_Vector2D( vNewPosition ) );
        bPositionUpdated_ = true;
    }
    const MT_Vector2D& vNewDirection = data.GetDirection();
    if( vDirection_ != vNewDirection )
    {
        vDirection_ = vNewDirection;
        bDirectionUpdated_ = true;
    }
    const double rNewSpeed = data.GetSpeed();
    if( rSpeed_ != rNewSpeed )
    {
        rSpeed_ = rNewSpeed;
        bSpeedUpdated_ = true;
    }
    const MIL_Army_ABC* pNewArmySurrenderedTo = data.GetArmySurrenderedTo();
    if( pArmySurrenderedTo_ != pNewArmySurrenderedTo )
    {
        pArmySurrenderedTo_ = pNewArmySurrenderedTo;
        bSurrenderedUpdated_ = true;
    }
    const bool bNewPrisoner = data.IsPrisoner();
    if( bPrisoner_ != bNewPrisoner )
    {
        bPrisoner_ = bNewPrisoner;
        bPrisonerUpdated_ = true;
    }
    const bool bNewRefugeeManaged = data.IsRefugeeManaged();
    if( bRefugeeManaged_ != bNewRefugeeManaged )
    {
        bRefugeeManaged_ = bNewRefugeeManaged;
        bRefugeeManagedUpdated_ = true;
    }
    const bool bNewDead = data.IsDead();
    if( bDead_ != bNewDead )
    {
        bDead_ = bNewDead;
        bDeadUpdated_ = true;
    }
    bWounded_ = data.IsWounded();
    double rNewOpState = data.GetOperationalState();
    if( rOperationalState_ != rNewOpState )
    {
        rOperationalState_ = rNewOpState;
        bOperationalStateUpdated_ = true;
    }
    if( !pArmy_ )
    {
        pArmy_= data.GetArmy();
        bIsPC_ = data.IsPC();
        bArmyAndPcUpdated_ = pArmy_ != 0;
    }
    rPopulationDensity_ = data.GetPopulationDensity();
    rAltitude_ = data.GetAltitude();
    pLastPosture_ = &data.GetLastPosture();
    pCurrentPosture_ = &data.GetCurrentPosture();
    rPostureCompletionPercentage_ = data.GetPostureCompletionPercentage();
    visionVolumes_ = data.GetVisibleVolumes();
    nTimeLastUpdate_ =  data.GetTimeLastUpdate();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::Update( const DEC_Knowledge_AgentPerceptionDataDetection& data )
{
    DoUpdate( data );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Update
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::Update( const DEC_Knowledge_AgentDataDetection& data )
{
    DoUpdate( data );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::Extrapolate
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::Extrapolate( const MIL_Agent_ABC& agentKnown )
{
    // Pas vraiment d'extrapolation : on prend la position réelle du pion
    const MT_Vector2D& vRealPos = agentKnown.GetRole< PHY_RoleInterface_Location >().GetPosition();
    if( vRealPos != *vPosition_ )
    {
        vPosition_.reset( new MT_Vector2D( vRealPos ) );
        bPositionUpdated_ = true;
    }
}

namespace
{
    bool CanSend( DEC_Knowledge_Agent::E_PerceptionTypes type, const PHY_PerceptionLevel& level )
    {
        const auto availableLevel = DEC_Knowledge_Agent::perceptionInfoAvailability_[ type ];
        return availableLevel != &PHY_PerceptionLevel::notSeen_ && *availableLevel <= level;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::SendFullState
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::SendFullState( sword::UnitKnowledgeUpdate& msg, const PHY_PerceptionLevel& level ) const
{
    NET_ASN_Tools::WritePoint( *vPosition_, *msg.mutable_position() );
    if( CanSend( DEC_Knowledge_Agent::ePerceptionType_Heading, level ) )
        NET_ASN_Tools::WriteDirection( vDirection_, *msg.mutable_direction() );
    if( CanSend( DEC_Knowledge_Agent::ePerceptionType_Speed, level ) )
        msg.set_speed( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) );
    if( CanSend( DEC_Knowledge_Agent::ePerceptionType_Surrendered, level ) )
        msg.mutable_surrendered_unit()->set_id( pArmySurrenderedTo_ ? pArmySurrenderedTo_->GetID() : 0 );
    if( CanSend( DEC_Knowledge_Agent::ePerceptionType_Prisoner, level ) )
        msg.set_prisoner( bPrisoner_ );
    if( CanSend( DEC_Knowledge_Agent::ePerceptionType_Refugees, level ) )
        msg.set_refugees_managed( bRefugeeManaged_ );
    if( CanSend( DEC_Knowledge_Agent::ePerceptionType_OpState, level ) )
    {
        msg.set_dead( bDead_ );
        msg.set_operational_state( std::max( 0, std::min( 100, static_cast< int >( rOperationalState_ * 100 ) ) ) );
    }
    if( pArmy_ && CanSend( DEC_Knowledge_Agent::ePerceptionType_Side, level ) )
        msg.mutable_party()->set_id( pArmy_->GetID() );
    if( CanSend( DEC_Knowledge_Agent::ePerceptionType_CommandPost, level ) )
        msg.set_command_post( bIsPC_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::SendChangedState
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataDetection::SendChangedState( sword::UnitKnowledgeUpdate& msg, const PHY_PerceptionLevel& level, bool levelChanged ) const
{
    if( bPositionUpdated_ )
        NET_ASN_Tools::WritePoint( *vPosition_, *msg.mutable_position() );
    if( ( levelChanged || bDirectionUpdated_ && !vDirection_.IsZero() ) && CanSend( DEC_Knowledge_Agent::ePerceptionType_Heading, level ) )
        NET_ASN_Tools::WriteDirection( vDirection_, *msg.mutable_direction() );
    if( ( levelChanged || bSpeedUpdated_ && rSpeed_ != std::numeric_limits< double >::max() ) && CanSend( DEC_Knowledge_Agent::ePerceptionType_Speed, level ) )
        msg.set_speed( MIL_Tools::ConvertSpeedSimToMos( rSpeed_ ) );
    if( ( levelChanged ||  bSurrenderedUpdated_ ) && CanSend( DEC_Knowledge_Agent::ePerceptionType_Surrendered, level ) )
        msg.mutable_surrendered_unit()->set_id( pArmySurrenderedTo_ ? pArmySurrenderedTo_->GetID() : 0 );
    if( ( levelChanged || bPrisonerUpdated_ ) && CanSend( DEC_Knowledge_Agent::ePerceptionType_Prisoner, level ) )
        msg.set_prisoner( bPrisoner_ );
    if( ( levelChanged || bRefugeeManagedUpdated_ ) && CanSend( DEC_Knowledge_Agent::ePerceptionType_Refugees, level ) )
        msg.set_refugees_managed( bRefugeeManaged_ );
    if( CanSend( DEC_Knowledge_Agent::ePerceptionType_OpState, level ) )
    {
        if( levelChanged || bDeadUpdated_ )
            msg.set_dead( bDead_ );
        if( levelChanged || bOperationalStateUpdated_ )
            msg.set_operational_state( std::max( 0, std::min( 100, static_cast< int >( rOperationalState_ * 100 ) ) ) );
    }
    if( ( levelChanged || bArmyAndPcUpdated_ ) && pArmy_ && CanSend( DEC_Knowledge_Agent::ePerceptionType_Side, level ) )
        msg.mutable_party()->set_id( pArmy_->GetID() );
    if( ( levelChanged || bArmyAndPcUpdated_ ) && CanSend( DEC_Knowledge_Agent::ePerceptionType_CommandPost, level ) )
        msg.set_command_post( bIsPC_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsDead
// Created: NLD 2006-02-01
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsDead() const
{
    return bDead_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetOperationalState
// Created: JSR 2014-07-07
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentDataDetection::GetOperationalState() const
{
    return rOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetArmy
// Created: JSR 2014-07-07
// -----------------------------------------------------------------------------
const MIL_Army_ABC* DEC_Knowledge_AgentDataDetection::GetArmy() const
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsPC
// Created: JSR 2014-07-07
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsPC() const
{
    return bIsPC_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsSurrendered() const
{
    return pArmySurrenderedTo_ != 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetArmySurrenderedTo
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
const MIL_Army_ABC* DEC_Knowledge_AgentDataDetection::GetArmySurrenderedTo() const
{
    return pArmySurrenderedTo_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsPrisoner() const
{
    return bPrisoner_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsRefugeeManaged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsRefugeeManaged() const
{
    return bRefugeeManaged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetSpeed
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentDataDetection::GetSpeed() const
{
    return rSpeed_ == std::numeric_limits< double >::max() ? 0. : rSpeed_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetAltitude
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentDataDetection::GetAltitude() const
{
    return rAltitude_ == std::numeric_limits< double >::max() ? 0. : rAltitude_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetDirection
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_AgentDataDetection::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetPosition
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_AgentDataDetection::GetPosition() const
{
    return *vPosition_;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_Knowledge_AgentDataDetection::GetPositionPtr
// Created: LDC 2012-09-14
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_Knowledge_AgentDataDetection::GetPositionPtr() const
{
    return vPosition_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetLastPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const PHY_Posture& DEC_Knowledge_AgentDataDetection::GetLastPosture() const
{
    assert( pLastPosture_ );
    return *pLastPosture_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetCurrentPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const PHY_Posture& DEC_Knowledge_AgentDataDetection::GetCurrentPosture() const
{
    assert( pCurrentPosture_ );
    return *pCurrentPosture_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetPostureCompletionPercentage
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentDataDetection::GetPostureCompletionPercentage() const
{
    return rPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetVisibleVolumes
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const DEC_Knowledge_AgentDataDetection::T_ComposanteVolumes& DEC_Knowledge_AgentDataDetection::GetVisibleVolumes() const
{
    return visionVolumes_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetTimeLastUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_AgentDataDetection::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::HasChanged
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::HasChanged() const
{
    return bDirectionUpdated_ || bSpeedUpdated_ || bPositionUpdated_ || bPrisonerUpdated_ || bSurrenderedUpdated_ 
        || bRefugeeManagedUpdated_ || bDeadUpdated_ || bOperationalStateUpdated_ || bArmyAndPcUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::GetPopulationDensity
// Created: LDC 2011-04-07
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentDataDetection::GetPopulationDensity() const
{
    return rPopulationDensity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataDetection::IsWounded
// Created: LGY 2012-12-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataDetection::IsWounded() const
{
    return bWounded_;
}
