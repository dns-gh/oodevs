// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_ObjectInteraction.inl $
// $Author: Jvt $
// $Modtime: 17/03/05 12:45 $
// $Revision: 4 $
// $Workfile: DEC_KS_ObjectInteraction.inl $
//
// *****************************************************************************

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::GetAgentInteracting
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& DEC_KS_ObjectInteraction::GetAgentInteracting() const
{
    assert( pAgentInteracting_ );
    return *pAgentInteracting_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::NotifyObjectInteraction
// Created: NLD 2004-03-26
// -----------------------------------------------------------------------------
inline
void DEC_KS_ObjectInteraction::NotifyObjectInteraction( MIL_RealObject_ABC& object )
{
    objectInteractions_.push_back( &object );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_ObjectInteraction::NotifyObjectCollision
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
inline
void DEC_KS_ObjectInteraction::NotifyObjectCollision( MIL_RealObject_ABC& object, const MT_Vector2D& vPosition )
{
    objectCollisions_.push_back( std::make_pair( &object, vPosition ) );
}


