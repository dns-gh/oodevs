// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Circulation/MIL_AgentTypePionCIRCULATION.inl $
// $Author: Nld $
// $Modtime: 10/03/05 11:15 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionCIRCULATION.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCIRCULATION::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionCIRCULATION::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionCIRCULATION( strName, xis );
}
