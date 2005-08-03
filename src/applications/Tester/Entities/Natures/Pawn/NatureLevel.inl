// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureLevel.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:03 $
// $Revision: 2 $
// $Workfile: NatureLevel.inl $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: NatureLevel::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const NatureLevel* NatureLevel::Find( const std::string& strName )
{
    CIT_NatureLevelMap it = natureLevel_.find( strName );
    return it == natureLevel_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: NatureLevel::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string* NatureLevel::Find( ASN1T_EnumNatureLevel asnId )
{
    for( CIT_NatureLevelMap it = natureLevel_.begin(); it != natureLevel_.end(); ++it )
        if( ( *it ).second->GetAsnID() == asnId )
            return &( *it ).second->GetName();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NatureLevel::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& NatureLevel::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: NatureLevel::GetKey
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
inline
const std::string& NatureLevel::GetKey() const
{
    return strKey_;
}

// -----------------------------------------------------------------------------
// Name: NatureLevel::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint NatureLevel::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: NatureLevel::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumNatureLevel NatureLevel::GetAsnID() const
{
    return nAsnID_;
}

} // end namespace TIC