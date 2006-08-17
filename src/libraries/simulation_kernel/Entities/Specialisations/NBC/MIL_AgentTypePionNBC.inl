// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Nbc/MIL_AgentTypePionNBC.inl $
// $Author: Nld $
// $Modtime: 29/10/04 9:46 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionNBC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionNBC::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionNBC::Create( const std::string& strName, MIL_InputArchive& archive )
{
    return new MIL_AgentTypePionNBC( strName, archive );
}
