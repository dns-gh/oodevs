// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Refugie/MIL_AutomateTypeREFUGIE.inl $
// $Author: Nld $
// $Modtime: 10/03/05 16:07 $
// $Revision: 1 $
// $Workfile: MIL_AutomateTypeREFUGIE.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeREFUGIE::Create
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
const MIL_AutomateType* MIL_AutomateTypeREFUGIE::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateTypeREFUGIE( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeREFUGIE::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
bool MIL_AutomateTypeREFUGIE::IsRefugee() const
{
    return true;
}
