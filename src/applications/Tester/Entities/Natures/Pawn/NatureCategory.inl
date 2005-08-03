// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureCategory.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:02 $
// $Revision: 2 $
// $Workfile: NatureCategory.inl $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: NatureCategory::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const NatureCategory* NatureCategory::Find( const std::string& strName )
{
    CIT_NatureCategoryMap it = natureCategory_.find( strName );
    return it == natureCategory_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: NatureCategory::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string* NatureCategory::Find( ASN1T_EnumUnitNatureCategory asnId )
{
    for( CIT_NatureCategoryMap it = natureCategory_.begin(); it != natureCategory_.end(); ++it )
        if( ( *it ).second->GetAsnID() == asnId )
            return &( *it ).second->GetName();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NatureCategory::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& NatureCategory::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: NatureCategory::GetKey
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
inline
const std::string& NatureCategory::GetKey() const
{
    return strKey_;
}

// -----------------------------------------------------------------------------
// Name: NatureCategory::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint NatureCategory::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: NatureCategory::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureCategory NatureCategory::GetAsnID() const
{
    return nAsnID_;
}

} // end namespace TIC