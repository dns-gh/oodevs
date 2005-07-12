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

#include "MIL_Pch.h"

#include "PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"

#include "Knowledge/DEC_KS_ObjectInteraction.h"

#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

#include "Tools/MIL_Tools.h"

#include "TER/TER_World.h"

#include "Hla/HLA_UpdateFunctor.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Location, "PHY_RolePion_Location" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Location::PHY_RolePion_Location( MT_RoleContainer& role, MIL_AgentPion& pion )
    : PHY_RoleInterface_Location( role     )
    , pPion_                    ( &pion    )
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
// Name: PHY_RolePion_Location constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RolePion_Location::PHY_RolePion_Location()
    : PHY_RoleInterface_Location(   )
    , pPion_                    ( 0 )
    , vDirection_               (  0.,  0. )
    , vPosition_                ( -1., -1. )    //$$$ Devrait être 'NULL'
    , rHeight_                  ( 0.       )
    , rCurrentSpeed_            ( 0.       )
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
         >> pPion_
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
         << pPion_
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
    assert( pPion_ );
    return *pPion_;    
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
        GetRole< PHY_RolePion_Posture >().UnsetPostureMovement();
    else
        GetRole< PHY_RolePion_Posture >().SetPostureMovement();

    if( rHeight == rHeight_ )
        return;

    TER_DynaObject_ABC::T_DynaObjectVector objectsColliding;
    TER_DynaObject_ABC::CIT_DynaObjectVector itObject;
    TER_World::GetWorld().GetListDynaObjectsAt( vPosition_, objectsColliding );
    for( itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyMovingOutsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );

    SetHeight( rHeight );

    for( itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyMovingInsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Hide
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Hide()
{
    TER_DynaObject_ABC::T_DynaObjectVector objectsColliding;
    TER_World::GetWorld().GetListDynaObjectsAt( vPosition_, objectsColliding );
    for( TER_DynaObject_ABC::CIT_DynaObjectVector itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
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

    TER_DynaObject_ABC::T_DynaObjectVector objectsColliding;
    TER_World::GetWorld().GetListDynaObjectsAt( vPosition_, objectsColliding );
    for( TER_DynaObject_ABC::CIT_DynaObjectVector itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyPutInsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );
    bHasDoneMagicMove_ = true;
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
        GetRole< PHY_RolePion_Posture >().UnsetPostureMovement();
    else
    {
        bHasMove_ = true;
        GetRole< PHY_RolePion_Posture >().SetPostureMovement();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Follow
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Follow( const MIL_Agent_ABC& agent )
{
    const PHY_RoleInterface_Location& roleLocation = agent.GetRole< PHY_RoleInterface_Location >();
    Move( roleLocation.GetPosition(), roleLocation.GetDirection(), roleLocation.GetCurrentSpeed() );    
    SetHeight( roleLocation.GetHeight() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Move
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::MagicMove( const MT_Vector2D& vPosition )
{
    if( GetRole< PHY_RolePion_Reinforcement >().IsReinforcing() || 
        GetRole< PHY_RolePion_Transported   >().IsTransported() )
        return;

    Hide();
    Show( vPosition );
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyObjectCollision
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyObjectCollision( MIL_RealObject_ABC& object )
{
    assert( pPion_ );
    pPion_->GetKSObjectInteraction().NotifyObjectCollision( object, vPosition_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyMovingInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    assert( pPion_ );
    
    object.NotifyAgentMovingInside( *pPion_ );
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).GetRole< PHY_RolePion_Location >().NotifyMovingInsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyMovingOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    assert( pPion_ );
    
    object.NotifyAgentMovingOutside( *pPion_ );
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).GetRole< PHY_RolePion_Location >().NotifyMovingOutsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPutInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyPutInsideObject( MIL_Object_ABC& object )
{
    assert( pPion_ );
    
    object.NotifyAgentPutInside( *pPion_ );
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
        (**it).GetRole< PHY_RolePion_Location >().NotifyPutInsideObject( object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPutOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyPutOutsideObject( MIL_Object_ABC& object )
{
    assert( pPion_ );
    
    object.NotifyAgentPutOutside( *pPion_ );
    const PHY_RolePion_Reinforcement::T_PionSet& reinforcements = GetRole< PHY_RolePion_Reinforcement >().GetReinforcements();
    for( PHY_RolePion_Reinforcement::CIT_PionSet it = reinforcements.begin(); it != reinforcements.end(); ++it )
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
        msg.GetAsnMsg().m.positionPresent = 1;
        NET_ASN_Tools::WritePoint( vPosition_, msg.GetAsnMsg().position );
    }

    if( bDirectionHasChanged_ )
    {
        msg.GetAsnMsg().m.directionPresent = 1;
        NET_ASN_Tools::WriteDirection( vDirection_, msg.GetAsnMsg().direction );
    }

    if( bHeightHasChanged_ )
    {
        msg.GetAsnMsg().m.hauteurPresent = 1;
        msg.GetAsnMsg().hauteur          = (uint)rHeight_;
    }

    if( bCurrentSpeedHasChanged_ )
    {
        msg.GetAsnMsg().m.vitessePresent = 1;
        msg.GetAsnMsg().vitesse          = (uint)MIL_Tools::ConvertSpeedSimToMos( rCurrentSpeed_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    msg.GetAsnMsg().m.positionPresent = 1;
    NET_ASN_Tools::WritePoint( vPosition_, msg.GetAsnMsg().position );

    msg.GetAsnMsg().m.directionPresent = 1;
    NET_ASN_Tools::WriteDirection( vDirection_, msg.GetAsnMsg().direction );

    msg.GetAsnMsg().m.hauteurPresent = 1;
    msg.GetAsnMsg().hauteur          = (uint)rHeight_;

    msg.GetAsnMsg().m.vitessePresent = 1;
    msg.GetAsnMsg().vitesse          = (uint)MIL_Tools::ConvertSpeedSimToMos( rCurrentSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Serialize
// Created: AGE 2004-11-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Serialize( HLA_UpdateFunctor& functor ) const
{
    static std::string strPos;
    MIL_Tools::ConvertCoordSimToMos( vPosition_, strPos );
    functor.Serialize( "position",  bPositionHasChanged_,     strPos        );
    functor.Serialize( "hauteur",   bHeightHasChanged_,       rHeight_      );
    functor.Serialize( "direction", bDirectionHasChanged_,    vDirection_   );
    functor.Serialize( "vitesse",   bCurrentSpeedHasChanged_, rCurrentSpeed_);
}

