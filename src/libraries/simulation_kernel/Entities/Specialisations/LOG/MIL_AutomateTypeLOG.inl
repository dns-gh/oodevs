// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Log/MIL_AutomateTypeLOG.inl $
// $Author: Nld $
// $Modtime: 27/12/04 16:11 $
// $Revision: 1 $
// $Workfile: MIL_AutomateTypeLOG.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::Create
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
const MIL_AutomateType* MIL_AutomateTypeLOG::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateTypeLOG( strName, xis );
}
