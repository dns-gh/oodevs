// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureMobility.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:03 $
// $Revision: 2 $
// $Workfile: NatureMobility.inl $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: NatureMobility::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const NatureMobility* NatureMobility::Find( const std::string& strName )
{
    CIT_NatureMobilityMap it = natureMobility_.find( strName );
    return it == natureMobility_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: NatureMobility::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string* NatureMobility::Find( ASN1T_EnumUnitNatureMobility asnId )
{
    for( CIT_NatureMobilityMap it = natureMobility_.begin(); it != natureMobility_.end(); ++it )
        if( ( *it ).second->GetAsnID() == asnId )
            return &( *it ).second->GetName();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NatureMobility::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
const std::string& NatureMobility::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: NatureMobility::GetKey
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
inline
const std::string& NatureMobility::GetKey() const
{
    return strKey_;
}

// -----------------------------------------------------------------------------
// Name: NatureMobility::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
uint NatureMobility::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: NatureMobility::GetAsnID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
inline
ASN1T_EnumUnitNatureMobility NatureMobility::GetAsnID() const
{
    return nAsnID_;
}

} // end namespace TIC