// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Fire.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: DEC_KS_Fire.inl $
//
// *****************************************************************************

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_AgentPion::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KS_Fire::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_KnowledgeSource_ABC >( *this )
            & pBlackBoard_;
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::NotifyHitByDirectFire
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
inline
void DEC_KS_Fire::NotifyAttackedBy( MIL_AgentPion& attacker )
{
    pionsAttacking_.insert( &attacker );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_Fire::NotifyAttackedBy
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
inline
void DEC_KS_Fire::NotifyAttackedBy( MIL_Population& attacker )
{
    populationsAttacking_.insert( &attacker );
}

