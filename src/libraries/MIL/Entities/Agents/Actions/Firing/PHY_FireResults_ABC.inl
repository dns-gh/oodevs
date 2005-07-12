// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/PHY_FireResults_ABC.inl $
// $Author: Nld $
// $Modtime: 28/10/04 9:43 $
// $Revision: 1 $
// $Workfile: PHY_FireResults_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetAgentFireResult
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
PHY_AgentFireResult& PHY_FireResults_ABC::GetAgentFireResult( const MIL_Agent_ABC& target )
{
    return results_[ &target ];
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::IncRef
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
void PHY_FireResults_ABC::IncRef()
{
    ++nNbrRefs_;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::DecRef
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
inline
void PHY_FireResults_ABC::DecRef()
{
    assert( nNbrRefs_ > 0 );
    --nNbrRefs_;
    if( nNbrRefs_ == 0 )
        delete this;
}