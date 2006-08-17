// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Convoy/MIL_AgentTypePionLOGConvoy.inl $
// $Author: Nld $
// $Modtime: 8/02/05 11:26 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionLOGConvoy.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionLOGConvoy::Create( const std::string& strName, MIL_InputArchive& archive )
{
    return new MIL_AgentTypePionLOGConvoy( strName, archive );
}
