// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Location/PHY_RolePion_Location.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 10:52 $
// $Revision: 22 $
// $Workfile: PHY_RolePion_Location.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_KS_PopulationInteraction.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_World.h"
#include "Hla/HLA_UpdateFunctor.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Location, "PHY_RolePion_Location" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_Location* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_Location* role, const unsigned int /*version*/ )
{
	MIL_AgentPion* pion;
	archive >> pion;
	::new( role )PHY_RolePion_Location( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Location::PHY_RolePion_Location( MIL_AgentPion& pion )
    : pion_                     ( pion    )
    , vDirection_               (  0.,  0. )
    , vPosition_                ( -1., -1. )    //$$$ Devrait être 'NULL'
    , rHeight_                  ( -1.      )
    , rCurrentSpeed_            ( -1.      )
    , bHasDoneMagicMove_        ( false    )
    , bHasMove_                 ( false    )
    , bPositionHasChanged_      ( true     )
    , bDirectionHasChanged_     ( true     )
    , bCurrentSpeedHasChanged_  ( true     )
    , bHeightHasChanged_        ( true     )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Location::~PHY_RolePion_Location()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Location >( *this )
		 >> vDirection_
         >> vPosition_
         >> bHasDoneMagicMove_
         >> bHasMove_;
         
    UpdatePatch();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Location >( *this )
         << vDirection_
         << vPosition_
         << bHasDoneMagicMove_
         << bHasMove_;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetAgent
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_Agent_ABC& PHY_RolePion_Location::GetAgent() const
{
    return pion_;    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetAltitude
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Location::GetAltitude() const
{
    return MIL_Tools::GetAltitude( vPosition_ ) + rHeight_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SetHeight
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::SetHeight( MT_Float rHeight )
{
    if( rHeight == rHeight_ )
        return;

    bHeightHasChanged_ = true;
    rHeight_           = rHeight;
}

//-----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SetPosition
// Created: DFT 02-02-27
// Last modified: JVT 04-06-01
//-----------------------------------------------------------------------------
inline
void PHY_RolePion_Location::SetPosition( const MT_Vector2D& vPosition )
{
    if( vPosition == vPosition_ )
        return;

    bPositionHasChanged_ = true;
    vPosition_           = vPosition;
    TER_World::GetWorld().ClipPointInsideWorld( vPosition_ );

    UpdatePatch();
}

//-----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SetDirection
// Created: JVT 02-07-18
//-----------------------------------------------------------------------------
inline
void PHY_RolePion_Location::SetDirection( const MT_Vector2D& vDirection )
{
    if( vDirection == vDirection_ )
        return;

    assert( MT_IsZero( vDirection.SquareMagnitude() - 1. ) );
    bDirectionHasChanged_ = true;
    vDirection_           = vDirection;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SetCurrentSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Location::SetCurrentSpeed( MT_Float rSpeed )
{
    if( rCurrentSpeed_ == rSpeed )
        return;
    bCurrentSpeedHasChanged_ = true;
    rCurrentSpeed_           = rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Fly
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Fly( MT_Float rHeight )
{

    if( rHeight == 0. )
        pion_.GetRole< PHY_RoleInterface_Posture >().UnsetPostureMovement();
    else
    {
        bHasMove_ = true;
        pion_.GetRole< PHY_RoleInterface_Posture >().SetPostureMovement();
    }

    if( rHeight == rHeight_ )
        return;

    // For decontamination zone ... refaire
    /*TER_Object_ABC::T_ObjectVector objectsColliding;
    TER_Object_ABC::CIT_ObjectVector itObject;
    TER_World::GetWorld().GetObjectManager().GetListAt( vPosition_, objectsColliding );
    for( itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyMovingOutsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );
    */

    SetHeight( rHeight );

  /*
    for( itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyMovingInsideObject( static_cast< MIL_Object_ABC& >( **itObject ) ); 
    */
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Move
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Move( const MT_Vector2D& vNewPosition, const MT_Vector2D& vNewDirection, MT_Float rNewSpeed )
{

    SetCurrentSpeed( rNewSpeed     );
    SetPosition    ( vNewPosition  );
    SetDirection   ( vNewDirection );

    if( rCurrentSpeed_ == 0. )
        pion_.GetRole< PHY_RoleInterface_Posture >().UnsetPostureMovement();
    else
    {
        bHasMove_ = true;
        pion_.GetRole< PHY_RoleInterface_Posture >().SetPostureMovement();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Follow
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Follow( const MIL_Agent_ABC& agent )
{
    const PHY_RoleInterface_Location& roleLocation = agent.GetRole< PHY_RoleInterface_Location >();
    Move     ( roleLocation.GetPosition(), roleLocation.GetDirection(), roleLocation.GetCurrentSpeed() );    
    SetHeight( roleLocation.GetHeight() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Hide
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Hide()
{
    TER_Object_ABC::T_ObjectVector objectsColliding;
    TER_World::GetWorld().GetObjectManager().GetListAt( vPosition_, objectsColliding );
    for( TER_Object_ABC::CIT_ObjectVector itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyPutOutsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );

    RemoveFromPatch();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Show
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Show( const MT_Vector2D& vPosition )
{
    Move( vPosition, vDirection_, 0. );

    TER_Object_ABC::T_ObjectVector objectsColliding;
    TER_World::GetWorld().GetObjectManager().GetListAt( vPosition_, objectsColliding );
    for( TER_Object_ABC::CIT_ObjectVector itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyPutInsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );
    bHasDoneMagicMove_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Move
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::MagicMove( const MT_Vector2D& vPosition )
{

    if(    pion_.GetRole< PHY_RoleInterface_Reinforcement      >().IsReinforcing()
        || pion_.GetRole< PHY_RoleInterface_Transported   >().IsTransported() )
        return;

    Hide();
    Show( vPosition );
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyPopulationCollision( MIL_PopulationFlow& population )
{

    pion_.GetKnowledge().GetKsPopulationInteraction().NotifyPopulationCollision( population );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyPopulationCollision( MIL_PopulationConcentration& population )
{

    pion_.GetKnowledge().GetKsPopulationInteraction().NotifyPopulationCollision( population );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyObjectCollision
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyObjectCollision( MIL_Object_ABC& object )
{

    pion_.GetKnowledge().GetKsObjectInteraction().NotifyObjectCollision( object, vPosition_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyMovingInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentMovingInside( pion_ );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion_.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).GetRole< PHY_RolePion_Location >().NotifyMovingInsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyMovingOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{

    
    object.NotifyAgentMovingOutside( pion_ );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion_.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).GetRole< PHY_RolePion_Location >().NotifyMovingOutsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPutInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyPutInsideObject( MIL_Object_ABC& object )
{

    
    object.NotifyAgentPutInside( pion_ );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion_.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).GetRole< PHY_RolePion_Location >().NotifyPutInsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPutOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyPutOutsideObject( MIL_Object_ABC& object )
{

    
    object.NotifyAgentPutOutside( pion_ );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion_.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).GetRole< PHY_RolePion_Location >().NotifyPutOutsideObject( object );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bPositionHasChanged_ )
    {
        msg().m.positionPresent = 1;
        NET_ASN_Tools::WritePoint( vPosition_, msg().position );
    }

    if( bDirectionHasChanged_ )
    {
        msg().m.directionPresent = 1;
        NET_ASN_Tools::WriteDirection( vDirection_, msg().direction );
    }

    if( bHeightHasChanged_ )
    {
        msg().m.hauteurPresent = 1;
        msg().hauteur          = (uint)rHeight_;
    }
    if( bHeightHasChanged_ || bPositionHasChanged_ )
    {
        msg().m.altitudePresent = 1;
        msg().altitude          = (uint)( rHeight_ + MIL_Tools::GetAltitude( vPosition_ ) );
    }

    if( bCurrentSpeedHasChanged_ )
    {
        msg().m.vitessePresent = 1;
        msg().vitesse          = (uint)MIL_Tools::ConvertSpeedSimToMos( rCurrentSpeed_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg().m.positionPresent = 1;
    NET_ASN_Tools::WritePoint( vPosition_, msg().position );

    msg().m.directionPresent = 1;
    NET_ASN_Tools::WriteDirection( vDirection_, msg().direction );

    msg().m.hauteurPresent = 1;
    msg().hauteur          = (uint)rHeight_;

    msg().m.vitessePresent = 1;
    msg().vitesse          = (uint)MIL_Tools::ConvertSpeedSimToMos( rCurrentSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Serialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Serialize( HLA_UpdateFunctor& functor ) const
{
    functor.Serialize( "position" , bPositionHasChanged_    , MIL_Tools::ConvertCoordSimToMos( vPosition_ ) );
    functor.Serialize( "hauteur"  , bHeightHasChanged_      , rHeight_       );
    functor.Serialize( "direction", bDirectionHasChanged_   , vDirection_    );
    functor.Serialize( "vitesse"  , bCurrentSpeedHasChanged_, rCurrentSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetHeight
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Location::GetHeight() const
{
    return rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetPosition
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RolePion_Location::GetPosition() const
{
    return vPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RolePion_Location::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetCurrentSpeed
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_Location::GetCurrentSpeed() const
{
    return rCurrentSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasDoneMagicMove
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Location::HasDoneMagicMove() const
{
    return bHasDoneMagicMove_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Update( bool bIsDead )
{
    if( bIsDead || !bHasMove_ )
    {
        Move( vPosition_, vDirection_, 0. );
        Fly ( 0. );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Clean()
{
    bHasMove_                = false;
    bHasDoneMagicMove_       = false;
    bPositionHasChanged_     = false;
    bDirectionHasChanged_    = false;
    bCurrentSpeedHasChanged_ = false;
    bHeightHasChanged_       = false;    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasSpeedChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Location::HasSpeedChanged() const
{
    return bCurrentSpeedHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasLocationChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Location::HasLocationChanged() const
{
    return bPositionHasChanged_ || bDirectionHasChanged_ || bHeightHasChanged_;
}
