// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureSpecialization.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:04 $
// $Revision: 2 $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: NatureSpecialization::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const NatureSpecialization* NatureSpecialization::Find( const std::string& strName )
{
    CIT_NatureSpecializationMap it = natureSpecialization_.find( strName );
    return it == natureSpecialization_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: NatureSpecialization::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string* NatureSpecialization::Find( ASN1T_EnumUnitNatureSpecialization asnId )
{
    for( CIT_NatureSpecializationMap it = natureSpecialization_.begin(); it != natureSpecialization_.end(); ++it )
        if( ( *it ).second->GetAsnID() == asnId )
            return &( *it ).second->GetName();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NatureSpecialization::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& NatureSpecialization::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: NatureSpecialization::GetKey
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
inline
const std::string& NatureSpecialization::GetKey() const
{
    return strKey_;
}

// -----------------------------------------------------------------------------
// Name: NatureSpecialization::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint NatureSpecialization::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: NatureSpecialization::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureSpecialization NatureSpecialization::GetAsnID() const
{
    return nAsnID_;
}

} // end namespace TIC