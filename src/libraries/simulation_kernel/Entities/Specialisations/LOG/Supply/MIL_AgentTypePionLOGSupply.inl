// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/Supply/MIL_AgentTypePionLOGSupply.inl $
// $Author: Nld $
// $Modtime: 21/01/05 10:22 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionLOGSupply.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion* MIL_AgentTypePionLOGSupply::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionLOGSupply( strName, xis );
}
