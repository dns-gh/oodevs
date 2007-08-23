// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Tc2/MIL_AgentTypePionLOGTC2.inl $
// $Author: Nld $
// $Modtime: 21/12/04 11:31 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionLOGTC2.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionLOGTC2::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGTC2( strName, xis );
}
