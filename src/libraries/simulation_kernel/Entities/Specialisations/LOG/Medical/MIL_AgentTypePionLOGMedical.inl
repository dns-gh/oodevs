// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Medical/MIL_AgentTypePionLOGMedical.inl $
// $Author: Nld $
// $Modtime: 14/01/05 16:41 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionLOGMedical.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionLOGMedical::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGMedical( strName, xis );
}
