// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Composantes/PHY_ComposanteState.inl $
// $Author: Nld $
// $Modtime: 11/03/05 11:04 $
// $Revision: 2 $
// $Workfile: PHY_ComposanteState.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::GetStates
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
PHY_ComposanteState::T_ComposanteStateVector& PHY_ComposanteState::GetStates()
{
    return composanteStates_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_ComposanteState::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::CanTransport
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteState::CanTransport() const
{
    return IsUsable() && *this != repairableWithEvacuation_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::IsUsable
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteState::IsUsable() const
{
    return bUsable_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::IsDamaged
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteState::IsDamaged() const
{
    return bDamaged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::GetID
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
uint PHY_ComposanteState::GetID() const
{
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::GetNbrStates
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
uint PHY_ComposanteState::GetNbrStates()
{
    return eNbrStates;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::Find
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteState& PHY_ComposanteState::Find( uint nID )
{
    assert( composanteStates_.size() > nID );
    return *composanteStates_[ nID ];
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::Find
// Created: NLD 2006-07-25
// -----------------------------------------------------------------------------
inline
const PHY_ComposanteState* PHY_ComposanteState::Find( const std::string& strName )
{
    for( CIT_ComposanteStateVector it = composanteStates_.begin(); it != composanteStates_.end(); ++it )
    {
        if( sCaseInsensitiveEqual()( (**it).GetName(), strName ) )
            return *it;
    }
    return 0;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::operator==
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteState::operator==( const PHY_ComposanteState& rhs ) const
{
    return nState_ == rhs.nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::operator!=
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteState::operator!=( const PHY_ComposanteState& rhs ) const
{
    return nState_ != rhs.nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_ComposanteState::operator<
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
bool PHY_ComposanteState::operator<( const PHY_ComposanteState& rhs ) const
{
    return nState_ < rhs.nState_;
}
