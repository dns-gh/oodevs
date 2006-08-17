// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/MIL_AgentPionLOG_ABC.inl $
// $Author: Nld $
// $Modtime: 27/12/04 15:19 $
// $Revision: 1 $
// $Workfile: MIL_AgentPionLOG_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::GetAutomate
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
const MIL_AutomateLOG& MIL_AgentPionLOG_ABC::GetAutomate() const
{
    return static_cast< const MIL_AutomateLOG& >( MIL_AgentPion::GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::GetAutomate
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG& MIL_AgentPionLOG_ABC::GetAutomate()
{
    return static_cast< MIL_AutomateLOG& >( MIL_AgentPion::GetAutomate() );
}
