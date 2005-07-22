// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureQualifier.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:03 $
// $Revision: 2 $
// $Workfile: NatureQualifier.inl $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: NatureQualifier::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const NatureQualifier* NatureQualifier::Find( const std::string& strName )
{
    CIT_NatureQualifierMap it = natureQualifier_.find( strName );
    return it == natureQualifier_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: NatureQualifier::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string* NatureQualifier::Find( ASN1T_EnumUnitNatureQualifier asnId )
{
    for( CIT_NatureQualifierMap it = natureQualifier_.begin(); it != natureQualifier_.end(); ++it )
        if( ( *it ).second->GetAsnID() == asnId )
            return &( *it ).second->GetName();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NatureQualifier::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& NatureQualifier::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: NatureQualifier::GetKey
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
inline
const std::string& NatureQualifier::GetKey() const
{
    return strKey_;
}

// -----------------------------------------------------------------------------
// Name: NatureQualifier::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint NatureQualifier::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: NatureQualifier::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureQualifier NatureQualifier::GetAsnID() const
{
    return nAsnID_;
}

} // end namespace TIC