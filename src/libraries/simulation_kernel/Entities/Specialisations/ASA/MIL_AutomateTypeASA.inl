// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/ASA/MIL_AutomateTypeASA.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_AutomateTypeASA.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeASA::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
inline
const MIL_AutomateType* MIL_AutomateTypeASA::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateTypeASA( strName, xis );
}
