//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_RealObject_ABC.inl $
// $Author: Nld $
// $Modtime: 11/05/05 18:05 $
// $Revision: 9 $
// $Workfile: MIL_RealObject_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::IsReal
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::IsReal() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetOccupier
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion* MIL_RealObject_ABC::GetOccupier() const
{
    return pOccupier_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetAvoidanceLocalisation
// Created: NLD 2003-10-10
// -----------------------------------------------------------------------------
inline
const TER_Localisation& MIL_RealObject_ABC::GetAvoidanceLocalisation() const
{
    return avoidanceLocalisation_;
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetConstructionPercentage
// Created: NLD 2003-05-20
//-----------------------------------------------------------------------------
inline
MT_Float MIL_RealObject_ABC::GetConstructionPercentage() const
{
    return rConstructionPercentage_;
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetMiningPercentage
// Created: NLD 2003-05-20
//-----------------------------------------------------------------------------
inline
MT_Float MIL_RealObject_ABC::GetMiningPercentage() const
{
    return rMiningPercentage_;
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetBypassPercentage
// Created: NLD 2003-05-20
//-----------------------------------------------------------------------------
inline
MT_Float MIL_RealObject_ABC::GetBypassPercentage() const
{
    return rBypassPercentage_;
}


//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::IsPrepared
// Created: AGN 03-07-29
//-----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::IsPrepared() const
{
    return bPrepared_;
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetType
// Created: NLD 2003-01-21
//-----------------------------------------------------------------------------
inline
const MIL_RealObjectType& MIL_RealObject_ABC::GetType() const
{
    assert( pType_ );
    return *pType_;
}
   
//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::IsMined
// Created: NLD 2003-03-13
//-----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::IsMined() const
{
    return rMiningPercentage_ > 0.;
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::IsBypassed
// Created: NLD 2003-03-13
//-----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::IsBypassed() const
{
    return rBypassPercentage_ > 0.;
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetID
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
inline
uint MIL_RealObject_ABC::GetID() const
{
    return nID_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& /*asnMsg*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::WriteSpecificAttributes
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::WriteSpecificAttributes( NET_ASN_MsgObjectUpdate&   /*asnMsg*/ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::NotifyAttributeUpdated
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::NotifyAttributeUpdated( E_AttributeUpdate nAttrToUpdate )
{
    xAttrToUpdate_ |= nAttrToUpdate;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetSizeCoef
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MT_Float MIL_RealObject_ABC::GetSizeCoef() const
{
    return rSizeCoef_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBePerceived
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::CanBePerceived() const
{
    return !IsMarkedForDestruction() && rConstructionPercentage_ > 0.;
}

// =============================================================================
// CONSTRUCTION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBeActivated
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::CanBeActivated() const
{
    return bPrepared_ && rConstructionPercentage_ > 0.;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBeConstructed
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::CanBeConstructed() const
{
    return !IsMarkedForDestruction() && !bPrepared_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBeDestroyed
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::CanBeDestroyed() const
{
    return !IsMarkedForDestruction();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetDotationNeededForConstruction
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
uint MIL_RealObject_ABC::GetDotationNeededForConstruction( MT_Float rDeltaPercentage ) const
{
    const MT_Float rNewPercentage = std::max( 0., std::min( 1., rConstructionPercentage_ + rDeltaPercentage ) );
    return (uint)( rNewPercentage * nFullNbrDotationForConstruction_ ) - nCurrentNbrDotationForConstruction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetDotationRecoveredWhenDestroying
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
inline
uint MIL_RealObject_ABC::GetDotationRecoveredWhenDestroying( MT_Float rDeltaPercentage ) const
{
    const MT_Float rNewPercentage = std::max( 0., std::min( 1., rConstructionPercentage_ - rDeltaPercentage ) );
    return nCurrentNbrDotationForConstruction_ - (uint)( rNewPercentage * nFullNbrDotationForConstruction_ );
}

//-----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::ChangeConstructionPercentage
// Created: NLD 2003-05-20
//-----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::ChangeConstructionPercentage( MT_Float rNewConstructionPercentage )
{
    if( IsMarkedForDestruction() )
        return;

    const MT_Float rDeltaPercentage = rNewConstructionPercentage - rConstructionPercentage_;
    if( rDeltaPercentage == 0. )
        return;
    if( rDeltaPercentage > 0. )
        Construct( rDeltaPercentage );
    else
    {
        rMiningPercentage_ = 0.;
        Destroy( -rDeltaPercentage );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::ChangeMiningPercentage
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::ChangeMiningPercentage( MT_Float rNewMiningPercentage )
{
    if( IsMarkedForDestruction() )
        return;

    const MT_Float rDeltaPercentage = rNewMiningPercentage - rMiningPercentage_;
    if( rDeltaPercentage == 0. )
        return;
    if( rDeltaPercentage > 0. )
        Mine( rDeltaPercentage );
    else
    {
        rMiningPercentage_ = 0;
        Demine( -rDeltaPercentage );
    }
}

// =============================================================================
// OCCUPATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::AddOccupier
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::AddOccupier( const MIL_AgentPion& agent )
{
    if( !pOccupier_ )
        pOccupier_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::RemoveOccupier
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::RemoveOccupier( const MIL_AgentPion& agent )
{
    if( pOccupier_ == &agent )
        pOccupier_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::CanBeOccupiedBy
// Created: NLD 2004-11-26
// -----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::CanBeOccupiedBy( const MIL_AgentPion& agent ) const
{
    return !pOccupier_ || pOccupier_ == &agent;
}

// =============================================================================
// ANIMATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::AddAnimator
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
inline
bool MIL_RealObject_ABC::AddAnimator( const MIL_AgentPion& agent )
{
    if( !CanBeAnimatedBy( agent ) )
        return false;

    animators_.insert( &agent );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::RemoveAnimator
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
inline
void MIL_RealObject_ABC::RemoveAnimator( const MIL_AgentPion& agent )
{
    animators_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObject_ABC::GetAnimators
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
inline
const MIL_RealObject_ABC::T_AgentSet& MIL_RealObject_ABC::GetAnimators() const
{
    return animators_;
}
