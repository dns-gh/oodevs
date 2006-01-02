// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/ASY/MIL_AgentTypePionASY.inl $
// $Author: Nld $
// $Modtime: 10/03/05 16:05 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionASY.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionASY::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionASY::Create( const std::string& strName, MIL_InputArchive& archive )
{
    return new MIL_AgentTypePionASY( strName, archive );
}
