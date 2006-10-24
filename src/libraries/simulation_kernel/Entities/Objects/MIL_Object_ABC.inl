//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_Object_ABC.inl $
// $Author: Nld $
// $Modtime: 26/11/04 17:15 $
// $Revision: 3 $
// $Workfile: MIL_Object_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsMarkedForDestruction
// Created: NLD 2003-10-10
// -----------------------------------------------------------------------------
inline
bool MIL_Object_ABC::IsMarkedForDestruction() const
{
    return bMarkedForDestruction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsReadyForDeletion
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
bool MIL_Object_ABC::IsReadyForDeletion() const
{
    return bReadyForDeletion_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::ProcessAgentEntering
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
inline
void MIL_Object_ABC::ProcessAgentEntering( MIL_Agent_ABC& /*agent*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::ProcessAgentExiting
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
inline
void MIL_Object_ABC::ProcessAgentExiting( MIL_Agent_ABC& /*agent*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::ProcessAgentMovingInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
inline
void MIL_Object_ABC::ProcessAgentMovingInside( MIL_Agent_ABC& /*agent*/ )
{
    // NOTHING   
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::ProcessAgentInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
inline
void MIL_Object_ABC::ProcessAgentInside( MIL_Agent_ABC& /*agent*/ )
{
    // NOTHING    
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MIL_Army& MIL_Object_ABC::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
MIL_Army& MIL_Object_ABC::GetArmy()
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetAgentsInside
// Created: NLD 2004-10-27
// -----------------------------------------------------------------------------
inline
const MIL_Object_ABC::T_AgentSet& MIL_Object_ABC::GetAgentsInside() const
{
    return agentInsideSet_;
}
